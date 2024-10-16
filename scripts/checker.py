from pathlib import Path
import asyncio
import sys

msg_run = "RUN  "
msg_ok = "OK    "
msg_failed = "FAILED"


def get_tests(tests_dir: Path) -> list[str]:
    return list(
        sorted(map(lambda p: p.stem, tests_dir.glob("./*.in")))
    )


async def run_all_tests(tests_dir: Path, calculator: Path) -> int:
    tests: list[str] = get_tests(tests_dir)
    failed_tests = 0
    for test in tests:
        notation = Path(tests_dir, f"{test}.notation").read_text().strip()
        cmd = f"{str(calculator)} {notation}"
        in_expression = Path(tests_dir, f"{test}.in").read_text()
        rc_expected = Path(tests_dir, f"{test}.rc").read_text().strip()
        out_expected = Path(tests_dir, f"{test}.out").read_text().strip()
        rc_actual, out_actual, err_actual = await run_test(test, cmd, in_expression)
        if not validate_test(
            test, int(rc_expected), rc_actual, out_expected, out_actual, err_actual
        ):
            failed_tests += 1
    return failed_tests


async def run_test(
    test_name: str,
    cmd: str,
    in_expression: str,
) -> tuple[int, str, str]:
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdin=asyncio.subprocess.PIPE,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
    )

    stdout, stderr = await proc.communicate(input=bytes(in_expression, "ascii"))
    rc = proc.returncode
    out_text = stdout.decode("ascii", "replace").strip()
    err_text = stderr.decode("ascii", "replace").strip()
    return rc, out_text, err_text


def validate_test(
    test_name: str,
    rc_expected: int,
    rc_actual: int,
    out_expected: str,
    out_actual: str,
    err_actual: str,
) -> bool:
    if rc_expected != rc_actual:
        print(f"{msg_failed} {test_name}: Return code {rc_expected} != {rc_actual}")
        return False
    if rc_expected != 0 and not err_actual:
        print(f"{msg_failed} {test_name}: stderr is empty in incorrect test")
        return False
    if rc_expected == 0 and err_actual:
        print(f"{msg_failed} {test_name}: stderr is not empty in correct test")
        return False
    if rc_expected == 0 and int(out_expected) != int(out_actual):
        print(f"{msg_failed} {test_name}: Result {out_expected} != {out_actual}")
        return False
    print(f"{msg_ok} {test_name}")
    return True


def main() -> int:
    calculator = Path(sys.argv[1])
    tests_dir = Path(sys.argv[2])
    failed_tests: int = asyncio.run(run_all_tests(tests_dir, calculator))
    if failed_tests > 0:
        print(f"{failed_tests} tests FAILED")
        return 1
    print(f"All tests PASSED")
    return 0


if __name__ == "__main__":
    rc = main()
    sys.exit(rc)

#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")
TESTDIR=$(realpath "$ROOTDIR/test/basic")
CALCULATOR=$(realpath "$ROOTDIR/build/src/calculator")
CHECKER="$BASEDIR/checker.py"
CORRECT="correct"
INCORRECT="incorrect"

echo "Test basic incorrect tests"
python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${INCORRECT}"

echo "Test basic correct tests"
python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${CORRECT}"

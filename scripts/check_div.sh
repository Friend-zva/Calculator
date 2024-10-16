#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")
TESTDIR=$(realpath "$ROOTDIR/test/support")
CALCULATOR=$(realpath "$ROOTDIR/build/src/calculator")
CHECKER="$BASEDIR/checker.py"

SUPPORTED="div_supported"
UNSUPPORTED_CORRECT="div_unsupported_correct"

if python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${SUPPORTED}" > /dev/null; then
	echo "supported=yes"
	exit 0
else
	if python3 "$CHECKER" "$CALCULATOR" "${TESTDIR}/${UNSUPPORTED_CORRECT}" > /dev/null; then
		echo "supported=no"
		exit 0
	else
		exit 1
	fi
fi

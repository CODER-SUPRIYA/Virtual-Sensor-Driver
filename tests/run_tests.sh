#!/bin/bash
# Run from the repo root: bash tests/run_tests.sh
cd "$(dirname "$0")/.." || exit 1
pass=0; fail=0
check() {
    if eval "$2" >/dev/null 2>&1; then echo "PASS: $1"; pass=$((pass+1))
    else echo "FAIL: $1"; fail=$((fail+1)); fi
}

sudo rmmod vsensor 2>/dev/null
check "driver builds"            "make -C driver && test -f driver/vsensor.ko"
check "app builds"               "make -C app && test -x app/vsensor_app"
check "module loads"             "sudo insmod driver/vsensor.ko"
check "/dev/vsensor0 exists"     "test -c /dev/vsensor0"
check "/proc/vsensor exists"     "test -f /proc/vsensor"
check "read returns 16 bytes"    "[ \$(head -c 16 /dev/vsensor0 | wc -c) -eq 16 ]"

s1=$(awk '/samples/{print $2}' /proc/vsensor); sleep 2
s2=$(awk '/samples/{print $2}' /proc/vsensor)
check "timer keeps sampling"     "[ $s2 -gt $s1 ]"

rm -f app/vsensor.log
(cd app && timeout -s INT 3 ./vsensor_app 0 -100000 200 > /tmp/app_out.txt)
check "app writes log"           "test -s app/vsensor.log"
check "HIGH alert raised"        "grep -q 'ALERT. HIGH' /tmp/app_out.txt"

(cd app && timeout -s INT 4 valgrind --leak-check=full ./vsensor_app 0 -100000 200 > /tmp/vg_out.txt 2>&1)
check "app has no leaks (valgrind)" "grep -q 'no leaks are possible' /tmp/vg_out.txt && grep -q '0 errors from 0 contexts' /tmp/vg_out.txt"

check "module unloads"           "sudo rmmod vsensor"
check "device removed"           "! test -e /dev/vsensor0"

echo "Passed: $pass  Failed: $fail"
[ $fail -eq 0 ]

#!/bin/bash

# color outputs for better visual
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo
# Trigger all your test cases with this script
gcc --coverage vm_riskxvii.c -o vm_riskxvii
for TEST in $(find tests -iregex '.*\.in' | xargs basename -s .in); do
    echo ""
    IN=tests/$TEST.in
    OUT=tests/$TEST.out
    diff $OUT <(./vr2017 $(<$IN)) && echo -e "testing $TEST: ${GREEN}SUCCESS!${NC}" || echo -e "testing $TEST: ${RED}FAILURE.${NC}"
done
gcov vm_riskxvii.c
echo
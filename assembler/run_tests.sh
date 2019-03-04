#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[1;37m' # No Color


tests="a b c d \
	e f g h i j k k1"

for f in $tests; do
	echo -ne ${NC}"test  $f \t\t\t"
	./assembler tests/$f > tests/$f.log
	cmp -s tests/$f.log tests/expected/$f.log
	if [ $? -eq 0 ]; then
		echo -e ${GREEN}passed${NC}
	else
		echo -e ${RED}failed${NC}
	fi
done

#!/bin/bash
# This first test program aims to compare the output of the GNU nm with our ft_nm on the same simple program

set -e

echo "int  main() { return 42; }" > test.c
cc test.c
nm > nm_output
ft_nm > ft_nm_output
if ! [ 'diff nm_output ft_nm_output' ]; then
  echo "Test succeeded"
else
  echo "Test failed"
fi

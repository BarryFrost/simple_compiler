#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./chibicc "$input" > tmp.s || exit
  gcc -static -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" == "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 21 '5+20-4'
assert 41 ' 12 + 34 -  5 '
assert 12 '12 + 0 + 0'
assert 27 ' 31 + 6 - 10'
assert 47 '5+6*7'
assert 15 '(9 - 6)*5'
assert 4  '(15 -7)/2'
assert -1   '101 + 102 * (103 - 104)'
echo OK

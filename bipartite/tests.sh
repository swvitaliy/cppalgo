#!/bin/bash

make kuhn

for i in {01..31}; do
  ./kuhn_ <"tests/${i}" >"tests/${i}.b"
done

for i in {01..31}; do
  a=$(cat "tests/${i}.a")
  b=$(cat "tests/${i}.b")
  if [[ "$a" == "$b" ]]; then
    echo "${i} - OK"
  else
    echo "${i} - FAILED"
  fi
done

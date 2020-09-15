#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Abekat" > test_files/ascii3.input
printf "	 \n" > test_files/ascii4.input
printf "Hvem hvad hvad stolper taenker?" > test_files/ascii5.input
printf "Fireworks, ikke med Katy Perry" > test_files/ascii6.input
printf "abcdefghijklmnopqrstuvwxyz" > test_files/ascii7.input
printf "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > test_files/ascii8.input
printf "Badeand: den bedste debugger" > test_files/ascii9.input
printf "Gaa hjem og vug, GDB!" > test_files/ascii10.input
printf "Sunpeaks" > test_files/ascii11.input
printf "2 + 2 = 5" > test_files/ascii12.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "\x00Hello World!\n" > test_files/data2.input
printf "Hello World!\n\x00" > test_files/data3.input
printf "Måne\x00" > test_files/data4.input
printf "Warum\x00" > test_files/data5.input
printf "\x00Farum" > test_files/data6.input
printf "Så er der suppeurter\x00\x00Supperurtsuppeurtsuppeurt! \x00" > test_files/data7.input
printf "In the jungle,\x00 the mighty jungle,\x00 the lion sleeps tonight\x00" > test_files/data8.input
printf "\x00Jeg er så småt ed at løbe tør for data test cases" > test_files/data9.input
printf "12\x00cases\x00\x00virker\x00\x00\x00lidt\x00\x00\x00\x00meget" > test_files/data10.input
printf "Min forståelse for hvornår file(1) melder \"data\" er utilstrækkelig til at lave 12 forkellige meningsfulde tests\x00\n" > test_files/data11.input
printf "\x00! Længe leve \x00!\n" > test_files/data12.input
printf "" > test_files/empty.input


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    "${f}" | sed 's/ASCII text.*/ASCII text/' > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode

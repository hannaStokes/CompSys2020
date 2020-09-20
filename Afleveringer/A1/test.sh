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

printf "® står for registered trademark sign" > test_files/ISO.input
printf "x®®®®®®®®®®®®®®®®®®" > test_files/ISO2.input
printf "Tegnet kan også stå til sidst®" > test_files/ISO3.input
printf "Dette tegn er også et ISO-8859 tegn: ÆÆÆÆÆ" > test_files/ISO4.input
printf "Det lyder rigtig irriterende når man siger: ÆÆÆÆÆÆÆÆÆÆÆÆÆ" > test_files/ISO5.input
printf "Eller ØØØØØØØØØØØØØÅÅÅÅÅÅÅÅÅ" > test_files/ISO6.input
printf "ISO-8859 indeholder også 'fractions' som eksempelvis ½" > test_files/ISO7.input
printf "Dette '×' betyder gange og dette 'x' betyder x" > test_files/ISO8.input
printf "Hvilket betyder at ½×½=¼" > test_files/ISO9.input
printf "¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼¼=1/4^22" > test_files/ISO10.input
printf "Ç bruges eksempelvis i det franske sprog" > test_files/ISO11.input
printf "ß bruges i det tyske" > test_files/ISO12.input

printf "Længe leve Ȱ" > test_files/UTF8.input
printf "ĳ er faktisk et tegn for sig selv der består af i og j" > test_files/UTF82.input
printf "Ĵ har en hat på ligesom HC Andersen" > test_files/UTF83.input
printf "Dette ĹĺĻļĽľĿŀŁł er 10 forskellige L'er" > test_files/UTF84.input
printf "Dette er test Ŋummer 5 for UTF8, men det kan godt ligne at det er test nummer 85" > test_files/UTF85.input
printf "Dette er ikke et normalt J: Ј" > test_files/UTF86.input
printf "ϵ og δ bruges i matematik" > test_files/UTF87.input
printf "ʬʬʬ.google.com findes ikke" > test_files/UTF88.input
printf "Dette ˸ er længere oppe end :" > test_files/UTF89.input
printf "ȦȨɨ der fløg en lille bi" > test_files/UTF810.input
printf "Hvorfor skulle man nogensinde bruge ʁ til andet end at teste UTF8?" > test_files/UTF811.input
printf "ʁʁʁʁʁʁʁʁʁʁʁʁʁʁʁ" > test_files/UTF812.input

printf "\xFF\xFE 256 er et højt tal" > test_files/LittleEndian.input
printf "\xFF\xFE MAMA MIA!" > test_files/LittleEndian2.input
printf "\xFF\xFE PIZZARIA!" > test_files/LittleEndian3.input
printf "\xFF\xFE Skal vi ikke bare se e film?" > test_files/LittleEndian4.input
printf "\xFF\xFE ÆØÅ er meget dansk" > test_files/LittleEndian5.input
printf "\xFF\xFE Både Ø og Å har en betydning" > test_files/LittleEndian6.input
printf "\xFF\xFE Æ har ikke nogen betydning" > test_files/LittleEndian7.input
printf "\xFF\xFE Man kan også dividere med ÷ tegnet når man bruger ISO" > test_files/LittleEndian8.input
printf "\xFF\xFE ËŁËƑĄŃŤ" > test_files/LittleEndian9.input
printf "\xFF\xFE Dette er et lille bogstav for HV på latin: ƕ" > test_files/LittleEndian10.input
printf "\xFF\xFE Man lærer noget nyt hver dƛg" > test_files/LittleEndian11.input
printf "\xFF\xFEʁʁʁʁʁʁʁʁʁʁʁʁʁʁʁ" > test_files/LittleEndian12.input

printf "\xFE\xFF 256 er et højt tal" > test_files/BigEndian.input
printf "\xFE\xFF MAMA MIA!" > test_files/BigEndian2.input
printf "\xFE\xFF PIZZARIA!" > test_files/BigEndian3.input
printf "\xFE\xFF Skal vi ikke bare se e film?" > test_files/BigEndian4.input
printf "\xFE\xFF ÆØÅ er meget dansk" > test_files/BigEndian5.input
printf "\xFE\xFF Både Ø og Å har en betydning" > test_files/BigEndian6.input
printf "\xFE\xFF Æ har ikke nogen betydning" > test_files/BigEndian7.input
printf "\xFE\xFF Man kan også dividere med ÷ tegnet når man bruger ISO" > test_files/BigEndian8.input
printf "\xFE\xFF ËŁËƑĄŃŤ" > test_files/BigEndian9.input
printf "\xFE\xFF Dette er et lille bogstav for HV på latin: ƕ" > test_files/BigEndian10.input
printf "\xFE\xFF Man lærer noget nyt hver dƛg" > test_files/BigEndian11.input
printf "\xFE\xFFʁʁʁʁʁʁʁʁʁʁʁʁʁʁʁ" > test_files/BigEndian12.input

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

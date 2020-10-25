make

# The command with which you run PRASM: You should likely change this variable
PRASM="python3 prasm.py"

TESTLOC=tests
TESTDIR=test_runs

if [[ ! -d "${TESTLOC}" ]] ; then
  echo "Cannot find the test location ${TESTLOC}"
  exit
fi

echo ">> I will now test all *.prime files in ${TESTLOC}/"

echo "Generating a test_runs directory.."
mkdir -p $TESTDIR
rm -f $TESTDIR/*

echo "Running the tests.."
exitcode=0

for f in tests/*.prime; do
  filename=${f#"$TESTLOC/"}
  fname=${filename%.prime}
  echo ""
  echo ">>> Testing ${filename}.."

  # Generate hex file
  hexfile=$TESTDIR/$fname.hex
  $PRASM $f
  mv $TESTLOC/$fname.hex $TESTDIR/
  ./sim $hexfile 0x0

done


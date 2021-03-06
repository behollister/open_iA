TEST_SRC_DIR=$1
TEST_BIN_DIR=$2
CONFIG_FILE=$3
CTEST_MODE=Nightly
if [ -n "$4" ];
then
	CTEST_MODE=$4
	echo "Using $CTEST_MODE CTest mode!"
fi
COMPILER=GCC
if [ -n "$5" ];
then
	COMPILER=$5
	echo "Compiler=$COMPILER!"
fi
TEST_FILES_DIR=$TEST_SRC_DIR/Test_files
if [ -n "$6" ];
then
	TEST_FILES_DIR=$6
fi
MODULE_DIRS=$TEST_SRC_DIR/modules
if [ -n "$7" ];
then
	MODULE_DIRS=$7
fi

TEST_CONFIG_DIR=$(mktemp --tmpdir=/tmp -d ctestconfigs.XXXXXXXXXX)

cd $TEST_SRC_DIR
GIT_BRANCH=$(git symbolic-ref --short HEAD)
# git pull origin master # done automatically by CTest!

# just in case it doesn't exist yet, create output directory:
mkdir -p $TEST_BIN_DIR
cd $TEST_BIN_DIR

# Set up basic build environment
make clean
cmake -C $CONFIG_FILE $TEST_SRC_DIR 2>&1

# Create test configurations:
mkdir -p $TEST_CONFIG_DIR
echo $MODULE_DIRS
python $TEST_FILES_DIR/CreateTestConfigurations.py $TEST_SRC_DIR $GIT_BRANCH $TEST_CONFIG_DIR $MODULE_DIRS $COMPILER

# Run with all flags enabled:
cmake -C $TEST_CONFIG_DIR/all_flags.cmake $TEST_SRC_DIR 2>&1
make clean
ctest -D $CTEST_MODE

# Run with no flags enabled:
cmake -C $TEST_CONFIG_DIR/no_flags.cmake $TEST_SRC_DIR 2>&1
make clean
ctest -D Experimental

# iterate over module tests, run build&tests for each:
for modulefile in $TEST_CONFIG_DIR/Module_*.cmake
do
	echo
	echo "--------------------------------------------------------------------------------"
	echo `basename $modulefile .cmake`
	cmake -C ${TEST_CONFIG_DIR}/no_flags.cmake $TEST_SRC_DIR 2>&1
	cmake -C $modulefile $TEST_SRC_DIR 2>&1
	make clean
	ctest -D Experimental
done

# CLEANUP:
# remove test configurations:
rm -r $TEST_CONFIG_DIR
rm -r $TEST_BIN_DIR

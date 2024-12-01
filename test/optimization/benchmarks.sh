#!/bin/bash

# Exit script on error
set -e

# Variables
BUILD_SCRIPT="../../bin/build.sh"
PROGRAM="./loopUnrolling"
LU_FILE="loopUnrolling.tip"
JT_FILE="jumpThreading.tip"
INTOP_FILE="functionMerge.tip"
SCCP_FILE="constantVariables.tip"

NUM_RUNS=10


# Function to execute the program multiple times and collect runtimes
run_and_collect_times() {
    local label="$1"
    local times_ref=$2  # Use nameref to store runtimes in the provided array
    local total=0
    local x=$( (time -p $PROGRAM) 2>&1 | awk '/^real/ {print $2}')  # running once because first run is usually much slower than rest
    echo "Running $label..."
    for ((i = 1; i <= NUM_RUNS; i++)); do
        local runtime
        runtime=$( (time -p $PROGRAM) 2>&1 | awk '/^real/ {print $2}')
        #echo "Run $i: $runtime seconds"
        total=$total+$runtime
        times_ref+=("$runtime")
    done
    echo "Total runtime for 10 runs:"
    bc <<< "scale=3; $total"
}

cd ../../build
make -j 32
cd ../test/optimization

# Run the second build command
echo "Building $LU_FILE without extra optimizations"
$BUILD_SCRIPT $LU_FILE

# Collect runtimes for the second build
run_and_collect_times "loopUnrolling without unrolling"
echo ""

# Run the first build command
echo "Building with -lu flag..."
$BUILD_SCRIPT -lu $LU_FILE

# Collect runtimes for the first build
run_and_collect_times "loopUnrolling with -lu"
echo ""
# Run the first build command
echo "Building with -lu and -lscfg flag..."
$BUILD_SCRIPT -lu -lscfg $LU_FILE

# Collect runtimes for the first build
run_and_collect_times "loopUnrolling with -lu and -lcsfg"
echo ""

rm loopUnrolling
rm loopUnrolling.tip.bc

PROGRAM="./jumpThreading" # whatever the new program has to be

# Run the second build command
echo "Building $JT_FILE without extra optimizations"
$BUILD_SCRIPT $JT_FILE

# Collect runtimes for the second build
run_and_collect_times "jumpThreading without jump threading"
echo ""

# Run the first build command
echo "Building with -jt flag..."
$BUILD_SCRIPT -jt $JT_FILE

# Collect runtimes for the first build
run_and_collect_times "jumpThreading with -jt"
echo ""

rm jumpThreading
rm jumpThreading.tip.bc

PROGRAM="./functionMerge" # whatever the new program has to be

# Run the second build command
echo "Building $INTOP_FILE without extra optimizations"
$BUILD_SCRIPT $INTOP_FILE

# Collect runtimes for the second build
run_and_collect_times "running file without interprocedural optimization"
echo ""

# Run the first build command
echo "Building with -intop flag..."
$BUILD_SCRIPT -intop $INTOP_FILE

# Collect runtimes for the first build
run_and_collect_times "Optimizing with -intop"
echo ""

rm functionMerge
rm functionMerge.tip.bc

PROGRAM="./constantVariables" # whatever the new program has to be

# Run the second build command
echo "Building $SCCP_FILE without extra optimizations"
$BUILD_SCRIPT $SCCP_FILE

# Collect runtimes for the second build
run_and_collect_times "running file without SCCP"
echo ""

# Run the first build command
echo "Building with -sccp flag..."
$BUILD_SCRIPT -sccp $SCCP_FILE

# Collect runtimes for the first build
run_and_collect_times "Optimizing with -sccp"
echo ""

rm constantVariables
rm constantVariables.tip.bc
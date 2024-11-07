#!/bin/bash
echo "This is creating a build folder. Confirm that you need to make a build folder"
read -p "Continue? (Y/N): " confirm && [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]] || exit 1
echo "creating build folder"
. ~/.bashrc
mkdir build
cd build
echo "running cmake"
cmake ..
echo "running make"
make -j 32
echo "ran"

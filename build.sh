#!/bin/bash

# Create the build directory if it doesn't exist
mkdir -p build

# Navigate into the build directory
cd build

# Run cmake to configure the project
cmake ..

# Build the project using make
make

# Run the program if build is successful
if [ $? -eq 0 ]; then
  echo "Running the program..."
  ./zapoctovy_program
else
  echo "Build failed. Please check for errors."
  exit 1
fi

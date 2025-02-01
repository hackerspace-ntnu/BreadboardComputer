#!/bin/bash

# Clone the repository
git clone https://github.com/Davidspilde/assemblerAtHome.git

# Navigate into the project directory
cd my-assembler

# Build the project using make
make

# Install the binary
sudo make install

# Cleanup
make clean

echo "Installation complete. You can run the assembler"

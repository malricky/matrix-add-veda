#!/bin/bash

# Set error handling
set -e

# Define paths
VEDA_INCLUDE_DIR="/opt/nec/ve/share/veda/include"
VEDA_LIB_DIR="/opt/nec/ve/veos/lib64"
NCC_BIN="/opt/nec/ve/bin/ncc-5.2.0"
GCC_BIN="gcc"

# Define source and output files
HOST_SRC="host.c"
DEVICE_SRC="device.vc"
DEVICE_OBJ="device.o"
SHARED_LIB="lib_device.vso"
HOST_EXE="host"

# Compile the host executable
echo "Compiling host executable..."
$GCC_BIN -o $HOST_EXE $HOST_SRC -I$VEDA_INCLUDE_DIR -L$VEDA_LIB_DIR -rdynamic -Wl,-rpath,$VEDA_LIB_DIR -lveda -lm

# Compile the device object file
echo "Compiling device object file..."
$NCC_BIN -x c -fpic -I$VEDA_INCLUDE_DIR -o $DEVICE_OBJ -c $DEVICE_SRC

# Create the shared library from the device object file
echo "Creating shared library from device object file..."
$NCC_BIN -shared -o $SHARED_LIB $DEVICE_OBJ

echo "Build completed successfully."


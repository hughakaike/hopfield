#!/bin/bash

INPUT_DIR="input"
SCRIPT_DIR="scr"

python $SCRIPT_DIR/convert2matrix.py $INPUT_DIR
./main.exe $INPUT_DIR
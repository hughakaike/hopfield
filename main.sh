#!/bin/bash

INPUT_DIR="input"
OUTPUT_DIR="output"
SCRIPT_DIR="scr"

python $SCRIPT_DIR/convert2matrix.py $INPUT_DIR
./main.exe $INPUT_DIR $OUTPUT_DIR
python $SCRIPT_DIR/convert2image.py $OUTPUT_DIR
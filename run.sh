#!/bin/bash

python3 src/convert2matrix.py
./build/main
python3 src/convert2image.py

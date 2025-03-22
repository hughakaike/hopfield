set INPUT_DIR=input
set OUTPUT_DIR=output
set SCRIPT_DIR=scr

echo convert image to matrix
python %SCRIPT_DIR%/convert2matrix.py %INPUT_DIR%
pause

echo start hopfield main
start main.exe %INPUT_DIR% %OUTPUT_DIR%
pause

echo convert matrix to image
python %SCRIPT_DIR%/convert2image.py %OUTPUT_DIR%
#python convert2matrix.py filename 
#convert filename/images/*.png to filename/matrix/*.dat

import sys
import os
import glob

from PIL import Image, ImageOps

CDIR=os.getcwd()

with open("config.txt") as f:
    for s_line in f:
        if "input_folder" in s_line:
            path = s_line.split()[1]
            print(path)
        elif "image_file_format" in s_line:
            image_file_format = s_line.split()[1]
            print(image_file_format)

try:
    os.chdir(os.path.join(CDIR,path,"images"))
except:
    print("No input folder found.")
    sys.exit(1)

image_files = glob.glob("*."+image_file_format)

if len(image_files) == 0:
    print("No image files found.")
    sys.exit(1)

try:
    os.chdir(os.path.join(CDIR,path,"matrix"))
except:
    os.mkdir(os.path.join(CDIR,path,"matrix"))
    os.chdir(os.path.join(CDIR,path,"matrix"))

print(image_files)

f_list = open(os.path.join(CDIR,path,"matrix","train_images.dat"), 'w')
for file in image_files:
    f_list.write(file[0:-4]+".dat\n")
    image = Image.open(os.path.join(CDIR,path,"images",file))
    image_gray = ImageOps.grayscale(image)
    w, h = image.size
    # prints "height width" to the 1st line
    f = open(os.path.join(CDIR,path,"matrix",file[0:-4]+".dat"), 'w')
    f.write('{} {}\n'.format(h, w))
    for y in range(h):
        for x in range(w):
            f.write('{} '.format(image_gray.getpixel((x, y))))
        f.write('\n')
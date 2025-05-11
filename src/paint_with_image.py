from tkinter import *
from tkinter.colorchooser import askcolor
from tkinter import font
import customtkinter as ctk
import os
import platform
from PIL import Image, ImageOps, ImageFilter, ImageTk, ImageDraw, EpsImagePlugin
import time
import subprocess

INPUT_FILE = "Aerial.bmp"
OUTPUT_SIZE = (256, 256)
OUTPUT_FORMAT = 'bmp'

class Paint():
    DEFAULT_PEN_SIZE = 5.0
    DEFAULT_COLOR = 'black'
    
    def __init__(self):
        self.root = Tk()
        self.root.title("paint with image")

        self.pen_button = Button(self.root, text='ペン', command=self.use_pen)
        self.pen_button.grid(row=0, column=0)

        self.eraser_button = Button(self.root, text='消しゴム', command=self.use_eraser)
        self.eraser_button.grid(row=0, column=1)

        font1 = font.Font(family='Helvetica', size=10)
        self.size_label = Label(self.root, text="width", font=font1)
        self.size_label.grid(row=1, column=2)

        self.size_button = Scale(self.root, from_=1, to=20, orient=HORIZONTAL)
        self.size_button.grid(row=0, column=2)

        font1 = font.Font(family='Helvetica', size=10, weight='bold')
        self.status = StringVar()
        self.status.set("(None.)")
        self.status_label = Label(self.root, textvariable=self.status, font=font1, fg="red")
        self.status_label.grid(row=0, column=3, columnspan=2)

        self.canvas = Canvas(self.root, bg='white', width=256, height=256)
        self.canvas.grid(row=2, column=0, columnspan=3)
        self.img_file_input = ImageTk.PhotoImage(Image.open("input/images/"+INPUT_FILE))
        self.canvas.create_image(0, 0, image = self.img_file_input, anchor = NW)

        self.image_canvas=Canvas(self.root, bg='white', width=256, height=256)
        self.image_canvas.grid(row=2, column=3, columnspan=3)
        self.img_file_result = None
        self.image_canvas.create_image(0, 0, image = self.img_file_result, anchor = NW)

        self.predict_button = Button(self.root, text='予測', command=self.predict)
        self.predict_button.grid(row=3, column=0)

        self.clear_button = Button(self.root, text='クリア', command=self.clear_canvas)
        self.clear_button.grid(row=3,column=1)

        self.setup()
        self.root.mainloop()

    def setup(self):
        self.old_x = None
        self.old_y = None
        self.line_width = self.size_button.get()
        self.color = self.DEFAULT_COLOR
        self.eraser_state = False
        self.active_button = self.pen_button
        self.canvas.bind('<B1-Motion>', self.paint)
        self.canvas.bind('<ButtonRelease-1>', self.reset)

    def use_pen(self):
        self.activate_button(self.pen_button)

    def use_brush(self):
        self.activate_button(self.brush_button)

    def use_eraser(self):
        self.activate_button(self.eraser_button, eraser_mode=True)

    def activate_button(self, some_button, eraser_mode=False):
        self.active_button.config(relief=RAISED)
        some_button.config(relief=SUNKEN)
        self.active_button = some_button
        self.eraser_state = eraser_mode

    def paint(self, event):
        self.line_width = self.size_button.get()
        paint_color = 'white' if self.eraser_state else self.color
        if self.old_x and self.old_y:
            self.canvas.create_line(self.old_x, self.old_y, event.x, event.y,
                               width=self.line_width, fill=paint_color,
                               capstyle=ROUND, smooth=TRUE, splinesteps=36)
        self.old_x = event.x
        self.old_y = event.y

    def reset(self, event):
        self.old_x, self.old_y = None, None

    def clear_canvas(self):
        self.canvas.delete("all")
        self.image_canvas.delete("all")
        self.canvas.create_image(0, 0, image = self.img_file_input, anchor = NW)

    def predict(self):
        CDIR=os.getcwd()
        self.canvas.postscript(file="test.ps")
        saveimg= Image.open(CDIR+"/test.ps")
        saveimg.convert("1")
        saveimg=saveimg.resize(size=OUTPUT_SIZE)
        saveimg.save(CDIR+"/input/images/test."+OUTPUT_FORMAT)

        self.status.set("画像を生成中・・・")
        self.root.update()
        if platform.system() == 'win32':
            cp=subprocess.run("start run.bat", shell=True)
        elif platform.system() == 'Linux':
            cp=subprocess.run("./run.sh",shell=True)
        else:
            self.status.set("OS not supported.")
            return
        if cp.returncode != 0:
            self.status.set("Error: " + str(cp.returncode))
            return
        self.img_file_result = PhotoImage(file = "output/images/output_image.png")
        self.image_canvas.create_image(0, 0, image = self.img_file_result, anchor = NW)
        self.status.set("完了") 

if __name__ == '__main__':
    print("os name = " + platform.system())
    Paint()
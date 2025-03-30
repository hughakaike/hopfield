from tkinter import *
from tkinter.colorchooser import askcolor
from tkinter import font
import os
from PIL import Image, ImageOps, ImageFilter, ImageTk, ImageDraw, EpsImagePlugin
import time

class Paint(object):
    DEFAULT_PEN_SIZE = 5.0
    DEFAULT_COLOR = 'black'

    def __init__(self):
        self.root = Tk()

        self.pen_button = Button(self.root, text='ペン', command=self.use_pen)
        self.pen_button.grid(row=0, column=0)

        self.brush_button = Button(self.root, text='ブラシ', command=self.use_brush)
        self.brush_button.grid(row=0, column=1)

        self.eraser_button = Button(self.root, text='消しゴム', command=self.use_eraser)
        self.eraser_button.grid(row=0, column=2)

        self.size_button = Scale(self.root, from_=1, to=10, orient=HORIZONTAL)
        self.size_button.grid(row=0, column=3)

        font1 = font.Font(family='Helvetica', size=10, weight='bold')
        self.status = StringVar()
        self.status.set("(None.)")
        self.status_label = Label(self.root, textvariable=self.status, fg="red", font=font1)
        self.status_label.grid(row=0, column=4, columnspan=2)

        self.canvas = Canvas(self.root, bg='white', width=256, height=256)
        self.canvas.grid(row=1, column=0, columnspan=3)

        self.image_canvas=Canvas(self.root, bg='white', width=256, height=256)
        self.image_canvas.grid(row=1, column=3, columnspan=3)
        self.img_file=None
        self.image_canvas.create_image(0, 0, image = self.img_file, anchor = NW)

        self_predict_button = Button(self.root, text='予測', command=self.predict)
        self_predict_button.grid(row=2, column=0)

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

    def predict(self):
        CDIR=os.getcwd()
        self.canvas.postscript(file="test.ps")
        saveimg= Image.open(CDIR+"/test.ps")
        saveimg.convert("1")
        saveimg=saveimg.resize(size=(256,256))
        saveimg.save(CDIR+"/input/images/test.bmp")
        self.status.set("画像を生成中・・・")
        self.root.update()
        os.system('./run.sh')
        time.sleep(3)
        self.img_file = PhotoImage(file = "output/images/output_image.png")
        self.image_canvas.create_image(0, 0, image = self.img_file, anchor = NW)
        self.status.set("完了") 

if __name__ == '__main__':
    Paint()
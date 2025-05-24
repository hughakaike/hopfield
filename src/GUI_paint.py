from pickle import OBJ
import customtkinter as ctk
from tkinter import *
from tkinter import font
import os
import platform
from PIL import Image, ImageOps, ImageFilter, ImageTk, ImageDraw, EpsImagePlugin, ImageEnhance
import time
import subprocess

class App(ctk.CTk):
    PEN_COLOR = 'black'
    BOTTON_HEIGHT = 120
    BOTTON_WIDTH = 120
    ACTIVE_BOTTON_COLOR = "gray"
    NORMAL_BOTTON_COLOR = "white"
    HOVER_BOTTON_COLOR = "gray"
    ACTIVE_TEXT_COLOR = "white"
    NORMAL_TEXT_COLOR = "black"
    ACTIVE_IMAGE_BRIGHTNESS = 3
    NORMAL_IMAGE_BRIGHTNESS = 0
    BORDER_COLOR = "gray"
    BORDER_WIDTH = 1
    SLIDER_BOTTON_COLOR = "gray"
    SLIDER_HOVER_BOTTON_COLOR = "gray"
    CANVAS_TEXT_COLOR = "gray"

    def __init__(self):
        super().__init__()
        self.eraser_state = False
        self.BOTTON_FONT = ctk.CTkFont(family='Helvetica', size=20, weight='bold')
        self.CANVAS_FONT = ctk.CTkFont(family='Helvetica', size=40, weight='bold')

        ctk.set_appearance_mode("light")
        ctk.set_default_color_theme("blue")
        self.geometry("1200x600")
        self.title("Hopfield")
        self.grid_rowconfigure(0,weight=1)
        self.grid_columnconfigure(0,weight=1)

        self.right_frame = RightFrame(master=self)
        self.right_frame.grid(row=0,column=1,sticky="ns",padx=30,pady=30)
        
        self.left_frame = LeftFrame(master=self)        
        self.left_frame.grid(row=0,column=0,sticky="nsew",padx=30,pady=30)
                
class RightFrame(ctk.CTkFrame):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs)
        self.parent = kwargs["master"]
        self.f = self.parent.BOTTON_FONT
        self.h = self.parent.BOTTON_HEIGHT
        self.w = self.parent.BOTTON_WIDTH
        self.abc = self.parent.ACTIVE_BOTTON_COLOR
        self.nbc = self.parent.NORMAL_BOTTON_COLOR
        self.hbc = self.parent.HOVER_BOTTON_COLOR
        self.atc = self.parent.ACTIVE_TEXT_COLOR
        self.ntc = self.parent.NORMAL_TEXT_COLOR
        self.aib = self.parent.ACTIVE_IMAGE_BRIGHTNESS
        self.nib = self.parent.NORMAL_IMAGE_BRIGHTNESS
        self.bc = self.parent.BORDER_COLOR
        self.bw = self.parent.BORDER_WIDTH
        self.sbc = self.parent.SLIDER_BOTTON_COLOR
        self.shbc = self.parent.SLIDER_HOVER_BOTTON_COLOR
        self.pen_filename = "src/UI_image/Pen_Image.png"
        self.brush_filename = "src/UI_image/Brush_Image.png"
        self.eraser_filename = "src/UI_image/Eraser_Image.png"
        self.pridict_filename = "src/UI_image/Pridict_Image.png"
        self.clear_filename = "src/UI_image/Clear_Image.png"

        self.pen_button = ctk.CTkButton(master=self, text="pen", image=self.Image(self.pen_filename,self.aib),
                                        command=self.use_pen, font=self.f, fg_color=self.abc, hover_color=self.hbc,
                                        width=self.w, height=self.h, corner_radius=0, text_color=self.atc,
                                        border_color=self.bc, border_width=self.bw)
        self.pen_button.grid(row=0, column=0,padx=(10,0),pady=(30,0),sticky="se")

        self.brush_button = ctk.CTkButton(master=self, text='brush', image=self.Image(self.brush_filename,self.nib),
                                          command=self.use_brush, font=self.f, fg_color=self.nbc, hover_color=self.hbc,
                                          width=self.w, height=self.h, corner_radius=0, text_color=self.ntc,
                                          border_color=self.bc, border_width=self.bw)
        self.brush_button.grid(row=0, column=1,padx=(0,10),pady=(30,0),sticky="sw")

        self.eraser_button = ctk.CTkButton(master=self, text="eraser", image=self.Image(self.eraser_filename,self.nib),
                                           command=self.use_eraser, font=self.f, fg_color=self.nbc, hover_color=self.hbc,
                                           width=self.w, height=self.h, corner_radius=0, text_color=self.ntc,
                                           border_color=self.bc, border_width=self.bw)
        self.eraser_button.grid(row=1, column=0,padx=(10,0),pady=(0,30),sticky="ne")

        self.size_button = ctk.CTkSlider(master=self, from_=1, to=20, number_of_steps=20,
                                         button_color=self.sbc,button_hover_color=self.shbc)
        self.size_button.grid(row=2, column=0,columnspan=2,padx=0,pady=(30,30))
        
        self.predict_button = ctk.CTkButton(master=self, text='pridict', image=self.Image(self.pridict_filename,self.nib),
                                            command=self.predict, font=self.f,fg_color=self.nbc, hover_color=self.hbc,
                                            width=self.w, height=self.h, corner_radius=0, text_color=self.ntc,
                                            border_color=self.bc, border_width=self.bw)
        self.predict_button.grid(row=3, column=0,padx=(10,0),pady=30,sticky="e")

        self.clear_button = ctk.CTkButton(master=self, text='clear', image=self.Image(self.clear_filename,self.nib),
                                          command=self.clear_canvas, font=self.f,fg_color=self.nbc, hover_color=self.hbc,
                                          width=self.w, height=self.h, corner_radius=0,text_color=self.ntc,
                                          border_color=self.bc, border_width=self.bw)
        self.clear_button.grid(row=3,column=1,padx=(0,10),pady=30,sticky="w")
        
        self.status = StringVar()
        self.status.set("")
        self.status_label = ctk.CTkLabel(master=self, textvariable=self.status, font=self.f)
        self.status_label.grid(row=4, column=0, columnspan=2,padx=0,pady=30,sticky="s")
        
        self.active_button = self.pen_button

    def Image(self,file_name,brighness):
        image = Image.open(file_name)
        image = ImageEnhance.Brightness(image).enhance(brighness)
        image = ctk.CTkImage(light_image=image, dark_image=image, size=(30, 30))
        return image


    def use_pen(self):
        self.activate_button(self.pen_button, self.pen_filename)

    def use_brush(self):
        self.activate_button(self.brush_button, self.brush_filename)

    def use_eraser(self):
        self.activate_button(self.eraser_button, self.eraser_filename, eraser_mode=True)

    def activate_button(self, some_button, filename, eraser_mode=False):
        self.active_button.configure(fg_color=self.nbc)
        self.active_button.configure(image=self.Image(filename,self.nib))
        self.active_button.configure(text_color=self.ntc)
        some_button.configure(fg_color=self.abc)
        some_button.configure(image=self.Image(filename,self.aib))
        some_button.configure(text_color=self.atc)
        self.active_button = some_button
        self.parent.eraser_state = eraser_mode

    def clear_canvas(self):
        self.parent.left_frame.canvas.delete("eraseable")
        self.parent.left_frame.image_canvas.delete("eraseable")
        self.parent.left_frame.check_canvas(self.parent.left_frame)

    def predict(self):
        CDIR=os.getcwd()
        self.parent.left_frame.canvas.postscript(file="test.ps")
        saveimg= Image.open(CDIR+"/test.ps")
        saveimg=saveimg.resize(size=(256,256))
        saveimg.convert("1")
        saveimg.save(CDIR+"/input/images/test.png")

        self.status.set("画像を生成中・・・")
        self.parent.update()
        if platform.system() == 'Windows':
            #cp=subprocess.run(["start", "run.bat"], shell=True)
            cp=subprocess.run("timeout /t 2 /nobreak >nul", shell=True)
            cp=subprocess.run("python src/convert2matrix.py", shell=True)
            cp=subprocess.run("timeout /t 2 /nobreak >nul", shell=True)
            cp=subprocess.run("start main.exe", shell=True)
            cp=subprocess.run("timeout /t 2 /nobreak >nul", shell=True)
            cp=subprocess.run("python src/convert2image.py", shell=True)
            cp=subprocess.run("timeout /t 1 /nobreak >nul", shell=True)
        elif platform.system() == 'Linux':
            cp=subprocess.run("./run.sh",shell=True)
        else:
            self.status.set("OS not supported.")
            return
        if cp.returncode != 0:
            self.status.set("Error: " + str(cp.returncode))
            return
        self.original_image = Image.open("output/images/output_image.png")
        self.parent.left_frame.image_canvas.update_idletasks()
        self.new_width = self.parent.left_frame.image_canvas.winfo_width()
        self.new_height = self.parent.left_frame.image_canvas.winfo_height()
        self.resized_image = self.original_image.resize((self.new_width, self.new_height))
        self.img_file = ImageTk.PhotoImage(self.resized_image)
        self.parent.left_frame.image_canvas.create_image(0, 0, image = self.img_file, anchor = NW, tags="eraseable")
        self.status.set("完了")

class LeftFrame(ctk.CTkFrame):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs)
        self.parent = kwargs["master"]
        self.f = self.parent.CANVAS_FONT
        self.ctc = self.parent.CANVAS_TEXT_COLOR
        self.color = self.parent.PEN_COLOR
        self.old_x = None
        self.old_y = None

        self.grid_rowconfigure(0,weight=1)
        self.grid_columnconfigure(0,weight=1)
        self.grid_columnconfigure(1,weight=1)
        
        self.canvas = Canvas(self, bg='white', width=640, height=640)
        self.canvas.grid(row=0, column=0,sticky="ne",pady=30)

        self.image_canvas = Canvas(self, bg='white', width=640, height=640)
        self.image_canvas.grid(row=0, column=1,sticky="nw",pady=30)
        self.img_file = None

        self.canvas.bind('<B1-Motion>', self.paint)
        self.canvas.bind('<ButtonRelease-1>', self.reset)
        self.canvas.bind("<Configure>", lambda event: self.update_canvas_text())        

    def update_canvas_text(self):
        self.canvas.update_idletasks()
        center_x = self.canvas.winfo_width() / 2
        center_y = self.canvas.winfo_height() / 2
        self.canvas.delete("left_text")
        self.left_text = self.canvas.create_text(center_x, center_y, text="一桁の数字を入力してください",
                                                  font=self.f, fill=self.ctc, tags="left_text")  
        self.image_canvas.update_idletasks()
        center_x = self.image_canvas.winfo_width() / 2
        center_y = self.image_canvas.winfo_height() / 2
        self.image_canvas.delete("right_text")
        self.right_text = self.image_canvas.create_text(center_x, center_y, text="ここに結果が出力されます",
                                                  font=self.f, fill=self.ctc, tags="right_text")  

    def check_canvas(self,object):
        items = object.canvas.find_all()
        if len(items) > 1:
            object.canvas.itemconfig(object.left_text, state="hidden")
        else:
            object.canvas.itemconfig(object.left_text, state="normal") 
        
        items = object.image_canvas.find_all()
        if len(items) > 1:
            object.image_canvas.itemconfig(object.right_text, state="hidden")
        else:
            object.image_canvas.itemconfig(object.right_text, state="normal") 

    def paint(self, event):
        self.line_width = self.parent.right_frame.size_button.get()
        if self.parent.eraser_state:
            items = self.canvas.find_overlapping(event.x - self.line_width, event.y - self.line_width,
                                        event.x + self.line_width, event.y + self.line_width)
            for item in items:
                if self.canvas.gettags(item) == ("eraseable",):
                    self.canvas.delete(item)
        else:
            if self.old_x and self.old_y:
                self.canvas.create_line(self.old_x, self.old_y, event.x, event.y,
                                   width=self.line_width, fill=self.color,
                                   capstyle=ROUND, smooth=TRUE, splinesteps=36, tags="eraseable")
        self.old_x = event.x
        self.old_y = event.y
        self.check_canvas(self)

    def reset(self, event):
        self.old_x, self.old_y = None, None


if __name__=="__main__":
    app=App()
    app.mainloop()
#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Written by VTKSauer, Wyvern
#Will work with "serial_motor_control_01a" loaded onto Arduino Feather M0
#  or similar code with [s][1][f][d][x] type serial input

#http://effbot.org/tkinterbook/tkinter-whats-tkinter.htm
#https://stackoverflow.com/questions/38153754/can-you-fit-multiple-buttons-in-one-grid-cell-in-tkinter

#Manually change the COM port to match the Arduino Feather M0

import serial
import tkinter as tk   

from functools import partial

ser = serial.Serial('COM5', 9600, timeout=1)

# class Application(tk.Frame):              
    # def __init__(self, master=None):
        # tk.Frame.__init__(self, master)   
        # self.grid()                       
        # self.createWidgets()

    # def createWidgets(self):
        # self.quitButton = tk.Button(self, text='Quit',
            # command=self.quit)            
        # self.quitButton.grid()            

# app = Application()                       
# app.master.title('Sample application')    
# app.mainloop()                            

#from tkinter import *

# root = tk.Tk()

# w = tk.Label(root, text="Hello, world!")
# w.pack()

# root.mainloop()

#from tkinter import *

class App:

    def __init__(self, master):

        frame = tk.Frame(master)
        frame.grid(row=0, column=3)
        
        frame_stepper_1 = tk.Frame(master)
        frame_stepper_1.grid(row=1, column=1)
        frame_stepper_2 = tk.Frame(master)
        frame_stepper_2.grid(row=2, column=2)
        frame_stepper_3 = tk.Frame(master)
        frame_stepper_3.grid(row=3, column=1)
        frame_stepper_4 = tk.Frame(master)
        frame_stepper_4.grid(row=2, column=0)
        frame_stepper_all = tk.Frame(master)
        frame_stepper_all.grid(row=10, column=1)

        self.button = tk.Button(
            frame, text="QUIT", fg="red", command=frame.quit
            )
        #self.button.pack(side=tk.LEFT)
        self.button.grid(row=0,column=2)

        #self.hi_there = tk.Button(frame, text="Hello", command=self.say_hi)
        #self.hi_there.pack(side=tk.LEFT)
        #self.hi_there.grid(row=0,column=1)
        
        # self.test_serial = tk.Button(frame,text='Test',command=partial(self.move_stepper_single,'1','f'))
        # self.test_serial.grid(row=0,column=1)

        self.stepper_1_label = tk.Label(frame_stepper_1, text="Stepper 1")
        self.stepper_1_forward = tk.Button(frame_stepper_1, text="Stepper 1 forward", command=partial(self.move_stepper_single,'1','f'))
        self.stepper_1_reverse = tk.Button(frame_stepper_1, text="Stepper 1 reverse", command=partial(self.move_stepper_single,'1','b'))
        self.stepper_1_label.grid(row=0,column=0)
        self.stepper_1_forward.grid(row=1,column=0)
        self.stepper_1_reverse.grid(row=3,column=0)

        self.stepper_2_label = tk.Label(frame_stepper_2, text="Stepper 2")
        self.stepper_2_forward = tk.Button(frame_stepper_2, text="Stepper 2 forward", command=partial(self.move_stepper_single,'2','f'))
        self.stepper_2_reverse = tk.Button(frame_stepper_2, text="Stepper 2 reverse", command=partial(self.move_stepper_single,'2','b'))
        self.stepper_2_label.grid(row=0,column=0)
        self.stepper_2_forward.grid(row=1,column=0)
        self.stepper_2_reverse.grid(row=3,column=0)

        self.stepper_3_label = tk.Label(frame_stepper_3, text="Stepper 3")
        self.stepper_3_forward = tk.Button(frame_stepper_3, text="Stepper 3 forward", command=partial(self.move_stepper_single,'3','f'))
        self.stepper_3_reverse = tk.Button(frame_stepper_3, text="Stepper 3 reverse", command=partial(self.move_stepper_single,'3','b'))
        self.stepper_3_label.grid(row=0,column=0)
        self.stepper_3_forward.grid(row=1,column=0)
        self.stepper_3_reverse.grid(row=3,column=0)

        self.stepper_4_label = tk.Label(frame_stepper_4, text="Stepper 4")
        self.stepper_4_forward = tk.Button(frame_stepper_4, text="Stepper 4 forward", command=partial(self.move_stepper_single,'4','f'))
        self.stepper_4_reverse = tk.Button(frame_stepper_4, text="Stepper 4 reverse", command=partial(self.move_stepper_single,'4','b'))
        self.stepper_4_label.grid(row=0,column=0)
        self.stepper_4_forward.grid(row=1,column=0)
        self.stepper_4_reverse.grid(row=3,column=0)

        self.stepper_all_label = tk.Label(frame_stepper_all, text="All steppers")
        self.stepper_all_forward = tk.Button(frame_stepper_all, text="All steppers forward", command=partial(self.move_stepper_single,'a','f'))
        self.stepper_all_reverse = tk.Button(frame_stepper_all, text="All steppers reverse", command=partial(self.move_stepper_single,'a','b'))
        self.stepper_all_label.grid(row=0,column=0)
        self.stepper_all_forward.grid(row=1,column=0)
        self.stepper_all_reverse.grid(row=3,column=0)


    # def say_hi(self):
        # print ("hi there, everyone!")

    # def say_stepper_1(self):
        # print ("Stepper 1!")

    # def say_stepper_2(self):
        # print ("Stepper 2!")

    # def say_stepper_3(self):
        # print ("Stepper 3!")

    # def say_stepper_4(self):
        # print ("Stepper 4!")

    # def move_stepper_1_forward(self):
        # print ("Stepper 1 forward!")
        # a = b's1fx\n'
        # print(a)
        # ser.write(a)

    # def move_stepper_1_reverse(self):
        # print ("Stepper 1 reverse!")
        # ser.write(b's1rx\n')

    # def move_stepper_2_forward(self):
        # print ("Stepper 2 forward!")
        # ser.write(b's2fx\n')

    # def move_stepper_2_reverse(self):
        # print ("Stepper 2 reverse!")
        # ser.write(b's2rx\n')

    # def move_stepper_3_forward(self):
        # print ("Stepper 3 forward!")
        # ser.write(b's3fx\n')

    # def move_stepper_3_reverse(self):
        # print ("Stepper 3 reverse!")
        # ser.write(b's3rx\n')

    # def move_stepper_4_forward(self):
        # print ("Stepper 4 forward!")
        # ser.write(b's4fx\n')

    # def move_stepper_4_reverse(self):
        # print ("Stepper 4 reverse!")
        # ser.write(b's4rx\n')

    # def move_stepper_all_forward(self):
        # print ("All steppers forward!")
        # ser.write(b'safx\n')

    # def move_stepper_all_reverse(self):
        # print ("All steppers reverse!")
        # ser.write(b'sarx\n')

    # def move_stepper(self,steppernumber,direction):
        # #steppernumber = str(steppernumer)
        # print (steppernumber + direction)
        # serialinputstr = "b\'s{}{}x\np".format(steppernumber,direction)
        # serialinput = serialinputstr.encode('utf-8')
        # ser.write(serialinput)
        # print (serialinputstr)

    def move_stepper_single(self,steppernumber,direction): #stepper 1-4,a; direction: f,r
        steppernumberprint = 'Stepper ' + steppernumber
        if steppernumber =='a':
            steppernumberprint = 'All steppers'
        if direction =='f':
            directionprint = 'forward'
        elif direction =='b':
            directionprint = 'backwards'
        else:
            directionprint = 'not moving correctly'
        print (steppernumberprint + ' ' +directionprint)
        a = "s" + steppernumber + direction + "\n"  #formatting command in string
        serialinput = a.encode('utf-8')             #encoding to bytes
        ser.write(serialinput)                      #input in bytes

root = tk.Tk()

app = App(root)

root.mainloop()
#root.destroy() # optional; see description below
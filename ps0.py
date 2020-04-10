# -*- coding: utf-8 -*-
"""
Created on Mon March 30 20:59:47 2020

@author: cassidykuhn
"""
import tkinter as tkr
from tkinter import ttk 
import csv

with open('/Users/cassidykuhn/Downloads/capstone_project-master/AcctDB.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    
    for row in csv_reader:
        print(row[0], row[1], row[2])

#other way to print the data from the csv file 
#import pandas- pandas is a cross to C 
#df = pandas.read_csv('/Users/cassidykuhn/Downloads/capstone_project-master/AcctDB.csv')
#print(df)

tk = tkr.Tk()
canvas = tkr.Canvas(tk, width = 500, height = 500)
canvas.grid()
tk.title("GUI Interface")

balance = float(input("How much money is in the account?"))
if balance > 0: 
    canvas.create_rectangle(5,5,500,500, fill='green') 
    canvas.create_text(250,250, text = "Positive Use Case")
else:
    canvas.create_rectangle(5,5,500,500, fill='red') 
    canvas.create_text(250,250, text = "Negative Use Case")


tk.mainloop()


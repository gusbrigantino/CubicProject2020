#imports
import socket	
import sys
import time
import threading
import tkinter as tkr
from PIL import Image

#const vars 
portNumber = 8080	
IPAddress = '127.0.0.1'
bufferSize = 1024

nameIndex = 0
balanceIndex = 1
foundStatusIndex = 2
balanceStatusIndex = 3

ticketPrice = 2.5

dataIndex = 0

delimiter = ','
dataEnd = b'\0'

guiScreenDelay = 3000                                                       #3 seconds

def main():

#   SOCKET INIT
    guiSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #create socket
    serverAddress = (IPAddress, portNumber)                                 #create socket info with IP address and port #
    guiSocket.bind(serverAddress)                                           #bind
    guiSocket.listen(16)                                                    #listen TODO: unsure what argument does/is


#   TKINTER INIT
    tk = tkr.Tk()

    smallScreen = False

    screenWidth = tk.winfo_screenwidth()
    screenHeight = tk.winfo_screenheight()

    if(screenWidth <= 1024):
        smallScreen = True

    tk.title("San Diego Metro Station")
    tk.bind("<F11>", lambda event: tk.attributes("-fullscreen", not tk.attributes("-fullscreen")))
    tk.bind("<Escape>", lambda event: tk.attributes("-fullscreen", False))
    tk.attributes('-fullscreen', True)

#   CANVAS INIT
    canvas = tkr.Canvas(tk, width = screenWidth, height = screenHeight, bg = 'white')
    canvas.grid()

#   CANVAS IMG
    if(smallScreen):
        cubicImg = tkr.PhotoImage(file='cubicImgSmall.png')
    else:
        cubicImg = tkr.PhotoImage(file='cubicImg.png')
    canvasCubicImg = canvas.create_image((screenWidth - cubicImg.width()), (screenHeight - cubicImg.height()), image=cubicImg, anchor='nw')

#   CANVAS TEXT
    canvasTitleText = canvas.create_text((screenWidth / 8), 10, fill = "darkblue", font = "Times 100", text = "San Diego Metro Station", anchor='nw')
    canvasAcctInfoRect = canvas.create_rectangle((screenWidth / 2), (screenHeight / 4), (screenWidth - 10), (screenHeight - cubicImg.height()), outline = "darkblue", width = 7)
    canvasAcctInfoText = canvas.create_text((screenWidth / 2) + 50, 230, fill = "darkblue", font = "Times 60", text = "Account Information:", anchor='nw')

#   TKINTER UPDATE
    tk.update_idletasks()
    tk.update()
    i = 0
    while True: 
        tk.update_idletasks()
        tk.update()
        Timer(500)

        connection, clientAddress = guiSocket.accept()	                    #connection made with client (val)
        
        try:
            #Incoming data in order: "<Acct Name>, <Acct Balance>, <Acct Found Status>, <Acct Balance Status>"
            #-----------------------------------------------------------------------------------------------#

            rawBytes = connection.recv(bufferSize)                          #get all of the incoming data (size of buffer)

            cleanBytes = rawBytes.split(dataEnd)                            #find the null terminator that ends the desired data in the buffer

            strData = cleanBytes[dataIndex].decode("utf-8")                 #decode the string type and keep whatever is in front of the null terminator
            
            data = strData.split(delimiter)

            acctName = data[nameIndex]                                      #set new vars
            acctBalance = data[balanceIndex]
            acctFoundStatus = int(data[foundStatusIndex])
            acctBalanceStatus = int(data[balanceStatusIndex])


            if(not acctFoundStatus):
                if(smallScreen):
                    invalidImg = tkr.PhotoImage(file='XSmall.png')
                else:
                    invalidImg = tkr.PhotoImage(file='X.png')

                canvasInvalidImg = canvas.create_image((invalidImg.width() / 4), 
                    (screenHeight - (invalidImg.height() + (invalidImg.height() / 4))), 
                    image=invalidImg, anchor = 'nw') 

                canvasNotFoundText = canvas.create_text((screenWidth / 2) + 90, 330, 
                    fill = "black", font = "Times 40", text = "Your account was not found", anchor = 'nw')

                canvasDeniedText = canvas.create_text((invalidImg.width() / 4) + 30, 300, 
                    fill = "red", font = "Times 40", text = "ACCESS DENIED", anchor = 'nw')
                

            elif(not acctBalanceStatus):
                if(smallScreen):
                    invalidImg = tkr.PhotoImage(file='XSmall.png')
                else:
                    invalidImg = tkr.PhotoImage(file='X.png')

                canvasInvalidImg = canvas.create_image((invalidImg.width() / 4), 
                    (screenHeight - (invalidImg.height() + (invalidImg.height() / 4))), 
                    image=invalidImg, anchor='nw') 

                canvasNoBalanceText = canvas.create_text((screenWidth / 2) + 90, 330, 
                    fill = "black", font = "Times 40", text = "Welcome " + acctName + ".\nYou have $" 
                    + acctBalance + " in your account.\nYou need $" + str(ticketPrice - float(acctBalance)) 
                    + "to purchase a ticket.", anchor = 'nw')

                canvasDeniedText = canvas.create_text((invalidImg.width() / 4) + 30, 300, 
                    fill = "red", font = "Times 40", text = "ACCESS DENIED", anchor = 'nw')

            else:
                if(smallScreen):
                    validImg = tkr.PhotoImage(file='checkSmall.png')
                else:
                    validImg = tkr.PhotoImage(file='check.png')

                canvasValidImg = canvas.create_image((validImg.width() / 4), 
                    (screenHeight - (validImg.height() + (validImg.height() / 4))), 
                    image=validImg, anchor='nw') 

                canvasWelcomeText = canvas.create_text((screenWidth / 2) + 90, 330, 
                    fill = "black", font = "Times 40", text = "Welcome " + acctName + ".\nYour new balance is $" 
                    + acctBalance + ".\nEnjoy your trip.", anchor = 'nw')

                canvasAcceptedText = canvas.create_text((validImg.width() / 4) + 30, 300, 
                    fill = "green", font = "Times 40", text = "ACCESS GRANTED", anchor = 'nw')


            tk.update_idletasks()
            tk.update()
            Timer(guiScreenDelay)
            

            if(not acctFoundStatus):
                canvas.delete(canvasInvalidImg)
                canvas.delete(canvasNotFoundText)
                canvas.delete(canvasDeniedText)
            elif(not acctBalanceStatus):
                canvas.delete(canvasInvalidImg)
                canvas.delete(canvasNoBalanceText)
                canvas.delete(canvasDeniedText)
            else:
                canvas.delete(canvasValidImg)
                canvas.delete(canvasWelcomeText)
                canvas.delete(canvasAcceptedText)

            #delays to keep screen displayed
            #will catch up if a new beacon arrives will still displaying 
            #as long as there is enough space in buffer
            #TODO: should open gate after GUI display completes
            #maybe find a better way to keep screen up using GUI library
            #TODO: make different delay times for different screens NF: 1s NB: 3s A: 3s
            

        finally:
            connection.close()                                              #close connection


def Timer(milliseconds):
    startTime = int(round(time.time()) * 1000)
    timerFlag = True

    while(timerFlag):
        millisecondsPassed = (int(round(time.time()) * 1000) - startTime)
        if(millisecondsPassed >= milliseconds):
            timerFlag = False


if __name__ == "__main__":
    main()
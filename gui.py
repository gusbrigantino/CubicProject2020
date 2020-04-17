#imports
import socket	
import sys
import time
import threading
import tkinter as tkr

#const vars 
portNumber = 8080	
IPAddress = '127.0.0.1'
bufferSize = 1024

nameIndex = 0
balanceIndex = 1
foundStatusIndex = 2
balanceStatusIndex = 3

dataIndex = 0

delimiter = ','
dataEnd = b'\0'

guiScreenDelay = 3000                                                       #3 seconds

def main():

#   SOCKET INIT
#    guiSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #create socket
#    serverAddress = (IPAddress, portNumber)                                 #create socket info with IP address and port #
#    guiSocket.bind(serverAddress)                                           #bind
#    guiSocket.listen(16)                                                    #listen TODO: unsure what argument does/is


#   TKINTER INIT
    tk = tkr.Tk()

    screenWidth = tk.winfo_screenwidth()
    screenHeight = tk.winfo_screenheight()

    tk.title("San Diego Metro Station")
    tk.bind("<F11>", lambda event: tk.attributes("-fullscreen", not tk.attributes("-fullscreen")))
    tk.bind("<Escape>", lambda event: tk.attributes("-fullscreen", False))
    tk.attributes('-fullscreen', True)

#   CANVAS INIT
    canvas = tkr.Canvas(tk, width = screenWidth, height = screenHeight, bg = 'white')
    canvas.grid()

#   CANVAS IMG
    cubicImg = tkr.PhotoImage(file='cubicImg.png')
    canvasCubicImg = canvas.create_image((screenWidth - cubicImg.width()), (screenHeight - cubicImg.height()), image=cubicImg, anchor='nw')
    
#   TKINTER UPDATE
    tk.update_idletasks()
    tk.update()
    i = 0
    while True: 
        tk.update_idletasks()
        tk.update()
        Timer(500)

#        connection, clientAddress = guiSocket.accept()	                    #connection made with client (val)
        
        try:
            #Incoming data in order: "<Acct Name>, <Acct Balance>, <Acct Found Status>, <Acct Balance Status>"
            #-----------------------------------------------------------------------------------------------#

#            rawBytes = connection.recv(bufferSize)                          #get all of the incoming data (size of buffer)
#            print(type(rawBytes))
#            cleanBytes = rawBytes.split(dataEnd)                            #find the null terminator that ends the desired data in the buffer

#            strData = cleanBytes[dataIndex].decode("utf-8")                 #decode the string type and keep whatever is in front of the null terminator
            
            strData = ["Venkat,34.0,1,1", "Venkat,34.0,0,1", "Venkat,34.0,1,0", "Venkat,34.0,1,1"]

            data = strData[i].split(delimiter)                                 #split data into vars
            i = i + 1
            if(i >= 4):
                i = 0
                                                                            #set new vars
            acctName = data[nameIndex]
            acctBalance = data[balanceIndex]
            acctFoundStatus = int(data[foundStatusIndex])
            acctBalanceStatus = int(data[balanceStatusIndex])


            if(not acctFoundStatus):
                print("NOT FOUND")
                invalidImg = tkr.PhotoImage(file='X.png')

                canvasInvalidImg = canvas.create_image((invalidImg.width() / 4), 
                    (screenHeight - (invalidImg.height() + (invalidImg.height() / 4))), 
                    image=invalidImg, anchor='nw') 

            elif(not acctBalanceStatus):
                print("NO BALANCE")
                invalidImg = tkr.PhotoImage(file='X.png')

                canvasInvalidImg = canvas.create_image((invalidImg.width() / 4), 
                    (screenHeight - (invalidImg.height() + (invalidImg.height() / 4))), 
                    image=invalidImg, anchor='nw') 

            else:
                print("ACCEPTED")
                invalidImg = tkr.PhotoImage(file='check.png')

                canvasValidImg = canvas.create_image((invalidImg.width() / 4), 
                    (screenHeight - (invalidImg.height() + (invalidImg.height() / 4))), 
                    image=invalidImg, anchor='nw') 


            tk.update_idletasks()
            tk.update()
            Timer(guiScreenDelay)
            

            if(not acctFoundStatus or not acctBalanceStatus):
                canvas.delete(canvasInvalidImg)
            else:
                canvas.delete(canvasValidImg)

            #delays to keep screen displayed
            #will catch up if a new beacon arrives will still displaying 
            #as long as there is enough space in buffer
            #TODO: should open gate after GUI display completes
            #maybe find a better way to keep screen up using GUI library
            #TODO: make different delay times for different screens NF: 1s NB: 3s A: 3s
            

        finally:
            print("close")
#            connection.close()                                              #close connection


def Timer(milliseconds):
    startTime = int(round(time.time()) * 1000)
    timerFlag = True

    while(timerFlag):
        millisecondsPassed = (int(round(time.time()) * 1000) - startTime)
        if(millisecondsPassed >= milliseconds):
            timerFlag = False


if __name__ == "__main__":
    main()
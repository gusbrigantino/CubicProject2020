#imports
import socket	
import sys
import time
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
#    guiSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #create socket
#    serverAddress = (IPAddress, portNumber)                                  #create socket info with IP address and port #
#    guiSocket.bind(serverAddress)                                           #bind

#    guiSocket.listen(16)                                                    #listen TODO: unsure what argument does/is

    while True: 
        
        #TODO: Launch GUI IDLE screen 
        tk = tkr.Tk()
        tk.attributes('-fullscreen', True)
        tk.title("San Diego Metro Station")

        screenWidth = tk.winfo_screenwidth()
        screenHeight = tk.winfo_screenheight()

        canvas = tkr.Canvas(tk, width = screenWidth, height = screenHeight, bg = 'white')
        canvas.grid()

        cubicImg = tkr.PhotoImage(file='cubic.png')

        canvas.create_image(10, 10, image=cubicImg, anchor='se')
        

        #canvas.create_rectangle(0, 0, screenWidth, screenHeight, fill = 'red') 
        #canvas.create_text(450, 450, text = "ACCT NOT FOUND")

        tk.update_idletasks()
        tk.update()


        Timer(5000)

#        connection, clientAddress = guiSocket.accept()	                    #connection made with client (val)
        
        try:
            #Incoming data in order: "<Acct Name>, <Acct Balance>, <Acct Found Status>, <Acct Balance Status>"
            #-----------------------------------------------------------------------------------------------#

#            rawBytes = connection.recv(bufferSize)                          #get all of the incoming data (size of buffer)

#            cleanBytes = rawBytes.split(dataEnd)                            #find the null terminator that ends the desired data in the buffer

#            strData = cleanBytes[dataIndex].decode("utf-8")                 #decode the string type and keep whatever is in front of the null terminator
            
            strData = "Venkat,34.0,1,1"

            data = strData.split(delimiter)                                 #split data into vars

                                                                            #set new vars
            acctName = data[nameIndex]
            acctBalance = data[balanceIndex]
            acctFoundStatus = int(data[foundStatusIndex])
            acctBalanceStatus = int(data[balanceStatusIndex])


            if(not acctFoundStatus):
                #TODO: Launch GUI NO ACCT screen
                #canvas.create_rectangle(0, 0, 1000, 1000, fill = 'red') 
                #canvas.create_text(450, 450, text = "ACCT NOT FOUND")
                print("ACCT NOT FOUND")
            elif(not acctBalanceStatus):
                #TODO: Launch GUI NO BALANCE screen
                #canvas.create_rectangle(0, 0, 1000, 1000, fill = 'red') 
                #canvas.create_text(450, 450, text = acctName + ", you do not have sufficient funds. Your balance is $" + acctBalance) 
                print("NOT ENOUGH ACCT BALANCE")
            else:
                #TODO: Launch GUI ACCEPTED screen
                #canvas.create_rectangle(0, 0, 1000, 1000, fill = 'green') 
                #canvas.create_text(450, 450, text = "Hello, " + acctName + "! Your remaining balance is $" + acctBalance)
                print("ACCT ACCEPTED")
            
            tk.update_idletasks()
            tk.update()
            
            Timer(guiScreenDelay)                                           #delays to keep screen displayed
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
#imports
import socket	
import sys

#const vars 
portNumber = 8080	
IPAdress = '127.0.0.1'
bufferSize = 1024

nameIndex = 0
balanceIndex = 1
foundStatusIndex = 2
balanceStatusIndex = 3

dataIndex = 0

delimeter = ", "
dataEnd = b'\0'

#program

guiSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #create socket
serverAddress = (IPAdress, portNumber)                                  #create socket info with IP address and port #
guiSocket.bind(serverAddress)                                           #bind

guiSocket.listen(16)                                                    #listen TODO: unsure what argument does/is

while True: 
    
    #TODO: Launch GUI IDLE screen 

    connection, clientAddress = guiSocket.accept()	                    #connection made with client (val)
    
    try:
        #Incoming data in order: "<Acct Name>, <Acct Balance>, <Acct Found Status>, <Acct Balance Status>"
        #-----------------------------------------------------------------------------------------------#

        rawBytes = connection.recv(bufferSize)                          #get all of the incoming data (size of buffer)

        cleanBytes = rawBytes.split(dataEnd)                            #find the null terminator that ends the desired data in the buffer

        strData = cleanBytes[dataIndex].decode("utf-8")                 #deconde to string type and keep whatever is in from of the null terminator
        
        data = strData.split(delimeter)                                 #split data into vars

                                                                        #set new vars
        acctName = data[nameIndex]
        acctBalance = data[balanceIndex]
        acctFoundStatus = int(data[foundStatusIndex])
        acctBalanceStatus = int(data[balanceStatusIndex])


        if(not acctFoundStatus):
            #TODO: Launch GUI NO ACCT screen
            print("ACCT NOT FOUND")
        elif(not acctBalanceStatus):
            #TODO: Launch GUI NO BALANCE screen 
            print("NOT ENOUGH ACCT BALANCE")
        else:
            #TODO: Launch GUI ACCEPTED screen
            print("ACCT ACCEPTED")

        #TODO: Add timer to keep screen up approx 5secs?
    finally:
        connection.close()                                              #close connection





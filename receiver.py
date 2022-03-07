#!/usr/bin/env python

import time
import serial
import re

ser = serial.Serial(port='/dev/ttyS0',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
) # open serial port

NODE_MAX = 1 # number of nodes in network

while 1: # run indefinitely
    for i in range(50): # Node data request loop, take n measurements
        for k in range(NODE_MAX): # Node ID loop, increases with more sensor nodes
            print("Working with [NODE " + str(k+1) + "]") # print current NODE ID
            
            raw_data = ser.readline().strip() # read data from serial
            print("Incoming Data Type: " + str(type(raw_data))) # print data from serial
            print("Data Received: " + str(raw_data)) # print data type of serial
            
            data = str(raw_data,'UTF-8') # convert serial data to string
            print("Converting Data to: " + str(type(data))) # print new data type
            
            post = re.split(r'\t+',data) # parse string, saves elements into a list
            print("Converted Data: " + str(post)) # print data type of list
            
            if len(post) >= 4: # check length of list, must have 4 elements in list
                print("Expected Packet Length: " + post[3]) # print third element, checksum value
                print("Actual Packet Length: " + str(len(data))) # print length of string
                
                if len(data) == int(post[3]): # packet is valid if checksum = length of string
                    print("Saving Packet to File...") # print attempting to save data to file
                    t = time.localtime(time.time()) # grab the current time
                    
                    if int(post[0]) == (k+1): # check if NODE ID from list = NODE ID in loop
                        f_name = "node_%d"%(k+1) # initialize file name based on NODE ID
                        file = open(f_name,"a+") # open file to append packet and time data
                        file.write(data + "\t" + str(t.tm_year) + "\t" + str(t.tm_mon) + \
                                    "\t" + str(t.tm_mday) + "\t" + str(t.tm_hour) + "\t" + \
                                    str(t.tm_min) + "\t" + str(t.tm_sec) + "\n")
                        file.close() # close file
                        print("Packet Saved on: ",time.ctime(time.time())) # print time of packet saved
                    else: # print error, unexpected NODE ID, may cause problems with timing other nodes
                        print("Save Unsuccessful: NODE ID Mismatch, Expected [NODE %d] Instead"%int(post[0]))
                else: # print error, checksum from packet did not match length of packet
                    print("Checksum Error: Packet Invalid [NODE " + str(k+1) + "]")
            else: # print error, incomplete packet, missing data
                print("Error: Incomplete Packet [NODE " + str(k+1) + "]")
            # reset variables
            print("\n")
            raw_data = ""
            data = ""
            post = ""
            time.sleep(5)
            
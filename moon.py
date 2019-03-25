#!/usr/bin/env python
import datetime, time
from astral import Astral
import socket
from IPy import IP

# l = Location()
# l.name = 'Aarhus,Thunogade'
# l.region = 'Denmark'
# l.latitude = 56.16090
# l.longitude = 10.20393
# l.timezone = 'Europe/Copenhagen'
# l.elevation = 40
# l.sun()

a=Astral() #instantiate object


print time.ctime(),"startup!"

TCP_IP = '192.168.0.5'
TCP_PORT = 1337
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) #make socket reuseable, for debugging (enables you to rerun the program before the socket has timed out)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

print 'Opening TCP port', TCP_PORT

while True:
    try:
        conn, addr = s.accept()
        print time.ctime(),'Connection from:', addr
        #txString = str(22)
        txString = str(int(a.moon_phase(datetime.datetime.now())))+'\r'
        conn.send(txString)#+'\n')  # echo
        print 'TX: ', txString #txString.replace('\r','\r\n')
        print 'Client disconnected.'
        print '--------------------------'
        conn.close()
    except Exception as e:
        #print "hmm.. It looks like there was an error: " + str(e)
        print 'Client disconnected... :',str(e)
        print '--------------------------'
        conn.close()

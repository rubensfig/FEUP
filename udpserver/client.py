#!/usr/bin/python
'''
  Client side part of the SSL secure UDP based program.
'''
import socket as so
from Crypto.PublicKey import RSA
import random as ran
import base64

class Main:
  
  name = ""
  socket = 0
  s = 0  
  key = 0
  
  def __init__(self, socket, name):
	self.name = name
	self.socket = socket
	self.s = so.socket(so.AF_INET, so.SOCK_DGRAM)
	#self.ssl_sock = ssl.wrap_socket(s, ca_certs= "./certs/server.crt", cert_reqs=ssl.CERT_REQUIRED)
	self.s.connect((name, socket))	
	
  def sendthings(self, data):
	try:
	  data = "".join(data)
	  self.s.sendto(data, (self.name, self.socket))
	except:
	  print "Send Error"	

  def runbegin(self):
	self.sendthings("helo@garbage")
	self.sendthings("cert@garbage")
	data =  self.s.recvfrom(1024)
	
	self.key = RSA.importKey(data[0])
	
	
  def loopInfo(self):
	tosend = ""
	while True:
	  tosend = "data@" + str(ran.randint(0,100)) + "," + str(ran.randint(0,100))
	  tosend = base64.b64encode("".join(self.key.encrypt(tosend, 3)))
	  print tosend
	  self.sendthings(tosend)
	  
	 #self.sendthings(self.key.encrypt(tosend,3))
	
if __name__ == "__main__":
  
  socket = 10025
  servername = "gnomo.fe.up.pt"
  
  main = Main(socket, servername)
  main.runbegin()
  main.loopInfo()
  

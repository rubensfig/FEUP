#!/usr/bin/python
'''
  Server side part of the SSL secure UDP based program.
'''
import socket as so
import ssl
from os import chmod
from Crypto.PublicKey import RSA
import base64

class mainLoop:
  
  newsocket = 0
  fromaddr = ""
  
  public = 0
  private = 0
  
  publicKeyFile = "./certs/server.pub"
  privateKeyFile = "./certs/server.key"
  keys = 0
  
  def resetkeys(self):
	self.keys = RSA.generate(1024)
	
	pubHandle = open(self.privateKeyFile, 'wb')
	pubHandle.write(self.keys.exportKey())
	pubHandle.close()
	
	pubHandle = open(self.publicKeyFile, 'wb')
	pubHandle.write(self.keys.publickey().exportKey())
	pubHandle.close()
  
  def __init__(self, socket):

	self.bindsocket = so.socket(so.AF_INET, so.SOCK_DGRAM)
	self.bindsocket.setsockopt(so.SOL_SOCKET, so.SO_REUSEADDR, 1)
	self.bindsocket.bind(('127.0.0.1',10025))
	
	self.resetkeys();
	
  def runtests(self):	
	data =  self.bindsocket.recvfrom(1024)	
	return data	
  
  def loop(self):
	while True:
	  data =  self.bindsocket.recvfrom(1024)	  
	  self.protocol(data)
	
  def protocol(self, data):
		
	info = data[0].split("@")
	if info[0] == 'helo':
	  print 'ok'
	if info[0] == 'cert':
	  self.bindsocket.sendto(self.keys.publickey().exportKey(), data[1])
	  print 'cert ok'
	else:	  
	  print self.keys.decrypt(base64.b64decode(info[0]))
	  
	
if __name__ == "__main__":
  
  socket = 10025
  
  main = mainLoop(socket)  
  main.loop()
	
	
  

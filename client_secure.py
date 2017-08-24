#!/usr/bin/python
'''
  Client side part of the SSL secure UDP based program.
'''
import socket as so
from Crypto.PublicKey import RSA
import random as ran
import base64
from OpenSSL import SSL


class Main:
  
  name = ""
  socket = 0
  s = 0  
  key = 0
  
  def verify_cb(conn, cert, errnum, depth, ok):
    # This obviously has to be updated
    print 'Got certificate: %s' % cert.get_subject()
    return ok
  
  def __init__(self, socket, name):
	
	ctx = SSL.Context(SSL.TLSv1_METHOD)
	ctx.set_verify(SSL.VERIFY_PEER, self.verify_cb) # Demand a certificate
	ctx.use_privatekey_file ('./certs/server.key')
	ctx.use_certificate_file('./certs/server.crt')

	self.name = name
	self.socket = socket
	self.s = SSL.Connection(ctx, so.socket(so.AF_INET, so.SOCK_DGRAM))
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
	i = 0
	while i<1:
	  tosend = "data@" + str(ran.randint(0,100)) + "," + str(ran.randint(0,100))
	  tosend = base64.b64encode("".join(self.key.encrypt(tosend, 3)))
	  files = open("whatever", 'wb')
	  files.write(tosend)
	  files.close()
	  print tosend
	  self.sendthings(tosend)
	  i+=1
	  
	 #self.sendthings(self.key.encrypt(tosend,3))
	
if __name__ == "__main__":
  
  socket = 10025
  servername = "localhost"
  
  main = Main(socket, servername)
  main.runbegin()
  main.loopInfo()
  

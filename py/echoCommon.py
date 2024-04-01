#   ============================================================
#   common base class for echo server and client
#   ============================================================
#   created by Manfred Sorgo
import re
import socket
from getopt import getopt
from sys import argv
from logging import info as logout, basicConfig as logconfig, INFO 
from os.path import basename

class EchoCommon(object):
    def __init__(self, *args):
        self.verbose = False
        opts, args = getopt(args, 'vh')
        for o, v in opts:
            if o == '-v':
                self.verbose = True
            elif o == '-h':
                self.help()

        rx4 = re.compile(r'^\d+(?:\.\d+){3}$')
        rx6 = re.compile(r'^[0-9a-fA-F]*(?:\:[0-9a-fA-F]*){5}$')
        rxP = re.compile(r'^\d+$')
        rxN = re.compile(r'^(\d+)[xX]+$')
        rxT = re.compile(r'^(\d+)[tT]+$')
        self.prot = socket.AF_INET
        self.addr = '127.0.0.1'
        self.port = 8080
        self.loops = 1
        self.threads = 1
        self.message = 'hello world'    
        for arg in args:
            if rx4.match(arg):
                self.self.addr = arg
            elif rx6.match(arg):
                self.addr = arg
                self.prot = socket.AF_INET6
            elif rxP.match(arg):
                self.port = int(arg)
            elif rxN.match(arg):
                self.loops = int(rxN.match(arg).group(1))
            elif rxT.match(arg):
                self.threads = int(rxT.match(arg).group(1))
            else:
                self.message = arg

        logconfig(format="%(asctime)s %(message)s", level=INFO, datefmt="%H:%M:%S")
        
        if self.verbose:
            self.tell = self.log 
            print()
            print('addr:', self.addr)
            print('prot:', 'ipv6' if self.prot == socket.AF_INET6 else 'ipv4')
            print('port:', self.port)
        else:
            self.tell = self.silent

    def getsocket(self):
        return socket.socket(self.prot, socket.SOCK_STREAM)

    def log(self, *args):
        logout(' '.join([str(c) for c in args]))

    def silent(self, *args):
        pass

    def help(self):
        self.usage(basename(argv[0]))
        print(
"""options:
    -v  verbose
    -h  this help
syntax:                 | e.g.
    address             |
    - ipv4  n.n.n.n     | 1.2.3.4
    - ipv6  n:n:n:n:n:n | 1:2:3:4:5:6 
    port    nn..        | 8080
    loops   n..x        | 2x, 12X
    threads n..t        | 5T, 11t
    message any other   | "hello world"
"""            
        )
        exit()

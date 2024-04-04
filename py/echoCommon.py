#   ============================================================
#   common base class for echo server and client
#   ============================================================
#   created by Manfred Sorgo
import re
import socket
from sys import argv
from logging import info as logout, basicConfig as logconfig, INFO 
from os.path import basename

class EchoCommon(object):
    #   init with cli arguments regardless of order
    def __init__(self, *args):
        #   ipv4 address
        rx4 = re.compile(r'^\d+(?:\.\d+){3}$')
        #   ipv6 address
        rx6 = re.compile(r'^[0-9a-fA-F]*(?:\:[0-9a-fA-F]*){5}$')
        #   port
        rxP = re.compile(r'^\d+$')
        #   loops
        rxL = re.compile(r'^(\d+)[xX]+$')
        #   threads
        rxT = re.compile(r'^(\d+)[tT]+$')
        #   verbose
        rxV = re.compile(r'^-[vV]+$')
        #   help
        rxH = re.compile(r'^-[hH]+$')

        self.prot = socket.AF_INET
        self.addr = '127.0.0.1'
        self.port = 8080
        self.loops = 1
        self.threads = 1
        self.message = 'hello world'    
        verbose = False
        for arg in args:
            if rxH.match(arg):
                self.help()
            elif rx4.match(arg):
                self.self.addr = arg
            elif rx6.match(arg):
                self.addr = arg
                self.prot = socket.AF_INET6
            elif rxP.match(arg):
                self.port = int(arg)
            elif rxL.match(arg):
                self.loops = int(rxL.match(arg).group(1))
            elif rxT.match(arg):
                self.threads = int(rxT.match(arg).group(1))
            elif rxV.match(arg):
                verbose = True
            else:
                self.message = arg

        logconfig(format='%(asctime)s %(message)s', level=INFO, datefmt='%H:%M:%S')
        
        if verbose:
            self.tell = self.log 
            print()
            print('addr:', self.addr)
            print('prot:', 'ipv6' if self.prot == socket.AF_INET6 else 'ipv4')
            print('port:', self.port)
        else:
            self.tell = self.silent

    def getsocket(self) -> socket.socket:
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
    - ipv4  n.n.n.n     | 127.2.3.4
    - ipv6  b:b:b:b:b:b | 1:0F::4:5:6 
    port    1-5 digits  | 8080
    loops   digits + X  | 2x, 12X
    threads digits + T  | 5T, 11t
    message any other   | "hello world"
"""            
        )
        exit()

#   ============================================================
#   common functions for echo server and client
#   ============================================================
#   created by Manfred Sorgo
import re
import socket

def getSocketData(*args):
    rx4 = re.compile(r'^\d+(?:\.\d+){3}$')
    rx6 = re.compile(r'^[0-9a-fA-F]*(?:\:[0-9a-fA-F]*){5}$')
    rxP = re.compile(r'^\d+$')
    rxN = re.compile(r'^(\d+)[xX]+$')
    rxT = re.compile(r'^(\d+)[tT]+$')
    prot = socket.AF_INET
    addr = '127.0.0.1'
    port = 8080
    loops = 1
    threads = 1
    message = 'hello world'    
    for arg in args:
        if rx4.match(arg):
            addr = arg
        elif rx6.match(arg):
            addr = arg
            prot = socket.AF_INET6
        elif rxP.match(arg):
            port = int(arg)
        elif rxN.match(arg):
            loops = int(rxN.match(arg).group(1))
        elif rxT.match(arg):
            threads = int(rxT.match(arg).group(1))
        else:
            message = arg
    print()
    print('addr:', addr)
    print('prot:', 'ipv6' if prot == socket.AF_INET6 else 'ipv4')
    print('port:', port)

    return [socket.socket(prot, socket.SOCK_STREAM), addr, port, message, loops, threads, prot]
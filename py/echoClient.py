#   ============================================================
#   multi thread TCP echo client
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import getSocketData
from sys import argv
from threading import Thread
from logging import info as log, basicConfig as logconfig, INFO 
from socket import socket, SOCK_STREAM
from time import sleep

def getEchoes(*args):
    [addr, port, message, loops, threads, prot] = getSocketData(*args)[1:]
    logconfig(format="%(asctime)s %(message)s", level=INFO, datefmt="%H:%M:%S")
    for n in range(threads):
        Thread(target=tfunc, args=(n + 1, prot, addr, port, message, loops)).start()

def tfunc(nr:int, prot:int, addr:str, port:int, message:str, loops:int):
    try:
        log("%02d connecting ..." % nr)
        s = socket(prot, SOCK_STREAM)
        s.connect((addr, port))
        log("%02d connected." % nr)
        for n in range(loops):
                if n > 0: sleep(0.25)
                log("%02d %d / %d send: '%s'" % (nr, n + 1, loops, message))
                s.sendall(message.encode())
                data = s.recv(1024)
                log("%02d %d / %d recv: '%s'" % (nr, n + 1, loops, data.decode()))
        s.close()
    except Exception as e:
        log("%02d %s" % (nr, e))

if __name__ == '__main__':
    getEchoes(*argv[1:])


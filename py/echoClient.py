#   ============================================================
#   simple TCP echo client
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import getSocketData
from sys import argv
from time import sleep
from select import select

def getEcho(*args):
    [s, addr, port, message, num, *x] = getSocketData(*args)
    enc = message.encode()
    try:
        s.connect((addr, port))
        for n in range(num):
            if n > 0: sleep(1)
            print(f'send: "{message}"')
            s.sendall(enc)
            data = s.recv(1024)
            print(f'recv: "{data.decode()}"')
        s.close()
    except Exception as e:
        print(e)

if __name__ == '__main__':
    getEcho(*argv[1:])


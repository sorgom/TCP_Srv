#   ============================================================
#   simple TCP echo client
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import getSocketData
from sys import argv

def getEcho(*args):
    s, addr, port, message = getSocketData(*args)
    print(f'msg : "{message}"')
    try:
        s.connect((addr, port))
        s.sendall(message.encode())
        data = s.recv(1024)
        s.close()
        print(f'recv: "{data.decode()}"')
    except Exception as e:
        print(e)

if __name__ == '__main__':
    getEcho(*argv[1:])


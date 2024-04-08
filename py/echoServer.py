#   ============================================================
#   multi thread TCP echo server
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import EchoCommon
from select import select
from threading import Thread
from socket import socket

class EchoServer(EchoCommon):
    def __init__(self, *args):
        super().__init__(*args)

    def run(self):
        try:
            listenSocket = self.getsocket()
            listenSocket.bind((self.addr, self.port))
            listenSocket.listen()

            print('Press Ctrl+C to stop')
            while True:
                for n in range(20):
                    rd, *x = select([listenSocket], [], [], 0.05)
                    if rd:
                        Thread(target=self.tfunc, args=(listenSocket,)).start()
                            
        except Exception as e:
            listenSocket.close()
            self.log(e)

    def tfunc(self, listenSocket:socket):
        try:
            clientSocket, *x = listenSocket.accept()
            with clientSocket:
                addr, port = clientSocket.getpeername()
                self.tell('CON', addr, port)
                while True:
                    data = clientSocket.recv(1024)
                    if not data: break
                    self.tell(f'{port} < {data.decode()} >')
                    clientSocket.sendall(data)
                clientSocket.close()
                self.tell('EX', port)
        except Exception as e:
            self.log('tfunc:', e)

if __name__ == '__main__':
    from sys import argv
    srv = EchoServer(*argv[1:])
    srv.run()

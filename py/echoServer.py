#   ============================================================
#   simple TCP echo server
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import EchoCommon
from select import select

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
                        conn, *x = listenSocket.accept()
                        with conn:
                            while True:
                                data = conn.recv(1024)
                                if data:
                                    self.tell(f'< {data.decode()} >')
                                else:
                                    break
                                conn.sendall(data)    
        except Exception as e:
            listenSocket.close()
            self.log(e)

    def usage(self, name):
        print(f'Usage: {name} [options] [address] [port]')

if __name__ == '__main__':
    from sys import argv
    srv = EchoServer(*argv[1:])
    srv.run()

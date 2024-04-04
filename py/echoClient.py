#   ============================================================
#   multi thread TCP echo client
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import EchoCommon
from threading import Thread
from time import sleep

class EchoClient(EchoCommon):
    def __init__(self, *args):
        super().__init__(*args)

    def usage(self, name):
        print(f'Usage: {name} [options] [address] [port] [message] [loops] [threads]')
        
    def run(self):
         for n in range(self.threads):
             Thread(target=self.tfunc, args=(n + 1,)).start()
    
    def tfunc(self, nr:int):
        try:
            s = self.getsocket()
            s.connect((self.addr, self.port))
            self.tell('%02d CON' % nr)
            for n in range(self.loops):
                if n > 0: sleep(0.25)
                self.tell('%02d %d / %d send: %s' % (nr, n + 1, self.loops, self.getmessage()))
                s.sendall(self.getdata())
                s.settimeout(None)
                rdata = bytes()
                data = s.recv(1024)
                try:
                    while len(data) > 0:
                        rdata += data
                        s.settimeout(0.125)
                        data = s.recv(1024)
                except Exception:
                    pass
                finally:
                    if len(rdata) > 0:
                        self.tell('%02d %d / %d recv: %s' % (nr, n + 1, self.loops, self.decode(rdata)))
            s.close()
            self.tell('%02d EX' % nr)
        except Exception as e:
            self.log('%02d %s' % (nr, e))

    def decode(self, data:bytes) -> str:
        return data.decode()

    def getmessage(self) -> str:
        return self.message

    def getdata(self) -> bytes:
        return self.message.encode()


if __name__ == '__main__':
    from sys import argv
    cl = EchoClient(*argv[1:])
    cl.run()


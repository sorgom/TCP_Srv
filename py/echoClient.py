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
            self.tell("%02d connecting ..." % nr)
            s = self.getsocket()
            s.connect((self.addr, self.port))
            self.tell("%02d connected" % nr)
            for n in range(self.loops):
                if n > 0: sleep(0.25)
                self.tell("%02d %d / %d send: '%s'" % (nr, n + 1, self.loops, self.message))
                s.sendall(self.message.encode())
                s.settimeout(None)
                answer = ''
                data = s.recv(1024)
                print('data:', len(data))
                try:
                    while len(data) > 0:
                        answer += data.decode()
                        print(answer)
                        s.settimeout(0.125)
                        data = s.recv(1024)
                except Exception as t:
                    pass
                finally:
                    self.tell("%02d %d / %d recv: '%s'" % (nr, n + 1, self.loops, answer))
            self.tell("%02d closing" % nr)
            s.close()
        except Exception as e:
            self.log("%02d %s" % (nr, e))

if __name__ == '__main__':
    from sys import argv
    cl = EchoClient(*argv[1:])
    cl.run()


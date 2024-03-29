#   ============================================================
#   simple TCP echo server
#   ============================================================
#   created by Manfred Sorgo

from echoCommon import getSocketData
from sys import argv
from select import select
import signal
from os.path import basename
from getopt import getopt

class EchoServer(object):
    def __init__(self, *args):
        self.verbose = False
        opts, args = getopt(args, 'vh')
        for o, v in opts:
            if o == '-v':
                self.verbose = True
            elif o == '-h':
                self.help()

        for sig in ('TERM', 'INT'):
            signal.signal(getattr(signal, 'SIG' + sig), lambda *any: self.done())

        self.sock, self.addr, self.port, *x = getSocketData(*args)

    def run(self):
        try:
            self.sock.bind((self.addr, self.port))
            self.sock.listen()

            print('Press Ctrl+C to stop')
            while True:
                if self.verbose: print('.', end='', flush=True)
                for n in range(20):
                    rd, *x = select([self.sock], [], [], 0.05)
                    if rd:
                        conn, *x = self.sock.accept()
                        with conn:
                            while True:
                                data = conn.recv(1024)
                                if not data:
                                    break
                                conn.sendall(data)    
        except Exception as e:
            self.sock.close()
            print(e)

    def done(*any):
        print()
        print('done')
        exit()

    def help(self):
        print(f'Usage: {basename(argv[0])} [options] [addr] [port]')
        print("""options:
    -v  verbose
    -h  this help""")
        exit()

if __name__ == '__main__':
    srv = EchoServer(*argv[1:])
    srv.run()


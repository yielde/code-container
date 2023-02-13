import os
import sys
import signal
from mylog.log import MYLOG
from threading import Event

logger = MYLOG.get_logger(__file__)


class MyDamon:
    def __init__(self, pidfile, stdin='/dev/null', stdout='/dev/null', stderr='/dev/null', kill_timeout=90):
        self.stdin = stdin
        self.stdout = stdout
        self.stderr = stderr
        self._kill_timeout = kill_timeout
        self.pidfile = pidfile
        self.loop_init = True
        self._daemon_stopped = Event()

    def damonize(self):
        try:
            pid = os.fork()
            if pid > 0:
                sys.exit(0)
        except OSError as e:
            sys.stderr.write('The first fork failed: %d (%s)\n' %
                             (e.errno, e.strerror))
            sys.exit(1)

        # 脱离原先的控制终端，成为会话首领
        os.setsid()

        # 与父进程环境分离
        os.chdir('/')
        os.umask(0)

        # 防止无控制终端的会话首领再次获取控制终端
        try:
            pid = os.fork()
            if pid > 0:
                sys.exit(0)
        except OSError as e:
            sys.stderr.write('The second fork failed: %d (%s)\n' %
                             (e.errno, e.strerror))
            sys.exit(1)

        sys.stdout.flush()
        sys.stderr.flush()

        # 避免0、1、2描述符I/O失败导致daemon corrupt
        stdin = open(self.stdin, 'r')
        stdout = open(self.stdout, 'a+')
        stderr = open(self.stderr, 'a+b', 0)
        os.dup2(stdin.fileno(), sys.stdin.fileno())
        os.dup2(stdout.fileno(), sys.stdout.fileno())
        os.dup2(stderr.fileno(), sys.stderr.fileno())

    def clean_pid_file(self):
        os.remove(self.pidfile)

    def handle_terminate(self, sig):
        logger.info('received signal {}'.format(sig))

    def daemon_running(self, wait=0):
        if self.loop_init and wait > 0:
            wait = 0
            self.loop_init = False
        return not self._daemon_stopped(wait)

    def start(self):
        self.pre_start()
        self.damonize()
        signal.signal(signal.SIGTERM)

    def pre_start(self):
        '''
        prepare for start daemon
        '''

    def pre_stop(self):
        '''
        prepare for stop daemon
        '''

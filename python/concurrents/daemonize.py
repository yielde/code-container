# -*- coding:utf-8 -*-
import os
import sys
import signal
import logging
import time
import atexit
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
        atexit.register(self.clean_pid_file)
        pid = str(os.getpid())
        with open(self.pidfile, 'w+') as f:
            f.write('%s\n' % pid)

    def clean_pid_file(self):
        os.remove(self.pidfile)

    def handle_terminate(self, sig, frame):
        logger.info('received signal {}, frame: {}'.format(sig, frame))
        if self.daemon_running():
            self._daemon_stopped.set()
            self.terminate()

    def handle_siguser1(self, sig, frame):
        logger.info('received signal {}, frame: {}'.format(sig, frame))
        self.siguser1()

    def handle_siguser2(self, sig, frame):
        logger.info('received signal {}, frame: {}'.format(sig, frame))
        self.siguser2()

    def daemon_running(self, wait=0):
        if self.loop_init and wait > 0:
            wait = 0
            self.loop_init = False
        return not self._daemon_stopped.wait(wait)

    def get_pid(self):
        try:
            with open(self.pidfile, 'r') as f:
                pid = f.read().strip()
            pid = int(pid) if len(pid) > 0 else None
        except IOError:
            pid = None
        return pid

    def start(self):
        self.damonize()
        signal.signal(signal.SIGTERM, self.handle_terminate)
        signal.signal(signal.SIGUSR1, self.handle_siguser1)
        signal.signal(signal.SIGUSR2, self.handle_siguser2)
        daemon_name = self.__class__.__name__
        self.pre_start()
        try:
            logger.info('Daemon {} start'.format(daemon_name))
            self.run()
            logger.info('Daemon {} stopped'.format(daemon_name))
        except Exception as e:
            logger.exception(
                'Daemon {} terminates, Unexpected errors {}'.format(daemon_name, str(e)))

    def stop(self):
        pid = self.get_pid()

        if not pid:
            sys.stderr.write(
                "pidfile {} does not exist, is daemon running?\n".format(pid))
            return

        timer = 0
        try:
            while True:
                if timer == 0:
                    os.kill(pid, signal.SIGTERM)
                elif timer < self._kill_timeout:
                    os.kill(pid, 0)
                else:
                    os.killpg(os.getpgid(pid), signal.SIGKILL)
                time.sleep(1)
                timer += 1
        except OSError as e:
            if str(e).find("No such process") > 0:
                if os.path.exists(self.pidfile):
                    os.remove(self.pidfile)
            else:
                print(str(e))
                sys.exit(1)

    def restart(self):
        self.stop()
        self.start()

    def run(self):
        '''
        can be overridden.
        your loop function, use self.daemon_running(loop_second) for while loop
        '''

    def siguser1(self):
        '''
        can be overridden.
        use `kill SIGUSR1 pid_number` to use your sig handler, 
        default is changing log level between info and debug
        '''
        new_level = logging.INFO if MYLOG.getLevel() == logging.DEBUG else logging.DEBUG
        MYLOG.setLevel(new_level)
        logger.info('Daemon {} log level is changed to {}'.format(
            self.__class__.__name__, 'debug' if new_level == logging.DEBUG else 'info'))

    def siguser2(self):
        '''
        can be overridden.
        use `kill SIGUSR2 pid_number` to use your sig handler
        '''

    def terminate(self):
        '''
        can be overridden.
        the work after SIGTERM received
        '''

    def pre_start(self):
        '''
        can be overridden.
        prepare for start daemon
        '''

    def pre_stop(self):
        '''
        can be overridden.
        prepare for stop daemon
        '''

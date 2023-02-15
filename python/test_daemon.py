from concurrents.daemonize import MyDamon
from mylog.log import MYLOG
import time
import sys

MYLOG.init_handler(file='./daemon_test.log')
logger = MYLOG.get_logger(__file__)


class TEST_C1(MyDamon):
    def pre_start(self):
        logger.info('test c1: prepare for starting Daemon'.format(
            self.__class__.__name__))

    def pre_stop(self):
        logger.info('test c1: prepare for stopping Daemon'.format(
            self.__class__.__name__))

    def siguser2(self):
        logger.info('test c1: starting SIGUSR2 function')
        time.sleep(10)
        logger.info('test c1: ending SIGUSR2 function')

    def terminate(self):
        logger.info('test c1: i clean something before daemon stopped')
        time.sleep(5)
        logger.info('test c1: ending clean')

    def run(self):
        count = 0
        while self.daemon_running(wait=5):
            count += 1
            logger.info('test c1: daemon run {} time'.format(count))
            logger.debug('test c1: i am a debug info')


class TEST_SINGLE_THREAD_LOOP(MyDamon):
    pass


if __name__ == '__main__':
    daemon = TEST_C1('/var/run/my-test-c1.pid', kill_timeout=30)
    if len(sys.argv) == 2:
        if sys.argv[1] == 'start':
            daemon.start()
        elif sys.argv[1] == 'stop':
            daemon.stop()
        elif sys.argv[1] == 'restart':
            daemon.restart()
        else:
            print('Error command')
            sys.exit(1)
    else:
        print('usage:{} <start|stop|restart>'.format(sys.argv[0]))

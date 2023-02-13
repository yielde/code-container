'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-08 12:56:27
LastEditTime: 2023-02-05 17:24:17
Description: 
'''
import threading
from mylog.log import MYLOG

logger = MYLOG.get_logger(__name__)


class MyLoopThread(threading.Thread):
    '''
    需要一个线程周期性的执行某些任务，例如在daemon中产生线程分别更新网卡信息、压缩日志、更新配置文件等
    '''

    def __init__(self, interval, name=None, func=None):
        super(MyLoopThread, self).__init__()
        self.interval = interval
        self.name = name if name else self.__class__.__name__
        self.func = func
        self.stop_flag = threading.Event()

    def run(self):
        logger.info("Start thread {}, loop interval {}".format(
            self.name, self.interval))

        self.pre_start()
        while not self.stop_flag.wait(self.interval):
            self.run_loop()
        logger.info("{} stopped".format(self.name))
        self.post_stop()

    def stop(self):
        '''
        停止loop线程
        '''
        self.pre_stop()
        self.stop_flag.set()

    def run_loop(self):
        if self.func:
            self.func()

    def pre_start(self):
        logger.info("Prepare for starting thread")
        '''
        Prepare for starting thread
        '''

    def pre_stop(self):
        logger.info("Prepare for stopping thread")
        '''
        Prepare for stopping thread
        '''

    def post_stop(self):
        '''
        Process after the thread has stopped
        '''
'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-02-03 17:55:47
LastEditTime: 2023-02-03 18:12:03
Description: 
'''
import time
import logging
from mylog.log import MYLOG
from concurrents.threadpool import MyLoopThread


MYLOG.init_handler(level=logging.DEBUG)

logger = MYLOG.get_logger(__name__)


def test():
    logger.info("Hello world!")


if __name__ == '__main__':
    loopt = MyLoopThread(5, name="loopthread1", func=test)
    loopt.start()
    while True:
        print("gggggg")
        time.sleep(1)
'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-10 16:45:55
LastEditTime: 2023-02-03 17:53:04
Description: 
'''
from concurrents.threadpool import MyLoopThread
from mylog.log import MYLOG
import logging

MYLOG.init_handler(level=logging.INFO)

def test():
    print("hello world!")
thread = MyLoopThread(5, name="myloop", func=test)
thread.start()

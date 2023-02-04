'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-13 15:26:16
LastEditTime: 2023-01-13 18:07:22
Description: 
'''
import time
from mylog.log import MYLOG

logger = MYLOG.get_logger(__file__)


def test1():
    for i in range(10):
        logger.info('hello world %d', i)
        time.sleep(2)

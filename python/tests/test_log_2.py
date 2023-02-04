'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-13 15:27:10
LastEditTime: 2023-01-13 18:37:11
Description: 
'''
from mylog.log import MYLOG
logger = MYLOG.get_logger("test_log_2.py")


def test2():
    for i in range(5):
        logger.warning("im logger 2: %d", i)

def test3():
    for i in range(3):
        logger.debug("im logger 3: %d", i)
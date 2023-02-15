'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-13 16:09:24
LastEditTime: 2023-01-13 18:37:37
Description:
'''
import os
import sys
from tests.test_log_1 import test1
from tests.test_log_2 import test2, test3
from mylog.log import MYLOG, FacilitySelector
from concurrent.futures import ThreadPoolExecutor

MYLOG().init_handler(file="/home/galen/workspace/code-container/python/test.log")
logger = MYLOG.get_logger(__file__)


def test1():
    logger.info("hello im main")
    with ThreadPoolExecutor(max_workers=6, thread_name_prefix='TEST_THREAD') as executor:
        executor.submit(test1)
        executor.submit(test2)
        with MYLOG.enable_debug_log():
            executor.submit(test3)


def test3():
    logger.info("test2")
    stdin = open('/dev/null', 'r')
    stdout = open('/dev/null', 'a+')
    stderr = open('/dev/null', 'a+b', 0)
    os.dup2(stdin.fileno(), sys.stdin.fileno())
    os.dup2(stdout.fileno(), sys.stdout.fileno())
    os.dup2(stderr.fileno(), sys.stderr.fileno())
    test3_1(1)


def test3_1(a):
    logger.info(a)


if __name__ == '__main__':
    test3()

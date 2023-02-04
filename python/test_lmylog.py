'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-13 16:09:24
LastEditTime: 2023-01-13 18:37:37
Description: 
'''
from tests.test_log_1 import test1
from tests.test_log_2 import test2, test3
from mylog.log import MYLOG, FacilitySelector
from concurrent.futures import ThreadPoolExecutor

MYLOG().init_handler(file="/home/galen/workspace/code-container/python/test.log")
logger = MYLOG.get_logger(__file__)

if __name__ == '__main__':
    logger.info("hello im main")
    with ThreadPoolExecutor(max_workers=6, thread_name_prefix='TEST_THREAD') as executor:
        executor.submit(test1)
        executor.submit(test2)
        with MYLOG.enable_debug_log():
            executor.submit(test3)

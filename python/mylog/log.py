# -*- coding:utf-8 -*-
'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-10 20:15:23
LastEditTime: 2023-01-13 18:06:46
Description:
'''
import logging
import sys
import os
from logging.handlers import SysLogHandler, WatchedFileHandler
from logging import StreamHandler
from threading import Lock
from utils import utils
from contextlib import contextmanager


class FacilitySelector:
    '''
    下发facility, 配合rsyslog使用
    '''
    SYSLOG = (SysLogHandler.LOG_USER,
              '/var/log/{}'.format(utils.get_syslog_file_name_by_distro()))
    MYAPILOG = (SysLogHandler.LOG_LOCAL0, '/var/log/myapi.log')


class MYLOG:
    _handler = None
    _modules = []  # 未初始化的模块
    _initial_level = logging.INFO
    _temp_debug_counter = 0
    _temp_debug_lock = Lock()

    @classmethod
    def init_handler(cls, level=logging.INFO, to_syslog=False, syslog_file=FacilitySelector.SYSLOG, file=None, fmt=None):
        if cls._handler:
            raise Exception('Handler initialized twice')

        if not fmt:
            fmt = '[%(asctime)s] [%(levelname)s] ' \
                '| [%(process)d] [%(name)s] ' \
                '| [%(thread)d] [%(threadName)s] | %(message)s'
        formatter = logging.Formatter(fmt)
        if to_syslog:
            # 1. /dev/log为本地logfile
            # 2. 也可以使用(ip, port)写到远端服务器
            cls._handler = SysLogHandler(
                address='/dev/log', facility=syslog_file[0])
        elif file:
            log_dir_path = os.path.dirname(file)
            if not os.path.exists(log_dir_path):
                os.makedirs(log_dir_path)
            cls._handler = WatchedFileHandler(file, encoding='UTF-8')
        else:
            cls._handler = StreamHandler(sys.stdout)

        cls._handler.setFormatter(formatter)
        cls._handler.setLevel(level)

        cls._initial_level = level
        for module in cls._modules:
            logger = logging.getLogger(module)
            if cls._handler not in logger.handlers:
                logger.addHandler(cls._handler)

    @classmethod
    def get_logger(cls, module):
        logger = logging.getLogger(module)
        logger.setLevel(logging.DEBUG)

        if cls._handler is None:
            cls._modules.append(module)
        elif cls._handler not in logger.handlers:
            logger.addHandler(cls._handler)
        return logger

    @classmethod
    def setLevel(cls, level):
        cls._initial_level = level
        cls._handler.setLevel(level)

    @classmethod
    def getLevel(cls):
        return cls._handler.level

    @classmethod
    def set_temp_level(cls, level):
        cls._handler.setLevel(level)

    @classmethod
    def get_initial_level(cls):
        return cls._initial_level

    # 通过with将在此函数上下文中的代码日志级别临时提高为debug
    @classmethod
    @contextmanager
    def enable_debug_log(cls):
        with cls._temp_debug_lock:
            cls._temp_debug_counter += 1
        old_level = cls.get_initial_level()
        cls.set_temp_level(logging.DEBUG)
        try:
            yield
        finally:
            with cls._temp_debug_lock:
                cls._temp_debug_counter -= 1
                if cls._temp_debug_counter == 0:
                    cls.set_temp_level(old_level)

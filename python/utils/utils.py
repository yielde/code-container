'''
Author: Hui Tong <937013596@qq.com>
Date: 2023-01-11 11:48:22
LastEditTime: 2023-01-13 18:07:11
Description: 
'''
import platform


def get_system_distro():
    # ubuntu与centos
    system_distro = platform.linux_distribution()
    if 'ubuntu' in system_distro[0].lower():
        return 'ubuntu'
    elif 'centos' in system_distro[0].lower():
        return 'centos'
    else:
        raise NameError('Unknow system distro: {}'.format(system_distro[0]))


def get_syslog_file_name_by_distro():
    distro = get_system_distro()
    if distro == "ubuntu":
        return "syslog"
    elif distro == "centos":
        return "messages"

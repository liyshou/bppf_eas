#-*- coding: utf-8 -*-
"""
ClassName:  None
Author:     YanJinMu
Date:       2015-08-18
Description:defined utile information and variables
"""

import platform




def SYS_WINDOW():
    return platform.system() == 'Windows'

def SYS_LINUX():
    return platform.system() == 'Linux'

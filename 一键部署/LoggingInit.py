#/usr/bin/python
#-*- coding: utf-8 -*-

import logging,logging.config


#读取logging日志模块的配置文件
logging.config.fileConfig("/app/bppf_omp/eas_script/AutoCompileLogging.conf")

LOGDebug = logging.getLogger('handler_hander_debug')
LOGError = logging.getLogger('handler_hander_error')
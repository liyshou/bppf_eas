#/usr/bin/python
#-*- coding: utf-8 -*-

import logging,logging.config



logging.config.fileConfig("logCrawlerLogging.conf")

LOGDebug = logging.getLogger('handler_hander_debug')
LOGError = logging.getLogger('handler_hander_error')


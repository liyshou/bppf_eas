#/usr/bin/python
#-*- coding: utf-8 -*-

import logging,logging.config

_FileLog_ = True

logging.config.fileConfig("public/logCrawlerLogging.conf")


LOGDebug = logging.getLogger('handler_hander_debug')
LOGError = logging.getLogger('handler_hander_error')



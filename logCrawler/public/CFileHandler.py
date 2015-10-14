#-*- coding: utf-8 -*-
"""
ClassName:  CFileHandler
Author:     YanJinMu
Date:       2015-08-14
Description:define all operation-interface about file
"""

import os
import re
from public.LoggingInit import LOGDebug,LOGError

MATCH_RULE = r'(?<=[\[])(?:[^[\]]|\[[^[\]]*\])*(?=[\]])'
MATCH_SESSIONID = r'(?<=\[)[0-9]{10}(?=\])'


class CFileHandler(object):
    def __init__(self):
        self.__is_section = False
        self.__str_line = ""
        self.__str_list = []
        self.__str_keywords = ""
        self.__str_lastline = ''

    def __Execute(self, str_cmd):
        print(str_cmd)
        ret = os.popen(str_cmd)
        return ret

    def __FilterKeyword(self, str_keyword, str_file_name):
        str_cmd = "grep -E \'%s\' %s"%(self.__str_keywords, str_file_name)
        ret = self.__Execute(str_cmd)
        return ret

    def __MultiFilterKeyword(self, str_list_keywords, str_file_name):
        str_keys = "\'"
        for key in str_list_keywords:
            str_keys = "%s|%s"%(str_keys, key)
        str_keys += "\'"
        str_cmd = "grep -E %s %s"%(str_keys, str_file_name)
        ret = self.__Execute(str_cmd)
        return ret

    def __Open(self, str_file_name):
        if self.__str_keywords == '':
            p_file = open(str_file_name, 'r')
        else:
            p_file = self.__FilterKeyword(self.__str_keywords, str_file_name)   # 关键词过滤
            pattern = re.compile(MATCH_RULE)            # 设置匹配模式
            str_dict = {}
            for line in p_file.readlines():             # 获取sessionID 及 文件名
                str_list = pattern.findall(line)
                int_len = len(str_list)
                if int_len > 0:
                    str_dict[str_list[2]] = str_file_name

            p_file.close()
            if len(str_dict) > 0:
                p_file = self.__MultiFilterKeyword(list(dict), str_file_name)
            else:
                p_file = None
        return p_file

    def FileOpen(self, str_fileName):
        if(str_fileName == ""):
            return -1
        try:
            self.__str_file = self.__Open(str_fileName)
            if self.__str_file == None:
                return -1
        except:
            LOGError.error("Open file faild!")
            return -1
        return 0

    def SetKeywordFilter(self, str_keyword):
        self.__str_keywords = str_keyword

    def ReadSection(self):
        self.__str_list = []
        is_eof = False
        cur_session_id = ''
        try:
            pattern = re.compile(MATCH_SESSIONID)
            while (not is_eof):

                if self.__str_lastline == '':
                    self.__str_line = self.__str_file.readline()
                else:
                    self.__str_line = self.__str_lastline
                    self.__str_lastline = ''

                #print("Read:len=%s"%self.__str_line)

                if self.__str_line == '':           ### 判断文件是否结束
                    LOGDebug.debug("Reach end of file.")
                    is_eof = True
                    continue

                session_id_list = pattern.findall( self.__str_line)
                if len(session_id_list) > 0:        # 找到session_id
                    if session_id_list[0] != cur_session_id:
                        if cur_session_id == '':
                            cur_session_id = session_id_list[0]
                        else:
                            self.__str_lastline = self.__str_line
                            break
                    self.__str_list.append(self.__str_line)

        except EOFError:
            LOGError.error("End of File Error!")
        except:
            LOGError.error("Read file faild!")
            self.__str_file.close()
            return []
        return self.__str_list

    def FileClose(self):
        try:
            self.__str_file.close()
        except:
            LOGError.error("Close file faild!")





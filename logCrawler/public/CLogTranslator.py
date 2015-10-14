#-*- coding: utf-8 -*-
"""
ClassName:  CLogTranslator
Author:     YanJinMu
Date:       2015-08-14
Description:translate string list to dictionary list
"""

from public.LoggingInit import LOGDebug,LOGError
import re

MATCH_RULE = r'(?<=[\[])(?:[^[\]]|\[[^[\]]*\])*(?=[\]])'

# 报文字段定义
HeaderKeys = ["date", "level", "sessionId", "pid", "service"]
MsgKeys = ["date", "level", "sessionId", "pid", "file_func", "line", "msg"]


class CLogTranslator(object):
    def __init__(self):
        self.__str_head_keys = HeaderKeys[:]
        self.__str_keys = MsgKeys[:]
        self.__str_data = []
        self.__str_dict_set = []
        self.__str_dict = {}

    def SetHeadKeys(self, str_list_keys):
        self.__str_head_keys = str_list_keys[:]

    def SetKeys(self, str_list_keys):
        self.__str_keys = str_list_keys[:]

    def SetLogInfo(self, str_list_data):
        self.__str_data = str_list_data[:]

    def DeleteBrack(self, str_data):
        if str_data is None:
            return ''
        str_data = str_data.strip()
        if str_data.startswith('[') and str_data.endswith(']'):
            str_data = str_data[1:len(str_data) - 1]
        return str_data

    def __Translate(self):
        self.__str_dict_set = []
        pattern = re.compile(MATCH_RULE)
        for str_line in self.__str_data:
            self.__str_dict = {}                            # clear dict data
            str_list = pattern.findall(str_line)

            int_len = len(str_list)
            if int_len > 0:
                int_head_len = len(self.__str_head_keys)
                int_msg_len = len(self.__str_keys)
                if int_head_len == int_len:
                    for i in range(int_head_len):
                        self.__str_dict[self.__str_head_keys[i]] = str_list[i]
                elif int_msg_len == int_len:
                    for i in range(int_msg_len):
                        self.__str_dict[self.__str_keys[i]] = str_list[i]
                else:
                    for i in range(6):
                        self.__str_dict[self.__str_keys[i]] = str_list[i]
                    self.__str_dict[self.__str_keys[6]] = self.DeleteBrack(str_line[str_line.find(str_list[5]) + len(str_list[5]) + 1:])

                if len(self.__str_dict) > 0:
                    self.__str_dict_set.append(self.__str_dict)

        return          # End of __Translate()


    def GetTransData(self):
        self.__Translate()
        return self.__str_dict_set

#-*- coding: utf-8 -*-
"""
ClassName:  COperFactory && COperCondiction
Author:     YanJinMu
Date:       2015-08-16
Description:operation factory for all COperCondiction
"""

from abc import ABCMeta,abstractmethod

TABLE_UNITE_LOGS = ["sessionId", "req_datagram", "resp_datagram", "mac_ip", "service", "begin_date", "end_date", "pid", "level_id", "file_name", "stat", "mark1", "mark2", "used_time", "batch_seqid", "id"]

class COperBase(object):
    __metaclass__ = ABCMeta

    def __init__(self):
        self.str_dict_cond = []

    def SetCondition(self, str_dict_cond):
        self.str_dict_cond = str_dict_cond[:]

    @abstractmethod
    def Operate(self):
        pass

    def GetCondition(self):
        return self.str_dict_cond


class COperToUniteLogs(COperBase):
    def __init__(self):
        super(COperToUniteLogs, self).__init__()
        self.__table_list = []

    def __Service(self, str_service):       # 获取服务名
        return str_service[0:7]

    def __IsRequst(self, str_service):      # 判断是请求还是响应报文
        if str_service[7] == '0':
            return True
        else:
            return False

    def __ToDate(self, str_time):
        str_list = str_time.split()
        return '%s %s'%(str_list[0], str_list[1])

    def __ToLevel(self, str_level):
        return 0

    def __ToTime(self, str_date):
        str_list = str_date.split()
        if len(str_list) < 2:
            print str_list
            return 0
        str_time = str_list[1].split(':')
        return (int(str_time[0]) * 3600 + int(str_time[1]) * 60 + int(str_time[2]))

    def __CalcTimes(self, str_begin_date, str_end_date):
        try:
            t1 = self.__ToTime(str_begin_date)
            t2 = self.__ToTime(str_end_date)
        except IndexError:
            t1 = None
            t2 = None

        if t1 is None or t2 is None:
            return 0
        else:
            return int(t2 - t1)

    def __GetOneRecord(self, index, max_len):

        record_dict = {'SEQ_ID': '', 'SEQ_REQU': '', 'SEQ_RESP': '','MAC_IP': '', 'SERVICE_NAME': '', 'BEGIN_DATE': '', 'END_DATE': ''
                , 'TMSERVICE_PID': '', 'LEVEL_ID': '', 'FILE_NAME': '', 'STAT': '', 'MARK1': '', 'MARK2': '', 'USED_TIME': 0.0, 'BATCH_SEQID': ''}
        msgtype_flag = 0          # 报文类型标识， 0： reqeust, 1:respond

        #for index in range(len(self.str_dict_cond)):
        while index < max_len:
            str_dict = self.str_dict_cond[index]
            if len(str_dict) == 5:
                # head
                if self.__IsRequst(str_dict.get('service')):
                    #request
                    if record_dict.get('SERVICE_NAME') == '':
                        msgtype_flag = 0                                                        # 标记后续报文为请求request 报文
                        record_dict['SEG_ID'] = str_dict.get('sessionId')
                        record_dict['SERVICE_NAME'] = self.__Service(str_dict.get('service'))
                        record_dict['BEGIN_DATE'] = self.__ToDate(str_dict.get('date'))
                        record_dict['TMSERVICE_PID'] = str_dict.get('pid')
                        record_dict['LEVEL_ID'] = self.__ToLevel(str_dict.get('level'))

                    elif record_dict['SERVICE_NAME'] != str_dict['service']:
                        index = self.__GetOneRecord(index, max_len)                             # 更新当前列表解析进度index

                    else:
                        # print error
                        pass

                else:
                    #respond
                    msgtype_flag = 1                                                            # 标记后续报文为请求respond 报文

            elif len(str_dict) >= 7:
                #msg
                str_msg = str_dict.get('msg')
                if str_msg != None and str_msg.startswith('FFFF'):
                    if msgtype_flag == 0:
                        record_dict['SEQ_REQU'] = str_msg.decode('unicode_escape')   # str_msg.encode("utf-8")gbk  str_msg.decode('gbk', 'ignore')
                    else:
                        record_dict['SEQ_RESP'] = str_msg.decode('unicode_escape')    #unicode(str_msg, 'GB2312') str_msg.decode('gb2312')
                        record_dict['END_DATE'] = self.__ToDate(str_dict.get('date'))
                        record_dict['USED_TIME'] = self.__CalcTimes(record_dict.get('BEGIN_DATE'), record_dict.get('END_DATE'))
                        self.__table_list.append(record_dict)                                       # 将子典结果存进子典列表
                        return index                                                               # 会话解析完成，返回
            else:
                # done nothing
                pass
            index += 1

        else:
            # end of while
            pass


        return index


    def Operate(self):
        index = 0
        max_len = len(self.str_dict_cond)
        self.__table_list = []
        while index < max_len:
            index = self.__GetOneRecord(index, max_len)      # 递归筛选，获取数据
        if len(self.__table_list) > 0:
            self.str_dict_cond = self.__table_list[:]
        else:
            self.str_dict_cond = []
    #end of Opereate()


class COperFactory(object):
    def __init__(self):
        self.__str_dict_cond = []
        self.__obj_cond_list = []

    def Append(self, obj_oper_cond):
        self.__obj_cond_list.append(obj_oper_cond)

    def Clean(self):
        self.__obj_cond_list = []

    def SetCond(self, str_dict_cond):
        self.__str_dict_cond = str_dict_cond[:]

    def GetCond(self):
        for obj_cond in self.__obj_cond_list:
            obj_cond.SetCondition(self.__str_dict_cond)
            obj_cond.Operate()
            self.__str_dict_cond = obj_cond.GetCondition()
        return self.__str_dict_cond

    # End of GetCond



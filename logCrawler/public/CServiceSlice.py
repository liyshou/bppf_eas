#-*- coding: utf-8 -*-
"""
ClassName:  CServiceSliceUnite & CServiceSlice
Author:     YanJinMu
Date:       2015-08-20
Description:The CServiceSliceUnite class is the  data struct object of Table named colServiceSlice.
            The CServiceSlice class defined some operation interface for CServiceSliceUnite.
"""

import threading
from public.LoggingInit import LOGDebug,LOGError

SliceUniteParam = {'Type': 0, 'SERVICE_NAME': '', 'TimeSlice':'', 'TimeLen':0, 'STAT':'', 'BATCH_SEQID':'', 'MAC_IP':''}      # type=0, request datagram; type = 1, respond datagram
ColServiceSlice = {'SERVICE_NAME':'', 'TimeSlice':'', 'RequestCount':0, 'ResponseCount':0, 'STAT':'', 'BATCH_SEQID':'', 'MAC_IP':'', 'REQ_AVG':0, 'REQ_MAX': 0.0, 'REQ_MIN': -1}

class CServiceSliceUnite(object):
    def __init__(self, dict_slice_param):
        assert len(dict_slice_param) == len(SliceUniteParam)          # judge whether input parament is right

        self.__ServiceSlice = dict(ColServiceSlice)
        self.__SumTimes = 0
        self.__mutex = threading.Lock()
        self.__SetValue(dict_slice_param)

    def __SetValue(self, dict_slice_param):
        self.__ServiceSlice['SERVICE_NAME'] = dict_slice_param.get('SERVICE_NAME');
        self.__ServiceSlice['TimeSlice'] = dict_slice_param.get('TimeSlice')
        self.__ServiceSlice['STAT'] = dict_slice_param.get('STAT')
        self.__ServiceSlice['BATCH_SEQID'] = dict_slice_param.get('BATCH_SEQID')
        self.__ServiceSlice['MAC_IP'] = dict_slice_param.get('MAC_IP')

        if dict_slice_param.get('Type') == 0:
            #request
            self.__ServiceSlice['RequestCount'] = 1
            int_time = dict_slice_param.get('TimeLen')
            self.__SumTimes += int_time
            self.__ServiceSlice['REQ_MAX'] = max(self.__ServiceSlice.get('REQ_MAX'), int_time)
            if self.__ServiceSlice['REQ_MIN'] == -1:
                self.__ServiceSlice['REQ_MIN'] = int_time
            self.__ServiceSlice['REQ_MIN'] = min(self.__ServiceSlice.get('REQ_MIN'), int_time)
        else:
            #respond
            self.__ServiceSlice['ResponseCount'] = 1

    def __Update(self, dict_slice_param):
        if dict_slice_param.get('Type') == 0:
            #request
            self.__ServiceSlice['RequestCount'] += 1
            int_time = dict_slice_param.get('TimeLen')
            self.__SumTimes += int_time
            self.__ServiceSlice['REQ_MAX'] = max(self.__ServiceSlice.get('REQ_MAX'), int_time)
            if self.__ServiceSlice['REQ_MIN'] == -1:
                self.__ServiceSlice['REQ_MIN'] = int_time
            self.__ServiceSlice['REQ_MIN'] = min(self.__ServiceSlice.get('REQ_MIN'), int_time)
        else:
            #respond
            self.__ServiceSlice['ResponseCount'] += 1

    def Update(self, dict_slice_param):
        assert len(dict_slice_param)==len(SliceUniteParam)          # judge whether input parament is right

        self.__mutex.acquire()
        self.__Update(dict_slice_param)
        self.__mutex.release()

    def GetSumTime(self):
        return self.__SumTimes

    def GetDict(self):
        return self.__ServiceSlice

class CSingleton(object):
    def __new__(cls, *args, **kw):
        if not hasattr(cls, '_instance'):
            orig = super(CSingleton, cls)
            cls._instance = orig.__new__(cls, *args, **kw)
        return cls._instance

class CServiceSlice(CSingleton):
    def __init__(self, size = 100):
        self.__size = size
        self.__dict_slice = {}
        self.__mutex = threading.Lock()

    def __ToKey(self, str_time, str_service):
        if str_time is None or str_service is None:
            return None
        str_key = str_time.replace(' ', '/')
        str_key += str_service
        return str_key

    def __GetRequst(self, dict_param):
        slice_unite = dict(SliceUniteParam)
        slice_unite['Type'] = 0
        slice_unite['SERVICE_NAME'] = dict_param.get('SERVICE_NAME')
        slice_unite['TimeSlice'] = dict_param.get('BEGIN_DATE')
        slice_unite['STAT'] = dict_param.get('STAT')
        slice_unite['TimeLen'] = dict_param.get('USED_TIME')
        slice_unite['BATCH_SEQID'] = dict_param.get('BATCH_SEQID')
        slice_unite['MAC_IP'] = dict_param.get('MAC_IP')
        return slice_unite

    def __GetRespond(self, dict_param):
        slice_unite = dict(SliceUniteParam)
        slice_unite['Type'] = 1
        slice_unite['SERVICE_NAME'] = dict_param.get('SERVICE_NAME')
        slice_unite['TimeSlice'] = dict_param.get('END_DATE')
        slice_unite['STAT'] = dict_param.get('STAT')
        slice_unite['TimeLen'] = dict_param.get('USED_TIME')
        slice_unite['BATCH_SEQID'] = dict_param.get('BATCH_SEQID')
        slice_unite['MAC_IP'] = dict_param.get('MAC_IP')
        return slice_unite

    def __Push(self, key, slice_unite):
        self.__mutex.acquire()                  # Lock
        obj_unite = self.__dict_slice.get(key)
        if obj_unite == None:
            # new a obj_unite
            try:
                self.__dict_slice[key] = CServiceSliceUnite(slice_unite)
            except:
                self.__mutex.release()
                LOGError.error("New ServiceSliceUnite abort!")
                return -1

            self.__mutex.release()

        else:
            # update the obj_unite
            self.__mutex.release()              # UnLock
            obj_unite.Update(slice_unite)
        return 0

    def Push(self, dict_param):
        slice_unite = self.__GetRequst(dict_param)
        key = self.__ToKey(dict_param.get('BEGIN_DATE'), dict_param.get('SERVICE_NAME'))
        if key is None:         # 报文日志异常，不做处理
            return
        # push request info
        self.__Push(key, slice_unite)

        # push respond info
        slice_unite = self.__GetRespond(dict_param)
        key = self.__ToKey(dict_param.get('END_DATE'), dict_param.get('SERVICE_NAME'))
        self.__Push(key, slice_unite)

    def Pop(self):
        self.__mutex.acquire()                  # Lock
        dict_list =  self.Pop_NonLock()
        self.__mutex.release()
        return dict_list

    def Push_NonLock(self, dict_param):
        slice_unite = dict(SliceUniteParam)
        slice_unite['Type'] = 0
        slice_unite['SERVICE_NAME'] = dict_param.get('SERVICE_NAME')
        slice_unite['TimeSlice'] = dict_param.get('BEGIN_DATE')
        slice_unite['STAT'] = dict_param.get('STAT')
        slice_unite['TimeLen'] = dict_param.get('USED_TIME')
        slice_unite['BATCH_SEQID'] = dict_param.get('BATCH_SEQID')
        slice_unite['MAC_IP'] = dict_param.get('MAC_IP')
        key = self.__ToKey(dict_param.get('BEGIN_DATE'), dict_param.get('SERVICE_NAME'))

        obj_unite = self.__dict_slice.get(key)
        if obj_unite == None:
            # new a obj_unite
            try:
                self.__dict_slice[key] = CServiceSliceUnite(slice_unite)
            except:
                LOGError.error("New ServiceSliceUnite abort!")
                return -1

        else:
            # update the obj_unite
            self.__mutex.release()              # UnLock
            obj_unite.Update(slice_unite)
            self.__dict_slice[key] = obj_unite

        return 0

    def Pop_NonLock(self):
        if len(self.__dict_slice) > 0:
            pa =self.__dict_slice.popitem()
            obj_slice = pa[1]
            dict_data = obj_slice.GetDict()
            if obj_slice.GetSumTime() > 0:                  # calculate the average time of requestion
                dict_data['REQ_AVG'] = float(obj_slice.GetSumTime()) / dict_data.get('RequestCount')
            return dict_data
        else:
            return None

    def Pop_All(self):
        dict_list = []
        while True:
            dict_item = self.Pop_NonLock()
            if dict_item is None:
                break
            else:
                dict_list.append(dict_item)
        return dict_list


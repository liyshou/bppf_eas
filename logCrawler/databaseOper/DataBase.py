#/usr/bin/python
#-*- coding: utf-8 -*-

from DataBaseAdapt import CDBAdapter
from abc import ABCMeta,abstractmethod

# DataBase public base class
class CDBbase(object):
    __metaclass__ = ABCMeta

    def __init__(self, dbType, host, dbName):
        self._dbAdapter = CDBAdapter(dbType)
        self._host = host
        self._dbName = dbName


    def Insert(self, DictData):
        return self._dbAdapter.AdaptDbInsert(DictData)


    def Delete(self, condition):
        return self._dbAdapter.AdaptDbDeleteData(condition)


    def Update(self, DictData, condition):
        return self._dbAdapter.AdaptDbUpdateData(DictData, condition)


    def Query(self, ListData, condition):
        return self._dbAdapter.AdaptDbFindData(ListData, condition)

    def CountData(self, condition):
        return self._dbAdapter.AdaptDbCountData(condition)



class CET_UnitLogs(CDBbase):

    def __init__(self, dbType, host):
        super(CET_UnitLogs, self).__init__(dbType, host, "tmdb")
        self._dbAdapter.SetDbObj(self._host,self._dbName,"colUnitlogs")
        #self._dbAdapter.SetDbObj(self._host,self._dbName,"person")
        self._dbAdapter.AdaptConnectDb()


class CET_ServiceSlice(CDBbase):

    def __init__(self, dbType, host):
        super(CET_ServiceSlice, self).__init__(dbType, host, "tmdb")
        self._dbAdapter.SetDbObj(self._host,self._dbName,"colServiceSlice")
        self._dbAdapter.AdaptConnectDb()


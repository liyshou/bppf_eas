#/usr/bin/python
#-*- coding: utf-8 -*-

from public.LoggingInit import LOGDebug,LOGError
from abc import ABCMeta,abstractmethod
import pymongo
import sys




class COperDb(object):
    __metaclass__ = ABCMeta

    def __init__(self, host, dbName, tableName):
        self._host = host
        self._dbName = dbName
        self._tableName = tableName

    @abstractmethod
    def ConnectDb(self):
        LOGDebug.debug("Connect database..")
        pass

    @abstractmethod
    def InsertData(self, DictData):
        LOGDebug("Table %s,insert data :%s"%(self._tableName, DictData))
        pass

    @abstractmethod
    def DeleteData(self, condition):
        LOGDebug("Table %s,delete data:%s"%(condition))
        pass

    @abstractmethod
    def UpdateData(self, DictData, condition):
        LOGDebug("Table %s,update data:set %s where"%(DictData,condition))
        pass

    @abstractmethod
    def FindData(self, ListData, condition):
        pass

    @abstractmethod
    def CountData(self, condition):
        pass

    @abstractmethod
    def ConditionTransfer(self,condition):
        pass

class COperMongoDb(COperDb):
    def __init__(self, host, dbName, tableName):
        super(COperMongoDb, self).__init__(host, dbName, tableName)


    def ConnectDb(self):
        try:
            # before operation,check db is connect or not
            self.__conn = pymongo.MongoClient(self._host,27017)
            self.__db = self.__conn[self._dbName]
            if [] == self.__db.collection_names():
                LOGError.error("dbName is invalid!")
            elif self._tableName not in self.__db.collection_names():
                LOGError.error("collection is invalid!")
            else:
                self.__coll = self.__db[self._tableName]
        except Exception,e:
            LOGError.error("some error happened: "%e)


    def InsertData(self, DictData):
        try:
            if type(DictData) is not dict:
                LOGError.error("Input parameter is not illegal, please check!")
                sys.exit(1)
            self.__coll.insert_one(DictData)
        except Exception,e:
            print DictData
            LOGError.error("some error happened: %s"%e)


    def DeleteData(self, condition):
        try:
            if type(condition) is not dict:
                LOGError.error("Input parameter is not illegal, please check!")
                sys.exit(1)
            self.__coll.delete_one(condition)
        except Exception,e:
            LOGError.error("some error happened: %s"%e)


    def UpdateData(self, DictData, condition):
        try:
            if type(condition) is not dict:
                LOGError.error("Input parameter is not illegal, please check!")
                sys.exit(1)
            if type(DictData) is not dict:
                LOGError.error("Input parameter is not illegal, please check!")
                sys.exit(1)
            self.__coll.update_one(condition,{"$set":DictData})
        except Exception,e:
            LOGError.error("some error happened: %s"%e)


    def FindData(self, ListData, condition):
        try:
            ListFindRet = []
            if type(condition) is not dict:
                LOGError.error("Input parameter is not illegal, please check!")
                sys.exit(1)
            findRets = self.__coll.find(condition)

            for rets in findRets:
                #find all the elements
                if [] == ListData:
                    ListFindRet.append(rets)
                    continue
                DictFindRet = {}
                for data in ListData:
                    if data in rets.keys():
                        DictFindRet[data] = rets[data]
                ListFindRet.append(DictFindRet)
            return ListFindRet
        except Exception,e:
            LOGError.error("some error happened: %s"%e)


    def CountData(self, condition):
        if {} == condition:
            return self.__coll.count()
        else:
            return self.__coll.find(condition).count()


    def ConditionTransfer(self,condition):
        pass

#end of COperMongoDb

class COperMySqlDb(COperDb):
    pass

class COperOracleDb(COperDb):
    pass


DictDataBaseType  = {
    "DBTYPE_MONGODB"   :    COperMongoDb,
    "DBTYPE_MYSQL"     :    COperMySqlDb,
    "DBTYPE_ORACLE"    :    COperOracleDb
}


class CDBAdapter(object):
    def __init__(self, dbType):
        self.__dbType = dbType
        self.__dbObj = None

    def SetDbObj(self, host,dbName, tableName):
        LOGDebug.debug("use Database %s", self.__dbType)
        self.__dbObj = DictDataBaseType.get(self.__dbType)(host, dbName, tableName)

    def AdaptConnectDb(self):
        return self.__dbObj.ConnectDb()

    def AdaptDbInsert(self,DictData):
        return  self.__dbObj.InsertData(DictData)

    def AdaptDbDeleteData(self, condition):
        return  self.__dbObj.DeleteData(condition)

    def AdaptDbUpdateData(self, DictData, condition):
        return self.__dbObj.UpdateData(DictData, condition)

    def AdaptDbFindData(self, ListData, condition):
        return self.__dbObj.FindData(ListData, condition)

    def AdaptDbCountData(self, condition):
        return self.__dbObj.CountData(condition)
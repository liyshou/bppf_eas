#-*- coding: utf-8 -*-
"""
ClassName:  CLogCrawler
Author:     YanJinMu
Date:       2015-08-18
Description:log crawler class
"""

from public.Common import *
from public.CServiceSlice import CServiceSlice
from  public.threadpool import *
# from threading import Lock
import os
import sys

slice = CServiceSlice()

class CLogCrawler(object):
    def __init__(self, fileName):
        self.__arg_list = []
        self.__file_handler = CFileHandler()
        self.__translator = CLogTranslator()
        self.__factory = COperFactory()
        self.unitLogObj = CET_UnitLogs('DBTYPE_MONGODB', '192.168.87.9')
        self.unitServSliceObj = CET_ServiceSlice('DBTYPE_MONGODB', '192.168.87.9')
        #self.unitLogObj = CET_UnitLogs('DBTYPE_MONGODB', '127.0.0.1')
        #self.unitServSliceObj = CET_ServiceSlice('DBTYPE_MONGODB', '127.0.0.1')
        self.__fileName = fileName
        self.__unite_logs = COperToUniteLogs()
        self.__factory.Append(self.__unite_logs)

    def __TraverseDir(self, str_dirpath, file_list, int_deep = 1000):                 # int_deep是遍历当前路径的深度， 默认为1000：所有， 0：当前目录， 1：当前及下级子目录
        #file_list = []
        deep = int_deep
        if os.path.isdir(str_dirpath):
            ret_list = os.listdir(str_dirpath)
            for str_file in ret_list:
                if SYS_WINDOW():
                    if str_dirpath.endswith('\\'):
                        str_file = str_dirpath + str_file
                    else:
                        str_file = '%s\\%s'%(str_dirpath, str_file)
                else:
                    if str_dirpath.endswith('/'):
                        str_file = str_dirpath + str_file
                    else:
                        str_file = '%s/%s'%(str_dirpath, str_file)

                #print("Debug:%s"%str_file)

                if os.path.isfile(str_file):
                    if str_file.endswith('.log'):
                        file_list.append(str_file)
                elif os.path.isdir(str_file) and int_deep > 0:
                    self.__TraverseDir(str_file, file_list, deep -1)
                else:
                    LOGError.error("Traverse Directory abort, file is %s"%str_file)
                    pass
            deep -= deep

        return deep
    #end of __TraverseDir()

    def __PrintHelp(self):
        strSpace = "                   "
        print("#########################Please run LogTraversal.py like the way below:###############################")
        print("python LogTraversal.py -h|--help:")
        print(strSpace + "打印帮助文档")
        print("python LogTraversal.py -v|--version:")
        print(strSpace + "打印工具版本信息")
        print("python LogTraversal.py -type  import –d  [directory]")
        print(strSpace + "导入当前文件夹所有文件的数据到数据库")
        print("python LogTraversal.py -type import  -f [pathfile]")
        print(strSpace + "导入指定文件的数据到数据库")
        print("#########################################################################################################")

    def __PrintVersion(self):
        print("version: v0.1")

    def ImportFile(self):
        global slice
        self.tbListOut = []

        # print self.__fileName
        if self.__file_handler.FileOpen(self.__fileName) < 0:
            LOGError.error("Open file failed!")
            return -1
        # seccesionCnt = 0
        while True:
            table_list = []
            sectionlist = self.__file_handler.ReadSection()                # 读取会话
            if len(sectionlist) < 1:
                LOGDebug.debug("There is nothing to read!")
                break
            #for str in sectionlist:
            #   print(str)


            self.__translator.SetLogInfo(sectionlist)
            str_dict_list = self.__translator.GetTransData()       # 转码成子典

            # for dict in str_dict_list:                     # 打印日志信息
            #     if len(dict) == 5:
            #         print (dict)
            #
            self.__factory.SetCond(str_dict_list)
            table_list = self.__factory.GetCond()                  # 数据操作
            # 导入colServiceSlice的数据
            for dict in table_list:
                # print dict
                # seccesionCnt = seccesionCnt + 1
                if len(dict) < 15:                            # 异常数据,忽略
                    print dict
                    continue
                self.tbListOut.append(dict)
                slice.Push(dict)



        # else:
        #     print  "end"

        # end of while
        # print seccesionCnt
        self.__file_handler.FileClose()

        return 0
    #end of __ExportFile


    def __ImportDir(self, str_dir_path):
        file_list = []
        self.__TraverseDir(str_dir_path, file_list)
        # operate dir,muli-thread
        pool = ThreadPool(2)
        requests = makeRequests(self.__ImportFile,file_list[0 : 2])
        [pool.putRequest(req) for req in requests]
        pool.wait()
        # for iLoop in range(len(file_list)/MAX_THREAD_NUM):
        #     requests = makeRequests(self.__ImportFile,file_list[iLoop*MAX_THREAD_NUM : (iLoop+1)*MAX_THREAD_NUM])
        #     [pool.putRequest(req) for req in requests]
        #     pool.wait()
        # for str_file in file_list:
        #     #print str_file
        #     self.__ImportFile(str_file)

    def __DealwithPrefixImport(self):
        if len(self.__arg_list) < 5:
            self.__PrintHelp()
        if "-d" == self.__arg_list[3]:
            if os.path.isdir(self.__arg_list[4]):
                # 目录操作
                self.__unite_logs = COperToUniteLogs()
                self.__factory.Append(self.__unite_logs)                          # 添加数据操作
                self.__ImportDir(self.__arg_list[4])
            else:
                print("请在-d 后面输入一个目录名")
        elif "-f" == self.__arg_list[3]:
            if os.path.isfile(self.__arg_list[4]):
                # 文件操作
                self.__unite_logs = COperToUniteLogs()
                self.__factory.Append(self.__unite_logs)                          # 添加数据操作
                self.ImportFile(self.__arg_list[4])
                pass
            else:
                print("请在-f 后面输入一个文件名")
        else:
            self.__PrintHelp()

    def __PrefixTypeHandler(self):
        if len(self.__arg_list) < 5:
            self.__PrintHelp()
            sys.exit(0)
        for typePrefix in  self.CmdTypePrefix.keys():
            if self.__arg_list[2] == typePrefix:
                self.CmdTypePrefix.get(typePrefix)(self)
            else:
                continue

    def HandlePrefix(self, arg_list):
        self.__arg_list = arg_list[:]
        if len(arg_list) < 2:
            self.__PrintHelp()
            sys.exit(0)
        for prefix in self.CmdPrefix.keys():
            if arg_list[1] == prefix:
                self.CmdPrefix.get(prefix)(self)
            else:
                continue

    def GetParams(self, str_list):
        pass

    def Run(self, str_dir):
        pass

    CmdPrefix = {
    '-h'             :    __PrintHelp,
    '--help'        :     __PrintHelp,
    '-v'             :    __PrintVersion,
    '--version'     :     __PrintVersion,
    '-type'         :     __PrefixTypeHandler
    }

    CmdTypePrefix = {
    'import'        :    __DealwithPrefixImport
    }


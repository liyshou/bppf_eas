#/usr/bin/python
#-*- coding: utf-8 -*-

from public.Common import *
from  public.threadpool import *
from CLogCrawler import CLogCrawler, slice
import sys
import os



def PrintHelp(arg_list):
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

def PrintVersion(arg_list):
    print("version: v0.1")


def TraverseDir(str_dirpath, file_list, int_deep = 1000):                 # int_deep是遍历当前路径的深度， 默认为1000：所有， 0：当前目录， 1：当前及下级子目录
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
                TraverseDir(str_file, file_list, deep -1)
            else:
                LOGError.error("Traverse Directory abort, file is %s"%str_file)
                pass
        deep -= deep

    return deep
#end of __TraverseDir()

def ThreadImportFile(crawler):
    return crawler.ImportFile()

def InsertOrUpdateSliceTable(obj_table, slice_record):
    dict_cond = {'TimeSlice':slice_record.get('TimeSlice'), 'SERVICE_NAME':slice_record.get('SERVICE_NAME')}
    listQuery = obj_table.Query([], dict_cond)
    if len(listQuery) > 0:
        for row in listQuery:
            row['RequestCount'] += slice_record.get('RequestCount')
            row['ResponseCount'] += slice_record.get('ResponseCount')

            req_cnt = row.get('RequestCount') + slice_record.get('RequestCount')
            if req_cnt > 0:
                row['REQ_AVG'] = ((row.get('REQ_AVG') * row.get('RequestCount')) + (slice_record.get('REQ_AVG') * slice_record.get('RequestCount'))) /req_cnt

            row['REQ_MAX'] = max(row['REQ_MAX'], slice_record.get('REQ_MAX'))

            if slice_record.get('REQ_MIN') > -1:
                if row.get('REQ_MIN') > -1:
                    row['REQ_MIN'] = min(row.get('REQ_MIN'), slice_record.get('REQ_MIN'))
                else:
                    row['REQ_MIN'] = slice_record.get('REQ_MIN')

            obj_table.Update(row, dict_cond)
    else:
        obj_table.Insert(slice_record)


# 在日志解析处理完成后进行表插入工作
def InsertAfterProcess(crawler):
    slicePopList = []
    # 插入colUnitlogs表
    for dict in crawler.tbListOut:                         # 插表操作
        crawler.unitLogObj.Insert(dict)


    global slice
    # 插入colServiceSlice表
    slicePopList = slice.Pop_All()

    #print len(slicePopList)
    for slicePop in slicePopList:
        InsertOrUpdateSliceTable(crawler.unitServSliceObj, slicePop)
        #crawler.unitServSliceObj.Insert(slicePop)


def ImportDir(str_dir_path):
    file_list = []
    TraverseDir(str_dir_path, file_list)
    # operate dir,muli-thread
    if len(file_list) <= MAX_THREAD_NUM:
        numTp = len(file_list)
    else:
        numTp = MAX_THREAD_NUM
    pool = ThreadPool(MAX_THREAD_NUM)

    for iLoop in range((len(file_list)/numTp) + 1):
        crawlerList = []
        #LOGDebug.debug("====== Begin Deal!===============")
        for iLoop1 in range(iLoop*numTp ,(iLoop+1)*numTp, 1):
            if iLoop1 < len(file_list):
                crawler = CLogCrawler(file_list[iLoop1])
                crawlerList.append(crawler)
            #print file_list[iLoop1]
        requests = makeRequests(ThreadImportFile,crawlerList)
        [pool.putRequest(req) for req in requests]
        pool.wait()
        # 处理完后统一插表
        #LOGDebug.debug("====== Begin insert!===============")
        for crawl in crawlerList:
            InsertAfterProcess(crawl)
        #LOGDebug.debug("====== End insert!===============")


def DealwithPrefixImport(arg_list):
    if len(arg_list) < 5:
        PrintHelp()
    if "-d" == arg_list[3]:
        if os.path.isdir(arg_list[4]):
            # 目录操作
            ImportDir(arg_list[4])
        else:
            print("请在-d 后面输入一个目录名")
    elif "-f" == arg_list[3]:
        if os.path.isfile(arg_list[4]):
            # 文件操作
            crawler = CLogCrawler(arg_list[4])
            crawler.ImportFile()
            pass
        else:
            print("请在-f 后面输入一个文件名")
    else:
        PrintHelp()




CmdTypePrefix = {
    'import'        :    DealwithPrefixImport
}




def PrefixTypeHandler(arg_list):
    if len(arg_list) < 5:
        PrintHelp()
        sys.exit(0)
    for typePrefix in  CmdTypePrefix.keys():
        if arg_list[2] == typePrefix:
            CmdTypePrefix.get(typePrefix)(arg_list)
        else:
            continue


CmdPrefix = {
    '-h'             :    PrintHelp,
    '--help'        :     PrintHelp,
    '-v'             :    PrintVersion,
    '--version'     :     PrintVersion,
    '-type'         :     PrefixTypeHandler
}


def HandlePrefix(arg_list):
    #self.__arg_list = arg_list[:]
    if len(arg_list) < 2:
        PrintHelp()
        sys.exit(0)
    for prefix in CmdPrefix.keys():
        if arg_list[1] == prefix:
            CmdPrefix.get(prefix)(arg_list)
        else:
            continue


def main():
    #crawler = CLogCrawler()
    HandlePrefix(sys.argv)


if __name__ == '__main__':
    main()




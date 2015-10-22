/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     CarryConfig.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : AutoCarry.cpp : 定义控制台应用程序的入口点。
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/
#include <iostream>
#include <stdio.h>
using namespace std;

#include "OpCarryOperator.h"
#include <time.h>


#ifndef __GLOBAL_VARIABLE_DEFILE2__
#define __GLOBAL_VARIABLE_DEFILE2__
ConnectPool * gl_pConnectPool = NULL;
#endif



int main()
{
	
	if (gl_pConnectPool == NULL)
	{
		/*因LINUX 多进程不能共享链接库的变量*/
		CreateInterface((void**)&gl_pConnectPool ,"ConnectPool" ); 
	}
   
	OpCarryOperator OpCarry(*(gl_pConnectPool->getConnection())); //①
	//OpCarryOperator OpCarry;
	//OpCarry.initializeDB(*(gl_pConnectPool->getConnection()));   //初始化数据库链接
	OpCarry.initInfo();                   //初始化操作数据
	OpCarry.getCarryConfig();             //获取 时间 IP地址
	OpCarry.initMachineIp();              //初始化MACHINE_NAME (IP:端口)
	
	while (1)
	{
		
		OpCarry.startoperator();
		OpCarry.sleeptime();      //睡眠
//#ifndef _WIN32
  //      sleep(60);
//#endif
		
	}

	return 0;
}


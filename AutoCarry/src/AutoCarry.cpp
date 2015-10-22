/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     CarryConfig.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : AutoCarry.cpp : �������̨Ӧ�ó������ڵ㡣
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
		/*��LINUX ����̲��ܹ������ӿ�ı���*/
		CreateInterface((void**)&gl_pConnectPool ,"ConnectPool" ); 
	}
   
	OpCarryOperator OpCarry(*(gl_pConnectPool->getConnection())); //��
	//OpCarryOperator OpCarry;
	//OpCarry.initializeDB(*(gl_pConnectPool->getConnection()));   //��ʼ�����ݿ�����
	OpCarry.initInfo();                   //��ʼ����������
	OpCarry.getCarryConfig();             //��ȡ ʱ�� IP��ַ
	OpCarry.initMachineIp();              //��ʼ��MACHINE_NAME (IP:�˿�)
	
	while (1)
	{
		
		OpCarry.startoperator();
		OpCarry.sleeptime();      //˯��
//#ifndef _WIN32
  //      sleep(60);
//#endif
		
	}

	return 0;
}


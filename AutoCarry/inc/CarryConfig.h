/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     CarryConfig.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : �Զ����ֶ�ʱ����
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/

#ifndef __CARRYCONFIG_H__
#define __CARRYCONFIG_H__

#include "stdtm.h"
#include "AutoDatabase.h"
#include "et_base.h"


class  CCarryConfig : public AutoDatabase
{
protected:

public:
	CCarryConfig();
	virtual ~CCarryConfig();

	//��ȡ�����ļ�
	void getCarryConfig();

	//��ʼ����������
	void initInfodata();
	
	//��ȡ��ǰʱ��
	string GetTuxTime(int Format);
};

#endif
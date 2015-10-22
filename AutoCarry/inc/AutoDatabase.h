/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     AutoDatabase.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : �Զ����ֳ��� - ���ݻ���
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/

#ifndef __AutoDatabase_h__
#define __AutoDatabase_h__

#include "stdtm.h"
#include "et_base.h"

class  AutoDatabase
{
public:
	AutoDatabase();
	~AutoDatabase();
	void initInfo();
protected:

	//��������
	string seq_id;
	string area_code;
	string cust_id;
	string cust_code;
	string prod_id;
	string prod_code;
	string action_id;
	string action_code;
	string pay_org;   //��ҵ�˻�֧������
	string pay_type;  //��ҵ�˻�֧��
	string acct_code;
	string pay_money;
	string bank_acct_code;   //�����˻�������
	string obj_code;
	string fail_times;
	string obj_acct_code;
	string bank_id;
	string obj_acct_name;
	string sign_id;
	string bank_code;   //���л�������
	string pay_org_bank;   //֧������
	string obj_acct_id;
	string m_strCumSecLevel; //�̻��Ķ����̻���

	//��ȡ�����ļ�
	string interval_time;
	string machineip;

	string current_date;
	string prtn_code;

protected:

	
};


#endif
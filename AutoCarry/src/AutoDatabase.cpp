/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     AutoDatabase.cpp
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

#include "AutoDatabase.h"
#include "et_base.h"

AutoDatabase::AutoDatabase()
{
	//��������
	seq_id     =  "";
	area_code  =  "";
	cust_id    =  "";
	cust_code  =  "";
	prod_id    =  "";
	prod_code  =  "";
	action_id  =  "";
	action_code = "";
	pay_org    =  "";
	pay_type   =  "";
	acct_code  =  "";
	pay_money  =  "";
	bank_acct_code="";   //�����˻�������
	obj_code   =  "";
	fail_times =  "";
	obj_acct_code  =  "";
	bank_id    =  "";
	obj_acct_name  =  "";
	sign_id    =  "";
	bank_code  =  "";   //���л�������
	pay_org_bank = "";   //֧������
	obj_acct_id  =  "";

	//��ȡ�����ļ�
	interval_time   =  "";
	machineip       =  "";

	current_date    =  "";
	prtn_code       =  "";
	
	m_strCumSecLevel = ""; //�����̻���

}

AutoDatabase::~AutoDatabase()
{

}

void AutoDatabase::initInfo()
{
	//��������
	seq_id     =  "";
	area_code  =  "";
	cust_id    =  "";
	cust_code  =  "";
	prod_id    =  "";
	prod_code  =  "";
	action_id  =  "";
	action_code = "";
	pay_org    =  "";
	pay_type   =  "";
	acct_code  =  "";
	pay_money  =  "";
	bank_acct_code="";
	obj_code   =  "";
	fail_times =  "";
	obj_acct_code  =  "";
	bank_id    =  "";
	obj_acct_name  =  "";
	sign_id    =  "";
	bank_code  =  "";   //���л�������
	pay_org_bank = "";   //֧������
	obj_acct_id  =  "";

	//��ȡ�����ļ�
	interval_time   =  "";
	machineip       =  "";

	current_date    =  "";
	prtn_code       =  "";


}
/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     OpCarryOperator.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : �Զ����ֶ�ʱ���� - ���������
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/

#ifndef __OPCARRYOPERATOR_H__
#define __OPCARRYOPERATOR_H__

#include "CarryDatabase.h"
#include <time.h>

//��ʱ�洢����
typedef struct
{
	string seq_id;
	string area_code;
	string cust_id;
	string cust_code;
	string prod_id;
	string prod_code;
	string action_id;
	string action_code;
	string pay_org;
	string pay_type;
	string acct_code;
	string pay_money;
	string bank_acct_code;   //�����˻�������
	string obj_code;
	string fail_times;
	string obj_acct_code;
	string bank_id;
	string obj_acct_name;
	string sign_id;
	string obj_acct_id;

}CP_OPER_RECORD,*CP_STR_OPER_RECORD;

class OpCarryOperator:public OpCarryDatabase
{
public:
	OpCarryOperator();
	OpCarryOperator(Connection& conn);
	~OpCarryOperator();

public:

	//��ʼִ�в���
	void startoperator();

	//��������ݰ�
	ParamParser setParamarser();

	//��ȡ·��
	bool getRouterRule(const string& bankId,const string& custCode);

	bool GetCartTypeAndPrivateType(const string& strCustCode, string& strCartType, string& strPrivateType);

	ParamSet setParamset401();
	ParamSet setParamset402();
	ParamSet setParamset404();
	ParamSet setParamset405();
	ParamSet setParamset407();
	ParamSet setParamset408();

	// CUM0003���
	ParamSet setParamset200(const string& strCustCode);
	ParamSet setParamset002();

private:
	
	ParamParser m_ppIntpar;   //�����
	ParamParser m_ppOutpar;   //�����

	//vector<CP_STR_OPER_RECORD> vec_operRecord;
	//vector<CP_STR_OPER_RECORD>::iterator ite;


};


#endif
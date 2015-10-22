/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     OpCarryOperator.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : 自动提现定时程序 - 具体操作类
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

//临时存储数据
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
	string bank_acct_code;   //银行账户开户行
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

	//开始执行操作
	void startoperator();

	//组调用数据包
	ParamParser setParamarser();

	//获取路由
	bool getRouterRule(const string& bankId,const string& custCode);

	bool GetCartTypeAndPrivateType(const string& strCustCode, string& strCartType, string& strPrivateType);

	ParamSet setParamset401();
	ParamSet setParamset402();
	ParamSet setParamset404();
	ParamSet setParamset405();
	ParamSet setParamset407();
	ParamSet setParamset408();

	// CUM0003组包
	ParamSet setParamset200(const string& strCustCode);
	ParamSet setParamset002();

private:
	
	ParamParser m_ppIntpar;   //输入包
	ParamParser m_ppOutpar;   //输出包

	//vector<CP_STR_OPER_RECORD> vec_operRecord;
	//vector<CP_STR_OPER_RECORD>::iterator ite;


};


#endif
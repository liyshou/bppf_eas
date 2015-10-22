/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     AutoDatabase.cpp
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : 自动提现程序 - 数据基类
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
	//操作数据
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
	bank_acct_code="";   //银行账户开户行
	obj_code   =  "";
	fail_times =  "";
	obj_acct_code  =  "";
	bank_id    =  "";
	obj_acct_name  =  "";
	sign_id    =  "";
	bank_code  =  "";   //银行机构编码
	pay_org_bank = "";   //支付机构
	obj_acct_id  =  "";

	//读取配置文件
	interval_time   =  "";
	machineip       =  "";

	current_date    =  "";
	prtn_code       =  "";
	
	m_strCumSecLevel = ""; //二级商户号

}

AutoDatabase::~AutoDatabase()
{

}

void AutoDatabase::initInfo()
{
	//操作数据
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
	bank_code  =  "";   //银行机构编码
	pay_org_bank = "";   //支付机构
	obj_acct_id  =  "";

	//读取配置文件
	interval_time   =  "";
	machineip       =  "";

	current_date    =  "";
	prtn_code       =  "";


}
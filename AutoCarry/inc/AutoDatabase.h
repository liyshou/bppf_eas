/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     AutoDatabase.h
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

	//操作数据
	string seq_id;
	string area_code;
	string cust_id;
	string cust_code;
	string prod_id;
	string prod_code;
	string action_id;
	string action_code;
	string pay_org;   //企业账户支付机构
	string pay_type;  //企业账户支付
	string acct_code;
	string pay_money;
	string bank_acct_code;   //银行账户开户行
	string obj_code;
	string fail_times;
	string obj_acct_code;
	string bank_id;
	string obj_acct_name;
	string sign_id;
	string bank_code;   //银行机构编码
	string pay_org_bank;   //支付机构
	string obj_acct_id;
	string m_strCumSecLevel; //商户的二级商户号

	//读取配置文件
	string interval_time;
	string machineip;

	string current_date;
	string prtn_code;

protected:

	
};


#endif
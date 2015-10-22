/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     CarryConfig.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : 自动提现定时程序
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

	//获取配置文件
	void getCarryConfig();

	//初始化操作数据
	void initInfodata();
	
	//获取当前时间
	string GetTuxTime(int Format);
};

#endif
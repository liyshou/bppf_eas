/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     CarryConfig.cpp
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

#include "CarryConfig.h"

CCarryConfig::CCarryConfig()
{

}
CCarryConfig::~CCarryConfig()
{

}

void CCarryConfig::getCarryConfig()
{
	EnvironmentVar env;
	
	IniConfig ini;

	char CarryConfigPath[512];
#ifdef _WIN32
	sprintf(CarryConfigPath,"%s\\job.ini",env.getConfPath());
#else
	sprintf(CarryConfigPath,"%s/job.ini",env.getConfPath());
#endif

	ini.setIniname(CarryConfigPath);

	interval_time   =    ini.IniParser_GetString("AUTOCARRY:INTERVAL_TIME","");

	machineip       =    ini.IniParser_GetString("AUTOCARRY:MACHINE_NAME","");

	cout<<"���ʱ��:��"<<interval_time<<"��"<<endl;
	cout<<"IP/�˿�: ��"<<machineip<<"��"<<endl;
	
        return;
}


//void CCarryConfig::initInfodata()
//{
//	//��������
//	seq_id     =  "";
//	area_code  =  "";
//	cust_id    =  "";
//	cust_code  =  "";
//	prod_id    =  "";
//	prod_code  =  "";
//	action_id  =  "";
//	action_code = "";
//	pay_org    =  "";
//	pay_type   =  "";
//	acct_code  =  "";
//	pay_money  =  "";
//	obj_code   =  "";
//	fail_times =  "";
//	obj_acct_code  =  "";
//	bank_id    =  "";
//	obj_acct_name  =  "";
//	sign_id    =  "";
//	bank_code  =  "";   //���л�������
//	pay_org_bank = "";   //֧������
//	prtn_code    =  "";
//}


string CCarryConfig::GetTuxTime(int Format)
{
	string Tuxtime = "";
	struct tm *newtime;
	time_t long_time;
	static char curtime[20] = "";
	time(&long_time);
	newtime = localtime(&long_time);
	if(Format == 1)
		sprintf(curtime,"%04d/%02d/%02d %02d:%02d:%02d",newtime->tm_year+1900,
		newtime->tm_mon+1,
		newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	else if (Format == 0)
		sprintf(curtime,"%04d%02d%02d%02d%02d%02d\0",
		newtime->tm_year+1900,newtime->tm_mon+1,newtime->tm_mday,
		newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	else if (Format == 10)
		sprintf(curtime,"%02d%02d\0",newtime->tm_mon+1,newtime->tm_mday);
	else if (Format == 20)
		sprintf(curtime,"%02d%02d%02d\0",newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	Tuxtime.assign(curtime);
	return Tuxtime;
}

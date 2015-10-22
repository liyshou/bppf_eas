/********************************************************************
Copyright (C)  �㶫��Ѷ���ſƼ����޹�˾
File Name:     CarryDatabase.h
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : �Զ����ֶ�ʱ���� - ����
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/
#ifndef __CARRYDATABASE_H__
#define __CARRYDATABASE_H__

#include "CarryConfig.h"

class  OpCarryDatabase : public CCarryConfig
{
protected:
	Connection *m_pConn;
protected:

	string gl_machine_name;

public:

	OpCarryDatabase();
	OpCarryDatabase(Connection& conn);
	~OpCarryDatabase();

	void initializeDB(Connection& conn);

	int execCommand(string& str);

	void execCommandNotCommit(string& str);

	void deleteRecord();

	int callService(const string& strServiceName, ParamParser& parInput, ParamParser& parOutput);

	//bool connectDB();

	//�ͷ�����
	void cleaniterator();

	void initMachineIp();

	void sleeptime();

	void updateoperator(ParamParser& m_pOutpar,string& times,string& seq_id);
	
	void updateRouterRule(string& ret_code,string& ret_info,string& times,string& seq_id);

	//�����ն���ˮ��
	string setTermseq(const string& systime);

	//��ȡ�ͻ�����ֵ
	string getCustomValue(const string& acct_id,const string& attr_id);

	string getBankCode(const string& bankId);

	string getPrtnCode(const string& prtnId);

	string getCumSecLevel(const string& bankCode,const string& custCode); //��ȡ�����̻���

    //��ȡ���۱�ʾ
    string GetCardType(const string& obj_custcode);
protected:

};

#endif
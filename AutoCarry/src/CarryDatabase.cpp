/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     CarryDatabase.cpp
Created Date:  2012-12-28
Author:        Tang Wang
Version: 1.0 
Description  : 自动提现定时程序 - 基类
Others :
Revision histroy :
1:
Date:
Author:
Modifiacation:
2: ...
*******************************************************************/
#include "CarryDatabase.h"
#include "base_util.h"


OpCarryDatabase::OpCarryDatabase()
{
	m_pConn  = NULL;
	gl_machine_name  = "";
}
OpCarryDatabase::OpCarryDatabase(Connection& conn)
{
	m_pConn  = &conn;
	gl_machine_name  =  "";
}
OpCarryDatabase::~OpCarryDatabase()
{
	if (m_pConn != NULL)
	{
		delete m_pConn;
		m_pConn  = NULL;
	}

}

//初始化 MACHINE_NAME
void OpCarryDatabase::initMachineIp()
{
	gl_machine_name = machineip;
	cout<<"IP/端口: 【"<<gl_machine_name<<"】"<<endl;
	
	return;
}

int OpCarryDatabase::execCommand(string& str)
{
	int ret = 0;
	try{
		if (m_pConn)
		{
			m_pConn->executeCommand(const_cast<char*>(str.c_str()));
			m_pConn->commitTrans();
		}
	}catch(DatabaseException* ex){
		//reportError("生成流程环节失败！","4002");
		cout<< str <<endl;
		cout << ex->getErrMessage() << endl;
		delete ex;
		ret = -1;
	}catch(CDbException* ex){
		//reportError("生成流程环节失败！","4002");
		cout<< str <<endl;
		cout << ex->getErrMsg() << endl;
		delete ex;
		ret = -1;
	}catch(...)
	{
		//reportError("交易失败","4002");
		cout<< str <<endl;
		ret = -1;
	}
	return ret;
}
int OpCarryDatabase::callService(const string& strServiceName, ParamParser& parInput, ParamParser& parOutput)
{
	int ret = -1;
	char * ptr=(char*)tmsalloc(2048);
	long maxlen=2048;
	long olen=0;
	char msg[256];
	PESVRINFO* pinfoInput=parInput.getData();
	cout <<endl;
	cout << "###################################################" << endl;
	pinfoInput->data[pinfoInput->len] ='\0';
	cout << "SEND:" << pinfoInput->data << endl;
	cout << "###################################################" << endl;
	//cout << "###################################################" << endl;
	ret = tmsTrancall(strServiceName.c_str(),pinfoInput->data,pinfoInput->len,&ptr,&olen,&maxlen,(char*)gl_machine_name.c_str()); //①
	//【若此方法导致超出数据库游标最大数时,用①方法】
	//ret = tmscall(strServiceName.c_str(),pinfoInput->data,pinfoInput->len,&ptr,&olen,&maxlen);
	//cout<<gl_machine_name<<endl;
	cout << "###################################################" << endl;
	cout << "RECV:" << ptr << endl;
	cout << "###################################################" << endl;

	//cout << "LENGTH = " << olen << endl;
	//cout << "RETURN = " << ret << endl;
	ParamParser pp(ptr,olen);

	if(ret>=0)
	{
		ParamSet * paramSet = NULL;
		if(pp.getType() ==  0)
		{
			paramSet = pp.getParamSet();
		}else
		{
			paramSet = pp.getParamSet("000");
		}

		string s1="";	
		if(paramSet!=NULL)
		{
			s1=paramSet->getDataItem("0001");
			if(atoi(s1.c_str()) == 0 )
			{
				ret = 0;
				parOutput = pp; 
			}else
			{
				ret = 0;
				ParamParser tmp_PP;
				ParamSet ps("000");
				ps.appendRow();
				ps.appendDataItem("0001",paramSet->getDataItem("0001"));
				ps.appendDataItem("0002",paramSet->getDataItem("0002"));
				ps.close();
				tmp_PP.addParamSet(&ps);
				parOutput = tmp_PP; 
			}

		}else
		{
			ret = -1;
			ParamParser tmp_PP;
			ParamSet ps("000");
			sprintf(msg,"服务[%s]返回失败![callService]",strServiceName.c_str());
			ps.appendRow();
			ps.appendDataItem("0001","0006");
			ps.appendDataItem("0002",msg);
			ps.close();
			tmp_PP.addParamSet(&ps);
			parOutput = tmp_PP; 
		}
	}
        else if (-2 == ret)
	{
		ParamParser tmp_PP;
		ParamSet ps("000");
		sprintf(msg,"调用服务[%s]失败！可能是网络异常![BKAutoBase::callService]",strServiceName.c_str());
		ps.appendRow();
		ps.appendDataItem("0001","0015");
		ps.appendDataItem("0002",msg);
		ps.close();
		tmp_PP.addParamSet(&ps);
		parOutput = tmp_PP; 
		ret = -2;
	}
        else
	{
		ParamParser tmp_PP;
		ParamSet ps("000");
		sprintf(msg,"调用服务[%s]失败！未知异常![callService]",strServiceName.c_str());
		ps.appendRow();
		ps.appendDataItem("0001","0006");
		ps.appendDataItem("0002",msg);
		ps.close();
		tmp_PP.addParamSet(&ps);
		parOutput = tmp_PP; 
		ret = -1;
	}

        if(ptr)
        {
	    tmsfree(ptr);
            ptr = NULL;
        }
	return ret;

}

void OpCarryDatabase::initializeDB( Connection& conn )
{
	if (m_pConn != NULL)
	{
		m_pConn->release();		//先释放原来的链接
		m_pConn = NULL;
	}
	m_pConn = &conn;

	return;
}

void OpCarryDatabase::execCommandNotCommit(string& str)
{
	try{
		if (m_pConn)
		{
			m_pConn->executeCommand(const_cast<char*>(str.c_str()));
			//m_pConn->commitTrans();
		}
	}catch(DatabaseException* ex){
		cout << ex->getErrMessage() << endl;
		delete ex;
	}catch(CDbException* ex){
		cout << ex->getErrMsg() << endl;
		delete ex;
	}catch(...)
	{
		//reportError("生成交易订单失败","4002");
	}

	return;

}
void OpCarryDatabase::cleaniterator()
{
	/*for (ite = vec_operRecord.begin(); ite != vec_operRecord.end(); ite++)
	{
		if (*ite != NULL)
		{
			delete (*ite);
			*ite = NULL;
		}
	}*/
	//vec_operRecord.clear();
}

//睡眠停留
void OpCarryDatabase::sleeptime()
{

	long int time = atol(interval_time.c_str());
#ifdef _WIN32
	Sleep(time);
#else
	sleep(time);
#endif

	return;

}

void OpCarryDatabase::updateoperator(ParamParser& m_pOutpar,string& times,string& seq_id)
{
	if (m_pConn == NULL)
	{
		cout <<"【数据库链接失败,请先链接数据库】"<< " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		return;
	}

	ParamSet* ret000   =  NULL;
	ParamSet* ret401   =  NULL;

	string ret_code   =  "";
	string ret_info   =  "";

	string order_id   =  "";
	string event_seq  =  "";

	ret000     =   m_pOutpar.getParamSet("000");
	if (ret000 !=  NULL)
	{
		ret_code   =   ret000->getDataItem("0001");
		ret_info   =   ret000->getDataItem("0002");
	}
	
	ret401     =   m_pOutpar.getParamSet("401");
	if (ret401 !=  NULL)
	{
		order_id   =   ret401->getDataItem("4002");
		event_seq  =   ret401->getDataItem("4118");
	}
	
	char upsql[1024];
	memset(upsql,0,sizeof(upsql));

	if ( ret_code == "0000" )
	{
		cout<<"【系统自动提现成功】"<<endl;
		cout<<"【更新数据:STAT='S0P' AND ORDER_ID ="<<order_id<<" AND EVENT_SEQ ="<<event_seq<<" 】"<<endl;

		sprintf(upsql,"update t_scs_speoper_list set stat = 'S0P',ret_code = '%s',ret_info = '%s',end_date = sysdate, order_id = '%s',event_seq = '%s' where seq_id = %d",
			ret_code.c_str(),ret_info.c_str(),order_id.c_str(),event_seq.c_str(),atol(seq_id.c_str()));
	}
	else if (ret_code == "9002")
	{
		cout<<"【系统自动提现超时】"<<endl;
		cout<<"【更新数据:STAT='S2P' AND ORDER_ID ="<<order_id<<" AND EVENT_SEQ ="<<event_seq<<" 】"<<endl;

		sprintf(upsql,"update t_scs_speoper_list set stat = 'S2P',ret_code = '%s',ret_info = '%s',end_date = sysdate, order_id = '%s',event_seq = '%s' where seq_id = %d",
			ret_code.c_str(),ret_info.c_str(),order_id.c_str(),event_seq.c_str(),atol(seq_id.c_str()));
	}
	else  //失败只做一次处理,避免生成多条相同的失败订单
	{
		cout<<"【系统自动提现失败】"<<endl;
		int ntimes  = atol(times.c_str()) + 1;
		cout<<"【更新数据:STAT='S1P' AND FAIL_TIMES ="<<ntimes<<" AND ORDER_ID ="<<order_id<<" AND EVENT_SEQ ="<<event_seq<<" 】"<<endl;

		sprintf(upsql,"update t_scs_speoper_list set stat = 'S1P',ret_code = '%s',ret_info = '%s',end_date = sysdate,order_id = '%s',event_seq = '%s' ,fail_times = %d where seq_id = %d",
			ret_code.c_str(),ret_info.c_str(),order_id.c_str(),event_seq.c_str(),ntimes,atol(seq_id.c_str()));
	}
	try
	{
		m_pConn->executeCommand(upsql);
		m_pConn->commitTrans();
		
		cout<<"【更新数据库语句:"<<upsql<<"】"<<endl;
	}
	catch (DatabaseException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMessage()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}
	catch (CDbException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMsg()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}

}

void OpCarryDatabase::updateRouterRule(string& ret_code,string& ret_info,string& times,string& seq_id)
{
	if (m_pConn == NULL)
	{
		return;
	}
	char upsql[256]   =  {0};
	cout<<"【自动提现路由失败】"<<endl;
	int ntimes  = atol(times.c_str()) + 1;

	sprintf(upsql,"update t_scs_speoper_list set stat = 'S1P',ret_code = '%s',ret_info = '%s',end_date = sysdate,fail_times = %d where seq_id = %s",
		ret_code.c_str(),ret_info.c_str(),ntimes,seq_id.c_str());

	try
	{
		m_pConn->executeCommand(upsql);
		m_pConn->commitTrans();

		cout<<"【更新数据库语句:"<<upsql<<"】"<<endl;
	}
	catch (DatabaseException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMessage()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}
	catch (CDbException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMsg()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}

}

void OpCarryDatabase::deleteRecord()
{
	try
	{
		char sql[1024]  =  {0};
		sprintf(sql,"SELECT SEQ_ID FROM T_SCS_SPEOPER_LIST WHERE TO_CHAR(END_DATE,'YYYYMMDD') = TO_CHAR(TRUNC(SYSDATE - 1),'YYYYMMDD') AND STAT IN ('S0P','S1P') AND SPEOPER_TYPE = '0007'");
		EntityBase et(*m_pConn);
		int ret = et.load(sql);
		et.close();
		
		if (ret)
		{
			cout<<"查询历史记录【"<<sql<<"】"<<endl;
			memset(sql,0,sizeof(sql));
			sprintf(sql,"DELETE T_SCS_SPEOPER_LIST WHERE STAT IN('S1P','S0P') AND TO_CHAR(END_DATE,'YYYYMMDD') = TO_CHAR(TRUNC(SYSDATE - 1),'YYYYMMDD') AND SPEOPER_TYPE = '0007'");
			m_pConn->executeCommand(sql);
			m_pConn->commitTrans();
			cout << sql<< " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		}
	}
	catch (DatabaseException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMessage()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}
	catch (CDbException* e)
	{
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMsg()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}

	return;
}

string OpCarryDatabase::setTermseq(const string& systime)
{
	string tradeSeq = "-1";
	char buff[256];
	memset(buff,0,sizeof(buff));
	int ret = 0;
	try
	{
		string curTime = GetTuxTime(0);
		sprintf(buff,"Auto%s%08d",(systime.substr(4,6)).c_str(),curTime.c_str());
		tradeSeq = buff;

	}catch (DatabaseException *e)
	{
		if (m_pConn)
			m_pConn->rollback();
		cout << e->getErrMessage() << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}
	catch (CDbException *e)
	{
		if (m_pConn)
			m_pConn->rollback();
		cout << e->getErrMsg()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}

	return tradeSeq;

}

string OpCarryDatabase::getCustomValue(const string& acct_id,const string& attr_id)
{
	string value1  =  "";
	char Sql[256]  =  {0};
	sprintf(Sql,"SELECT VALUE1 FROM T_CUM_ACCT_ATTR WHERE ACCT_ID = %s AND ATTR_ID = %s AND STAT = 'S0A'",acct_id.c_str(),attr_id.c_str());
	EntityBase et(*m_pConn);
	int ret = et.load(Sql);
	if (ret)
	{
		value1  =  et.getValue("VALUE1");
	}
	et.close();

	cout<<"【查询属性值:"<<Sql<<"】"<<endl;
	return value1;
}

string OpCarryDatabase::getBankCode(const string& bankId)
{
	string bankCode  =  "";
	char sql[1024];
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT BANK_CODE,BANK_TYPE FROM T_SYM_BANK WHERE BANK_ID = %s AND STAT = 'S0A'",bankId.c_str());
	EntityBase et(*m_pConn);
	if (et.load(sql))
	{
		bankCode = et.getValue("BANK_CODE");
	}
	et.close();
	cout<<"【查询银行编码:"<<sql<<"】"<<endl;
	return bankCode;
}

string OpCarryDatabase::getPrtnCode(const string& custCode)
{

	/*****************************************************
	******************************************************/
	//查询合作伙伴编码,是支付企业账户所属合作伙伴还是收款方合作伙伴
	string prtn_code = "";
	char sql[2048];
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT PRTN_CODE FROM T_PNM_PARTNER WHERE PRTN_ID = (SELECT PRTN_ID FROM T_CUM_INFO WHERE CUST_CODE = '%s' AND STAT = 'S0A') AND STAT = 'S0A'",custCode.c_str());

	EntityBase et2(*m_pConn);
	if (et2.load(sql) > 0)
	{
		prtn_code = et2.getValue("PRTN_CODE");
	}
	et2.close();

	cout<<"【查询合作伙伴编码:"<<sql<<"】"<<endl;
	return prtn_code;
}

string OpCarryDatabase::getCumSecLevel(const string& bankCode,const string& custCode)
{
	string strCumSecLevel = "";
	string strSQL = "SELECT LEVEL_ACCT FROM T_CUM_LEVEL_ACCT WHERE CUST_CODE = :CUST_CODE AND BANK_CODE = :BANK_CODE AND LEVEL_TYPE = 'L2' and STAT = 'S0A'";
	
	EntityBase entityBase(*m_pConn);
	entityBase.setCHAR(const_cast<char*>(custCode.c_str()));
	entityBase.setCHAR(const_cast<char*>(bankCode.c_str()));
	
	cout << "查询商户[" << custCode << "]是否有二级商户号(" << strSQL << "[CUST_CODE:" << custCode << ",BANK_CODE:" << bankCode << "])" << endl;

	try
	{
		int ret = entityBase.load(const_cast<char*>(strSQL.c_str()));
		if (ret > 0)
		{
			strCumSecLevel = entityBase.getValue("LEVEL_ACCT");
			cout << "查询到商户[" << custCode << "]的二级商户号为：" << strCumSecLevel << endl;
		}
		else if (ret == 0)
		{
			cout << "商户[" << custCode << "]没有二级商户号" << endl;
		}
	}
	catch (DatabaseException* ex)
	{
		cout << "DatabaseException:" << ex->getErrMessage() << endl;
		delete ex;
	}
	catch (CDbException* ex)
	{
		cout << "CDbException:" << ex->getErrMsg() << endl;
		delete ex;
	}
	catch (PEException* ex)
	{
		cout << "CDbException:" << ex->getErrMessage() << endl;
		delete ex;
	}catch(...)
	{
		cout << "未知异常!" << endl;
	}
	entityBase.close();

	return strCumSecLevel;
}
//获取卡折标示
string OpCarryDatabase::GetCardType(const string& obj_custcode)
{
    string strCardType="0";
    char sel_sql[512] = {0};
    EntityBase et(*m_pConn);

    try
    {
        sprintf(sel_sql," SELECT A.CARD_TYPE FROM T_CUM_ACCT A WHERE A.ACCT_TYPE = 'ACCT001' AND A.CUST_ID = (SELECT T.CUST_ID FROM T_CUM_INFO T WHERE T.CUST_CODE = :CUST_CODE   AND T.STAT = 'S0A') AND A.STAT='S0A' ");

        et.setCHAR( const_cast<char*>(obj_custcode.c_str()) );
        int ret  = et.load(sel_sql);
        if (ret>0)
        {
            strCardType  = et.getValue("CARD_TYPE");
            strCardType = TrimSp(strCardType);
            if ( 0 == atoi(strCardType.c_str()) )
            {
                strCardType="0";
            }
        }
    }
    catch (DatabaseException* ex)
    {
        cout << "DatabaseException:" << ex->getErrMessage() << endl;
        delete ex;
    }
    catch (CDbException* ex)
    {
        cout << "CDbException:" << ex->getErrMsg() << endl;
        delete ex;
    }
    catch (PEException* ex)
    {
        cout << "CDbException:" << ex->getErrMessage() << endl;
        delete ex;
    }catch(...)
    {
        cout << "未知异常!" << endl;
    }

    et.close();

    return strCardType=="0" ? "1" : strCardType;
}

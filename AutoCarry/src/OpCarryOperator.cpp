/********************************************************************
Copyright (C)  广东天讯电信科技有限公司
File Name:     OpCarryOperator.cpp
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

#include "OpCarryOperator.h"

#define RECHARGEID "1"
#define WITHDRAWID "5"

extern ConnectPool* gl_pConnectPool;

OpCarryOperator::OpCarryOperator()
{

}
OpCarryOperator::OpCarryOperator(Connection& conn):OpCarryDatabase(conn)
{

}
OpCarryOperator::~OpCarryOperator()
{
	if (m_pConn != NULL)
	{
		delete m_pConn;
		m_pConn = NULL;
	}

}

//****************************************************************
// Function name	: OpCarryOperator::startoperator
// Author	        : Tang Wang
// Coding Date      : 
// Description	    : 数据处理,执行服务
// Argument         : 
// Return type		: 
//****************************************************************
void OpCarryOperator::startoperator()
{

	//LP_STR_OPER_RECORD OperRecrd = NULL;
	

	if (m_pConn == NULL)
	{
		cout<<"【数据库链接失败,请先链接数据库】"<<endl;
		return;
	}
	//vector<CP_STR_OPER_RECORD> vec_operRecord;
        vector<CP_OPER_RECORD> vec_operRecord;
	vec_operRecord.clear();

	string querySql = "";

	/*querySql = "SELECT SP.SEQ_ID,SP.AREA_CODE,SP.CUST_ID,SP.CUST_CODE,SP.PROD_ID,SP.PROD_CODE,SP.ACTION_ID,SP.ACTION_CODE, SP.PAY_TYPE,SP.PAY_ORG,SP.ACCT_CODE,SP.PAY_MONEY, SP.OBJ_CODE,"
		         "SP.FAIL_TIMES,AC.ACCT_CODE AS OBJ_ACCT_CODE,AC.BANK_ID,AC.ACCT_NAME,ATTR.VALUE1 AS SIGN_ID "
		         "FROM T_SCS_SPEOPER_LIST SP,T_CUM_ACCT AC,T_CUM_ACCT_ATTR ATTR "
		         "WHERE SP.SPEOPER_TYPE = '0007' AND SP.STAT IN('S0A','S0W') AND SP.FAIL_TIMES < 2 AND AC.ACCT_TYPE = 'ACCT001' AND AC.STAT = 'S0A' AND AC.CUST_ID = SP.CUST_ID "
		         "AND AC.ACCT_ID = ATTR.ACCT_ID AND ATTR.ATTR_ID = 2810  AND ATTR.STAT = 'S0A'";*/

	querySql  =  "SELECT * FROM (SELECT SP.SEQ_ID,SP.AREA_CODE,SP.CUST_ID,SP.CUST_CODE,SP.PROD_ID,SP.PROD_CODE,SP.ACTION_ID,SP.ACTION_CODE, SP.PAY_TYPE,SP.PAY_ORG,SP.ACCT_CODE,SP.PAY_MONEY, SP.OBJ_CODE,SP.FAIL_TIMES,"
		"(select a.value1  from  T_CUM_ACCT_ATTR a,t_cum_acct b   where a.acct_id=b.acct_id  and a.attr_id=2569  and b.cust_id=SP.CUST_ID and  b.stat='S0A'  and b.acct_type='ACCT001' ) AS BANK_ACCT_CODE,"
		"(SELECT ACCT_CODE FROM T_CUM_ACCT WHERE ACCT_TYPE = 'ACCT001' AND CUST_ID = (SELECT CUST_ID FROM T_CUM_INFO WHERE CUST_CODE = SP.OBJ_CODE AND STAT = 'S0A') AND STAT = 'S0A') AS OBJ_ACCT_CODE,"
		"(SELECT BANK_ID FROM T_CUM_ACCT WHERE ACCT_TYPE = 'ACCT001' AND CUST_ID = (SELECT CUST_ID FROM T_CUM_INFO WHERE CUST_CODE = SP.OBJ_CODE AND STAT = 'S0A') AND STAT = 'S0A') AS BANK_ID,"
		"(SELECT ACCT_NAME FROM T_CUM_ACCT WHERE ACCT_TYPE = 'ACCT001' AND CUST_ID = (SELECT CUST_ID FROM T_CUM_INFO WHERE CUST_CODE = SP.OBJ_CODE AND STAT = 'S0A') AND STAT = 'S0A') AS OBJ_ACCT_NAME,"
		"(SELECT ACCT_ID FROM T_CUM_ACCT WHERE ACCT_TYPE = 'ACCT001' AND CUST_ID = (SELECT CUST_ID FROM T_CUM_INFO WHERE CUST_CODE = SP.OBJ_CODE AND STAT = 'S0A') AND STAT = 'S0A') AS OBJ_ACCT_ID,"
		"(SELECT VALUE1 FROM T_CUM_ACCT_ATTR WHERE ACCT_ID IN (SELECT ACCT_ID FROM T_CUM_ACCT WHERE ACCT_TYPE = 'ACCT001' AND CUST_ID = (SELECT CUST_ID FROM T_CUM_INFO "
		"WHERE CUST_CODE = SP.OBJ_CODE AND STAT = 'S0A') AND STAT = 'S0A') AND ATTR_ID = 2810 AND STAT = 'S0A') AS SIGN_ID "
		"FROM T_SCS_SPEOPER_LIST SP WHERE SP.SPEOPER_TYPE = '0007' AND SP.STAT = 'S0A' AND SP.FAIL_TIMES < 2 ) WHERE ROWNUM <= 30";

	int ret = 0;
	bool sflag = true;
	
	try
	{
		EntityBase base(*m_pConn);
		ret = base.load(const_cast<char*>(querySql.c_str()));
		if (ret>0)
		{
			cout << querySql<< " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
			while (ret > 0)
			{
				//CP_STR_OPER_RECORD OperRecrd  = new CP_OPER_RECORD();
                CP_OPER_RECORD OperRecrd;
				OperRecrd.seq_id        =     base.getValue("SEQ_ID");
				OperRecrd.area_code     =     base.getValue("AREA_CODE");
				OperRecrd.cust_id       =     base.getValue("CUST_ID");
				OperRecrd.cust_code     =     base.getValue("CUST_CODE");
				OperRecrd.prod_id       =     base.getValue("PROD_ID");
				OperRecrd.prod_code     =     base.getValue("PROD_CODE");
				OperRecrd.action_id     =     base.getValue("ACTION_ID");
				OperRecrd.action_code   =     base.getValue("ACTION_CODE");
				OperRecrd.pay_org       =     base.getValue("PAY_ORG");
				OperRecrd.pay_type      =     base.getValue("PAY_TYPE");
				OperRecrd.acct_code     =     base.getValue("ACCT_CODE");
				OperRecrd.bank_acct_code=     base.getValue("BANK_ACCT_CODE");
				OperRecrd.pay_money     =     base.getValue("PAY_MONEY");
				OperRecrd.obj_code      =     base.getValue("OBJ_CODE");
				OperRecrd.fail_times    =     base.getValue("FAIL_TIMES");
				OperRecrd.obj_acct_code =     base.getValue("OBJ_ACCT_CODE");
				OperRecrd.bank_id       =     base.getValue("BANK_ID");
				OperRecrd.obj_acct_name =     base.getValue("OBJ_ACCT_NAME");
				OperRecrd.sign_id       =     base.getValue("SIGN_ID");
				OperRecrd.obj_acct_id   =     base.getValue("OBJ_ACCT_ID");

				ret = base.next();
				vec_operRecord.push_back(OperRecrd);
			}
			base.close();
		}
		else
		{
			base.close();
			return;
		}
		
		//int n = 0;
		for (vector<CP_OPER_RECORD>::iterator ite = vec_operRecord.begin(); ite != vec_operRecord.end(); ite++)
		{

			//cout <<" 执行次数:  "<< ++n <<__FUNCTION__ << ":"<< __LINE__ << endl;
			seq_id           =  "";
			area_code        =  "";
			cust_id          =  "";
			cust_code        =  "";
			prod_id          =  "";
			prod_code        =  "";
			action_id        =  "";
			action_code      =  "";
			pay_org          =  "";
			pay_type         =  "";
			acct_code        =  "";
			bank_acct_code   =  "";
			pay_money        =  "";
			obj_code         =  "";
			fail_times       =  "";
			obj_acct_code    =  "";
			bank_id          =  "";
			obj_acct_name    =  "";
			sign_id          =  "";
			obj_acct_id      =  "";

			seq_id           =  (*ite).seq_id;
			area_code        =  (*ite).area_code;
			cust_id          =  (*ite).cust_id;
			cust_code        =  (*ite).cust_code;
			prod_id          =  (*ite).prod_id;
			prod_code        =  (*ite).prod_code;
			action_id        =  (*ite).action_id;
			action_code      =  (*ite).action_code;
			pay_org          =  (*ite).pay_org;
			pay_type         =  (*ite).pay_type;
			acct_code        =  (*ite).acct_code;
			bank_acct_code   =  (*ite).bank_acct_code;
			pay_money        =  (*ite).pay_money;
			obj_code         =  (*ite).obj_code;
			fail_times       =  (*ite).fail_times;
			obj_acct_code    =  (*ite).obj_acct_code;
			bank_id          =  (*ite).bank_id;
			obj_acct_name    =  (*ite).obj_acct_name;
			sign_id          =  (*ite).sign_id;
			obj_acct_id      =  (*ite).obj_acct_id;


			/******************************************************/
			/////////******************////////////////////////////
            //   调用一次路由 → 成功 → 调用SCS0001进行提现操作
			//         ↓
			//        失败 → 未配置路由 → 不进行提现,跳出本次提现操作 
			////////******************/////////////////////////////
			
			sflag  =  getRouterRule(bank_id,obj_code);
			if (sflag == false)  //路由失败,则更新状态为失败.
			{
				continue;
			}

			//查询商户二级商户号
			m_strCumSecLevel = getCumSecLevel(pay_org_bank,cust_code);

			current_date  =  GetTuxTime(0);
			m_ppIntpar = setParamarser();
			
			int retS = callService("SCS0001",m_ppIntpar,m_ppOutpar);
                        if (retS == -2) continue;

			ParamSet * retSet = m_ppOutpar.getParamSet("000");
			if (retSet == NULL)   //如果调用服务发生异常则不更新此次操作
			{
				continue;
			}
			updateoperator(m_ppOutpar,fail_times,seq_id);

			/*****************************************************/
		}

		/*for(vector<CP_STR_OPER_RECORD>::iterator ite = vec_operRecord.begin(); ite != vec_operRecord.end(); ite++)
		{
			if(*ite)
			{
				delete (*ite);
				(*ite) = NULL;
			}
		}
                */

	}
	catch (DatabaseException* e)
	{
		//cleaniterator();
		vec_operRecord.clear();
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMessage()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;
	}
	catch (CDbException* e)
	{
		//cleaniterator();
		vec_operRecord.clear();
		if(m_pConn) m_pConn->rollback();
		cout << e->getErrMsg()  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
		delete e;

	}catch (...)
	{
		//cleaniterator();
		vec_operRecord.clear();
		if(m_pConn) m_pConn->rollback();
		cout << "未知异常"  << " " <<__FUNCTION__ << ":"<< __LINE__ << endl;
	}

	//deleteRecord();
        vec_operRecord.clear();

	return;
}


bool OpCarryOperator::getRouterRule(const string& bankId,const string& custCode)
{

	if (m_pConn == NULL)
	{
		return false;
	}
	bool flag   =  true;
    string strCartType = "";
    string strPrivateType = "";
	
	try
	{
	    //scs0019改造，增加卡折标识与对公/对私标识字段
	    flag = GetCartTypeAndPrivateType(custCode, strCartType, strPrivateType);
        if (false == flag)
        {
            cout<<"AutoCarry,getRouterRule,GetCartTypeAndPrivateType failed!"<<endl;
        }

		bank_code  =  getBankCode(bankId);
		prtn_code  =  getPrtnCode(obj_code);

		ParamParser m_inpar;
		ParamSet set404("404");
		set404.appendRow();
		set404.appendDataItem("4004",cust_code);
		set404.appendDataItem("4007","000006000003");   //终端号 给定值
		set404.appendDataItem("4144","90");
		set404.appendDataItem("4006",area_code);
		set404.appendDataItem("4051",action_code);
		set404.appendDataItem("4052",obj_code);
		set404.close();

		m_inpar.addParamSet(&set404);

		ParamSet set408("408");
		set408.appendRow();
        //scs0019改造，增加卡折标识与对公/对私标识字段
        set408.appendDataItem("4106",strCartType);
        set408.appendDataItem("4124",strPrivateType);
        
		set408.appendDataItem("2011",prtn_code);
		set408.appendDataItem("4023",pay_money);
		set408.appendDataItem("4097","PT1004");      //银行提现,支付类型
		set408.appendDataItem("4098",bank_code);     //银行机构编码
		//20151016 add 填银行账户开户行字段
		set408.appendDataItem("4100", bank_acct_code);    //银行账户开户行
		set408.close();

		m_inpar.addParamSet(&set408);

		ParamParser m_outpar;

		int retcall = callService("SCS0019",m_inpar,m_outpar);
                if (retcall == -2) return false;

		ParamSet* paramSet  =  NULL;
		string ret_code     =  "";
		string ret_info     =  "";
		paramSet  =  m_outpar.getParamSet("000");
		if (paramSet != NULL)
		{
			ret_code  =  paramSet->getDataItem("0001");
			ret_info  =  paramSet->getDataItem("0002");
		}
		else
		{
			flag  =  false;
		}
		if (ret_code.compare("0000") != 0)
		{
			flag  =  false;
		}

		paramSet  =  m_outpar.getParamSet("423");
		if (paramSet != NULL)
		{
			pay_org_bank   =  paramSet->getDataItem("4098");
		}
		if ( flag == false )
			updateRouterRule(ret_code,ret_info,fail_times,seq_id);

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


	return flag;

}

ParamSet OpCarryOperator::setParamset200(const string& strCustCode)
{
    ParamSet setCum0003_200("200");

    setCum0003_200.appendRow();
	setCum0003_200.appendDataItem("2002",strCustCode);
    setCum0003_200.close();
    return setCum0003_200;
}

ParamSet OpCarryOperator::setParamset002()
{
    ParamSet setCum0003_002("002");

    setCum0003_002.appendRow();
	setCum0003_002.appendDataItem("0011","207");
    setCum0003_002.close();
    return setCum0003_002;
}

bool OpCarryOperator::GetCartTypeAndPrivateType(const string& strCustCode, string& strCartType, string& strPrivateType)
{
    bool bRet = true;
    const string strServiceName = "CUM0003";
    int nServiceRet = 0;
    string strAcctType = "";
    string strAttrType = "";
    
    ParamParser m_inpar;
    ParamParser m_outpar;
    ParamSet* paramSetOut = NULL;
    ParamSet* paramSet207  =  NULL;
    ParamSet* paramSet216  =  NULL;

    cout<<"make up CUM0003"<<endl;
    // 组CUM0003报文
    ParamSet setCum0003_200 = setParamset200(strCustCode);
    m_inpar.addParamSet(&setCum0003_200);

    ParamSet setCum0003_002 = setParamset002();
    m_inpar.addParamSet(&setCum0003_002);

    cout<<"call CUM0003"<<endl;
    // 调用CUM0003接口
    nServiceRet = callService(strServiceName, m_inpar, m_outpar);
    if (-2 == nServiceRet)
    {
        cout<<"CUM0003调用失败"<<endl;
        return false;
    }

    cout<<"call CUM0003 000"<<endl;
    // 判断返回值
	string ret_code     =  "";
	string ret_info     =  "";
	paramSetOut  =  m_outpar.getParamSet("000");
	if (paramSetOut != NULL)
	{
		ret_code  =  paramSetOut->getDataItem("0001");
		ret_info  =  paramSetOut->getDataItem("0002");
	}
	else
	{
		return false;
	}
	if (ret_code.compare("0000") != 0)
	{
	    cout<<"返回码:"<<ret_code<<" "<<"错误信息"<<ret_info<<endl;
		return false;
	}
    
    cout<<"call CUM0003 207"<<endl;
    // 响应报文中207组获取卡折标识
    paramSet207 = m_outpar.getParamSet("207");
    if (NULL != paramSet207)
    {
        while (!paramSet207->isEOF())
        {
            strAcctType = paramSet207->getDataItem("2048");
            if ("ACCT001" == strAcctType)
            {
                strCartType = paramSet207->getDataItem("2052");
                cout<<"get strCartType"<<endl;
            }
            paramSet207->moveNext();
        }
        
    }

    cout<<"call CUM0003 216"<<endl;
    // 响应报文中216组获取对公对私标识
    paramSet216 = m_outpar.getParamSet("216");
    if (NULL != paramSet216)
    {
        while (!paramSet216->isEOF())
        {
            strAttrType = paramSet216->getDataItem("2069");
            if ("2570" == strAttrType)
            {
                strPrivateType = paramSet216->getDataItem("2071");
                cout<<"get strPrivateType"<<endl;
            }
            paramSet216->moveNext();
        }
        
    }
    return true;
}

ParamParser OpCarryOperator::setParamarser()
{
	ParamParser ppIntpar;
	ParamSet set401 = setParamset401();
	ppIntpar.addParamSet(&set401);

	ParamSet set402 = setParamset402();
	ppIntpar.addParamSet(&set402);

	ParamSet set404 = setParamset404();
	ppIntpar.addParamSet(&set404);

	ParamSet set405 = setParamset405();
	ppIntpar.addParamSet(&set405);

	ParamSet set407 = setParamset407();
	ppIntpar.addParamSet(&set407);

	ParamSet set408 = setParamset408();
	ppIntpar.addParamSet(&set408);

	return ppIntpar;
}

ParamSet OpCarryOperator::setParamset401(){

	string begin_time = GetTuxTime(0);
	string term_seq   = setTermseq(begin_time);
	string cust_name  = "";
	char Sql[256];
	memset(Sql,0,sizeof(Sql));
	sprintf(Sql,"SELECT CUST_NAME FROM T_CUM_INFO WHERE CUST_CODE = '%s' AND STAT = 'S0A'",cust_code.c_str());
	EntityBase et(*m_pConn);
	int ret = et.load(Sql);
	if (ret>0)
	{
		cust_name  = et.getValue("CUST_NAME");
	}
	et.close();

	ParamSet set401("401");
	set401.appendRow();
	set401.appendDataItem("4004",cust_code);
	set401.appendDataItem("4005","OT001");
	set401.appendDataItem("4006",area_code);
	//set401.appendDataItem("4007","123456");      //给定值 000006000003
	set401.appendDataItem("4007","000006000003");  //给定值:000006000003
	set401.appendDataItem("4008",begin_time);
	set401.appendDataItem("4016","LG001");
	set401.appendDataItem("4144","00");
	set401.appendDataItem("4017",term_seq);
	set401.appendDataItem("4018","0.0.0.0");
	set401.appendDataItem("4146","Auto");
	set401.appendDataItem("4012","自动提现");
	set401.appendDataItem("4280",cust_name);
	set401.close();

	return set401;
}


ParamSet OpCarryOperator::setParamset402(){

	ParamSet set402("402");
	set402.appendRow();
	set402.appendDataItem("4021","0001");
	set402.appendDataItem("4023",pay_money);
	set402.appendDataItem("4024","0");
	set402.appendDataItem("4025",pay_money);
	set402.close();
	
	return set402;
}


ParamSet OpCarryOperator::setParamset404(){

	ParamSet set404("404");
	set404.appendRow();
	set404.appendDataItem("4047","1");
	set404.appendDataItem("4049",prod_code);
	set404.appendDataItem("4051",action_code);
	//set404.appendDataItem("4052",obj_code);
	set404.appendDataItem("4052",obj_acct_name);
	set404.appendDataItem("4053","1");
	set404.close();
	
	return set404;
}


ParamSet OpCarryOperator::setParamset405(){

	ParamSet set405("405");
	set405.appendRow();
	set405.appendDataItem("4047","1");
	set405.appendDataItem("4021","0001");
	set405.appendDataItem("4066",pay_money); //金额
	set405.appendDataItem("4067","0");
	set405.appendDataItem("4068",pay_money); //金额
	set405.appendDataItem("4071","101");
	set405.close();
	
	return set405;
}


ParamSet OpCarryOperator::setParamset407()
{

	string address  =  "";
	string phone    =  "";
	string bank_area =  "";
	string cert_code  =  "";
	string cert_type  =  "";
	string card_flag  =  "";
	address  =  getCustomValue(obj_acct_id,"8038");
	phone    =  getCustomValue(obj_acct_id,"2814");
	bank_area  = getCustomValue(obj_acct_id,"2568");
	cert_code  = getCustomValue(obj_acct_id,"2567");
	cert_type  = getCustomValue(obj_acct_id,"2566");
	card_flag  = getCustomValue(obj_acct_id,"2570");
	

        if(address.empty()==true) address = "00";
        if(phone.empty() == true) phone = "00";
	ParamSet set407("407");
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_TRADETYPE");
	set407.appendDataItem("4088","020011");
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_CERTID");
	set407.appendDataItem("4088",cert_type);
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_CERTCODE");
	set407.appendDataItem("4088",cert_code);
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_BANKID");
	set407.appendDataItem("4088",bank_code);  //bank_code
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_BANKCARDID");
    string strCard_Type = GetCardType(obj_code);
    cout<<"卡折标示为:"<<strCard_Type<<endl;
    set407.appendDataItem("4088",strCard_Type);    
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_BANKCARDTYPE");
	set407.appendDataItem("4088",card_flag);    //银行卡标识 对公/对私
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_DEALTYPE");
	set407.appendDataItem("4088","0200");
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_DSTCAACCODE");
	set407.appendDataItem("4088",obj_acct_code);
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_CARDEFFDATE");
	set407.appendDataItem("4088",current_date); //时间
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_CARDCHECKDATE");
	set407.appendDataItem("4088",current_date); //时间
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_PHONENUM");
	set407.appendDataItem("4088",phone); //电话号码
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_ADDREASS");
	set407.appendDataItem("4088",address); //地址
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_BANKBELONG");
	set407.appendDataItem("4088",bank_area);  //账户归属地
	set407.appendDataItem("4080","0");

	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_CREDITEFFDATE");
	set407.appendDataItem("4088",current_date); //时间
	set407.appendDataItem("4080","0");
	
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051","01020003");
	set407.appendDataItem("4087","SCS_CREDITVERIFYCODE");
	set407.appendDataItem("4088","123456");
	set407.appendDataItem("4080","0");
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_PAYTYPE");
	set407.appendDataItem("4088","PT1004"); //支付类型
	set407.appendDataItem("4080","0");

	if (sign_id != "")
	{
		set407.appendRow();
		set407.appendDataItem("4047","1");
		set407.appendDataItem("4051",action_code);
		set407.appendDataItem("4087","SCS_SIGNID");
		set407.appendDataItem("4088",sign_id); //签约ID
		set407.appendDataItem("4080","0");
	}

	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_PAYORG");
	set407.appendDataItem("4088",pay_org_bank);    //支付机构
	set407.appendDataItem("4080","0");

	if (!m_strCumSecLevel.empty())
	{
		set407.appendRow();
		set407.appendDataItem("4047","1");
		set407.appendDataItem("4051",action_code);
		set407.appendDataItem("4087","CUM_LEVTWOCUST");
		set407.appendDataItem("4088",m_strCumSecLevel);    //二级商户号
		set407.appendDataItem("4080","0");
	}
	//
	set407.appendRow();
	set407.appendDataItem("4047","1");
	set407.appendDataItem("4051",action_code);
	set407.appendDataItem("4087","SCS_BANKNAME");
	set407.appendDataItem("4088",obj_acct_name);    //银行帐号户名
	set407.appendDataItem("4080","0");
	
	set407.close();
	
	return set407;
}


ParamSet OpCarryOperator::setParamset408(){

	
	ParamSet set408("408");
	set408.appendRow();
	set408.appendDataItem("4103","1");
	set408.appendDataItem("4097",pay_type);  
	set408.appendDataItem("4098",pay_org);   
	set408.appendDataItem("4099","0001");
	set408.appendDataItem("4101",acct_code);
	set408.appendDataItem("4102","123456");
	set408.appendDataItem("4021","0001");
	set408.appendDataItem("4104",pay_money);
	set408.appendDataItem("4100", bank_acct_code);
	set408.close();
	
	return set408;
}

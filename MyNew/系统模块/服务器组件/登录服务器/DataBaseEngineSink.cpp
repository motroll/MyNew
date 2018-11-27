#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//������Ϣ
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�ر�����
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//�ر�����
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnRequestLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VISITOR_ACCOUNTS:		//�ο͵�¼
		{
			return OnRequestLogonVisitor(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_MACHINE:			//�޸Ļ���
		{
			return OnRequestModifyMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_PUBLIC_NOTIC:			//�Զ�������
		{
			return OnRequestPublicNotic(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_LOGON_PASS:		//�޸�����
		{
			return OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//�޸�����
		{
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_ACCOUNTS:		//�޸��ʺ�
		{
			return OnRequestModifyAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SPREADER:		//�޸�����
		{
			return OnRequestModifySpreader(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_UNDER_WRITE:		//�޸�ǩ��
		{
			return OnRequestModifyUnderWrite(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_CUSTOM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifyCustomFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_STARVALUE:		//�޸�����
		{
			return OnRequestModifyStarValue(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SHARE_AWARD:
		{
			return OnRequestShareAward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INGAME_SERVERID:		//��Ϸ״̬
		{
			return OnRequestUserInGameServerID(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CHECK_PAY_INFO://�������д�
		{
			return OnCheckAllPayList(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GP_USER_SAVE_SCORE:		//������Ϸ��
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//��ȡ��Ϸ��
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_SCORE:	//ת����Ϸ��
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INDIVIDUAL:		//��ѯ����
		{
			return OnRequestQueryIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_ACCOUNTINFO:		//��ѯ����
		{
			return OnRequestQueryAccountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//��ѯ����
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INSURE_HISTORY:	// ��ѯ��ʷ
		{
			return OnRequestQueryInsureHistory(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_INFO:		//��ѯ�û�
		{
			return OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//�����б�
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//������Ϣ
		{
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKIN_QUERY_INFO:		//��ѯǩ��
		{
			return OnRequestCheckInQueryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BEGINNER_QUERY_INFO:		//��ѯǩ��
		{
			return OnRequestBeginnerQueryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BEGINNER_DONE:		//��ѯǩ��
		{
			return OnRequestBeginnerDone(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_ADDRANK_CONFIG:		
		{
			return OnRequestAddBankAwardConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ADDRANK_UP_AWARD:		
		{
			return OnRequestAddBackUpAward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_ADDRANK:		
		{
			return OnRequestGetAddBank(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_CHECKIN_REWARD:	//ǩ������
		{
			return OnRequestCheckInReward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKIN_DONE:			//ִ��ǩ��
		{
			return OnRequestCheckInDone(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKIN_AWARD:			//ִ��ǩ��
		{
			return OnRequestCheckAward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_ROULETE_AWARD:			//ִ��ǩ��
		{
			return OnRequestRouletteReward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ROULETE_DONE:			//ִ��ǩ��
		{
			return OnRequestRouletteDone(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GAME_CONFIG:			//ִ��ǩ��
		{
			return OnRequestGameConfig(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ROULETE_USERINFO:			//ִ��ǩ��
		{
			return OnRequestUserRouletteAward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_BASEENSURE:		//���صͱ�
		{
			return OnRequestLoadBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_BEGINNER:		//���صͱ�
		{
			return OnRequestBeginnerConfig(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BASEENSURE_TAKE:		//��ȡ�ͱ�
		{
			return OnRequestTakeBaseEnsure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MATCH_SIGNUP:			//��������
		{
			return OnRequestMatchSignup(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MATCH_UNSIGNUP:			//ȡ������
		{
			return OnRequestMatchUnSignup(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MATCH_AWARD:			//ȡ������
		{
			return OnRequestLoadMatchReward(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_EXCHANGE_HUAFEI:
		{
			 return OnRequestGetExchangeHuaFeiList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EXCHANGE_DONE:
		{
			return OnRequestExchangeHuaFeiDone(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_SHOPINFO:
		{
			return OnRequestGetShopInfoList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_BUYODER:
		{
			return OnRequestGetBuyOder(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SELL_ITEM:
		{
			return OnRequestSellItem(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_SELL_RECORD:
		{
			return OnRequestGetSellRecordList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GAME_RECORD_LIST:
		{
			return OnRequestGameRecordList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GAME_TOTAL_RECORD:
		{
			return OnRequestGameTotalRecord(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GAME_TOTAL_CHILD:
		{
			return OnRequestGameChildRecord(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CREATE_CLUB: // �᳤�������ֲ�phw
		{
			return  OnRequestCreateClub(dwContextID,pData,wDataSize);
		}
	case DBR_GR_JOIN_CLUB: // ���������ֲ�
		{
			return OnRequestJoinClub(dwContextID,pData,wDataSize);
		}
	case DBR_GR_JOIN_CLUB_COMFIRM: // �᳤ͬ���ͬ�������ֲ�
		{
			return OnRequestJoinClubComfirm(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LEAVE_CLUB: // �뿪���ֲ�
		{
			return OnRequestLeaveClub(dwContextID,pData,wDataSize);
		}
	case DBR_GR_KICK_CLUB: // �߳����ֲ�
		{
			return OnRequestKickClub(dwContextID,pData,wDataSize);
		}
	case DBR_GR_INVITE_CLUB: // ������Ҽ�����ֲ�
		{
			return OnRequestInviteClub(dwContextID,pData,wDataSize);
		}
	case   DBR_GR_GET_MY_CLUB_REQ://��ȡ�Լ��ľ��ֲ�����
		{
			return  OnRequestGetMyClubReq(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CLUB_MEMBER_LIST_REQ: // ���ֲ���Ա�б�����
		{
			return OnRequestSubMmemberListReq(dwContextID,pData,wDataSize);
		}
	case DBR_GR_UPDATE_CLUB_REQ: // �᳤���¾��ֲ�����
		{
			return OnRequestSubUpdateClubReq(dwContextID,pData,wDataSize);
		}
	case DBR_GR_DISMISS_CLUB_REQ: // ��ɢ���ֲ�����
		{
			return OnRequestSubDismissClubReq(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CLUB_CHANGE_MASTER_REQ: // ת�û᳤����
		{
			return OnRequestSubChangeMasterReq(dwContextID,pData,wDataSize);
		}
	case  DBR_GR_CLUB_SET_MANAGER_REQ: // �᳤���ù���Ա����
		{
			return OnRequestSubSetManagerReq(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

		//������
		DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if(!pBindParameter) return false;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonVisitor));
	if (wDataSize!=sizeof(DBR_GP_LogonVisitor)) return false;

	//ִ�в�ѯ
	DBR_GP_LogonVisitor * pLogonVisitor=(DBR_GP_LogonVisitor *)pData;

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&pLogonVisitor->dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pLogonVisitor->szComputerID);

	LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyVisitorID"),true);

	if (lReturnValue==0L)
	{
		TCHAR		szAccounts[LEN_NICKNAME];				//��¼�ʺ�
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),szAccounts,CountArray(szAccounts));

		DBR_GP_RegisterAccounts kRegister;
		ZeroMemory(&kRegister,sizeof(kRegister));
		kRegister.pBindParameter = pLogonVisitor->pBindParameter;
		lstrcpyn(kRegister.szAccounts,szAccounts,CountArray(kRegister.szAccounts));
		lstrcpyn(kRegister.szNickName,pLogonVisitor->szNickName,CountArray(kRegister.szNickName));
		lstrcpyn(kRegister.szLogonPass,pLogonVisitor->szPassWord,CountArray(kRegister.szLogonPass));
		lstrcpyn(kRegister.szInsurePass,pLogonVisitor->szPassWordBank,CountArray(kRegister.szInsurePass));
		lstrcpyn(kRegister.szMachineID,pLogonVisitor->szComputerID,CountArray(kRegister.szMachineID));
		kRegister.dwClientAddr = pLogonVisitor->dwClientIP;
		kRegister.wFaceID = pLogonVisitor->wFaceID;
		kRegister.cbGender = pLogonVisitor->cbGender;

		OnRequestRegisterAccounts(dwContextID,&kRegister,sizeof(kRegister));
	}
	else
	{

		DBO_MB_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//��������
		LogonFailure.lResultCode=lReturnValue;
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonFailure.szDescribeString,CountArray(LogonFailure.szDescribeString));
		//���ͽ��
		WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		return true;
	}
	return false;
}
//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//������
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//������
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//������
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�󶨲���
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//�������
		if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
		{
			//��������
			DBO_GP_OperateSuccess OperateSuccess;
			ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
			WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//������
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
		if (wDataSize!=sizeof(DBR_GP_ModifyCustomFace)) return false;

		//������
		DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//��������
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

		//���ñ���
		SafeArrayBound.lLbound=0L;
		SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

		//��������
		BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
		SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

		//��������
		for (LONG nIndex=0;nIndex<sizeof(pModifyCustomFace->dwCustomFace);nIndex++)
		{
			SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
		}

		//���ñ���
		VARIANT VarChunk;
		VariantClear(&VarChunk);
		VarChunk.parray=pSafeArray;
		VarChunk.vt=VT_ARRAY|VT_UI1;

		//��ȡ����
		ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
		IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

		//ͷ����Ϣ
		pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�ͷ���Դ
		SafeArrayDestroy(pSafeArray);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=INVALID_WORD;
			UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			
			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//������
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//������
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸��ʺ�
bool CDataBaseEngineSink::OnRequestModifyAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyAccounts));
		if (wDataSize!=sizeof(DBR_GP_ModifyAccounts)) return false;

		//������
		DBR_GP_ModifyAccounts * pModifyInsurePass=(DBR_GP_ModifyAccounts *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesAccounts"),pModifyInsurePass->szDesAccount);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyAccount"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}
//�޸��Ƽ���
bool CDataBaseEngineSink::OnRequestModifySpreader(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySpreader));
		if (wDataSize!=sizeof(DBR_GP_ModifySpreader)) return false;

		//������
		DBR_GP_ModifySpreader * pModifyUnderWrite=(DBR_GP_ModifySpreader *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@dwSpreader"),atoi(pModifyUnderWrite->szSpreader));
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifySpreader"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);		

		DBO_GP_SpreaderResoult OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//�������
		OperateSuccess.lResultCode=lResultCode;
		if (lResultCode == DB_SUCCESS)
		{
			OperateSuccess.lScore=m_AccountsDBAide.GetValue_INT(TEXT("SpreaderID"));
		}
		lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SPREADER_RESOULT,dwContextID,&OperateSuccess,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}
//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
		if (wDataSize!=sizeof(DBR_GP_ModifyUnderWrite)) return false;

		//������
		DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//������
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
		m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
		m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
		m_AccountsDBAide.AddParameter(TEXT("@strHeadHttp"),pModifyIndividual->szHeadHttp);
		m_AccountsDBAide.AddParameter(TEXT("@strUserChannel"),pModifyIndividual->szUserChannel);
		m_AccountsDBAide.AddParameter(TEXT("@strUserGps"),pModifyIndividual->szUserGps);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestModifyStarValue(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyStarValue));
		if (wDataSize!=sizeof(DBR_GP_ModifyStarValue)) return false;

		//������
		DBR_GP_ModifyStarValue * pModifyIndividual=(DBR_GP_ModifyStarValue *)pData;


		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwStarValue"),pModifyIndividual->dwStarValue);
		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserStarValue"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
	
}

//��������Ȧ����
bool CDataBaseEngineSink::OnRequestShareAward(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ShareAward));
		if (wDataSize!=sizeof(DBR_GP_ShareAward)) return false;

		//������
		DBR_GP_ShareAward * pModifyParent=(DBR_GP_ShareAward *)pData;


		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyParent->dwUserID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_ShareCirclAward"),true);

		if(lResultCode == DB_SUCCESS)
		{
			//��������
			DBO_GP_AWARD_RETURN kUserParent;
			ZeroMemory(&kUserParent,sizeof(kUserParent));

			//��������
			kUserParent.dwUserID = m_AccountsDBAide.GetValue_WORD(TEXT("UserID"));
			kUserParent.dwInsure = m_AccountsDBAide.GetValue_WORD(TEXT("InsureScore"));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_INSURE_RETURN,dwContextID,&kUserParent,sizeof(kUserParent));
		}

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestUserInGameServerID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryUserInGameServerID));
		if (wDataSize!=sizeof(DBR_GP_QueryUserInGameServerID)) return false;

		//������
		DBR_GP_QueryUserInGameServerID * pModifyIndividual=(DBR_GP_QueryUserInGameServerID *)pData;


		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserInGameServerID"),true);

		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInGameServerID kGPNetInfo;
			ZeroMemory(&kGPNetInfo,sizeof(kGPNetInfo));

			//�������
			kGPNetInfo.LockKindID=m_AccountsDBAide.GetValue_DWORD(TEXT("LockKindID"));
			kGPNetInfo.LockServerID=m_AccountsDBAide.GetValue_DWORD(TEXT("LockServerID"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INGAME_SERVER_ID,dwContextID,&kGPNetInfo,sizeof(kGPNetInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestPublicNotic(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_PublicNotic));
		if (wDataSize!=sizeof(DBR_GP_PublicNotic)) return false;

		//������
		DBR_GP_PublicNotic * pDBNetInfo=(DBR_GP_PublicNotic *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strKeyName"),pDBNetInfo->szKeyName);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_PublicNotic"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		if (lResultCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_PublicNotice kGPNetInfo;
			ZeroMemory(&kGPNetInfo,sizeof(kGPNetInfo));

			//�������
			kGPNetInfo.lResultCode=lResultCode;
			lstrcpyn(kGPNetInfo.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(kGPNetInfo.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(kGPNetInfo.szDescribeString);
			WORD wHeadSize=sizeof(kGPNetInfo)-sizeof(kGPNetInfo.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PUBLIC_NOTICE,dwContextID,&kGPNetInfo,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//�������
			OperateFailure.lResultCode=lResultCode;
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//������
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),(int)pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//������
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//������
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// ��ѯ��ֵ�б�����ֵ
bool CDataBaseEngineSink::OnCheckAllPayList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������
	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_GetPayList"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��ȡ��¼
			std::vector<tagPayInfo> infos;
			while(m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				tagPayInfo info;
				info.rid = m_TreasureDBAide.GetValue_WORD(TEXT("roleid"));
				info. pid = m_TreasureDBAide.GetValue_WORD(TEXT("productid"));
				info. isband = m_TreasureDBAide.GetValue_WORD(TEXT("isband"));
				m_TreasureDBAide.GetValue_String(TEXT("orderId"),info.szOrderId,CountArray(info.szOrderId));	
				infos.push_back(info);
				//�ƶ��α� 
				m_TreasureDBModule->MoveToNext();
			}

			// ��ֵ
			for (std::size_t i = 0; i!= infos.size(); ++i)
			{
				tagPayInfo& info = infos[i];
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),info.rid);
				m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),PidToInsureScore(info.pid) );
				DWORD dstscore = info.isband > 0 ? SrcInsureToBandInsure(PidToInsureScore(info.pid)) : PidToInsureScore(info.pid);
				m_TreasureDBAide.AddParameter(TEXT("@DstScore"), dstscore );
				m_TreasureDBAide.AddParameter(TEXT("@pid"),info.pid);
				m_TreasureDBAide.AddParameter(TEXT("@rechargekey"),info.szOrderId);
				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_AddInsureScore"),true);
				if (lResultCode ==DB_SUCCESS )
				{
					// ֪ͨ,������
					// OnPayDisposeResult(dwContextID,lResultCode,0,_T(""));

					// ����֧��״̬
					m_TreasureDBAide.ResetParameter();
					m_TreasureDBAide.AddParameter(TEXT("@orderid"),info.szOrderId);
					//�������
					TCHAR szDescribeString[128]=TEXT("");
					m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

					//ִ�в�ѯ
					LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_SetPayFlg"),true);
				}
			}
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ѯ������Ϣ
bool CDataBaseEngineSink::OnRequestQueryAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryAccountInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryAccountInfo)) return false;

		//������
		DBR_GP_QueryAccountInfo * pQueryAccountInfo=(DBR_GP_QueryAccountInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryAccountInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryAccountInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserAccountInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserAccountInfo kUserAccountInfo;
			ZeroMemory(&kUserAccountInfo,sizeof(kUserAccountInfo));

			//��������
			kUserAccountInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			kUserAccountInfo.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			kUserAccountInfo.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			kUserAccountInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			kUserAccountInfo.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			kUserAccountInfo.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			kUserAccountInfo.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			kUserAccountInfo.dwSpreaderID=m_AccountsDBAide.GetValue_DWORD(TEXT("SpreaderID"));

			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),kUserAccountInfo.szAccounts,CountArray(kUserAccountInfo.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),kUserAccountInfo.szNickName,CountArray(kUserAccountInfo.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("LogonIP"),kUserAccountInfo.szLogonIp,CountArray(kUserAccountInfo.szLogonIp));

			//�û��ɼ�
			kUserAccountInfo.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			kUserAccountInfo.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			kUserAccountInfo.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			kUserAccountInfo.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),kUserAccountInfo.szUnderWrite,CountArray(kUserAccountInfo.szUnderWrite));

			//��Ա����
			kUserAccountInfo.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),kUserAccountInfo.MemberOverDate);

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_ACCOUNTINFO,dwContextID,&kUserAccountInfo,sizeof(kUserAccountInfo));
		}
		else
		{
			//������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),szDescribeString,false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}
//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
		if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

		//������
		DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserIndividual UserIndividual;
			ZeroMemory(&UserIndividual,sizeof(UserIndividual));

			//�û���Ϣ
			UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

			//�绰����
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

			//��ϵ����
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));
			m_AccountsDBAide.GetValue_String(TEXT("HeadHttp"),UserIndividual.szHeadHttp,CountArray(UserIndividual.szHeadHttp));
			m_AccountsDBAide.GetValue_String(TEXT("UserChannel"),UserIndividual.szUserChannel,CountArray(UserIndividual.szUserChannel));
			m_AccountsDBAide.GetValue_String(TEXT("UserIP"),UserIndividual.szLogonIP,CountArray(UserIndividual.szLogonIP));
			m_AccountsDBAide.GetValue_String(TEXT("UserGps"),UserIndividual.szUserGps,CountArray(UserIndividual.szUserGps));
			UserIndividual.dwUserStarValue = m_AccountsDBAide.GetValue_DWORD(TEXT("UserStarValue"));
			UserIndividual.dwUserStarCout = m_AccountsDBAide.GetValue_DWORD(TEXT("UserStarCout"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
	return true;
}

//��ѯ��ֵ��ʷ
bool CDataBaseEngineSink::OnRequestQueryInsureHistory(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureHestory));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureHestory)) return false;

		//������
		DBR_GP_QueryInsureHestory * pQueryInsureInfo=(DBR_GP_QueryInsureHestory *)pData;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		
		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_GetRecordInsure"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInsureHistory UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			// �������н��
			int i= 0;
			while (m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				if (i >= 100)
				{
					break;
				}
				SRechargeInsure& info = UserInsureInfo.infos[i++];
				info.m_rechargeid = m_TreasureDBAide.GetValue_WORD(TEXT("RecordID"));
				info.m_add_insure = m_TreasureDBAide.GetValue_WORD(TEXT("SwapScore"));
				info.m_get_insure = m_TreasureDBAide.GetValue_WORD(TEXT("TargetBank"));
				info.m_insure_type = m_TreasureDBAide.GetValue_WORD(TEXT("KindID"));
				m_TreasureDBAide.GetValue_String(TEXT("CollectNote"), info.szRechargeKey, CountArray(info.szRechargeKey));
				m_TreasureDBAide.GetValue_SystemTime(TEXT("CollectDate"),info.m_insure_time);

				//�ƶ���¼
				m_TreasureDBModule->MoveToNext();
			}

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_HISTORY,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		OnCheckAllPayList(0, NULL, 0);		

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//������
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
	if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

	//������
	DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//Ч������
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//������ʶ
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//������Ϣ
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//ִ������
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//���ؽ���
bool CDataBaseEngineSink::OnRequestCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadCheckInReward"),true);

		//����ṹ
		DBO_GP_CheckInReward CheckInReward;
		ZeroMemory(&CheckInReward,sizeof(CheckInReward));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wDayIndex=0;
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				wDayIndex = m_AccountsDBAide.GetValue_WORD(TEXT("DayID"));
				if(wDayIndex<=LEN_SIGIN)
				{
					CheckInReward.lRewardGold[wDayIndex-1] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardGold")); 
					CheckInReward.lRewardType[wDayIndex-1] = m_AccountsDBAide.GetValue_BYTE(TEXT("RewardType")); 
					CheckInReward.lRewardDay[wDayIndex-1]  = m_AccountsDBAide.GetValue_BYTE(TEXT("RewardDay")); 
				}

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_REWARD,dwContextID,&CheckInReward,sizeof(CheckInReward));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//���ؽ���
bool CDataBaseEngineSink::OnRequestBeginnerConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBeginnerConfig"),true);

		//����ṹ
		DBO_GP_BeginnerCofig kDBInfo;
		ZeroMemory(&kDBInfo,sizeof(kDBInfo));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wDayIndex=0;
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				wDayIndex = m_AccountsDBAide.GetValue_WORD(TEXT("DayID"));
				if(wDayIndex<=LEN_BEGINNER)
				{
					kDBInfo.lRewardGold[wDayIndex-1] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardGold")); 
					kDBInfo.lRewardType[wDayIndex-1] = m_AccountsDBAide.GetValue_BYTE(TEXT("RewardType")); 
				}

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_CONFIG,dwContextID,&kDBInfo,sizeof(kDBInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestBeginnerQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_BeginnerQueryInfo));
		if (wDataSize!=sizeof(DBR_GP_BeginnerQueryInfo)) return false;

		//������
		DBR_GP_BeginnerQueryInfo * pNetInfo=(DBR_GP_BeginnerQueryInfo *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pNetInfo->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_BeginnerQueryInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_BeginnerInfo CheckInInfo;
			ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

			//������Ϣ
			CheckInInfo.wSeriesDate=m_AccountsDBAide.GetValue_WORD(TEXT("SeriesDate"));
			CheckInInfo.bTodayChecked = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("TodayCheckIned"));
			CheckInInfo.bLastCheckIned = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("LastCheckIned"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_INFO,dwContextID,&CheckInInfo,sizeof(CheckInInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//����ṹ
			DBO_GP_BeginnerResult kResoult;
			kResoult.bSuccessed=false;
			lstrcpyn(kResoult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(kResoult.szNotifyContent));

			//���ͽ��
			WORD wSendSize=sizeof(kResoult)-sizeof(kResoult.szNotifyContent)+CountStringBuffer(kResoult.szNotifyContent);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_RESULT,dwContextID,&kResoult,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}
bool CDataBaseEngineSink::OnRequestBeginnerDone(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_BeginnerDone));
		if (wDataSize!=sizeof(DBR_GP_BeginnerDone)) return false;

		//������
		DBR_GP_BeginnerDone * pNetInfo=(DBR_GP_BeginnerDone *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pNetInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pNetInfo->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_BeginnerDone"),true);

		//��������
		DBO_GP_BeginnerResult kNetInfo;
		ZeroMemory(&kNetInfo,sizeof(kNetInfo));
		kNetInfo.lAwardType = 0;

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			kNetInfo.lAwardType = m_AccountsDBAide.GetValue_LONGLONG(TEXT("AwardType"));
			kNetInfo.lAwardCout = m_AccountsDBAide.GetValue_LONGLONG(TEXT("AwardCout"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);


		kNetInfo.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(kNetInfo.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(kNetInfo.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(kNetInfo)-sizeof(kNetInfo.szNotifyContent)+CountStringBuffer(kNetInfo.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_RESULT,dwContextID,&kNetInfo,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BEGINNER_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}
//
bool CDataBaseEngineSink::OnRequestAddBankAwardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetAddBankConfig));
		if (wDataSize!=sizeof(DBR_GP_GetAddBankConfig)) return false;

		//������
		DBR_GP_GetAddBankConfig * pNetInfo=(DBR_GP_GetAddBankConfig *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@iRankIdex"),pNetInfo->iIdex);

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_AddPointConfig"),true);

		//����ṹ
		DBO_GP_AddRankAwardConfig kDBInfo;
		ZeroMemory(&kDBInfo,sizeof(DBO_GP_AddRankAwardConfig));

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wDayIndex=0;
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				wDayIndex = m_AccountsDBAide.GetValue_WORD(TEXT("DayID"))-1;
				if(wDayIndex<=LEN_ADDRANK && wDayIndex >= 0)
				{
					kDBInfo.iIdex = pNetInfo->iIdex; 
					kDBInfo.kRewardGold[wDayIndex] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardGold")); 
					kDBInfo.kRewardType[wDayIndex] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardType")); 
				}
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ADDRANK_AWARD_CONFIG,dwContextID,&kDBInfo,sizeof(DBO_GP_AddRankAwardConfig));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
//
bool CDataBaseEngineSink::OnRequestAddBackUpAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_AccountsDBAide.ResetParameter();

		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_UserAddPoint"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
	return true;
}
//
bool CDataBaseEngineSink::OnRequestGetAddBank(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetAddBank));
		if (wDataSize!=sizeof(DBR_GP_GetAddBank)) return false;

		//������
		DBR_GP_GetAddBank * pNetInfo=(DBR_GP_GetAddBank *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pNetInfo->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@iRankIdex"),pNetInfo->iIdex);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_UserGetAddRank"),true);

		//����ṹ
		DBO_GP_BackAddBank kDBInfo;
		kDBInfo.iRankIdex = pNetInfo->iIdex;

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wDayIndex=0;
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				TCHAR		szNickName[LEN_NICKNAME];				//��¼�ʺ�
				m_AccountsDBAide.GetValue_String(TEXT("NickName"),szNickName,CountArray(szNickName));
				kDBInfo.kNickName.push_back(szNickName);
				kDBInfo.kUserID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("UserID"))); 
				kDBInfo.kUserPoint.push_back(m_AccountsDBAide.GetValue_LONGLONG(TEXT("UserPoint"))); 
				kDBInfo.kFaceID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("FaceID")));  
				kDBInfo.kCustomID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("CustomID")));  
				
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			datastream kTempDataStream;
			kDBInfo.StreamValue(kTempDataStream,true);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ADDRANK_RANK_BACK,dwContextID,kTempDataStream.data(),kTempDataStream.size());
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
//��ѯǩ��
bool CDataBaseEngineSink::OnRequestCheckInQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CheckInQueryInfo));
		if (wDataSize!=sizeof(DBR_GP_CheckInQueryInfo)) return false;

		//������
		DBR_GP_CheckInQueryInfo * pCheckInQueryInfo=(DBR_GP_CheckInQueryInfo *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckInQueryInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pCheckInQueryInfo->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInQueryInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_CheckInInfo CheckInInfo;
			ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

			//������Ϣ
			CheckInInfo.wSeriesDate=m_AccountsDBAide.GetValue_WORD(TEXT("SeriesDate"));
			CheckInInfo.wAwardDate =m_AccountsDBAide.GetValue_WORD(TEXT("AwardDate"));
			CheckInInfo.bTodayChecked = (bool)m_AccountsDBAide.GetValue_BYTE(TEXT("TodayCheckIned"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_INFO,dwContextID,&CheckInInfo,sizeof(CheckInInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//����ṹ
			DBO_GP_CheckInResult CheckInResult;
			CheckInResult.bSuccessed=false;
			lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

			//���ͽ��
			WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}
//ִ��ǩ��
bool CDataBaseEngineSink::OnRequestCheckInDone(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CheckInDone));
		if (wDataSize!=sizeof(DBR_GP_CheckInDone)) return false;

		//������
		DBR_GP_CheckInDone * pCheckInDone=(DBR_GP_CheckInDone *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pCheckInDone->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckInDone->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pCheckInDone->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pCheckInDone->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInDone"),true);

		//��������
		DBO_GP_CheckInResult CheckInResult;
		ZeroMemory(&CheckInResult,sizeof(CheckInResult));
		CheckInResult.bType = false;

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			CheckInResult.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		
		CheckInResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}
bool CDataBaseEngineSink::OnRequestCheckAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CheckInDone));
		if (wDataSize!=sizeof(DBR_GP_CheckInDone)) return false;

		//������
		DBR_GP_CheckInDone * pCheckInDone=(DBR_GP_CheckInDone *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pCheckInDone->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckInDone->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pCheckInDone->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pCheckInDone->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CheckAward"),true);

		//��������
		DBO_GP_CheckInResult CheckInResult;
		ZeroMemory(&CheckInResult,sizeof(CheckInResult));
		CheckInResult.bType = true;

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			CheckInResult.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);


		CheckInResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_CheckInResult CheckInResult;
		CheckInResult.bSuccessed=false;
		lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

		return false;
	}
}

//�������̽���
bool CDataBaseEngineSink::OnRequestRouletteReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_LoadRouletteReward"),true);
		
		//����ṹ
		DBO_GP_RouletteAward kDBInfo;
		ZeroMemory(&kDBInfo,sizeof(kDBInfo));
		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//��������
			WORD wIndex=0;
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				kDBInfo.lRewardGold[wIndex] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("RewardGold"));
				kDBInfo.cbRewardType[wIndex] = m_AccountsDBAide.GetValue_BYTE(TEXT("RewardType"));
				kDBInfo.cbRouletteIdex[wIndex] = m_AccountsDBAide.GetValue_BYTE(TEXT("RouletteIdex"));
				kDBInfo.cbAwardPercent[wIndex] = m_AccountsDBAide.GetValue_BYTE(TEXT("AwardPercent"));

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
				wIndex++;
			}
			kDBInfo.cbCout = wIndex;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ROULETE_AWAED,dwContextID,&kDBInfo,sizeof(kDBInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestUserRouletteAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_ReqUserRouletteAward));
		if (wDataSize!=sizeof(DBR_GR_ReqUserRouletteAward)) return false;

		//��ȡ����
		DBR_GR_ReqUserRouletteAward * pNetInfo=(DBR_GR_ReqUserRouletteAward *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwVaildDay"),pNetInfo->dwVaildDay);

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_RouletteQueryInfo"),true);
		DBO_GP_RouletteUserInfo kDBInfo;

		if(lResultCode==DB_SUCCESS)
		{
			kDBInfo.dwUserID = pNetInfo->dwUserID;
			kDBInfo.dwHaveDone = m_AccountsDBAide.GetValue_DWORD(TEXT("HaveDone"));
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ROULETE_USERINFO,dwContextID,&kDBInfo,sizeof(kDBInfo));
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}
//ִ������
bool CDataBaseEngineSink::OnRequestRouletteDone(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RouletteDone));
		if (wDataSize!=sizeof(DBR_GP_RouletteDone)) return false;

		//��ȡ����
		DBR_GP_RouletteDone * pNetInfo=(DBR_GP_RouletteDone *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@RewardGold"),pNetInfo->lRewardGold);
		m_AccountsDBAide.AddParameter(TEXT("@RewardType"),pNetInfo->cbRewardType);

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_RouletteDone"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//�������̽���
bool CDataBaseEngineSink::OnRequestGameConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//����ṹ
		tagGameConfig kDBInfo;
		ZeroMemory(&kDBInfo,sizeof(kDBInfo));

		//ִ�гɹ�
		m_AccountsDBAide.ResetParameter();
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_LOAD_GAME_CONFIG"),true);

		if(lResultCode==DB_SUCCESS)
		{
			m_AccountsDBAide.GetValue_String(TEXT("DownLoadHttp"),kDBInfo.szDownLoadHttp,128);
			kDBInfo.dwVersion = m_AccountsDBAide.GetValue_DWORD(TEXT("Version"));
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_CONFIG,dwContextID,&kDBInfo,sizeof(kDBInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
//�ͱ�����
bool CDataBaseEngineSink::OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBaseEnsure"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//����ṹ
			DBO_GP_BaseEnsureParameter BaseEnsureParameter;
			ZeroMemory(&BaseEnsureParameter,sizeof(BaseEnsureParameter));

			//��������
			if(m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				BaseEnsureParameter.cbTakeTimes = m_AccountsDBAide.GetValue_WORD(TEXT("TakeTimes"));
				BaseEnsureParameter.lScoreAmount = m_AccountsDBAide.GetValue_WORD(TEXT("ScoreAmount"));
				BaseEnsureParameter.lScoreCondition = m_AccountsDBAide.GetValue_WORD(TEXT("ScoreCondition"));				

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_PARAMETER,dwContextID,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//��ȡ�ͱ�
bool CDataBaseEngineSink::OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_TakeBaseEnsure));
		if (wDataSize!=sizeof(DBR_GP_TakeBaseEnsure)) return false;

		//��ȡ����
		DBR_GP_TakeBaseEnsure * pTakeBaseEnsure=(DBR_GP_TakeBaseEnsure *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pTakeBaseEnsure->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pTakeBaseEnsure->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pTakeBaseEnsure->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pTakeBaseEnsure->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�нű�
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_TakeBaseEnsure"),true);

		//��������
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//��ȡ����
		if(lResultCode==DB_SUCCESS && m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			BaseEnsureResult.lGameScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		//��ȡ����
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);

		//������Ϣ
		BaseEnsureResult.bSuccessed=lResultCode==DB_SUCCESS;
		lstrcpyn(BaseEnsureResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(BaseEnsureResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//����ṹ
		DBO_GP_BaseEnsureResult BaseEnsureResult;
		ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

		//���ñ���
		BaseEnsureResult.bSuccessed=false;
		lstrcpyn(BaseEnsureResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(BaseEnsureResult.szNotifyContent));

		//���ͽ��
		WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
		wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestMatchSignup(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MatchSignup));
		if (wDataSize!=sizeof(DBR_GP_MatchSignup)) return false;

		//������
		DBR_GP_MatchSignup * pMatchSignup=(DBR_GP_MatchSignup *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchSignup->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pMatchSignup->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pMatchSignup->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@wServerID"),pMatchSignup->wServerID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchID"),pMatchSignup->dwMatchID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchSignup->dwMatchNO);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pMatchSignup->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_DeductMatchFee"),true);

		//�������
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = true;
		MatchSignupResult.bSuccessed = lResultCode==DB_SUCCESS;
		lstrcpyn(MatchSignupResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = true;
		MatchSignupResult.bSuccessed = false;
		lstrcpyn(MatchSignupResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

//ȡ������
bool CDataBaseEngineSink::OnRequestMatchUnSignup(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MatchUnSignup));
		if (wDataSize!=sizeof(DBR_GP_MatchUnSignup)) return false;

		//������
		DBR_GP_MatchUnSignup * pMatchUnSignup=(DBR_GP_MatchUnSignup *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchUnSignup->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pMatchUnSignup->dwUserID);
		m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pMatchUnSignup->szPassword);
		m_PlatformDBAide.AddParameter(TEXT("@wServerID"),pMatchUnSignup->wServerID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchID"),pMatchUnSignup->dwMatchID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchUnSignup->dwMatchNO);
		m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pMatchUnSignup->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_ReturnMatchFee"),true);

		//�������
		CDBVarValue DBVarValue;
		m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = false;
		MatchSignupResult.bSuccessed = lResultCode==DB_SUCCESS;
		lstrcpyn(MatchSignupResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT(""),false);

		//������
		DBO_GP_MatchSignupResult MatchSignupResult;
		MatchSignupResult.bSignup = false;
		MatchSignupResult.bSuccessed = false;
		lstrcpyn(MatchSignupResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(MatchSignupResult.szDescribeString));

		//Ͷ�ݽ��
		WORD wDataSize=CountStringBuffer(MatchSignupResult.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_SIGNUP_RESULT,dwContextID,&MatchSignupResult,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestLoadMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LoadMatchReward));
	if (wDataSize!=sizeof(DBR_GR_LoadMatchReward)) return false;

	//��������
	DBR_GR_LoadMatchReward * pMatchReward=(DBR_GR_LoadMatchReward *)pData;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchID"),pMatchReward->dwMatchID);
		m_PlatformDBAide.AddParameter(TEXT("@dwMatchNo"),pMatchReward->dwMatchNO);

		//�������
		LONG lReturnValue=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadMatchReward"),true);

		//����ṹ
		DBO_GR_MatchAwardList MatchRewardResult;

		//��ȡ�Ƹ�
		if(lReturnValue==DB_SUCCESS)
		{
			MatchRewardResult.dwMatchID = pMatchReward->dwMatchID;
			MatchRewardResult.dwMatchNO = pMatchReward->dwMatchNO;
			while(m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				tagDBMatchAwardkInfo kAwardInfo;
				kAwardInfo.MatchRank = m_PlatformDBAide.GetValue_WORD("MatchRank");
				kAwardInfo.RewardGold = m_PlatformDBAide.GetValue_LONGLONG("RewardGold");
				kAwardInfo.RewardMedal = m_PlatformDBAide.GetValue_DWORD("RewardMedal");
				kAwardInfo.RewardExperience = m_PlatformDBAide.GetValue_DWORD("RewardExperience");
				MatchRewardResult.kAwards.push_back(kAwardInfo);
				m_PlatformDBModule->MoveToNext();
			}
		}
		datastream kDataStream;
		MatchRewardResult.StreamValue(kDataStream,true);
		//���ͽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MATCH_AWARD,dwContextID,&kDataStream[0],kDataStream.size());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
//��Ϸ��¼
bool CDataBaseEngineSink::OnRequestGameRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//������
		DBR_GP_GameRecordList * pNetInfo = (DBR_GP_GameRecordList *)pData;

		//�������
		m_TreasureDBAide.ResetParameter();

		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);

		//ִ������
		LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GameRecord_List_RecordID"),true);

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			//����ṹ
			tagPrivateRandTotalRecordList kGameRecordList;
			kGameRecordList.dwUserID = pNetInfo->dwUserID;

			std::vector<int>	kRecordID;
			//��������
			while(m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				kRecordID.push_back(m_TreasureDBAide.GetValue_DWORD("RecordID"));
				m_TreasureDBModule->MoveToNext();
			}
			for (int i = 0;i<(int)kRecordID.size();i++)
			{
				m_TreasureDBAide.ResetParameter();
				m_TreasureDBAide.AddParameter(TEXT("@dwRecordID"),kRecordID[i]);
				LONG lTempResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_PrivateGameRecord"),true);
				if (lTempResultCode == DB_SUCCESS)
				{
					datastream kTempData;
					m_TreasureDBAide.GetParameter(TEXT("UserDate"),kTempData);

					tagPrivateRandTotalRecord kTotalRecord;
					kTotalRecord.StreamValue(kTempData,false);
					kTotalRecord.iRecordID = kRecordID[i];
					kGameRecordList.kList.push_back(kTotalRecord);
				}
			}

			datastream kDataStream;
			kGameRecordList.StreamValue(kDataStream,true);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_RECORD_LIST,dwContextID,&kDataStream[0],kDataStream.size());
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}
//��Ϸ�ط�
bool CDataBaseEngineSink::OnRequestGameChildRecord(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//������
		DBR_GP_GetGameChildRecord * pNetInfo = (DBR_GP_GetGameChildRecord *)pData;

		datastream kDataStream;
		tagPrivateRandRecordChild kChildRecord;
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwRecordID"),pNetInfo->dwRecordID);
		LONG lTempCode1 = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_PrivateGameRecord_Child"),true);
		if (lTempCode1 == DB_SUCCESS)
		{
			//��������
			while(m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				m_TreasureDBAide.GetParameter(TEXT("UserDate"),kDataStream);
				kChildRecord.StreamValue(kDataStream,false);
				m_TreasureDBModule->MoveToNext();
				break;
			}
		}
		kChildRecord.StreamValue(kDataStream,true);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_RECORD_CHILD,dwContextID,&kDataStream[0],kDataStream.size());


		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}
//��Ϸ�ط�
bool CDataBaseEngineSink::OnRequestGameTotalRecord(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	try
	{
		//������
		DBR_GP_GetGameTotalRecord * pNetInfo = (DBR_GP_GetGameTotalRecord *)pData;

		tagPrivateRandTotalRecord kTotalRecord;

		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwRecordID"),pNetInfo->dwRecordID);
		LONG lTempResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_PrivateGameRecord"),true);
		if (lTempResultCode == DB_SUCCESS)
		{
			datastream kTempData;
			m_TreasureDBAide.GetParameter(TEXT("UserDate"),kTempData);
			
			kTotalRecord.StreamValue(kTempData,false);
		}

		
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwRecordID"),pNetInfo->dwRecordID);
		//ִ������
		LONG lResultCode2 = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_PrivateGameRecord_AllChild"),true);
		if(lResultCode2==DB_SUCCESS)
		{
			int index = 0;
			//��������
			while(m_TreasureDBModule->IsRecordsetEnd()==false)
			{
				DWORD RecordChildID = m_TreasureDBAide.GetValue_DWORD(TEXT("RecordChildID"));
				datastream kDataStream;
				m_TreasureDBAide.GetParameter(TEXT("UserDate"),kDataStream);
				tagPrivateRandRecordChild kRecordChild;
				kRecordChild.StreamValue(kDataStream,false);
				kRecordChild.iRecordChildID = RecordChildID;
				kTotalRecord.kRecordChild.push_back(kRecordChild);
				
				m_TreasureDBModule->MoveToNext();

				index++;
				if(index == 8)
				{
					break;
				}

			}

			bool flag = false;
			if(m_TreasureDBModule->IsRecordsetEnd()==true)
			{
				kTotalRecord.iEnd = 1;
				kTotalRecord.isEight = 1;
				flag = true;
			}
			else
			{
				kTotalRecord.iEnd = 0;
				kTotalRecord.isEight = 0;
				flag = false;
			}
			datastream kDataStream;
			kTotalRecord.StreamValue(kDataStream,true);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_RECORD_TOTAL,dwContextID,&kDataStream[0],kDataStream.size());

			if(!flag)
			{
				tagPrivateRandTotalRecord kTotalRecord1;
				while (m_TreasureDBModule->IsRecordsetEnd()==false)
				{
					DWORD RecordChildID = m_TreasureDBAide.GetValue_DWORD(TEXT("RecordChildID"));
					datastream kDataStream;
					m_TreasureDBAide.GetParameter(TEXT("UserDate"),kDataStream);
					tagPrivateRandRecordChild kRecordChild;
					kRecordChild.StreamValue(kDataStream,false);
					kRecordChild.iRecordChildID = RecordChildID;
					kTotalRecord1.kRecordChild.push_back(kRecordChild);

					m_TreasureDBModule->MoveToNext();
				}

				kTotalRecord1.iEnd = 1;
				kTotalRecord1.isEight = 0;

				datastream kDataStream;
				kTotalRecord1.StreamValue(kDataStream,true);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_RECORD_TOTAL,dwContextID,&kDataStream[0],kDataStream.size());

			}

		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			//TODO check is ban user
			if(LogonSuccess.dwCustomID == 2){
				//��������
				DBO_GP_LogonFailure LogonFailure;
				ZeroMemory(&LogonFailure,sizeof(LogonFailure));

				//��������
				LogonFailure.lResultCode = DB_ERROR;
				lstrcpyn(LogonFailure.szDescribeString,TEXT("�����˺��Ѿ�����ͣ��"),CountArray(LogonFailure.szDescribeString));

				//���ͽ��
				WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
				WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
				return;
			}

			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			LogonSuccess.dwSpreaderID=m_AccountsDBAide.GetValue_DWORD(TEXT("SpreaderID"));
			LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_DWORD(TEXT("InsureEnabled"));
			
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			try
			{
				TCHAR szPictureName[LEN_PICTURE]=TEXT("");
				if (true)
				{
					DWORD dwTemp = 0;
					m_AccountsDBAide.ResetParameter();
					m_AccountsDBAide.AddParameter(TEXT("@temp"),dwTemp);
					//ִ�в�ѯ
					m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ActivityPicture"),true);
					m_AccountsDBAide.GetValue_String(TEXT("strPictureName"),LogonSuccess.szPictureName,CountArray(LogonSuccess.szPictureName));
					LogonSuccess.dwPresent[0] = m_AccountsDBAide.GetValue_DWORD("dwPresent");
					LogonSuccess.dwPresent[1] = m_AccountsDBAide.GetValue_DWORD("dwPresent1");
					LogonSuccess.dwPresent[2] = m_AccountsDBAide.GetValue_DWORD("dwPresent2");
					LogonSuccess.dwPresent[3] = m_AccountsDBAide.GetValue_DWORD("dwPresent3");
					LogonSuccess.dwPresent[4] = m_AccountsDBAide.GetValue_DWORD("dwPresent4");
					LogonSuccess.dwPresent[5] = m_AccountsDBAide.GetValue_DWORD("dwPresent5");
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
			};

			try
			{
				//�������
				m_PlatformDBAide.ResetParameter();
				m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);

				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadMatchSignupList"),true);
				if(lResultCode==DB_SUCCESS)
				{
					//��������
					tagSignupMatchInfo * pSignupMatchInfo=NULL;

					while (m_PlatformDBModule->IsRecordsetEnd()==false)
					{
						//����ж�
						if(LogonSuccess.wSignupCount>=CountArray(LogonSuccess.SignupMatchInfo)) break;

						//��ȡ����
						pSignupMatchInfo=&LogonSuccess.SignupMatchInfo[LogonSuccess.wSignupCount++];

						//��ȡ��¼
						pSignupMatchInfo->wServerID=m_PlatformDBAide.GetValue_WORD(TEXT("ServerID"));
						pSignupMatchInfo->dwMatchID=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchID"));
						pSignupMatchInfo->dwMatchNO=m_PlatformDBAide.GetValue_DWORD(TEXT("MatchNo"));

						//�ƶ���¼
						m_PlatformDBModule->MoveToNext();
					}
				}
			}
			catch(...)
			{
				ASSERT(FALSE);
			};
			
			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else
		{
			//��������
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lDestScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("DestScore"));
		UserInsureSuccess.lDestInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("DestInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//�������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode == DB_SUCCESS)
	{
		//��������
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//�������
		OperateSuccess.lResultCode = dwErrorCode;
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize = CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize = sizeof(OperateSuccess) - sizeof(OperateSuccess.szDescribeString);
		
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS, dwContextID, &OperateSuccess, wHeadSize + wDataSize);
	}
	else
	{
		//��������
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//�������
		OperateFailure.lResultCode = dwErrorCode;
		lstrcpyn(OperateFailure.szDescribeString, pszErrorString, CountArray(OperateFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE, dwContextID, &OperateFailure, wHeadSize+wDataSize);
	}

	return;
}


bool CDataBaseEngineSink::OnRequestGetExchangeHuaFeiList( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetExchangeHuaFei));
		if (wDataSize!=sizeof(DBR_GP_GetExchangeHuaFei)) return false;

		//������
		DBR_GP_GetExchangeHuaFei * pNetInfo=(DBR_GP_GetExchangeHuaFei *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetExchangeHuaFei"),true);

		//����ṹ
		DBO_GP_BackExchangeHuaFei kDBInfo;

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				kDBInfo.kExchangeID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("ExchangeID")));
				kDBInfo.kUseType.push_back(m_AccountsDBAide.GetValue_INT(TEXT("UseType"))); 
				kDBInfo.kUseNum.push_back(m_AccountsDBAide.GetValue_INT(TEXT("UseNum"))); 
				kDBInfo.kGetType.push_back(m_AccountsDBAide.GetValue_INT(TEXT("GetType")));  
				kDBInfo.kGetNum.push_back(m_AccountsDBAide.GetValue_INT(TEXT("GetNum")));  

				TCHAR		szGoodsName[LEN_NICKNAME];				// ��Ʒ����
				m_AccountsDBAide.GetValue_String(TEXT("GoodsName"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kGoodsName.push_back(szGoodsName);

				m_AccountsDBAide.GetValue_String(TEXT("ExchangeDesc"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kExchangeDesc.push_back(szGoodsName);

				m_AccountsDBAide.GetValue_String(TEXT("ImgIcon"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kImgIcon.push_back(szGoodsName);

				kDBInfo.kFlag.push_back(m_AccountsDBAide.GetValue_INT(TEXT("Flag")));  

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			datastream kTempDataStream;
			kDBInfo.StreamValue(kTempDataStream,true);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHAGE_HUAFEI_BACK,dwContextID,kTempDataStream.data(),kTempDataStream.size());
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestExchangeHuaFeiDone( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ExchangeDone));
		if (wDataSize!=sizeof(DBR_GP_ExchangeDone)) return false;

		//������
		DBR_GP_ExchangeDone * pNetInfo=(DBR_GP_ExchangeDone *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwExchangeID"),pNetInfo->dwExchangeID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pNetInfo->szPassword);
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ExchangeDone"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestGetShopInfoList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetShopInfo));
		if (wDataSize!=sizeof(DBR_GP_GetShopInfo)) return false;

		//������
		DBR_GP_GetShopInfo * pNetInfo=(DBR_GP_GetShopInfo *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetShopInfo"),true);

		//����ṹ
		DBO_GP_BackShopInfo kDBInfo;

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				kDBInfo.kItemID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("ItemID")));
				kDBInfo.kItemType.push_back(m_AccountsDBAide.GetValue_INT(TEXT("ItemType"))); 
				kDBInfo.kOrderID_IOS.push_back(m_AccountsDBAide.GetValue_INT(TEXT("OrderID_IOS"))); 
				kDBInfo.kOrderID_Android.push_back(m_AccountsDBAide.GetValue_INT(TEXT("OrderID_Android")));  
				kDBInfo.kPrice.push_back(m_AccountsDBAide.GetValue_INT(TEXT("Price")));  
				kDBInfo.kGoodsNum.push_back(m_AccountsDBAide.GetValue_INT(TEXT("GoodsNum")));  

				TCHAR		szGoodsName[LEN_NICKNAME];				// ��Ʒ����
				m_AccountsDBAide.GetValue_String(TEXT("ItemTitle"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kItemTitle.push_back(szGoodsName);

				m_AccountsDBAide.GetValue_String(TEXT("ItemDesc"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kItemDesc.push_back(szGoodsName);

				m_AccountsDBAide.GetValue_String(TEXT("ItemIcon"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kItemIcon.push_back(szGoodsName);

				m_AccountsDBAide.GetValue_String(TEXT("ItemName"),szGoodsName,CountArray(szGoodsName));
				kDBInfo.kItemName.push_back(szGoodsName);

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

			datastream kTempDataStream;
			kDBInfo.StreamValue(kTempDataStream,true);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SHOPINFO_BACK,dwContextID,kTempDataStream.data(),kTempDataStream.size());
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}
bool CDataBaseEngineSink::OnRequestGetBuyOder(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		datastream kDataStream(pData,wDataSize);
		DBR_GP_UpShopOder kNetInfo;
		kNetInfo.StreamValue(kDataStream,false);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),kNetInfo.dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwItemID"),kNetInfo.dwItemID);
		m_AccountsDBAide.AddParameter(TEXT("@total_fee"),kNetInfo.total_fee);
		m_AccountsDBAide.AddParameter(TEXT("@appid"),kNetInfo.appid.c_str());
		m_AccountsDBAide.AddParameter(TEXT("@mch_id"),kNetInfo.mch_id.c_str());
		m_AccountsDBAide.AddParameter(TEXT("@prepay_id"),kNetInfo.prepay_id.c_str());
		m_AccountsDBAide.AddParameter(TEXT("@strOSType"),kNetInfo.strOSType.c_str());
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_BuyOder"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestSellItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		datastream kDataStream(pData,wDataSize);
		DBR_GP_SellItem kNetInfo;
		kNetInfo.StreamValue(kDataStream,false);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwSrcUserID"),kNetInfo.dwSrcUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strSrcPassword"),kNetInfo.strSrcPassword.c_str());
		m_AccountsDBAide.AddParameter(TEXT("@dwDestUserID"),kNetInfo.dwDestUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwItemType"),kNetInfo.dwItemType);
		m_AccountsDBAide.AddParameter(TEXT("@dwItemValue"),kNetInfo.dwItemValue);
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SellItem"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestGetSellRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GetSellRecordList));
		if (wDataSize!=sizeof(DBR_GP_GetSellRecordList)) return false;
		//������
		DBR_GP_GetSellRecordList * pNetInfo=(DBR_GP_GetSellRecordList *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetSellRecordList"),true);

		//����ṹ
		DBO_GP_BackSellOderList kDBInfo;

		//ִ�гɹ�
		if(lResultCode==DB_SUCCESS)
		{
			while (m_AccountsDBModule->IsRecordsetEnd()==false)
			{
				DBO_GP_BackSellOderItem kItem;
				kItem.DestUserID = m_AccountsDBAide.GetValue_INT(TEXT("DestUserID"));
				kItem.kItemType = m_AccountsDBAide.GetValue_INT(TEXT("DestUserID"));
				kItem.kItemValue = m_AccountsDBAide.GetValue_INT(TEXT("DestUserID"));
				m_AccountsDBAide.GetValue_SystemTime(TEXT("OderDate"),kItem.kPlayTime);
				TCHAR		szDestUserName[LEN_NICKNAME];				// ��Ʒ����
				m_AccountsDBAide.GetValue_String(TEXT("DestUserName"),szDestUserName,CountArray(szDestUserName));
				kItem.DestUserName = szDestUserName;

				kDBInfo.kItems.push_back(kItem);

				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}
		}
		datastream kTempDataStream;
		kDBInfo.StreamValue(kTempDataStream,true);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SELLLIST_BACK,dwContextID,kTempDataStream.data(),kTempDataStream.size());

	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
		return false;
	}

	return true;
}

//��ֵ���֪ͨ
VOID CDataBaseEngineSink::OnPayDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString)
{
	//��������
	DBO_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//�������
	UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
	UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
	UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
	UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
	UserInsureSuccess.lDestScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("DestScore"));
	UserInsureSuccess.lDestInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("DestInsure"));
	lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

	//���ͽ��
	WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_PAY_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
}

// �󶨺��Ԫ��
DWORD CDataBaseEngineSink::SrcInsureToBandInsure(DWORD srcInsure)
{
	switch (srcInsure)
	{
	case 6:
		return 7;
	case 18:
		return 22;
	case 30:
		return 40;
	case 68:
		return 93;
	case 128:
		return 178;
	case 328:
		return 478;
	default:
		break;

		//huodong
// 	case 6:
// 		return 8;
// 	case 18:
// 		return 22;
// 	case 30:
// 		return 38;
// 	case 68:
// 		return 92;
// 	case 128:
// 		return 178;
// 	case 328:
// 		return 500;
// 	default:
// 		break;

	}
//	return srcInsure * 120 / 100;
}

// ��ֵ��Ԫ��
DWORD CDataBaseEngineSink::PidToInsureScore(DWORD pid)
{
	switch (pid)
	{
	case 0:
		return 6;
	case 1:
		return 18;
	case 2:
		return 30;
	case 3:
		return 68;
	case 4:
		return 128;
	case 5:
		return 328;
	default:
		break;
	}
	return 0;
}

// �������ֲ�
bool CDataBaseEngineSink::OnRequestCreateClub(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Create_Club));
		if (wDataSize != sizeof(DBR_GR_Create_Club)) 
			return false;

		//������
		DBR_GR_Create_Club * pCreatClub = (DBR_GR_Create_Club *) pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@imasterid"), pCreatClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strclub_name"), pCreatClub->stClubName);
		m_AccountsDBAide.AddParameter(TEXT("@igametypeindex"), pCreatClub->bGameTypeIndex);
		m_AccountsDBAide.AddParameter(TEXT("@igametypeqing"), pCreatClub->bGameTypeQing);
		m_AccountsDBAide.AddParameter(TEXT("@iplaycoutidex"), pCreatClub->bPlayCoutIdex);
		m_AccountsDBAide.AddParameter(TEXT("@igamepaytype"), pCreatClub->bGamePayType);
		m_AccountsDBAide.AddParameter(TEXT("@bmaxscore"), pCreatClub->bMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@iqiangganghu"), pCreatClub->bQiangGangHu);
		m_AccountsDBAide.AddParameter(TEXT("@iallother"), pCreatClub->bAllOther);
		m_AccountsDBAide.AddParameter(TEXT("@ipanthertype"), pCreatClub->bPantherType);
		m_AccountsDBAide.AddParameter(TEXT("@iplayernum"), pCreatClub->bPlayerNum);
		m_AccountsDBAide.AddParameter(TEXT("@ibaoisoutcard"), pCreatClub->bBaoIsOutCard);
		m_AccountsDBAide.AddParameter(TEXT("@ipiaoshu"), pCreatClub->bPiaoShu);
		m_AccountsDBAide.AddParameter(TEXT("@igamebasescore"), pCreatClub->bGameBaseScore);
		//�������
		TCHAR szDescribeString[128] = TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CreateClubHG"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

		//��������
		DBO_GP_Create_Join_Res CreateJoinRes;
		ZeroMemory(&CreateJoinRes, sizeof(CreateJoinRes));

		//�������
		CreateJoinRes.dwCreateJoin = 1;
		CreateJoinRes.dwError = lResultCode;
		lstrcpyn(CreateJoinRes.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(CreateJoinRes.szDescribeString));

		//���ͽ��
		WORD wDataSize = CountStringBuffer(CreateJoinRes.szDescribeString);
		WORD wHeadSize = sizeof(CreateJoinRes)-sizeof(CreateJoinRes.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CREATE_JOIN_RES,dwContextID, &CreateJoinRes, wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;   
}

// ���������ֲ�
bool CDataBaseEngineSink::OnRequestJoinClub(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Join_Club));
		if (wDataSize != sizeof(DBR_GR_Join_Club)) return false;

		//������
		DBR_GR_Join_Club * pJoinClub = (DBR_GR_Join_Club *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pJoinClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClubID"),pJoinClub->dwClubID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_JoinClub"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);

		//��������
		DBO_GP_Create_Join_Res CreateJoinRes;
		ZeroMemory(&CreateJoinRes, sizeof(CreateJoinRes));

		//�������
		CreateJoinRes.dwCreateJoin = 2;
		CreateJoinRes.dwError = lResultCode;
		lstrcpyn(CreateJoinRes.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(CreateJoinRes.szDescribeString));

		//���ͽ��
		WORD wDataSize = CountStringBuffer(CreateJoinRes.szDescribeString);
		WORD wHeadSize = sizeof(CreateJoinRes)-sizeof(CreateJoinRes.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CREATE_JOIN_RES,dwContextID, &CreateJoinRes, wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// �᳤ͬ���ͬ�������ֲ�
bool CDataBaseEngineSink::OnRequestJoinClubComfirm(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Jion_Club_Comfirm));
		if (wDataSize != sizeof(DBR_GR_Jion_Club_Comfirm)) return false;

		//������
		DBR_GR_Jion_Club_Comfirm * pJoinClubComfirm = (DBR_GR_Jion_Club_Comfirm *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwOperateID"), pJoinClubComfirm->dwOperateID);
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pJoinClubComfirm->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClubID"), pJoinClubComfirm->dwClubID);
		m_AccountsDBAide.AddParameter(TEXT("@bComfirm"), pJoinClubComfirm->bComfirm);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_JoinClubComfirm"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID, DB_ERROR, TEXT("�������ݿ�����쳣�������Ժ����ԣ�"), false);

		return false;
	}

	return true;
}

// �뿪���ֲ�
bool CDataBaseEngineSink::OnRequestLeaveClub(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Leave_Club));
		if (wDataSize != sizeof(DBR_GR_Leave_Club)) return false;

		//������
		DBR_GR_Leave_Club * pLeaveClub = (DBR_GR_Leave_Club *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pLeaveClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClubID"),pLeaveClub->dwClubID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"), szDescribe, sizeof(szDescribe), adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LeaveClub"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// �߳����ֲ�
bool CDataBaseEngineSink::OnRequestKickClub(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Kick_Club));
		if (wDataSize != sizeof(DBR_GR_Kick_Club)) return false;

		//������
		DBR_GR_Kick_Club * pKickClub = (DBR_GR_Kick_Club *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwRequestID"), pKickClub->dwRequestID);
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pKickClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), pKickClub->dwClubID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_KickClub"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// ������Ҽ�����ֲ�
bool CDataBaseEngineSink::OnRequestInviteClub(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Invite_Club));
		if (wDataSize != sizeof(DBR_GR_Invite_Club)) return false;

		//������
		DBR_GR_Invite_Club * pInviteClub = (DBR_GR_Invite_Club *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwRequestID"), pInviteClub->dwRequestID);
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pInviteClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClubID"),pInviteClub->dwClubID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_InviteClub"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡ�Լ��ľ��ֲ�����
bool  CDataBaseEngineSink::OnRequestGetMyClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try{

		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Get_My_Club_Req));
		if (wDataSize != sizeof(DBR_GR_Get_My_Club_Req)) return false;

		//������
		DBR_GR_Get_My_Club_Req * pJoinClubInfo = (DBR_GR_Get_My_Club_Req *) pData;

		DBO_GR_Get_My_Club_Res   pMyClubRes;
		ZeroMemory(&pMyClubRes,sizeof(DBO_GR_Get_My_Club_Res));
		pMyClubRes.dwUserID = pJoinClubInfo->dwUserID;
		if (true)
		{
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pJoinClubInfo->dwUserID);
			//ִ�в�ѯ
			LONG lResultCode1 = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckAgent"), false);
			pMyClubRes.cbAgent = lResultCode1;
		}
		
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pJoinClubInfo->dwUserID);
		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ClubMember"), true);
		int i = 0;
		if(lResultCode == DB_SUCCESS)
		{
			while(m_AccountsDBModule->IsRecordsetEnd() == false)
			{
				pMyClubRes.dwClubId[i] = m_AccountsDBAide.GetValue_DWORD(TEXT("IClubID"));
				pMyClubRes.dwMemberNum[i] = m_AccountsDBAide.GetValue_DWORD(TEXT("IMemberNum"));
				pMyClubRes.dwMasterId[i] = m_AccountsDBAide.GetValue_DWORD(TEXT("IMasterID"));
				m_AccountsDBAide.GetValue_String(TEXT("SzClubName"),pMyClubRes.szClubName[i],sizeof(pMyClubRes.szClubName[i]));
				m_AccountsDBAide.GetValue_String(TEXT("SzMasterName"),pMyClubRes.szMaterName[i],sizeof(pMyClubRes.szMaterName[i]));
				m_AccountsDBAide.GetValue_String(TEXT("SzHeadHttp"),pMyClubRes.szPatritUrl[i],sizeof(pMyClubRes.szPatritUrl[i]));
				m_AccountsDBAide.GetValue_String(TEXT("strClubNotice"),pMyClubRes.stClubNotice[i],sizeof(pMyClubRes.stClubNotice[i]));
				pMyClubRes.cbGameTypeIndex[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iGameTypeIndex"));
				pMyClubRes.bGameTypeQing[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iGameTypeQing"));
				pMyClubRes.bPlayCoutIdex[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iPlayCoutIdex"));
				pMyClubRes.bGamePayType[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iGamePayType"));
				pMyClubRes.bQiangGangHu[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iQiangGangHu"));
				pMyClubRes.bAllOther[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iAllOther"));
				pMyClubRes.cbPantherType[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iPantherType"));
				pMyClubRes.cbPlayerNum[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iPlayerNum"));
				pMyClubRes.cbBaoIsOutCard[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iBaoIsOutCard"));
				pMyClubRes.cbPiaoShu[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iPiaoShu"));
				pMyClubRes.cbGameBaseScore[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("iGameBaseScore"));
				m_AccountsDBAide.GetValue_SystemTime(TEXT("dCreateTime"),pMyClubRes.dCreateTime[i]);
				pMyClubRes.cbGameMaxScore[i] = m_AccountsDBAide.GetValue_BYTE(TEXT("imaxScore"));
				pMyClubRes.dwClubManagerId[i][0] = m_AccountsDBAide.GetValue_DWORD(TEXT("iManagerId1"));
				pMyClubRes.dwClubManagerId[i][1] = m_AccountsDBAide.GetValue_DWORD(TEXT("iManagerId2"));
				pMyClubRes.dwClubManagerId[i][1] = m_AccountsDBAide.GetValue_DWORD(TEXT("iManagerId3"));
			
				m_AccountsDBModule->MoveToNext();
				i++;
			}
			pMyClubRes.dwClubNum = i ;
	
			//��������			
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_MY_CLUB_RES,dwContextID,&pMyClubRes,sizeof(DBO_GR_Get_My_Club_Res));

		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;

}

// ���ֲ���Ա�б�����
bool CDataBaseEngineSink::OnRequestSubMmemberListReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBO_GR_Club_Member_List_Req));
		if (wDataSize != sizeof(DBO_GR_Club_Member_List_Req)) return false;

		//������
		DBO_GR_Club_Member_List_Req * pNetInfo = (DBO_GR_Club_Member_List_Req *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pNetInfo->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwClubID"),pNetInfo->dwClubID);
		m_AccountsDBAide.AddParameter(TEXT("@dwFlag"),pNetInfo->dwFlag);

		////������ʾ
		//TCHAR szDescribe[64]=TEXT("");
		//_sntprintf(szDescribe,CountArray(szDescribe),TEXT("%d,%d,%d,"),pNetInfo->dwUserID, pNetInfo->dwClubID, pNetInfo->dwFlag);
		////��ʾ��Ϣ
		//CTraceService::TraceString(szDescribe,TraceLevel_Normal);

		//ִ������
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MemberListReq"), true);

		//����ṹ
		DBO_GP_Club_Mmember_List_Res kDBInfo;
		kDBInfo.iUserID.clear();
		kDBInfo.szHeadHttp0.clear();
		kDBInfo.strClubName.clear();
		kDBInfo.strUserName.clear();
		kDBInfo.strJoinTime.clear();
		BYTE bTemp = 0;
		//ִ�гɹ�
		if(lResultCode == DB_SUCCESS)
		{
			int i = 0;
			while (m_AccountsDBModule->IsRecordsetEnd() == false)
			{
				kDBInfo.iUserID.push_back(m_AccountsDBAide.GetValue_INT(TEXT("iUserIDTemp")));

				TCHAR szHeadHttp0Temp[255];
				m_AccountsDBAide.GetValue_String(TEXT("szHeadHttp0Temp"),szHeadHttp0Temp,CountArray(szHeadHttp0Temp));
				kDBInfo.szHeadHttp0.push_back(szHeadHttp0Temp);

				TCHAR szNameTemp[LEN_NICKNAME];
				m_AccountsDBAide.GetValue_String(TEXT("strClubNameTemp"),szNameTemp,CountArray(szNameTemp));
				kDBInfo.strClubName.push_back(szNameTemp);

				m_AccountsDBAide.GetValue_String(TEXT("strUsernameTemp"),szNameTemp,CountArray(szNameTemp));
				kDBInfo.strUserName.push_back(szNameTemp);

				TCHAR szJoinTimeTemp[32];
				m_AccountsDBAide.GetValue_String(TEXT("strJoinTime"),szJoinTimeTemp,CountArray(szJoinTimeTemp));
				kDBInfo.strJoinTime.push_back(szJoinTimeTemp);
				i++;
				if (i == 40)
				{
					i = 0;
					m_AccountsDBModule->MoveToNext();
					if (m_AccountsDBModule->IsRecordsetEnd() == false)
					{
						kDBInfo.bFlag = 0;
						datastream kTempDataStream;
						kDBInfo.StreamValue(kTempDataStream, true);
						m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CLUB_MEMBER_LIST_RES,dwContextID,kTempDataStream.data(),kTempDataStream.size());
						kDBInfo.iUserID.clear();
						kDBInfo.szHeadHttp0.clear();
						kDBInfo.strClubName.clear();
						kDBInfo.strUserName.clear();
						kDBInfo.strJoinTime.clear();
						continue;
					}
					else
					{
						kDBInfo.bFlag = 1;
						datastream kTempDataStream;
						kDBInfo.StreamValue(kTempDataStream, true);
						m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CLUB_MEMBER_LIST_RES,dwContextID,kTempDataStream.data(),kTempDataStream.size());
						bTemp = 1;
						kDBInfo.iUserID.clear();
						kDBInfo.szHeadHttp0.clear();
						kDBInfo.strClubName.clear();
						kDBInfo.strUserName.clear();
						kDBInfo.strJoinTime.clear();
						break;
					}
				}
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}
			if (bTemp == 0)
			{
				kDBInfo.bFlag = 1;
				datastream kTempDataStream;
				kDBInfo.StreamValue(kTempDataStream, true);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CLUB_MEMBER_LIST_RES,dwContextID,kTempDataStream.data(),kTempDataStream.size());
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

// �᳤���¾��ֲ���������
bool CDataBaseEngineSink::OnRequestSubUpdateClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Update_Club_Req));
		if (wDataSize != sizeof(DBR_GR_Update_Club_Req)) return false;

		//������
		DBR_GR_Update_Club_Req * pKickClub = (DBR_GR_Update_Club_Req *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@iuserid"),pKickClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@iclubid"), pKickClub->dwClubID);
		m_AccountsDBAide.AddParameter(TEXT("@iGameTypeIndex"), pKickClub->bGameTypeIndex);
		m_AccountsDBAide.AddParameter(TEXT("@iGameTypeQing"),pKickClub->bGameTypeQing);
		m_AccountsDBAide.AddParameter(TEXT("@iPlayCoutIdex"), pKickClub->bPlayCoutIdex);
		m_AccountsDBAide.AddParameter(TEXT("@iGamePayType"), pKickClub->bGamePayType);
		m_AccountsDBAide.AddParameter(TEXT("@iMaxScore"), pKickClub->bMaxScore);
		m_AccountsDBAide.AddParameter(TEXT("@iQiangGangHu"), pKickClub->bQiangGangHu);
		m_AccountsDBAide.AddParameter(TEXT("@iAllOther"), pKickClub->bAllOther);
		m_AccountsDBAide.AddParameter(TEXT("@iPantherType"), pKickClub->bPantherType);
		m_AccountsDBAide.AddParameter(TEXT("@iPlayerNum"), pKickClub->bPlayerNum);
		m_AccountsDBAide.AddParameter(TEXT("@iBaoIsOutCard"), pKickClub->bBaoIsOutCard);
		m_AccountsDBAide.AddParameter(TEXT("@iPiaoShu"), pKickClub->bPiaoShu);
		m_AccountsDBAide.AddParameter(TEXT("@iGameBaseScore"), pKickClub->bGameBaseScore);
		m_AccountsDBAide.AddParameter(TEXT("@strClubName"), pKickClub->stClubName);
		m_AccountsDBAide.AddParameter(TEXT("@strclubnotice"), pKickClub->stClubNotice);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);
		 
		//ִ�в�ѯ	
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UpdateClub"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// ��ɢ���ֲ�����
bool CDataBaseEngineSink::OnRequestSubDismissClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Dismiss_Club_Req));
		if (wDataSize != sizeof(DBR_GR_Dismiss_Club_Req)) return false;

		//������
		DBR_GR_Dismiss_Club_Req * pKickClub = (DBR_GR_Dismiss_Club_Req *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@iuserid"), pKickClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@iclubid"), pKickClub->dwClubID);
		
		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_DismissClub"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// ת�û᳤����
bool CDataBaseEngineSink::OnRequestSubChangeMasterReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Club_Change_Master_Req));
		if (wDataSize != sizeof(DBR_GR_Club_Change_Master_Req)) return false;

		//������
		DBR_GR_Club_Change_Master_Req * pKickClub = (DBR_GR_Club_Change_Master_Req *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@iuserid"), pKickClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@iclubid"), pKickClub->dwClubID);
		m_AccountsDBAide.AddParameter(TEXT("@iMasterID"), pKickClub->dwMasterID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ChangeMaster"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

// �᳤���ù���Ա����
bool CDataBaseEngineSink::OnRequestSubSetManagerReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_Club_Set_Manager));
		if (wDataSize != sizeof(DBR_GR_Club_Set_Manager)) return false;

		//������
		DBR_GR_Club_Set_Manager * pKickClub = (DBR_GR_Club_Set_Manager *) pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@iUserID"), pKickClub->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@iClubID"), pKickClub->dwClubID);
		m_AccountsDBAide.AddParameter(TEXT("@iManagerID"), pKickClub->dwManagerID);

		//�������
		WCHAR szDescribe[128] = L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SetManager"), false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID, lResultCode, CW2CT(DBVarValue.bstrVal), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////

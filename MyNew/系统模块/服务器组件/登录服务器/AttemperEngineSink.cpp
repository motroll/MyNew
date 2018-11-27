#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"


//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������
#define IDI_CHECK_PAY_INFO		    4			//ÿ���Ӽ��֧��


#define TIME_CHECK_PAY_INFO			30L									//ÿ���Ӽ��֧��

//////////////////////////////////////////////////////////////////////////////////

void ConvertUtf8ToGBK(CString &strUtf8)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
	wchar_t * wszGBK = new wchar_t[len];
	memset(wszGBK,0,len);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len); 

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}
void ConvertUtf8ToGBK(char* pChar,int iLen)
{
	CString kString(pChar);
	ConvertUtf8ToGBK(kString);
	strncpy(pChar,kString.GetString(),iLen);
}
//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//��Ƶ����
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	ZeroMemory((void*)&m_kCheckInInfo,sizeof(m_kCheckInInfo));
	ZeroMemory(&m_BaseEnsureParameter,sizeof(m_BaseEnsureParameter));
	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CString szFileName;
	GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	szFileName=szPath;
	int nIndex = szFileName.ReverseFind(TEXT('\\'));
	szFileName = szFileName.Left(nIndex);
	szFileName += TEXT("\\PlazaOptionConfig.ini");

	//��ȡ����
	m_bShowServerStatus = (GetPrivateProfileInt(TEXT("ServerStatus"),TEXT("ShowServerStatus"),0,szFileName) != 0);

	//��ȡĿ¼
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
	TCHAR szVideoFileName[MAX_PATH]=TEXT("");
	_sntprintf(szVideoFileName,CountArray(szVideoFileName),TEXT("%s\\VideoOption.ini"),szPath);
	m_wAVServerPort=GetPrivateProfileInt(TEXT("VideoOption"),TEXT("ServerPort"), 0,szVideoFileName);
	DWORD dwAddrLen=GetPrivateProfileString(TEXT("VideoOption"),TEXT("ServerAddr"), TEXT(""), szServerAddr,LEN_SERVER,szVideoFileName);
	if(dwAddrLen>0)
	{
		CT2CA strServerDomain(szServerAddr);
		m_dwAVServerAddr=inet_addr(strServerDomain);
	}
	else
	{
		m_dwAVServerAddr=0;
	}

	// ��ֵ
	m_pITimerEngine->SetTimer(IDI_CHECK_PAY_INFO,TIME_CHECK_PAY_INFO*1000L,TIMES_INFINITY,NULL);
	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();

	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			//���ؽ���
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_CHECKIN_REWARD,0,NULL,0);

			//���صͱ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BASEENSURE,0,NULL,0);

			//������������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_BEGINNER,0,NULL,0);

			//������������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_ROULETE_AWARD,0,NULL,0);

			//������Ϸ����
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_CONFIG,0,NULL,0);

			DBR_GP_GetAddBankConfig kGetAddBankConfig;
			kGetAddBankConfig.iIdex = 0;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_ADDRANK_CONFIG,0,&kGetAddBankConfig,sizeof(kGetAddBankConfig));

			kGetAddBankConfig.iIdex = 1;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_ADDRANK_CONFIG,0,&kGetAddBankConfig,sizeof(kGetAddBankConfig));

			kGetAddBankConfig.iIdex = 2;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_ADDRANK_CONFIG,0,&kGetAddBankConfig,sizeof(kGetAddBankConfig));
			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_CHECK_PAY_INFO:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CHECK_PAY_INFO,0,NULL,0);
		}
		break;
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			return true;
		}
	}

	return false;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//��¼����
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VALIDATE_MBCARD:
		{
			return OnDBPCLogonValidateMBCard(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//�û���Ϣ
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_ACCOUNTINFO:		//�û�������Ϣ
		{
			return OnDBPCUserAccountInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_INSURE_RETURN:
		{
			return OnDBPCUserInsureReturn(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_HISTORY:
		{
			return OnDBUserInsuerHistory(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_PAY_SUCCESS:
		{
			return OnDBUserPaySuccess(dwContextID,pData,wDataSize);
		}
		break;
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //�û���Ϣ
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_PUBLIC_NOTICE:		//�Զ������ݲ�ѯ
		{
			return OnDBPCPublicNotice(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INGAME_SERVER_ID:		//�Զ������ݲ�ѯ
		{
			return OnDBPCInGameSevrerID(dwContextID,pData,wDataSize);
		}
	case DBO_GP_MATCH_SIGNUP_RESULT:	//�������
		{
			return OnDBPCUserMatchSignupResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_MATCH_AWARD:	//�����б�
		{
			return OnDBPCMacthAwardList(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SPREADER_RESOULT:		//�����ɹ�
		{
			return OnDBPCOSpreaderResoult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//�ڵ�����
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//��������
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_REWARD:			//ǩ������
		{
			return OnDBPCCheckInReward(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_INFO:			//ǩ����Ϣ
		{
			return OnDBPCUserCheckInInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHECKIN_RESULT:			//ǩ�����
		{
			return OnDBPCUserCheckInResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BEGINNER_CONFIG:		//���ֻ����
		{
			return OnDBPCBeginnerConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BEGINNER_RESULT:		//���ֻ����
		{
			return OnDBPCUserBeginnerResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ROULETE_AWAED:		//���ֻ����
		{
			return OnDBPCUserRouletteAward(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_CONFIG:		//��Ϸ����
		{
			return OnDBPCGameConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ROULETE_USERINFO:		//���ֻ����
		{
			return OnDBPCUserRouletteUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BEGINNER_INFO:			//���ֻ��Ϣ
		{
			return OnDBPCUserBeginnerInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_PARAMETER:	//�ͱ�����
		{
			return OnDBPCBaseEnsureParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BASEENSURE_RESULT:		//�ͱ����
		{
			return OnDBPCBaseEnsureResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ADDRANK_AWARD_CONFIG:		//�ͱ����
		{
			return OnDBPCAddBankAwardConfig(dwContextID,pData,wDataSize);
		}
	case DBO_GP_ADDRANK_RANK_BACK:		//�ͱ����
		{
			return OnDBPCAddBankBack(dwContextID,pData,wDataSize);
		}
	case DBO_GP_EXCHAGE_HUAFEI_BACK:	//�һ������б�
		{
			return OnDBPCExchangeHuaFeiBack(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SHOPINFO_BACK:		//�̳��б�
		{
			return OnDBPCShopInfoBack(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SELLLIST_BACK:		//�̳��б�
		{
			return OnDBPCSellListBack(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_RECORD_LIST:		//�̳��б�
		{
			return OnDBPCGameRecordList(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_RECORD_TOTAL:		//�̳��б�
		{
			return OnDBPCGameRecordTotal(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_RECORD_CHILD:		//�̳��б�
		{
			return OnDBPCGameRecordChild(dwContextID,pData,wDataSize);
		}
	case  DBO_GP_CREATE_JOIN_RES: //���������������ֲ���Ӧ
		{
			return  OnDBPCCreateJoinRes(dwContextID,pData,wDataSize);
		}
	case   DBO_GP_GET_MY_CLUB_RES: //��ȡ�Լ��ľ��ֲ���Ӧ
		{
			return  OnDBPGetMyClubRes(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CLUB_MEMBER_LIST_RES: // �᳤���ֲ���Ա�б��Ӧ
		{
			return OnDBPCClubMasterListRes(dwContextID,pData,wDataSize);
		}
	}
	
	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				return true;
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//��������
			DWORD dwOldOnlineCount=0;
			dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
			pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

			//Ŀ¼����
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
			if (pGameKindItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				//��������
				DWORD dwOldOnlineCount=0, dwOldFullCount=0;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//�޸ķ�����Ϣ
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//Ŀ¼����
				CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
				if (pGameKindItem!=NULL)
				{
					tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
					pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
					pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

					pGameKindItem->m_GameKind.dwFullCount -= dwOldFullCount;
					pGameKindItem->m_GameKind.dwFullCount += pGameServer->dwFullCount;
				}
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	case SUB_CS_S_MATCH_INSERT:		//��������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameMatch)==0);
			if (wDataSize%sizeof(tagGameMatch)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameMatch);
			tagGameMatch * pGameMatch=(tagGameMatch *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				CGameServerItem * pGameServerItem = m_ServerListManager.SearchGameServer(pGameMatch->wServerID);
				if(pGameServerItem!=NULL)
				{
					CopyMemory(&pGameServerItem->m_GameMatch,pGameMatch++,sizeof(pGameServerItem->m_GameMatch));
				}
			}

			return true;
		}
	}

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_VISITOR:		//�ο͵�¼
		{
			return OnTCPNetworkSubPCVisitor(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//�����б�
			if (m_pInitParameter->m_cbDelayList==TRUE)
			{
				//�����б�
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);
			}
			else
			{
				//����ҳ��
				SendGamePageInfo(dwSocketID,0);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//��ȡ����
		{
			//Ч������
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//�����б�
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//��ȡ�ղ�
		{
			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GAME_RECORD_LIST://��Ϸ��¼
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetGameRecord_List));
			if (wDataSize!=sizeof(CMD_GP_GetGameRecord_List)) return false;

			//������Ϣ
			CMD_GP_GetGameRecord_List * pNetInfo=(CMD_GP_GetGameRecord_List *)pData;
			//��������
			DBR_GP_GameRecordList kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));

			//��������
			kDBInfo.dwUserID = pNetInfo->dwUserID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_RECORD_LIST,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_GAME_RECORD_TOTAL://��Ϸ��¼
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetGameTotalRecord));
			if (wDataSize!=sizeof(CMD_GP_GetGameTotalRecord)) return false;

			//������Ϣ
			CMD_GP_GetGameTotalRecord * pNetInfo=(CMD_GP_GetGameTotalRecord *)pData;
			//��������
			DBR_GP_GetGameTotalRecord kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));

			//��������
			kDBInfo.dwUserID = pNetInfo->dwUserID;
			kDBInfo.dwRecordID = pNetInfo->dwRecordID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_TOTAL_RECORD,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_GAME_RECORD_CHILD://��Ϸ��¼
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_GetGameChildRecord));
			if (wDataSize!=sizeof(CMD_GP_GetGameChildRecord)) return false;

			//������Ϣ
			CMD_GP_GetGameChildRecord * pNetInfo=(CMD_GP_GetGameChildRecord *)pData;
			//��������
			DBR_GP_GetGameChildRecord kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));

			//��������
			kDBInfo.dwRecordID = pNetInfo->dwRecordID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GAME_TOTAL_CHILD,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_QUERY_PUBLIC_NOTICE://�Զ����ֶβ�ѯ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryNotice));
			if (wDataSize!=sizeof(CMD_GP_QueryNotice)) return false;

			//������Ϣ
			CMD_GP_QueryNotice * pNetInfo=(CMD_GP_QueryNotice *)pData;
			//��������
			DBR_GP_PublicNotic kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));

			//��������
			lstrcpyn(kDBInfo.szKeyName,pNetInfo->szKeyName,CountArray(kDBInfo.szKeyName));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_PUBLIC_NOTIC,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_MODIFY_MACHINE:		//�󶨻���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//������Ϣ
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//��������
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//������Ϣ
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//��������
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_ACCOUNTS:	//�޸��ʺ�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyAccounts));
			if (wDataSize!=sizeof(CMD_GP_ModifyAccounts)) return false;

			//������Ϣ
			CMD_GP_ModifyAccounts * pModifyInsurePass=(CMD_GP_ModifyAccounts *)pData;
			pModifyInsurePass->szDesAccount[CountArray(pModifyInsurePass->szDesAccount)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyAccounts ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesAccount,pModifyInsurePass->szDesAccount,CountArray(ModifyInsurePass.szDesAccount));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_ACCOUNTS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_SPREADER:	//�޸��Ƽ���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifySpreader));
			if (wDataSize!=sizeof(CMD_GP_ModifySpreader)) return false;

			//������Ϣ
			CMD_GP_ModifySpreader * pModifyInsurePass=(CMD_GP_ModifySpreader *)pData;
			pModifyInsurePass->szPassword[CountArray(pModifyInsurePass->szPassword)-1]=0;
			pModifyInsurePass->szSpreader[CountArray(pModifyInsurePass->szSpreader)-1]=0;

			//��������
			DBR_GP_ModifySpreader ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szPassword,pModifyInsurePass->szPassword,CountArray(ModifyInsurePass.szPassword));
			lstrcpyn(ModifyInsurePass.szSpreader,pModifyInsurePass->szSpreader,CountArray(ModifyInsurePass.szSpreader));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SPREADER,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_UNDER_WRITE:	//�޸�ǩ��
		{
			//��������
			CMD_GP_ModifyUnderWrite * pModifyUnderWrite=(CMD_GP_ModifyUnderWrite *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_GP_ModifyUnderWrite));
			ASSERT(wDataSize>=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)));
			ASSERT(wDataSize==(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite)));

			//Ч�����
			if (wDataSize>sizeof(CMD_GP_ModifyUnderWrite)) return false;
			if (wDataSize<(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite))) return false;
			if (wDataSize!=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite))) return false;

			//������Ϣ
			pModifyUnderWrite->szPassword[CountArray(pModifyUnderWrite->szPassword)-1]=0;
			pModifyUnderWrite->szUnderWrite[CountArray(pModifyUnderWrite->szUnderWrite)-1]=0;

			//��������
			DBR_GP_ModifyUnderWrite ModifyUnderWrite;
			ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			//��������
			ModifyUnderWrite.dwUserID=pModifyUnderWrite->dwUserID;
			ModifyUnderWrite.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyUnderWrite.szPassword,pModifyUnderWrite->szPassword,CountArray(ModifyUnderWrite.szPassword));
			lstrcpyn(ModifyUnderWrite.szUnderWrite,pModifyUnderWrite->szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_UNDER_WRITE,dwSocketID,&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_CustomFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_CustomFaceInfo)) return false;

			//������Ϣ
			CMD_GP_CustomFaceInfo * pCustomFaceInfo=(CMD_GP_CustomFaceInfo *)pData;

			//��������
			DBR_GP_ModifyCustomFace ModifyCustomFace;
			ZeroMemory(&ModifyCustomFace,sizeof(ModifyCustomFace));

			//��������
			ModifyCustomFace.dwUserID=pCustomFaceInfo->dwUserID;
			ModifyCustomFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyCustomFace.szPassword,pCustomFaceInfo->szPassword,CountArray(ModifyCustomFace.szPassword));
			lstrcpyn(ModifyCustomFace.szMachineID,pCustomFaceInfo->szMachineID,CountArray(ModifyCustomFace.szMachineID));
			CopyMemory(ModifyCustomFace.dwCustomFace,pCustomFaceInfo->dwCustomFace,sizeof(ModifyCustomFace.dwCustomFace));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_CUSTOM_FACE,dwSocketID,&ModifyCustomFace,sizeof(ModifyCustomFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:	//��ѯ��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//������Ϣ
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//��������
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_QUERY_ACCOUNTINFO:	//��ѯ������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryAccountInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryAccountInfo)) return false;

			//������Ϣ
			CMD_GP_QueryAccountInfo * pQueryIndividual=(CMD_GP_QueryAccountInfo *)pData;

			//��������
			DBR_GP_QueryAccountInfo QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_ACCOUNTINFO,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_QUERY_INGAME_SEVERID:	//��ѯ��Ϸ״̬
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserInGameServerID));
			if (wDataSize!=sizeof(CMD_GP_UserInGameServerID)) return false;

			//������Ϣ
			CMD_GP_UserInGameServerID * pNetInfo=(CMD_GP_UserInGameServerID *)pData;

			//��������
			DBR_GP_QueryUserInGameServerID kDBInfo;
			ZeroMemory(&kDBInfo,sizeof(kDBInfo));
			kDBInfo.dwUserID=pNetInfo->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_INGAME_SERVERID,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//��չ��Ϣ
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName))
						{
							CopyMemory(&ModifyIndividual.szNickName,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szNickName[CountArray(ModifyIndividual.szNickName)-1]=0;
							ConvertUtf8ToGBK(ModifyIndividual.szNickName,LEN_NICKNAME);
						}
						break;
					}
				case DTP_GP_UI_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite))
						{
							CopyMemory(&ModifyIndividual.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUnderWrite[CountArray(ModifyIndividual.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_USER_NOTE:		//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote))
						{
							CopyMemory(&ModifyIndividual.szUserNote,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserNote[CountArray(ModifyIndividual.szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_COMPELLATION:	//��ʵ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation))
						{
							CopyMemory(&ModifyIndividual.szCompellation,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szCompellation[CountArray(ModifyIndividual.szCompellation)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone))
						{
							CopyMemory(ModifyIndividual.szSeatPhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szSeatPhone[CountArray(ModifyIndividual.szSeatPhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone))
						{
							CopyMemory(ModifyIndividual.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szMobilePhone[CountArray(ModifyIndividual.szMobilePhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_QQ:				//Q Q ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ))
						{
							CopyMemory(ModifyIndividual.szQQ,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szQQ[CountArray(ModifyIndividual.szQQ)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_EMAIL:			//�����ʼ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail))
						{
							CopyMemory(ModifyIndividual.szEMail,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szEMail[CountArray(ModifyIndividual.szEMail)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace))
						{
							CopyMemory(ModifyIndividual.szDwellingPlace,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szDwellingPlace[CountArray(ModifyIndividual.szDwellingPlace)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_HEAD_HTTP:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szHeadHttp));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szHeadHttp))
						{
							CopyMemory(ModifyIndividual.szHeadHttp,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szHeadHttp[CountArray(ModifyIndividual.szHeadHttp)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_CHANNEL:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserChannel));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserChannel))
						{
							CopyMemory(ModifyIndividual.szUserChannel,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserChannel[CountArray(ModifyIndividual.szUserChannel)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_GPS:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserGps));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserGps))
						{
							CopyMemory(ModifyIndividual.szUserGps,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserGps[CountArray(ModifyIndividual.szUserChannel)-1]=0;
						}
						break;
					}
				}
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_SHARE_AWARD:
		{
			ASSERT(wDataSize==sizeof(CMD_GP_ShareAward));
			if (wDataSize!=sizeof(CMD_GP_ShareAward)) return false;

			//������Ϣ
			CMD_GP_ShareAward * pNetInfo=(CMD_GP_ShareAward *)pData;

			DBR_GP_ShareAward kDBInfo;
			kDBInfo.dwUserID = pNetInfo->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SHARE_AWARD,dwSocketID,&kDBInfo,sizeof(kDBInfo));

			return true;
		}
	case SUB_GP_MODIFY_STARVALUE:	//������Ϸ��
		{
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyStarValue));
			if (wDataSize!=sizeof(CMD_GP_ModifyStarValue)) return false;

			//������Ϣ
			CMD_GP_ModifyStarValue * pNetInfo=(CMD_GP_ModifyStarValue *)pData;
			
			DBR_GP_ModifyStarValue kDBInfo;
			kDBInfo.dwUserID = pNetInfo->dwUserID;
			kDBInfo.dwStarValue = pNetInfo->dwStarValue;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_STARVALUE,dwSocketID,&kDBInfo,sizeof(kDBInfo));
			return true;
			//Ч�����
		};
	case SUB_GP_USER_SAVE_SCORE:	//������Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//ȡ����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE://ת����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//������Ϣ
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szNickName[CountArray(pUserTransferScore->szNickName)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//��������
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.cbByNickName=pUserTransferScore->cbByNickName;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szNickName,pUserTransferScore->szNickName,CountArray(UserTransferScore.szNickName));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_USER_INSURE_HESTORY:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureHestory));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureHestory)) return false;

			//������Ϣ
			CMD_GP_QueryInsureHestory * pQueryInsureInfo=(CMD_GP_QueryInsureHestory *)pData;

			//��������
			DBR_GP_QueryInsureHestory QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_INSURE_HISTORY,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //��ѯ�û�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//������Ϣ
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//��������
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//��������
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryInsureUserInfo.szNickName));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case SUB_GP_CHECKIN_QUERY:		  //��ѯǩ��
		{
			OnTCPNetworkSubPCCheckinQuery(wSubCmdID,pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GP_BEGINNER_QUERY:		  //��������ǩ��
		{
			OnTCPNetworkSubPCBiggerQuery(wSubCmdID,pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GP_BEGINNER_DONE:		  //��������ǩ��
		{
			OnTCPNetworkSubPCBiggerAward(wSubCmdID,pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GP_CHECKIN_DONE:			  //ִ��ǩ��
		{
			OnTCPNetworkSubPCCheckInDone(wSubCmdID,pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GP_CHECKIN_AWARD:			  //ִ��ǩ��
		{
			OnTCPNetworkSubPCCheckAward(wSubCmdID,pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GP_BASEENSURE_LOAD:		//���صͱ�
		{
			return OnTCPNetworkSubPCBaseensureLoad(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_BASEENSURE_TAKE:		  //��ȡ�ͱ�
		{
			return OnTCPNetworkSubPCBaseensureTake(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_ROULETTE_USERINFO:		  //�����û���Ϣ
		{
			return OnTCPNetworkSubPCRouletteUserInfo(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_ROULETTE_DONE:		  //ִ������
		{
			return OnTCPNetworkSubPCRouletteDone(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_ADDRANK_GET_AWARD_INFO:		  
		{
			return OnTCPNetworkSubAddBankAwardInfo(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_ADDRANK_GET_RANK:		  
		{
			return OnTCPNetworkSubGetAddBank(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_MATCH_SIGNUP:			//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_MatchSignup));
			if (wDataSize!=sizeof(CMD_GP_MatchSignup)) return false;

			//������Ϣ
			CMD_GP_MatchSignup * pMatchSignup=(CMD_GP_MatchSignup *)pData;
			pMatchSignup->szPassword[CountArray(pMatchSignup->szPassword)-1]=0;
			pMatchSignup->szMachineID[CountArray(pMatchSignup->szMachineID)-1]=0;			

			//���ҷ���
			CGameServerItem * pGameServerItem = m_ServerListManager.SearchGameServer(pMatchSignup->wServerID);
			if(pGameServerItem==NULL || pGameServerItem->IsMatchServer()==false)
			{
				//����ʧ��
				SendOperateFailure(dwSocketID, 0,TEXT("��Ǹ���������ı��������ڻ����Ѿ�������"));
				return true;
			}

			//����ṹ
			DBR_GP_MatchSignup MatchSignup;

			//������Ϣ
			MatchSignup.wServerID = pMatchSignup->wServerID;
			MatchSignup.dwMatchID = pMatchSignup->dwMatchID;
			MatchSignup.dwMatchNO = pMatchSignup->dwMatchNO;

			//�û���Ϣ
			MatchSignup.dwUserID = pMatchSignup->dwUserID;
			lstrcpyn(MatchSignup.szPassword,pMatchSignup->szPassword,CountArray(MatchSignup.szPassword));

			//������Ϣ
			MatchSignup.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(MatchSignup.szMachineID,pMatchSignup->szMachineID,CountArray(MatchSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MATCH_SIGNUP,dwSocketID,&MatchSignup,sizeof(MatchSignup));

			return true;
		}
	case SUB_GP_MATCH_UNSIGNUP:			//ȡ������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_MatchUnSignup));
			if (wDataSize!=sizeof(CMD_GP_MatchUnSignup)) return false;

			//������Ϣ
			CMD_GP_MatchUnSignup * pMatchUnSignup=(CMD_GP_MatchUnSignup *)pData;
			pMatchUnSignup->szPassword[CountArray(pMatchUnSignup->szPassword)-1]=0;
			pMatchUnSignup->szMachineID[CountArray(pMatchUnSignup->szMachineID)-1]=0;			

			//����ṹ
			DBR_GP_MatchUnSignup MatchUnSignup;

			//������Ϣ
			MatchUnSignup.wServerID = pMatchUnSignup->wServerID;
			MatchUnSignup.dwMatchID = pMatchUnSignup->dwMatchID;
			MatchUnSignup.dwMatchNO = pMatchUnSignup->dwMatchNO;

			//�û���Ϣ
			MatchUnSignup.dwUserID = pMatchUnSignup->dwUserID;
			lstrcpyn(MatchUnSignup.szPassword,pMatchUnSignup->szPassword,CountArray(MatchUnSignup.szPassword));

			//������Ϣ
			MatchUnSignup.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(MatchUnSignup.szMachineID,pMatchUnSignup->szMachineID,CountArray(MatchUnSignup.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MATCH_UNSIGNUP,dwSocketID,&MatchUnSignup,sizeof(MatchUnSignup));

			return true;
		}
	case SUB_GP_MATCH_AWARD_LIST:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_MatchGetAward));
			if (wDataSize!=sizeof(CMD_GP_MatchGetAward)) return false;

			//������Ϣ
			CMD_GP_MatchGetAward * pMacthGetAward = (CMD_GP_MatchGetAward *)pData;		

			//����ṹ
			DBR_GR_LoadMatchReward kLoadMatchReward;

			//������Ϣ
			kLoadMatchReward.dwUserID = pMacthGetAward->dwUserID;
			kLoadMatchReward.dwMatchID = pMacthGetAward->dwMatchID;
			kLoadMatchReward.dwMatchNO = pMacthGetAward->dwMatchNO;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MATCH_AWARD,dwSocketID,&kLoadMatchReward,sizeof(kLoadMatchReward));

			return true;
		}
	case SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO:
		{
			return OnTCPNetworkSubGetExchangeHuaFei(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_EXCHANGE_DONE:
		{
			return OnTCPNetworkSubExchangeDone(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_SHOPINFO_GET_LIST_INFO:
		{
			return OnTCPNetworkSubGetShopInfo(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_SHOPINFO_BUYODER:
		{
			return OnTCPNetworkSubBuyOder(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_SELLITEM:
		{
			return OnTCPNetworkSubSellItem(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GP_SELLLIST_BACK:
		{
			return OnTCPNetworkSubGetSellItemRecord(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_CREATE_CLUB: // �������ֲ� phw
		{
			return OnTCPNetworkSubCreatClub(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_JOIN_CLUB: // ������ֲ�
		{
			return OnTCPNetworkSubJoinClub(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_JOIN_CLUB_COMFIRM: // �᳤ͬ���ͬ�������ֲ�
		{
			return OnTCPNetworkSubJoinClubComfirm(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LEAVE_CLUB: // �뿪���ֲ�
		{
			return OnTCPNetworkSubLeaveClub(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_KICK_CLUB: // �߳����ֲ�
		{
			return OnTCPNetworkSubKickClub(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_INVITE_CLUB: // ������Ҽ�����ֲ�
		{
			return OnTCPNetworkSubInviteClub(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_GET_MY_CLUB_REQ: // ��ȡ�Լ��ľ��ֲ�����
		{
			return   OnTCPNetworkSubGetMyClubReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_CLUB_MEMBER_LIST_REQ: // �᳤���ֲ���Ա�б�����
		{
			return OnTCPNetworkSubMmmberListReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_UPDATE_CLUB_REQ: // �᳤���¾��ֲ�����
		{
			return OnTCPNetworkSubUpdateClubReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_DISMISS_CLUB_REQ: // ��ɢ���ֲ�����
		{
			return OnTCPNetworkSubDismissClubReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_CLUB_CHANGE_MASTER_REQ: // ת�û᳤����
		{
			return OnTCPNetworkSubChangeMasterReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_CLUB_SET_MANAGER_REQ: // �᳤���ù���Ա����
		{
			return OnTCPNetworkSubSetManagerReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SERVER_ADDRESS_REQ:  //
		{
			return  OnTCPNetworkServerAdressReq(wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//����ʧ��
VOID CAttemperEngineSink::SendOperateFailure(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe)
{
	//Ч�����
	ASSERT(pszDescribe != NULL);
	if(pszDescribe == NULL) return;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	OperateFailure.lResultCode=lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pszDescribe,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return;
}

bool CAttemperEngineSink::OnTCPNetworkSubPCBaseensureLoad(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����ṹ
	CMD_GP_BaseEnsureParamter BaseEnsureParameter;
	BaseEnsureParameter.cbTakeTimes=m_BaseEnsureParameter.cbTakeTimes;
	BaseEnsureParameter.lScoreAmount=m_BaseEnsureParameter.lScoreAmount;
	BaseEnsureParameter.lScoreCondition=m_BaseEnsureParameter.lScoreCondition;

	//Ͷ������
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_PARAMETER,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubPCBaseensureTake(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_BaseEnsureTake));
	if(wDataSize!=sizeof(CMD_GP_BaseEnsureTake)) return false;

	//��ȡ����
	CMD_GP_BaseEnsureTake * pBaseEnsureTake = (CMD_GP_BaseEnsureTake *)pData;
	pBaseEnsureTake->szPassword[CountArray(pBaseEnsureTake->szPassword)-1]=0;
	pBaseEnsureTake->szMachineID[CountArray(pBaseEnsureTake->szMachineID)-1]=0;

	//����ṹ
	DBR_GP_TakeBaseEnsure TakeBaseEnsure;
	TakeBaseEnsure.dwUserID = pBaseEnsureTake->dwUserID;
	TakeBaseEnsure.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(TakeBaseEnsure.szPassword,pBaseEnsureTake->szPassword,CountArray(TakeBaseEnsure.szPassword));
	lstrcpyn(TakeBaseEnsure.szMachineID,pBaseEnsureTake->szMachineID,CountArray(TakeBaseEnsure.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BASEENSURE_TAKE,dwSocketID,&TakeBaseEnsure,sizeof(TakeBaseEnsure));
	return true;
}
//������Ϣ
bool CAttemperEngineSink::OnTCPNetworkSubPCRouletteUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_ReqRouletteUeserInfo));
	if(wDataSize!=sizeof(CMD_GP_ReqRouletteUeserInfo)) return false;

	//��ȡ����
	CMD_GP_ReqRouletteUeserInfo * pNetInfo = (CMD_GP_ReqRouletteUeserInfo *)pData;
	
	//����ṹ
	DBR_GR_ReqUserRouletteAward kDBInfo;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwVaildDay = pNetInfo->dwVaildDay;
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ROULETE_USERINFO,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}
//ִ������
bool CAttemperEngineSink::OnTCPNetworkSubPCRouletteDone(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_ReqRouletteDone));
	if(wDataSize!=sizeof(CMD_GP_ReqRouletteDone)) return false;

	//��ȡ����
	CMD_GP_ReqRouletteDone * pNetInfo = (CMD_GP_ReqRouletteDone *)pData;

	int iPercent = 0;
	for (int i = 0;i<m_kBackRouletteUeserInfo.cbCout;i++)
	{
		iPercent += m_kBackRouletteUeserInfo.cbAwardPercent[i];
	}
	if (iPercent <= 0)
	{
		return true;
	}
	DBR_GP_RouletteDone kDBInfo;
	ZeroMemory(&kDBInfo,sizeof(DBR_GP_RouletteDone));
	int iRandValue = rand()%iPercent;
	for (int i = 0;i<m_kBackRouletteUeserInfo.cbCout;i++)
	{
		if (iRandValue < m_kBackRouletteUeserInfo.cbAwardPercent[i])
		{
			kDBInfo.dwUserID = pNetInfo->dwUserID;
			kDBInfo.cbRewardType = m_kBackRouletteUeserInfo.cbRewardType[i];
			kDBInfo.lRewardGold = m_kBackRouletteUeserInfo.lRewardGold[i];
			kDBInfo.cbRewardIdex = m_kBackRouletteUeserInfo.cbRouletteIdex[i];
			break;
		}
		iRandValue -= m_kBackRouletteUeserInfo.cbAwardPercent[i];
	}
	if (kDBInfo.dwUserID == 0)
	{
		return true;
	}
	CMD_GP_BackRouletteDone kSendCMDInfo;
	kSendCMDInfo.dwUserID = kDBInfo.dwUserID;
	kSendCMDInfo.cbRewardType = kDBInfo.cbRewardType;
	kSendCMDInfo.lRewardGold = kDBInfo.lRewardGold;
	kSendCMDInfo.cbRewardIdex = kDBInfo.cbRewardIdex;
	//Ͷ������
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_ROULETTE_DONE,&kSendCMDInfo,sizeof(kSendCMDInfo));
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ROULETE_DONE,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ӱ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//���ұ���
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}
//��ѯǩ��
bool CAttemperEngineSink::OnTCPNetworkSubPCCheckinQuery(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_CheckInQueryInfo));
	if(wDataSize!=sizeof(CMD_GP_CheckInQueryInfo)) return false;

	//��ȡ����
	CMD_GP_CheckInQueryInfo *pCheckInQueryInfo = (CMD_GP_CheckInQueryInfo *)pData;
	pCheckInQueryInfo->szPassword[CountArray(pCheckInQueryInfo->szPassword)-1]=0;

	//����ṹ
	DBR_GP_CheckInQueryInfo CheckInQueryInfo;
	CheckInQueryInfo.dwUserID = pCheckInQueryInfo->dwUserID;
	lstrcpyn(CheckInQueryInfo.szPassword,pCheckInQueryInfo->szPassword,CountArray(CheckInQueryInfo.szPassword));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECKIN_QUERY_INFO,dwSocketID,&CheckInQueryInfo,sizeof(CheckInQueryInfo));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubPCBiggerQuery(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_BeginnerQueryInfo));
	if(wDataSize!=sizeof(CMD_GP_BeginnerQueryInfo)) return false;

	//��ȡ����
	CMD_GP_BeginnerQueryInfo *pNetInfo = (CMD_GP_BeginnerQueryInfo *)pData;
	pNetInfo->szPassword[CountArray(pNetInfo->szPassword)-1]=0;

	//����ṹ
	DBR_GP_BeginnerQueryInfo kDBInfo;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	lstrcpyn(kDBInfo.szPassword,pNetInfo->szPassword,CountArray(kDBInfo.szPassword));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BEGINNER_QUERY_INFO,dwSocketID,&kDBInfo,sizeof(kDBInfo));

	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubPCBiggerAward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_BeginnerDone));
	if(wDataSize!=sizeof(CMD_GP_BeginnerDone)) return false;

	//��ȡ����
	CMD_GP_BeginnerDone *pCheckInDone = (CMD_GP_BeginnerDone *)pData;
	pCheckInDone->szPassword[CountArray(pCheckInDone->szPassword)-1]=0;
	pCheckInDone->szMachineID[CountArray(pCheckInDone->szMachineID)-1]=0;

	//����ṹ
	DBR_GP_BeginnerDone CheckInDone;
	CheckInDone.dwUserID = pCheckInDone->dwUserID;
	CheckInDone.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(CheckInDone.szPassword,pCheckInDone->szPassword,CountArray(CheckInDone.szPassword));
	lstrcpyn(CheckInDone.szMachineID,pCheckInDone->szMachineID,CountArray(CheckInDone.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BEGINNER_DONE,dwSocketID,&CheckInDone,sizeof(CheckInDone));

	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubGetAddBank(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	ASSERT(wDataSize==sizeof(CMD_GP_GetAddBank));
	if(wDataSize!=sizeof(CMD_GP_GetAddBank)) return false;

	//��ȡ����
	CMD_GP_GetAddBank *pNetInfo = (CMD_GP_GetAddBank *)pData;
	pNetInfo->szPassword[CountArray(pNetInfo->szPassword)-1]=0;

	DBR_GP_GetAddBank kDBInfo;
	kDBInfo.iIdex = pNetInfo->iRankIdex;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	lstrcpyn(kDBInfo.szPassword,pNetInfo->szPassword,CountArray(kDBInfo.szPassword));


	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_ADDRANK,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubAddBankAwardInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_ADDRANK_BACK_AWARD_INFO,&m_BackAddBankAwardInfo,sizeof(m_BackAddBankAwardInfo));

	return true;
}
//ִ��
bool CAttemperEngineSink::OnTCPNetworkSubPCCheckInDone(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_CheckInDone));
	if(wDataSize!=sizeof(CMD_GP_CheckInDone)) return false;

	//��ȡ����
	CMD_GP_CheckInDone *pCheckInDone = (CMD_GP_CheckInDone *)pData;
	pCheckInDone->szPassword[CountArray(pCheckInDone->szPassword)-1]=0;
	pCheckInDone->szMachineID[CountArray(pCheckInDone->szMachineID)-1]=0;

	//����ṹ
	DBR_GP_CheckInDone CheckInDone;
	CheckInDone.dwUserID = pCheckInDone->dwUserID;
	CheckInDone.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(CheckInDone.szPassword,pCheckInDone->szPassword,CountArray(CheckInDone.szPassword));
	lstrcpyn(CheckInDone.szMachineID,pCheckInDone->szMachineID,CountArray(CheckInDone.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECKIN_DONE,dwSocketID,&CheckInDone,sizeof(CheckInDone));

	return true;
}
//ִ��
bool CAttemperEngineSink::OnTCPNetworkSubPCCheckAward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//����У��
	ASSERT(wDataSize==sizeof(CMD_GP_CheckInDone));
	if(wDataSize!=sizeof(CMD_GP_CheckInDone)) return false;

	//��ȡ����
	CMD_GP_CheckInDone *pCheckInDone = (CMD_GP_CheckInDone *)pData;
	pCheckInDone->szPassword[CountArray(pCheckInDone->szPassword)-1]=0;
	pCheckInDone->szMachineID[CountArray(pCheckInDone->szMachineID)-1]=0;

	//����ṹ
	DBR_GP_CheckInDone CheckInDone;
	CheckInDone.dwUserID = pCheckInDone->dwUserID;
	CheckInDone.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(CheckInDone.szPassword,pCheckInDone->szPassword,CountArray(CheckInDone.szPassword));
	lstrcpyn(CheckInDone.szMachineID,pCheckInDone->szMachineID,CountArray(CheckInDone.szMachineID));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECKIN_AWARD,dwSocketID,&CheckInDone,sizeof(CheckInDone));

	return true;
}
//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID))
	{
		if (wDataSize<sizeof(CMD_GP_LogonGameID)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonGameID->dwPlazaVersion,dwSocketID,((pLogonGameID->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter + LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID = pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr = (m_pBindParameter + LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	LogonGameID.cbNeeValidateMBCard=(pLogonGameID->cbValidateFlags&MB_VALIDATE_FLAGS);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_LogonAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	ConvertUtf8ToGBK(pRegisterAccounts->szNickName,LEN_NICKNAME);
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubPCVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	ASSERT(wDataSize>=sizeof(CMD_GP_VisitorLogon));
	if (wDataSize<sizeof(CMD_GP_VisitorLogon)) return false;
	CMD_GP_VisitorLogon * pVisitorAccounts=(CMD_GP_VisitorLogon *)pData;
	pVisitorAccounts->szPassWord[CountArray(pVisitorAccounts->szPassWord)-1]=0;
	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pVisitorAccounts->dwPlazaVersion,dwSocketID,((pVisitorAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}
	DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

	ConvertUtf8ToGBK(pVisitorAccounts->szNickName,LEN_NICKNAME);

	DBR_GP_LogonVisitor VisitorAccounts;
	ZeroMemory(&VisitorAccounts,sizeof(VisitorAccounts));
	VisitorAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));
	VisitorAccounts.dwClientIP=dwClientAddr;
	VisitorAccounts.wFaceID=pVisitorAccounts->wFaceID;
	VisitorAccounts.cbGender=pVisitorAccounts->cbGender;
	lstrcpyn(VisitorAccounts.szNickName,pVisitorAccounts->szNickName,CountArray(VisitorAccounts.szNickName));
	lstrcpyn(VisitorAccounts.szSpreader,pVisitorAccounts->szSpreader,CountArray(VisitorAccounts.szSpreader));
	lstrcpyn(VisitorAccounts.szPassWord,pVisitorAccounts->szPassWord,CountArray(VisitorAccounts.szPassWord));
	lstrcpyn(VisitorAccounts.szPassWordBank,pVisitorAccounts->szPassWordBank,CountArray(VisitorAccounts.szPassWordBank));

	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VISITOR_ACCOUNTS,dwSocketID,&VisitorAccounts,sizeof(VisitorAccounts));
	return true;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//��������
	(m_pBindParameter+LOWORD(dwContextID))->dwUserID = pDBOLogonSuccess->dwUserID;
	//
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->dwSpreaderID=pDBOLogonSuccess->dwSpreaderID;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	pCMDLogonSuccess->cbInsureEnabled=pDBOLogonSuccess->cbInsureEnabled;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));
	lstrcpyn(pCMDLogonSuccess->szPictureName,pDBOLogonSuccess->szPictureName,CountArray(pCMDLogonSuccess->szPictureName));
	pCMDLogonSuccess->dwPresent[0] = pDBOLogonSuccess->dwPresent[0];
	pCMDLogonSuccess->dwPresent[1] = pDBOLogonSuccess->dwPresent[1];
	pCMDLogonSuccess->dwPresent[2] = pDBOLogonSuccess->dwPresent[2];
	pCMDLogonSuccess->dwPresent[3] = pDBOLogonSuccess->dwPresent[3];
	pCMDLogonSuccess->dwPresent[4] = pDBOLogonSuccess->dwPresent[4];
	pCMDLogonSuccess->dwPresent[5] = pDBOLogonSuccess->dwPresent[5];

	////������ʾ
	//TCHAR szDescribe[LEN_PICTURE]=TEXT("");
	//sprintf_s(szDescribe,TEXT("%s"),pCMDLogonSuccess->szPictureName);
	////��ʾ��Ϣ
	//CTraceService::TraceString(szDescribe,TraceLevel_Normal);

	////������ʾ
	//TCHAR szDescribe1[LEN_PICTURE]=TEXT("");
	//sprintf_s(szDescribe1,TEXT("%d,%d,%d,%d,%d,%d"),pCMDLogonSuccess->dwPresent[0],pCMDLogonSuccess->dwPresent[1],pCMDLogonSuccess->dwPresent[2],
	//	pCMDLogonSuccess->dwPresent[3],pCMDLogonSuccess->dwPresent[4],pCMDLogonSuccess->dwPresent[5]);
	////��ʾ��Ϣ
	//CTraceService::TraceString(szDescribe1,TraceLevel_Normal);

	//�û��ɼ�
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//������Ϣ
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//����ǩ��
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//�����б�
	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		//�����б�
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,INVALID_WORD);
		SendGameNodeInfo(dwContextID,INVALID_WORD);
		SendGameServerInfo(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		SendGameTypeInfo(dwContextID);
		SendGameKindInfo(dwContextID);
		SendGamePageInfo(dwContextID,0);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}

	//�����б�
	SendUserSignupInfo(dwContextID,pDBOLogonSuccess->wSignupCount,pDBOLogonSuccess->SignupMatchInfo);

	//��¼���
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	return true;
}

//���ͱ���
VOID CAttemperEngineSink::SendUserSignupInfo(DWORD dwSocketID,WORD wSignupCount,tagSignupMatchInfo * pSignupMatchInfo)
{
	//����У��
	if(wSignupCount==0 || pSignupMatchInfo==NULL) return;

	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//��������	
	while((wSignupCount*sizeof(tagSignupMatchInfo))>=sizeof(cbDataBuffer))
	{
		//��������
		wSendSize = (sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo))*sizeof(tagSignupMatchInfo);
		CopyMemory(cbDataBuffer,pSignupMatchInfo,wSendSize);

		//��������
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);

		//����ָ��
		pSignupMatchInfo += sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo);
		wSignupCount -= sizeof(cbDataBuffer)/sizeof(tagSignupMatchInfo);		
	}

	//ʣ�෢��
	if(wSignupCount>0)
	{
		//��������
		wSendSize = wSignupCount*sizeof(tagSignupMatchInfo);
		CopyMemory(cbDataBuffer,pSignupMatchInfo,wSendSize);
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_MATCH_SIGNUPINFO,cbDataBuffer,wSendSize);
	}

	return;
}
//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_ValidateMBCard));
	if (wDataSize!=sizeof(DBR_GP_ValidateMBCard)) return false;

	DBR_GP_ValidateMBCard *pValidateMBCard=(DBR_GP_ValidateMBCard *)pData;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_MBCARD,pData,wDataSize);

	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;
	UserFaceInfo.dwCustomID=pUserFaceInfo->dwCustomID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//���ñ���
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//��������
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;
	pCMDUserIndividual->dwUserStarValue = pDBOUserIndividual->dwUserStarValue;
	pCMDUserIndividual->dwUserStarCout = pDBOUserIndividual->dwUserStarCout;
	//�û���Ϣ
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//��ʵ����
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//�绰����
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//�ƶ��绰
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//��ϵ����
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//�����ʼ�
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//��ϵ��ַ
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}
	//ͷ��http
	if (pDBOUserIndividual->szHeadHttp[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szHeadHttp);
		SendPacket.AddPacket(pDBOUserIndividual->szHeadHttp,wBufferSize,DTP_GP_UI_HEAD_HTTP);
	}
	//IP
	if (pDBOUserIndividual->szLogonIP[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szLogonIP);
		SendPacket.AddPacket(pDBOUserIndividual->szLogonIP,wBufferSize,DTP_GP_UI_IP);
	}
	//������
	if (pDBOUserIndividual->szUserChannel[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserChannel);
		SendPacket.AddPacket(pDBOUserIndividual->szUserChannel,wBufferSize,DTP_GP_UI_CHANNEL);
	}
	//GPS
	if (pDBOUserIndividual->szUserGps[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserGps);
		SendPacket.AddPacket(pDBOUserIndividual->szUserGps,wBufferSize,DTP_GP_UI_GPS);
	}
	
	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);
	return true;
}
bool CAttemperEngineSink::OnDBPCUserInsureReturn(DWORD dwContextID, VOID* pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_AWARD_RETURN * pDBOUserIndividual=(DBO_GP_AWARD_RETURN *)pData;
	CMD_GP_AWARD_RETURN kCMDUserParent;
	CopyMemory(&kCMDUserParent,pDBOUserIndividual,sizeof(kCMDUserParent));
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_AWARD_RETURN,&kCMDUserParent,sizeof(kCMDUserParent));
	return true;
}
//�û�������Ϣ
bool CAttemperEngineSink::OnDBPCUserAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserAccountInfo * pDBOUserIndividual=(DBO_GP_UserAccountInfo *)pData;
	CMD_GP_UserAccountInfo kCMDUserIndividual;
	CopyMemory(&kCMDUserIndividual,pDBOUserIndividual,sizeof(kCMDUserIndividual));
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_ACCOUNTINFO,&kCMDUserIndividual,sizeof(kCMDUserIndividual));
	return true;
}

// ��ѯ��ҳ�ֵ��¼
bool CAttemperEngineSink::OnDBUserInsuerHistory(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureHistory * pUserInsureInfo=(DBO_GP_UserInsureHistory *)pData;

	//��������
	CMD_GP_UserInsureHistory UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));
	for (int i = 0;i!= 100; ++i)
	{
		UserInsureInfo.infos[i].m_rechargeid = pUserInsureInfo->infos[i].m_rechargeid;
		UserInsureInfo.infos[i].m_add_insure = pUserInsureInfo->infos[i].m_add_insure;
		UserInsureInfo.infos[i].m_get_insure = pUserInsureInfo->infos[i].m_get_insure;
		UserInsureInfo.infos[i].m_insure_time = pUserInsureInfo->infos[i].m_insure_time;
		CopyMemory(UserInsureInfo.infos[i].szRechargeKey,pUserInsureInfo->infos[i].szRechargeKey , 128);
	}

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INSURE_HESTORY_SUCCESS,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	// m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	//�ر�����
	// m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
//��ֵ�ɹ�
bool CAttemperEngineSink::OnDBUserPaySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;
	if(pUserInsureSuccess==NULL)
	{
		return false;
	}
	DWORD dwUserID = pUserInsureSuccess->dwUserID;
	if (dwUserID == 0)
	{
		return false;
	}

	bool getit = false;
	for (int i = 0; i < m_pInitParameter->m_wMaxConnect; i++)
	{
		tagBindParameter * ptag = m_pBindParameter +i;
		if (ptag != NULL && ptag->dwUserID == dwUserID)
		{
			dwContextID = ptag->dwSocketID;
			getit = true;
			break;
		}
	}
	if (getit == false)
	{
		return false;
	}

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	//DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lDestScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lDestInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lDestScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lDestInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//��������
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}
//�Զ������ݲ�ѯ
bool CAttemperEngineSink::OnDBPCPublicNotice(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_PublicNotice kCMDOutInfo;
	ZeroMemory(&kCMDOutInfo,sizeof(kCMDOutInfo));

	//��������
	DBO_GP_PublicNotice * pDBOutInfo=(DBO_GP_PublicNotice *)pData;

	//��������
	kCMDOutInfo.lResultCode=pDBOutInfo->lResultCode;
	lstrcpyn(kCMDOutInfo.szDescribeString,pDBOutInfo->szDescribeString,CountArray(kCMDOutInfo.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(kCMDOutInfo.szDescribeString);
	WORD wHeadSize=sizeof(kCMDOutInfo)-sizeof(kCMDOutInfo.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_PUBLIC_NOTICE,&kCMDOutInfo,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}
bool CAttemperEngineSink::OnDBPCInGameSevrerID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_UserInGameServerID * pDBInfo=(DBO_GP_UserInGameServerID *)pData;
	//��������
	CMD_GP_InGameSeverID kNetInfo;
	ZeroMemory(&kNetInfo,sizeof(kNetInfo));

	kNetInfo.LockKindID=pDBInfo->LockKindID;
	kNetInfo.LockServerID=pDBInfo->LockServerID;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_INGAME_SEVERID,&kNetInfo,sizeof(kNetInfo));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
bool CAttemperEngineSink::OnDBPCOSpreaderResoult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_SpreaderResoult * pDBInfo=(DBO_GP_SpreaderResoult *)pData;
	//��������
	CMD_GP_SpreaderResoult SpreaderResoult;
	ZeroMemory(&SpreaderResoult,sizeof(SpreaderResoult));

	SpreaderResoult.lResultCode=pDBInfo->lResultCode;
	SpreaderResoult.lScore=pDBInfo->lScore;
	lstrcpyn(SpreaderResoult.szDescribeString,pDBInfo->szDescribeString,CountArray(SpreaderResoult.szDescribeString));

	WORD wDescribe=CountStringBuffer(SpreaderResoult.szDescribeString);
	WORD wHeadSize=sizeof(SpreaderResoult)-sizeof(SpreaderResoult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_SPREADER_RESOULT,&SpreaderResoult,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�������
bool CAttemperEngineSink::OnDBPCUserMatchSignupResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_MatchSignupResult * pMatchSignupResult=(DBO_GP_MatchSignupResult *)pData;

	//��������
	CMD_GP_MatchSignupResult MatchSignupResult;
	ZeroMemory(&MatchSignupResult,sizeof(MatchSignupResult));

	//����ṹ
	MatchSignupResult.bSignup = pMatchSignupResult->bSignup;
	MatchSignupResult.bSuccessed = pMatchSignupResult->bSuccessed;
	lstrcpyn(MatchSignupResult.szDescribeString,pMatchSignupResult->szDescribeString,CountArray(MatchSignupResult.szDescribeString));

	//��������
	WORD wSendSize = sizeof(MatchSignupResult)-sizeof(MatchSignupResult.szDescribeString)+CountStringBuffer(MatchSignupResult.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_MATCH_SIGNUP_RESULT,&MatchSignupResult,sizeof(MatchSignupResult));

	return true;
}
// ��������
bool CAttemperEngineSink::OnDBPCMacthAwardList( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//DBO_GR_MatchAwardList
	//DBO_GR_MatchAwardList
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_MATCH_AWARD_LIST,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//��������
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//��������
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwGameID=pDBOLogonSuccess->dwGameID;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	LogonSuccess.dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));

	//��¼�ɹ�
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//��Ϸ�ڵ�
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//��������
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//�����б�
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//��������
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//�����б�
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}
//ǩ������
bool CAttemperEngineSink::OnDBPCCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(DBO_GP_CheckInReward));
	if(wDataSize!=sizeof(DBO_GP_CheckInReward)) return false;

	//��ȡ����
	DBO_GP_CheckInReward * pCheckInReward=(DBO_GP_CheckInReward *)pData;

	//��������
	CopyMemory(m_kCheckInInfo.lRewardDay,pCheckInReward->lRewardDay,sizeof(m_kCheckInInfo.lRewardDay));
	CopyMemory(m_kCheckInInfo.lRewardGold,pCheckInReward->lRewardGold,sizeof(m_kCheckInInfo.lRewardGold));
	CopyMemory(m_kCheckInInfo.lRewardType,pCheckInReward->lRewardType,sizeof(m_kCheckInInfo.lRewardType));

	return true;
}

//ǩ����Ϣ
bool CAttemperEngineSink::OnDBPCUserCheckInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_CheckInInfo * pCheckInInfo=(DBO_GP_CheckInInfo *)pData;

	//��������
	CMD_GP_CheckInInfo CheckInInfo;
	ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

	//�������
	CheckInInfo.bTodayChecked=pCheckInInfo->bTodayChecked;
	CheckInInfo.wAwardDate=pCheckInInfo->wAwardDate;
	CheckInInfo.wSeriesDate=pCheckInInfo->wSeriesDate;
	CopyMemory(CheckInInfo.lRewardDay,m_kCheckInInfo.lRewardDay,sizeof(CheckInInfo.lRewardDay));
	CopyMemory(CheckInInfo.lRewardGold,m_kCheckInInfo.lRewardGold,sizeof(CheckInInfo.lRewardGold));
	CopyMemory(CheckInInfo.lRewardType,m_kCheckInInfo.lRewardType,sizeof(CheckInInfo.lRewardType));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_INFO,&CheckInInfo,sizeof(CheckInInfo));

	return true;
}

//ǩ�����
bool CAttemperEngineSink::OnDBPCUserCheckInResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_CheckInResult * pCheckInResult=(DBO_GP_CheckInResult *)pData;

	//��������
	CMD_GP_CheckInResult CheckInResult;
	ZeroMemory(&CheckInResult,sizeof(CheckInResult));

	//�������
	CheckInResult.bType=pCheckInResult->bType;
	CheckInResult.bSuccessed=pCheckInResult->bSuccessed;
	CheckInResult.lScore=pCheckInResult->lScore;
	lstrcpyn(CheckInResult.szNotifyContent,pCheckInResult->szNotifyContent,CountArray(CheckInResult.szNotifyContent));

	//��������
	WORD wSendSize = sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_CHECKIN_RESULT,&CheckInResult,wSendSize);

	return true;
}
//������������
bool CAttemperEngineSink::OnDBPCBeginnerConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����У��
	ASSERT(wDataSize==sizeof(DBO_GP_BeginnerCofig));
	if(wDataSize!=sizeof(DBO_GP_BeginnerCofig)) return false;

	//��ȡ����
	DBO_GP_BeginnerCofig * pDBInfo=(DBO_GP_BeginnerCofig *)pData;

	//��������
	CopyMemory((void*)(&m_kBeginnerInfo.lRewardGold),(void*)&pDBInfo->lRewardGold,sizeof(m_kBeginnerInfo.lRewardGold));
	CopyMemory((void*)(&m_kBeginnerInfo.lRewardType),(void*)&pDBInfo->lRewardType,sizeof(m_kBeginnerInfo.lRewardType));

	return true;
}
//����������Ϣ
bool CAttemperEngineSink::OnDBPCUserBeginnerInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BeginnerInfo * pDBInfo=(DBO_GP_BeginnerInfo *)pData;

	//��������
	CMD_GP_BeginnerInfo kNetInfo;
	ZeroMemory(&kNetInfo,sizeof(kNetInfo));

	//�������
	kNetInfo.bTodayChecked=pDBInfo->bTodayChecked;
	kNetInfo.wSeriesDate=pDBInfo->wSeriesDate;
	kNetInfo.bLastCheckIned=pDBInfo->bLastCheckIned;
	CopyMemory((void*)(&kNetInfo.lRewardGold),(void*)&m_kBeginnerInfo.lRewardGold,sizeof(kNetInfo.lRewardGold));
	CopyMemory((void*)(&kNetInfo.lRewardType),(void*)&m_kBeginnerInfo.lRewardType,sizeof(kNetInfo.lRewardType));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BEGINNER_INFO,&kNetInfo,sizeof(kNetInfo));

	return true;
}
//�����������
bool CAttemperEngineSink::OnDBPCUserBeginnerResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BeginnerResult * pDBInfo = (DBO_GP_BeginnerResult *)pData;

	//��������
	CMD_GP_BeginnerResult kNetInfo;
	ZeroMemory(&kNetInfo,sizeof(kNetInfo));

	//�������
	kNetInfo.bSuccessed=pDBInfo->bSuccessed;
	kNetInfo.lAwardCout=pDBInfo->lAwardCout;
	kNetInfo.bSuccessed=pDBInfo->bSuccessed;
	kNetInfo.lAwardType=pDBInfo->lAwardType;
	lstrcpyn(kNetInfo.szNotifyContent,pDBInfo->szNotifyContent,CountArray(kNetInfo.szNotifyContent));

	//��������
	WORD wSendSize = sizeof(kNetInfo)-sizeof(kNetInfo.szNotifyContent)+CountStringBuffer(kNetInfo.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BEGINNER_RESULT,&kNetInfo,wSendSize);

	return true;
}
bool CAttemperEngineSink::OnDBPCUserRouletteAward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_RouletteAward * pDBInfo = (DBO_GP_RouletteAward *)pData;

	ZeroMemory(&m_kBackRouletteUeserInfo,sizeof(m_kBackRouletteUeserInfo));
	m_kBackRouletteUeserInfo.cbCout = pDBInfo->cbCout;
	memcpy(m_kBackRouletteUeserInfo.cbAwardPercent,pDBInfo->cbAwardPercent,sizeof(m_kBackRouletteUeserInfo.cbAwardPercent));
	memcpy(m_kBackRouletteUeserInfo.cbRewardType,pDBInfo->cbRewardType,sizeof(m_kBackRouletteUeserInfo.cbRewardType));
	memcpy(m_kBackRouletteUeserInfo.cbRouletteIdex,pDBInfo->cbRouletteIdex,sizeof(m_kBackRouletteUeserInfo.cbRouletteIdex));
	memcpy(m_kBackRouletteUeserInfo.lRewardGold,pDBInfo->lRewardGold,sizeof(m_kBackRouletteUeserInfo.lRewardGold));
	return true;
}
bool CAttemperEngineSink::OnDBPCGameConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	tagGameConfig * pDBInfo = (tagGameConfig *)pData;

	memcpy(&m_kGameConfig,pData,sizeof(tagGameConfig));

	return true;
}
bool CAttemperEngineSink::OnDBPCUserRouletteUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_RouletteUserInfo * pDBInfo = (DBO_GP_RouletteUserInfo *)pData;

	m_kBackRouletteUeserInfo.dwUserID = pDBInfo->dwUserID;
	m_kBackRouletteUeserInfo.dwHaveDone = pDBInfo->dwHaveDone;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_ROULETTE_USERINFO,&m_kBackRouletteUeserInfo,sizeof(m_kBackRouletteUeserInfo));
	return true;
}
//�ͱ�����
bool CAttemperEngineSink::OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_BaseEnsureParameter * pEnsureParameter=(DBO_GP_BaseEnsureParameter *)pData;

	//���ñ���
	m_BaseEnsureParameter.cbTakeTimes = pEnsureParameter->cbTakeTimes;
	m_BaseEnsureParameter.lScoreAmount = pEnsureParameter->lScoreAmount;
	m_BaseEnsureParameter.lScoreCondition = pEnsureParameter->lScoreCondition;

	return true;
}
//׬�����
bool CAttemperEngineSink::OnDBPCAddBankAwardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	DBO_GP_AddRankAwardConfig * pDBDataInfo=(DBO_GP_AddRankAwardConfig *)pData;
	ASSERT(pDBDataInfo->iIdex >=0 && pDBDataInfo->iIdex < 3);
	if (!(pDBDataInfo->iIdex >=0 && pDBDataInfo->iIdex < 3))
	{
		return false;
	}

	CopyMemory(m_BackAddBankAwardInfo.kRewardGold[pDBDataInfo->iIdex],pDBDataInfo->kRewardGold,sizeof(pDBDataInfo->kRewardGold));
	CopyMemory(m_BackAddBankAwardInfo.kRewardType[pDBDataInfo->iIdex],pDBDataInfo->kRewardType,sizeof(pDBDataInfo->kRewardType));

	return true;
}
//׬�������
bool CAttemperEngineSink::OnDBPCAddBankBack(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//DBO_GP_BackAddBank;//
	//CMD_GP_BackAddBank;//

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_ADDRANK_BACK_RANK,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
bool CAttemperEngineSink::OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_BaseEnsureResult * pBaseEnsureResult=(DBO_GP_BaseEnsureResult *)pData;

	//����ṹ
	CMD_GP_BaseEnsureResult BaseEnsureResult;
	BaseEnsureResult.bSuccessed=pBaseEnsureResult->bSuccessed;
	BaseEnsureResult.lGameScore=pBaseEnsureResult->lGameScore;
	lstrcpyn(BaseEnsureResult.szNotifyContent,pBaseEnsureResult->szNotifyContent,CountArray(BaseEnsureResult.szNotifyContent));

	//��������
	WORD wSendDataSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
	wSendDataSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_BASEENSURE_RESULT,&BaseEnsureResult,wSendDataSize);

	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//��������
	DWORD dwVersion=m_kGameConfig.dwVersion;


	//�����ж�
	if (dwVersion != dwPlazaVersion)
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=true;
		UpdateNotify.cbAdviceUpdate=true;
		UpdateNotify.dwCurrentVersion=dwVersion;

		lstrcpyn(UpdateNotify.szDownLoadHttp,m_kGameConfig.szDownLoadHttp,CountArray(m_kGameConfig.szDownLoadHttp));

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
		return false;
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͽڵ�
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//���Ͷ���
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	POSITION iPosition=NULL;
	CGameServerItem * pGameServerItem=NULL;
	/*int  iMaxServerCount = 0;
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//ѡ��ǰʣ�෿��������
		POSITION   itempPositon = Position;
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if(pGameServerItem == NULL) break;

		TCHAR  temp[128]  = {0};
		sprintf(temp,"ServerNumber: %d  FullCount:%d OnLineCount: %d",m_ServerListManager.GetGameServerCount(), pGameServerItem->m_GameServer.dwFullCount,
			pGameServerItem->m_GameServer.dwOnLineCount);
		CTraceService::TraceString(temp, TraceLevel_Debug);
 		if(iMaxServerCount < (pGameServerItem->m_GameServer.dwFullCount -  pGameServerItem->m_GameServer.dwOnLineCount))
		{
			iPosition =   itempPositon;
			iMaxServerCount = pGameServerItem->m_GameServer.dwFullCount -  pGameServerItem->m_GameServer.dwOnLineCount;

		}		
	}*/
	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{


		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

			//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(iPosition);
		if (pGameServerItem==NULL) break;

			//��������
		if (pGameServerItem !=NULL && (wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	//���ñ���
	wSendSize=0;
	ZeroMemory(cbDataBuffer,sizeof(cbDataBuffer));

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameMatch))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		if (pGameServerItem->IsMatchServer()==false) continue;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameMatch,sizeof(tagGameMatch));
			wSendSize+=sizeof(tagGameMatch);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_MATCH,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//��������
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//��������
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if (pGameServerItem->m_GameServer.wKindID==wModuleID)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}



bool CAttemperEngineSink::OnTCPNetworkSubGetExchangeHuaFei( WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	ASSERT(wDataSize==sizeof(CMD_GP_GetExchangeHuaFei));
	if(wDataSize!=sizeof(CMD_GP_GetExchangeHuaFei)) return false;

	//��ȡ����
	CMD_GP_GetExchangeHuaFei *pNetInfo = (CMD_GP_GetExchangeHuaFei *)pData;
	pNetInfo->szPassword[CountArray(pNetInfo->szPassword)-1]=0;

	DBR_GP_GetExchangeHuaFei kDBInfo;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	lstrcpyn(kDBInfo.szPassword,pNetInfo->szPassword,CountArray(kDBInfo.szPassword));


	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_EXCHANGE_HUAFEI,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubExchangeDone( WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	ASSERT(wDataSize==sizeof(CMD_GP_ExchangeDone));
	if(wDataSize!=sizeof(CMD_GP_ExchangeDone)) return false;

	//��ȡ����
	CMD_GP_ExchangeDone *pNetInfo = (CMD_GP_ExchangeDone *)pData;
	pNetInfo->szPassword[CountArray(pNetInfo->szPassword)-1]=0;

	DBR_GP_ExchangeDone kDBInfo;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwExchangeID = pNetInfo->dwExchangeID;
	lstrcpyn(kDBInfo.szPassword,pNetInfo->szPassword,CountArray(kDBInfo.szPassword));


	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_DONE,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubGetShopInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	ASSERT(wDataSize==sizeof(CMD_GP_GetShopInfo));
	if(wDataSize!=sizeof(CMD_GP_GetShopInfo)) return false;

	//��ȡ����
	CMD_GP_GetShopInfo *pNetInfo = (CMD_GP_GetShopInfo *)pData;
	pNetInfo->szPassword[CountArray(pNetInfo->szPassword)-1]=0;

	DBR_GP_GetShopInfo kDBInfo;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	lstrcpyn(kDBInfo.szPassword,pNetInfo->szPassword,CountArray(kDBInfo.szPassword));


	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_SHOPINFO,dwSocketID,&kDBInfo,sizeof(kDBInfo));
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubBuyOder(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//CMD_GP_UpShopOder
	//DBR_GP_UpShopOder

	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_BUYODER,dwSocketID,pData,wDataSize);
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubSellItem(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//CMD_GP_SellItem
	//DBR_GP_SellItem
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SELL_ITEM,dwSocketID,pData,wDataSize);
	return true;
}
bool CAttemperEngineSink::OnTCPNetworkSubGetSellItemRecord(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_SELL_RECORD,dwSocketID,pData,wDataSize);
	return true;
}

// �һ������б���
bool CAttemperEngineSink::OnDBPCExchangeHuaFeiBack( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//DBO_GP_BackShopInfo
	//CMD_GP_BackShopInfo
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_EXCHANGEHUAFEI_BACK,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

bool CAttemperEngineSink::OnDBPCShopInfoBack(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//DBO_GP_BackExchangeHuaFei
	//CMD_GP_BackExchangeHuaFei
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_SHOPINFO_BACK,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}
bool CAttemperEngineSink::OnDBPCSellListBack(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_SELLLIST_BACK,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ϸ��¼
bool CAttemperEngineSink::OnDBPCGameRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//DBO_GP_GameRecord_List
	//CMD_GP_BackGameRecord_List
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_LIST,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}

//��Ϸ��¼
bool CAttemperEngineSink::OnDBPCGameRecordTotal(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//DBO_GP_GameRecord_Video
	//CMD_GP_BackGameRecord_Video
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_TOTAL,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}
bool CAttemperEngineSink::OnDBPCGameRecordChild(DWORD dwContextID, VOID * pData, WORD wDataSize)
{

	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_GAME_RECORD_CHILD,pData,wDataSize);
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}

// �᳤�������ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubCreatClub(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(SUB_GR_Create_Club));	
	if (wDataSize!= sizeof(SUB_GR_Create_Club)) return false; 

	//������Ϣ
	SUB_GR_Create_Club * pNetInfo = (SUB_GR_Create_Club *) pData;

	//��������
	DBR_GR_Create_Club kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Create_Club));
	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.bGameTypeIndex = pNetInfo->bGameTypeIndex;
	kDBInfo.bGameTypeQing = pNetInfo->bGameTypeQing;
	kDBInfo.bPlayCoutIdex = pNetInfo->bPlayCoutIdex;
	kDBInfo.bGamePayType = pNetInfo->bGamePayType;
	kDBInfo.bQiangGangHu = pNetInfo->bQiangGangHu;
	kDBInfo.bAllOther = pNetInfo->bAllOther;
	kDBInfo.bMaxScore = pNetInfo->bMaxScore;
	kDBInfo.bPantherType = pNetInfo->bPantherType;
	kDBInfo.bPlayerNum = pNetInfo->bPlayerNum;
	kDBInfo.bBaoIsOutCard = pNetInfo->bBaoIsOutCard;
	kDBInfo.bPiaoShu = pNetInfo->bPiaoShu; 
	kDBInfo.bGameBaseScore = pNetInfo->bGameBaseScore;
	lstrcpyn(kDBInfo.stClubName, pNetInfo->stClubName, CountArray(kDBInfo.stClubName));
	kDBInfo.stClubName[CountArray(kDBInfo.stClubName) - 1] = 0;
	ConvertUtf8ToGBK(kDBInfo.stClubName, LEN_NICKNAME);

	if(kDBInfo.bGameTypeIndex == 2 || kDBInfo.bGameTypeIndex == 4 ) kDBInfo.bQiangGangHu=0;
	if(kDBInfo.bGameTypeIndex == 2 || kDBInfo.bGameTypeIndex == 3 ) kDBInfo.bAllOther = 0;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CREATE_CLUB, dwSocketID, &kDBInfo, sizeof(DBR_GR_Create_Club));
	return true;
}

// ���������ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubJoinClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Join_Club));
	if (wDataSize != sizeof(SUB_GR_Join_Club)) 
		return false;

	//������Ϣ
	SUB_GR_Join_Club * pNetInfo = (SUB_GR_Join_Club *) pData;
	//��������
	DBR_GR_Join_Club kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Join_Club));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_JOIN_CLUB, dwSocketID, &kDBInfo, sizeof(DBR_GR_Join_Club));
	return true;
}

// �᳤ͬ���ͬ�������ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubJoinClubComfirm(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Join_Club_Comfirm));
	if (wDataSize != sizeof(SUB_GR_Join_Club_Comfirm)) return false;

	//������Ϣ
	SUB_GR_Join_Club_Comfirm * pNetInfo = (SUB_GR_Join_Club_Comfirm *) pData;
	//��������
	DBR_GR_Jion_Club_Comfirm kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Jion_Club_Comfirm));

	//��������
	kDBInfo.dwOperateID = pNetInfo->dwOperateID;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.bComfirm = pNetInfo->bComfirm;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_JOIN_CLUB_COMFIRM, dwSocketID, &kDBInfo, sizeof(DBR_GR_Jion_Club_Comfirm));
	return true;
}

// �뿪���ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubLeaveClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Leave_Club));
	if (wDataSize != sizeof(SUB_GR_Leave_Club)) return false;

	//������Ϣ
	SUB_GR_Leave_Club * pNetInfo = (SUB_GR_Leave_Club *) pData;
	//��������
	DBR_GR_Leave_Club kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Leave_Club));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_CLUB, dwSocketID, &kDBInfo, sizeof(DBR_GR_Leave_Club));
	return true;
}

// �߳����ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubKickClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Kick_Club));
	if (wDataSize != sizeof(SUB_GR_Kick_Club)) return false;

	//������Ϣ
	SUB_GR_Kick_Club * pNetInfo = (SUB_GR_Kick_Club *) pData;
	if (pNetInfo->dwRequestID == pNetInfo->dwUserID) // �Լ����Լ�
	{
		return false;
	}
	//��������
	DBR_GR_Kick_Club kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Kick_Club));

	//��������
	kDBInfo.dwRequestID = pNetInfo->dwRequestID;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_KICK_CLUB, dwSocketID, &kDBInfo, sizeof(DBR_GR_Kick_Club));
	return true;
}

// ������Ҽ�����ֲ�
bool CAttemperEngineSink::OnTCPNetworkSubInviteClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Invite_Club));
	if (wDataSize != sizeof(SUB_GR_Invite_Club)) return false;

	//������Ϣ
	SUB_GR_Invite_Club * pNetInfo = (SUB_GR_Invite_Club *) pData;
	//��������
	DBR_GR_Invite_Club kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Invite_Club));

	//��������
	kDBInfo.dwRequestID = pNetInfo->dwRequestID;
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_INVITE_CLUB, dwSocketID, &kDBInfo, sizeof(DBR_GR_Invite_Club));
	return true;
}

//���������������ֲ���Ӧ��Ϣ
bool  CAttemperEngineSink::OnDBPCCreateJoinRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GP_Create_Join_Res  *pGPCreateJoinRes = (DBO_GP_Create_Join_Res *) pData;
	SUB_GR_Create_Join_Res  GRCreateJoinRes;
	ZeroMemory(&GRCreateJoinRes,sizeof(SUB_GR_Create_Join_Res));
	memcpy(&GRCreateJoinRes, pGPCreateJoinRes, sizeof(SUB_GR_Create_Join_Res));
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_CREATE_JOIN_RES,&GRCreateJoinRes,sizeof(SUB_GR_Create_Join_Res));
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}
//��ȡ�Լ��ľ��ֲ�����
bool   CAttemperEngineSink::OnTCPNetworkSubGetMyClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Get_My_Club_Req));
	if (wDataSize != sizeof(SUB_GR_Get_My_Club_Req)) return false;

	//������Ϣ
	SUB_GR_Get_My_Club_Req * pNetInfo = (SUB_GR_Get_My_Club_Req *) pData;
	//��������
	DBR_GR_Get_My_Club_Req kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Get_My_Club_Req));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_GET_MY_CLUB_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Get_My_Club_Req));
	return true;
}
bool  CAttemperEngineSink::OnDBPGetMyClubRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DBO_GR_Get_My_Club_Res  *pClubMaterInfo = (DBO_GR_Get_My_Club_Res *)pData;
	SUB_GR_Get_My_Club_Res  pMyClubRes;
	ZeroMemory(&pMyClubRes,sizeof(SUB_GR_Get_My_Club_Res));
	memcpy(&pMyClubRes,pClubMaterInfo,sizeof(SUB_GR_Get_My_Club_Res));
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_GET_MY_CLUB_RES,&pMyClubRes,sizeof(SUB_GR_Get_My_Club_Res));
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	return true;
}

// �᳤���ֲ���Ա�б�����
bool CAttemperEngineSink::OnTCPNetworkSubMmmberListReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Club_Member_List_Req));
	if (wDataSize != sizeof(SUB_GR_Club_Member_List_Req)) return false;

	//������Ϣ
	SUB_GR_Club_Member_List_Req * pNetInfo = (SUB_GR_Club_Member_List_Req *) pData;
	//��������
	DBO_GR_Club_Member_List_Req kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBO_GR_Club_Member_List_Req));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.dwFlag = pNetInfo->dwFlag;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CLUB_MEMBER_LIST_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Kick_Club));
	return true;
}

// �᳤���ֲ���Ա�б��Ӧ
bool CAttemperEngineSink::OnDBPCClubMasterListRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID) < m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter + LOWORD(dwContextID))->dwSocketID != dwContextID) return true;
	SUB_GR_Club_Member_List_Res *pMsgRes = (SUB_GR_Club_Member_List_Res *) pData;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GR_CLUB_MEMBER_LIST_RES,pData,wDataSize);
	if (pMsgRes->bFlag == 1)
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}
	return true;
}

// �᳤���¾��ֲ���������
bool CAttemperEngineSink::OnTCPNetworkSubUpdateClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Update_Club_Req));	
	if (wDataSize!= sizeof(SUB_GR_Update_Club_Req)) return false; 

	//������Ϣ
	SUB_GR_Update_Club_Req * pNetInfo = (SUB_GR_Update_Club_Req *) pData;

	//��������
	DBR_GR_Update_Club_Req kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Update_Club_Req));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.bGameTypeIndex = pNetInfo->bGameTypeIndex;
	kDBInfo.bGameTypeQing = pNetInfo->bGameTypeQing;
	kDBInfo.bPlayCoutIdex = pNetInfo->bPlayCoutIdex;
	kDBInfo.bGamePayType = pNetInfo->bGamePayType;
	kDBInfo.bMaxScore = pNetInfo->bMaxScore;
	kDBInfo.bQiangGangHu = pNetInfo->bQiangGangHu;
	kDBInfo.bAllOther = pNetInfo->bAllOther;
	kDBInfo.bPantherType = pNetInfo->bPantherType;
	kDBInfo.bPlayerNum = pNetInfo->bPlayerNum;
	kDBInfo.bBaoIsOutCard = pNetInfo->bBaoIsOutCard;
	kDBInfo.bPiaoShu = pNetInfo->bPiaoShu;
	kDBInfo.bGameBaseScore = pNetInfo->bGameBaseScore;
	if(kDBInfo.bGameTypeIndex == 2 || kDBInfo.bGameTypeIndex == 4) kDBInfo.bQiangGangHu = 0;
	else if(kDBInfo.bGameTypeIndex == 2 || kDBInfo.bGameTypeIndex == 3) kDBInfo.bAllOther = 0;

	lstrcpyn(kDBInfo.stClubName, pNetInfo->stClubName, CountArray(kDBInfo.stClubName));
	kDBInfo.stClubName[CountArray(kDBInfo.stClubName) - 1] = 0;
	ConvertUtf8ToGBK(kDBInfo.stClubName, LEN_NICKNAME);
	lstrcpyn(kDBInfo.stClubNotice, pNetInfo->stClubNotice, CountArray(kDBInfo.stClubNotice));
	kDBInfo.stClubNotice[CountArray(kDBInfo.stClubNotice) - 1] = 0;
	ConvertUtf8ToGBK(kDBInfo.stClubNotice, 124);
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_UPDATE_CLUB_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Update_Club_Req));
	return true;
}

// ��ɢ���ֲ�����
bool CAttemperEngineSink::OnTCPNetworkSubDismissClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Dismiss_Club_Req));	
	if (wDataSize!= sizeof(SUB_GR_Dismiss_Club_Req)) return false; 

	//������Ϣ
	SUB_GR_Dismiss_Club_Req * pNetInfo = (SUB_GR_Dismiss_Club_Req *) pData;
	//��������
	DBR_GR_Dismiss_Club_Req kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Dismiss_Club_Req));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_DISMISS_CLUB_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Dismiss_Club_Req));
	return true;
}

// ת�û᳤����
bool CAttemperEngineSink::OnTCPNetworkSubChangeMasterReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Club_Change_Master_Req));	
	if (wDataSize!= sizeof(SUB_GR_Club_Change_Master_Req)) return false; 

	//������Ϣ
	SUB_GR_Club_Change_Master_Req * pNetInfo = (SUB_GR_Club_Change_Master_Req *) pData;
	//��������
	DBR_GR_Club_Change_Master_Req kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Club_Change_Master_Req));

	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.dwMasterID = pNetInfo->dwMasterID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CLUB_CHANGE_MASTER_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Club_Change_Master_Req));
	return true;
}

// �᳤���ù���Ա����
bool CAttemperEngineSink::OnTCPNetworkSubSetManagerReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(SUB_GR_Club_Set_Manager));	
	if (wDataSize!= sizeof(SUB_GR_Club_Set_Manager)) return false; 

	//������Ϣ
	SUB_GR_Club_Set_Manager * pNetInfo = (SUB_GR_Club_Set_Manager *) pData;
	//��������
	DBR_GR_Club_Set_Manager kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(DBR_GR_Club_Set_Manager));
	//��������
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.dwManagerID = pNetInfo->dwManagerID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CLUB_SET_MANAGER_REQ, dwSocketID, &kDBInfo, sizeof(DBR_GR_Club_Set_Manager));
	return true;
}

//��ȡ��������ַ����
bool CAttemperEngineSink::OnTCPNetworkServerAdressReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(CMD_MB_Server_Address_Req));	
	if (wDataSize!= sizeof(CMD_MB_Server_Address_Req)) return false; 

	//������Ϣ
	CMD_MB_Server_Address_Req * pNetInfo = (CMD_MB_Server_Address_Req *) pData;
	//��������
	CMD_CS_S_ServerAddrReq ServerAddrReq;
	ZeroMemory(&ServerAddrReq, sizeof(CMD_CS_S_ServerAddrReq));
	ServerAddrReq.dwSocketID  =  dwSocketID;
	ServerAddrReq.dwUserID  = pNetInfo->dwUserID;
	ServerAddrReq.dwClubID = pNetInfo->dwClubID;
	//��������
	m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_SERVERADDR,&ServerAddrReq,sizeof(ServerAddrReq));  
	return true;
}
//��ȡ��������ַ����
bool CAttemperEngineSink::OnTCPNetworkServerAdressRes(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize == sizeof(CMD_CS_S_ServerAddrRes));	
	if (wDataSize!= sizeof(CMD_CS_S_ServerAddrRes)) return false; 

	//������Ϣ
	CMD_CS_S_ServerAddrRes * pNetInfo = (CMD_CS_S_ServerAddrRes *) pData;
	//��������
	CMD_MB_Server_Address_Res ServerAddrRes;
	ZeroMemory(&ServerAddrRes, sizeof(CMD_MB_Server_Address_Res));
	ServerAddrRes.dwUserID  = pNetInfo->dwUserID;
	ServerAddrRes.dwClubID = pNetInfo->dwClubID;
	ServerAddrRes.dwServerID  = pNetInfo->iServerID;
	//��������
	m_pITCPNetworkEngine->SendData(pNetInfo->dwSocketID,MDM_GP_USER_SERVICE,SUB_GR_SERVER_ADDRESS_RES,&ServerAddrRes,sizeof(ServerAddrRes));  
	return true;
}
//////////////////////////////////////////////////////////////////////////////////

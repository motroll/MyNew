#include "StdAfx.h"
#include "ModuleDBParameter.h"
#include "ServerInfoManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerInfoBuffer::CServerInfoBuffer()
{
}

//��������
CServerInfoBuffer::~CServerInfoBuffer()
{
	//��������
	WORD wServerID=0;
	tagGameServerInfo * pGameServerInfo=NULL;
	POSITION Position=m_GameServerInfoMap.GetStartPosition();

	//ɾ������
	while (Position!=NULL)
	{
		m_GameServerInfoMap.GetNextAssoc(Position,wServerID,pGameServerInfo);
		SafeDelete(pGameServerInfo);
	}

	//ɾ������
	for (INT_PTR i=0;i<m_GameServerInfoArray.GetCount();i++)
	{
		pGameServerInfo=m_GameServerInfoArray[i];
		SafeDelete(pGameServerInfo);
	}

	//ɾ������
	m_GameServerInfoMap.RemoveAll();
	m_GameServerInfoArray.RemoveAll();

	return;
}

//��������
bool CServerInfoBuffer::ResetServerInfo()
{
	//��������
	WORD wServerID=0;
	tagGameServerInfo * pGameServerInfo=NULL;
	POSITION Position=m_GameServerInfoMap.GetStartPosition();

	//ɾ������
	while (Position!=NULL)
	{
		m_GameServerInfoMap.GetNextAssoc(Position,wServerID,pGameServerInfo);
		m_GameServerInfoArray.Add(pGameServerInfo);
	}

	//ɾ������
	m_GameServerInfoMap.RemoveAll();

	return true;
}

//ɾ������
bool CServerInfoBuffer::DeleteServerInfo(WORD wServerID)
{
	//��������
	tagGameServerInfo * pGameServerInfo=NULL;
	if (m_GameServerInfoMap.Lookup(wServerID,pGameServerInfo)==FALSE) return false;

	//ɾ������
	m_GameServerInfoMap.RemoveKey(wServerID);
	m_GameServerInfoArray.Add(pGameServerInfo);

	//���ñ���
	ZeroMemory(pGameServerInfo,sizeof(tagGameServerInfo));

	return true;
}

//��������
bool CServerInfoBuffer::InsertServerInfo(tagGameServerInfo * pGameServerInfo)
{
	//Ч�����
	ASSERT(pGameServerInfo!=NULL);
	if (pGameServerInfo==NULL) return false;

	//�����ִ�
	WORD wServerID=pGameServerInfo->wServerID;
	tagGameServerInfo * pGameServerInsert=SearchServerInfo(wServerID);

	//�����ж�
	if (pGameServerInsert==NULL)
	{
		//��������
		pGameServerInsert=CreateServerInfo();

		//����ж�
		if (pGameServerInsert==NULL)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//��������
	m_GameServerInfoMap[wServerID]=pGameServerInsert;
	CopyMemory(pGameServerInsert,pGameServerInfo,sizeof(tagGameServerInfo));

	return true;
}

//��ȡ��Ŀ
DWORD CServerInfoBuffer::GetServerInfoCount()
{
	return (DWORD)(m_GameServerInfoMap.GetCount());
}

//��������
tagGameServerInfo * CServerInfoBuffer::SearchServerInfo(WORD wServerID)
{
	tagGameServerInfo * pGameServerInfo=NULL;
	m_GameServerInfoMap.Lookup(wServerID,pGameServerInfo);
	return pGameServerInfo;
}

//��������
tagGameServerInfo * CServerInfoBuffer::CreateServerInfo()
{
	//��������
	tagGameServerInfo * pGameServerInfo=NULL;

	//��������
	try
	{
		INT_PTR nArrayCount=m_GameServerInfoArray.GetCount();
		if (nArrayCount>0)
		{
			pGameServerInfo=m_GameServerInfoArray[nArrayCount-1];
			m_GameServerInfoArray.RemoveAt(nArrayCount-1);
		}
		else
		{
			pGameServerInfo=new tagGameServerInfo;
			if (pGameServerInfo==NULL) return NULL;
		}
	}
	catch (...) { return false; }

	//���ñ���
	ZeroMemory(pGameServerInfo,sizeof(tagGameServerInfo));

	return pGameServerInfo;
}


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CMatchOptionBuffer::CMatchOptionBuffer()
{
}

//��������
CMatchOptionBuffer::~CMatchOptionBuffer()
{
	//��������
	tagGameMatchOption * pGameMatchOption=NULL;

	//ɾ������
	m_GameMatchOptionBuffer.Append(m_GameMatchOptionArray);
	for (INT_PTR i=0;i<m_GameMatchOptionBuffer.GetCount();i++)
	{
		pGameMatchOption=m_GameMatchOptionBuffer[i];
		SafeDelete(pGameMatchOption);
	}

	//ɾ������
	m_GameMatchOptionArray.RemoveAll();
	m_GameMatchOptionBuffer.RemoveAll();

	return;
}

//��������
bool CMatchOptionBuffer::ResetMatchOption()
{
	//��Ӷ���
	m_GameMatchOptionBuffer.Append(m_GameMatchOptionArray);

	//ɾ��Ԫ��
	m_GameMatchOptionArray.RemoveAll();

	return true;
}

//ɾ������
bool CMatchOptionBuffer::DeleteMatchOption(DWORD dwMatchID,DWORD wMatchNO)
{
	//��������
	tagGameMatchOption * pGameMatchOption=NULL;

	//���Ҷ���
	for(INT_PTR i=0;i<m_GameMatchOptionArray.GetCount();i++)
	{
		pGameMatchOption=m_GameMatchOptionArray[i];
		if(pGameMatchOption->dwMatchID==dwMatchID && pGameMatchOption->dwMatchNO==wMatchNO)
		{
			//ɾ������
			m_GameMatchOptionArray.RemoveAt(i);
			m_GameMatchOptionBuffer.Add(pGameMatchOption);

			//���ñ���
			ZeroMemory(pGameMatchOption,sizeof(tagGameMatchOption));

			break;
		}
	}	

	return true;
}

//��������
bool CMatchOptionBuffer::InsertMatchOption(tagGameMatchOption * pGameMatchOption)
{
	//Ч�����
	ASSERT(pGameMatchOption!=NULL);
	if (pGameMatchOption==NULL) return false;

	//�����ִ�
	tagGameMatchOption * pGameMatchInsert=SearchMatchOption(pGameMatchOption->dwMatchID,pGameMatchOption->dwMatchNO);

	//�����ж�
	if (pGameMatchInsert==NULL)
	{
		//��������
		pGameMatchInsert=CreateMatchOption();

		//����ж�
		if (pGameMatchInsert==NULL)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//��������
	m_GameMatchOptionArray.Add(pGameMatchInsert);
	CopyMemory(pGameMatchInsert,pGameMatchOption,sizeof(tagGameMatchOption));

	return true;
}

//��ȡ��Ŀ
DWORD CMatchOptionBuffer::GetMatchOptionCount()
{
	return (DWORD)(m_GameMatchOptionArray.GetCount());
}

//��������
tagGameMatchOption * CMatchOptionBuffer::SearchMatchOption(DWORD dwMatchID,DWORD wMatchNO)
{
	//��������
	tagGameMatchOption * pGameMatchOption=NULL;

	//���Ҷ���
	for(INT_PTR i=0;i<m_GameMatchOptionArray.GetCount();i++)
	{
		pGameMatchOption=m_GameMatchOptionArray[i];
		if(pGameMatchOption->dwMatchID==dwMatchID && pGameMatchOption->dwMatchNO==wMatchNO)
		{
			return pGameMatchOption;
		}
	}

	return NULL;
}

//��������
tagGameMatchOption * CMatchOptionBuffer::CreateMatchOption()
{
	//��������
	tagGameMatchOption * pGameMatchOption=NULL;

	//��������
	try
	{
		INT_PTR nArrayCount=m_GameMatchOptionBuffer.GetCount();
		if (nArrayCount>0)
		{
			pGameMatchOption=m_GameMatchOptionBuffer[nArrayCount-1];
			m_GameMatchOptionBuffer.RemoveAt(nArrayCount-1);
		}
		else
		{
			pGameMatchOption=new tagGameMatchOption;
			if (pGameMatchOption==NULL) return NULL;
		}
	}
	catch (...) { return false; }

	//���ñ���
	ZeroMemory(pGameMatchOption,sizeof(tagGameMatchOption));

	return pGameMatchOption;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerInfoManager::CServerInfoManager()
{
}

//��������
CServerInfoManager::~CServerInfoManager()
{
}

//���ط���
bool CServerInfoManager::LoadGameServerInfo(TCHAR szMachineID[LEN_MACHINE_ID], CServerInfoBuffer & ServerInfoBuffer)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wServerID"),0);
		PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�б�
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadGameRoomItem"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//����б�
		ServerInfoBuffer.ResetServerInfo();

		//��ȡ�б�
		while (PlatformDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ����
			tagGameServerInfo GameServerResult;
			ReadGameServerInfo(PlatformDBAide,GameServerResult);

			//�����б�
			ServerInfoBuffer.InsertServerInfo(&GameServerResult);

			//�ƶ���¼
			PlatformDBModule->MoveToNext();
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return false;
	}

	return false;
}

//���ط���
bool CServerInfoManager::LoadGameServerInfo(TCHAR szMachineID[LEN_MACHINE_ID], WORD wServerID, tagGameServerInfo & GameServerResult)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//��ȡ�б�
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wServerID"),wServerID);
		//PlatformDBAide.AddParameter(TEXT("@strServiceMachine"),szMachineID);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�б�
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadGameRoomItem"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//��ȡ����
		if (PlatformDBModule->IsRecordsetEnd()==false)
		{
			ReadGameServerInfo(PlatformDBAide,GameServerResult);
			return true;
		}

		return false;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return false;
	}

	return false;
}

//ɾ������
bool CServerInfoManager::DeleteGameServer(WORD wServerID)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wServerID"),wServerID);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ������
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_DeleteGameRoom"),false)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);
	}

	return false;
}

//���뷿��
bool CServerInfoManager::InsertGameServer(tagGameServerCreate * pGameServerCreate, tagGameServerInfo & GameServerResult)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//��չ����
		TCHAR szCustomRule[CountArray(pGameServerCreate->cbCustomRule)*2+1]=TEXT("");
		for (INT i=0;i<CountArray(pGameServerCreate->cbCustomRule);i++) _stprintf(&szCustomRule[i*2],TEXT("%02X"),pGameServerCreate->cbCustomRule[i]);

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wGameID"),pGameServerCreate->wGameID);
		PlatformDBAide.AddParameter(TEXT("@wKindID"),pGameServerCreate->wKindID);
		PlatformDBAide.AddParameter(TEXT("@wNodeID"),pGameServerCreate->wNodeID);
		PlatformDBAide.AddParameter(TEXT("@wSortID"),pGameServerCreate->wSortID);
		PlatformDBAide.AddParameter(TEXT("@lCellScore"),pGameServerCreate->lCellScore);
		PlatformDBAide.AddParameter(TEXT("@wRevenueRatio"),pGameServerCreate->wRevenueRatio);
		PlatformDBAide.AddParameter(TEXT("@lServiceScore"),pGameServerCreate->lServiceScore);
		PlatformDBAide.AddParameter(TEXT("@lRestrictScore"),pGameServerCreate->lRestrictScore);
		PlatformDBAide.AddParameter(TEXT("@lMinTableScore"),pGameServerCreate->lMinTableScore);
		PlatformDBAide.AddParameter(TEXT("@lMinEnterScore"),pGameServerCreate->lMinEnterScore);
		PlatformDBAide.AddParameter(TEXT("@lMaxEnterScore"),pGameServerCreate->lMaxEnterScore);
		PlatformDBAide.AddParameter(TEXT("@cbMinEnterMember"),pGameServerCreate->cbMinEnterMember);
		PlatformDBAide.AddParameter(TEXT("@cbMaxEnterMember"),pGameServerCreate->cbMaxEnterMember);
		PlatformDBAide.AddParameter(TEXT("@dwServerRule"),pGameServerCreate->dwServerRule);
		PlatformDBAide.AddParameter(TEXT("@dwAttachUserRight"),pGameServerCreate->dwAttachUserRight);
		PlatformDBAide.AddParameter(TEXT("@wMaxPlayer"),pGameServerCreate->wMaxPlayer);
		PlatformDBAide.AddParameter(TEXT("@wTableCount"),pGameServerCreate->wTableCount);
		PlatformDBAide.AddParameter(TEXT("@wServerPort"),pGameServerCreate->wServerPort);
		PlatformDBAide.AddParameter(TEXT("@wServerType"),pGameServerCreate->wServerType);
		PlatformDBAide.AddParameter(TEXT("@strServerName"),pGameServerCreate->szServerName);
		PlatformDBAide.AddParameter(TEXT("@cbDistributeRule"),pGameServerCreate->cbDistributeRule);
		PlatformDBAide.AddParameter(TEXT("@wMinDistributeUser"),pGameServerCreate->wMinDistributeUser);
		PlatformDBAide.AddParameter(TEXT("@wMaxDistributeUser"),pGameServerCreate->wMaxDistributeUser);
		PlatformDBAide.AddParameter(TEXT("@wDistributeTimeSpace"),pGameServerCreate->wDistributeTimeSpace);
		PlatformDBAide.AddParameter(TEXT("@wDistributeDrawCount"),pGameServerCreate->wDistributeDrawCount);
		PlatformDBAide.AddParameter(TEXT("@wDistributeStartDelay"),pGameServerCreate->wDistributeStartDelay);
		PlatformDBAide.AddParameter(TEXT("@strDataBaseName"),pGameServerCreate->szDataBaseName);
		PlatformDBAide.AddParameter(TEXT("@strDataBaseAddr"),pGameServerCreate->szDataBaseAddr);
		PlatformDBAide.AddParameter(TEXT("@strCustomRule"),szCustomRule);
		PlatformDBAide.AddParameter(TEXT("@strServiceMachine"),pGameServerCreate->szServiceMachine);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ������
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_InsertGameRoom"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//��ȡ����
		ReadGameServerInfo(PlatformDBAide,GameServerResult);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);
	}

	return false;
}

//�޸ķ���
bool CServerInfoManager::ModifyGameServer(tagGameServerCreate * pGameServerCreate, tagGameServerInfo & GameServerResult)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//��չ����
		TCHAR szCustomRule[CountArray(pGameServerCreate->cbCustomRule)*2+1]=TEXT("");
		for (INT i=0;i<CountArray(pGameServerCreate->cbCustomRule);i++) _stprintf(&szCustomRule[i*2],TEXT("%02X"),pGameServerCreate->cbCustomRule[i]);

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wServerID"),pGameServerCreate->wServerID);
		PlatformDBAide.AddParameter(TEXT("@wKindID"),pGameServerCreate->wKindID);
		PlatformDBAide.AddParameter(TEXT("@wNodeID"),pGameServerCreate->wNodeID);
		PlatformDBAide.AddParameter(TEXT("@wSortID"),pGameServerCreate->wSortID);
		PlatformDBAide.AddParameter(TEXT("@lCellScore"),pGameServerCreate->lCellScore);
		PlatformDBAide.AddParameter(TEXT("@wRevenueRatio"),pGameServerCreate->wRevenueRatio);
		PlatformDBAide.AddParameter(TEXT("@lServiceScore"),pGameServerCreate->lServiceScore);
		PlatformDBAide.AddParameter(TEXT("@lRestrictScore"),pGameServerCreate->lRestrictScore);
		PlatformDBAide.AddParameter(TEXT("@lMinTableScore"),pGameServerCreate->lMinTableScore);
		PlatformDBAide.AddParameter(TEXT("@lMinEnterScore"),pGameServerCreate->lMinEnterScore);
		PlatformDBAide.AddParameter(TEXT("@lMaxEnterScore"),pGameServerCreate->lMaxEnterScore);
		PlatformDBAide.AddParameter(TEXT("@cbMinEnterMember"),pGameServerCreate->cbMinEnterMember);
		PlatformDBAide.AddParameter(TEXT("@cbMaxEnterMember"),pGameServerCreate->cbMaxEnterMember);
		PlatformDBAide.AddParameter(TEXT("@dwServerRule"),pGameServerCreate->dwServerRule);
		PlatformDBAide.AddParameter(TEXT("@dwAttachUserRight"),pGameServerCreate->dwAttachUserRight);
		PlatformDBAide.AddParameter(TEXT("@wMaxPlayer"),pGameServerCreate->wMaxPlayer);
		PlatformDBAide.AddParameter(TEXT("@wTableCount"),pGameServerCreate->wTableCount);
		PlatformDBAide.AddParameter(TEXT("@wServerPort"),pGameServerCreate->wServerPort);
		PlatformDBAide.AddParameter(TEXT("@wServerType"),pGameServerCreate->wServerType);
		PlatformDBAide.AddParameter(TEXT("@strServerName"),pGameServerCreate->szServerName);
		PlatformDBAide.AddParameter(TEXT("@cbDistributeRule"),pGameServerCreate->cbDistributeRule);
		PlatformDBAide.AddParameter(TEXT("@wMinDistributeUser"),pGameServerCreate->wMinDistributeUser);
		PlatformDBAide.AddParameter(TEXT("@wMaxDistributeUser"),pGameServerCreate->wMaxDistributeUser);
		PlatformDBAide.AddParameter(TEXT("@wDistributeTimeSpace"),pGameServerCreate->wDistributeTimeSpace);
		PlatformDBAide.AddParameter(TEXT("@wDistributeDrawCount"),pGameServerCreate->wDistributeDrawCount);
		PlatformDBAide.AddParameter(TEXT("@wDistributeStartDelay"),pGameServerCreate->wDistributeStartDelay);
		PlatformDBAide.AddParameter(TEXT("@strDataBaseName"),pGameServerCreate->szDataBaseName);
		PlatformDBAide.AddParameter(TEXT("@strDataBaseAddr"),pGameServerCreate->szDataBaseAddr);
		PlatformDBAide.AddParameter(TEXT("@strCustomRule"),szCustomRule);
		PlatformDBAide.AddParameter(TEXT("@strServiceMachine"),pGameServerCreate->szServiceMachine);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ������
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_ModifyGameRoom"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//��ȡ����
		ReadGameServerInfo(PlatformDBAide,GameServerResult);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return false;
	}

	return false;
}

//��������
bool CServerInfoManager::ConnectPlatformDB(CDataBaseHelper & PlatformDBModule)
{
	//��ȡ����
	CModuleDBParameter * pModuleDBParameter=CModuleDBParameter::GetModuleDBParameter();
	tagDataBaseParameter * pDataBaseParameter=pModuleDBParameter->GetPlatformDBParameter();

	//��������
	if ((PlatformDBModule.GetInterface()==NULL)&&(PlatformDBModule.CreateInstance()==false))
	{
		AfxMessageBox(TEXT("CreateGameServer ���� PlatformDBModule ����ʧ��"),MB_ICONERROR);
		return false;
	}

	//��������
	PlatformDBModule->SetConnectionInfo(pDataBaseParameter->szDataBaseAddr,pDataBaseParameter->wDataBasePort,
		pDataBaseParameter->szDataBaseName,pDataBaseParameter->szDataBaseUser,pDataBaseParameter->szDataBasePass);

	//��������
	PlatformDBModule->OpenConnection();

	return true;
}

//��ȡ����
bool CServerInfoManager::ReadGameServerInfo(CDataBaseAide & PlatformDBAide, tagGameServerInfo & GameServerResult)
{
	//���ñ���
	ZeroMemory(&GameServerResult,sizeof(GameServerResult));

	//��������
	GameServerResult.wGameID=PlatformDBAide.GetValue_WORD(TEXT("GameID"));
	GameServerResult.wServerID=PlatformDBAide.GetValue_WORD(TEXT("ServerID"));

	//�ҽ�����
	GameServerResult.wKindID=PlatformDBAide.GetValue_WORD(TEXT("KindID"));
	GameServerResult.wNodeID=PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
	GameServerResult.wSortID=PlatformDBAide.GetValue_WORD(TEXT("SortID"));

	//�汾��Ϣ
	GameServerResult.dwClientVersion=PlatformDBAide.GetValue_DWORD(TEXT("ClientVersion"));
	GameServerResult.dwServerVersion=PlatformDBAide.GetValue_DWORD(TEXT("ServerVersion"));

	//ģ������
	PlatformDBAide.GetValue_String(TEXT("GameName"),GameServerResult.szGameName,CountArray(GameServerResult.szGameName));
	PlatformDBAide.GetValue_String(TEXT("ServerDLLName"),GameServerResult.szServerDLLName,CountArray(GameServerResult.szServerDLLName));
	PlatformDBAide.GetValue_String(TEXT("ClientEXEName"),GameServerResult.szClientEXEName,CountArray(GameServerResult.szClientEXEName));

	//˰������
	GameServerResult.lCellScore=PlatformDBAide.GetValue_LONG(TEXT("CellScore"));
	GameServerResult.wRevenueRatio=PlatformDBAide.GetValue_WORD(TEXT("RevenueRatio"));
	GameServerResult.lServiceScore=PlatformDBAide.GetValue_LONGLONG(TEXT("ServiceScore"));

	//��������
	GameServerResult.lRestrictScore=PlatformDBAide.GetValue_LONGLONG(TEXT("RestrictScore"));
	GameServerResult.lMinTableScore=PlatformDBAide.GetValue_LONGLONG(TEXT("MinTableScore"));
	GameServerResult.lMinEnterScore=PlatformDBAide.GetValue_LONGLONG(TEXT("MinEnterScore"));
	GameServerResult.lMaxEnterScore=PlatformDBAide.GetValue_LONGLONG(TEXT("MaxEnterScore"));

	//��Ա����
	GameServerResult.cbMinEnterMember=PlatformDBAide.GetValue_BYTE(TEXT("MinEnterMember"));
	GameServerResult.cbMaxEnterMember=PlatformDBAide.GetValue_BYTE(TEXT("MaxEnterMember"));

	//��������
	GameServerResult.dwServerRule=PlatformDBAide.GetValue_DWORD(TEXT("ServerRule"));
	GameServerResult.dwAttachUserRight=PlatformDBAide.GetValue_DWORD(TEXT("AttachUserRight"));

	//��������
	GameServerResult.wMaxPlayer=PlatformDBAide.GetValue_WORD(TEXT("MaxPlayer"));
	GameServerResult.wTableCount=PlatformDBAide.GetValue_WORD(TEXT("TableCount"));
	GameServerResult.wServerPort=PlatformDBAide.GetValue_WORD(TEXT("ServerPort"));
	GameServerResult.wServerType=PlatformDBAide.GetValue_WORD(TEXT("ServerType"));
	PlatformDBAide.GetValue_String(TEXT("ServerName"),GameServerResult.szServerName,CountArray(GameServerResult.szServerName));

	//��������
	GameServerResult.cbDistributeRule=PlatformDBAide.GetValue_BYTE(TEXT("DistributeRule"));
	GameServerResult.wMinDistributeUser=PlatformDBAide.GetValue_WORD(TEXT("MinDistributeUser"));
	GameServerResult.wMaxDistributeUser=PlatformDBAide.GetValue_WORD(TEXT("MaxDistributeUser"));
	GameServerResult.wDistributeTimeSpace=PlatformDBAide.GetValue_WORD(TEXT("DistributeTimeSpace"));
	GameServerResult.wDistributeDrawCount=PlatformDBAide.GetValue_WORD(TEXT("DistributeDrawCount"));
	GameServerResult.wDistributeStartDelay=PlatformDBAide.GetValue_WORD(TEXT("DistributeStartDelay"));

	//��������
	PlatformDBAide.GetValue_String(TEXT("DataBaseName"),GameServerResult.szDataBaseName,CountArray(GameServerResult.szDataBaseName));
	PlatformDBAide.GetValue_String(TEXT("DataBaseAddr"),GameServerResult.szDataBaseAddr,CountArray(GameServerResult.szDataBaseAddr));

	//��չ����
	TCHAR szCustomRule[CountArray(GameServerResult.cbCustomRule)*2+1];
	PlatformDBAide.GetValue_String(TEXT("CustomRule"),szCustomRule,CountArray(szCustomRule));
	PlatformDBAide.GetValue_String(TEXT("ServiceMachine"),GameServerResult.szServiceMachine,CountArray(GameServerResult.szServiceMachine));

	//��չ����
	if (szCustomRule[0]!=0)
	{
		//��ȡ����
		ASSERT(lstrlen(szCustomRule)%2==0);
		INT nCustomRuleSize=lstrlen(szCustomRule)/2;

		//ת���ַ�
		for (INT i=0;i<nCustomRuleSize;i++)
		{
			//��ȡ�ַ�
			TCHAR cbChar1=szCustomRule[i*2];
			TCHAR cbChar2=szCustomRule[i*2+1];

			//Ч���ַ�
			ASSERT((cbChar1>=TEXT('0'))&&(cbChar1<=TEXT('9'))||(cbChar1>=TEXT('A'))&&(cbChar1<=TEXT('F')));
			ASSERT((cbChar2>=TEXT('0'))&&(cbChar2<=TEXT('9'))||(cbChar2>=TEXT('A'))&&(cbChar2<=TEXT('F')));

			//���ɽ��
			if ((cbChar2>=TEXT('0'))&&(cbChar2<=TEXT('9'))) GameServerResult.cbCustomRule[i]+=(cbChar2-TEXT('0'));
			if ((cbChar2>=TEXT('A'))&&(cbChar2<=TEXT('F'))) GameServerResult.cbCustomRule[i]+=(cbChar2-TEXT('A')+0x0A);

			//���ɽ��
			if ((cbChar1>=TEXT('0'))&&(cbChar1<=TEXT('9'))) GameServerResult.cbCustomRule[i]+=(cbChar1-TEXT('0'))*0x10;
			if ((cbChar1>=TEXT('A'))&&(cbChar1<=TEXT('F'))) GameServerResult.cbCustomRule[i]+=(cbChar1-TEXT('A')+0x0A)*0x10;
		}
	}

	//���ذ汾
	LPCTSTR pszServerDLLName=GameServerResult.szServerDLLName;
	CWHService::GetModuleVersion(pszServerDLLName,GameServerResult.dwNativeVersion);

	return true;
}

//��ȡ����
bool CServerInfoManager::ReadGameMatchOption(CDataBaseAide & PlatformDBAide, tagGameMatchOption & GameMatchResult)
{
	//���ñ���
	ZeroMemory(&GameMatchResult,sizeof(GameMatchResult));

	//��������
	GameMatchResult.dwMatchID=PlatformDBAide.GetValue_INT(TEXT("MatchID"));
	GameMatchResult.dwMatchNO=PlatformDBAide.GetValue_INT(TEXT("MatchNo"));

	//������Ϣ
	GameMatchResult.cbMatchFeeType= PlatformDBAide.GetValue_INT(TEXT("MatchFeeType"));
	GameMatchResult.lMatchFee= PlatformDBAide.GetValue_LONGLONG(TEXT("MatchFee"));

	GameMatchResult.lMatchEnterScore= PlatformDBAide.GetValue_LONGLONG(TEXT("MatchEnterScore"));
	
	//������Ϣ
	GameMatchResult.cbMemberOrder = PlatformDBAide.GetValue_INT(TEXT("MemberOrder"));
	GameMatchResult.cbMatchType = PlatformDBAide.GetValue_INT(TEXT("MatchType"));
	PlatformDBAide.GetValue_String(TEXT("MatchName"),GameMatchResult.szMatchName,CountArray(GameMatchResult.szMatchName));

	//��ʱ����
	if(GameMatchResult.cbMatchType==MATCH_TYPE_LOCKTIME)
	{
		//��������
		GameMatchResult.MatchType.LockTime.wMatchPlayCount= PlatformDBAide.GetValue_LONGLONG(TEXT("MinPlayCount"));

		//ʱ������
		PlatformDBAide.GetValue_SystemTime(TEXT("StartTime"),GameMatchResult.MatchType.LockTime.MatchStartTime);
		PlatformDBAide.GetValue_SystemTime(TEXT("EndTime"),GameMatchResult.MatchType.LockTime.MatchEndTime);

		//��������
		GameMatchResult.MatchType.LockTime.lMatchInitScore = PlatformDBAide.GetValue_LONGLONG(TEXT("InitScore"));
		GameMatchResult.MatchType.LockTime.lMatchCullScore = PlatformDBAide.GetValue_LONGLONG(TEXT("CullScore"));
	}

	//��ʱ����
	if(GameMatchResult.cbMatchType==MATCH_TYPE_IMMEDIATE)
	{
		//������Ϣ
		GameMatchResult.MatchType.Immediate.wStartUserCount = PlatformDBAide.GetValue_WORD(TEXT("StartUserCount"));
		GameMatchResult.MatchType.Immediate.wAndroidUserCount = PlatformDBAide.GetValue_WORD(TEXT("AndroidUserCount"));

		//������Ϣ
		GameMatchResult.MatchType.Immediate.dwInitalBase = PlatformDBAide.GetValue_DWORD(TEXT("InitialBase"));
		GameMatchResult.MatchType.Immediate.dwInitalScore = PlatformDBAide.GetValue_DWORD(TEXT("InitialScore"));
		GameMatchResult.MatchType.Immediate.wMinEnterGold = PlatformDBAide.GetValue_WORD(TEXT("MinEnterGold"));

		//������Ϣ
		GameMatchResult.MatchType.Immediate.cbPlayCount = PlatformDBAide.GetValue_BYTE(TEXT("PlayCount"));
		GameMatchResult.MatchType.Immediate.cbSwitchTableCount = PlatformDBAide.GetValue_BYTE(TEXT("SwitchTableCount"));
		GameMatchResult.MatchType.Immediate.wPrecedeTimer = PlatformDBAide.GetValue_WORD(TEXT("PrecedeTimer"));
	}

	return true;
}


//���ؽ���
WORD CServerInfoManager::LoadGameMatchReward(DWORD dwMatchID,DWORD dwMatchNO)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@dwMatchID"),dwMatchID);
		PlatformDBAide.AddParameter(TEXT("@dwMatchNo"),dwMatchNO);

		//��ȡ�б�
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadMatchReward"),true)==DB_SUCCESS)
		{
			return PlatformDBModule->GetRecordCount();
		}		

		return 0;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return 0;
	}

	return 0;
}

//���ر���
bool CServerInfoManager::LoadGameMatchOption(WORD wKindID,CMatchOptionBuffer & MatchOptionBuffer)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//�������
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wKindID"),wKindID);
		PlatformDBAide.AddParameter(TEXT("@dwMatchID"),0);
		PlatformDBAide.AddParameter(TEXT("@dwMatchNo"),0);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�б�
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadGameMatchItem"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//����б�
		MatchOptionBuffer.ResetMatchOption();
		//��ȡ�б�
		while (PlatformDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ����
			tagGameMatchOption GameMatchResult;
			ReadGameMatchOption(PlatformDBAide,GameMatchResult);

			//�����б�
			MatchOptionBuffer.InsertMatchOption(&GameMatchResult);

			//�ƶ���¼
			PlatformDBModule->MoveToNext();
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return false;
	}

	return false;
}

//���ط���
bool CServerInfoManager::LoadGameMatchOption(WORD wKindID,DWORD dwMatchID,DWORD dwMatchNO,tagGameMatchOption & GameMatchResult)
{
	//��������
	CDataBaseAide PlatformDBAide;
	CDataBaseHelper PlatformDBModule;

	//ִ���߼�
	try
	{
		//�������ݿ�
		if (ConnectPlatformDB(PlatformDBModule)==false)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ö���
		PlatformDBAide.SetDataBase(PlatformDBModule.GetInterface());

		//��ȡ�б�
		PlatformDBAide.ResetParameter();
		PlatformDBAide.AddParameter(TEXT("@wKindID"),wKindID);
		PlatformDBAide.AddParameter(TEXT("@dwMatchID"),dwMatchID);
		PlatformDBAide.AddParameter(TEXT("@dwMatchNo"),dwMatchNO);

		//�����Ϣ
		TCHAR szDescribeString[128]=TEXT("");
		PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�б�
		if (PlatformDBAide.ExecuteProcess(TEXT("GSP_GS_LoadGameMatchItem"),true)!=DB_SUCCESS)
		{
			//��ȡ��Ϣ
			PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),szDescribeString,CountArray(szDescribeString));

			//������ʾ
			CTraceService::TraceString(szDescribeString,TraceLevel_Exception);

			//������ʾ
			AfxMessageBox(szDescribeString,MB_ICONERROR);

			return false;
		}

		//��ȡ����
		if (PlatformDBModule->IsRecordsetEnd()==false)
		{
			ReadGameMatchOption(PlatformDBAide,GameMatchResult);
			return true;
		}

		return false;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������ʾ
		AfxMessageBox(pszDescribe,MB_ICONERROR);

		return false;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

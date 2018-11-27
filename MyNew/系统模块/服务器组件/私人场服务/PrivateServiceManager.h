#ifndef GAME_PRIVATE_SERVICE_MANAGER_HEAD_FILE
#define GAME_PRIVATE_SERVICE_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "PrivateServiceHead.h"

//////////////////////////////////////////////////////////////////////////

//�������������
class PRIVATE_SERVICE_CLASS CPrivateServiceManager : public IPrivateServiceManager
{
	//״̬����
protected:
	bool								m_bIsService;					//�����ʶ	
	
	//�ӿڱ���
protected:	
	IGamePrivateItem *					m_pIGamePrivatetem;				//��������

	//����ӿ�
protected:
	IMainServiceFrame *					m_pIGameServiceFrame;			//���ܽӿ�

	//��������
public:
	//���캯��
	CPrivateServiceManager(void);
	//��������
	virtual ~CPrivateServiceManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);
	
	//���ƽӿ�
public:
	//ֹͣ����
	virtual bool StopService();
	//��������
	virtual bool StartService();
	
	//����ӿ�
public:
	//��������
	virtual bool CreatePrivateMatch();
	//������
	virtual bool BindTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	//��ʼ���ӿ�
	virtual bool InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter);	

	//ϵͳ�¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//˽�˷��¼�
	virtual bool OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	

	//�û��¼�
public:
	//�û���¼
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//�û��ǳ�
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem);
	//����
	virtual bool OnEventEnterPrivate(DWORD dwSocketID,VOID* pData,DWORD dwUserIP, bool bIsMobile);
	//�û��μ�
	virtual bool OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID);
	//�û��˳�
	virtual bool OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);		

	//�û�����
public:
	//�û��뿪
	virtual bool OnEventReqStandUP(IServerUserItem * pIServerUserItem);
	
	//�ӿ���Ϣ
public:
	//�û��ӿ�
	virtual IUnknownEx * GetServerUserItemSink();
};

//////////////////////////////////////////////////////////////////////////
#endif
#ifndef GAME_PRIVATE_SINK_HEAD_FILE
#define GAME_PRIVATE_SINK_HEAD_FILE

#pragma once

//�����ļ�
#include "PrivateServiceHead.h"

//////////////////////////////////////////////////////////////////////////

//���ӹ�����
class CTableFramePrivate : public ITableFramePrivate,public ITableUserAction
{
	//��Ԫ����
	friend class CGameServiceManager;
	
	//�ӿڱ���
public:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	
	//�¼��ӿ�
protected:
	IPrivateEventSink				* m_pPrivateEventSink;				//˽�˷��¼�

	//���ñ���
protected:
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	
	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����

	//��������
public:
	//���캯��
	CTableFramePrivate();
	//��������
	virtual ~CTableFramePrivate();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//���������¼��ӿ�
	virtual bool SetPrivateEventSink(IUnknownEx * pIUnknownEx);
	//��ʼ��
	virtual bool InitTableFramePrivate(IUnknownEx * pIUnknownEx);		

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	//��Ϸ����
	virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	
	//�û�����
public:	
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);		 
	//�û���ȥ��Ϸ
	virtual bool OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem);

public:	
	//�û�����
	virtual bool AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex);
	//д��
	virtual bool WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount, datastream& kData);	

};

//////////////////////////////////////////////////////////////////////////

#endif
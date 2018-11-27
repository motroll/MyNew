#ifndef PRIVATE_HEAD_FILE
#define PRIVATE_HEAD_FILE

#pragma once

//�����ļ�
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"
#include "PrivateTableInfo.h"
#include "ClubGame.h"

///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

#define  MAX_CLUB_GAME_NUM                         200      //��Ϸ�����ֲ�
#define  CLUB_DISS_TIME						       5000		//��ɢ���ֲ�ʱ��
struct DBR_GR_Create_Private;
struct DBR_GR_Join_Club_Rom;

//��ʱ��
class PriaveteGame 
	:public IGamePrivateItem
	,public IPrivateEventSink
	,public IServerUserItemSink
{
	//��������
protected:
	tagGameServiceOption *				m_pGameServiceOption;			//��������
	tagGameServiceAttrib *				m_pGameServiceAttrib;			//��������

	CMD_GR_Private_Info					m_kPrivateInfo;
	//�ں˽ӿ�
protected:
	PrivateTableInfo*					m_vecTableInfo;				//
	ClubGameInfo*                       m_pClubGame;                    //���ֲ���Ϣ
	ITimerEngine *						m_pITimerEngine;				//ʱ������
	IDBCorrespondManager *				m_pIDataBaseEngine;				//��������	
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//��������

	//����ӿ�
protected:
	IMainServiceFrame *					m_pIGameServiceFrame;			//���ܽӿ�
	IServerUserManager *				m_pIServerUserManager;			//�û�����
	IAndroidUserManager	*				m_pAndroidUserManager;			//��������
	IServerUserItemSink *				m_pIServerUserItemSink;			//�û��ص�

	//��������
public:
	//���캯��
	PriaveteGame();
	//��������
	virtual ~PriaveteGame(void);

	bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	bool SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, bool needReady=true);

	void CreatePrivateCost(PrivateTableInfo* pTableInfo);

	int joinPrivateRoom(IServerUserItem * pIServerUserItem,ITableFrame * pICurrTableFrame,DWORD dwSeatNum = -1);
	bool selectRoomChair(IServerUserItem* pIServerUserItem, ITableFrame* pICurrTableFrame, BYTE idex);

	bool OnEventCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	bool OnEnventJoinClubRom(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	//ͨ��˽�˳���ʽ������ֲ������ж�
	bool OnEnventClubRomPriavteRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	void sendPrivateRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo);

	PrivateTableInfo* getTableInfoByRoomID(DWORD dwRoomID);

	ClubGameInfo* getClubInfoByClubID(DWORD dwClubID);
	ClubGameInfo* getClubInfoByRoomID(DWORD dwRoomID,DWORD dwClubID);

	ClubGameInfo * getClubInfoIsNullClub();

	void   SetDismissRomInClubRom(DWORD dwClubID);

	int PriaveteGame::getRoomCoutByCreaterID(DWORD dwUserID);

	std::vector<PrivateTableInfo*> getTableInfoByUserID(DWORD dwUserD);

	PrivateTableInfo* getTableInfoByTableID(DWORD dwRoomID);

	PrivateTableInfo* getTableInfoByTableFrame(ITableFrame* pTableFrame);

	void DismissRoom(PrivateTableInfo* pTableInfo);

	void ClearRoom(PrivateTableInfo* pTableInfo);

	void DBR_CreatePrivate(DBR_GR_Create_Private* kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem,std::string kHttpChannel);

	void DBR_JoinClubRom(DBR_GR_Join_Club_Rom *kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem);

	void DBR_CreateClubFloor(DB_GR_Create_Floor_Stress *kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem);

	//�����ӿ�
public:
 	//�ͷŶ���
 	virtual VOID Release(){ delete this; }
 	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//����֪ͨ
	virtual void OnStartService();

	//����ӿ�
public:
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
	//˽�˳���Ϣ
	virtual bool OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	
	//����˽�˳�
	bool OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//���¼���˽�˳�
	bool OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//����˽�˳�
	bool OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//��ɢ
	bool OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//�˳�������
	bool OnTCPNetworkSubExitSave(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//�˳�������
	bool OnTCPNetworkSubSelfRoomList(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//ѡ������
	bool OnTCPNetworkSubSelectChair(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//��ȡ�����Ϣ
	bool OnTCPNetworkGetChairInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID);
	//������󷿼���Ϣ
	bool OnTCPNetworkGetTableInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID);
	//������󷿼��б�
	bool OnTCPNetworkSubMyRoom(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//��Ҽ�����ֲ�������Ϣ
	bool OnTCPNetWorkSubJoinClubRom(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//������ֲ���������
	bool OnTCPNetWorkSubIntoClubRoomReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//������ֲ������Ӧ
	bool OnTCPNetWorkIntoClubRoomRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//��ҽ�����ֲ�����
	bool OnTCPNetWorkSubEnterClubRoomReq(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel); 
	//����¥������
	bool OnTCPNetworkCreateFloorStressReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//����¥���Ӧ
	bool OnTCPNetCreateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//���¾��ֲ�¥������
	bool OnTCPNetUpdateClubFloorReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//������ֲ�¥���Ӧ
	bool OnTCPNetUpdateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//�������ֲ�����
	bool OnTCPNetCreateClubRoom(ClubGameInfo  *pClubGameInfo,WORD &wTableID, BYTE bFloorIndex);
	//���ͷ�����Ϣ
	void OnNotifyClubRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo *pTableInfo,BYTE  bFloorIndex, BYTE bLastFlag);
	
public:
	//�û�����
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//�û�״̬
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//�û�Ȩ��
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true);	

	//˽�˳��û��¼�
	virtual bool AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex);

	//�¼��ӿ�
public:
	//�û���¼
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//�û��ǳ�
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile);	
	//�û�����
	virtual bool OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID);
	//�û�����
	virtual bool OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);

	 //���ܺ���
public:
	 //��Ϸ��ʼ
	 virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	 //��Ϸ����
	 virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	 //�û���ȥ��Ϸ
	 virtual bool OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem);

	 //�û��¼�
public:
	 //�û�����
	 virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	 //�û�����
	 virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�����
	 virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //�û�ͬ��
	 virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	
public:
	 //�û�����
	virtual bool OnEventReqStandUP(IServerUserItem * pIServerUserItem);

	virtual bool WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData);	
	//��������
protected:
	//ռ����Ϸ����������
	void  OnSubCreateOneClubRoom(IServerUserItem * pIServerUserItem,DWORD  dwClubID, BYTE bFloorIndex,DWORD dwMasterID,DWORD dwManagerID);
	int m_iRoomNumId; // ÿ�δ��������ǰһ�������


	std::map<int,std::map<int,std::vector<int>>>   m_clubRoom; 
};

#endif
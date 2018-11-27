#ifndef PRIVATE_HEAD_FILE
#define PRIVATE_HEAD_FILE

#pragma once

//引入文件
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"
#include "PrivateTableInfo.h"
#include "ClubGame.h"

///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////

#define  MAX_CLUB_GAME_NUM                         200      //游戏最多俱乐部
#define  CLUB_DISS_TIME						       5000		//解散俱乐部时间
struct DBR_GR_Create_Private;
struct DBR_GR_Join_Club_Rom;

//定时赛
class PriaveteGame 
	:public IGamePrivateItem
	,public IPrivateEventSink
	,public IServerUserItemSink
{
	//比赛配置
protected:
	tagGameServiceOption *				m_pGameServiceOption;			//服务配置
	tagGameServiceAttrib *				m_pGameServiceAttrib;			//服务属性

	CMD_GR_Private_Info					m_kPrivateInfo;
	//内核接口
protected:
	PrivateTableInfo*					m_vecTableInfo;				//
	ClubGameInfo*                       m_pClubGame;                    //俱乐部信息
	ITimerEngine *						m_pITimerEngine;				//时间引擎
	IDBCorrespondManager *				m_pIDataBaseEngine;				//数据引擎	
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//网络引擎

	//服务接口
protected:
	IMainServiceFrame *					m_pIGameServiceFrame;			//功能接口
	IServerUserManager *				m_pIServerUserManager;			//用户管理
	IAndroidUserManager	*				m_pAndroidUserManager;			//机器管理
	IServerUserItemSink *				m_pIServerUserItemSink;			//用户回调

	//函数定义
public:
	//构造函数
	PriaveteGame();
	//析构函数
	virtual ~PriaveteGame(void);

	bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	bool SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, bool needReady=true);

	void CreatePrivateCost(PrivateTableInfo* pTableInfo);

	int joinPrivateRoom(IServerUserItem * pIServerUserItem,ITableFrame * pICurrTableFrame,DWORD dwSeatNum = -1);
	bool selectRoomChair(IServerUserItem* pIServerUserItem, ITableFrame* pICurrTableFrame, BYTE idex);

	bool OnEventCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	bool OnEnventJoinClubRom(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);

	//通过私人场方式加入俱乐部房间判断
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

	//基础接口
public:
 	//释放对象
 	virtual VOID Release(){ delete this; }
 	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//控制接口
public:
	//启动通知
	virtual void OnStartService();

	//管理接口
public:
	//绑定桌子
	virtual bool BindTableFrame(ITableFrame * pTableFrame,WORD wTableID);
	//初始化接口
	virtual bool InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter);	

	//系统事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//私人场消息
	virtual bool OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);	
	//创建私人场
	bool OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//重新加入私人场
	bool OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//加入私人场
	bool OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//解散
	bool OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//退出但保存
	bool OnTCPNetworkSubExitSave(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//退出但保存
	bool OnTCPNetworkSubSelfRoomList(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//选择桌子
	bool OnTCPNetworkSubSelectChair(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//获取玩家信息
	bool OnTCPNetworkGetChairInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID);
	//玩家请求房间信息
	bool OnTCPNetworkGetTableInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID);
	//玩家请求房间列表
	bool OnTCPNetworkSubMyRoom(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//玩家加入俱乐部房间信息
	bool OnTCPNetWorkSubJoinClubRom(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//进入俱乐部房间请求
	bool OnTCPNetWorkSubIntoClubRoomReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//进入俱乐部房间回应
	bool OnTCPNetWorkIntoClubRoomRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//玩家进入俱乐部房间
	bool OnTCPNetWorkSubEnterClubRoomReq(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel); 
	//创建楼层请求
	bool OnTCPNetworkCreateFloorStressReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//创建楼层回应
	bool OnTCPNetCreateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//更新俱乐部楼层请求
	bool OnTCPNetUpdateClubFloorReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//请求俱乐部楼层回应
	bool OnTCPNetUpdateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel);
	//创建俱乐部房间
	bool OnTCPNetCreateClubRoom(ClubGameInfo  *pClubGameInfo,WORD &wTableID, BYTE bFloorIndex);
	//发送房间信息
	void OnNotifyClubRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo *pTableInfo,BYTE  bFloorIndex, BYTE bLastFlag);
	
public:
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//用户状态
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//用户权限
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true);	

	//私人场用户事件
	virtual bool AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex);

	//事件接口
public:
	//用户登录
	virtual bool OnEventUserLogon(IServerUserItem * pIServerUserItem);
	//用户登出
	virtual bool OnEventUserLogout(IServerUserItem * pIServerUserItem);
	//进入事件
	virtual bool OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile);	
	//用户参赛
	virtual bool OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID);
	//用户退赛
	virtual bool OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);

	 //功能函数
public:
	 //游戏开始
	 virtual bool OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	 //游戏结束
	 virtual bool OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	 //用户进去游戏
	 virtual bool OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem);

	 //用户事件
public:
	 //用户断线
	 virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	 //用户坐下
	 virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户起来
	 virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户同意
	 virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	
public:
	 //用户起来
	virtual bool OnEventReqStandUP(IServerUserItem * pIServerUserItem);

	virtual bool WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData);	
	//辅助函数
protected:
	//占用游戏服务器房间
	void  OnSubCreateOneClubRoom(IServerUserItem * pIServerUserItem,DWORD  dwClubID, BYTE bFloorIndex,DWORD dwMasterID,DWORD dwManagerID);
	int m_iRoomNumId; // 每次创建房间的前一个房间号


	std::map<int,std::map<int,std::vector<int>>>   m_clubRoom; 
};

#endif
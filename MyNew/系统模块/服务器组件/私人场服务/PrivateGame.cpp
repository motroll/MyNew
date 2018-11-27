#include "StdAfx.h"
#include "PrivateGame.h"
#include "..\游戏服务器\DataBasePacket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//时钟定义
#define IDI_DISMISS_WAITE_END		(IDI_PRIVATE_MODULE_START+1)					//请求解散结束

#define DISMISS_WAITE_TIME		120					//请求解散时间

#define AGAIN_WAITE_TIME		40					//重新开始等待时间

#define OFFLINE_WAITE_TIME		300			//全部掉线等待时间

#define EMPTY_WAITE_TIME		3600				//空房间解散时间
#define EMPTY_WAITE_TIME_CLUB   600				    //空房间解散时间(俱乐部的房间)

#define IDI_DISMISS_UNSTART			(IDI_DISMISS_WAITE_END + 1)
#define GAME_UNSTART_DISMISS_TIME  3600			//房间没开始多久解散


using namespace  std;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
PriaveteGame::PriaveteGame()
{
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;

	//内核接口
	m_vecTableInfo=NULL;
	m_pClubGame=NULL;
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngineEvent=NULL;

	//服务接口
	m_pIGameServiceFrame=NULL;
	m_pIServerUserManager=NULL;
	m_pAndroidUserManager=NULL;

	// 初始化变量
	m_iRoomNumId = 0;
}

PriaveteGame::~PriaveteGame(void)
{
	//释放资源
	SafeDeleteArray(m_vecTableInfo);
	SafeDeleteArray(m_pClubGame);
	//关闭定时器
	m_pITimerEngine->KillTimer(IDI_DISMISS_WAITE_END);
}

//接口查询
VOID* PriaveteGame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{	
	QUERYINTERFACE(IGamePrivateItem,Guid,dwQueryVer);
	QUERYINTERFACE(IPrivateEventSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);	
	QUERYINTERFACE_IUNKNOWNEX(IGamePrivateItem,Guid,dwQueryVer);
	return NULL;
}

//绑定桌子
bool PriaveteGame::BindTableFrame(ITableFrame * pTableFrame,WORD wTableID)
{
	if(pTableFrame==NULL || wTableID>m_pGameServiceOption->wTableCount)
	{
		ASSERT(false);
		return false;
	}

	//创建钩子
	CTableFramePrivate * pTableFrameHook=new CTableFramePrivate();
	pTableFrameHook->InitTableFramePrivate(QUERY_OBJECT_PTR_INTERFACE(pTableFrame,IUnknownEx));
	pTableFrameHook->SetPrivateEventSink(QUERY_OBJECT_PTR_INTERFACE(this,IUnknownEx));

	//设置接口
	pTableFrame->SetTableFramePrivate(QUERY_OBJECT_PTR_INTERFACE(pTableFrameHook,IUnknownEx));
	m_vecTableInfo[wTableID].pITableFrame=pTableFrame;
	m_vecTableInfo[wTableID].restValue();

	return true;
}

//初始化接口
bool PriaveteGame::InitPrivateInterface(tagPrivateManagerParameter & MatchManagerParameter)
{
	m_pGameServiceOption=MatchManagerParameter.pGameServiceOption;
	m_pGameServiceAttrib=MatchManagerParameter.pGameServiceAttrib;

	//内核组件
	m_pITimerEngine=MatchManagerParameter.pITimerEngine;
	m_pIDataBaseEngine=MatchManagerParameter.pICorrespondManager;
	m_pITCPNetworkEngineEvent=MatchManagerParameter.pTCPNetworkEngine;

	//服务组件		
	m_pIGameServiceFrame=MatchManagerParameter.pIMainServiceFrame;		
	m_pIServerUserManager=MatchManagerParameter.pIServerUserManager;
	m_pAndroidUserManager=MatchManagerParameter.pIAndroidUserManager;
	m_pIServerUserItemSink=MatchManagerParameter.pIServerUserItemSink;

	//创建桌子
	if (m_vecTableInfo==NULL)
	{
		m_vecTableInfo = new PrivateTableInfo[m_pGameServiceOption->wTableCount];
	}

	//创建俱乐部
	if(m_pClubGame==NULL)
	{
		m_pClubGame = new ClubGameInfo[MAX_CLUB_GAME_NUM]; 
	}

	return true;
}
void PriaveteGame::OnStartService()
{
	//变量定义
	DBR_GR_Private_Info kPrivateInfo;
	ZeroMemory(&kPrivateInfo,sizeof(kPrivateInfo));
	kPrivateInfo.wKindID=m_pGameServiceOption->wKindID;
	m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_PRIVATE_INFO,0L,&kPrivateInfo,sizeof(kPrivateInfo));

	m_pITimerEngine->SetTimer(IDI_DISMISS_WAITE_END,5000L,TIMES_INFINITY,0);
}

//时间事件
bool PriaveteGame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{	
	switch(dwTimerID)
	{
	case IDI_DISMISS_WAITE_END:				//解散等待时间 10s
		{
			for(int i = 0;i<m_pGameServiceOption->wTableCount;i++)
			{
				PrivateTableInfo* pTableInfo = &m_vecTableInfo[i];
				if (pTableInfo->dwRoomNum == 0)
				{
					continue;
				}
				if (pTableInfo->bInEnd)
				{
					pTableInfo->fAgainPastTime += 5.0f;
					if (pTableInfo->fAgainPastTime >= AGAIN_WAITE_TIME)
					{
						if(pTableInfo->dwClubID != 0)  
						{
							ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
							if(pClubGame != NULL )
							{
								pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
							}
						}
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
				if (pTableInfo->kDismissChairID.size())
				{
					pTableInfo->fDismissPastTime += 5.0f;
					if (pTableInfo->fDismissPastTime >= DISMISS_WAITE_TIME)
					{
						if (pTableInfo->kNotAgreeChairID.size() <= 1)
						{
							if(pTableInfo->dwClubID != 0)  
							{
								ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
								if(pClubGame != NULL )
								{
									pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
								}
							}
							DismissRoom(pTableInfo);
							ClearRoom(pTableInfo);
						}
					}
				}
				if (pTableInfo->IsAllOffline())
				{
					pTableInfo->fOfflinePastTime += 5.0f;
					if (pTableInfo->fOfflinePastTime >= OFFLINE_WAITE_TIME)
					{
						if(pTableInfo->dwClubID != 0)  
						{
							ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
							if(pClubGame != NULL )
							{
								pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
							}
						}
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
				if (pTableInfo->pITableFrame->GetSitUserCount() == 0)
				{
					pTableInfo->fEmptyPastTime += 5.0f;
					if (pTableInfo->dwClubID != 0)
					{
						if(pTableInfo->fEmptyPastTime >= EMPTY_WAITE_TIME_CLUB)  
						{
							ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
							if(pClubGame != NULL )
							{
								pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
							}
						}
					}
					if (pTableInfo->fEmptyPastTime >= EMPTY_WAITE_TIME)
					{
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
				if(pTableInfo->bStart == false)
				{
					pTableInfo->fDismissUnStartTime += 5.0f;
					if(pTableInfo->fDismissUnStartTime >= GAME_UNSTART_DISMISS_TIME)
					{
						if(pTableInfo->dwClubID != 0)  
						{
							ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
							if(pClubGame != NULL )
							{
								pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
							}
						}
						DismissRoom(pTableInfo);
						ClearRoom(pTableInfo);
					}
				}
			}
			//for(int i =0; i < MAX_CLUB_GAME_NUM; i++)
			//{
			//	if(m_pClubGame[i].m_bCanDiss  &&  (m_pClubGame[i].m_dwDissTime - time(NULL)) >= CLUB_DISS_TIME)
			//	{
			//		m_pClubGame[i].ResetValue();
			//	}
			//}
			return true;
		}
	}
	
	return true;
}

//发送数据
bool PriaveteGame::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if(pIServerUserItem!=NULL)
		return m_pIGameServiceFrame->SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	else
		return m_pIGameServiceFrame->SendData(BG_ALL_CLIENT, wMainCmdID,wSubCmdID, pData, wDataSize);

	return true;
}
bool PriaveteGame::SendTableData(ITableFrame*	pITableFrame, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, bool needReady)
{
	return pITableFrame->SendTableData(INVALID_CHAIR,wSubCmdID,pData,wDataSize,wMainCmdID, needReady);
}
void PriaveteGame::CreatePrivateCost(PrivateTableInfo* pTableInfo)
{
	if (pTableInfo->cbRoomType == Type_Private)
	{
		if(pTableInfo->bGamePayType == 1)
		{
			int chairCount = pTableInfo->pITableFrame->GetChairCount();
			for(int i = 0; i < chairCount; ++i)
			{
				IServerUserItem *item = pTableInfo->pITableFrame->GetTableUserItem(i);
				if(item)
				{
					DBR_GR_Create_Private_Cost kNetInfo;
					kNetInfo.dwUserID = item->GetUserID();
					kNetInfo.dwCost = pTableInfo->dwPlayCost/4;
					kNetInfo.dwCostType = pTableInfo->cbRoomType;
					m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_CREATE_PRIVAT_COST,0L,&kNetInfo,sizeof(kNetInfo));
				}
			}
		}
		else
		{
			DBR_GR_Create_Private_Cost kNetInfo;
			kNetInfo.dwUserID = pTableInfo->dwCreaterUserID;
			kNetInfo.dwCost = pTableInfo->dwPlayCost;
			kNetInfo.dwCostType = pTableInfo->cbRoomType;
			m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_CREATE_PRIVAT_COST,0L,&kNetInfo,sizeof(kNetInfo));
		}		
	}
	if (pTableInfo->cbRoomType == Type_Public)
	{
		for (int i = 0;i<pTableInfo->pITableFrame->GetSitUserCount();i++)
		{
			if (!pTableInfo->pITableFrame->GetTableUserItem(i))
			{
				continue;
			}
			DBR_GR_Create_Private_Cost kNetInfo;
			kNetInfo.dwUserID = pTableInfo->pITableFrame->GetTableUserItem(i)->GetUserID();
			kNetInfo.dwCost = pTableInfo->dwPlayCost;
			kNetInfo.dwCostType = pTableInfo->cbRoomType;
			m_pIDataBaseEngine->PostDataBaseRequest(0L,DBR_GR_CREATE_PRIVAT_COST,0L,&kNetInfo,sizeof(kNetInfo));
		}
	}

}

PrivateTableInfo* PriaveteGame::getTableInfoByRoomID(DWORD dwRoomID)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_vecTableInfo[i].dwRoomNum == dwRoomID)
		{
			return &m_vecTableInfo[i];
		}
	}
	return NULL;
}
ClubGameInfo *PriaveteGame::getClubInfoByRoomID(DWORD dwRoomID,DWORD  dwClubID)
{
	for(int i = 0; i < MAX_CLUB_GAME_NUM ; i++)
	{
		if(m_pClubGame[i].ClubID ==  dwClubID && m_pClubGame[i].GetTableByRoomNum(dwRoomID))
		{
			return  &m_pClubGame[i];
		}
	}
	return NULL;
}

//获取俱乐部
ClubGameInfo * PriaveteGame::getClubInfoByClubID(DWORD dwClubID)
{
	for(int i = 0; i < MAX_CLUB_GAME_NUM ; i++)
	{
		if(m_pClubGame[i].ClubID == dwClubID)
		{
			return  &m_pClubGame[i];
		}
	}
	return NULL; 
}

//获取没有占用的俱乐部
ClubGameInfo* PriaveteGame::getClubInfoIsNullClub()
{
	for(int  i = 0; i < MAX_CLUB_GAME_NUM; i++)
	{
		if(m_pClubGame[i].ClubID == 0 || m_pClubGame[i].clubRomNum == 0)
		{
			return   &m_pClubGame[i];
		}
	}
	return  NULL;
}
void  PriaveteGame::SetDismissRomInClubRom(DWORD dwClubID)
{
	int   index ;
	for(index = 0 ; index <MAX_CLUB_GAME_NUM;index++)
	{
		if(m_pClubGame[index].ClubID == dwClubID)
		{

			m_pClubGame[index].clubRomNum -- ;
			break;
		}
	}

}

int PriaveteGame::getRoomCoutByCreaterID(DWORD dwUserID)
{
	int iRoomCout = 0;
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if ((m_vecTableInfo+i)&&m_vecTableInfo[i].pITableFrame && m_vecTableInfo[i].dwCreaterUserID == dwUserID)
		{
			iRoomCout ++;
		}
	}
	return iRoomCout;
}

std::vector<PrivateTableInfo*> PriaveteGame::getTableInfoByUserID(DWORD dwUserD)
{
	std::vector<PrivateTableInfo*> ret;
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if ((m_vecTableInfo+i)&&m_vecTableInfo[i].pITableFrame && m_vecTableInfo[i].dwCreaterUserID == dwUserD && m_vecTableInfo[i].dwClubID == 0)
		{
			ret.push_back(&m_vecTableInfo[i]);
		}
	}
	return ret;
}
PrivateTableInfo* PriaveteGame::getTableInfoByTableID(DWORD dwRoomID)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if ((m_vecTableInfo+i)&&m_vecTableInfo[i].pITableFrame && m_vecTableInfo[i].pITableFrame->GetTableID() == dwRoomID)
		{
			return &m_vecTableInfo[i];
		}
	}
	return NULL;
}
PrivateTableInfo* PriaveteGame::getTableInfoByTableFrame(ITableFrame* pTableFrame)
{
	for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
	{
		if (m_vecTableInfo[i].pITableFrame == pTableFrame)
		{
			return &m_vecTableInfo[i];
		}
	}
	return NULL;
}

//数据库事件
bool PriaveteGame::OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_PRIVATE_INFO:		//私人场信息
		{
			//参数效验
			if(wDataSize>sizeof(DBO_GR_Private_Info)) return false;

			//提取数据
			DBO_GR_Private_Info * pPrivate = (DBO_GR_Private_Info*)pData;
			m_kPrivateInfo.wKindID = pPrivate->wKindID;
			m_kPrivateInfo.lCostGold = pPrivate->lCostGold;
			memcpy(&m_kPrivateInfo.bPlayCout,pPrivate->bPlayCout,sizeof(m_kPrivateInfo.bPlayCout));
			memcpy(&m_kPrivateInfo.lPlayCost,pPrivate->lPlayCost,sizeof(m_kPrivateInfo.lPlayCost));

			break;
		}
	case DBO_GR_CREATE_PRIVATE:		//私人场信息
		{
			OnEventCreatePrivate(wRequestID,pIServerUserItem,pData,wDataSize,"");
			break;
		}
	case  DBO_GR_JOIN_CLUB_ROM:    //加入俱乐部房间信息
		{
			OnEnventJoinClubRom(wRequestID, pIServerUserItem, pData, wDataSize, "");
			//OnTCPNetWorkSubEnterClubRoomReq(wRequestID, pIServerUserItem, pData, wDataSize, "");
			break;
		}
	case  DBO_GR_CLUB_ROM_PRIVATE_RES:
		{
			OnEnventClubRomPriavteRes(wRequestID, pIServerUserItem, pData, wDataSize,"");
			break;
		}
	case DBR_GR_CLUB_JOIN_CLUB_ROM_RES:
		{
			OnTCPNetWorkIntoClubRoomRes(wRequestID, pIServerUserItem, pData, wDataSize,"");
			break;
		}
	case DBR_GR_CLUB_UPDATE_FLOOR_RES:
		{
			OnTCPNetUpdateClubFloorRes(wRequestID, pIServerUserItem, pData, wDataSize,"");
			break;
		}
	case DBR_GR_CLUB_CREATE_FLOOR_RES:
		{
			OnTCPNetCreateClubFloorRes(wRequestID, pIServerUserItem, pData, wDataSize,"");
			break;
		}
	}
	return true;
}

int PriaveteGame::joinPrivateRoom(IServerUserItem * pIServerUserItem,ITableFrame * pICurrTableFrame, DWORD dwSeatNum)
{
	WORD wGaveInChairID = INVALID_CHAIR;
	for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
	{
		if (pICurrTableFrame->GetTableUserItem(i)==pIServerUserItem)
		{
			wGaveInChairID = i;
			break;
		}
	}
	if (wGaveInChairID!=INVALID_CHAIR)
	{
		//pIServerUserItem->SetUserStatus(US_READY,pICurrTableFrame->GetTableID(),wGaveInChairID);
		return true;
	}
	WORD wChairID = INVALID_CHAIR;
	if(dwSeatNum !=-1)
	{
		wChairID = dwSeatNum;
	}
	else 
	{
		//椅子搜索
		for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
		{
			if (pICurrTableFrame->GetTableUserItem(i)==NULL)
			{
				wChairID = i;
				break;
			}
		}
	}


	//分配用户
	if (wChairID!=INVALID_CHAIR)
	{
		//用户起立
// 		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
// 		{
// 			return pIServerUserItem->GetTableID() == pICurrTableFrame->GetTableID();
// 		}

		//wChairID = INVALID_CHAIR;
		//用户坐下
		int ret = pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		if(ret != 1)
		//if(pICurrTableFrame->PerformSelectChairAction(pIServerUserItem)==false)
		{
			return ret;
		}
		return true;
	}	
	return false;
}
bool PriaveteGame::selectRoomChair(IServerUserItem* pIServerUserItem, ITableFrame* pICurrTableFrame, BYTE idex)
{
	WORD wGaveInChairID = INVALID_CHAIR;
	for (WORD i=0;i<pICurrTableFrame->GetChairCount();i++)
	{
		if (pICurrTableFrame->GetTableUserItem(i)==pIServerUserItem)
		{
			wGaveInChairID = i;
			break;
		}
	}
// 	if (wGaveInChairID!=INVALID_CHAIR)
// 	{
// 		//pIServerUserItem->SetUserStatus(US_READY,pICurrTableFrame->GetTableID(),wGaveInChairID);
// 		return true;
// 	}
	WORD wChairID = INVALID_CHAIR;
	bool isExchange = false;

	IServerUserItem* pOtherUserItem = pICurrTableFrame->GetTableUserItem(idex);
	if (pICurrTableFrame->GetTableUserItem(idex)!=NULL && pIServerUserItem->GetChairID() != idex && pOtherUserItem)
	{
		if(pOtherUserItem->GetUserStatus() > US_SELECT)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("加入房间失败。"),SMT_EJECT);
			return true;
		}
		else
		{
			//pOtherUserItem->SetUserTableChair(US_SELECT,pOtherUserItem->GetTableID(),wGaveInChairID);

			wChairID = idex;
			if(wChairID != INVALID_CHAIR)
			{
				if(pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					return false;
				}
			}
			if(pICurrTableFrame->performExchangeChairAction(pOtherUserItem,wGaveInChairID)==false)
			{
				return false;
			}

			return true;
		}
	}

	wChairID = idex;

	if (wChairID!=INVALID_CHAIR)
	{
// 		//用户起立
// 		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
// 		{
// 			return pIServerUserItem->GetTableID() == pICurrTableFrame->GetTableID();
// 		}

		//用户坐下
		if(pICurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
		{
			return false;
		}
		return true;
	}

	return false;
}
//创建房间
bool PriaveteGame::OnEventCreatePrivate(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//参数效验
	if(pIServerUserItem==NULL) return true;
	if(wDataSize>sizeof(DBO_GR_CreatePrivateResoult)) return false;

	//提取数据
	DBO_GR_CreatePrivateResoult* pPrivate = (DBO_GR_CreatePrivateResoult*)pData;	
	DWORD dwAgaginTable = pPrivate->dwAgaginTable;
	//报名失败
	if(pPrivate->bSucess==false)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pPrivate->szDescribeString,SMT_EJECT);

		return true;
	}
	if (pPrivate->bPlayCoutIdex < 0 || pPrivate->bPlayCoutIdex >= 4)
	{
		return false;
	}

	//寻找位置
	int  iClubTableNum  = 0;
	ITableFrame * pICurrTableFrame=NULL;
	PrivateTableInfo* pCurrTableInfo=NULL;
	ClubGameInfo *pClubInfo = NULL;
	if(pPrivate->dwClubID !=0)
	{
		pClubInfo = getClubInfoByClubID(pPrivate->dwClubID);  //判断该俱乐部是否在游戏场中有房间
		OnSubCreateOneClubRoom(pIServerUserItem,pPrivate->dwClubID,pPrivate->bFloorIndex,pPrivate->dwManagerID,pPrivate->dwMasterID);
		return  true;

	}
	if (dwAgaginTable != INVALID_DWORD)
	{
		pCurrTableInfo = getTableInfoByTableID(dwAgaginTable);
		if (!pCurrTableInfo)
		{
			return false;
		}
		pICurrTableFrame = pCurrTableInfo->pITableFrame;
		if (pCurrTableInfo->bInEnd == false)
		{
			joinPrivateRoom(pIServerUserItem,pICurrTableFrame);
			return true;
		}
		else
		{
			pCurrTableInfo->restAgainValue();
			sendPrivateRoomInfo(NULL,pCurrTableInfo);
		}
	}
	else
	{
		WORD  wTableID = 0;
		PrivateTableInfo* pTableInfo   = getTableInfoByTableID(pIServerUserItem->GetTableID());
		if (pTableInfo)
		{
			joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame);
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("你已经在游戏中，不能再创建。"),SMT_EJECT|SMT_CREATE_ROOM);
			return true;
		}
		for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
		{
			//获取对象
			ASSERT(m_vecTableInfo[wTableID].pITableFrame!=NULL);
			PrivateTableInfo* pPrivateInfo = &m_vecTableInfo[wTableID];
			ITableFrame * pITableFrame=pPrivateInfo->pITableFrame;
			if(m_vecTableInfo[wTableID].bInEnd)
			{
				continue;
			}
			//状态判断
			if (pPrivateInfo->dwRoomNum == 0)
			{
				pICurrTableFrame = pITableFrame;
				pCurrTableInfo = &m_vecTableInfo[wTableID];
				pCurrTableInfo->restValue();
				break;
			}	
		}
	}
	//桌子判断
	if(pICurrTableFrame==NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败。"),SMT_EJECT|SMT_CREATE_ROOM);
		return true;
	}

	// todo wlda
	if(pPrivate->bGamePayType ==1)
	{
		if(!joinPrivateRoom(pIServerUserItem,pICurrTableFrame))
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败。"),SMT_EJECT|SMT_CREATE_ROOM);
			return true;
		}
	}

	int iRandNum = 0;
	while (iRandNum == 0 || getTableInfoByRoomID(iRandNum) || iRandNum == m_iRoomNumId)
	{
		iRandNum = 1000+rand()%8900;
		if(m_pGameServiceOption->wServerID >= 90)
		{
			int count = m_pGameServiceOption->wServerID / 90;
			iRandNum = (m_pGameServiceOption->wServerID - (90 * count) + 10) * 10000 + iRandNum;
		}
		else
			iRandNum = (m_pGameServiceOption->wServerID+10)*10000+iRandNum;
	}
	m_iRoomNumId = iRandNum;
	//end
	pCurrTableInfo->dwPlayCout = (DWORD)m_kPrivateInfo.bPlayCout[pPrivate->bPlayCoutIdex];	
	pCurrTableInfo->setRoomNum(iRandNum);
	pCurrTableInfo->dwCreaterUserID = pIServerUserItem->GetUserID();

	pCurrTableInfo->kHttpChannel = kChannel;
	pCurrTableInfo->cbRoomType = pPrivate->cbRoomType;

	pCurrTableInfo->bGameRuleIdex = pPrivate->bGameRuleIdex;
	pCurrTableInfo->bGameTypeIdex = pPrivate->bGameTypeIdex;
	pCurrTableInfo->bPlayCoutIdex = pPrivate->bPlayCoutIdex;
	pCurrTableInfo->bGamePayType = pPrivate->bGamePayType;
	pCurrTableInfo->bGameBaseScore = pPrivate->bGameBaseScore;
	pCurrTableInfo->bGameGuDingMa = pPrivate->bGameGuDingMa;
	pCurrTableInfo->bGameCardType = pPrivate->bGameCardType;
	//if(pPrivate->bGameTypeIndex !=4 && pPrivate->bGameTypeIndex != 6) pPrivate->bAllOther = 0;
	//if(pPrivate->bGameTypeIndex !=3 && pPrivate->bGameTypeIndex != 6) pPrivate->bQiangGangHu = 0;
	pCurrTableInfo->bAllOther = pPrivate->bAllOther;
	pCurrTableInfo->bQiangGangHu = pPrivate->bQiangGangHu;
	pCurrTableInfo->bGameTypeIndex = pPrivate->bGameTypeIndex; 
	pCurrTableInfo->bGameTypeQing = pPrivate->bGameTypeQing;
	pCurrTableInfo->cbPantherType = pPrivate->cbPantherType;
	pCurrTableInfo->cbPlayerNum = pPrivate->cbPlayerNum;

	pCurrTableInfo->bGameMaxScore = pPrivate->bGameMaxScore;
	pCurrTableInfo->bGameMaxScoreGold = pPrivate->bGameMaxScoreGold;
	pCurrTableInfo->bPiaoShu = pPrivate->bPiaoShu;
	pCurrTableInfo->bBaoIsOutCard = pPrivate->bBaoIsOutCard;
	pCurrTableInfo->bFendDing = pPrivate->bFendDing;
	pCurrTableInfo->bJingDing = pPrivate->bJingDing;
	
	pCurrTableInfo->bLimitIP = pPrivate->bLimitIP;
	pCurrTableInfo->fEmptyPastTime =0;
	pCurrTableInfo->fDismissUnStartTime=0;

	if (pPrivate->cbRoomType == Type_Private)
	{
		if(pCurrTableInfo->dwPlayCout == 16)
		{
			if(pCurrTableInfo->bGameTypeIndex == 3 && pCurrTableInfo->bGameBaseScore == 1)
			{
				pCurrTableInfo->dwPlayCost = 8;
			}
			else if(pCurrTableInfo->bGameBaseScore == 3)
			{
				pCurrTableInfo->dwPlayCost = 16;
			}
			else 
			{
				pCurrTableInfo->dwPlayCost = 12;
			}
		}
		else
		{
			pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lPlayCost[pPrivate->bPlayCoutIdex];
		}
	}
	else
	{
		pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lCostGold;
	}
	pCurrTableInfo->dwPlayCost = pCurrTableInfo->dwPlayCost / 4 * pCurrTableInfo->cbPlayerNum; 
	pCurrTableInfo->dwFinishPlayCout = 0;
	GetLocalTime(&pCurrTableInfo->kTotalRecord.kPlayTime);

	pICurrTableFrame->SetPrivateInfo(pCurrTableInfo->bGameTypeIdex,pCurrTableInfo->bGameRuleIdex,pCurrTableInfo->bGameBaseScore,pCurrTableInfo->bGameGuDingMa,pCurrTableInfo->bQiangGangHu,pCurrTableInfo->bGameCardType,pCurrTableInfo->bLimitIP);
	pICurrTableFrame->SetPrivateFushunInfo(pCurrTableInfo->bGameTypeIndex,pCurrTableInfo->bGameTypeQing,pCurrTableInfo->bLimitIP,
		pCurrTableInfo->bGameMaxScore,pCurrTableInfo->bGameMaxScoreGold,pCurrTableInfo->bPiaoShu,pCurrTableInfo->bBaoIsOutCard,
		pCurrTableInfo->bFendDing,pCurrTableInfo->bJingDing,pCurrTableInfo->bAllOther, pCurrTableInfo->cbPantherType,pCurrTableInfo->cbPlayerNum);
	pICurrTableFrame->SetCreateUserID(pCurrTableInfo->dwCreaterUserID);

	CMD_GF_Create_Private_Sucess kSucessInfo;
	kSucessInfo.lCurSocre = pPrivate->bSucess;
	kSucessInfo.dwRoomNum = pCurrTableInfo->dwRoomNum;
	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_CREATE_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));


	sendPrivateRoomInfo(NULL,pCurrTableInfo);
	return true;
}

	//创建私人场
bool PriaveteGame::OnTCPNetworkSubCreatePrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Create_Private));
	if(wDataSize!=sizeof(CMD_GR_Create_Private)) return false;

	CMD_GR_Create_Private* pCMDInfo = (CMD_GR_Create_Private*)pData;
	pCMDInfo->stHttpChannel[CountArray(pCMDInfo->stHttpChannel)-1]=0;
	ClubGameInfo *pClubNode  = NULL;
	if(pCMDInfo->dwClubID != 0)
	{
		pClubNode = getClubInfoByClubID(pCMDInfo->dwClubID);
		if(pClubNode==NULL )
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建俱乐部房间失败"),SMT_EJECT|SMT_CHAT|SMT_CREATE_ROOM);
			return true;
		}
		std::map<BYTE,FloorStress>::iterator  iterFloor =pClubNode->m_mapFloorStress.find(pCMDInfo->bFloorIndex);
		if(iterFloor==pClubNode->m_mapFloorStress.end())
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建俱乐部房间失败"),SMT_EJECT|SMT_CHAT|SMT_CREATE_ROOM);
			return true;
		}
		
		DBR_GR_Create_Private kDBRInfo;
		ZeroMemory(&kDBRInfo,sizeof(DBR_GR_Create_Private));
		kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
		kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
		kDBRInfo.cbRoomType = Type_Private;
		kDBRInfo.dwCostType = Type_Private;
		kDBRInfo.dwAgaginTable = INVALID_DWORD;
		kDBRInfo.dwCost = m_kPrivateInfo.lPlayCost[iterFloor->second.lGameInfo.bPlayCoutIdex];
		kDBRInfo.dwClubID = pCMDInfo->dwClubID;
		kDBRInfo.bFloorIndex = pCMDInfo->bFloorIndex;
		DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,pCMDInfo->stHttpChannel);
		return  true;
	}
	else if(pCMDInfo->cbGameType != Type_Public)
	{
		DBR_GR_Create_Private kDBRInfo;

		kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
		kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
		kDBRInfo.cbRoomType = Type_Private;
		kDBRInfo.dwCostType = Type_Private;
		kDBRInfo.dwAgaginTable = INVALID_DWORD;
		kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pCMDInfo->bPlayCoutIdex];
		kDBRInfo.bPlayCoutIdex = pCMDInfo->bPlayCoutIdex;
		kDBRInfo.bGameRuleIdex = pCMDInfo->bGameRuleIdex;
		kDBRInfo.bGameTypeIdex = pCMDInfo->bGameTypeIdex;
		kDBRInfo.bGamePayType = pCMDInfo->bGamePayType;
		kDBRInfo.bGameBaseScore = pCMDInfo->bGameBaseScore;
		kDBRInfo.bGameGuDingMa = pCMDInfo->bGameGuDingMa;
		kDBRInfo.bQiangGangHu = pCMDInfo->bQiangGangHu;
		kDBRInfo.bGameCardType = pCMDInfo->bGameCardType;
		kDBRInfo.bAllOther = pCMDInfo->bAllOther;
		kDBRInfo.cbPantherType = pCMDInfo->cbPantherType;
		kDBRInfo.cbPlayerNum = pCMDInfo->cbPlayerNum;
	
		
		//fushun
		kDBRInfo.bGameTypeIndex = pCMDInfo->bGameTypeIndex;
		kDBRInfo.bGameTypeQing = pCMDInfo->bGameTypeQing;

		kDBRInfo.bGameMaxScore = pCMDInfo->bGameMaxScore;
		kDBRInfo.bGameMaxScoreGold = pCMDInfo->bGameMaxScoreGold;
		kDBRInfo.bBaoIsOutCard = pCMDInfo->bBaoIsOutCard;
		kDBRInfo.bPiaoShu = pCMDInfo->bPiaoShu;
		kDBRInfo.bFendDing = pCMDInfo->bFendDing;
		kDBRInfo.bJingDing = pCMDInfo->bJingDing;
		kDBRInfo.bFloorIndex = pCMDInfo->bFloorIndex;
		kDBRInfo.dwClubID = pCMDInfo->dwClubID;
		kDBRInfo.bLimitIP = pCMDInfo->bLimitIP;
		if(kDBRInfo.bGameTypeIndex == 2 || kDBRInfo.bGameTypeIndex == 4) kDBRInfo.bQiangGangHu=0 ;
		if(kDBRInfo.bGameTypeIndex == 2 || kDBRInfo.bGameTypeIndex == 3 ) kDBRInfo.bAllOther = 0;

		DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,pCMDInfo->stHttpChannel);
	}
	else
	{
		ITableFrame * pICurrTableFrame=NULL;
		PrivateTableInfo* pCurrTableInfo=NULL;
		WORD wTableID = 0;
		for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
		{
			//获取对象
			PrivateTableInfo& pTableInfo = m_vecTableInfo[wTableID];
			ASSERT(pTableInfo.pITableFrame!=NULL);
			ITableFrame * pITableFrame=pTableInfo.pITableFrame;
			if(pTableInfo.bInEnd)
			{
				continue;
			}
			if(pTableInfo.cbRoomType != Type_Public)
			{
				continue;
			}
			if (m_vecTableInfo[wTableID].bGameRuleIdex != pCMDInfo->bGameRuleIdex 
				|| m_vecTableInfo[wTableID].bGameTypeIdex != pCMDInfo->bGameTypeIdex )
			{
				continue;
			}
			//状态判断
			if (pITableFrame->GetNullChairCount() >= 0)
			{
				pICurrTableFrame = pITableFrame;
				pCurrTableInfo = &m_vecTableInfo[wTableID];
				break;
			}
		}
		if (pICurrTableFrame == NULL)
		{
			DBR_GR_Create_Private kDBRInfo;
			kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
			kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
			kDBRInfo.cbRoomType = Type_Public;
			kDBRInfo.dwCostType = Type_Public;
			kDBRInfo.dwAgaginTable = INVALID_DWORD;
			kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pCMDInfo->bPlayCoutIdex];
			kDBRInfo.bPlayCoutIdex = pCMDInfo->bPlayCoutIdex;
			kDBRInfo.bGameRuleIdex = pCMDInfo->bGameRuleIdex;
			kDBRInfo.bGameTypeIdex = pCMDInfo->bGameTypeIdex;
			kDBRInfo.bGamePayType = pCMDInfo->bGamePayType;
			kDBRInfo.bGameBaseScore = pCMDInfo->bGameBaseScore;
			kDBRInfo.bGameGuDingMa = pCMDInfo->bGameGuDingMa;
			kDBRInfo.bQiangGangHu = pCMDInfo->bQiangGangHu;
			kDBRInfo.bGameCardType = pCMDInfo->bGameCardType;
			kDBRInfo.bAllOther = pCMDInfo->bAllOther;
			kDBRInfo.cbPantherType = pCMDInfo->cbPantherType;
			kDBRInfo.cbPlayerNum = pCMDInfo->cbPlayerNum;
			//fushun
			kDBRInfo.bGameTypeIndex = pCMDInfo->bGameTypeIndex;
			kDBRInfo.bGameTypeQing = pCMDInfo->bGameTypeQing;

			kDBRInfo.bGameMaxScore = pCMDInfo->bGameMaxScore;
			kDBRInfo.bGameMaxScoreGold = pCMDInfo->bGameMaxScoreGold;
			kDBRInfo.bPiaoShu = pCMDInfo->bPiaoShu;
			kDBRInfo.bBaoIsOutCard = pCMDInfo->bBaoIsOutCard;
			kDBRInfo.bFendDing = pCMDInfo->bFendDing;
			kDBRInfo.bJingDing = pCMDInfo->bJingDing;
			kDBRInfo.dwClubID = pCMDInfo->dwClubID;
			kDBRInfo.bLimitIP = pCMDInfo->bLimitIP;

			if(kDBRInfo.bGameTypeIndex == 2 || kDBRInfo.bGameTypeIndex == 4) kDBRInfo.bQiangGangHu=0 ;
			if(kDBRInfo.bGameTypeIndex == 2 || kDBRInfo.bGameTypeIndex == 3 ) kDBRInfo.bAllOther = 0;

			DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,"");
		}
		else
		{
			joinPrivateRoom(pIServerUserItem,pICurrTableFrame);
			if(pCurrTableInfo->dwClubID == 0 )
			{
				return true;
			}
			ClubGameInfo  *pClubGame = getClubInfoByClubID(pCurrTableInfo->dwClubID);
			if(pClubGame !=NULL) 
			{
				return true;	
			}
			pClubGame->UpdateRoomPlayerNum(pCurrTableInfo->pITableFrame->GetNullChairCount(),	
						pCurrTableInfo->bFloorIndex, pCurrTableInfo->dwRoomNum);
		}
	}
	return true;
}
//重新加入私人场
bool PriaveteGame::OnTCPNetworkSubAgainEnter(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Again_Private));
	if(wDataSize!=sizeof(CMD_GR_Again_Private)) return false;

	CMD_GR_Again_Private* pCMDInfo = (CMD_GR_Again_Private*)pData;
	pCMDInfo->stHttpChannel[CountArray(pCMDInfo->stHttpChannel)-1] = 0;

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->dwCreaterUserID != pIServerUserItem->GetUserID())
	{
		joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame);
		if(pTableInfo->dwClubID !=0 )
		{
			ClubGameInfo  *pClubGame = getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame !=NULL) 
			{
				pClubGame->UpdateRoomPlayerNum(pTableInfo->pITableFrame->GetNullChairCount(),	
						pTableInfo->bFloorIndex, pTableInfo->dwRoomNum);
			}
		}
		return true;
	}
	if (!pTableInfo->bInEnd && pTableInfo->dwRoomNum != 0)
	{
		joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame);
		if(pTableInfo->dwClubID !=0 )
		{
			ClubGameInfo  *pClubGame = getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame !=NULL) 
			{
				pClubGame->UpdateRoomPlayerNum(pTableInfo->pITableFrame->GetNullChairCount(),	
						pTableInfo->bFloorIndex, pTableInfo->dwRoomNum);
			}
		}
		return true;
	}
	DBR_GR_Create_Private kDBRInfo;
	ZeroMemory(&kDBRInfo,sizeof(kDBRInfo));
	kDBRInfo.cbRoomType = pTableInfo->cbRoomType;
	kDBRInfo.dwUserID = pIServerUserItem->GetUserID();
	kDBRInfo.wKindID = m_pGameServiceAttrib->wKindID;
	kDBRInfo.dwCost = (DWORD)m_kPrivateInfo.lPlayCost[pTableInfo->bPlayCoutIdex];
	kDBRInfo.dwAgaginTable = pIServerUserItem->GetTableID();
	kDBRInfo.bGameTypeIdex = pTableInfo->bGameTypeIdex;
	kDBRInfo.bGameRuleIdex = pTableInfo->bGameRuleIdex;
	kDBRInfo.bPlayCoutIdex = pTableInfo->bPlayCoutIdex;
	kDBRInfo.bGamePayType = pTableInfo->bGamePayType;

	DBR_CreatePrivate(&kDBRInfo,dwSocketID,pIServerUserItem,pCMDInfo->stHttpChannel);
	return true;
}

//加入私人场
bool PriaveteGame::OnTCPNetworkSubJoinPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Join_Private));
	if(wDataSize!=sizeof(CMD_GR_Join_Private)) return false;

	CMD_GR_Join_Private* pCMDInfo = (CMD_GR_Join_Private*)pData;
	PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomNum);
	ClubGameInfo  *pClubInfo = NULL;

	if (pTableInfo == NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("没有找到该房间，可能房主已经退出。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	if(pTableInfo->dwClubID !=0)
	{
		pClubInfo = getClubInfoByRoomID(pCMDInfo->dwRoomNum,pTableInfo->dwClubID);
	}
	if((pTableInfo!= NULL && pTableInfo->dwClubID !=0) || pClubInfo !=NULL)
	{
		DBR_GR_Join_Club_Rom   pJoinClub;
		ZeroMemory(&pJoinClub,sizeof(DBR_GR_Join_Club_Rom));
		pJoinClub.dwUserID =pIServerUserItem->GetUserID();
		if(pTableInfo!= NULL && pTableInfo->dwClubID !=0) 
		{
			pJoinClub.dwClubID = pTableInfo->dwClubID;
		}
		pJoinClub.dwRoomNum = pCMDInfo->dwRoomNum;
		pJoinClub.dwSeatNum = pCMDInfo->dwSeatNum;
		if(pTableInfo !=NULL &&pTableInfo->pITableFrame->GetTableUserItem(pJoinClub.dwSeatNum) !=NULL)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("该座位已经有玩家加入,请选择其他座位"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
			return false;
		}
		//if(pTableInfo->pITableFrame->dwS)
		if (pTableInfo !=NULL && pTableInfo->bGamePayType == 0) // 房主支付 
		{
			pJoinClub.dwPlayCost = pTableInfo->dwPlayCost;
		}
		m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_JOIN_CLUB_ROM_PRIVATE,dwSocketID,&pJoinClub,sizeof(DBR_GR_Join_Club_Rom));
		return  true;
	}
	if (pTableInfo->pITableFrame->GetNullChairCount() <= 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏人数已满，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	if (pTableInfo->bStart || pTableInfo->bInEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏已经开始，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	if(pTableInfo->bGamePayType == 1)
	{
		int cost = pTableInfo->dwPlayCost / 4;
		if (pTableInfo->cbPlayerNum)
		{
			cost = pTableInfo->dwPlayCost / pTableInfo->cbPlayerNum;
		}
		tagUserInfo* userInfo = pIServerUserItem->GetUserInfo();
		if(userInfo->lInsure < cost)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("房主选择4人支付房费，您钻石不够，请充值。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
			return true;
		}
	}
	int ret = joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame);
	if (ret > 0) 
	{
		if (ret == 2)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("同ip用户不能同时进房。"),SMT_EJECT|SMT_JOIN_ROOM);
			return true;
		}
		else
		{
			if(ret == 0)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("加入房间失败。"),SMT_EJECT|SMT_JOIN_ROOM);
				return true;
			}			
		}		
		CMD_GF_Join_Private_Sucess kSucessInfo;
		kSucessInfo.dwRoomNum = pTableInfo->dwRoomNum;
		kSucessInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));
		return true;
	}
	else
	{
		CMD_GF_Join_Private_Sucess kSucessInfo;
		kSucessInfo.dwRoomNum = pTableInfo->dwRoomNum;
		kSucessInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));
	}
	return true;
}
bool PriaveteGame::OnTCPNetworkSubDismissPrivate(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Dismiss_Private));
	if(wDataSize!=sizeof(CMD_GR_Dismiss_Private)) return false;

	CMD_GR_Dismiss_Private* pCMDInfo = (CMD_GR_Dismiss_Private*)pData;

	if (pCMDInfo->bAgent == 0) // 普通解算
	{
		PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
		if (!pTableInfo)
		{
			return false;
		}
		if (!pTableInfo->bStart && pTableInfo->dwCreaterUserID != pIServerUserItem->GetUserID())
		{
			//return true;
		}
		if (pTableInfo->bInEnd)
		{
			return true;
		}
		if (!pTableInfo)
		{
			return true;
		}
		if (pTableInfo->kDismissChairID.size() == 0 && !pCMDInfo->bDismiss)
		{
			return true;
		}
		if (pTableInfo->fDismissPastTime == 0 && pCMDInfo->bDismiss != 2)
		{
			return true;
		}
		if (pTableInfo->kDismissChairID.size() == 0)
		{
			pTableInfo->fDismissPastTime = 1;
		}
		if(pCMDInfo->bDismiss)
		{
			for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
			{
				if (pTableInfo->kDismissChairID[i] == pIServerUserItem->GetChairID())
				{
					return true;
				}
			}
			pTableInfo->kDismissChairID.push_back(pIServerUserItem->GetChairID());
		}
		else
		{
			for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
			{
				if (pTableInfo->kNotAgreeChairID[i] == pIServerUserItem->GetChairID())
				{
					return true;
				}
			}
			pTableInfo->kNotAgreeChairID.push_back(pIServerUserItem->GetChairID());
		}
		CMD_GF_Private_Dismiss_Info kNetInfo;
		kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
		kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
		for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
		{
			kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
		}
		for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
		{
			kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
		}
		kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);

		bool bClearDismissInfo = false;
		if (pTableInfo->kNotAgreeChairID.size() >= 1)
		{
			bClearDismissInfo = true;
		}

		if (/*!pTableInfo->bStart || */ (int)kNetInfo.dwDissUserCout >= (int)pTableInfo->pITableFrame->GetSitUserCount())
		{
			bClearDismissInfo = true;

			pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
			if(pTableInfo->dwClubID != 0)  
			{
				ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
				if(pClubGame != NULL )
				{
					//pClubGame->DeleteClubRom(pTableInfo);
					pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
				}
			}
			DismissRoom(pTableInfo);

			ClearRoom(pTableInfo);
		}
		if (bClearDismissInfo)
		{
			pTableInfo->kNotAgreeChairID.clear();
			pTableInfo->kDismissChairID.clear();
			pTableInfo->fDismissPastTime = 0;
			kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
			kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
			SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);
		}
	}
	else if (pCMDInfo->bAgent == 1) // 代理解散
	{
		if(pCMDInfo->dwClubID != 0 )  //群主解散俱乐部 
		{
			ClubGameInfo  *pClubInfo = getClubInfoByClubID(pCMDInfo->dwClubID);
			if(pClubInfo )
			{
				map<BYTE,vector<RoomPlayer>>::iterator  iter = pClubInfo->m_mapRoomPlayer.begin();

				for(;iter != pClubInfo->m_mapRoomPlayer.end();iter++)
				{
					vector<RoomPlayer>::iterator iterPlayer = iter->second.begin();
			
					for(;iterPlayer !=iter->second.end();iterPlayer++)
					{
						PrivateTableInfo* pTableInfo = &m_vecTableInfo[iterPlayer->dwTableID] ;
						if (!pTableInfo)
						{
							continue;
						}
						if (pTableInfo->bInEnd)
						{
							continue;
						}
						CMD_GF_Private_Dismiss_Info kNetInfo;
						kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
						kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
						for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
						{
							kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
						}
						for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
						{
							kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
						}
						kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
						SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);

						bool bClearDismissInfo = true;

						pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);

						/*if(pTableInfo->dwClubID != 0)  
						{
							kNetInfo.dwClubID = pTableInfo->dwClubID;
							ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
							if(pClubGame != NULL )
							{
								pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
							}
						}*/

						DismissRoom(pTableInfo);

						ClearRoom(pTableInfo);

						pTableInfo->kNotAgreeChairID.clear();
						pTableInfo->kDismissChairID.clear();
						pTableInfo->fDismissPastTime = 0;
						kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
						kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
					
						SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);
					}

				}
				pClubInfo->ResetValue();
				CMD_GF_Private_Dismiss_Info kNetInfo;
				kNetInfo.dwClubID = pCMDInfo->dwClubID;
				SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
			}
			
		}
		else 
		{
			PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomID);
			if (!pTableInfo)
			{
				return false;
			}
			if (pTableInfo->bInEnd)
			{
				return true;
			}
			CMD_GF_Private_Dismiss_Info kNetInfo;
			kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
			kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
			for (int i = 0;i<(int)pTableInfo->kDismissChairID.size();i++)
			{
				kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
			}
			for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size();i++)
			{
				kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
			}
			kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
			SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);

			bool bClearDismissInfo = true;

			pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);

			if(pTableInfo->dwClubID != 0)  
			{
				ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
				if(pClubGame != NULL )
				{
					pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
				}
			}

			DismissRoom(pTableInfo);

			ClearRoom(pTableInfo);

			pTableInfo->kNotAgreeChairID.clear();
			pTableInfo->kDismissChairID.clear();
			pTableInfo->fDismissPastTime = 0;
			kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
			kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
			SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo), false);

			//发送消息
			pTableInfo->pITableFrame->SendRoomMessage(pIServerUserItem , TEXT("房间已被解散！"),SMT_EJECT|SMT_CHAT);
		}	
	}

	return true;
}
bool PriaveteGame::OnTCPNetworkSubExitSave(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->bStart && !pTableInfo->bInEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏已经开始，无法退出。"),SMT_EJECT);
		return true;
	}
	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && pTableInfo->bInEnd)
	{
		pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
		if(pTableInfo->dwClubID != 0)  
		{
			ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame != NULL )
			{
				pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
			}
		}
		ClearRoom(pTableInfo);
		return true;
	}
	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && !pTableInfo->bInEnd)
	{
		pTableInfo->pITableFrame->PerformStandUpActionEx(pIServerUserItem);
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("您已返回大厅,房间将会继续保留。"),SMT_CLOSE_GAME);
		return true;
	}
	return true;
}
//选择座位
bool PriaveteGame::OnTCPNetworkSubSelectChair(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	ASSERT(wDataSize==sizeof(CMD_GR_Select_Chair_Private));
	if(wDataSize!=sizeof(CMD_GR_Select_Chair_Private)) return false;

	CMD_GR_Select_Chair_Private* pCMDInfo = (CMD_GR_Select_Chair_Private*)pData;
	
	PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomNum);
	if(pTableInfo == NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("加入房间失败。"),SMT_EJECT);
		return false;
	}
	selectRoomChair(pIServerUserItem,pTableInfo->pITableFrame,pCMDInfo->bChairIdex);

	if(pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() )
	{
		pTableInfo->pITableFrame->SetCreateUserID(pTableInfo->dwCreaterUserID);
	}
	return true;
}
bool PriaveteGame::OnTCPNetworkGetChairInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID)
{
	CMD_GR_Get_Room_Chair_Info* pCMDInfo = (CMD_GR_Get_Room_Chair_Info*)pData;
	PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomNum);
	pTableInfo->pITableFrame->GetTableChairInfo(pIServerUserItem);
	return true;
}

bool PriaveteGame::OnTCPNetworkSubMyRoom(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	CMD_GR_Req_My_Room_Info* pCMDInfo = (CMD_GR_Req_My_Room_Info*)pData;
	std::vector<PrivateTableInfo*> vec = getTableInfoByUserID(pCMDInfo->dwUserID);

	if(vec.size() == 0)
	{
		CMD_GF_Ack_My_Room_Info kRoomInfo;
		kRoomInfo.bIsLast = 1;	
		kRoomInfo.dwRoomNum = 0;
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_ACK_MY_ROOM_INFO,&kRoomInfo,sizeof(kRoomInfo));
	}
	else
	{
		for (int i=0;i<vec.size();i++)
		{			
			CMD_GF_Ack_My_Room_Info kRoomInfo;
			PrivateTableInfo* pTableInfo = vec[i];
			kRoomInfo.dwRoomNum = pTableInfo->dwRoomNum;
			kRoomInfo.cbCount= 4-pTableInfo->pITableFrame->GetNullChairCount();								//当前人数
			kRoomInfo.cbState =  pTableInfo->pITableFrame->GetGameStatus();							// 开始状态
			kRoomInfo.bPlayCoutIdex = pTableInfo->bPlayCoutIdex;
			kRoomInfo.bGamePayType = pTableInfo->bGamePayType;
			kRoomInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
			kRoomInfo.bQiangGangHu = pTableInfo->bQiangGangHu;
			kRoomInfo.bGameCardType = pTableInfo->bGameCardType;
			kRoomInfo.bGameTypeIndex = pTableInfo->bGameTypeIndex;
			kRoomInfo.bGameTypeQing = pTableInfo->bGameTypeQing;
			kRoomInfo.bGameMaxScore = pTableInfo->bGameMaxScore;
			kRoomInfo.bPlayerNum = pTableInfo->cbPlayerNum;
			kRoomInfo.bBaoIsOutCard = pTableInfo->bBaoIsOutCard;
			kRoomInfo.bPatherType = pTableInfo->cbPantherType;
			kRoomInfo.bPiaoShu = pTableInfo->bPiaoShu;
			if(i== vec.size() - 1)
			{
				kRoomInfo.bIsLast = 1;
			}
			else
			{
				kRoomInfo.bIsLast = 0;
			}

			SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_ACK_MY_ROOM_INFO,&kRoomInfo,sizeof(kRoomInfo));
		}
		
	}
	
	return true;
}

bool PriaveteGame::OnTCPNetworkGetTableInfo(VOID* pData, WORD wDataSize, IServerUserItem* pIServerUserItem, DWORD dwSocketID)
{
	CMD_GR_Req_Table_Info* pCMDInfo = (CMD_GR_Req_Table_Info*)pData;
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pCMDInfo->dwTableID);

	CMD_GF_Join_Private_Sucess kSucessInfo;
	kSucessInfo.dwRoomNum = pTableInfo->dwRoomNum;
	kSucessInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));
	return true;
}

//私人场事件
bool PriaveteGame::OnEventSocketPrivate(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_PRIVATE_INFO:	//私人场信息
		{
			SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_PRIVATE_INFO,&m_kPrivateInfo,sizeof(m_kPrivateInfo));
			return true;
		}
	case SUB_GR_CREATE_PRIVATE:	//创建私人场
		{
			return OnTCPNetworkSubCreatePrivate(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_RIVATE_AGAIN:	//重新加入私人场
		{
			return OnTCPNetworkSubAgainEnter(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_JOIN_PRIVATE:	//加入私人场
		{
			return OnTCPNetworkSubJoinPrivate(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
	case SUB_GR_PRIVATE_DISMISS:	//解散
		{
			return OnTCPNetworkSubDismissPrivate(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
	case SUB_GR_EXIT_SAVE:	//解散
		{
			return OnTCPNetworkSubExitSave(pData,wDataSize,pIServerUserItem,dwSocketID);	
		}
	case SUB_GR_SELECT_CHAIR: //选座位
		{
			return OnTCPNetworkSubSelectChair(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_GET_TABLE_INFO://在座的玩家信息
		{
			return OnTCPNetworkGetChairInfo(pData,wDataSize,pIServerUserItem,dwSocketID);

		}
	case SUB_GR_GET_SELECT_INFO://刚上线的玩家请求房间信息
		{
			return OnTCPNetworkGetTableInfo(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_GET_MY_ROOM_INFO:	//房间列表
		{
			return OnTCPNetworkSubMyRoom(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_CLUB_ROM_INFO_REQ:  //俱乐部房间信息请求
		{
			return  OnTCPNetWorkSubJoinClubRom(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_CLUB_JOIN_CLUB_ROM_REQ: //加入俱乐部房间请求
		{
			return  OnTCPNetWorkSubIntoClubRoomReq(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_CREATE_FLOOR_STRESS_REQ: //新开楼层请求
		{
			return  OnTCPNetworkCreateFloorStressReq(pData,wDataSize,pIServerUserItem,dwSocketID);
		}
	case SUB_GR_CLUB_UPDATE_FLOOR_REQ: //更新楼层玩法
		{
			return OnTCPNetUpdateClubFloorReq(pData,wDataSize,pIServerUserItem,dwSocketID);
		}

	}
	return true;
}

//重置桌位
void PriaveteGame::DismissRoom(PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo!=NULL);
	if (pTableInfo==NULL) return;

	ITableFrame* pTableFrame=pTableInfo->pITableFrame;
	ASSERT(pTableFrame!=NULL);
	if (pTableFrame==NULL) return;

	if (pTableInfo->bStart)
	{
		CMD_GF_Private_End_Info kNetInfo;
		kNetInfo.lPlayerWinLose.resize(pTableFrame->GetChairCount());
		kNetInfo.lPlayerAction.resize(pTableFrame->GetChairCount()*MAX_PRIVATE_ACTION);
		for (int i = 0;i<pTableFrame->GetChairCount();i++)
		{
			kNetInfo.lPlayerWinLose[i] = pTableInfo->lPlayerWinLose[i];
			for (int m = 0;m<MAX_PRIVATE_ACTION;m++)
			{
				kNetInfo.lPlayerAction[i*MAX_PRIVATE_ACTION+m] = pTableInfo->lPlayerAction[i][m];
			}
		}
		kNetInfo.kPlayTime = pTableInfo->kTotalRecord.kPlayTime;
		datastream kDataStream;
		kNetInfo.StreamValue(kDataStream,true);
		SendTableData(pTableFrame,MDM_GR_PRIVATE,SUB_GF_PRIVATE_END,&kDataStream[0],kDataStream.size());
		
	}
	pTableFrame->TableFrameEnd();
	pTableInfo->bInEnd = true;


	//强制解散游戏
	if (pTableFrame->IsGameStarted()==true)
	{
		bool bSuccess=pTableFrame->DismissGame();
		if (bSuccess==false)
		{
			CTraceService::TraceString(TEXT("PriaveteGame 解散游戏"),TraceLevel_Exception);
			return;
		}
	}

	if (pTableInfo->bStart)
	{

		datastream kDataStream;
		pTableInfo->kTotalRecord.StreamValue(kDataStream,true);

		m_pIDataBaseEngine->PostDataBaseRequest(INVALID_DWORD,DBR_GR_PRIVATE_GAME_RECORD,0,&kDataStream[0],kDataStream.size());	

		pTableInfo->bStart = false;
		sendPrivateRoomInfo(NULL,pTableInfo);
	}
	else
	{
		/*if(pTableInfo->dwClubID != 0)  
		{
			ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame != NULL )
			{
				pClubGame->DeleteClubTable(pTableInfo->dwRoomNum);
			}
		}
		ClearRoom(pTableInfo);*/
	}
	return;
}
void PriaveteGame::ClearRoom(PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo!=NULL);
	if (pTableInfo==NULL) return;

	ITableFrame* pTableFrame=pTableInfo->pITableFrame;
	ASSERT(pTableFrame!=NULL);
	if (pTableFrame==NULL) return;

	//强制解散游戏
	if (pTableFrame->IsGameStarted()==true)
	{
		bool bSuccess=pTableFrame->DismissGame();
		if (bSuccess==false)
		{
			CTraceService::TraceString(TEXT("PriaveteGame 解散游戏"),TraceLevel_Exception);
			return;
		}
	}
	//该桌用户全部离开
	for (int i=0;i<pTableFrame->GetChairCount();i++)
	{
		IServerUserItem* pUserItem=pTableFrame->GetTableUserItem(i);
		if(pUserItem&&pUserItem->GetTableID()!=INVALID_TABLE)
		{
			pTableFrame->PerformStandUpActionEx(pUserItem);
		}
	}
	pTableInfo->pITableFrame->ResAccountInfo();
	pTableInfo->restValue();
}
void PriaveteGame::DBR_CreatePrivate(DBR_GR_Create_Private* kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem,std::string kHttpChannel)
{
	m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_CREATE_PRIVAT,dwSocketID,kInfo,sizeof(DBR_GR_Create_Private));

}
//用户积分
bool PriaveteGame::OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	m_pIServerUserItemSink->OnEventUserItemScore(pIServerUserItem,cbReason);
	return true;
}

//用户状态
bool PriaveteGame::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemStatus(pIServerUserItem,wOldTableID,wOldChairID);
	}

	return true;
}

//用户权限
bool PriaveteGame::OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight)
{
	if(m_pIServerUserItemSink!=NULL)
	{
		return m_pIServerUserItemSink->OnEventUserItemRight(pIServerUserItem,dwAddRight,dwRemoveRight,bGameRight);
	}

	return true;
}

//用户登录
bool PriaveteGame::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	return true;
}

//用户登出
bool PriaveteGame::OnEventUserLogout(IServerUserItem * pIServerUserItem)
{
	//清除俱乐部数据
	if(pIServerUserItem->m_dwClubID != 0 )  
	{
		ClubGameInfo *pClubGame =  getClubInfoByClubID(pIServerUserItem->m_dwClubID);
		if(pClubGame == NULL)  return  true;
		pClubGame->m_dwUserNum --;
		if(pClubGame->m_dwUserNum == 0)   
		{
			pClubGame->ResetValue();
		}
	}
	return true;
}

//进入事件
bool PriaveteGame::OnEventEnterPrivate(DWORD dwSocketID ,VOID* pData,DWORD dwUserIP, bool bIsMobile)
{
	//手机用户
	if(bIsMobile == true)
	{
		//处理消息
		CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
		pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
		pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

		//变量定义
		DBR_GR_LogonMobile LogonMobile;
		ZeroMemory(&LogonMobile,sizeof(LogonMobile));

		//构造数据
		LogonMobile.dwUserID=pLogonMobile->dwUserID;
		LogonMobile.dwClientAddr=dwUserIP;		
		LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
		LogonMobile.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		LogonMobile.wPageTableCount=pLogonMobile->wPageTableCount;		
		lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
		lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));

		//投递请求
		m_pIDataBaseEngine->PostDataBaseRequest(LogonMobile.dwUserID,DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));		
	}
	else
	{
		//处理消息
		CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
		pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
		pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

		//变量定义
		DBR_GR_LogonUserID LogonUserID;
		ZeroMemory(&LogonUserID,sizeof(LogonUserID));

		//构造数据
		LogonUserID.dwClientAddr=dwUserIP;
		LogonUserID.dwUserID=pLogonUserID->dwUserID;
		lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
		lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));

		//投递请求
		m_pIDataBaseEngine->PostDataBaseRequest(LogonUserID.dwUserID,DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));
	}
	return true;
}
bool PriaveteGame::AddPrivateAction(ITableFrame* pTbableFrame,DWORD dwChairID, BYTE	bActionIdex)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pTbableFrame);
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	if (dwChairID >= 100 || bActionIdex >= MAX_PRIVATE_ACTION)
	{
		return true;
	}
	pTableInfo->lPlayerAction[dwChairID][bActionIdex] ++;
	return true;
}

//用户
bool PriaveteGame::OnEventUserJoinPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason,DWORD dwSocketID)
{
	//参数校验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem == NULL) return false;

	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_PRIVATE_INFO,&m_kPrivateInfo,sizeof(m_kPrivateInfo));

	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	//判断状态
	if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		sendPrivateRoomInfo(pIServerUserItem,getTableInfoByTableID(pIServerUserItem->GetTableID()));
	}
	return true;
}

//用户退赛
bool PriaveteGame::OnEventUserQuitPrivate(IServerUserItem * pIServerUserItem, BYTE cbReason, WORD *pBestRank, DWORD dwContextID)
{
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	return true;
}

//游戏开始
bool PriaveteGame::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	pTableInfo->dwStartPlayCout ++;
	pTableInfo->bStart = true;
	pTableInfo->newRandChild();
	if(pTableInfo->dwClubID !=0)
	{
		ClubGameInfo *pClubNode = getClubInfoByClubID(pTableInfo->dwClubID);
		if(pClubNode != NULL)
		{
			pClubNode->UpdateRoomPlayerNum(pTableInfo->pITableFrame->GetNullChairCount(),
				pTableInfo->bFloorIndex,pTableInfo->dwRoomNum);
		}
	}

	sendPrivateRoomInfo(NULL,pTableInfo);
	return true;
}

//游戏结束
bool PriaveteGame::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ASSERT(pITableFrame!=NULL);
	if (pITableFrame==NULL) return false;

	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	if (!pTableInfo)
	{
		return true;
	}
	ZeroMemory(pTableInfo->cbLastOfflineReadyState,sizeof(pTableInfo->cbLastOfflineReadyState));
	if (pTableInfo->cbRoomType == Type_Private)
	{
		if (pTableInfo->dwFinishPlayCout == 0
			&& !pTableInfo->bInEnd)
		{
			CreatePrivateCost(pTableInfo);
		}
		pTableInfo->dwFinishPlayCout ++;
		sendPrivateRoomInfo(NULL,pTableInfo);
		if (pTableInfo->dwFinishPlayCout >= pTableInfo->dwPlayCout)
		{
			DismissRoom(pTableInfo);
		}
	}
	else if (pTableInfo->cbRoomType == Type_Public)
	{
		CreatePrivateCost(pTableInfo);
		pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
		if(pTableInfo->dwClubID != 0)  
		{
			ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame != NULL )
			{
				pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
			}
		}
		DismissRoom(pTableInfo);
		ClearRoom(pTableInfo);
	}
	return true;
}
bool PriaveteGame::WriteTableScore(ITableFrame* pITableFrame,tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& kData)
{
	PrivateTableInfo* pTableInfo = getTableInfoByTableFrame(pITableFrame);
	if (!pTableInfo)
	{
		return true;
	}
	pTableInfo->writeSocre( ScoreInfoArray,wScoreCount,kData);

	return true;
}
void PriaveteGame::sendPrivateRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo* pTableInfo)
{
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return;
	}

	CMD_GF_Private_Room_Info kNetInfo;
	kNetInfo.cbRoomType = pTableInfo->cbRoomType;
	kNetInfo.bStartGame = pTableInfo->bStart;
	kNetInfo.dwRoomNum = pTableInfo->dwRoomNum;
	kNetInfo.dwPlayCout = pTableInfo->dwStartPlayCout;
	kNetInfo.dwCreateUserID = pTableInfo->dwCreaterUserID;
	kNetInfo.bGameRuleIdex = pTableInfo->bGameRuleIdex;
	kNetInfo.bGameTypeIdex = pTableInfo->bGameTypeIdex;
	kNetInfo.bPlayCoutIdex = pTableInfo->bPlayCoutIdex;
	kNetInfo.bGamePayType = pTableInfo->bGamePayType;
	kNetInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
	kNetInfo.bGameGuDingMa = pTableInfo->bGameGuDingMa;
	kNetInfo.bQiangGangHu = pTableInfo->bQiangGangHu;
	kNetInfo.bGameCardType = pTableInfo->bGameCardType;
	kNetInfo.dwPlayTotal = pTableInfo->dwPlayCout;
	kNetInfo.cbPlayerNum = 4-pTableInfo->pITableFrame->GetNullChairCount();
	kNetInfo.kWinLoseScore.resize(pTableInfo->pITableFrame->GetChairCount());
	for (WORD i = 0;i<pTableInfo->pITableFrame->GetChairCount();i++)
	{
		kNetInfo.kWinLoseScore[i] =  (int)pTableInfo->lPlayerWinLose[i];
	}

	datastream kDataStream;
	kNetInfo.StreamValue(kDataStream,true);
	
	if (pIServerUserItem)
	{
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GF_PRIVATE_ROOM_INFO,&kDataStream[0],kDataStream.size());
	}
	else
	{
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GF_PRIVATE_ROOM_INFO,&kDataStream[0],kDataStream.size());
	}

	if(pIServerUserItem && pIServerUserItem->GetUserStatus() == US_SELECT)
	{
		CMD_GR_Notify_Room_Select_States kStatesInfo;

		kStatesInfo.dwTableID = pIServerUserItem->GetTableID();

		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_NOTIFY_GET_PRIVATE,&kStatesInfo,sizeof(kStatesInfo));
	}
}
//断线
bool PriaveteGame::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return false;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return false;
	}
	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && !pTableInfo->bInEnd)
	{
		return true;
	}
// 	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && pTableInfo->bInEnd)
// 	{
// 		pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
// 		ClearRoom(pTableInfo);
// 	}
	if (pTableInfo->bInEnd)
	{
		pTableInfo->pITableFrame->PerformStandUpActionEx(pIServerUserItem);
	}
	return true;
}
//用户坐下
bool PriaveteGame::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{ 
// 	if (!pIServerUserItem)
// 	{
// 		return true;
// 	}
// 	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
// 	if (!pTableInfo)
// 	{
// 		return true;
// 	}
// 	sendPrivateRoomInfo(pIServerUserItem,pTableInfo);

	return true; 
}

//用户起立
bool PriaveteGame::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//移除分组
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->bInEnd)
	{
		return false;
	}
	if (wChairID >= MAX_CHAIR)
	{
		return false;
	}
	pTableInfo->cbLastOfflineReadyState[wChairID] = 0;
	return true;
}

 //用户同意
bool PriaveteGame::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{ 
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->bInEnd)
	{
		return false;
	}
	if (wChairID >= MAX_CHAIR)
	{
		return false;
	}
	pTableInfo->cbLastOfflineReadyState[wChairID] = 1;
	return true; 
}

bool PriaveteGame::OnEventReqStandUP(IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	ASSERT(pTableInfo);
	if (!pTableInfo)
	{
		return true;
	}
	if (pTableInfo->bStart && !pTableInfo->bInEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏已经开始，无法退出。"),SMT_EJECT);
		return true;
	}
	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && !pTableInfo->bInEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("您已返回大厅,房间将会继续保留。"),SMT_CLOSE_GAME);
		return true;
	}
	if (pTableInfo->dwCreaterUserID == pIServerUserItem->GetUserID() && pTableInfo->bInEnd)
	{
		pTableInfo->pITableFrame->SendGameMessage(TEXT("房间已被解散！"),SMT_EJECT);
		if(pTableInfo->dwClubID != 0)  
		{
			ClubGameInfo  *pClubGame =  getClubInfoByClubID(pTableInfo->dwClubID);
			if(pClubGame != NULL )
			{
				pClubGame->DeleteClubTable(pTableInfo->dwRoomNum,pTableInfo->bFloorIndex);
			}
		}
		ClearRoom(pTableInfo);
		return true;
	}
	pTableInfo->pITableFrame->PerformStandUpActionEx(pIServerUserItem);
	return true;
}
bool PriaveteGame::OnEventClientReady(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	ASSERT(pIServerUserItem);
	if (!pIServerUserItem)
	{
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByTableID(pIServerUserItem->GetTableID());
	if (!pTableInfo)
	{
		return true;
	}
	sendPrivateRoomInfo(pIServerUserItem,pTableInfo);

	if (pTableInfo->pITableFrame->GetGameStatus() == GAME_STATUS_FREE)
	{
		if (wChairID < MAX_CHAIR && pTableInfo->cbLastOfflineReadyState[wChairID])
		{
			pIServerUserItem->SetUserStatus(US_READY,
				pTableInfo->pITableFrame->GetTableID(),wChairID);
		}
		else
		{
			pIServerUserItem->SetUserStatus(US_SIT,
				pTableInfo->pITableFrame->GetTableID(),wChairID);
		}
	}

	if (pTableInfo->kDismissChairID.size())
	{
		CMD_GF_Private_Dismiss_Info kNetInfo;
		kNetInfo.dwDissUserCout = pTableInfo->kDismissChairID.size();
		kNetInfo.dwNotAgreeUserCout = pTableInfo->kNotAgreeChairID.size();
		for (int i = 0;i<(int)pTableInfo->kDismissChairID.size() && i < MAX_CHAIR -1;i++)
		{
			kNetInfo.dwDissChairID[i] = pTableInfo->kDismissChairID[i];
		}
		for (int i = 0;i<(int)pTableInfo->kNotAgreeChairID.size() && i < MAX_CHAIR -1;i++)
		{
			kNetInfo.dwNotAgreeChairID[i] = pTableInfo->kNotAgreeChairID[i];
		}
		kNetInfo.dwValue2 = DISMISS_WAITE_TIME - (DWORD)pTableInfo->fDismissPastTime;
		SendTableData(pTableInfo->pITableFrame,MDM_GR_PRIVATE,SUB_GR_PRIVATE_DISMISS,&kNetInfo,sizeof(kNetInfo));
	}

	return true;
}

// 
bool PriaveteGame::OnTCPNetWorkSubJoinClubRom(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Join_Club_Rom));
	if(wDataSize!=sizeof(CMD_GR_Join_Club_Rom)) return false;

	CMD_GR_Join_Club_Rom* pCMDInfo = (CMD_GR_Join_Club_Rom*)pData;  

	DBR_GR_Join_Club_Rom  kClubRom ;
	ZeroMemory(&kClubRom,sizeof(kClubRom));
	kClubRom.dwUserID = pCMDInfo->dwUserId;
	kClubRom.dwClubID = pCMDInfo->dwClubId;
	kClubRom.dwUpdateStatus =  pCMDInfo->dwUpdateStatus;
	DBR_JoinClubRom(&kClubRom,dwSocketID,pIServerUserItem);
	return true;

}
void PriaveteGame::DBR_JoinClubRom(DBR_GR_Join_Club_Rom *kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem)
{
	m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_JOIN_CLUB_ROM,dwSocketID,kInfo,sizeof(DBR_GR_Join_Club_Rom));
}

//获取俱乐部房间信息
bool PriaveteGame::OnEnventJoinClubRom(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//开始发送俱乐部列表
	if(pIServerUserItem == NULL) return true;
	if(wDataSize > sizeof(DBR_GR_Join_Club_RomResult)) return false;
	DBR_GR_Join_Club_RomResult * pClubRominfo = (DBR_GR_Join_Club_RomResult *) pData;
	time_t iTimeNow = time(NULL);
	if(pClubRominfo->bSucess != 0 )
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("非该俱乐部成员，无法进入俱乐部"),SMT_EJECT);
		return true;
	}
	if (iTimeNow >= pClubRominfo->dwRepairTimeBegin && iTimeNow <= pClubRominfo->dwRepairTimeEnd)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏正在维护中！"),SMT_EJECT);
		return true;
	}
	ClubGameInfo *pClubInfo = getClubInfoByClubID(pClubRominfo->dwClubID);  //判断该俱乐部是否在游戏场中有房间
	if(pClubInfo == NULL) 
	{
		pClubInfo = getClubInfoIsNullClub();  //获取一个新的俱乐部
		//if(pClubInfo != NULL)
		if(pClubInfo == NULL)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("没有可用的俱乐部，所有俱乐部已被占用"),SMT_EJECT);
			return true;
		}
		pClubInfo->ClubID = pClubRominfo->dwClubID;
		pClubInfo->m_dwManagerID = pClubRominfo->dwManageId;
		pClubInfo->m_dwMasterID = pClubRominfo->dwMasterID;
	}

	//新建楼层  
	std::vector<RoomInfo>::iterator  iterRoomInfo  = pClubRominfo->vecRoomInfo.begin(); 
	for(; iterRoomInfo !=pClubRominfo->vecRoomInfo.end(); iterRoomInfo++)
	{
		std::map<BYTE, FloorStress>::iterator  iterFloorStress = pClubInfo->m_mapFloorStress.find(iterRoomInfo->bFloorIndex);
		if(iterFloorStress ==  pClubInfo->m_mapFloorStress.end())  //新建一个楼层
		{			
			FloorStress  tFloorStress;
			RoomInfo     tRoomInfo =  *iterRoomInfo;
			ZeroMemory(&tFloorStress,sizeof(FloorStress));
			memcpy(&tFloorStress.lGameInfo,&tRoomInfo.lGameInfo,sizeof(tRoomInfo.lGameInfo));
			tFloorStress.dwCreateUser = tRoomInfo.dwCreateUser;
			pClubInfo->m_mapFloorStress.insert(std::pair<BYTE, FloorStress>(iterRoomInfo->bFloorIndex,tFloorStress));
		//	pClubInfo->m_mapFloorStress.insert(std::pair<BYTE, FloorStress>(iterRoomInfo->bFloorIndex+1,tFloorStress));
		}
	}
	
	//新建房间
	std::map<BYTE, FloorStress>::iterator  iterFloor = pClubInfo->m_mapFloorStress.begin();

	for(;iterFloor!=pClubInfo->m_mapFloorStress.end(); iterFloor++)
	{
		bool   isCreateOne = true;   //是否创建
		int	   iRandNum = 0;         //房号
		WORD   wTableID  =  0;
		ITableFrame * pICurrTableFrame=NULL;
		PrivateTableInfo* pCurrTableInfo=NULL;
		std::map<BYTE,std::vector<RoomPlayer>>::iterator  iterPlayer  =  pClubInfo->m_mapRoomPlayer.find(iterFloor->first);
		if(iterPlayer == pClubInfo->m_mapRoomPlayer.end())
		{
		
		}
		else 
		{
			std::vector<RoomPlayer>::iterator iterRoom = iterPlayer->second.begin();
			for(;iterRoom !=iterPlayer->second.end();iterRoom++)
			{
				if(iterRoom->bLeftChair == iterFloor->second.lGameInfo.bPlayerNum)
				{
					isCreateOne = false;
				}
			}
		}	
		if(isCreateOne == true)   //创建房间
		{
			for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
			{
				//获取对象
				ASSERT(m_vecTableInfo[wTableID].pITableFrame!=NULL);
				PrivateTableInfo* pPrivateInfo = &m_vecTableInfo[wTableID];
				ITableFrame * pITableFrame = pPrivateInfo->pITableFrame;
				if(m_vecTableInfo[wTableID].bInEnd)
				{
					continue;
				}
				//状态判断
				if (pPrivateInfo->dwRoomNum == 0) //俱乐部新增房间
				{
					pICurrTableFrame = pITableFrame;
					pCurrTableInfo = &m_vecTableInfo[wTableID];
					pCurrTableInfo->restValue();
					pCurrTableInfo->dwClubID = pClubInfo->ClubID;
					break;
				}
			}
			//桌子判断
			if(pICurrTableFrame==NULL)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败。"),SMT_EJECT|SMT_CREATE_ROOM);
				continue;
			}
			while (iRandNum == 0 || getTableInfoByRoomID(iRandNum) || iRandNum == m_iRoomNumId)
			{
				iRandNum = 1000 + rand() % 8900;
				if(m_pGameServiceOption->wServerID >= 90)
				{
					int count = m_pGameServiceOption->wServerID / 90;
					iRandNum = (m_pGameServiceOption->wServerID - (90 * count) + 10) * 10000 + iRandNum;
				}
				else
					iRandNum = (m_pGameServiceOption->wServerID + 10) * 10000 + iRandNum;
			}
			m_iRoomNumId = iRandNum;
			pICurrTableFrame = pCurrTableInfo->pITableFrame;
			pCurrTableInfo->fEmptyPastTime =0;
			pCurrTableInfo->fDismissUnStartTime=0;
			pCurrTableInfo->setRoomNum(iRandNum,pClubInfo->ClubID);
			pCurrTableInfo->dwFinishPlayCout = 0;
			pCurrTableInfo->dwClubID = pClubInfo->ClubID;
			pCurrTableInfo->bFloorIndex = iterFloor->first;
			pCurrTableInfo->bGameTypeIndex = iterFloor->second.lGameInfo.bGameTypeIndex;
			pCurrTableInfo->bGameTypeQing = iterFloor->second.lGameInfo.bGameTypeQing;
			pCurrTableInfo->dwPlayCout =   m_kPrivateInfo.bPlayCout[iterFloor->second.lGameInfo.bPlayCoutIdex];;
			pCurrTableInfo->bGamePayType = iterFloor->second.lGameInfo.bGamePayType;
			pCurrTableInfo->bQiangGangHu = iterFloor->second.lGameInfo.bQiangGangHu;
			pCurrTableInfo->bAllOther = iterFloor->second.lGameInfo.bAllOther;
			pCurrTableInfo->bGameMaxScore = iterFloor->second.lGameInfo.bGameMaxScore;
			pCurrTableInfo->cbPantherType = iterFloor->second.lGameInfo.bPatherType;
			pCurrTableInfo->cbPlayerNum = iterFloor->second.lGameInfo.bPlayerNum;
			pCurrTableInfo->bGameBaseScore = iterFloor->second.lGameInfo.bGameBaseScore;
			pCurrTableInfo->bBaoIsOutCard = iterFloor->second.lGameInfo.bBaoIsOutCard;
			pCurrTableInfo->bPiaoShu =iterFloor->second.lGameInfo.bPiaoShu;
			pCurrTableInfo->dwCreaterUserID = iterFloor->second.dwCreateUser;
			GetLocalTime(&pCurrTableInfo->kTotalRecord.kPlayTime);
			pCurrTableInfo->bGameRuleIdex = 906;
			if(pCurrTableInfo->dwPlayCout == 16)
			{
				if(pCurrTableInfo->bGameTypeIndex == 3)
				{
					if (pCurrTableInfo->bGameMaxScore == 20)
					{
						pCurrTableInfo->dwPlayCost = 8;
					}
					else if (pCurrTableInfo->bGameMaxScore == 30)
					{
						pCurrTableInfo->dwPlayCost = 12;
					}
					else
					{
						pCurrTableInfo->dwPlayCost = 16;
					}
				}
				else if (pCurrTableInfo->bGameTypeIndex == 2 && pCurrTableInfo->bGameMaxScore == 50)
				{
					pCurrTableInfo->dwPlayCost = 16;
				}
				else
					pCurrTableInfo->dwPlayCost = 12;
			}
			else
			{
				pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lPlayCost[iterFloor->second.lGameInfo.bPlayCoutIdex];
			}
			pICurrTableFrame->SetPrivateInfo(pCurrTableInfo->bGameTypeIndex,pCurrTableInfo->bGameRuleIdex,pCurrTableInfo->bGameBaseScore,
				pCurrTableInfo->bGameGuDingMa,pCurrTableInfo->bQiangGangHu,pCurrTableInfo->bGameCardType,pCurrTableInfo->bLimitIP);

			pICurrTableFrame->SetPrivateFushunInfo(pCurrTableInfo->bGameTypeIndex,pCurrTableInfo->bGameTypeQing,pCurrTableInfo->bLimitIP,
				pCurrTableInfo->bGameMaxScore,pCurrTableInfo->bGameMaxScoreGold,pCurrTableInfo->bPiaoShu,pCurrTableInfo->bBaoIsOutCard,
				pCurrTableInfo->bFendDing,pCurrTableInfo->bJingDing,pCurrTableInfo->bAllOther, pCurrTableInfo->cbPantherType,pCurrTableInfo->cbPlayerNum,pCurrTableInfo->dwClubID);
			pClubInfo->AddClubRom(iterFloor->first,wTableID,iRandNum,pIServerUserItem->GetUserID(),iterFloor->second.lGameInfo.bPlayerNum);
			sendPrivateRoomInfo(NULL,pCurrTableInfo);
		}

	}
	std::map<BYTE, FloorStress>::iterator  itermapFloor = pClubInfo->m_mapFloorStress.begin();

	if(pClubInfo->m_mapFloorStress.size() == 0)
	{
		CMD_GF_Ack_My_Room_Info kRoomInfo;
		kRoomInfo.bIsLast = 1;	
		kRoomInfo.dwRoomNum = 0;
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_CLUB_ROOM_INFO_RES,&kRoomInfo,sizeof(kRoomInfo));
	}
	else 
	{
		BYTE   index  = 0;
		//char   pFloorBuffer[4000];
		//ZeroMemory(pFloorBuffer,sizeof(pFloorBuffer));
		for(;itermapFloor!=pClubInfo->m_mapFloorStress.end(); itermapFloor++)
		{
			std::map<BYTE,std::vector<RoomPlayer>>::iterator iter = pClubInfo->m_mapRoomPlayer.find(itermapFloor->first);
			std::vector<RoomPlayer>::iterator   vecRoomPlayer = iter->second.begin();
			for(;vecRoomPlayer!=iter->second.end();vecRoomPlayer++)
			{
				if(vecRoomPlayer->dwTableID < 0 || vecRoomPlayer->dwTableID >=500 )  continue;
				PrivateTableInfo* pTableInfo = &m_vecTableInfo[vecRoomPlayer->dwTableID];
				if(pTableInfo !=NULL && pTableInfo->dwRoomNum == vecRoomPlayer->dwRoomNum 
					&& pTableInfo->dwClubID == pClubRominfo->dwClubID)
				{
				
					//memcpy(pRoomPlayerBuffer+(index-1)*sizeof(kRoomInfo),&kRoomInfo,sizeof(kRoomInfo));
					//kFloorRoom.bSendRoomNum++;
					index++;
					OnNotifyClubRoomInfo(pIServerUserItem,pTableInfo,iter->first,index == pClubInfo->clubRomNum);
					
				}
			}	
		}
		//发送楼层信息
	}
	return true;
}

//进入俱乐部房间回应
bool PriaveteGame::OnTCPNetWorkIntoClubRoomRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//参数效验
	ASSERT(wDataSize==sizeof(DBR_GR_Into_Club_Table_Res));
	if(wDataSize!=sizeof(DBR_GR_Into_Club_Table_Res)) return false;

	DBR_GR_Into_Club_Table_Res *pCMDInfo = (DBR_GR_Into_Club_Table_Res *) pData;
	if (pCMDInfo->bSucess == 1)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("游戏正在维护中！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	else if (pCMDInfo->bSucess == 2)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("俱乐部房间错误！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	else if (pCMDInfo->bSucess == 3)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("房主钻石不够，不能进入房间！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	PrivateTableInfo *pTableInfo = getTableInfoByRoomID(pCMDInfo->dwRoomNum);

	if(pTableInfo == NULL)  //找不到桌子
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，房间不存在，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);				
		return  true;
	}
	else 
	{
		ClubGameInfo *pClubGame = getClubInfoByClubID(pTableInfo->dwClubID);
		if(pClubGame ==NULL)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，该房间在俱乐部中还没创建，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
			return true;
		}
		else
		{
			if (pTableInfo->pITableFrame->GetNullChairCount() <= 0)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏人数已满，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
				return true;
			}
			if (pTableInfo->bStart || pTableInfo->bInEnd)
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏已经开始，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
				return true;
			}
			if(pTableInfo->bGamePayType == 1)
			{
				int cost = pTableInfo->dwPlayCost / 4;
				tagUserInfo* userInfo = pIServerUserItem->GetUserInfo();
				if(userInfo->lInsure < cost)
				{
					m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("房主选择4人支付房费，您钻石不够，请充值。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
					return true;
				}
			}
			if (!joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame))
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("此游戏桌存在与您 IP 地址相同的玩家，不能加入游戏！"),SMT_EJECT|SMT_JOIN_ROOM);
				return true;
			}
			else 
			{
				if(pTableInfo->dwClubID !=0 )
				{
					ClubGameInfo  *pClubGame = getClubInfoByClubID(pTableInfo->dwClubID);
					if(pClubGame !=NULL) 
					{
						pClubGame->UpdateRoomPlayerNum(pTableInfo->pITableFrame->GetNullChairCount(),
						pTableInfo->bFloorIndex,pTableInfo->dwRoomNum);
					}
				}
			}
		}
		//m_vecUserItem.push_back(pIServerUserItem);
		CMD_GR_Into_Club_Table_Res kSucessInfo;
		kSucessInfo.dwRoomNum = pTableInfo->dwRoomNum;
		kSucessInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
		SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_CLUB_INTO_CLUB_TABLE_RES,&kSucessInfo,sizeof(kSucessInfo));
		//SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));
		return true;
	}	
	return  true;

}

//通过私人场方式加入俱乐部房间判断
bool PriaveteGame::OnEnventClubRomPriavteRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//开始发送俱乐部列表
	if(pIServerUserItem == NULL) return true;
	if(wDataSize > sizeof(DBR_GR_Club_RomPrivateRes)) return false;
	DBR_GR_Club_RomPrivateRes* pClubRomRes = (DBR_GR_Club_RomPrivateRes *) pData;	
	if (pClubRomRes->bSucess == 2)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("加入俱乐部房间错误！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	else if (pClubRomRes->bSucess == 3)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("房主的钻石不够，不能进入房间！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	else if(pClubRomRes->bSucess !=0 )
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，非俱乐部房间成员，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	PrivateTableInfo* pTableInfo = getTableInfoByRoomID(pClubRomRes->dwTableID);
	if(pTableInfo == NULL )
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，没有找到该房间，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
	}
	if (pTableInfo !=NULL && pTableInfo->pITableFrame->GetNullChairCount() <= 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏人数已满，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	if (pTableInfo !=NULL && (pTableInfo->bStart || pTableInfo->bInEnd))
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起，游戏已经开始，无法加入。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	if(pTableInfo !=NULL && pTableInfo->bGamePayType == 1)
	{
		int cost = pTableInfo->dwPlayCost / 4;
		tagUserInfo* userInfo = pIServerUserItem->GetUserInfo();
		if(userInfo->lInsure < cost)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("房主选择4人支付房费，您钻石不够，请充值。"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
			return true;
		}
	}
	if(pClubRomRes->dwClubID !=0 && pClubRomRes->dwClubID!= pTableInfo->dwClubID )
	{
	
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("对不起,加入俱乐部房间错误！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	else 
	{
		ClubGameInfo  *pClubGame = getClubInfoByClubID(pTableInfo->dwClubID);
		if(pClubGame == NULL || pClubGame->GetTableByRoomNum(pTableInfo->dwRoomNum)== 0)
		{
			m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("对不起,加入俱乐部房间错误！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
			return true;
		}
	}
	if (!joinPrivateRoom(pIServerUserItem,pTableInfo->pITableFrame,pClubRomRes->dwSeatNum))
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("此游戏桌存在与您 IP 地址相同的玩家，不能加入游戏！"),SMT_EJECT|SMT_JOIN_ROOM);
		return true;
	}

	CMD_GF_Join_Private_Sucess kSucessInfo;
	kSucessInfo.dwRoomNum = pTableInfo->dwRoomNum;
	kSucessInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_JOIN_PRIVATE_SUCESS,&kSucessInfo,sizeof(kSucessInfo));
	return true;
}



//进入俱乐部房间请求
bool PriaveteGame::OnTCPNetWorkSubIntoClubRoomReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数效验
	ASSERT(wDataSize==sizeof(CMD_GR_Into_Club_Table_Req));
	if(wDataSize!=sizeof(CMD_GR_Into_Club_Table_Req)) return false;

	CMD_GR_Into_Club_Table_Req* pCMDInfo = (CMD_GR_Into_Club_Table_Req*) pData;  

	// 检查当前是否可以进入俱乐部房间
	DBR_GR_Into_Club_Table_Req  kInfo;
	ZeroMemory(&kInfo, sizeof(kInfo));
	kInfo.dwRoomNum = pCMDInfo->dwRoomNum;
	m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_CLUB_JOIN_CLUB_ROM_REQ,dwSocketID,&kInfo,sizeof(DBR_GR_Into_Club_Table_Req));
	return  true;
}
bool PriaveteGame::OnTCPNetworkCreateFloorStressReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
		//效验参数
	ASSERT(wDataSize == sizeof(SUB_GR_Create_Floor_Stress));	
	if (wDataSize!= sizeof(SUB_GR_Create_Floor_Stress)) return false; 

	//处理消息
	SUB_GR_Create_Floor_Stress * pNetInfo = (SUB_GR_Create_Floor_Stress *) pData;
	//变量定义
	DB_GR_Create_Floor_Stress kDBInfo;
	ZeroMemory(&kDBInfo, sizeof(SUB_GR_Create_Floor_Stress));
	//构造数据
	kDBInfo.dwUserID = pNetInfo->dwUserID;
	if(kDBInfo.dwUserID  != pIServerUserItem->GetUserID())  
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem, TEXT("对不起,创建楼层参数错误！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return true;
	}
	kDBInfo.dwClubID = pNetInfo->dwClubID;
	kDBInfo.lGameInfo.bGameTypeIndex = pNetInfo->bGameTypeIndex;
	kDBInfo.lGameInfo.bGameTypeQing = pNetInfo->bGameTypeQing;
	kDBInfo.lGameInfo.bPlayCoutIdex = pNetInfo->bPlayCoutIdex;
	kDBInfo.lGameInfo.bQiangGangHu = pNetInfo->bQiangGangHu;
	kDBInfo.lGameInfo.bAllOther = pNetInfo->bAllOther;
	kDBInfo.lGameInfo.bGameMaxScore = pNetInfo->bMaxScore;
	kDBInfo.lGameInfo.bPatherType = pNetInfo->bPantherType;
	kDBInfo.lGameInfo.bBaoIsOutCard = pNetInfo->bBaoIsOutCard;
	kDBInfo.lGameInfo.bPiaoShu = pNetInfo->bPiaoShu;
	kDBInfo.lGameInfo.bPlayerNum = pNetInfo->bPlayerNum;
	kDBInfo.lGameInfo.bGameBaseScore = pNetInfo->bGameBaseScore;
	DBR_CreateClubFloor(&kDBInfo,dwSocketID,pIServerUserItem);
	return true;
}
void PriaveteGame::DBR_CreateClubFloor(DB_GR_Create_Floor_Stress *kInfo,DWORD dwSocketID,IServerUserItem* pIServerUserItem)
{
	m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_CREATE_FLOOR_STRESS_REQ,dwSocketID,kInfo,sizeof(DB_GR_Create_Floor_Stress));
}
//更新俱乐部楼层请求
bool PriaveteGame::OnTCPNetUpdateClubFloorReq(VOID  *pData,WORD wDataSize,IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//参数校验
	ASSERT(wDataSize==sizeof(CMD_GF_Update_Floor_Stress));
	if(wDataSize!=sizeof(CMD_GF_Update_Floor_Stress)) return false;
	if(pIServerUserItem == NULL)  return false;
	CMD_GF_Update_Floor_Stress *pCMDInfo  = (CMD_GF_Update_Floor_Stress *) pData;

	if(pCMDInfo->dwClubID <=0 || pCMDInfo->bFloorIndex > 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("更新楼层信息错误"),SMT_EJECT|SMT_CREATE_ROOM);
		return true;
	}
	DBR_GR_Update_Club_Floor_Req  UpdateClub;
	ZeroMemory(&UpdateClub,sizeof(DBR_GR_Update_Club_Floor_Req));
	UpdateClub.bFloorIndex = pCMDInfo->bFloorIndex;
	UpdateClub.dwClubID  = pCMDInfo->dwClubID;
	UpdateClub.dwUserID  = pIServerUserItem->GetUserID();
	UpdateClub.lGameInfo.bAllOther =  pCMDInfo->bAllOther;
	UpdateClub.lGameInfo.bGameTypeIndex =  pCMDInfo->bGameTypeIndex;
	UpdateClub.lGameInfo.bGameTypeQing =  pCMDInfo->bGameTypeQing;
	UpdateClub.lGameInfo.bPlayCoutIdex =  pCMDInfo->bPlayCoutIdex;
	UpdateClub.lGameInfo.bGamePayType =  pCMDInfo->bGamePayType;
	UpdateClub.lGameInfo.bGameBaseScore =  pCMDInfo->bGameBaseScore;
	UpdateClub.lGameInfo.bQiangGangHu =  pCMDInfo->bQiangGangHu;
	UpdateClub.lGameInfo.bGameMaxScore =  pCMDInfo->bGameMaxScore;
	UpdateClub.lGameInfo.bPiaoShu =  pCMDInfo->bPiaoShu;
	UpdateClub.lGameInfo.bBaoIsOutCard =  pCMDInfo->bBaoIsOutCard;
	UpdateClub.lGameInfo.bPatherType =  pCMDInfo->bPatherType;
	m_pIDataBaseEngine->PostDataBaseRequest(pIServerUserItem->GetUserID(),DBR_GR_CLUB_UPDATE_FLOOR_REQ,dwSocketID,&UpdateClub,sizeof(DBR_GR_Update_Club_Floor_Req));

	return  true;

}
//俱乐部创建私人场 
void  	PriaveteGame::OnSubCreateOneClubRoom(IServerUserItem * pIServerUserItem,DWORD  dwClubID,BYTE bFloorIndex,DWORD dwMasterID,DWORD dwManagerID)
{
	
	ClubGameInfo  *pClubGameInfo =  getClubInfoByClubID(dwClubID);
	if(pClubGameInfo == NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起,新增楼层失败,没有找到该俱乐部"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		//return  true;
	}
	WORD  wTableId  = 500;
	if(OnTCPNetCreateClubRoom(pClubGameInfo, wTableId,bFloorIndex))
	{
		if( wTableId > 500 )  return ;
		PrivateTableInfo  *pTableNode =  &m_vecTableInfo[wTableId];
		if(pTableNode == NULL)  return;
		if(pTableNode->bGamePayType ==1)
		{
			if(!joinPrivateRoom(pIServerUserItem,pTableNode->pITableFrame))
			{
				m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败。"),SMT_EJECT|SMT_CREATE_ROOM);
				return ;
			}
		}
		else 
		{
			OnNotifyClubRoomInfo(pIServerUserItem,pTableNode,bFloorIndex,true);
		
		}
	}



	//sendPrivateRoomInfo(NULL,pCurrTableInfo);
}
//创建楼层
bool PriaveteGame::OnTCPNetCreateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//参数效验
	ASSERT(wDataSize==sizeof(DB_GR_Create_Floor_Stress_Res));
	if(wDataSize!=sizeof(DB_GR_Create_Floor_Stress_Res)) return false;

	DB_GR_Create_Floor_Stress_Res *pCMDInfo = (DB_GR_Create_Floor_Stress_Res *) pData;
	if (pCMDInfo->bSucess != 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,pCMDInfo->strErrorDecrible,SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	ClubGameInfo  *pClubGameInfo =  getClubInfoByClubID(pCMDInfo->dwClubID);
	if(pClubGameInfo == NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起,新增楼层失败,没有找到该俱乐部"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	map<BYTE,FloorStress>::iterator  iter = pClubGameInfo->m_mapFloorStress.find(pCMDInfo->bFloorIndex);
	if(iter != pClubGameInfo->m_mapFloorStress.end())
	{
		//
	}
	WORD  wTableId  = 500;
	FloorStress   kFloorstress;
	ZeroMemory(&kFloorstress,sizeof(kFloorstress));
	memcpy(&kFloorstress.lGameInfo,&pCMDInfo->lGameInfo,sizeof(pCMDInfo->lGameInfo));
	pClubGameInfo->m_mapFloorStress.insert(pair<BYTE,FloorStress>(pCMDInfo->bFloorIndex,kFloorstress));
	if(OnTCPNetCreateClubRoom(pClubGameInfo,wTableId,pCMDInfo->bFloorIndex) == true)
	{
		//发送房间列表
		PrivateTableInfo  *pTableNode =  &m_vecTableInfo[wTableId];
		if(pTableNode == NULL)  return  true;
		OnNotifyClubRoomInfo(pIServerUserItem,pTableNode,pCMDInfo->bFloorIndex,true);
	}
	return true;
}
//更新俱乐部房间回应
bool PriaveteGame::OnTCPNetUpdateClubFloorRes(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize,std::string kChannel)
{
	//参数效验
	ASSERT(wDataSize==sizeof(DBR_GR_Update_Club_Floor_Res));
	if(wDataSize!=sizeof(DBR_GR_Update_Club_Floor_Res)) return false;

	DBR_GR_Update_Club_Floor_Res *pCMDInfo = (DBR_GR_Update_Club_Floor_Res *) pData;
	if (pCMDInfo->bSucess != 0)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起,修改玩法失败！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	ClubGameInfo  *pClubGameInfo =  getClubInfoByClubID(pCMDInfo->dwClubID);
	if(pClubGameInfo == NULL)
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起,修改玩法失败,没有找到该俱乐部！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	map<BYTE,FloorStress>::iterator  iter = pClubGameInfo->m_mapFloorStress.find(pCMDInfo->bFloorIndex);
	if(iter == pClubGameInfo->m_mapFloorStress.end())
	{
		m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("对不起,修改玩法失败,没有找到该楼层！"),SMT_EJECT|SMT_CHAT|SMT_JOIN_ROOM);
		return  true;
	}
	memcpy(&(iter->second.lGameInfo),&pCMDInfo->lGameInfo,sizeof(pCMDInfo->lGameInfo));
	
	map<BYTE,vector<RoomPlayer>>::iterator  iterRoom= pClubGameInfo->m_mapRoomPlayer.find(pCMDInfo->bFloorIndex);
	if(iterRoom == pClubGameInfo->m_mapRoomPlayer.end())
	{
	}
	else 
	{
		vector<RoomPlayer>::iterator iterPlayer = iterRoom->second.begin();
		for(;iterPlayer!=iterRoom->second.end();iterPlayer++)
		{
			if(iterPlayer->bLeftChair == iter->second.lGameInfo.bPlayerNum && 
				iterPlayer->dwTableID >= 0 && iterPlayer->dwTableID < 500)
			{
				PrivateTableInfo  *pTableInfo = &m_vecTableInfo[iterPlayer->dwTableID];
				if(pTableInfo != NULL)
				{
					DismissRoom(pTableInfo);
					ClearRoom(pTableInfo);
				}
			}
		}
	}
	WORD  wTableID = 0;
	if(OnTCPNetCreateClubRoom(pClubGameInfo,wTableID,pCMDInfo->bFloorIndex) == true)
	{
		//发送房间列表
		PrivateTableInfo  *pTableNode =  &m_vecTableInfo[wTableID];
		OnNotifyClubRoomInfo(pIServerUserItem,pTableNode,pCMDInfo->bFloorIndex,true);
	}
	
	return  true;

}
//创建俱乐部房间
bool   PriaveteGame::OnTCPNetCreateClubRoom(ClubGameInfo  *pClubGameInfo,WORD  &wTableID, BYTE bFloorIndex)
{
	if(pClubGameInfo == NULL)  return  false;

	map<BYTE,FloorStress>::iterator  iter = pClubGameInfo->m_mapFloorStress.find(bFloorIndex);
	if(iter == pClubGameInfo->m_mapFloorStress.end()) return false;

	int    iRandNum  = 0;
	PrivateTableInfo *pCurrTableInfo = NULL;
	ITableFrame * pICurrTableFrame = NULL;

	for (wTableID=0;wTableID<m_pGameServiceOption->wTableCount;wTableID++)
	{
		//获取对象
		ASSERT(m_vecTableInfo[wTableID].pITableFrame!=NULL);
		PrivateTableInfo* pPrivateInfo = &m_vecTableInfo[wTableID];
		ITableFrame * pITableFrame = pPrivateInfo->pITableFrame;
		if(m_vecTableInfo[wTableID].bInEnd)
		{
			continue;
		}
		//状态判断
		if (pPrivateInfo->dwRoomNum == 0) //俱乐部新增房间
		{
			pICurrTableFrame = pITableFrame;
			pCurrTableInfo = &m_vecTableInfo[wTableID];
			pCurrTableInfo->restValue();
			pCurrTableInfo->dwClubID = pClubGameInfo->ClubID;
			break;
		}
	}
	//桌子判断
	if(pICurrTableFrame==NULL)
	{
		//m_pIGameServiceFrame->SendRoomMessage(pIServerUserItem,TEXT("创建房间失败。"),SMT_EJECT|SMT_CREATE_ROOM);
		return true;
	}
	while (iRandNum == 0 || getTableInfoByRoomID(iRandNum) || iRandNum == m_iRoomNumId)
	{
		iRandNum = 1000 + rand() % 8900;
		if(m_pGameServiceOption->wServerID >= 90)
		{
			int count = m_pGameServiceOption->wServerID / 90;
			iRandNum = (m_pGameServiceOption->wServerID - (90 * count) + 10) * 10000 + iRandNum;
		}
		else
			iRandNum = (m_pGameServiceOption->wServerID + 10) * 10000 + iRandNum;
	}
	m_iRoomNumId = iRandNum;
	pICurrTableFrame = pCurrTableInfo->pITableFrame;
	pCurrTableInfo->fEmptyPastTime =0;
	pCurrTableInfo->fDismissUnStartTime=0;
	pCurrTableInfo->setRoomNum(iRandNum,pClubGameInfo->ClubID);
	pCurrTableInfo->dwFinishPlayCout = 0;
	pCurrTableInfo->dwClubID =  pClubGameInfo->ClubID;
	pCurrTableInfo->bGameTypeIndex = iter->second.lGameInfo.bGameTypeIndex;
	pCurrTableInfo->bGameTypeQing  = iter->second.lGameInfo.bGameTypeQing;
	pCurrTableInfo->dwPlayCout = m_kPrivateInfo.bPlayCout[iter->second.lGameInfo.bPlayCoutIdex];
	pCurrTableInfo->bGamePayType = iter->second.lGameInfo.bGamePayType;
	pCurrTableInfo->bQiangGangHu = iter->second.lGameInfo.bQiangGangHu;
	pCurrTableInfo->bAllOther = iter->second.lGameInfo.bAllOther;
	pCurrTableInfo->bGameMaxScore = iter->second.lGameInfo.bGameMaxScore;
	pCurrTableInfo->cbPantherType = iter->second.lGameInfo.bPatherType;
	pCurrTableInfo->cbPlayerNum = iter->second.lGameInfo.bPlayerNum;
	pCurrTableInfo->bGameBaseScore = iter->second.lGameInfo.bGameBaseScore;
	pCurrTableInfo->bBaoIsOutCard = iter->second.lGameInfo.bBaoIsOutCard;
	pCurrTableInfo->bPiaoShu = iter->second.lGameInfo.bPiaoShu;
	pCurrTableInfo->dwCreaterUserID = iter->second.dwCreateUser;
	GetLocalTime(&pCurrTableInfo->kTotalRecord.kPlayTime);
	pCurrTableInfo->bGameRuleIdex = 906;
	if(pCurrTableInfo->dwPlayCout == 16)
	{
		if(pCurrTableInfo->bGameTypeIndex == 3)
		{
			if (pCurrTableInfo->bGameMaxScore == 20)
			{
				pCurrTableInfo->dwPlayCost = 8;
			}
			else if (pCurrTableInfo->bGameMaxScore == 30)
			{
				pCurrTableInfo->dwPlayCost = 12;
			}
			else
			{
				pCurrTableInfo->dwPlayCost = 16;
			}
		}
		else if (pCurrTableInfo->bGameTypeIndex == 2 && pCurrTableInfo->bGameMaxScore == 50)
		{
			pCurrTableInfo->dwPlayCost = 16;
		}
		else
			pCurrTableInfo->dwPlayCost = 12;
	}
	else
	{
		pCurrTableInfo->dwPlayCost = (DWORD)m_kPrivateInfo.lPlayCost[iter->second.lGameInfo.bPlayCoutIdex];
	}
	pICurrTableFrame->SetPrivateInfo(iter->second.lGameInfo.bGameTypeIndex,pCurrTableInfo->bGameRuleIdex,pCurrTableInfo->bGameBaseScore,
			pCurrTableInfo->bGameGuDingMa,iter->second.lGameInfo.bQiangGangHu,pCurrTableInfo->bGameCardType,pCurrTableInfo->bLimitIP);

	pICurrTableFrame->SetPrivateFushunInfo(pCurrTableInfo->bGameTypeIndex,pCurrTableInfo->bGameTypeQing,pCurrTableInfo->bLimitIP,
			pCurrTableInfo->bGameMaxScore,pCurrTableInfo->bGameMaxScoreGold,pCurrTableInfo->bPiaoShu,pCurrTableInfo->bBaoIsOutCard,
			pCurrTableInfo->bFendDing,pCurrTableInfo->bJingDing,pCurrTableInfo->bAllOther, pCurrTableInfo->cbPantherType,pCurrTableInfo->cbPlayerNum,
			pCurrTableInfo->dwClubID);
	pClubGameInfo->AddClubRom(bFloorIndex,wTableID,iRandNum,pCurrTableInfo->dwCreaterUserID,pCurrTableInfo->cbPlayerNum );
	//sendPrivateRoomInfo(NULL,pCurrTableInfo);
	return  true;
}

void  PriaveteGame::OnNotifyClubRoomInfo(IServerUserItem * pIServerUserItem,PrivateTableInfo *pTableInfo,BYTE  bFloorIndex, BYTE bLastFlag)
{

	CMD_GF_Ack_My_Room_Info   kRoomInfo;
	ZeroMemory(&kRoomInfo,sizeof(kRoomInfo));
	kRoomInfo.bGameTypeIndex = pTableInfo->bGameTypeIndex;
	kRoomInfo.bGameTypeQing  = pTableInfo->bGameTypeQing;
	kRoomInfo.bGamePayType = pTableInfo->bGamePayType;
	kRoomInfo.bGameBaseScore = pTableInfo->bGameBaseScore;
	kRoomInfo.bQiangGangHu =  pTableInfo->bQiangGangHu;
	kRoomInfo.bPlayCoutIdex =  pTableInfo->bPlayCoutIdex;
	kRoomInfo.bAllOther =  pTableInfo->bAllOther;
	kRoomInfo.bGameMaxScore =  pTableInfo->bGameMaxScore;
	kRoomInfo.bPatherType  =  pTableInfo->cbPantherType;
	kRoomInfo.bPlayerNum =  pTableInfo->cbPlayerNum;
	kRoomInfo.bBaoIsOutCard =  pTableInfo->bBaoIsOutCard;
	kRoomInfo.bPiaoShu =  pTableInfo->bPiaoShu;
	kRoomInfo.bFloorIndex = bFloorIndex;
	IServerUserItem* user0 = pTableInfo->pITableFrame->GetTableUserItem(0);
	IServerUserItem* user1 = pTableInfo->pITableFrame->GetTableUserItem(1);
	IServerUserItem* user2 = pTableInfo->pITableFrame->GetTableUserItem(2);
	IServerUserItem* user3 = pTableInfo->pITableFrame->GetTableUserItem(3);		

	kRoomInfo.cbCount = 4 - pTableInfo->pITableFrame->GetNullChairCount();
	kRoomInfo.dwRoomNum = pTableInfo->dwRoomNum;
	//kRoomInfo.cbPlayCount = pTableInfo->pITableFrame->StartGame
	if(user0)
	{
		kRoomInfo.cbStatus0 = user0->GetUserStatus();
		kRoomInfo.dwUserID0 = user0->GetUserID();
		lstrcpyn(kRoomInfo.cbUserName0,user0->GetNickName(),LEN_NICKNAME);
		tagUserInfo * pUserInfo=user0->GetUserInfo();
		DWORD dwUserIDTemp = pUserInfo->dwUserID;
		lstrcpyn(kRoomInfo.szHeadHttp0,user0->GetUserInfo()->szHeadHttp,255);			
	}
	else
	{
		kRoomInfo.cbStatus0 = US_NULL;
		lstrcpyn(kRoomInfo.cbUserName0,"",LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp0,"",255);
		kRoomInfo.dwUserID0 = 0;
	}

	if(user1)
	{
		kRoomInfo.cbStatus1 = user1->GetUserStatus();
		kRoomInfo.dwUserID1 = user1->GetUserID();
		lstrcpyn(kRoomInfo.cbUserName1,user1->GetNickName(),LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp1,user1->GetUserInfo()->szHeadHttp,255);		
	}
	else
	{
		kRoomInfo.cbStatus1 = US_NULL;
		lstrcpyn(kRoomInfo.cbUserName1,"",LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp1,"",255);
		kRoomInfo.dwUserID1 = 1;
	}

	if(user2)
	{
		kRoomInfo.cbStatus2 = user2->GetUserStatus();
		kRoomInfo.dwUserID2 = user2->GetUserID();
		lstrcpyn(kRoomInfo.cbUserName2,user2->GetNickName(),LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp2,user2->GetUserInfo()->szHeadHttp,255);		
	}
	else
	{
		kRoomInfo.cbStatus2 = US_NULL;
		kRoomInfo.dwUserID2 = 2;
		lstrcpyn(kRoomInfo.cbUserName2,"",LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp2,"",255);
	}

	if(user3)
	{
		kRoomInfo.cbStatus3 = user3->GetUserStatus();
		kRoomInfo.dwUserID3 = user3->GetUserID();
		lstrcpyn(kRoomInfo.cbUserName3,user3->GetNickName(),LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp3,user3->GetUserInfo()->szHeadHttp,255);		
	}
	else
	{
		kRoomInfo.cbStatus3 = US_NULL;
		kRoomInfo.dwUserID3 = 3;
		lstrcpyn(kRoomInfo.cbUserName3,"",LEN_NICKNAME);
		lstrcpyn(kRoomInfo.szHeadHttp3,"",255);
	}
	kRoomInfo.bIsLast  = bLastFlag;
	SendData(pIServerUserItem,MDM_GR_PRIVATE,SUB_GR_CLUB_ROOM_INFO_RES,&kRoomInfo,sizeof(CMD_GF_Ack_My_Room_Info));
}

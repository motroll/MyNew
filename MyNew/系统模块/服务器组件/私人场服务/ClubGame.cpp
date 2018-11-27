#include "stdafx.h"
#include "ClubGame.h"


ClubGameInfo::ClubGameInfo(void)
{
	clubRomNum = 0;		
	ClubID = 0;			
	m_dwUserNum = 0;      
	m_cbQiangGaneHu = 0;	
	m_cbAllOther = 0;		
	m_cbGameTypeIndex = 0;
	m_cbGameTypeQing = 0; 
	m_mapFloorStress.clear();
}
//新增俱乐部房间
bool ClubGameInfo::AddClubRom(BYTE  bFloorIndex,WORD  wTableID, int iRoomNum,int iCreateUser,BYTE  bPlayerNum)
{
	if(bFloorIndex <= 0 |  bFloorIndex > 5 )
	{
		return  false;
	}
	RoomPlayer  rRoomPlayer;
	ZeroMemory(&rRoomPlayer,sizeof(rRoomPlayer));
	rRoomPlayer.dwTableID  =  wTableID;
	rRoomPlayer.dwRoomNum = iRoomNum;
	rRoomPlayer.bLeftChair = bPlayerNum;
	rRoomPlayer.dwCreateUserID = iCreateUser;
	map<BYTE,vector<RoomPlayer>>::iterator  iter = m_mapRoomPlayer.find(bFloorIndex);
	if(iter !=m_mapRoomPlayer.end())
	{
		iter->second.push_back(rRoomPlayer);
	}
	else 
	{
		vector<RoomPlayer>  vecRoomPlayer;
		vecRoomPlayer.clear();
		vecRoomPlayer.push_back(rRoomPlayer);
		m_mapRoomPlayer.insert(pair<BYTE,vector<RoomPlayer>>(bFloorIndex,vecRoomPlayer));
	}
	clubRomNum++;
	return   true;
}
//释放俱乐部房间
void ClubGameInfo::DeleteClubTable(int iRoomNum,BYTE  bFloorIndex)
{
	std::map<BYTE,vector<RoomPlayer>>::iterator iter = m_mapRoomPlayer.find(bFloorIndex);
	if(iter == m_mapRoomPlayer.end())
	{
		return ;
	}
	vector<RoomPlayer>::iterator  iterRoom = iter->second.begin();
	for(;iterRoom!=iter->second.end(); iterRoom++)
	{
		if(iterRoom->dwRoomNum ==  iRoomNum)
		{
			iter->second.erase(iterRoom);
			clubRomNum--;
			break;
		}
	}
	if(clubRomNum == 0)
	{
		ResetValue();
	}
}
//更新房间人数
void ClubGameInfo::UpdateRoomPlayerNum(BYTE  bNullChair,BYTE  bFloorIndex, int iRoomNum)
{
	std::map<BYTE,vector<RoomPlayer>>::iterator iter = m_mapRoomPlayer.find(bFloorIndex);
	if(iter == m_mapRoomPlayer.end())
	{
		return ;
	}
	std::map<BYTE,FloorStress>::iterator  iterFloor =  m_mapFloorStress.find(bFloorIndex);
	vector<RoomPlayer>::iterator  iterRoom = iter->second.begin();
	for(;iterRoom!=iter->second.end(); iterRoom++)
	{
		if(iterRoom->dwRoomNum ==  iRoomNum)
		{
			iterRoom->bLeftChair = iterFloor->second.lGameInfo.bPlayerNum -bNullChair;
			break;
		}
	}
}
//新增楼层
bool ClubGameInfo::AddClubFloor(FloorStress kFloorStress,BYTE bFloorIndex)
{
	if(bFloorIndex <= 0 |  bFloorIndex > 5 )
	{
		return  false;
	}
	if(kFloorStress.lGameInfo.bGameTypeIndex != m_cbGameTypeIndex)
	{
		return false;
	}
	map<BYTE,FloorStress>::iterator iter = m_mapFloorStress.find(bFloorIndex);
	if(iter!=m_mapFloorStress.end())
	{
	}
	m_mapFloorStress.insert(pair<BYTE,FloorStress>(bFloorIndex,kFloorStress));
	
}

//判断该房间是否在该俱乐部
int  ClubGameInfo::GetTableByRoomNum(int iRoomNum)
{
	std::map<BYTE,std::vector<RoomPlayer>>::iterator iter = m_mapRoomPlayer.begin();
	for(;iter !=m_mapRoomPlayer.end();iter++)
	{
		std::vector<RoomPlayer>::iterator iterPlayer = iter->second.begin();
		for(;iterPlayer!=iter->second.end();iterPlayer++)
		{
			if(iterPlayer->dwRoomNum == iRoomNum)
			{
				return  iRoomNum;
			}
		}
	}
	return 0;
}
//获取当前俱乐部的桌子数
int ClubGameInfo::GetTableNum()
{
	return clubRomNum;
}

//清除数据
void ClubGameInfo::ResetValue()
{
	ClubID  = 0;
	clubRomNum = 0;
	m_dwUserNum = 0;
	m_dwManagerID = 0;
	m_dwMasterID = 0; 
	m_cbGameTypeIndex = 0;
	m_cbGameTypeQing = 0;
	m_mapFloorStress.clear();
	m_mapRoomPlayer.clear();

}
ClubGameInfo::~ClubGameInfo(void)
{
	//m_vecTableInfo.clear();
	ResetValue();
}

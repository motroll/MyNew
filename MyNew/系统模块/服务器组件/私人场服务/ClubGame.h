#pragma once
#include "PrivateTableInfo.h"
#include "..\游戏服务器\DataBasePacket.h"
#include <vector>
#include <map>

#define      CLUB_MAX_FLOOR_NUM        5    //每个楼层房间数
#define      CLUB_MAX_FLOOR_ROOM      30    //每个楼层最多房间数 

using  namespace  std;

struct   RoomPlayer
{
	BYTE     bLeftChair;               // 剩余空位数
	WORD     dwTableID;                 // 桌子位置
	DWORD    dwRoomNum;                 // 房间号
	DWORD    dwCreateUserID;            //创建房间用户
};
struct   FloorStress 
{
	BYTE				 bFloorIndex;             // 楼层
	FloorStressGameInfo  lGameInfo ;	          // 游戏玩法
	DWORD				 dwCreateUser;            // 创建者      
};

class ClubGameInfo
{
public:
	ClubGameInfo(void);
	~ClubGameInfo(void);

public:
	int  GetClubScoreNullChair(int iMaxScore);		//统计某个积分的剩余房间数
	int  GetClubNullChair(DWORD  dwUpdateStatus, int &iFourPlayerTable, int &iTwoPlayerTable);
	int  GetClubNullChair(DWORD  dwUpdateStatus,DWORD dwDissRoom[3]);
	void DeleteClubTable(int iRoomNum,BYTE  bFloorIndex);       //释放俱乐部房间
	bool AddClubRom(BYTE  bFloorIndex,WORD  wTableID, int iRoomNum,int iCreateUser,BYTE  bPlayerNum); //添加房间
	void UpdateRoomPlayerNum(BYTE  bNullChair,BYTE  bFloorIndex, int iRoomNum);
	bool AddClubFloor(FloorStress kFloorStress,BYTE bFloorIndex);//新增楼层

	int  GetTableNum();    //获取当前俱乐部的桌子数
	void ResetValue();
	bool EnterOneNewTable(int iRoomNum,byte bPlayerNum);  //进入一个新的房间 
	int  GetTableByRoomNum(int iRoomNum); //判断该房间是否在该俱乐部
	void UpateClubInfo();

	std::map<BYTE,FloorStress>  m_mapFloorStress;
	std::map<BYTE,vector<RoomPlayer>>  m_mapRoomPlayer;
	//楼层管理 

public:	

	DWORD						 clubRomNum;				           //当前占用房间数量
	DWORD						 ClubID;					           //俱乐部编号
	DWORD                        m_dwUserNum;                          //当前用户在线人
	BYTE						 m_cbQiangGaneHu;					   //黄州的抢杠胡
	BYTE						 m_cbAllOther;						   //溪水的全求人
	BYTE                         m_cbGameTypeIndex;					   //玩法
	BYTE                         m_cbGameTypeQing;                     //门清
	DWORD                        m_dwMasterID;                         //俱乐部会长
	DWORD                        m_dwManagerID;						   //俱乐部管理员
	BOOL                         m_bCanDiss;                           //可以释放
	DWORD                        m_dwDissTime;			               //可以解散计时时间
};


#ifndef PRIVATE_TABLE_INFO_FILE
#define PRIVATE_TABLE_INFO_FILE

#pragma once

//引入文件
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"

///////////////////////////////////////////////////////////////////////////////////////////

enum RoomType
{
	Type_Private,
	Type_Public,
};

//定时赛
class PrivateTableInfo 
{
public:
	PrivateTableInfo();
	~PrivateTableInfo();

	void restValue();
	void restAgainValue();
	void newRandChild();
	WORD getChairCout();
	bool IsAllOffline();
	void setRoomNum(DWORD RoomNum,DWORD ClubID = 0);
	void writeSocre(tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream& daUserDefine);

	ITableFrame*	pITableFrame;
	DWORD			dwCreaterUserID;
	DWORD			dwRoomNum;
	DWORD			dwPlayCout;
	DWORD			dwPlayCost;
	bool			bStart;
	bool			bInEnd;
	float			fAgainPastTime;
	std::string		kHttpChannel;

	BYTE			cbRoomType;

	DWORD			dwStartPlayCout;
	DWORD			dwFinishPlayCout;



	BYTE			bPlayCoutIdex;		//玩家局数
	BYTE			bGameTypeIdex;		//游戏类型
	DWORD			bGameRuleIdex;		//游戏规则
	BYTE			bGamePayType;		//游戏付费类型
	BYTE			bGameBaseScore;		//底分
	BYTE			bGameGuDingMa;		//固定吗
	BYTE			bQiangGangHu;		//抢杠胡，0没有抢杠胡，1有抢杠胡
	BYTE			bGameCardType;
	BYTE			bAllOther;
	BYTE			cbPantherType;		// 风 将 单 色 豹
	BYTE            cbPlayerNum;        //游戏人数

	BYTE			bBaoIsOutCard;		// 0 有宝不可打 1 有宝可打(黄梅)  2 无宝
	BYTE			bPiaoShu;			// 飘数 0 不飘 1 飘 1 ... 飘10
	//黄石
	BYTE			bGameTypeIndex;		//
	BYTE			bGameTypeQing;		//
	BYTE			bLimitIP;

	BYTE			bGameMaxScore;		//封顶
	BYTE			bGameMaxScoreGold;	//金顶
	BYTE			bOpenMouthFan;		//开口番
	BYTE			bQiDuiMengQing;		//七对门前清
	BYTE			bFendDing;			//只上封顶
	BYTE			bJingDing;			//金顶

	BYTE			cbLastOfflineReadyState[MAX_CHAIR];		//上次断线状态
	SCORE			lPlayerWinLose[MAX_CHAIR];
	BYTE			lPlayerAction[MAX_CHAIR][MAX_PRIVATE_ACTION];

	float			fDismissPastTime;
	std::vector<DWORD> kDismissChairID;
	std::vector<DWORD> kNotAgreeChairID;

	float			fOfflinePastTime;

	float			fEmptyPastTime;

	float			fDismissUnStartTime;

	DWORD             dwClubID;              //俱乐部房间
	BYTE              bFloorIndex;           //楼层

	tagPrivateRandTotalRecord kTotalRecord;
};

#endif
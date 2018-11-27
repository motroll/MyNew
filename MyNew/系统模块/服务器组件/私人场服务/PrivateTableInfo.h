#ifndef PRIVATE_TABLE_INFO_FILE
#define PRIVATE_TABLE_INFO_FILE

#pragma once

//�����ļ�
#include "CTableFramePrivate.h"
#include "PrivateServiceHead.h"

///////////////////////////////////////////////////////////////////////////////////////////

enum RoomType
{
	Type_Private,
	Type_Public,
};

//��ʱ��
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



	BYTE			bPlayCoutIdex;		//��Ҿ���
	BYTE			bGameTypeIdex;		//��Ϸ����
	DWORD			bGameRuleIdex;		//��Ϸ����
	BYTE			bGamePayType;		//��Ϸ��������
	BYTE			bGameBaseScore;		//�׷�
	BYTE			bGameGuDingMa;		//�̶���
	BYTE			bQiangGangHu;		//���ܺ���0û�����ܺ���1�����ܺ�
	BYTE			bGameCardType;
	BYTE			bAllOther;
	BYTE			cbPantherType;		// �� �� �� ɫ ��
	BYTE            cbPlayerNum;        //��Ϸ����

	BYTE			bBaoIsOutCard;		// 0 �б����ɴ� 1 �б��ɴ�(��÷)  2 �ޱ�
	BYTE			bPiaoShu;			// Ʈ�� 0 ��Ʈ 1 Ʈ 1 ... Ʈ10
	//��ʯ
	BYTE			bGameTypeIndex;		//
	BYTE			bGameTypeQing;		//
	BYTE			bLimitIP;

	BYTE			bGameMaxScore;		//�ⶥ
	BYTE			bGameMaxScoreGold;	//��
	BYTE			bOpenMouthFan;		//���ڷ�
	BYTE			bQiDuiMengQing;		//�߶���ǰ��
	BYTE			bFendDing;			//ֻ�Ϸⶥ
	BYTE			bJingDing;			//��

	BYTE			cbLastOfflineReadyState[MAX_CHAIR];		//�ϴζ���״̬
	SCORE			lPlayerWinLose[MAX_CHAIR];
	BYTE			lPlayerAction[MAX_CHAIR][MAX_PRIVATE_ACTION];

	float			fDismissPastTime;
	std::vector<DWORD> kDismissChairID;
	std::vector<DWORD> kNotAgreeChairID;

	float			fOfflinePastTime;

	float			fEmptyPastTime;

	float			fDismissUnStartTime;

	DWORD             dwClubID;              //���ֲ�����
	BYTE              bFloorIndex;           //¥��

	tagPrivateRandTotalRecord kTotalRecord;
};

#endif
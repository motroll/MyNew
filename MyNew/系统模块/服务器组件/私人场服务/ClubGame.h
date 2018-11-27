#pragma once
#include "PrivateTableInfo.h"
#include "..\��Ϸ������\DataBasePacket.h"
#include <vector>
#include <map>

#define      CLUB_MAX_FLOOR_NUM        5    //ÿ��¥�㷿����
#define      CLUB_MAX_FLOOR_ROOM      30    //ÿ��¥����෿���� 

using  namespace  std;

struct   RoomPlayer
{
	BYTE     bLeftChair;               // ʣ���λ��
	WORD     dwTableID;                 // ����λ��
	DWORD    dwRoomNum;                 // �����
	DWORD    dwCreateUserID;            //���������û�
};
struct   FloorStress 
{
	BYTE				 bFloorIndex;             // ¥��
	FloorStressGameInfo  lGameInfo ;	          // ��Ϸ�淨
	DWORD				 dwCreateUser;            // ������      
};

class ClubGameInfo
{
public:
	ClubGameInfo(void);
	~ClubGameInfo(void);

public:
	int  GetClubScoreNullChair(int iMaxScore);		//ͳ��ĳ�����ֵ�ʣ�෿����
	int  GetClubNullChair(DWORD  dwUpdateStatus, int &iFourPlayerTable, int &iTwoPlayerTable);
	int  GetClubNullChair(DWORD  dwUpdateStatus,DWORD dwDissRoom[3]);
	void DeleteClubTable(int iRoomNum,BYTE  bFloorIndex);       //�ͷž��ֲ�����
	bool AddClubRom(BYTE  bFloorIndex,WORD  wTableID, int iRoomNum,int iCreateUser,BYTE  bPlayerNum); //��ӷ���
	void UpdateRoomPlayerNum(BYTE  bNullChair,BYTE  bFloorIndex, int iRoomNum);
	bool AddClubFloor(FloorStress kFloorStress,BYTE bFloorIndex);//����¥��

	int  GetTableNum();    //��ȡ��ǰ���ֲ���������
	void ResetValue();
	bool EnterOneNewTable(int iRoomNum,byte bPlayerNum);  //����һ���µķ��� 
	int  GetTableByRoomNum(int iRoomNum); //�жϸ÷����Ƿ��ڸþ��ֲ�
	void UpateClubInfo();

	std::map<BYTE,FloorStress>  m_mapFloorStress;
	std::map<BYTE,vector<RoomPlayer>>  m_mapRoomPlayer;
	//¥����� 

public:	

	DWORD						 clubRomNum;				           //��ǰռ�÷�������
	DWORD						 ClubID;					           //���ֲ����
	DWORD                        m_dwUserNum;                          //��ǰ�û�������
	BYTE						 m_cbQiangGaneHu;					   //���ݵ����ܺ�
	BYTE						 m_cbAllOther;						   //Ϫˮ��ȫ����
	BYTE                         m_cbGameTypeIndex;					   //�淨
	BYTE                         m_cbGameTypeQing;                     //����
	DWORD                        m_dwMasterID;                         //���ֲ��᳤
	DWORD                        m_dwManagerID;						   //���ֲ�����Ա
	BOOL                         m_bCanDiss;                           //�����ͷ�
	DWORD                        m_dwDissTime;			               //���Խ�ɢ��ʱʱ��
};


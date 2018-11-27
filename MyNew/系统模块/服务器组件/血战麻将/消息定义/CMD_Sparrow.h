#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE
#include <vector>
#include "..\..\..\ȫ�ֶ���\datastream.h"
//�������
struct GDMJ_CMD_WeaveItem 
{
	DWORD							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbWeaveKind);
		Stream_VALUE(cbCenterCard);
		Stream_VALUE(cbPublicCard);
		Stream_VALUE(wProvideUser);
	}
};

//////////////////////////////////////////////////////////////////////////
//�����궨��
#pragma pack(1)

#define KIND_ID						302									//��Ϸ I D

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_NAME					TEXT("��ʯ�齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GAME_STATUS_FREE								//����״̬
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)						//��Ϸ״̬

#define GS_MJ_HUANPAI                 (GAME_STATUS_PLAY+2)						//����״̬
#define GS_MJ_XUANQUE                 (GAME_STATUS_PLAY+3)						//ѡȱ״̬
#define GS_MJ_PIAODANG					(GAME_STATUS_PLAY+4)						//Ʈ��
#define GS_MJ_DINGZHUANG				(GAME_STATUS_PLAY+5)					//��ׯ
#define GS_MJ_ROLLDICE					(GAME_STATUS_PLAY+6)					//������
			

//��������
#define MAX_WEAVE					50									//������
#define MAX_PENG					7									//������
#define MAX_INDEX					34									//�������
#define MAX_COUNT					14									//�����Ŀ
#define MAX_REPERTORY				136									//�����
#define MAX_REPERTORY_WUFENG		108									//�޷������
#define MAX_REPERTORY_HZWUFENG		112									//�����޷������
#define MAX_REPERTORY_HUANGZHOU     120                                 //���ݻλ������
#define TWO_THREE_HUANGZHOU         84                                  //���ݻλ�2-3�������
#define TWO_THREE_REPERTORY         100                                 //����,�ˮ2-3�������

//�˿˶���
#define HEAP_FULL_COUNT				26									//����ȫ��

#define MAX_RIGHT_COUNT				1									//���ȨλDWORD����			

#define		GAME_TYPE_SICHUAN  0                      //�Ĵ��齫
#define		GAME_TYPE_CHENGDU  1					   //�ɶ��齫
#define		GAME_TYPE_YICHANG  2

#define		GAME_RULE_NO_WAIT			1
#define		GAME_RULE_DIN_QUE			2
#define		GAME_RULE_DAODAO_HU			3
#define		GAME_RULE_ZIMOHU			4
#define		GAME_RULE_QIDUI				5
#define		GAME_DIAN_GANG_HUA			6
#define		GAME_DIAN_GANE_ZIMO			7
#define		GAME_RULE_DING_ZHUANG		8
#define		GAME_RULE_ROLL_DICE			9
#define		GAME_RULE_ZIMIANMA			10
#define		GAME_RULE_XIAMA_QIANGGANG	11
#define		GAME_RULE_HONGZHONG			12
#define		GAME_RULE_GUANGDONG			13
#define		GAME_RULE_FIVETIMES			14
//fushun
#define		GAME_RULE_HUN			15
#define		GAME_RULE_XI			16
#define		GAME_RULE_HU_SCORE_TYPE		17
#define		GAME_RULE_GANG_SCORE_TYPE	18
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_ROLL_DICE		111									//������ȷ��ׯ��
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//
#define SUB_S_GAME_SELECT_HU		111									//ѡ�ƺ��ú�����Ϣ	
#define SUB_S_GAME_TING				112									//���������Ϣ
#define SUB_S_GAME_OUT_TING			113
#define SUB_S_REQ_PIAODANG				114									//����Ư�͵� �ͻ��˷���ʾ��������ҵ�Ʈ�͵� ����������ʾҪ��ͻ���ѡ��Ʈ�͵�
#define SUB_S_ACK_PIAODANG				115									//����Ư�͵� �ͻ��˷���ʾ�Լ���Ʈ�͵�)	����������ʾ�Լ���������ҵ�Ʈ�͵�
#define SUB_S_REQ_ROOM_INFO				116									//
#define SUB_S_ACK_ROOM_INFO				117									//
#define SUB_S_ACK_COUNT_INFO			118									//	�ͻ���->������ ��������
#define SUB_S_HIDEQFJ_NOTICE            119                                 //  ���ؿͻ��˱���罫��֪ͨ
#define SUB_S_ACK_COUNT_INFO_RES        131									//	�ͻ���û�����󵽽��������Ӧ
#define SUB_S_ACK_COUNT_INFO_DATA        132									//  ������Ϣ

//2016.5.20
#define SUB_S_HUANPAI 120				//����
#define SUB_S_XUANQUE 121				//ѡȱ
#define SUB_S_XUANQUE_NOTICE 122		//֪ͨ��ʼѡȱ
#define SUB_S_HUANPAI_CHENGDU 123				//����

#define SUB_S_HUANPAI_ANDROID 124

#define SUB_S_HUANPAI_NOTICE  125				//����

#define SUB_S_ANDROID_RULE         200                                 //���͹����������

#define SUB_S_MASTER_HANDCARD		220									//
#define SUB_S_MASTER_LEFTCARD		221									//ʣ���ƶ�


//���Ƶ÷�
struct GDMJ_tagGangScore
{
	bool		bMingGang;							//�ܸ���
	BYTE		cbGangCount;						//�ܸ���
	LONGLONG	lScore[MAX_WEAVE][GAME_PLAYER];		//ÿ���ܵ÷�
	BYTE		cbGangCount_Single;
};

//���Ʊ���
struct GDMJ_tagEnjoinPengCount
{
	BYTE		cbEnjoinCount;						//�ܸ���
	BYTE		cbEnjoinCard[MAX_PENG];					//��������
};

//��Ϸ״̬
struct GDMJ_CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

struct GDMJ_CMD_S_DingZhuang
{
	WORD							wBankerUser;
	WORD							wSice;
};

//��Ϸ״̬
struct GDMJ_CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wSliceCount;								//ɫ����

	BYTE							cbHunIndex;									// ��
	BYTE							cbPiIndex;									// Ƥ
	BYTE							cbLaiPiIndex;								// ��Ƥ
	BYTE							cbBaoIndex;								    // ��
	BYTE							cbPantherType;								// �� �� �� ɫ��
	bool                            cbShowBao[GAME_PLAYER];                     // ����
	BYTE							cbFaCaiNum[GAME_PLAYER];					// ���Ƹ���

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	DWORD							cbActionMask;								//��������
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	WORD							wWinOrder[GAME_PLAYER];						//
	BYTE							cbWinCout;									//��ʤ��Ŀ

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	GDMJ_CMD_WeaveItem				WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	WORD							wHeapHand;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ


	BYTE							nQueColor[GAME_PLAYER];						//ȱɫ
	BYTE							nChiHuCard[GAME_PLAYER][60];
	WORD							nChiHuCardNum[GAME_PLAYER];
	bool							bHasHu[GAME_PLAYER];
	bool							bHasTing[GAME_PLAYER];									//�Լ��Ƿ�����

	LONGLONG						nGangScore[GAME_PLAYER];						//���ָܷ�
	BYTE							cbGangSiceNum[16];							//���ݻλ�ÿ�θܵ����ӵ���
	DWORD                           dwQingFengJiang[4];                         //����罫��־λ
};

//��Ϸ��ʼ
struct GDMJ_CMD_S_GameStart
{
	LONG							lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	DWORD							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//�˿��б�
	WORD							wHeapHand;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
	BYTE							cbLeftCardCount;							//

	BYTE							nLeftTime;									//���Ƶ���ʱ
	BYTE							cbPantherType;								//0x01���� 0x02�籪 0x04���� 0x08ɫ�� 

	WORD							wHun;
	WORD							wPi;
	WORD							wLaiPi;

	//���ݻλ�
	WORD                            wBao; // ��
};

struct GDMJ_CMD_S_StateHuanPai
{
	WORD							wBankerUser;								//ׯ���û�
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//���Ƶ���ʱ
	bool                            bHuan;									    //�Ƿ��Ѿ�����
};

struct GDMJ_CMD_S_StateXuanQue
{
	WORD							wBankerUser;								//ׯ���û�
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//XuanQue����ʱ
	BYTE                            nQueColor;									
};
//��������
struct GDMJ_CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct GDMJ_CMD_S_SendCard
{
	BYTE					cbCardData;			//�˿�����
	DWORD					cbActionMask;		//��������
	WORD					wCurrentUser;		//��ǰ�û�
	bool					bTail;				//ĩβ����
	BYTE                    nLeftTime;
	bool					bHasHuPai;			//�Ƿ��Ѿ���
};

//������Ϣ����
struct GDMJ_CMD_AckCount
{
};
struct GDMJ_CMD_AckCount_Data
{
	BYTE cbPantherType; 
	BYTE  iMagicIndex; 
	BYTE  iPiIndex; 
	BYTE  iLaiPiIndex;
	BYTE cbDisCardCount[GAME_PLAYER];
	BYTE cbDisCardValue[GAME_PLAYER][60];
};
struct GDMJ_CMD_AckCount_Res
{

};
//��������
struct GDMJ_CMD_S_OperateReqRoomInfo
{
	BYTE							cbUser;		//���id
};

//��������
struct GDMJ_CMD_S_OperateAckRoomInfo
{
	//��ʯ
	BYTE			bGameTypeIndex;		//2�з磬1û�磬3���ݣ�4Ϫˮ
	BYTE			bGameTypeQing;		//2������ ��1 û����
	BYTE			bGameMaxScore;		//�ⶥ
	BYTE			bGameMaxScoreGold;	//��
	BYTE			bFendDing;			//ֻ�Ϸⶥ
	BYTE			bJingDing;			//��
	BYTE            bAllOther;          //ȫ���� 0 ��ȫ���� 1 ��ȫ����
	BYTE            bQiangGanghu;       //���ܺ�
	BYTE			bPantherType;		//����  �籪 ���� ɫ��
	BYTE            bPlayerNum;         //��Ϸ����
	BYTE			bPiaoShu;			//Ư��
	BYTE			bBaoIsOutCard;      //0�б����ɴ�1�б��ɴ�
	BYTE			bGameBaseScore;		//�׷�
	DWORD           dwClubId;           //���ֲ�ID
};

//����Ʈ�͵�
struct GDMJ_CMD_S_OperateReqPiaoDang
{
	BYTE							cbUser;								//���id
};

//����Ʈ�͵�
struct GDMJ_CMD_S_OperateAckPiaoDang
{
	BYTE							cbUser;								//���id
	DWORD							cbPiao;								//Ʈ
	DWORD							cbDang;								//��
};

//������ʾ
struct GDMJ_CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	DWORD							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
	bool							bHasHuPai;
};

//��������
struct GDMJ_CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	WORD                            wGangSiceNum;                       //�ܺ����ӵ���
};

//���ر�����֪ͨ
struct  GDMJ_CMD_S_HideQFJNotice 
{
	WORD                             wPeratorUser;                      //�����û�
};

//�û��й�
struct GDMJ_CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
	EstimatKind_XiGangCard,			//ϲ��Ч��
};

//2016.5.20
struct GDMJ_CMD_S_HuanPai
{
	WORD   wChairId;
	BYTE   cbHuanCard[3];
};

struct GDMJ_CMD_S_HuanPaiChengDu
{
	WORD   wChairId;
	BYTE   cbRemoveHuanCard[3];
	BYTE   cbHuanCard[3];
	WORD   wSice;
};

struct GDMJ_CMD_S_XuanQue
{
	BYTE nQueColor[4];
	BYTE nLeftTime;
};
struct GDMJ_CMD_S_XuanQueNotice
{
	WORD   wChairId;
	BYTE nLeftTime;
	BYTE nQueColor;
};

struct GDMJ_CMD_AndroidCardIndex
{
	BYTE   cbCardIndex[GAME_PLAYER][MAX_INDEX];
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_SELECT_CARD			5									//ѡ����
#define SUB_C_GET_TING				9									//����
#define SUB_C_GET_TING_CARDS		10									//���Ƶı�ǩҳ

#define SUB_C_MASTER_LEFTCARD		6									//ʣ���ƶ�
#define SUB_C_MASTER_CHEAKCARD		7									//ѡ�����
#define SUB_C_MASTER_ZHANIAO		8									//����
//
//2016.5.20
#define SUB_C_HUANPAI		11				//����
#define SUB_C_XUANQUE		12				//ѡȱ

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//��������
struct GDMJ_CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//ѡ������
struct GDMJ_CMD_C_SelectCard
{
	BYTE							cbCardData;
};

//��ȡ��������
struct GDMJ_CMD_C_GetTingCard
{
	BYTE							cbCardData;
};

//������Ʊ�ǩ
struct GDMJ_CMD_C_TingCards
{
	BYTE							cbCardData;
};

//��������
struct GDMJ_CMD_C_OperateCard
{
	BYTE							cbOperateCard;						//�����˿�
	DWORD							cbOperateCode;						//��������
};

//�û��й�
struct GDMJ_CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//2016.5.20
struct GDMJ_CMD_C_HuanPai
{
	BYTE     cbHuanCard[3];
	BYTE     nMasttKindColor;
};
struct GDMJ_CMD_C_XuanQue
{
	BYTE nQueChoice;
};
//////////////////////////////////////////////////////////////////////////


struct  GDMJ_Card_Info
{
	int		nLen;
	int		nColor;
	std::vector<BYTE> kCardList;

	bool   operator <  (const   GDMJ_Card_Info&   rhs   )  const   //��������ʱ����д�ĺ���
	{  
		return   nLen   <   rhs.nLen;
	}
};

struct GDMJ_MaterCheckCard
{
	BYTE							cbCheakCard;						//��������
};

struct GDMJ_MaterNiaoCout
{
	BYTE							cbNiaoCout;							//��������
};

struct GDMJ_MasterLeftCard
{
	BYTE      kMasterLeftIndex[MAX_INDEX];
	BYTE      kMasterCheakCard;
};


#pragma pack()


struct  GDMJGameRecordPlayer
{
	DWORD dwUserID;
	std::string kHead;
	std::string kNickName;
	std::vector<BYTE> cbCardData;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(kHead);
		Stream_VALUE(kNickName);
		Stream_VECTOR(cbCardData);
	}
};

struct  GDMJGameRecordOperateResult
{
	enum Type
	{
		TYPE_NULL,
		TYPE_OperateResult,
		TYPE_SendCard,
		TYPE_OutCard,
		TYPE_ChiHu,
	};
	GDMJGameRecordOperateResult()
	{
		cbActionType = 0;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0;
		cbOperateCard = 0;
	}
	BYTE							cbActionType;
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	DWORD							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbActionType);
		Stream_VALUE(wOperateUser);
		Stream_VALUE(wProvideUser);
		Stream_VALUE(cbOperateCode);
		Stream_VALUE(cbOperateCard);
	}
};
struct  GDMJGameRecord
{
	DWORD dwKindID;
	DWORD dwVersion;
	BYTE							cbHunIndex;									// ��
	BYTE							cbPiIndex;									// Ƥ
	BYTE							cbLaiPiIndex;								// ��Ƥ
	BYTE                            cbBaoIndex;                                 // ��
	std::vector<GDMJGameRecordPlayer> kPlayers;
	std::vector<GDMJGameRecordOperateResult> kAction;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(GDMJGameRecordPlayer,kPlayers);
		StructVecotrMember(GDMJGameRecordOperateResult,kAction);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
		Stream_VALUE(cbHunIndex);
		Stream_VALUE(cbPiIndex);
		Stream_VALUE(cbLaiPiIndex);
		Stream_VALUE(cbBaoIndex);
	}

};

struct GDMJ_MasterHandCardInfo
{
	int nChairId;
	std::vector<BYTE>    kMasterHandCard;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(nChairId);
		Stream_VECTOR(kMasterHandCard);
	}
};

struct GDMJ_MasterHandCard
{
	std::vector<GDMJ_MasterHandCardInfo>    kMasterHandCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(GDMJ_MasterHandCardInfo,kMasterHandCardList);
	}
};

struct GDMJ_CMD_S_ChiHu
{
	WORD							wChiHuUser;							//
	WORD							wProviderUser;						//
	WORD							wBaoPlayer;					// ������� ���û�а�����10
	BYTE							cbChiHuCard;						//
	std::vector<LONGLONG>            lGameScore;
	DWORD							dwChiHuRight;				// ��������
	BYTE							cbGenCount;							//
	BYTE							cbHuScore;					// ���ƻ�����
	BYTE							cbHuScoreOther;             // �мҵ׷�
	std::vector<LONGLONG>			lMenHun;					// �ƻ��/����
	std::vector<LONGLONG> 			lGang;						// �ܸܷ�
	std::vector<LONGLONG> 			lPiao;						// Ʈ/Ƥ�ܴ���
	std::vector<LONGLONG> 			lDang;						// ��/���ܴ���
	std::vector<LONGLONG> 			lMingGang;					// ���ܷ��� / ����
	std::vector<LONGLONG> 			lAnGang;					// ���ܷ��� / ����
	std::vector<LONGLONG> 			lXiGang;					// ϲ�ܷ��� / ����
	std::vector<LONGLONG> 			lDianGang;					// ��ܷ��� 
	std::vector<LONGLONG> 			lHuPaiScore;				// ���Ʒ���
	std::vector<LONGLONG> 			lBaMao;						// ��ë����
	std::vector<LONGLONG> 			lGangHu;					// �ܺ�����
	BYTE							cbSuHu;						// 0��1��
	BYTE							cbQiDui;					// 0��1��	

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChiHuUser);
		Stream_VALUE(wProviderUser);
		Stream_VALUE(wBaoPlayer);
		Stream_VALUE(cbChiHuCard);
		Stream_VECTOR(lGameScore);
		Stream_VALUE(dwChiHuRight);
		Stream_VALUE(cbGenCount);
		Stream_VALUE(cbHuScore);
		Stream_VALUE(cbHuScoreOther);
		Stream_VECTOR(lMenHun);
		Stream_VECTOR(lGang);
		Stream_VECTOR(lPiao);
		Stream_VECTOR(lDang);
		Stream_VECTOR(lMingGang);
		Stream_VECTOR(lAnGang);
		Stream_VECTOR(lXiGang);
		Stream_VECTOR(lDianGang);
		Stream_VECTOR(lHuPaiScore);
		Stream_VECTOR(lBaMao);
		Stream_VECTOR(lGangHu);
		Stream_VALUE(cbSuHu);
		Stream_VALUE(cbQiDui);

	}	
	void resizeData()
	{
		lGameScore.resize(GAME_PLAYER);
		lMenHun.resize(GAME_PLAYER);
		lGang.resize(GAME_PLAYER);
		lPiao.resize(GAME_PLAYER);
		lDang.resize(GAME_PLAYER);
		lMingGang.resize(GAME_PLAYER);
		lAnGang.resize(GAME_PLAYER);
		lXiGang.resize(GAME_PLAYER);
		lDianGang.resize(GAME_PLAYER);
		lHuPaiScore.resize(GAME_PLAYER);
		lBaMao.resize(GAME_PLAYER);
		lGangHu.resize(GAME_PLAYER);
	}
};

struct GDMJ_CMD_S_ChiHu_HM // ��÷
{
	BYTE							cbChiHuCard;
	WORD							wProviderUser;
	std::vector<DWORD>				dwChiHuRight;				// ��������
	std::vector<DWORD>				wChiHuUser;					// ���Ʒ�
	std::vector<LONGLONG> 			lMingGang;					// ���ܷ��� / ����
	std::vector<LONGLONG> 			lAnGang;					// ���ܷ��� / ����
	std::vector<LONGLONG> 			lHuPaiScore;				// ���Ʒ���
	std::vector<LONGLONG> 			lFaCaiScore;				// ���Ʒ���
	std::vector<LONGLONG>			lBaiBanScore;				// �װ����
	std::vector<LONGLONG>			lHongZhongScore;			// ���з���

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbChiHuCard);
		Stream_VALUE(wProviderUser);
		Stream_VECTOR(dwChiHuRight);
		Stream_VECTOR(wChiHuUser);
		Stream_VECTOR(lMingGang);

		Stream_VECTOR(lAnGang);
		Stream_VECTOR(lHuPaiScore);
		Stream_VECTOR(lFaCaiScore);
		Stream_VECTOR(lBaiBanScore);
		Stream_VECTOR(lHongZhongScore);
	}	
	void resizeData()
	{
		dwChiHuRight.resize(GAME_PLAYER);
		wChiHuUser.resize(GAME_PLAYER);
		lMingGang.resize(GAME_PLAYER);
		lAnGang.resize(GAME_PLAYER);
		lHuPaiScore.resize(GAME_PLAYER);
		lFaCaiScore.resize(GAME_PLAYER);
		lBaiBanScore.resize(GAME_PLAYER);
		lHongZhongScore.resize(GAME_PLAYER);
	}
};


//
struct GDMJ_CMD_S_GangScore
{
	WORD							wChairId;							//
	WORD							wPravadeId;							//
	BYTE							 cbXiaYu;
	std::vector<LONGLONG>            lGangScore;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairId);
		Stream_VALUE(wPravadeId);
		Stream_VALUE(cbXiaYu);
		Stream_VECTOR(lGangScore);
	}
	void resizeData()
	{
		lGangScore.resize(GAME_PLAYER);
	}
};

struct GDMJ_CMD_CardInfoList
{
	std::vector<BYTE>					kHandCardList;
	std::vector<BYTE>					kLeftCardList;
	std::vector<GDMJ_CMD_WeaveItem>    kWeaveItemList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kHandCardList);
		Stream_VECTOR(kLeftCardList);
		StructVecotrMember(GDMJ_CMD_WeaveItem,kWeaveItemList);
	}
};

struct GDMJ_CMD_GetHuCardList
{
	std::vector<BYTE>				kHuCards;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kHuCards);
	}
};

struct GDMJ_CMD_GetTingCards
{
	BYTE							bType;
	std::vector<BYTE>				kHuCards;
	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(bType);
		Stream_VECTOR(kHuCards);
	}
};

//������Ʊ��
struct GDMJ_CMD_GetOutCardsTing
{
	BYTE							bType;
	WORD							wChairID;
	std::vector<BYTE>				kOutCards;
	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(bType);
		Stream_VALUE(wChairID);
		Stream_VECTOR(kOutCards);
	}
};

struct GDMJ_MaPaiInfo
{
	BYTE							bMaPai;
	int								bIsZhong;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(bMaPai);
		Stream_VALUE(bIsZhong);
	}
};

struct GDMJ_AllEndInfo
{
	std::vector<LONGLONG>				lGameScoreTotal;
	std::vector<LONGLONG>				lHuaZhuScore;
	std::vector<LONGLONG>				lChaJiaoScore;
	std::vector<BYTE>					cbGenCount;
	std::vector<BYTE>					cbFanShu;
	std::vector<GDMJ_MaPaiInfo>			cbMaPai;
	std::vector<GDMJ_CMD_S_ChiHu>		kChiHuInfoList;
	std::vector<GDMJ_CMD_S_ChiHu_HM>	kChiHuInfoListHM;
	std::vector<GDMJ_CMD_S_GangScore>	kGangInfoList;
	std::vector<GDMJ_CMD_CardInfoList>  kCardInfoList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(lGameScoreTotal);
		Stream_VECTOR(lHuaZhuScore);
		Stream_VECTOR(lChaJiaoScore);
		Stream_VECTOR(cbGenCount);
		Stream_VECTOR(cbFanShu);
		StructVecotrMember(GDMJ_MaPaiInfo,cbMaPai);
		StructVecotrMember(GDMJ_CMD_S_ChiHu,kChiHuInfoList);
		StructVecotrMember(GDMJ_CMD_S_ChiHu_HM,kChiHuInfoListHM);
		StructVecotrMember(GDMJ_CMD_S_GangScore,kGangInfoList);
		StructVecotrMember(GDMJ_CMD_CardInfoList,kCardInfoList);
	}
	void resizeData()
	{
		lGameScoreTotal.resize(GAME_PLAYER);
		lHuaZhuScore.resize(GAME_PLAYER);
		lChaJiaoScore.resize(GAME_PLAYER);
		cbGenCount.resize(GAME_PLAYER);
		cbFanShu.resize(GAME_PLAYER);
		kCardInfoList.resize(GAME_PLAYER);
	}
};
//





#endif
#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE
#include <vector>
#include "..\..\..\全局定义\datastream.h"
//组合子项
struct GDMJ_CMD_WeaveItem 
{
	DWORD							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbWeaveKind);
		Stream_VALUE(cbCenterCard);
		Stream_VALUE(cbPublicCard);
		Stream_VALUE(wProvideUser);
	}
};

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)

#define KIND_ID						302									//游戏 I D

//组件属性
#define GAME_PLAYER					4									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_NAME					TEXT("黄石麻将")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_MJ_FREE					GAME_STATUS_FREE								//空闲状态
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)						//游戏状态

#define GS_MJ_HUANPAI                 (GAME_STATUS_PLAY+2)						//换牌状态
#define GS_MJ_XUANQUE                 (GAME_STATUS_PLAY+3)						//选缺状态
#define GS_MJ_PIAODANG					(GAME_STATUS_PLAY+4)						//飘档
#define GS_MJ_DINGZHUANG				(GAME_STATUS_PLAY+5)					//定庄
#define GS_MJ_ROLLDICE					(GAME_STATUS_PLAY+6)					//掷骰子
			

//常量定义
#define MAX_WEAVE					50									//最大组合
#define MAX_PENG					7									//最大组合
#define MAX_INDEX					34									//最大索引
#define MAX_COUNT					14									//最大数目
#define MAX_REPERTORY				136									//最大库存
#define MAX_REPERTORY_WUFENG		108									//无风最大库存
#define MAX_REPERTORY_HZWUFENG		112									//红中无风最大库存
#define MAX_REPERTORY_HUANGZHOU     120                                 //黄州晃晃最大库存
#define TWO_THREE_HUANGZHOU         84                                  //黄州晃晃2-3人最大库存
#define TWO_THREE_REPERTORY         100                                 //罗田,浠水2-3人最大库存

//扑克定义
#define HEAP_FULL_COUNT				26									//堆立全牌

#define MAX_RIGHT_COUNT				1									//最大权位DWORD个数			

#define		GAME_TYPE_SICHUAN  0                      //四川麻将
#define		GAME_TYPE_CHENGDU  1					   //成都麻将
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
//服务器命令结构
#define SUB_S_GAME_ROLL_DICE		111									//掷骰子确认庄家
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_OUT_CARD				101									//出牌命令
#define SUB_S_SEND_CARD				102									//发送扑克
#define SUB_S_OPERATE_NOTIFY		104									//操作提示
#define SUB_S_OPERATE_RESULT		105									//操作命令
#define SUB_S_GAME_END				106									//游戏结束
#define SUB_S_TRUSTEE				107									//用户托管
#define SUB_S_CHI_HU				108									//
#define SUB_S_GANG_SCORE			110									//
#define SUB_S_GAME_SELECT_HU		111									//选牌后获得胡牌信息	
#define SUB_S_GAME_TING				112									//获得听牌信息
#define SUB_S_GAME_OUT_TING			113
#define SUB_S_REQ_PIAODANG				114									//请求漂和档 客户端发表示看其他玩家的飘和档 服务器发表示要求客户端选择飘和档
#define SUB_S_ACK_PIAODANG				115									//返回漂和档 客户端发表示自己的飘和档)	服务器发表示自己和其他玩家的飘和档
#define SUB_S_REQ_ROOM_INFO				116									//
#define SUB_S_ACK_ROOM_INFO				117									//
#define SUB_S_ACK_COUNT_INFO			118									//	客户端->服务器 结算请求
#define SUB_S_HIDEQFJ_NOTICE            119                                 //  隐藏客户端报清风将的通知
#define SUB_S_ACK_COUNT_INFO_RES        131									//	客户端没有请求到结算请求回应
#define SUB_S_ACK_COUNT_INFO_DATA        132									//  宝的信息

//2016.5.20
#define SUB_S_HUANPAI 120				//换牌
#define SUB_S_XUANQUE 121				//选缺
#define SUB_S_XUANQUE_NOTICE 122		//通知开始选缺
#define SUB_S_HUANPAI_CHENGDU 123				//换牌

#define SUB_S_HUANPAI_ANDROID 124

#define SUB_S_HUANPAI_NOTICE  125				//换牌

#define SUB_S_ANDROID_RULE         200                                 //发送规则给机器人

#define SUB_S_MASTER_HANDCARD		220									//
#define SUB_S_MASTER_LEFTCARD		221									//剩余牌堆


//杠牌得分
struct GDMJ_tagGangScore
{
	bool		bMingGang;							//杠个数
	BYTE		cbGangCount;						//杠个数
	LONGLONG	lScore[MAX_WEAVE][GAME_PLAYER];		//每个杠得分
	BYTE		cbGangCount_Single;
};

//碰牌保存
struct GDMJ_tagEnjoinPengCount
{
	BYTE		cbEnjoinCount;						//杠个数
	BYTE		cbEnjoinCard[MAX_PENG];					//禁碰的牌
};

//游戏状态
struct GDMJ_CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//基础金币
	WORD							wBankerUser;						//庄家用户
	bool							bTrustee[GAME_PLAYER];				//是否托管
};

struct GDMJ_CMD_S_DingZhuang
{
	WORD							wBankerUser;
	WORD							wSice;
};

//游戏状态
struct GDMJ_CMD_S_StatusPlay
{
	//游戏变量
	LONGLONG						lCellScore;									//单元积分
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	WORD							wSliceCount;								//色子数

	BYTE							cbHunIndex;									// 混
	BYTE							cbPiIndex;									// 皮
	BYTE							cbLaiPiIndex;								// 赖皮
	BYTE							cbBaoIndex;								    // 宝
	BYTE							cbPantherType;								// 将 风 单 色豹
	bool                            cbShowBao[GAME_PLAYER];                     // 亮宝
	BYTE							cbFaCaiNum[GAME_PLAYER];					// 发财个数

	//状态变量
	BYTE							cbActionCard;								//动作扑克
	DWORD							cbActionMask;								//动作掩码
	BYTE							cbLeftCardCount;							//剩余数目
	bool							bTrustee[GAME_PLAYER];						//是否托管
	WORD							wWinOrder[GAME_PLAYER];						//
	BYTE							cbWinCout;									//获胜数目

	//出牌信息
	WORD							wOutCardUser;								//出牌用户
	BYTE							cbOutCardData;								//出牌扑克
	BYTE							cbDiscardCount[GAME_PLAYER];				//丢弃数目
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//丢弃记录

	//扑克数据
	BYTE							cbCardCount;								//扑克数目
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbSendCardData;								//发送扑克

	//组合扑克
	BYTE							cbWeaveCount[GAME_PLAYER];					//组合数目
	GDMJ_CMD_WeaveItem				WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克

	//堆立信息
	WORD							wHeapHand;									//堆立头部
	WORD							wHeapTail;									//堆立尾部
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆牌信息


	BYTE							nQueColor[GAME_PLAYER];						//缺色
	BYTE							nChiHuCard[GAME_PLAYER][60];
	WORD							nChiHuCardNum[GAME_PLAYER];
	bool							bHasHu[GAME_PLAYER];
	bool							bHasTing[GAME_PLAYER];									//自己是否听牌

	LONGLONG						nGangScore[GAME_PLAYER];						//当局杠分
	BYTE							cbGangSiceNum[16];							//黄州晃晃每次杠的骰子点数
	DWORD                           dwQingFengJiang[4];                         //报清风将标志位
};

//游戏开始
struct GDMJ_CMD_S_GameStart
{
	LONG							lSiceCount;									//骰子点数
	WORD							wBankerUser;								//庄家用户
	WORD							wCurrentUser;								//当前用户
	DWORD							cbUserAction;								//用户动作
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];			//扑克列表
	WORD							wHeapHand;									//堆立牌头
	WORD							wHeapTail;									//堆立牌尾
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//堆立信息
	BYTE							cbLeftCardCount;							//

	BYTE							nLeftTime;									//换牌倒计时
	BYTE							cbPantherType;								//0x01将豹 0x02风豹 0x04单豹 0x08色豹 

	WORD							wHun;
	WORD							wPi;
	WORD							wLaiPi;

	//黄州晃晃
	WORD                            wBao; // 宝
};

struct GDMJ_CMD_S_StateHuanPai
{
	WORD							wBankerUser;								//庄家用户
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//换牌倒计时
	bool                            bHuan;									    //是否已经换了
};

struct GDMJ_CMD_S_StateXuanQue
{
	WORD							wBankerUser;								//庄家用户
	BYTE							cbCardData[MAX_COUNT];						//扑克列表
	BYTE							cbLeftCardCount;							//
	BYTE							nLeftTime;									//XuanQue倒计时
	BYTE                            nQueColor;									
};
//出牌命令
struct GDMJ_CMD_S_OutCard
{
	WORD							wOutCardUser;						//出牌用户
	BYTE							cbOutCardData;						//出牌扑克
};

//发送扑克
struct GDMJ_CMD_S_SendCard
{
	BYTE					cbCardData;			//扑克数据
	DWORD					cbActionMask;		//动作掩码
	WORD					wCurrentUser;		//当前用户
	bool					bTail;				//末尾发牌
	BYTE                    nLeftTime;
	bool					bHasHuPai;			//是否已经胡
};

//结算信息请求
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
//操作房间
struct GDMJ_CMD_S_OperateReqRoomInfo
{
	BYTE							cbUser;		//玩家id
};

//操作房间
struct GDMJ_CMD_S_OperateAckRoomInfo
{
	//黄石
	BYTE			bGameTypeIndex;		//2有风，1没风，3黄州，4溪水
	BYTE			bGameTypeQing;		//2有门清 ，1 没门清
	BYTE			bGameMaxScore;		//封顶
	BYTE			bGameMaxScoreGold;	//金顶
	BYTE			bFendDing;			//只上封顶
	BYTE			bJingDing;			//金顶
	BYTE            bAllOther;          //全求人 0 无全求人 1 有全求人
	BYTE            bQiangGanghu;       //抢杠胡
	BYTE			bPantherType;		//将豹  风豹 单豹 色豹
	BYTE            bPlayerNum;         //游戏人数
	BYTE			bPiaoShu;			//漂数
	BYTE			bBaoIsOutCard;      //0有宝不可打，1有宝可打
	BYTE			bGameBaseScore;		//底分
	DWORD           dwClubId;           //俱乐部ID
};

//操作飘和档
struct GDMJ_CMD_S_OperateReqPiaoDang
{
	BYTE							cbUser;								//玩家id
};

//操作飘和档
struct GDMJ_CMD_S_OperateAckPiaoDang
{
	BYTE							cbUser;								//玩家id
	DWORD							cbPiao;								//飘
	DWORD							cbDang;								//档
};

//操作提示
struct GDMJ_CMD_S_OperateNotify
{
	WORD							wResumeUser;						//还原用户
	DWORD							cbActionMask;						//动作掩码
	BYTE							cbActionCard;						//动作扑克
	bool							bHasHuPai;
};

//操作命令
struct GDMJ_CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	DWORD							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
	WORD                            wGangSiceNum;                       //杠后骰子点数
};

//隐藏报警的通知
struct  GDMJ_CMD_S_HideQFJNotice 
{
	WORD                             wPeratorUser;                      //操作用户
};

//用户托管
struct GDMJ_CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户
};

//

//效验类型
enum enEstimatKind
{
	EstimatKind_OutCard,			//出牌效验
	EstimatKind_GangCard,			//杠牌效验
	EstimatKind_XiGangCard,			//喜杠效验
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
//客户端命令结构

#define SUB_C_OUT_CARD				1									//出牌命令
#define SUB_C_OPERATE_CARD			3									//操作扑克
#define SUB_C_TRUSTEE				4									//用户托管
#define SUB_C_SELECT_CARD			5									//选择牌
#define SUB_C_GET_TING				9									//听牌
#define SUB_C_GET_TING_CARDS		10									//听牌的标签页

#define SUB_C_MASTER_LEFTCARD		6									//剩余牌堆
#define SUB_C_MASTER_CHEAKCARD		7									//选择的牌
#define SUB_C_MASTER_ZHANIAO		8									//扎鸟
//
//2016.5.20
#define SUB_C_HUANPAI		11				//换牌
#define SUB_C_XUANQUE		12				//选缺

#define CARD_COLOR_NULL			0
#define CARD_COLOR_TONG			1
#define CARD_COLOR_WAN			2
#define CARD_COLOR_TIAO			3
//出牌命令
struct GDMJ_CMD_C_OutCard
{
	BYTE							cbCardData;							//扑克数据
};

//选牌命令
struct GDMJ_CMD_C_SelectCard
{
	BYTE							cbCardData;
};

//获取听牌数据
struct GDMJ_CMD_C_GetTingCard
{
	BYTE							cbCardData;
};

//获得听牌标签
struct GDMJ_CMD_C_TingCards
{
	BYTE							cbCardData;
};

//操作命令
struct GDMJ_CMD_C_OperateCard
{
	BYTE							cbOperateCard;						//操作扑克
	DWORD							cbOperateCode;						//操作代码
};

//用户托管
struct GDMJ_CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
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

	bool   operator <  (const   GDMJ_Card_Info&   rhs   )  const   //升序排序时必须写的函数
	{  
		return   nLen   <   rhs.nLen;
	}
};

struct GDMJ_MaterCheckCard
{
	BYTE							cbCheakCard;						//操作代码
};

struct GDMJ_MaterNiaoCout
{
	BYTE							cbNiaoCout;							//操作代码
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
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	DWORD							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
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
	BYTE							cbHunIndex;									// 混
	BYTE							cbPiIndex;									// 皮
	BYTE							cbLaiPiIndex;								// 赖皮
	BYTE                            cbBaoIndex;                                 // 宝
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
	WORD							wBaoPlayer;					// 包胡玩家 如果没有包胡设10
	BYTE							cbChiHuCard;						//
	std::vector<LONGLONG>            lGameScore;
	DWORD							dwChiHuRight;				// 胡牌类型
	BYTE							cbGenCount;							//
	BYTE							cbHuScore;					// 胡牌基本分
	BYTE							cbHuScoreOther;             // 闲家底分
	std::vector<LONGLONG>			lMenHun;					// 闷混分/开口
	std::vector<LONGLONG> 			lGang;						// 总杠分
	std::vector<LONGLONG> 			lPiao;						// 飘/皮杠次数
	std::vector<LONGLONG> 			lDang;						// 档/赖杠次数
	std::vector<LONGLONG> 			lMingGang;					// 明杠分数 / 明杠
	std::vector<LONGLONG> 			lAnGang;					// 暗杠分数 / 暗杠
	std::vector<LONGLONG> 			lXiGang;					// 喜杠分数 / 番数
	std::vector<LONGLONG> 			lDianGang;					// 点杠分数 
	std::vector<LONGLONG> 			lHuPaiScore;				// 胡牌分数
	std::vector<LONGLONG> 			lBaMao;						// 拔毛分数
	std::vector<LONGLONG> 			lGangHu;					// 杠胡分数
	BYTE							cbSuHu;						// 0假1真
	BYTE							cbQiDui;					// 0假1真	

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

struct GDMJ_CMD_S_ChiHu_HM // 黄梅
{
	BYTE							cbChiHuCard;
	WORD							wProviderUser;
	std::vector<DWORD>				dwChiHuRight;				// 胡牌类型
	std::vector<DWORD>				wChiHuUser;					// 胡牌方
	std::vector<LONGLONG> 			lMingGang;					// 明杠分数 / 明杠
	std::vector<LONGLONG> 			lAnGang;					// 暗杠分数 / 暗杠
	std::vector<LONGLONG> 			lHuPaiScore;				// 胡牌分数
	std::vector<LONGLONG> 			lFaCaiScore;				// 发财分数
	std::vector<LONGLONG>			lBaiBanScore;				// 白板分数
	std::vector<LONGLONG>			lHongZhongScore;			// 红中分数

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

//获得听牌标记
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
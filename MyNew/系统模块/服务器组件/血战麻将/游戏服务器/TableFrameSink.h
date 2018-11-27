#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//枚举定义

#define IDI_TIMER_HUANPAI			1
#define IDI_TIMER_XUANQUE			2
#define IDI_TIMER_CHUPAI			3
#define IDI_TIMER_ACTION			4
#define IDI_TIMER_PIAODANG			5

#define TIME_HUANPAI				8
#define TIME_XUANQUE				8
#define TIME_CHUPAI					10
#define TIME_ACTION					8
#define TIME_PIAODANG				12


#define GAME_TYPE_ITEM_WITH_FENG	0  // 有风麻将
#define GAME_TYPE_ITEM_NO_FENG		1  // 无风麻将

enum 
{
	TYPE_FENGBAO  = 0x01, // 麻城风翻
	TYPE_JIANGBAO = 0x02, // 麻城将翻
	TYPE_DANBAO   = 0x04, // 麻城连宝翻
	TYPE_SEBAO    = 0x08
};


//////////////////////////////////////////////////////////////////////////
//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
public:
	struct TingTags
	{
		std::vector<BYTE>				m_TingTags;
	};

	BYTE cbTemp;

	GDMJGameRecord						m_kGameRecord;
	BYTE								m_bGameTypeIdex;
	DWORD								m_bGameRuleIdex;
	BYTE								m_bGameBaseScore;//底分
	BYTE								m_bGameGuDingMa;
	BYTE								m_bQiangGangHu; //是否有抢杠胡，0没有，1有(黄州晃晃，黄州红中杠)黄梅 0小胡有宝不可抢杠  1 小胡有宝可抢杠
	BYTE								m_bGameCardType;//有风，无风，红中
	BYTE								m_bAllOther; //	全求人 (溪水，黄州红中杠)
	BYTE								m_cbPantherType; // 0x01风豹，0x02将豹，0x04单豹，0x08色豹(麻城0x01风翻，0x02将翻，0x04联宝翻)
	BYTE                                m_cbPlayerNum;  //游戏人数
	//黄石
	BYTE								m_bGameTypeIndex;//2罗田，3黄州晃晃, 4浠水, 6黄州红中杠,7黄梅,8英山，9麻城
	BYTE								m_bGameTypeQing;//2有门清 ，1 没门清(麻城写做满跑，勾选满跑即有门清，不勾即没门清)

	BYTE								m_bLimitIP;
	BYTE								m_bOperateCard;
	DWORD								m_bUserOperate;

	BYTE								m_bGameMaxScore;		//封顶
	BYTE								m_bGameMaxScoreGold;	//金顶
	BYTE								m_bPiaoShu;				//飘数（0不漂，1漂1，2漂2 ...10漂10）（黄梅）
	BYTE								m_bBaoIsOutCard;		//0有宝不可打，1有宝可打  2 无宝（黄梅）(英山时用作包或不包，0不包，1包)
	BYTE								m_bFendDing;			//只上封顶
	BYTE								m_bJingDing;			//金顶
	int									m_nBaseScore;
	int									m_nGangScore;
	std::vector<int>					m_vBaseScore;
protected:
	GDMJ_CMD_S_ChiHu ChiHu;
	std::vector<GDMJ_CMD_S_ChiHu>		m_kChiHuInfoList;
	GDMJ_CMD_S_ChiHu_HM ChiHuHM;
	std::vector<GDMJ_CMD_S_ChiHu_HM>	m_kChiHuInfoListHM;
	std::vector<GDMJ_CMD_S_GangScore>	m_kGangInfoList;

public:

	BYTE                            m_cbMasterCheckCard[GAME_PLAYER];
	static const BYTE				m_cbCardWord[MAX_CARD_NUM];		//扑克数据
	TingTags						m_sTingTags[GAME_PLAYER];
	//游戏变量
protected:
	DWORD							m_bPlayPiao[GAME_PLAYER];				//飘
	DWORD							m_bPlayDang[GAME_PLAYER];				//裆
	BYTE							m_bSelectedPiaoDang[GAME_PLAYER];		//是否选过飘和档
	BYTE							m_bOutHun[GAME_PLAYER];
	BYTE							m_bOpenMouth[GAME_PLAYER];
	int								m_nUsefulOper[GAME_PLAYER];
	DWORD							m_nUsefulQingFengJiang[GAME_PLAYER];
	DWORD							m_nCheckQingFengJiang[GAME_PLAYER];
	BYTE                            m_bOpenProvider[GAME_PLAYER];           //第三手提供吃碰者 
//黄州 
	int								m_iBaoIndex;                            // 定宝的牌
	int                             m_iStartBaoNum[GAME_PLAYER];            // 起手的宝数量
	int                             m_iOpenMouthBaoNum[GAME_PLAYER];        // 开口后宝的数量
	bool                            m_bShowBao[GAME_PLAYER];                // 亮宝
	int                             m_iBaoNum[GAME_PLAYER];                 // 玩家获得的宝数量 
	BYTE                            m_cbSiceCountPi;                        // 定皮的骰子点数或骰子的较小点数
	bool                            m_bPiGang;                              // 皮牌是否已经被杠走（溪水共用）
	int								m_cGangCountXS;							// 一局游戏杠的总次数(溪水用)
	BYTE							m_cbGangSiceNum[16];		            // 黄州晃晃每次杠的骰子点数
	// 修改报清风将20180524
	bool                            m_bTalkQingFengQing[GAME_PLAYER];       // 是否报了清风将 （改罗田黄州浠水）20180524
	BYTE							m_cbQiangGangHu[GAME_PLAYER];			// 胡牌是否是抢杠胡 0不是，1是
	BYTE							m_cbTalkAllOther[GAME_PLAYER];			// 是否报了全求人0没有，1已报
	BYTE							m_cbPeng[GAME_PLAYER];					//是否碰过牌(黄梅用)
	// shuangshi
	BYTE							m_bDaHu;
	WORD							m_wBankerUserNext;
	LONG							m_lSiceCount;							// 骰子点数
	WORD							m_wBankerUser;							// 庄家用户
	LONGLONG						m_lGameScore[GAME_PLAYER];				// 游戏得分
	LONGLONG						m_nHistoryScore[GAME_PLAYER];			// 本桌历史分
	LONGLONG						m_nGangScoreNow[GAME_PLAYER];			// 当局杠分
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	// 用户扑克
	bool							m_bTrustee[GAME_PLAYER];				// 是否托管
	BYTE							m_cbPiGenCount[GAME_PLAYER];			//
	BYTE							m_cbLaiGenCount[GAME_PLAYER];			//
	BYTE							m_cbGenCount[GAME_PLAYER];				//
	bool							m_bPlayStatus[GAME_PLAYER];				//
	BYTE							m_cbWinCount;							//
	WORD							m_wWinOrder[GAME_PLAYER];				//
	LONG							m_lGameTax[GAME_PLAYER];				//
	GDMJ_tagGangScore				m_GangScore[GAME_PLAYER];				//杠分
	GDMJ_tagEnjoinPengCount			m_EnjoinPengCount[GAME_PLAYER];			//碰了多少次

	bool							m_HuanPai[GAME_PLAYER];					//换牌
	BYTE							m_nQueColor[GAME_PLAYER];				//缺色
	BYTE							m_cbHuanPaiData[GAME_PLAYER][3];		//玩家所要换的牌

	bool							m_bHasHuPai[GAME_PLAYER];
	WORD							m_nFanShu[GAME_PLAYER];
	DWORD							m_dwClubID;//俱乐部id

	BYTE							m_cbPantherTypeServer;				// 0x01风豹，0x02将豹，0x04单豹，0x08色豹
	BYTE							m_cbFaCaiNum[GAME_PLAYER];			//发财的个数(黄梅)
	DWORD							m_dwChiHuUserTemp;					//吃胡玩家(黄梅)
	BYTE							m_cbFrontHunIndex;					//上局的混(麻城连宝翻用)

	//出牌信息
protected:
	WORD							m_wOutCardUser;							//出牌用户
	BYTE							m_cbOutCardData;						//出牌扑克
	BYTE							m_cbOutCardCount;						//出牌数目
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//丢弃数目
	BYTE							m_cbDiscardCard[GAME_PLAYER][60];		//丢弃记录

	//发牌信息
protected:
	BYTE							m_cbSendCardData;						//发牌扑克
	BYTE							m_cbSendCardCount;						//发牌数目
	BYTE							m_cbLeftCardCount;						//剩余数目
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//库存扑克

	//堆立变量
protected:
	WORD							m_wHeapHand;							//堆立头部
	WORD							m_wHeapTail;							//堆立尾部
	BYTE							m_cbHeapCardInfo[4][2];					//堆牌信息

	//运行变量
protected:
	WORD							m_wResumeUser;							//还原用户
	WORD							m_wCurrentUser;							//当前用户
	WORD							m_wProvideUser;							//供应用户
	WORD							m_gProvideUser;							//点杠用户
	BYTE							m_cbProvideCard;						//供应扑克

	//状态变量
protected:
	bool							m_bSendStatus;							//发牌状态
	bool							m_bGangStatus;							//抢杆状态
	bool                            m_bGangHuaStatus;						//点杠花

	bool							m_bGangOutStatus;						//
	int                             m_iHuScore[GAME_PLAYER];                //一圈内胡上家的分数
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//禁止吃胡
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//禁止吃碰
	std::vector<int>				m_vecEnjoinChiPeng[GAME_PLAYER];	
	bool                            m_bEnjoyGang[GAME_PLAYER];              //是否能明杠
	bool							m_bNeedCalGang;
	//用户状态
public:
	bool							m_bResponse[GAME_PLAYER];				//响应标志
	DWORD							m_cbUserAction[GAME_PLAYER];			//用户动作
	BYTE							m_cbOperateCard[GAME_PLAYER];			//操作扑克
	DWORD							m_cbPerformAction[GAME_PLAYER];			//执行动作
	BYTE							m_bIsFirstTurn[GAME_PLAYER];			//是否用户第一次动作(黄梅查看发财)

	//组合扑克
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//组合数目
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];//组合扑克

	//结束信息
protected:
	BYTE							m_cbChiHuCard;							//吃胡扑克
	DWORD							m_dwChiHuKind[GAME_PLAYER];				//吃胡结果
	CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//
	CChiHuRight						m_ChiHuRightTemp[GAME_PLAYER];			//检查胡分时用
	CChiHuRight						m_ChiHuRightHM[GAME_PLAYER];			//胡牌类型（黄梅用）
	WORD							m_wProvider[GAME_PLAYER];				//
	BYTE                            m_cbHuType[GAME_PLAYER];                //大胡标志位 0 没有胡  1 小胡  2 大胡 
	BYTE							m_wMaCardScore;							//马牌积分
	std::vector<GDMJ_MaPaiInfo>		m_wMaCards;								//马牌
	std::vector<BYTE>				m_vHuPlayers;							//连胡次数

	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数

private:
	bool							m_isDingZhuang;
	bool							m_isRollDice;
	bool							m_isPiaoDang;
	bool							m_isFirstPiaoDang;
	int								m_nHunIndex;
	int								m_nPiIndex;
	int								m_nPiIndexHM;
	int								m_nLaiPiIndex;
	std::vector<tagPengItem>		m_guoshoupeng;
	int 							m_lSelfGangCount[GAME_PLAYER];		// 杠次数
	int 							m_lOtherGangCount[GAME_PLAYER];		// 点杠次数
	bool							m_bFirstGang;
	GDMJ_AllEndInfo                 m_GameEndInfo;
	int                             m_GameEndSize;


	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { }
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();
	//清空结算信息 
	virtual VOID  ResAccountInfo(){ m_GameEndSize = 0;};//m_GameEndInfo.resizeData();}

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){  return 0; };
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; };
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//游戏事件
public:
	//掷骰子确认庄家
	virtual bool OnEventDingZhuang(int nChairID);
	//掷骰子
	virtual bool OnEventRollSice();
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }

	//网络接口
public:
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }
		
	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex, BYTE bBaseScore, BYTE bGuDingMa, BYTE bQiangGangHu, BYTE bGameType);
	virtual void SetPrivateFushunInfo(BYTE bGameTypeIndex, BYTE bGameTypeQing, BYTE bLimitIP,
		BYTE bGameMaxScore,BYTE bGameMaxScoreGold,BYTE bPiaoShu,BYTE bBaoIsOutCard,BYTE bFendDing,BYTE bJingDing,BYTE bAllOther, BYTE	cbPantherType,BYTE cbPlayerNum, DWORD dwClubID = 0);

	virtual void SetCreateUserID(DWORD	dwUserID);

	//游戏事件
protected:
	//用户出牌
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//用户操作
	bool OnUserOperateCard(WORD wChairID, DWORD cbOperateCode, BYTE cbOperateCard,BYTE bCheck = 1);
	//用户选牌
	bool OnSelectCard(WORD wChairID, BYTE cbCardData);
	//用户听牌
	bool OnGetTingCard(WORD wChairID, BYTE cbCardData);
	//听牌标签
	bool OnGetTingCardTag(WORD wChairID, BYTE cbCardData);
	bool UpdateTingCardTag(WORD wChairID, BYTE cbCardData,bool isAdd);
	bool OnGetTingCardTagClient(WORD wChairID,BYTE cbCardData);
	//辅助函数
	void RandCard();
	bool CheckRandCard();
	//飘档选择
	bool OnUserPiaoDang(WORD wChairID, DWORD cbPiao, DWORD cbDang, IServerUserItem * pIServerUserItem);
protected:
	//发送操作
	bool SendOperateNotify();
	//发送飘裆操作
	bool SendPiaoDang(WORD wChairID);
	//派发扑克
	bool DispatchCardData(WORD wCurrentUser,bool bTail=false);
	//响应判断
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//
	void ProcessChiHuUser( WORD wChairId, bool bJustScore, int &m_iGetScore,bool bGiveUp, CChiHuRight &chr);
	//
	int FiltrateRight( WORD wChairId,CChiHuRight &chr );
	int FiltrateRightNew( WORD wChairId,CChiHuRight &chr,int &nBaseScore, int &nBaseFan );
	//黄州晃晃算分
	int  StatisticalFraction( WORD wChairId,CChiHuRight &chr,int &nBaseScore, int &nBaseFan,int   &nPlayerScore,DWORD  bAction = WIK_NULL);
	//黄州晃晃积分结算
	void AccountGameScore(WORD wChairId, bool bGiveUp, CChiHuRight &chiHuPlayer,int &m_iGetScore, bool  bJustScore = false);
	//浠水算番
	void XiShuiAccountFanShu(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, CChiHuRight &chr, int nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//浠水算分
	int XiShuiAccountScore(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, bool bGiveUp, bool bCheckScore = false);
	//黄州红中杠算分
	int AccountScoreHZHZG(WORD wChairId, bool bCheckScore = false);
	//黄州红中杠算番
	int AccountFanShuHZHZG(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//黄梅算分
	int AccountScoreHM(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId);
	//黄梅算分辅助
	void AccountScoreHMFZ(const BYTE cbCardIndex[MAX_INDEX],WORD wChairId,int &iScore,int &iFaCaiScore,int &iBaiBanScore,int &iHongZhongScore,CChiHuRight &chr);
	// 发送结束分数(黄梅)
	void SendGameEndScore();
	//英山算分
	int AccountScoreYS(WORD wChairId, bool bCheckScore = false);
	//英山算番
	void AccountFanShuYS(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//麻城算分
	int AccountScoreMC(WORD wChairId, bool bCheckScore = false);
	//麻城算番
	void AccountFanShuMC(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER]);

	void OnUserHuanPaiSiChuan(WORD wChairID,GDMJ_CMD_C_HuanPai* pHuanPai);
	void OnUserHuanPaiChengDu(WORD wChairID,GDMJ_CMD_C_HuanPai* pHuanPai);
	void OnUserHuanPaiEnd_SiChuan();
	void OnUserHuanPaiEnd_ChengDu();

	BYTE GetQueColor(WORD wChairID);
public:
	void starGameRecord();
	void addGameRecordAction(GDMJ_CMD_S_OperateResult kNetInfo);
	void addGameRecordAction(GDMJ_CMD_S_SendCard kNetInfo);
	void addGameRecordAction(GDMJ_CMD_S_OutCard kNetInfo);
	void addGameRecordAction(GDMJ_CMD_S_ChiHu kNetInfo);
public:
	void sendMasterHandCard(int nChairdID);
	void sendMasterLeftCard(int nChairldID);
	BYTE getSendCardData(WORD dwSendUser);
public:
	bool hasRule(DWORD cbRule);
	void sendHuanPaiNotice();
	void sendXuanQueNotice();
	void startPlay();
public:
	bool cheakSelfGangPi(int nIsLaiPi,WORD wChairID,BYTE cbOperateCard,bool& bPublic);
	bool cheakSelfGang(WORD wChairID,BYTE cbOperateCard,bool& bPublic);
	void sendGangScore(SCORE lScore,WORD wTargetID,WORD wProvideID,bool bAn = false);
	void sendGangScoreReal(SCORE lScore,WORD wTargetID,WORD wProvideID,bool bAn = false);
	void SendOperateResulet(WORD wChairID,WORD wProvideID,DWORD cbOperateCode, BYTE cbOperateCard, DWORD wGangSiceNum = 0);
	void SendHideQfjNotice(WORD  wChairID);//隐藏报警按钮的通知(当前牌型不符合报清风将条件)

	bool removeCardByAction(WORD wTargetUser,DWORD cbAction,BYTE cbTargetCard); //被动操作时
	
	void getGangScore(std::vector<LONGLONG> * pVec,int nScore,WORD wChairId,WORD ProvideUser);
	void getHuScore(std::vector<LONGLONG> * pVec,int nScore,int nBaseFan,WORD wChairId,WORD ProvideUser,bool bIsDahu,int nBaoPlayer, int &iHuScore);
	//黄州晃晃算分
	void getHuScore(std::vector<LONGLONG> * pVec,int nScore, int nPlayerScore, int nBaseFan,WORD wChairId,WORD ProvideUser,int nBaoPlayer, int &iHuScore);
	int FloorScore(int nScore);
	int FloorScore5(int nScore);
	int FloorScore10(int nScore);


	void chaJiaoAndHuaZhu(int cbLeftUserCount,LONGLONG lHuaZhuScore[],LONGLONG lChaJiaoScore[],WORD	 lChaJiaoFanShuMax[],LONGLONG lGangScore[]);
	void chaJiaoAndHuaZhu_YiChang(int cbLeftUserCount,LONGLONG lHuaZhuScore[],LONGLONG lChaJiaoScore[],WORD	 lChaJiaoFanShuMax[],LONGLONG lGangScore[]);

	DWORD AnalyseQiangGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	DWORD AnalyseMagicInexCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	DWORD AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, int nCurrentUser, BYTE ucBaoHou = 0, BYTE QFJ = 0);
	DWORD AnalyseChiHuCardBD(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	void  ClearChiHuData()
	{
		ChiHu.wChiHuUser = 0;
		ChiHu.wProviderUser = 0;
		ChiHu.wBaoPlayer = 0;
		ChiHu.cbChiHuCard = 0;
		ChiHu.dwChiHuRight = 0;
		ChiHu.cbGenCount = 0;
		ChiHu.cbHuScore = 0;
		ChiHu.cbHuScoreOther = 0;
		ChiHu.cbSuHu = 0;
		ChiHu.cbQiDui = 0;

		ChiHu.lMenHun.clear();	
		ChiHu.lGang.clear();		
		ChiHu.lPiao.clear();		
		ChiHu.lDang.clear();		
		ChiHu.lMingGang.clear();	
		ChiHu.lAnGang.clear();	
		ChiHu.lXiGang.clear();	
		ChiHu.lDianGang.clear();	
		ChiHu.lHuPaiScore.clear();
		ChiHu.lBaMao.clear();		
		ChiHu.lGangHu.clear();	
		ChiHu.lGameScore.clear();
	}
	void  ClearChiHuDataHM()
	{
		ChiHuHM.cbChiHuCard = 0;
		ChiHuHM.wProviderUser = 0;
		ChiHuHM.dwChiHuRight.clear();
		ChiHuHM.wChiHuUser.clear();
		ChiHuHM.lMingGang.clear();
		ChiHuHM.lAnGang.clear();
		ChiHuHM.lHuPaiScore.clear();
		ChiHuHM.lFaCaiScore.clear();
		ChiHuHM.lBaiBanScore.clear();
		ChiHuHM.lHongZhongScore.clear();
	}

	void getChiHuCardList(WORD wChairID,std::vector<BYTE>& cbChiHuCardList);

	void gameStart();
	void checkState();
	void sendDingZhuang();
	void sendRollDice();

	void getScoreByMa(WORD wChiarID);
	BYTE getZiMianMaScore(BYTE card, WORD wChiarID);
	BYTE getGuDingMaScore(BYTE card, WORD wChairID);

	void setZhuangID(int zhuangID);

	void setCurrentHuPlayer(BYTE wChairID);
	int getLiuJuCount();

	bool checkIsGuoShouPeng(WORD chairID, BYTE card);

	bool IsLaiPi(int nIndex);
	bool IsHun(int nIndex);
	void DoFirstGang();
	void DoSicePiGang(); // 黄州晃晃判断皮与骰子的点数
	void DoSicePiGangXS(); // 溪水判断皮与骰子的点数
	bool bCheckAllOther(const BYTE cbCardIndex[MAX_INDEX]); // 检查英山的全求人
};

//////////////////////////////////////////////////////////////////////////

#endif

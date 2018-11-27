#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

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


#define GAME_TYPE_ITEM_WITH_FENG	0  // �з��齫
#define GAME_TYPE_ITEM_NO_FENG		1  // �޷��齫

enum 
{
	TYPE_FENGBAO  = 0x01, // ��Ƿ緭
	TYPE_JIANGBAO = 0x02, // ��ǽ���
	TYPE_DANBAO   = 0x04, // ���������
	TYPE_SEBAO    = 0x08
};


//////////////////////////////////////////////////////////////////////////
//��Ϸ������
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
	BYTE								m_bGameBaseScore;//�׷�
	BYTE								m_bGameGuDingMa;
	BYTE								m_bQiangGangHu; //�Ƿ������ܺ���0û�У�1��(���ݻλΣ����ݺ��и�)��÷ 0С���б���������  1 С���б�������
	BYTE								m_bGameCardType;//�з磬�޷磬����
	BYTE								m_bAllOther; //	ȫ���� (Ϫˮ�����ݺ��и�)
	BYTE								m_cbPantherType; // 0x01�籪��0x02������0x04������0x08ɫ��(���0x01�緭��0x02������0x04������)
	BYTE                                m_cbPlayerNum;  //��Ϸ����
	//��ʯ
	BYTE								m_bGameTypeIndex;//2���3���ݻλ�, 4�ˮ, 6���ݺ��и�,7��÷,8Ӣɽ��9���
	BYTE								m_bGameTypeQing;//2������ ��1 û����(���д�����ܣ���ѡ���ܼ������壬������û����)

	BYTE								m_bLimitIP;
	BYTE								m_bOperateCard;
	DWORD								m_bUserOperate;

	BYTE								m_bGameMaxScore;		//�ⶥ
	BYTE								m_bGameMaxScoreGold;	//��
	BYTE								m_bPiaoShu;				//Ʈ����0��Ư��1Ư1��2Ư2 ...10Ư10������÷��
	BYTE								m_bBaoIsOutCard;		//0�б����ɴ�1�б��ɴ�  2 �ޱ�����÷��(Ӣɽʱ�������򲻰���0������1��)
	BYTE								m_bFendDing;			//ֻ�Ϸⶥ
	BYTE								m_bJingDing;			//��
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
	static const BYTE				m_cbCardWord[MAX_CARD_NUM];		//�˿�����
	TingTags						m_sTingTags[GAME_PLAYER];
	//��Ϸ����
protected:
	DWORD							m_bPlayPiao[GAME_PLAYER];				//Ʈ
	DWORD							m_bPlayDang[GAME_PLAYER];				//��
	BYTE							m_bSelectedPiaoDang[GAME_PLAYER];		//�Ƿ�ѡ��Ʈ�͵�
	BYTE							m_bOutHun[GAME_PLAYER];
	BYTE							m_bOpenMouth[GAME_PLAYER];
	int								m_nUsefulOper[GAME_PLAYER];
	DWORD							m_nUsefulQingFengJiang[GAME_PLAYER];
	DWORD							m_nCheckQingFengJiang[GAME_PLAYER];
	BYTE                            m_bOpenProvider[GAME_PLAYER];           //�������ṩ������ 
//���� 
	int								m_iBaoIndex;                            // ��������
	int                             m_iStartBaoNum[GAME_PLAYER];            // ���ֵı�����
	int                             m_iOpenMouthBaoNum[GAME_PLAYER];        // ���ں󱦵�����
	bool                            m_bShowBao[GAME_PLAYER];                // ����
	int                             m_iBaoNum[GAME_PLAYER];                 // ��һ�õı����� 
	BYTE                            m_cbSiceCountPi;                        // ��Ƥ�����ӵ��������ӵĽ�С����
	bool                            m_bPiGang;                              // Ƥ���Ƿ��Ѿ������ߣ�Ϫˮ���ã�
	int								m_cGangCountXS;							// һ����Ϸ�ܵ��ܴ���(Ϫˮ��)
	BYTE							m_cbGangSiceNum[16];		            // ���ݻλ�ÿ�θܵ����ӵ���
	// �޸ı���罫20180524
	bool                            m_bTalkQingFengQing[GAME_PLAYER];       // �Ƿ�����罫 ������������ˮ��20180524
	BYTE							m_cbQiangGangHu[GAME_PLAYER];			// �����Ƿ������ܺ� 0���ǣ�1��
	BYTE							m_cbTalkAllOther[GAME_PLAYER];			// �Ƿ���ȫ����0û�У�1�ѱ�
	BYTE							m_cbPeng[GAME_PLAYER];					//�Ƿ�������(��÷��)
	// shuangshi
	BYTE							m_bDaHu;
	WORD							m_wBankerUserNext;
	LONG							m_lSiceCount;							// ���ӵ���
	WORD							m_wBankerUser;							// ׯ���û�
	LONGLONG						m_lGameScore[GAME_PLAYER];				// ��Ϸ�÷�
	LONGLONG						m_nHistoryScore[GAME_PLAYER];			// ������ʷ��
	LONGLONG						m_nGangScoreNow[GAME_PLAYER];			// ���ָܷ�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	// �û��˿�
	bool							m_bTrustee[GAME_PLAYER];				// �Ƿ��й�
	BYTE							m_cbPiGenCount[GAME_PLAYER];			//
	BYTE							m_cbLaiGenCount[GAME_PLAYER];			//
	BYTE							m_cbGenCount[GAME_PLAYER];				//
	bool							m_bPlayStatus[GAME_PLAYER];				//
	BYTE							m_cbWinCount;							//
	WORD							m_wWinOrder[GAME_PLAYER];				//
	LONG							m_lGameTax[GAME_PLAYER];				//
	GDMJ_tagGangScore				m_GangScore[GAME_PLAYER];				//�ܷ�
	GDMJ_tagEnjoinPengCount			m_EnjoinPengCount[GAME_PLAYER];			//���˶��ٴ�

	bool							m_HuanPai[GAME_PLAYER];					//����
	BYTE							m_nQueColor[GAME_PLAYER];				//ȱɫ
	BYTE							m_cbHuanPaiData[GAME_PLAYER][3];		//�����Ҫ������

	bool							m_bHasHuPai[GAME_PLAYER];
	WORD							m_nFanShu[GAME_PLAYER];
	DWORD							m_dwClubID;//���ֲ�id

	BYTE							m_cbPantherTypeServer;				// 0x01�籪��0x02������0x04������0x08ɫ��
	BYTE							m_cbFaCaiNum[GAME_PLAYER];			//���Ƶĸ���(��÷)
	DWORD							m_dwChiHuUserTemp;					//�Ժ����(��÷)
	BYTE							m_cbFrontHunIndex;					//�ϾֵĻ�(�����������)

	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][60];		//������¼

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����˿�
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�

	//��������
protected:
	WORD							m_wHeapHand;							//����ͷ��
	WORD							m_wHeapTail;							//����β��
	BYTE							m_cbHeapCardInfo[4][2];					//������Ϣ

	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	WORD							m_gProvideUser;							//����û�
	BYTE							m_cbProvideCard;						//��Ӧ�˿�

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//����״̬
	bool                            m_bGangHuaStatus;						//��ܻ�

	bool							m_bGangOutStatus;						//
	int                             m_iHuScore[GAME_PLAYER];                //һȦ�ں��ϼҵķ���
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����
	std::vector<int>				m_vecEnjoinChiPeng[GAME_PLAYER];	
	bool                            m_bEnjoyGang[GAME_PLAYER];              //�Ƿ�������
	bool							m_bNeedCalGang;
	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	DWORD							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����˿�
	DWORD							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���
	BYTE							m_bIsFirstTurn[GAME_PLAYER];			//�Ƿ��û���һ�ζ���(��÷�鿴����)

	//����˿�
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];//����˿�

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��˿�
	DWORD							m_dwChiHuKind[GAME_PLAYER];				//�Ժ����
	CChiHuRight						m_ChiHuRight[GAME_PLAYER];				//
	CChiHuRight						m_ChiHuRightTemp[GAME_PLAYER];			//������ʱ��
	CChiHuRight						m_ChiHuRightHM[GAME_PLAYER];			//�������ͣ���÷�ã�
	WORD							m_wProvider[GAME_PLAYER];				//
	BYTE                            m_cbHuType[GAME_PLAYER];                //�����־λ 0 û�к�  1 С��  2 ��� 
	BYTE							m_wMaCardScore;							//���ƻ���
	std::vector<GDMJ_MaPaiInfo>		m_wMaCards;								//����
	std::vector<BYTE>				m_vHuPlayers;							//��������

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

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
	int 							m_lSelfGangCount[GAME_PLAYER];		// �ܴ���
	int 							m_lOtherGangCount[GAME_PLAYER];		// ��ܴ���
	bool							m_bFirstGang;
	GDMJ_AllEndInfo                 m_GameEndInfo;
	int                             m_GameEndSize;


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();
	//��ս�����Ϣ 
	virtual VOID  ResAccountInfo(){ m_GameEndSize = 0;};//m_GameEndInfo.resizeData();}

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){  return 0; };
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){ return 0; };
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϸ�¼�
public:
	//������ȷ��ׯ��
	virtual bool OnEventDingZhuang(int nChairID);
	//������
	virtual bool OnEventRollSice();
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) { return false; }
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) { return false; }

	//����ӿ�
public:
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }
		
	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex, BYTE bBaseScore, BYTE bGuDingMa, BYTE bQiangGangHu, BYTE bGameType);
	virtual void SetPrivateFushunInfo(BYTE bGameTypeIndex, BYTE bGameTypeQing, BYTE bLimitIP,
		BYTE bGameMaxScore,BYTE bGameMaxScoreGold,BYTE bPiaoShu,BYTE bBaoIsOutCard,BYTE bFendDing,BYTE bJingDing,BYTE bAllOther, BYTE	cbPantherType,BYTE cbPlayerNum, DWORD dwClubID = 0);

	virtual void SetCreateUserID(DWORD	dwUserID);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, DWORD cbOperateCode, BYTE cbOperateCard,BYTE bCheck = 1);
	//�û�ѡ��
	bool OnSelectCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnGetTingCard(WORD wChairID, BYTE cbCardData);
	//���Ʊ�ǩ
	bool OnGetTingCardTag(WORD wChairID, BYTE cbCardData);
	bool UpdateTingCardTag(WORD wChairID, BYTE cbCardData,bool isAdd);
	bool OnGetTingCardTagClient(WORD wChairID,BYTE cbCardData);
	//��������
	void RandCard();
	bool CheckRandCard();
	//Ʈ��ѡ��
	bool OnUserPiaoDang(WORD wChairID, DWORD cbPiao, DWORD cbDang, IServerUserItem * pIServerUserItem);
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//����Ʈ�ɲ���
	bool SendPiaoDang(WORD wChairID);
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser,bool bTail=false);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//
	void ProcessChiHuUser( WORD wChairId, bool bJustScore, int &m_iGetScore,bool bGiveUp, CChiHuRight &chr);
	//
	int FiltrateRight( WORD wChairId,CChiHuRight &chr );
	int FiltrateRightNew( WORD wChairId,CChiHuRight &chr,int &nBaseScore, int &nBaseFan );
	//���ݻλ����
	int  StatisticalFraction( WORD wChairId,CChiHuRight &chr,int &nBaseScore, int &nBaseFan,int   &nPlayerScore,DWORD  bAction = WIK_NULL);
	//���ݻλλ��ֽ���
	void AccountGameScore(WORD wChairId, bool bGiveUp, CChiHuRight &chiHuPlayer,int &m_iGetScore, bool  bJustScore = false);
	//�ˮ�㷬
	void XiShuiAccountFanShu(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, CChiHuRight &chr, int nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//�ˮ���
	int XiShuiAccountScore(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, bool bGiveUp, bool bCheckScore = false);
	//���ݺ��и����
	int AccountScoreHZHZG(WORD wChairId, bool bCheckScore = false);
	//���ݺ��и��㷬
	int AccountFanShuHZHZG(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//��÷���
	int AccountScoreHM(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId);
	//��÷��ָ���
	void AccountScoreHMFZ(const BYTE cbCardIndex[MAX_INDEX],WORD wChairId,int &iScore,int &iFaCaiScore,int &iBaiBanScore,int &iHongZhongScore,CChiHuRight &chr);
	// ���ͽ�������(��÷)
	void SendGameEndScore();
	//Ӣɽ���
	int AccountScoreYS(WORD wChairId, bool bCheckScore = false);
	//Ӣɽ�㷬
	void AccountFanShuYS(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer);
	//������
	int AccountScoreMC(WORD wChairId, bool bCheckScore = false);
	//����㷬
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
	void SendHideQfjNotice(WORD  wChairID);//���ر�����ť��֪ͨ(��ǰ���Ͳ����ϱ���罫����)

	bool removeCardByAction(WORD wTargetUser,DWORD cbAction,BYTE cbTargetCard); //��������ʱ
	
	void getGangScore(std::vector<LONGLONG> * pVec,int nScore,WORD wChairId,WORD ProvideUser);
	void getHuScore(std::vector<LONGLONG> * pVec,int nScore,int nBaseFan,WORD wChairId,WORD ProvideUser,bool bIsDahu,int nBaoPlayer, int &iHuScore);
	//���ݻλ����
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
	void DoSicePiGang(); // ���ݻλ��ж�Ƥ�����ӵĵ���
	void DoSicePiGangXS(); // Ϫˮ�ж�Ƥ�����ӵĵ���
	bool bCheckAllOther(const BYTE cbCardIndex[MAX_INDEX]); // ���Ӣɽ��ȫ����
};

//////////////////////////////////////////////////////////////////////////

#endif

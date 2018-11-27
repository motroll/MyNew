#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "DlgCustomRule.h"
#include "Randkiss.h"
extern CRandKiss g_RandKiss;

//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	cbTemp = 0;
	m_bGameTypeIdex = 0;
	m_bGameRuleIdex = 0;
	m_bGameBaseScore = 0;
	m_bQiangGangHu = 0;
	m_pITableFrame = NULL;
	m_wBankerUserNext = INVALID_CHAIR;
	m_isDingZhuang = false;
	m_isRollDice = false;
	m_isPiaoDang = true;
	m_isFirstPiaoDang= true;
	m_bGameMaxScore = 50;
	m_bGameMaxScoreGold = 70;
	m_cbSiceCountPi = 0;
	m_bPiGang = false;
	m_cGangCountXS = -1;
	m_cbPantherType = 0;
	m_cbPantherTypeServer = 0;
	m_bPiaoShu = 0;		
	m_bBaoIsOutCard = 0;
	m_dwChiHuUserTemp = 100;
	m_cbFrontHunIndex = MAX_INDEX;
	ZeroMemory( m_nUsefulQingFengJiang, sizeof(m_nUsefulQingFengJiang));
	ZeroMemory( m_nCheckQingFengJiang, sizeof(m_nCheckQingFengJiang));
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_bTalkQingFengQing[i] = false;
	}
	ZeroMemory( m_cbGangSiceNum, sizeof(m_cbGangSiceNum));
	ZeroMemory( m_cbQiangGangHu, sizeof(m_cbQiangGangHu));
	// fushun
	ZeroMemory( m_bPlayPiao,sizeof(m_bPlayPiao) );
	ZeroMemory( m_bPlayDang,sizeof(m_bPlayDang) );
	ZeroMemory( m_bSelectedPiaoDang,sizeof(m_bSelectedPiaoDang) );
	ZeroMemory( m_cbFaCaiNum,sizeof(m_cbFaCaiNum) );
	ZeroMemory( m_cbPeng,sizeof(m_cbPeng) );
	
	memset(&m_GameEndInfo,0,sizeof(GDMJ_AllEndInfo));
	m_GameEndSize = 0;
	m_dwClubID = 0;
	m_vHuPlayers.clear();
	m_vBaseScore.clear();
	m_vBaseScore.push_back(1);
	m_vBaseScore.push_back(2);
	m_vBaseScore.push_back(5);
	m_vBaseScore.push_back(10);
	m_vBaseScore.push_back(20);
	m_vBaseScore.push_back(50);
	m_vBaseScore.push_back(100);

	RepositionSink();
}
const int BaseScoreArray[7] = {1,2,5,10,20,50,100};
const int GudingMaArray[4] = {2, 4, 6, 8};

const BYTE CTableFrameSink::m_cbCardWord[MAX_INDEX]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
};

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	m_vBaseScore.clear();
	m_GameEndInfo.cbFanShu.clear();
	m_GameEndInfo.cbGenCount.clear();
	m_GameEndInfo.cbMaPai.clear();
	m_GameEndInfo.kCardInfoList.clear();
	m_GameEndInfo.kChiHuInfoList.clear();
	m_GameEndInfo.kGangInfoList.clear();
	m_GameEndInfo.lChaJiaoScore.clear();
	m_GameEndInfo.lGameScoreTotal.clear();
	m_GameEndInfo.lHuaZhuScore.clear();
}

//�ӿڲ�ѯ
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	m_bGameTypeIdex = GAME_TYPE_CHENGDU;
	if (pCustomConfig->cbGameType1)
	{
		m_bGameTypeIdex = GAME_TYPE_SICHUAN;
	}
	if (pCustomConfig->cbGameType2)
	{
		m_bGameTypeIdex = GAME_TYPE_CHENGDU;
	}
	for (int i = 0;i<GAME_RULE_DIN_QUE;i++)
	{
		if (pCustomConfig->cbGameRule[i])
		{
			FvMask::Add(m_bGameRuleIdex,_MASK_(i+1));
		}
	}
	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	m_isPiaoDang= true;
	m_isFirstPiaoDang=true;

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory( m_GangScore,sizeof(m_GangScore) );
	ZeroMemory( m_EnjoinPengCount,sizeof(m_EnjoinPengCount) );
	ZeroMemory( m_cbGenCount,sizeof(m_cbGenCount) );
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );
	ZeroMemory( m_bPlayStatus,sizeof(m_bPlayStatus) );
	ZeroMemory( m_bIsFirstTurn,sizeof(m_bIsFirstTurn) );
	ZeroMemory( m_nGangScoreNow,sizeof(m_nGangScoreNow) );
	ZeroMemory( m_bOutHun,sizeof(m_bOutHun) );
	ZeroMemory( m_bOpenMouth,sizeof(m_bOpenMouth) );	
	ZeroMemory( m_nUsefulOper,sizeof(m_nUsefulOper) );	
	ZeroMemory( m_nUsefulQingFengJiang,sizeof(m_nUsefulQingFengJiang) );
	ZeroMemory( m_nCheckQingFengJiang,sizeof(m_nCheckQingFengJiang) );
	ZeroMemory( m_lSelfGangCount,sizeof(m_lSelfGangCount) );
	ZeroMemory( m_lOtherGangCount,sizeof(m_lOtherGangCount) );
	ZeroMemory( m_cbGangSiceNum, sizeof(m_cbGangSiceNum));
	ZeroMemory( m_cbFaCaiNum,sizeof(m_cbFaCaiNum) );
	ZeroMemory( m_cbTalkAllOther, sizeof(m_cbTalkAllOther));
	ZeroMemory( m_cbPeng,sizeof(m_cbPeng) );
	
	for(int i = 0 ;i < GAME_PLAYER;i ++)
	{
		m_vecEnjoinChiPeng[i].clear();
	}
	m_bFirstGang = true;
	m_bDaHu = 0;
	m_cbWinCount = 0;
	memset( m_wWinOrder,INVALID_CHAIR,sizeof(m_wWinOrder) );
	ZeroMemory( m_lGameTax,sizeof(m_lGameTax) );
	m_cbSiceCountPi = 0;
	m_bNeedCalGang= false;
	m_bPiGang = false;
	m_cGangCountXS = -1;
	m_cbPantherTypeServer = 0;
	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_HuanPai,sizeof(m_HuanPai));
	ZeroMemory(m_nQueColor,sizeof(m_nQueColor));
	ZeroMemory(m_cbHuanPaiData,sizeof(m_cbHuanPaiData));
	//������Ϣ

	m_wHeapHand = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus = false;
	m_bGangHuaStatus = false;
	m_bGangOutStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));
	ZeroMemory(m_iHuScore,sizeof(m_iHuScore));
	ZeroMemory(m_bEnjoyGang,sizeof(m_bEnjoyGang));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));
	ZeroMemory( m_cbMasterCheckCard,sizeof(m_cbMasterCheckCard) );

	//������Ϣ
	m_cbChiHuCard=0;
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_ChiHuRight[i].SetEmpty();
		m_ChiHuRightTemp[i].SetEmpty();
		m_bOpenProvider[i] = i;
	}
	ZeroMemory( m_cbHuType,sizeof(m_cbHuType) );
	ZeroMemory( m_bHasHuPai,sizeof(m_bHasHuPai) );
	ZeroMemory( m_nFanShu,sizeof(m_nFanShu) );

	m_kChiHuInfoList.clear();
	m_kChiHuInfoListHM.clear();
	m_kGangInfoList.clear();

	if(m_pITableFrame)
	{
		m_pITableFrame->KillGameTimer(IDI_TIMER_HUANPAI);
		m_pITableFrame->KillGameTimer(IDI_TIMER_XUANQUE);
		m_pITableFrame->KillGameTimer(IDI_TIMER_CHUPAI);
		m_pITableFrame->KillGameTimer(IDI_TIMER_ACTION);
	}

	ZeroMemory(m_iStartBaoNum,sizeof(m_iStartBaoNum));
	ZeroMemory(m_bShowBao,sizeof(m_bShowBao));
	ZeroMemory(m_iBaoNum,sizeof(m_iBaoNum));
	ZeroMemory(m_iOpenMouthBaoNum, sizeof(m_iOpenMouthBaoNum));
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_bTalkQingFengQing[i] = false;
	}
	return;
}

bool CTableFrameSink::OnEventDingZhuang(int nChairID)
{
	if(hasRule(GAME_RULE_DING_ZHUANG) && !m_isDingZhuang)
	{
		sendDingZhuang();
		m_isDingZhuang = true;

		return false;
	}

	return true;
}

bool CTableFrameSink::OnEventRollSice()
{
	if(hasRule(GAME_RULE_ROLL_DICE) && !m_isRollDice)
	{
		sendRollDice();
		return false;
	}
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{	
	m_pITableFrame->KillGameTimer(IDI_TIMER_HUANPAI);
	m_pITableFrame->KillGameTimer(IDI_TIMER_XUANQUE);
	m_pITableFrame->KillGameTimer(IDI_TIMER_CHUPAI);
	m_pITableFrame->KillGameTimer(IDI_TIMER_ACTION);
	m_pITableFrame->KillGameTimer(IDI_TIMER_PIAODANG);
	
	m_iBaoIndex = MAX_INDEX;
	m_nHunIndex = MAX_INDEX;
	m_nLaiPiIndex = MAX_INDEX;
	m_nPiIndex = MAX_INDEX;
	m_nPiIndexHM = MAX_INDEX;
	m_dwChiHuUserTemp = 100;

	//�����˿�
	RandCard();
	for(int i=0;i <10;i++)
	{
		if(!CheckRandCard())
		{
			RandCard();
		}
		else
		{
			break;
		}
	}

	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	m_wBankerUser =  m_wBankerUserNext;
	m_wBankerUserNext = INVALID_CHAIR;

	//������ʾ
	//TCHAR szDescribe[12]=TEXT("");
	//TCHAR szDescribe1[520]=TEXT("");
	//for (int i = 0; i < 136; i++)
	//{
	//	sprintf_s(szDescribe,TEXT("%02x,"),m_cbRepertoryCard[i]);
	//	strcat_s(szDescribe1, szDescribe);
	//}
	////��ʾ��Ϣ
	//CTraceService::TraceString(szDescribe1,TraceLevel_Normal);

	if (m_cbPlayerNum == 4)
	{
		/*BYTE byTest[] = {
		0x13,0x17,0x21,0x18,0x15,0x18,0x18,
		0x22,0x31,0x15,0x18,0x18,0x35,0x34,
		0x33,0x34,0x35,0x36,0x37,0x35,0x36,
		0x37,0x35,0x36,0x04,0x14,0x14,0x25,
		0x16,0x21,0x02,0x21,0x15,0x04,0x14,
		0x15,0x07,0x27,0x22,0x16,0x05,0x34,
		0x27,0x19,0x18,0x34,0x15,0x06,0x11,
		0x33,0x36,0x19,0x11,0x12,0x16,0x25,
		0x02,0x24,0x03,0x35,0x14,0x14,0x14, 0x14,
		0x05,0x05,0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x14, 0x14,
		0x14,0x14,0x14, 0x24, 0x26, 0x26, 0x27, 0x32, 0x13, 0x37,

		0x15,0x15,0x05,0x05,	0x15,0x24,0x05,0x11,	0x15,0x32,0x05,0x11,	0x15,0x24,0x05,0x11,
		0x02,0x03,0x04,0x05,	0x11,0x11,0x11,0x26,	0x11,0x11,0x11,0x26,	0x11,0x11,0x11,0x26,
		0x06,0x15,0x16,0x18,	0x27,0x35,0x28,0x28,	0x35,0x28,0x28,0x28,	0x27,0x28,0x28,0x28,
		0x18,					0x12,					0x12,					0x12,					
		};
		CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
		m_wBankerUser = 0;*/

		// ���ݡ���÷����
		/*byte byTest1[] = {
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x08,0x01,0x05,0x12,0x22,0x35,0x13,0x02,

		0x08,0x03,0x25,0x28,0x27,0x24,0x08,0x19,0x29,
		0x17,0x23,0x08,0x15,0x24,0x11,0x01,0x27,0x11,
		0x25,0x28,0x35,0x22,0x17,0x08,0x01,0x01,0x01,
		0x01,0x14,0x14,0x14,0x14,0x15,0x15,0x15,0x15,

		0x18,0x19,0x22,0x23,	0x08,0x011,0x05,0x26,	0x01,0x01,0x15,0x15,	0x13,0x14,0x15,0x15,
		0x24,0x12,0x13,0x16,	0x12,0x12,0x12,0x15,	0x22,0x25,0x24,0x25,	0x22,0x23,0x24,0x25,
		0x24,0x25,0x26,0x29,	0x16,0x17,0x27,0x28,	0x25,0x25,0x27,0x27,	0x25,0x25,0x27,0x27,
		0x29,					0x18,					0x16,					0x16,				
		};
		CopyMemory(m_cbRepertoryCard, byTest1, sizeof(byTest1));
		m_wBankerUser = 0;*/
	}
	
	// 3��2�˲���
	/*BYTE byTest[] = {
		0x22,0x02,0x16,0x25,0x04,
		0x01,0x02,0x16,0x25,0x05,
		0x01,0x02,0x16,0x25,0x05,
		0x01,0x22,0x16,0x25,0x24,
		0x02,0x24,0x03,0x35,0x37,
		0x35,0x24,0x01,0x01,0x24,
		0x01,0x21,0x21,0x26,0x23,
		0x27,0x28,0x28,0x28,0x05,
		0x22,0x04,0x04,0x04,0x32,	
		0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x23,
		0x24,0x24,0x24,0x24,0x26,0x26,0x27,0x23, 

		0x32,0x32,0x28,0x01,
		0x01,0x21,0x21,0x26,
		0x35,0x28,0x28,0x28,
		0x22,				

		0x02,0x01,0x05,0x06,	0x01,0x01,0x02,0x05,	
		0x07,0x08,0x21,0x22,	0x05,0x06,0x06,0x21,	
		0x23,0x24,0x25,0x26,	0x21,0x24,0x24,0x28,	
		0x28,					0x28,						
	};
	CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	m_wBankerUser = 0;*/
	//end test

	//��������
	GDMJ_CMD_S_GameStart GameStart;
	if (m_bGameTypeIndex == 7 && m_bBaoIsOutCard == 2)
	{
		GameStart.wPi = MAX_INDEX;
		GameStart.wHun = MAX_INDEX;
		GameStart.wLaiPi = MAX_INDEX;
		GameStart.wBao = MAX_INDEX;
		m_GameLogic.SetPiIndex(GameStart.wPi);
		m_GameLogic.SetLaiPiIndex(m_nLaiPiIndex);
		GameStart.wPi = m_nPiIndex;
		GameStart.wBao = m_iBaoIndex;
		m_GameLogic.SetMagicIndex(m_iBaoIndex);
		m_GameLogic.SetBaoIndex( m_iBaoIndex);
	}
	else if (m_bGameTypeIndex == 3 || m_bGameTypeIndex == 7) // ���ݻλ�,��÷
	{
		BYTE cbPiTemp = 0xFF;
		BYTE cbBaoTemp = 0xFF;
		GameStart.wPi = MAX_INDEX;
		GameStart.wHun = MAX_INDEX;
		GameStart.wLaiPi = MAX_INDEX;
		GameStart.wBao = MAX_INDEX;
		if (m_bGameTypeIndex == 7)
		{
			m_nPiIndex = m_GameLogic.SwitchToCardIndex(m_cbRepertoryCard[0]); // Ƥ
			m_nPiIndexHM = m_nPiIndex;
		}
		else if (m_bGameTypeIndex == 3)
		{
			WORD wSiceTemp = WORD(m_lSiceCount & 0xffff);
			m_cbSiceCountPi = HIBYTE(wSiceTemp) + LOBYTE(wSiceTemp);
			m_nPiIndex = m_GameLogic.SwitchToCardIndex(m_cbRepertoryCard[m_cbSiceCountPi * 2 - 2]); // Ƥ
		}
		m_iBaoIndex = m_nPiIndex + 1; // Ƥ��һΪ��
		if(m_nPiIndex < 27)
		{
			if(m_iBaoIndex == 9)
			{
				m_iBaoIndex = 0;
			}
			if(m_iBaoIndex == 18)
			{
				m_iBaoIndex = 9;
			}
			if(m_iBaoIndex == 27)
			{
				m_iBaoIndex = 18;
			}
		}
		else
		{
			if (m_bGameTypeIndex == 3)
			{
				if(m_nPiIndex == 31)
				{
					m_iBaoIndex = 32;
				}
				else if(m_nPiIndex == 32)
				{
					m_iBaoIndex = 33;
				}
				else if(m_nPiIndex == 33)
				{
					m_iBaoIndex = 31;
				}
			}
			else
			{
				if(m_nPiIndex == 31 || m_nPiIndex == 32)
				{
					m_iBaoIndex = 33;
				}
				else if(m_nPiIndex == 33)
				{
					m_iBaoIndex = 31;
				}
			}
		}
		m_GameLogic.SetPiIndex(GameStart.wPi);
		m_GameLogic.SetLaiPiIndex(m_nLaiPiIndex);
		GameStart.wPi = m_nPiIndex;
		GameStart.wBao = m_iBaoIndex;
		m_GameLogic.SetMagicIndex(m_iBaoIndex);
		m_GameLogic.SetBaoIndex( m_iBaoIndex);
		if (m_bGameTypeIndex == 7)  m_nPiIndex = MAX_INDEX;
	}
	else
	{
		BYTE cbPiTemp = 0;
		BYTE cbHunTemp = 0;
		GameStart.wPi = MAX_INDEX;
		GameStart.wHun = MAX_INDEX;
		GameStart.wLaiPi = MAX_INDEX;
		GameStart.wBao = MAX_INDEX;
		if(true)
		{
			if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8) // Ϫˮ���߻��ݺ��и�Ӣɽ
			{
				WORD wSiceTemp = WORD(m_lSiceCount & 0xffff);
				m_cbSiceCountPi = (HIBYTE(wSiceTemp) < LOBYTE(wSiceTemp) ? HIBYTE(wSiceTemp) : LOBYTE(wSiceTemp)) * 2 - 2;
				m_nPiIndex= m_GameLogic.SwitchToCardIndex(m_cbRepertoryCard[0]);
				if (m_bGameTypeIndex == 6)
				{
					m_bGameTypeQing = 1; // ���ݺ��и�ֻ��������
					// �ж�ɫ��
					if (m_cbPantherType & TYPE_SEBAO)
					{
						if (HIBYTE(wSiceTemp) == LOBYTE(wSiceTemp))
						{
							m_cbPantherTypeServer |= TYPE_SEBAO;
						}
					}
				}
			}
			else
				m_nPiIndex= m_GameLogic.SwitchToCardIndex(m_cbRepertoryCard[0]);//ׯ�ҵ�һ��ץ����Ƥ
			
			m_nHunIndex = m_nPiIndex + 1;
			m_nLaiPiIndex=31;

			if (m_bGameTypeIndex == 6 || m_bGameTypeIndex == 9)
			{
				cbPiTemp = m_cbRepertoryCard[0];
				cbHunTemp = cbPiTemp + 1;
				if (cbPiTemp == 0x09)
				{
					cbHunTemp = 0x01;
				}
				else if (cbPiTemp == 0x19)
				{
					cbHunTemp = 0x11;
				}
				else if (cbPiTemp == 0x29)
				{
					cbHunTemp = 0x21;
				}
				else if (cbPiTemp == 0x37)
				{
					cbHunTemp = 0x31;
				}
			}
			if(m_nPiIndex<27)
			{
				if(m_nHunIndex==9)
				{
					m_nHunIndex=0;
				}
				if(m_nHunIndex==18)
				{
					m_nHunIndex=9;
				}
				if(m_nHunIndex==27)
				{
					m_nHunIndex=18;
				}			
			}
			else
			{
				if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 8)
				{
					if(m_nPiIndex==30)
					{
						m_nHunIndex=27;
					}
					if(m_nPiIndex==31)
					{
						m_nHunIndex=32;
						m_nLaiPiIndex=33;
					}
					if(m_nPiIndex==32)
					{
						m_nHunIndex=33;
					}
					if(m_nPiIndex==33)
					{
						m_nHunIndex=31;
						m_nLaiPiIndex=32;
					}
				}
				else
				{
					if(m_nPiIndex==30)
					{
						if (m_bGameTypeIndex == 6)
						{
							m_nHunIndex=32;
							m_nLaiPiIndex=31;
						}
						else
						{
							m_nHunIndex=31;
							m_nLaiPiIndex=32;
						}
					}
					if(m_nPiIndex==31)
					{
						m_nHunIndex=32;
						if (m_bGameTypeIndex == 6)
						{
							m_nLaiPiIndex=30;
						}
						else
							m_nLaiPiIndex=33;
					}
					if(m_nPiIndex==32)
					{
						m_nHunIndex=33;
					}
					if(m_nPiIndex==33)
					{
						m_nHunIndex=27;
					}
				}
			}		
			GameStart.wHun = m_nHunIndex;
			GameStart.wPi = m_nPiIndex;
			GameStart.wLaiPi = m_nLaiPiIndex;
			m_GameLogic.SetMagicIndex(m_nHunIndex);
			m_GameLogic.SetPiIndex( m_nPiIndex);
			m_GameLogic.SetLaiPiIndex( m_nLaiPiIndex);
			m_GameLogic.SetBaoIndex( m_iBaoIndex);
		}
		// �жϷ� �� ���� ���ݺ��и�(��Ƿ緭��������������)
		if (m_bGameTypeIndex == 6 || m_bGameTypeIndex == 9)
		{
			if (m_cbPantherType & TYPE_FENGBAO)
			{
				if (cbHunTemp >= 0x31 && cbHunTemp <= 0x37)
				{
					m_cbPantherTypeServer |= TYPE_FENGBAO;
				}
			}
			if (m_cbPantherType & TYPE_JIANGBAO)
			{
				do 
				{
					if (cbHunTemp >= 0x31)
					{
						break;
					}
					BYTE cbValue = (cbHunTemp & MASK_VALUE);
					if( cbValue != 2 && cbValue != 5 && cbValue != 8 )
					{
						break;
					}
					m_cbPantherTypeServer |= TYPE_JIANGBAO;
				} while (false);
			}
			if (m_bGameTypeIndex == 6)
			{
				if ((cbPiTemp < 0x31) && (m_cbPantherType & TYPE_DANBAO))
				{
					if (!(cbPiTemp % 2))
					{
						m_cbPantherTypeServer |= TYPE_DANBAO;
					}
				}
			}
			else
			{
				if (m_cbFrontHunIndex == m_nHunIndex) m_cbPantherTypeServer |= TYPE_DANBAO;
			}
		}
	}

	m_cbLeftCardCount = m_cbLeftCardCount;
	BYTE cbCardIndexTemp[MAX_COUNT-1];
	if(!m_pITableFrame ) return false;
 	//�ַ��˿�
	
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if( m_pITableFrame->GetTableUserItem(i) != NULL )
		{
			m_bPlayStatus[i] = true;
			m_bHasHuPai[i] = false;
			m_cbLeftCardCount -= (MAX_COUNT-1);
			ZeroMemory(cbCardIndexTemp, 0, sizeof(cbCardIndexTemp));
			if(m_cbPlayerNum == 4)
			{
				if (m_bGameTypeIndex != 3 && m_bGameTypeIndex != 7)
				{
					for (BYTE j = 0; j < 4; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + (MAX_REPERTORY - 52) + j];
					}
					for (BYTE j = 4; j < 8; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + (MAX_REPERTORY - 40) + j];
					}
					for (BYTE j = 8; j < 12; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + (MAX_REPERTORY - 28) + j];
					}
					cbCardIndexTemp[12] = m_cbRepertoryCard[(MAX_REPERTORY - 4) + i];
				}
				else
				{
					for (BYTE j = 0; j < 4; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + 68 + j];
					}
					for (BYTE j = 4; j < 8; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + 80 + j];
					}
					for (BYTE j = 8; j < 12; j++)
					{
						cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + 92 + j];
					}
					cbCardIndexTemp[12] = m_cbRepertoryCard[116 + i];
				}
				m_GameLogic.SwitchToCardIndex(cbCardIndexTemp,MAX_COUNT-1,m_cbCardIndex[i]);
			
			}
			else // 3�˺�2��
			{
				if(m_bGameTypeIndex !=3 && m_bGameTypeIndex != 7)
				{
					for(int z = 0 ; z < 3; z++ )
					{
						for(int j = z*4; j < (z+1)*4;j++)
						{
							cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + (TWO_THREE_REPERTORY - m_cbPlayerNum * (MAX_COUNT-1) + z*4 * (m_cbPlayerNum - 1)) + j]; //ÿ��ȡ������
						
						}
					}
					cbCardIndexTemp[12] = m_cbRepertoryCard[(TWO_THREE_REPERTORY - m_cbPlayerNum) + i];
					m_GameLogic.SwitchToCardIndex(cbCardIndexTemp,MAX_COUNT-1,m_cbCardIndex[i]);
				}
				else 
				{
					for(int z = 0 ; z < 3; z++ )
					{
						for(int j = z*4; j < (z+1)*4;j++)
						{
							cbCardIndexTemp[j] = m_cbRepertoryCard[i * 4 + (TWO_THREE_HUANGZHOU - m_cbPlayerNum * (MAX_COUNT-1) + z*4 * (m_cbPlayerNum - 1)) + j]; //ÿ��ȡ������
						
						}
					}
					cbCardIndexTemp[12] = m_cbRepertoryCard[(TWO_THREE_HUANGZHOU - m_cbPlayerNum) + i]; //
					m_GameLogic.SwitchToCardIndex(cbCardIndexTemp,MAX_COUNT-1,m_cbCardIndex[i]);
				}			
			}	
		}
		else
		{
			m_bPlayStatus[i] = false;
		}
	}

	// ��������� 20180807(4��)
	if (m_cbPlayerNum == 4)
	{
		BYTE cbCardIndexTemp1[GAME_PLAYER][MAX_INDEX] = {0};	// �û��˿���ʱ����
		memcpy(cbCardIndexTemp1, m_cbCardIndex, sizeof(cbCardIndexTemp1));
		WORD wPlayerTemp[GAME_PLAYER] = {0, 1, 2, 3};
		std::random_shuffle(wPlayerTemp, wPlayerTemp+CountArray(wPlayerTemp));

		for (WORD j = 0 ; j < GAME_PLAYER; j++)
		{
			memcpy(m_cbCardIndex[j], cbCardIndexTemp1[wPlayerTemp[j]], sizeof(m_cbCardIndex[j]));
			if(m_bGameTypeIndex == 3 && m_iBaoIndex != MAX_INDEX)
			{
				m_iStartBaoNum[j] = m_cbCardIndex[j][m_iBaoIndex]; //ͳ�����ֱ�������
				m_iBaoNum[j]  = m_iStartBaoNum[j];
				m_bEnjoyGang[j] = true;
				if(m_iStartBaoNum[j] >= 2 )
				{
					m_bEnjoyGang[j] = false;
				}
			}
		}
	}
	if(m_bGameTypeIndex == 3 && m_iBaoIndex != MAX_INDEX)
	{
		for (WORD j = 0 ; j < m_cbPlayerNum; j++)
		{
			m_iStartBaoNum[j] = m_cbCardIndex[j][m_iBaoIndex]; //ͳ�����ֱ�������
			m_iBaoNum[j]  = m_iStartBaoNum[j];
			m_bEnjoyGang[j] = true;
			if(m_iStartBaoNum[j] >= 2 )
			{
				m_bEnjoyGang[j] = false;
			}
		}
	}

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	BYTE cbTakeCount = 0;
	if(m_cbPlayerNum == 4)
	{
		if (m_bGameTypeIndex == 3 || m_bGameTypeIndex == 7)
		{
			cbTakeCount = MAX_REPERTORY_HUANGZHOU - m_cbLeftCardCount;
		}
		else
		{
			cbTakeCount = MAX_REPERTORY - m_cbLeftCardCount;
		}
	}
	else 
	{
		if (m_bGameTypeIndex == 3 || m_bGameTypeIndex == 7)
		{
			cbTakeCount = TWO_THREE_REPERTORY - m_cbLeftCardCount;
		}
		else
		{
			cbTakeCount = TWO_THREE_HUANGZHOU - m_cbLeftCardCount;
		}
	}

	for (WORD i = 0; i < m_cbPlayerNum; i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
			//��ͷ����4��,�չ�108����
			m_cbHeapCardInfo[m_wHeapHand][0] -= 4;
			break;
		}
		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	m_bSendStatus = true;
	GameStart.lSiceCount = m_lSiceCount;
	GameStart.wBankerUser = m_wBankerUser;
	GameStart.wCurrentUser = m_wCurrentUser;
	GameStart.wHeapHand = m_wHeapHand;
	GameStart.wHeapTail = m_wHeapTail;
	GameStart.cbLeftCardCount = m_cbLeftCardCount;
	GameStart.cbPantherType = m_cbPantherTypeServer;
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//��������
	for (WORD i=0;i<m_cbPlayerNum;i++)
	{
		if( !m_bPlayStatus[i] ) continue;

		//���ñ���
		GameStart.cbUserAction=m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		m_sTingTags[i].m_TingTags.clear();

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<m_cbPlayerNum; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
		}

		GameStart.nLeftTime = TIME_HUANPAI;
		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	ZeroMemory(m_sTingTags,sizeof(m_sTingTags));

	if (hasRule(GAME_RULE_DIN_QUE))
	{
		sendXuanQueNotice();
	}
	else
	{
		startPlay();
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			GDMJ_AllEndInfo GameEnd;
			GameEnd.resizeData();
			m_GameEndInfo.cbFanShu.clear();
			m_GameEndInfo.cbGenCount.clear();
			m_GameEndInfo.cbMaPai.clear();
			m_GameEndInfo.kCardInfoList.clear();
			m_GameEndInfo.kChiHuInfoList.clear();
			m_GameEndInfo.kChiHuInfoListHM.clear();
			m_GameEndInfo.kGangInfoList.clear();
			m_GameEndInfo.lChaJiaoScore.clear();
			m_GameEndInfo.lGameScoreTotal.clear();
			m_GameEndInfo.lHuaZhuScore.clear();
			m_GameEndInfo.resizeData();

			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
		
			GameEnd.kChiHuInfoList = m_kChiHuInfoList;
			m_GameEndInfo.kChiHuInfoList =  m_kChiHuInfoList;
			GameEnd.kChiHuInfoListHM = m_kChiHuInfoListHM;
			m_GameEndInfo.kChiHuInfoListHM =  m_kChiHuInfoListHM;
			//������Ϣ
			WORD wWinner = INVALID_CHAIR;
			BYTE cbLeftUserCount = 0;			//�ж��Ƿ�����
			bool bUserStatus[GAME_PLAYER];		//

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//���������
				if(!m_bHasHuPai[i]) cbLeftUserCount++;
				//��ǰ���״̬
				if( NULL != m_pITableFrame->GetTableUserItem(i) ) bUserStatus[i] = true;
				else bUserStatus[i] = false;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.kCardInfoList[i].kHandCardList);
				m_GameLogic.SwitchToCardData(m_cbCardIndex[i],m_GameEndInfo.kCardInfoList[i].kHandCardList);
				m_sTingTags[i].m_TingTags.clear();
			}

			ZeroMemory(m_sTingTags,sizeof(m_sTingTags));

			bool isQiangGang = false;

			LONGLONG lHuaZhuScore[GAME_PLAYER];
			LONGLONG lChaJiaoScore[GAME_PLAYER];
			WORD	 lChaJiaoFanShuMax[GAME_PLAYER];
			LONGLONG lGangScore[GAME_PLAYER];

			int baseScore = m_vBaseScore[m_bGameBaseScore];

			ZeroMemory( lGangScore,sizeof(lGangScore) );
			ZeroMemory( lHuaZhuScore,sizeof(lHuaZhuScore) );
			ZeroMemory( lChaJiaoScore,sizeof(lChaJiaoScore) );
			ZeroMemory( lChaJiaoFanShuMax,sizeof(lChaJiaoFanShuMax) );
			ZeroMemory( m_cbFaCaiNum,sizeof(m_cbFaCaiNum) );
			ZeroMemory( m_cbTalkAllOther, sizeof(m_cbTalkAllOther));
			ZeroMemory( m_cbPeng,sizeof(m_cbPeng) );
			m_cbFrontHunIndex = m_nHunIndex;

			SCORE lScoreTemp[GAME_PLAYER]={0};
			SCORE lHuScoreTemp[GAME_PLAYER]={0};
			if (m_kChiHuInfoListHM.size() > 0)
			{
				for(int t=0;t<1;t++)
				{
					GDMJ_CMD_S_ChiHu_HM msg = m_kChiHuInfoListHM[0];
					if(m_bNeedCalGang)
					{			
						for (WORD k=0;k<GAME_PLAYER;k++)
						{
							int nCount = m_lSelfGangCount[k];
							for(int o = 0;o<nCount;o++)
							{
								m_pITableFrame->AddPrivateAction(k,4);
							}			
							nCount = m_lOtherGangCount[k];
							for(int o = 0;o<nCount;o++)
							{
								m_pITableFrame->AddPrivateAction(k,3);
							}		
						}						
					}				
				}
				for(int t=0;t<m_kChiHuInfoListHM.size();t++)
				{
					GDMJ_CMD_S_ChiHu_HM msg = m_kChiHuInfoListHM[t];			
					for (WORD k=0;k<GAME_PLAYER;k++)
					{
						lScoreTemp[k]+=msg.lHuPaiScore[k];
						lHuScoreTemp[k]+=msg.lHuPaiScore[k];
					}				
				}
			}
			if(m_kChiHuInfoList.size() > 0)
			{
				for(int t=0;t<1;t++)
				{
					GDMJ_CMD_S_ChiHu msg = m_kChiHuInfoList[0];			
					if(m_bNeedCalGang)
					{			
						for (WORD k=0;k<GAME_PLAYER;k++)
						{
							int nCount = m_lSelfGangCount[k];
							for(int o = 0;o<nCount;o++)
							{
								m_pITableFrame->AddPrivateAction(k,4);
							}			
							nCount = m_lOtherGangCount[k];
							for(int o = 0;o<nCount;o++)
							{
								m_pITableFrame->AddPrivateAction(k,3);
							}		
						}						
					}				
				}
				for(int t=0;t<m_kChiHuInfoList.size();t++)
				{
					GDMJ_CMD_S_ChiHu msg = m_kChiHuInfoList[t];			
					for (WORD k=0;k<GAME_PLAYER;k++)
					{
						lScoreTemp[k]+=msg.lHuPaiScore[k];
						lHuScoreTemp[k]+=msg.lHuPaiScore[k];
					}				
				}
			}
			if (m_bGameTypeIndex == 7)
			{
				for (WORD k=0;k<GAME_PLAYER;k++)
				{
					m_lGameScore[k]+=lScoreTemp[k];
					if(m_kChiHuInfoListHM.size() > 1)
					{
						for(int t=0;t<m_kChiHuInfoListHM.size();t++)
						{
							GDMJ_CMD_S_ChiHu_HM* pmsg = &GameEnd.kChiHuInfoListHM[t];			
							pmsg->lHuPaiScore[k] = lHuScoreTemp[k];
							GDMJ_CMD_S_ChiHu_HM *pnsg = &m_GameEndInfo.kChiHuInfoListHM[t];
							pnsg->lHuPaiScore[k] = lHuaZhuScore[k]; 
						}
					}
				}
			}
			else
			{
				for (WORD k=0;k<GAME_PLAYER;k++)
				{
					m_lGameScore[k]+=lScoreTemp[k];
					if(m_kChiHuInfoList.size() > 1)
					{
						for(int t=0;t<m_kChiHuInfoList.size();t++)
						{
							GDMJ_CMD_S_ChiHu* pmsg = &GameEnd.kChiHuInfoList[t];
							pmsg->lHuPaiScore[k] = lHuScoreTemp[k];
							GDMJ_CMD_S_ChiHu *pnsg = &m_GameEndInfo.kChiHuInfoList[t];
							pnsg->lHuPaiScore[k] = lHuaZhuScore[k]; 
						}
					}
				}
			}
			
			//ͳ�ƻ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if( NULL == m_pITableFrame->GetTableUserItem(i) ) continue;

				//���û���
				if (m_lGameScore[i]>0L)
				{
					m_lGameTax[i] = 0;
					m_lGameScore[i] -= m_lGameTax[i];
				}

				BYTE ScoreKind;
				if( m_lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( m_lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;

				ScoreInfoArray[i].lScore   = m_lGameScore[i];
				ScoreInfoArray[i].lRevenue = m_lGameTax[i];
				ScoreInfoArray[i].cbType   = ScoreKind;

			}

 			//д�����/
			datastream kDataStream;
			m_kGameRecord.StreamValue(kDataStream,true);
			m_pITableFrame->WriteTableScore(ScoreInfoArray,m_cbPlayerNum,kDataStream);

			for (int i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.lGameScoreTotal[i]  =m_lGameScore[i];
				GameEnd.lHuaZhuScore[i]     = lHuaZhuScore[i];
				GameEnd.lChaJiaoScore[i]    =lChaJiaoScore[i];
				GameEnd.cbGenCount[i]      =m_cbGenCount[i];
				GameEnd.cbFanShu[i]        =(BYTE)m_nFanShu[i];
				m_GameEndInfo.lGameScoreTotal[i] =  m_lGameScore[i];
				m_GameEndInfo.lHuaZhuScore[i] = lHuaZhuScore[i];
				m_GameEndInfo.lChaJiaoScore[i]    =lChaJiaoScore[i];
				m_GameEndInfo.cbGenCount[i]      =m_cbGenCount[i];
				m_GameEndInfo.cbFanShu[i]        =(BYTE)m_nFanShu[i];
			}

			//��ʾʣ�����
			int leftCard = m_cbLeftCardCount;
			WORD userIdx = (m_wCurrentUser + 1) % GAME_PLAYER;

			BYTE	cbCardLeft[GAME_PLAYER][MAX_INDEX];
			ZeroMemory(cbCardLeft, sizeof(cbCardLeft));
			int indx = 0;
			int j = 0;

			// edit by wld
			GameEnd.cbMaPai.clear();
			m_GameEndInfo.cbMaPai.clear();

			for(int i = 0; i < leftCard; i++)
			{
				BYTE card = getSendCardData(userIdx);
				cbCardLeft[userIdx][indx] = card;
				userIdx = (userIdx + 1) % GAME_PLAYER;
				if((i + j + 1) % 4 == 0 )
					indx++;
			}

			for(WORD i = 0; i < GAME_PLAYER; i++)
			{
				GameEnd.kCardInfoList[i].kLeftCardList.clear();
				m_GameEndInfo.kCardInfoList[i].kLeftCardList.clear();
				for (BYTE j=0;j<MAX_INDEX;j++)
				{
					if (cbCardLeft[i][j]!=0)
					{
						GameEnd.kCardInfoList[i].kLeftCardList.push_back(cbCardLeft[i][j]);
						m_GameEndInfo.kCardInfoList[i].kLeftCardList.push_back(cbCardLeft[i][j]);
					}
				}				
				
				for(int j =0 ;j< MAX_WEAVE;j++)
				{
					if(m_WeaveItemArray[i][j].cbWeaveKind != WIK_NULL)
					{
						GDMJ_CMD_WeaveItem weaveitem;
						weaveitem.cbCenterCard = m_WeaveItemArray[i][j].cbCenterCard;
						weaveitem.cbPublicCard = m_WeaveItemArray[i][j].cbPublicCard;
						weaveitem.cbWeaveKind = m_WeaveItemArray[i][j].cbWeaveKind;
						weaveitem.wProvideUser = m_WeaveItemArray[i][j].wProvideUser;
						GameEnd.kCardInfoList[i].kWeaveItemList.push_back(weaveitem);
						m_GameEndInfo.kCardInfoList[i].kWeaveItemList.push_back(weaveitem);
					}
				}
			}
			//end ʣ�����

			datastream kDataEnd;
			GameEnd.StreamValue(kDataEnd,true);
			m_GameEndSize =  kDataEnd.size();

			//���ͽ�����Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GAME_END,&kDataEnd[0],kDataEnd.size() );
			m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GAME_END,&kDataEnd[0],kDataEnd.size() );

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);
			m_isDingZhuang = false;
			m_isRollDice = false;
			ZeroMemory( m_nUsefulQingFengJiang, sizeof(m_nUsefulQingFengJiang));
			ZeroMemory( m_nCheckQingFengJiang, sizeof(m_nCheckQingFengJiang));
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				m_bTalkQingFengQing[i] = false;
			}
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);

			m_isDingZhuang = false;
			m_isRollDice = false;
// 			m_isPiaoDang= true;
// 			m_isFirstPiaoDang=true;

			ZeroMemory( m_bPlayPiao,sizeof(m_bPlayPiao) );
			ZeroMemory( m_bPlayDang,sizeof(m_bPlayDang) );
			ZeroMemory( m_bSelectedPiaoDang,sizeof(m_bSelectedPiaoDang) );
			ZeroMemory( m_nUsefulQingFengJiang, sizeof(m_nUsefulQingFengJiang));
			ZeroMemory( m_nCheckQingFengJiang, sizeof(m_nCheckQingFengJiang));
			ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
			ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));
			ZeroMemory( m_cbFaCaiNum,sizeof(m_cbFaCaiNum) );
			ZeroMemory( m_cbTalkAllOther, sizeof(m_cbTalkAllOther));
			ZeroMemory( m_cbPeng,sizeof(m_cbPeng) );
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				m_bTalkQingFengQing[i] = false;
			}

			return true;
		}
	case GER_NETWORK_ERROR:		//�������
	case GER_USER_LEAVE:		//�û�ǿ��
		{
			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_MJ_FREE);
			m_isDingZhuang = false;
			m_isRollDice = false;
// 			m_isPiaoDang= true;
// 			m_isFirstPiaoDang=true;


			return true;
		}
	}

	//�������
	ASSERT(FALSE);
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			GDMJ_CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_HUANPAI:	//����״̬
		{
			//��������
			GDMJ_CMD_S_StateHuanPai GameHuanPai;
			GameHuanPai.wBankerUser=m_wBankerUser;
			GameHuanPai.cbLeftCardCount = m_cbLeftCardCount;
			GameHuanPai.bHuan = m_HuanPai[wChiarID];

			ZeroMemory(GameHuanPai.cbCardData,sizeof(GameHuanPai.cbCardData));
			m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],GameHuanPai.cbCardData);

			//ȫ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)- TIME_HUANPAI;
			GameHuanPai.nLeftTime = (BYTE)(TIME_HUANPAI - __min(dwPassTime, TIME_HUANPAI));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&GameHuanPai,sizeof(GameHuanPai));
		}
	case GS_MJ_XUANQUE:	//ѡȱ״̬
		{
			//��������
			GDMJ_CMD_S_StateXuanQue GameXuanQue;
			GameXuanQue.wBankerUser=m_wBankerUser;
			GameXuanQue.cbLeftCardCount = m_cbLeftCardCount;

			ZeroMemory(GameXuanQue.cbCardData,sizeof(GameXuanQue.cbCardData));
			m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],GameXuanQue.cbCardData);

			m_nQueColor[wChiarID] = GetQueColor(wChiarID);
			GameXuanQue.nQueColor = m_nQueColor[wChiarID];

			//ȫ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)- TIME_XUANQUE;
			GameXuanQue.nLeftTime = (BYTE)(TIME_XUANQUE - __min(dwPassTime, TIME_XUANQUE));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&GameXuanQue,sizeof(GameXuanQue));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
			//��������
			GDMJ_CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wSliceCount = (WORD)m_lSiceCount;
			StatusPlay.cbHunIndex = m_nHunIndex;
			if (m_bGameTypeIndex == 7) StatusPlay.cbPiIndex = m_nPiIndexHM;
			else StatusPlay.cbPiIndex = m_nPiIndex;
			StatusPlay.cbLaiPiIndex = m_nLaiPiIndex;
			StatusPlay.cbBaoIndex = m_iBaoIndex;
			StatusPlay.cbPantherType = m_cbPantherTypeServer;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			CopyMemory(StatusPlay.wWinOrder,m_wWinOrder,sizeof(m_wWinOrder));
			CopyMemory(StatusPlay.cbFaCaiNum,m_cbFaCaiNum,sizeof(m_cbFaCaiNum));
			StatusPlay.cbWinCout = m_cbWinCount;

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbShowBao,m_bShowBao,sizeof(StatusPlay.cbShowBao));
			for (int i = 0;i<GAME_PLAYER;i++)
			{
				CopyMemory(StatusPlay.cbDiscardCard[i],m_cbDiscardCard[i],sizeof(BYTE)*60);
			}
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//������Ϣ
			StatusPlay.wHeapHand = m_wHeapHand;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			StatusPlay.cbSendCardData=((m_cbSendCardData!=0)&&(m_wProvideUser==wChiarID))?m_cbSendCardData:0x00;

			CopyMemory(StatusPlay.nQueColor,m_nQueColor,sizeof(m_nQueColor));

			for (int n = 0;n<GAME_PLAYER;n++)
			{
				for (int i=0;i<m_kChiHuInfoList.size();i++)
				{
					if(m_kChiHuInfoList[i].wChiHuUser == n)
					{
						StatusPlay.nChiHuCard[n][StatusPlay.nChiHuCardNum[n]] = m_kChiHuInfoList[i].cbChiHuCard;
						StatusPlay.nChiHuCardNum[n]++;
					}
				}
				//StatusPlay.bHasHu[n] = OnGetTingCard(n, 0);
				StatusPlay.nGangScore[n] = m_nGangScoreNow[n];
			}
			CopyMemory(StatusPlay.bHasHu,m_bHasHuPai,sizeof(m_bHasHuPai));
			CopyMemory(StatusPlay.cbGangSiceNum,m_cbGangSiceNum,sizeof(m_cbGangSiceNum));

			//���ͳ���
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			// ����Ʈ�͵�
/*			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GDMJ_CMD_S_OperateAckPiaoDang OperateNotify;
				OperateNotify.cbPiao=m_bPlayPiao[i];
				OperateNotify.cbDang=m_bPlayDang[i];
				OperateNotify.cbUser=i;

				m_pITableFrame->SendTableData(i,SUB_S_ACK_PIAODANG);
				m_pITableFrame->SendLookonData(i,SUB_S_ACK_PIAODANG,&OperateNotify,sizeof(OperateNotify));
			}
			*/
			return true;
		}
	}

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	
	switch (wTimerID)
	{
	case IDI_TIMER_PIAODANG:  //Ʈ������
		{
			m_isPiaoDang = false;
			if(hasRule(GAME_RULE_DING_ZHUANG) )
			{
				sendDingZhuang();
				return false;
			}
			return true;
		}
	case IDI_TIMER_HUANPAI:  //���ƽ���
		{
			if (m_bGameTypeIdex == GAME_TYPE_CHENGDU || m_bGameTypeIdex == GAME_TYPE_YICHANG)
			{
				OnUserHuanPaiEnd_ChengDu();
			}
			if (m_bGameTypeIdex == GAME_TYPE_SICHUAN)
			{
				OnUserHuanPaiEnd_SiChuan();
			}
			return true;
		}
	case IDI_TIMER_XUANQUE:  //ѡȱ����
		{
			startPlay();
			return true;
		}
	case IDI_TIMER_CHUPAI:
		{
			if (FvMask::HasAny(m_bGameRuleIdex,_MASK_(GAME_RULE_NO_WAIT)))
			{
				return true;
			}
			BYTE cbCardData =	m_GameLogic.GetAutoDiscardData(m_cbCardIndex[m_wCurrentUser],m_nQueColor[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser]*3);
			OnUserOutCard(m_wCurrentUser,cbCardData);
			return true;
		}
	case IDI_TIMER_ACTION:
		{
			if (FvMask::HasAny(m_bGameRuleIdex,_MASK_(GAME_RULE_NO_WAIT)))
			{
				return true;
			}
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if (m_cbUserAction[i]!=WIK_NULL)
				{
					OnUserOperateCard(i,WIK_NULL,0,0);
					break;
				}
			}
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID* pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_S_ACK_PIAODANG:		//Ʈ����Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(GDMJ_CMD_S_OperateAckPiaoDang));
			if (wDataSize!=sizeof(GDMJ_CMD_S_OperateAckPiaoDang)) return false;

// 			if (m_pITableFrame->GetGameStatus() != GS_MJ_PIAODANG)
// 			{
// 				return true;
// 			}
			
			//��Ϣ����
			GDMJ_CMD_S_OperateAckPiaoDang * pOutCard=(GDMJ_CMD_S_OperateAckPiaoDang *)pDataBuffer;
			bool bSucess = OnUserPiaoDang(pIServerUserItem->GetChairID(),pOutCard->cbPiao,pOutCard->cbDang,pIServerUserItem);
			if (!bSucess)
			{
				ASSERT(false);
			}
			return bSucess;
		}
	case SUB_S_REQ_ROOM_INFO:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(GDMJ_CMD_S_OperateReqRoomInfo));
			if (wDataSize!=sizeof(GDMJ_CMD_S_OperateReqRoomInfo)) return false;

			//��Ϣ����
			GDMJ_CMD_S_OperateReqRoomInfo * pOutCard=(GDMJ_CMD_S_OperateReqRoomInfo *)pDataBuffer;
			GDMJ_CMD_S_OperateAckRoomInfo msg;

			msg.bGameTypeIndex = m_bGameTypeIndex;
			msg.bGameTypeQing = m_bGameTypeQing;
			msg.bGameMaxScore = m_bGameMaxScore;
			msg.bGameMaxScoreGold = m_bGameMaxScoreGold;
			msg.bPiaoShu = m_bPiaoShu;
			msg.bBaoIsOutCard = m_bBaoIsOutCard;
			msg.bGameBaseScore = m_bGameBaseScore;
			msg.bFendDing = m_bFendDing;
			msg.bJingDing = m_bJingDing;
			msg.bPlayerNum = m_cbPlayerNum;
			if(m_bGameTypeIndex !=4 && m_bGameTypeIndex != 6)  m_bAllOther = 0;
			if(m_bGameTypeIndex !=3 && m_bGameTypeIndex != 6 && m_bGameTypeIndex != 7)  m_bQiangGangHu = 0;
			msg.bAllOther = m_bAllOther;
			msg.bQiangGanghu = m_bQiangGangHu;
			msg.bPantherType = m_cbPantherType;
			msg.dwClubId = m_dwClubID;
			m_pITableFrame->SendTableData(pOutCard->cbUser,SUB_S_ACK_ROOM_INFO,&msg,sizeof(msg));
			m_pITableFrame->SendLookonData(pOutCard->cbUser,SUB_S_ACK_ROOM_INFO,&msg,sizeof(msg));

			return true;
		}
	case SUB_S_REQ_PIAODANG:		//Ʈ����Ϣ
		{
			//Ч����Ϣ 
			ASSERT(wDataSize==sizeof(GDMJ_CMD_S_OperateReqPiaoDang));
			if (wDataSize!=sizeof(GDMJ_CMD_S_OperateReqPiaoDang)) return false;

			//��Ϣ����
			GDMJ_CMD_S_OperateReqPiaoDang * pOutCard=(GDMJ_CMD_S_OperateReqPiaoDang *)pDataBuffer;


			bool bSucess = true;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GDMJ_CMD_S_OperateAckPiaoDang OperateNotify;
				OperateNotify.cbPiao=m_bPlayPiao[i];
				OperateNotify.cbDang=m_bPlayDang[i];
				OperateNotify.cbUser=(BYTE)i;

				m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_ACK_PIAODANG,&OperateNotify,sizeof(OperateNotify));
				m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(),SUB_S_ACK_PIAODANG,&OperateNotify,sizeof(OperateNotify));
			}
			return bSucess;
		}
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(GDMJ_CMD_C_OutCard));
			if (wDataSize!=sizeof(GDMJ_CMD_C_OutCard)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING)
			{
				return true;
			}
			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
			{
				return true;
			}

			m_pITableFrame->KillGameTimer(IDI_TIMER_CHUPAI);
			//��Ϣ����
			GDMJ_CMD_C_OutCard * pOutCard=(GDMJ_CMD_C_OutCard *)pDataBuffer;

			if (m_GameLogic.SwitchToCardIndex(pOutCard->cbCardData) == m_nHunIndex)
			{
				if (m_bGameTypeIndex == 3 || (m_bGameTypeIndex == 7 && !m_bBaoIsOutCard)) return true;
			}

			bool bSucess = OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
			if (!bSucess)
			{
				ASSERT(false);
			}
			return bSucess;
		}
	case SUB_C_SELECT_CARD:
		{
			return true;
// 			ASSERT(wDataSize==sizeof(GDMJ_CMD_C_SelectCard));
// 			if (wDataSize!=sizeof(GDMJ_CMD_C_SelectCard)) return false;
// 
// 			//�û�Ч��
// 			if (pIServerUserItem->GetUserStatus()!=US_PLAYING)
// 			{
// 				return true;
// 			}
// 			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
// 			{
// 				return true;
// 			}
// 
// 			GDMJ_CMD_C_SelectCard* pSelectCard=(GDMJ_CMD_C_SelectCard*)pDataBuffer;
// 
// 
// 			bool bSuccess = OnSelectCard(pIServerUserItem->GetChairID(),pSelectCard->cbCardData);
// 			if (!bSuccess)
// 			{
// 				ASSERT(false);
// 			}
// 			return bSuccess;

		}
	case SUB_C_GET_TING:
		{
			return true;
// 			ASSERT(wDataSize==sizeof(GDMJ_CMD_C_GetTingCard));
// 			if(wDataSize!=sizeof(GDMJ_CMD_C_GetTingCard)) return false;
// 
// 			//�û�Ч��
// 			if (pIServerUserItem->GetUserStatus()!=US_PLAYING)
// 			{
// 				return true;
// 			}
// 			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
// 			{
// 				return true;
// 			}
// 
// 			GDMJ_CMD_C_GetTingCard* pTing = (GDMJ_CMD_C_GetTingCard*)pDataBuffer;
// 
// 			bool bSuccess = OnGetTingCard(pIServerUserItem->GetChairID(),pTing->cbCardData);
// 			if(!bSuccess)
// 			{
// 				ASSERT(false);
// 			}
// 
// 			return bSuccess;
		}
	case SUB_C_GET_TING_CARDS:
		{
			return true;
// 			ASSERT(wDataSize==sizeof(GDMJ_CMD_C_TingCards));
// 			if(wDataSize!=sizeof(GDMJ_CMD_C_TingCards)) return false;
// 
// 			//�û�Ч��
// 			if (pIServerUserItem->GetUserStatus()!=US_PLAYING)
// 			{
// 				return true;
// 			}
// 			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
// 			{
// 				return true;
// 			}
// 
// 			GDMJ_CMD_C_TingCards* pTing = (GDMJ_CMD_C_TingCards*)pDataBuffer;
// 
// 			bool bSuccess = OnGetTingCardTag(pIServerUserItem->GetChairID(),pTing->cbCardData);
// 			if(!bSuccess)
// 			{
// 				ASSERT(false);
// 			}
// 
// 			return bSuccess;
			
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(GDMJ_CMD_C_OperateCard));
			if (wDataSize!=sizeof(GDMJ_CMD_C_OperateCard)) 
			{
				//������ʾ
				TCHAR szDescribe[64]=TEXT("");
				sprintf_s(szDescribe,TEXT("SUB_C_OPERATE_CARD,%d,%d"),wDataSize, sizeof(GDMJ_CMD_C_OperateCard));
				//��ʾ��Ϣ
				CTraceService::TraceString(szDescribe,TraceLevel_Normal);

				return false;
			}

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING)
			{
				//������ʾ
				TCHAR szDescribe[64]=TEXT("");
				sprintf_s(szDescribe,TEXT("!=US_PLAYING,%d,%d"),pIServerUserItem->GetUserStatus(), US_PLAYING);
				//��ʾ��Ϣ
				CTraceService::TraceString(szDescribe,TraceLevel_Normal);
				return true;
			}

			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
			{
				//������ʾ
				TCHAR szDescribe[64]=TEXT("");
				sprintf_s(szDescribe,TEXT("!= GS_MJ_PLAY,%d,%d"),m_pITableFrame->GetGameStatus(), GS_MJ_PLAY);
				//��ʾ��Ϣ
				CTraceService::TraceString(szDescribe,TraceLevel_Normal);
				return true;
			}

			m_pITableFrame->KillGameTimer(IDI_TIMER_ACTION);
			//��Ϣ����
			GDMJ_CMD_C_OperateCard * pOperateCard=(GDMJ_CMD_C_OperateCard *)pDataBuffer;
			if ((pOperateCard->cbOperateCode == WIK_QING_BAO || pOperateCard->cbOperateCode == WIK_FENG_BAO || pOperateCard->cbOperateCode == WIK_JIANG_BAO) && m_bGameTypeIndex == 3)
			{
				return true;
			}
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_TRUSTEE:
		{
			GDMJ_CMD_C_Trustee *pTrustee =(GDMJ_CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(GDMJ_CMD_C_Trustee)) return false;


			if (m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
			{
				return true;
			}

			m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
			GDMJ_CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case  SUB_C_HUANPAI:
		{
			if (wDataSize != sizeof(GDMJ_CMD_C_HuanPai))return false;
			GDMJ_CMD_C_HuanPai* pHuanPai = (GDMJ_CMD_C_HuanPai*)pDataBuffer;
			if (m_pITableFrame->GetGameStatus() != GS_MJ_HUANPAI)
			{
				return true;
			}
			if (pIServerUserItem->GetChairID() > 4)
			{
				return false;
			}
			if (m_HuanPai[pIServerUserItem->GetChairID()]) 
			{
				return true;
			}
			if (m_bGameTypeIdex == GAME_TYPE_SICHUAN)
			{
				m_HuanPai[pIServerUserItem->GetChairID()] = true;
				OnUserHuanPaiSiChuan(pIServerUserItem->GetChairID(),pHuanPai);
			}
			else if (m_bGameTypeIdex == GAME_TYPE_CHENGDU || m_bGameTypeIdex == GAME_TYPE_YICHANG)
			{
				m_HuanPai[pIServerUserItem->GetChairID()] = true;
				OnUserHuanPaiChengDu(pIServerUserItem->GetChairID(),pHuanPai);
			}

			if (m_pGameServiceOption->wServerType == GAME_GENRE_EDUCATE)
			{
				bool bHuan = true;
				for (int i=0;i<GAME_PLAYER;i++)
				{
					if (!m_HuanPai[i])
					{
						bHuan = false;
						break;
					}
				}
				if (bHuan && m_pITableFrame->GetGameStatus() == GS_MJ_HUANPAI)
				{
					if (m_bGameTypeIdex == GAME_TYPE_CHENGDU || m_bGameTypeIdex == GAME_TYPE_YICHANG)
					{
						OnUserHuanPaiEnd_ChengDu();
					}
					if (m_bGameTypeIdex == GAME_TYPE_SICHUAN)
					{
						OnUserHuanPaiEnd_SiChuan();
					}
				}
			}

			return true;
		}
	case SUB_C_XUANQUE:
		{
			if (wDataSize != sizeof(GDMJ_CMD_C_XuanQue))return false;
			GDMJ_CMD_C_XuanQue* pInfo = (GDMJ_CMD_C_XuanQue*)pDataBuffer;
			WORD wChairID = pIServerUserItem->GetChairID();
			m_nQueColor[wChairID] = pInfo->nQueChoice;

			bool bQue = true;
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if (m_nQueColor[i] == 0)
				{
					bQue = false;
					break;
				}
			}
			if (bQue && m_pITableFrame->GetGameStatus() != GS_MJ_PLAY)
			{
				startPlay();
			}
			return true;
		}
	case SUB_C_MASTER_LEFTCARD:
		{
			WORD wChairID = pIServerUserItem->GetChairID();
			sendMasterLeftCard(wChairID);
			return true;
		}
	case SUB_C_MASTER_CHEAKCARD:
		{
			GDMJ_MaterCheckCard *pMaterCheckCard =(GDMJ_MaterCheckCard *)pDataBuffer;
			if(wDataSize != sizeof(GDMJ_MaterCheckCard)) return false;

			WORD wChairID = pIServerUserItem->GetChairID();
			m_cbMasterCheckCard[wChairID] = pMaterCheckCard->cbCheakCard;
			return true;
		}
	case SUB_C_MASTER_ZHANIAO:
		{

			// 			MaterNiaoCout *pMaterCheckCard =(MaterNiaoCout *)pDataBuffer;
			// 			if(wDataSize != sizeof(MaterNiaoCout)) return false;
			// 
			// 			BYTE cbNiaoCout = pMaterCheckCard->cbNiaoCout;
			// 			if (cbNiaoCout > GetNiaoCardNum())
			// 			{
			// 				cbNiaoCout = GetNiaoCardNum();
			// 			}
			// 			WORD wChairID = pIServerUserItem->GetChairID();
			// 			m_cbMasterZhaNiao[wChairID] = cbNiaoCout;

			return true;
		}
	case  SUB_S_ACK_COUNT_INFO:
		{
			//Ч����
			ASSERT(wDataSize==sizeof(GDMJ_CMD_AckCount));
			if (wDataSize!=sizeof(GDMJ_CMD_AckCount))return false;

			if( m_GameEndSize >0 && !m_GameEndInfo.kCardInfoList.empty() )
			{	

				if(m_iBaoIndex !=MAX_INDEX  && m_bGameTypeIndex == 3)
				{
					GDMJ_CMD_AckCount_Data  AcountData;
					ZeroMemory(&AcountData,sizeof(GDMJ_CMD_AckCount_Data));
					AcountData.cbPantherType = m_cbPantherTypeServer;
					AcountData.iMagicIndex = m_iBaoIndex;
					AcountData.iPiIndex = m_nPiIndex;
					AcountData.iLaiPiIndex =m_nLaiPiIndex ;
					m_pITableFrame->SendTableData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_DATA,&AcountData,sizeof(GDMJ_CMD_AckCount_Data) );
					m_pITableFrame->SendLookonData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_DATA,&AcountData,sizeof(GDMJ_CMD_AckCount_Data) );
				}
				else if(m_nHunIndex !=MAX_INDEX )
				{
					GDMJ_CMD_AckCount_Data  AcountData;
					ZeroMemory(&AcountData,sizeof(GDMJ_CMD_AckCount_Data));
					AcountData.cbPantherType = m_cbPantherTypeServer;
					AcountData.iMagicIndex =m_nHunIndex;
					AcountData.iPiIndex = m_nPiIndex;
					AcountData.iLaiPiIndex =m_nLaiPiIndex;
					for (int i = 0;i<GAME_PLAYER;i++)
					{
						CopyMemory(AcountData.cbDisCardValue[i],m_cbDiscardCard[i],sizeof(BYTE)*60);
					}
					CopyMemory(AcountData.cbDisCardCount,m_cbDiscardCount,sizeof(AcountData.cbDisCardCount));
					m_pITableFrame->SendTableData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_DATA,&AcountData,sizeof(GDMJ_CMD_AckCount_Data) );
					m_pITableFrame->SendLookonData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_DATA,&AcountData,sizeof(GDMJ_CMD_AckCount_Data) );
				}
			}

			if( m_GameEndSize >0 && !m_GameEndInfo.cbFanShu.empty() )
			{	
				datastream kStream; 
				ChiHu.StreamValue(kStream,true);
				m_pITableFrame->SendTableData( pIServerUserItem->GetChairID(),SUB_S_CHI_HU,&kStream[0],kStream.size() );
				m_pITableFrame->SendLookonData( pIServerUserItem->GetChairID(),SUB_S_CHI_HU,&kStream[0],kStream.size() );

				datastream kDataEnd;
				m_GameEndInfo.StreamValue(kDataEnd,true);
				m_pITableFrame->SendTableData( pIServerUserItem->GetChairID(),SUB_S_GAME_END,&kDataEnd[0],kDataEnd.size() );
				m_pITableFrame->SendLookonData( pIServerUserItem->GetChairID(),SUB_S_GAME_END,&kDataEnd[0],kDataEnd.size() );
			}	
			else 
			{

				GDMJ_CMD_AckCount_Res  AckCountRes;
				ZeroMemory(&AckCountRes,sizeof(GDMJ_CMD_AckCount_Res));
				m_pITableFrame->SendTableData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_RES,&AckCountRes,sizeof(GDMJ_CMD_AckCount_Res) );
				m_pITableFrame->SendLookonData( pIServerUserItem->GetChairID(),SUB_S_ACK_COUNT_INFO_RES,&AckCountRes,sizeof(GDMJ_CMD_AckCount_Res) );
			}
			return   true;
		}
	}
	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

void CTableFrameSink::SetPrivateFushunInfo(BYTE bGameTypeIndex, BYTE bGameTypeQing,BYTE bLimitIP,BYTE bGameMaxScore,BYTE bGameMaxScoreGold,BYTE bPiaoShu,
										   BYTE bBaoIsOutCard,BYTE bFendDing,BYTE bJingDing,BYTE bAllOther,BYTE cbPantherType,BYTE cbPlayerNum,DWORD dwClubID)
{
	m_bGameTypeIndex = bGameTypeIndex;
	m_bGameTypeQing = (BYTE)bGameTypeQing;
	if (m_bGameTypeIndex == 7) m_bGameTypeQing = 2;
	else if (m_bGameTypeIndex == 8) m_bGameTypeQing = 1;
	m_bAllOther = bAllOther;
	m_bGameMaxScore = bGameMaxScore;
	m_bGameMaxScoreGold = bGameMaxScoreGold;
	m_bPiaoShu = bPiaoShu;
	m_bBaoIsOutCard = bBaoIsOutCard;
	m_bFendDing = bFendDing;
	m_bJingDing = bJingDing;
	m_bLimitIP = bLimitIP;
	m_dwClubID = dwClubID;
	m_cbPantherType = cbPantherType;
	m_cbPlayerNum = cbPlayerNum; 
	if( bGameTypeIndex == 1  || bGameTypeIndex == 2)
	{
		if (m_bGameMaxScore == 20 )
		{
			m_bGameMaxScoreGold = 25;
		}
		if (m_bGameMaxScore == 30)
		{
			m_bGameMaxScoreGold = 40;
		}
		if (m_bGameMaxScore == 50)
		{
			m_bGameMaxScoreGold = 70;
		}
		if (m_bGameMaxScore == 70)
		{
			m_bGameMaxScoreGold = 100;
		} 
	}
	else  if(bGameTypeIndex == 3 && m_bGameMaxScore == 0)
	{
		m_bGameMaxScore = 20;
	}
	//ע:�ˮ���������д����  �ⶥֵ ����ֵ ��ֵ   
}

void CTableFrameSink::SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex, BYTE bBaseScore, BYTE bGuDingMa, BYTE bQiangGangHu, BYTE bGameType)
{
	m_bGameTypeIdex = bGameTypeIdex;
	m_bGameRuleIdex = bGameRuleIdex;
	m_bGameBaseScore = bBaseScore;
	m_bQiangGangHu = bQiangGangHu;
	m_bGameCardType = bGameType;
}

void CTableFrameSink::SetCreateUserID(DWORD dwUserID)
{
	IServerUserItem* pUserItem = m_pITableFrame->SearchUserItem(dwUserID);

	m_vHuPlayers.clear();
	if (pUserItem)
	{
		m_wBankerUser = pUserItem->GetChairID();
		m_wBankerUserNext = pUserItem->GetChairID();
	}
}

void CTableFrameSink::setZhuangID(int zhuangID)
{
	m_wBankerUser = zhuangID;
	m_wBankerUserNext = zhuangID;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//SendPiaoDang(wChairID);
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if (bLookonUser==false)
	{
		m_bTrustee[wChairID]=false;
		GDMJ_CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	m_isDingZhuang = false;
	m_isRollDice = false;
	return true;
}
//�û�ѡƮ�͵�
bool  CTableFrameSink::OnUserPiaoDang(WORD wChairID, DWORD cbPiao, DWORD cbDang, IServerUserItem * pIServerUserItem)
{
// 	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PIAODANG);
// 	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PIAODANG) return true;

	if(cbPiao<0&&cbPiao>2)return true;
	if(cbDang<0&&cbDang>3)return true;


	m_bPlayPiao[wChairID] = cbPiao;				//Ʈ
	
	m_bPlayDang[wChairID] = cbDang;				//��
	

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	m_bUserOperate= 0;

	//Ч�����
	if (wChairID!=m_wCurrentUser) return true;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return true;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return true;
	}

	//���ñ���
	m_bSendStatus=true;
	if( m_bGangStatus )
	{
		m_bGangStatus = false;
		m_bGangHuaStatus = false;
		m_bGangOutStatus = true;
	}
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//��������
	GDMJ_CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;
	addGameRecordAction(OutCard);

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	//�û��л�
	// todo zyd
	m_wCurrentUser=(wChairID+1)%m_cbPlayerNum;
	//end
	while( !m_bPlayStatus[m_wCurrentUser] ) 
		m_wCurrentUser=(m_wCurrentUser+m_cbPlayerNum-1)%m_cbPlayerNum;

	bool bAroseAction = false;
	bool bCheck = true;
	if(true)
	{
		int nCardIndex = m_GameLogic.SwitchToCardIndex(cbCardData);
		if( m_bGameTypeIndex != 3 && m_bGameTypeIndex != 7 && (IsHun(nCardIndex) || IsLaiPi(nCardIndex))) // ��������Ƥ���˲��ܲ���
		{
			bCheck= false;
		}
		if(m_bGameTypeIndex != 3 && m_bGameTypeIndex != 7 && IsHun(nCardIndex))
		{
			m_bOutHun[wChairID]++;
			DoFirstGang();
			DispatchCardData(wChairID,true);

			return true;
		}
	}
	if(bCheck)
	{
		//��Ӧ�ж�
		bAroseAction = EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);
	}

	//�ɷ��˿�
	if (bAroseAction == false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�ѡ��
bool CTableFrameSink::OnSelectCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//�������
// 	ASSERT(wChairID==m_wCurrentUser);
// 	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);
// 
// 	//Ч�����
// 	if (wChairID!=m_wCurrentUser) return true;
// 	if (m_GameLogic.IsValidCard(cbCardData)==false) return true;

// 	std::vector<BYTE> cbHuCardList;
// 	getChiHuCardList(wChairID,cbHuCardList);

	GDMJ_CMD_GetHuCardList cardinfos;

	BYTE cbWeaveCount=m_cbWeaveItemCount[wChairID];
	//	BYTE temp = m_GameLogic.AnalyseTingCard_DaoDao( m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID], cbWeaveCount );
	//getChiHuCardList(wChairID,cardinfos.kHuCards);

	//ȥ��Ҫ�����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,m_cbCardIndex[wChairID],sizeof(cbCardIndexTemp) );
	int index = 0;
	for(int i = 0; i < MAX_INDEX; i++)
	{
		BYTE temp = m_cbCardWord[i];
		if(temp == cbCardData)
			index = i;
	}

	cbCardIndexTemp[index]--;

	bool bIsWuFeng = false;
	if(m_bGameCardType == GAME_TYPE_ITEM_WITH_FENG)
		bIsWuFeng = false;
	else if(m_bGameCardType == GAME_TYPE_ITEM_NO_FENG)
		bIsWuFeng = true;

	cardinfos.kHuCards = m_GameLogic.GetTingCard_DaoDao(cbCardIndexTemp,m_WeaveItemArray[wChairID], cbWeaveCount,bIsWuFeng);
	
	datastream kDataStream;
	cardinfos.StreamValue(kDataStream,true);


	m_pITableFrame->SendTableData( wChairID,SUB_S_GAME_SELECT_HU,&kDataStream[0],kDataStream.size() );

	return true;
}

bool CTableFrameSink::OnGetTingCardTag(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

// 	if(m_cbCardIndex[wChairID][31] == 3)
// 		return true;

	GDMJ_CMD_GetOutCardsTing cardinfo;
	cardinfo.bType = cbCardData;
	cardinfo.wChairID = wChairID;
	cardinfo.kOutCards.clear();

	BYTE cbWeaveCount=m_cbWeaveItemCount[wChairID];

	BYTE cbCardCount = m_GameLogic.GetCardCount(m_cbCardIndex[wChairID]);

	bool bIsWuFeng = false;
	if(m_bGameCardType == GAME_TYPE_ITEM_WITH_FENG)
		bIsWuFeng = false;
	else if(m_bGameCardType == GAME_TYPE_ITEM_NO_FENG)
		bIsWuFeng = true;

	BYTE cbCardIndexTemp[MAX_INDEX];
	BYTE tempCardIdx = 0;
	BYTE tempCard = 0;
	bool tempflag = false;
	//�������ϵ�ÿһ����
	for(int i = 0; i < MAX_INDEX; i++)
	{
		if(m_cbCardIndex[wChairID][i] == 0)continue;
		if(tempCardIdx == m_cbCardIndex[wChairID][i] && tempflag)
		{
			cardinfo.kOutCards.push_back(tempCard);
			continue;
		}
		BYTE tempCardIdx = m_cbCardIndex[wChairID][i];
		tempflag = false;
		tempCard = 0;

		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));
		CopyMemory( cbCardIndexTemp,m_cbCardIndex[wChairID],sizeof(cbCardIndexTemp) );
		if( cbCardIndexTemp[i] == 0 ) continue;
		cbCardIndexTemp[i]--;

		bool flag = m_GameLogic.GetTingTag_DaoDao(cbCardIndexTemp,m_WeaveItemArray[wChairID],cbWeaveCount,bIsWuFeng);
		if(flag)
		{
			BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(i);
			tempCard = cbCurrentCard;
			cardinfo.kOutCards.push_back(cbCurrentCard);
			m_sTingTags[wChairID].m_TingTags.push_back(cbCurrentCard);
			tempflag = true;
		}
	}

	datastream kDataStream;
	cardinfo.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_OUT_TING,&kDataStream[0],kDataStream.size());

	return true;
}

bool CTableFrameSink::UpdateTingCardTag(WORD wChairID, BYTE cbCardData, bool isAdd)
{
	if(isAdd)
	{
		if(m_sTingTags[wChairID].m_TingTags.size() == 0)
		{
			OnGetTingCardTag(wChairID, 1);
		}
		else
		{
			ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

			GDMJ_CMD_GetOutCardsTing cardinfo;
			cardinfo.bType = cbCardData;
			cardinfo.wChairID = wChairID;
			cardinfo.kOutCards.clear();

			if(m_cbCardIndex[wChairID][31] == 3)
			{
				std::vector<BYTE>::iterator it = m_sTingTags[wChairID].m_TingTags.begin();
				for(; it != m_sTingTags[wChairID].m_TingTags.end(); ++it)
				{
					BYTE cardData = *it;
					cardinfo.kOutCards.push_back(cardData);
				}

				datastream kDataStream;
				cardinfo.StreamValue(kDataStream,true);

				m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_OUT_TING,&kDataStream[0],kDataStream.size());


				return true;
			}


			BYTE cbWeaveCount=m_cbWeaveItemCount[wChairID];

			BYTE cbCardCount = m_GameLogic.GetCardCount(m_cbCardIndex[wChairID]);

			bool bIsWuFeng = false;
			if(m_bGameCardType == GAME_TYPE_ITEM_WITH_FENG)
				bIsWuFeng = false;
			else if(m_bGameCardType == GAME_TYPE_ITEM_NO_FENG)
				bIsWuFeng = true;

			

			BYTE cbCardIndexTemp[MAX_INDEX];

			ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));
			CopyMemory( cbCardIndexTemp,m_cbCardIndex[wChairID],sizeof(cbCardIndexTemp) );
			
			int index = 0;
			for(int i = 0; i < MAX_INDEX; i++)
			{
				BYTE temp = m_cbCardWord[i];
				if(temp == cbCardData)
					index = i;
			}

			cbCardIndexTemp[index]--;

			bool flag = m_GameLogic.GetTingTag_DaoDao(cbCardIndexTemp,m_WeaveItemArray[wChairID],cbWeaveCount,bIsWuFeng);
			if(flag)
			{
				m_sTingTags[wChairID].m_TingTags.push_back(cbCardData);
			}	

			std::vector<BYTE>::iterator it = m_sTingTags[wChairID].m_TingTags.begin();
			for(; it != m_sTingTags[wChairID].m_TingTags.end(); ++it)
			{
				BYTE cardData = *it;
				cardinfo.kOutCards.push_back(cardData);
			}
			
			datastream kDataStream;
			cardinfo.StreamValue(kDataStream,true);

			m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_OUT_TING,&kDataStream[0],kDataStream.size());
		}
	}
	else
	{
		ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
		if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

		GDMJ_CMD_GetTingCards cardinfo;
		cardinfo.bType = 0;
		cardinfo.kHuCards.clear();
		std::vector<BYTE>::iterator iter = m_sTingTags[wChairID].m_TingTags.begin();
		for(; iter != m_sTingTags[wChairID].m_TingTags.end(); ++iter)
		{
			BYTE cardData = *iter;
			cardinfo.kHuCards.push_back(cardData);
			break;
		}

		datastream kDataStream;
		cardinfo.StreamValue(kDataStream,true);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_TING,&kDataStream[0],kDataStream.size());

		std::vector<BYTE>::iterator it = m_sTingTags[wChairID].m_TingTags.begin();
		for(; it != m_sTingTags[wChairID].m_TingTags.end(); ++it)
		{
			BYTE currentCard = *it;
			if(currentCard == cbCardData)
			{
				it = m_sTingTags[wChairID].m_TingTags.erase(it);
				break;
			}
		}
	}

	return true;
}

bool CTableFrameSink::OnGetTingCardTagClient(WORD wChairID,BYTE cbCardData)
{
	GDMJ_CMD_GetOutCardsTing cardinfo;
	cardinfo.bType = cbCardData;
	cardinfo.wChairID = wChairID;
	cardinfo.kOutCards.clear();

	std::vector<BYTE>::iterator it = m_sTingTags[wChairID].m_TingTags.begin();
	for(; it != m_sTingTags[wChairID].m_TingTags.end(); ++it)
	{
		BYTE cardData = *it;
		cardinfo.kOutCards.push_back(cardData);
	}

	datastream kDataStream;
	cardinfo.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_OUT_TING,&kDataStream[0],kDataStream.size());

	return true;
}

// ������Ʊ�ǩ
bool CTableFrameSink::OnGetTingCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	BYTE cbWeaveCount=m_cbWeaveItemCount[wChairID];

	bool bIsWuFeng = false;
	if(m_bGameCardType == GAME_TYPE_ITEM_WITH_FENG)
		bIsWuFeng = false;
	else if(m_bGameCardType == GAME_TYPE_ITEM_NO_FENG)
		bIsWuFeng = true;

	GDMJ_CMD_GetTingCards cardinfo;
	cardinfo.bType = cbCardData;
	if(cbCardData == 1)
		cardinfo.kHuCards = m_GameLogic.GetTingCard_DaoDao(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],cbWeaveCount,bIsWuFeng);
	else if(cbCardData == 0)
		cardinfo.kHuCards = m_GameLogic.GetTing_DaoDao(m_cbCardIndex[wChairID],m_WeaveItemArray[wChairID],cbWeaveCount,bIsWuFeng);

	datastream kDataStream;
	cardinfo.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_TING,&kDataStream[0],kDataStream.size());

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, DWORD cbOperateCode, BYTE cbOperateCard,BYTE bCheck)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//Ч���û�	ע�⣺�������п��ܷ����˶���
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//�¼� �����������µ����ʾ 20180810
	//if (m_cbUserAction[wChairID] & WIK_CHI_HU && cbOperateCode == WIK_NULL)
	//{
	//	//������
	//	GDMJ_CMD_S_OperateResult OperateResult;
	//	OperateResult.wOperateUser = wChairID;
	//	if (m_wCurrentUser == INVALID_CHAIR)
	//	{
	//		OperateResult.wProvideUser = m_wProvideUser;
	//	}
	//	else
	//		OperateResult.wProvideUser = wChairID;
	//	OperateResult.cbOperateCode = cbOperateCode;
	//	OperateResult.cbOperateCard = cbOperateCard;
	//	OperateResult.wGangSiceNum = 0;
	//	addGameRecordAction(OperateResult);
	//}

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		if (m_bResponse[wChairID]==true) 
			return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return true;

		//��������
		WORD wTargetUser=wChairID;
		DWORD cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=m_cbProvideCard;

		//�жϹ��ֺ�
		if (cbTargetAction == WIK_NULL && (WIK_CHI_HU & m_cbUserAction[wTargetUser]) && m_wProvideUser != wTargetUser)
		{
			if (m_bGameTypeIndex == 6) // ���ݺ��и�
			{
				m_iHuScore[wTargetUser] = AccountScoreHZHZG(wChairID, true);
				m_cbHuType[wTargetUser] = m_cbHuType[m_wProvideUser];
			}
			else if (m_bGameTypeIndex == 4) //�ˮ
			{
				m_iHuScore[wTargetUser] = XiShuiAccountScore(m_cbCardIndex[wChairID],wChairID, false, true);
				m_cbHuType[wTargetUser] = m_cbHuType[m_wProvideUser];
			}
			else if(m_bGameTypeIndex == 3) //���ݻλ�
			{
				int iBaseScore =  0;
				AccountGameScore(wChairID,false,m_ChiHuRightTemp[wTargetUser],iBaseScore,true);
				m_iHuScore[wTargetUser] = m_iHuScore[wTargetUser] < iBaseScore ? iBaseScore : m_iHuScore[wTargetUser];
				if(!(m_ChiHuRightTemp[wTargetUser] & CHR_DA_HU).IsEmpty() )
				{
					m_cbHuType[wTargetUser] = 2;
				}
				else 
				{
					m_cbHuType[wTargetUser] = 1;
				}
			}
			else if(m_bGameTypeIndex == 1 || m_bGameTypeIndex == 2)
			{
				int iHuScore = 0;
				ProcessChiHuUser(wChairID, true, iHuScore, false,m_ChiHuRightTemp[wTargetUser]);
				m_iHuScore[wTargetUser]  = m_iHuScore[wTargetUser] < iHuScore ? iHuScore : m_iHuScore[wTargetUser];
				if(!(m_ChiHuRightTemp[wTargetUser] & CHR_DA_HU).IsEmpty() )
				{
					m_cbHuType[wTargetUser] = 2;
				}
				else 
				{
					m_cbHuType[wTargetUser] = 1;
				}
			}
		}
		//���Լ�δ����һ���Ƶ�һȦ�ڣ��������ϼҶ����¼�
		if( cbTargetAction == WIK_NULL && (WIK_PENG&m_cbUserAction[wTargetUser]) && m_wProvideUser != wTargetUser)
		{
			bool bFound =false;
			if (m_vecEnjoinChiPeng[wTargetUser].size() > 0)
			{
				for (int i=0;i <m_vecEnjoinChiPeng[wTargetUser].size(); i ++ )
				{
					if (m_vecEnjoinChiPeng[wTargetUser][i] == m_cbProvideCard)
					{
						bFound = true;
						break;
					}
				}
			}
			if (bFound)
			{
			}
			else
			{
				m_vecEnjoinChiPeng[wTargetUser].push_back(m_cbProvideCard);
			}
		}

		//ִ���ж�
		for (WORD i=0;i<m_cbPlayerNum;i++)
		{
			//��ȡ����
			DWORD cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//��������
		if (cbTargetAction == WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
			
			//�����˿�
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];

		//���Ʊ���
		m_wOutCardUser=INVALID_CHAIR;
		m_bEnjoinChiHu[wTargetUser]=false;
		m_bEnjoinChiPeng[wTargetUser] = false;	// ��ֹ���ϼ�zyd
		m_vecEnjoinChiPeng[wTargetUser].clear();

		//���Ʋ���
		WORD wLastHuUser = INVALID_CHAIR;
		int nHuCount = 0;
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;
			if (m_bGameTypeIndex == 7) // ��÷
			{
				ClearChiHuDataHM();
				ChiHuHM.resizeData();
				for(BYTE i = 0; i < ChiHuHM.wChiHuUser.size(); i++)
				{
					ChiHuHM.wChiHuUser[i] = 100;
				}
				for(BYTE j = 0; j < GAME_PLAYER; j++)
				{
					if (m_cbUserAction[j] & WIK_CHI_HU)
					{
						if (j == m_wProvideUser) continue;
						m_cbCardIndex[j][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
						AccountScoreHM(m_cbCardIndex[j], j);
						nHuCount++;
						if (m_wBankerUserNext == INVALID_CHAIR)
						{
							m_wBankerUserNext = j;
						}
						m_cbCardIndex[j][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
					}
				}
				SendGameEndScore();
			}
			else 
			{
				for (WORD i=(m_wProvideUser+m_cbPlayerNum+1)%m_cbPlayerNum;i!=m_wProvideUser;i = (i+m_cbPlayerNum+1)%m_cbPlayerNum)
				{
					//�����ж�
					if ((m_cbPerformAction[i] & WIK_CHI_HU) == 0)
						continue;

					//�����ж�
					BYTE cbWeaveItemCount = m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem = m_WeaveItemArray[i];

					m_dwChiHuKind[i] = AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i],i);

					//�����˿�
					if (m_dwChiHuKind[i] != WIK_NULL) 
					{
						if (m_bGameTypeIndex == 6) // ���ݺ��и�
						{
							AccountScoreHZHZG(i, false);
						}
						else if (m_bGameTypeIndex == 8) // Ӣɽ
						{
							AccountScoreYS(i);
						}
						else if(m_bGameTypeIndex == 3)
						{
							int iBasecore = 0;
							if (m_cbQiangGangHu[i] == 1)
							{
								m_ChiHuRight[i] |= CHR_QIANG_GANG;
							}
							AccountGameScore(i,false,m_ChiHuRight[i],iBasecore);
						}
						else if (m_bGameTypeIndex == 4)
						{
							XiShuiAccountScore(m_cbCardIndex[i], i, false, false);
						}
						else 
						{
							int  iScore = 0 ;
							ProcessChiHuUser( i,false, iScore, false,m_ChiHuRight[i]);
						}
						wLastHuUser = i;
						nHuCount++;

						if (m_wBankerUserNext == INVALID_CHAIR)
						{
							m_wBankerUserNext = i;
						}
						break;
					}
				}
			}

			if (nHuCount>=2)
			{
				m_wBankerUserNext = m_wProvideUser;
			}
			//�жϽ���
			BYTE cbPlayCount = 0;
			for( WORD i = 0; i < m_cbPlayerNum; i++ )
			{
				if( m_bPlayStatus[i] ) cbPlayCount++;
			}

			if( cbPlayCount < 2 || hasRule(GAME_RULE_DAODAO_HU)) 
			{
				m_wResumeUser = wLastHuUser;
				OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );
			}
			else
			{
				//�û�״̬
				ZeroMemory(m_bResponse,sizeof(m_bResponse));
				ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
				ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
				ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
				{
					//�����˿�
					m_wResumeUser = (wLastHuUser+GAME_PLAYER-1)%GAME_PLAYER;
				}
				DispatchCardData(m_wResumeUser);
			}

			return true;
		}
		removeCardByAction(wTargetUser,cbTargetAction,cbTargetCard);
		if (cbTargetAction >= WIK_LEFT && cbTargetAction <= WIK_GANG)
		{
			m_bOpenMouth[wTargetUser] = 1;
			m_bEnjoyGang[wTargetUser] = true;
			m_iHuScore[wTargetUser] = -1;
			m_cbHuType[wTargetUser] = 0;
			if (cbTargetAction == WIK_PENG) m_cbPeng[wTargetUser] = 1;
		}		
		m_nUsefulOper[wTargetUser]++;
		
		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
		
		{
			//����˿�	
			WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
			m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
			m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
			m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		}		
		if ((m_bEnjoinChiPeng[wTargetUser] == false) && (m_cbLeftCardCount > 0))
		{
			tagGangCardResult GangCardResult;
			GangCardResult.cbCardData[0] = m_cbSendCardData;
			if(m_bHasHuPai[wTargetUser])
			{
				bool bHasYiChang = false;
				std::vector<BYTE> cbHuCardList;
				getChiHuCardList(wTargetUser,cbHuCardList);
				m_cbUserAction[wTargetUser]|=m_GameLogic.AnalyseGangCard_HasHu(bHasYiChang,m_cbCardIndex[wTargetUser],
					m_WeaveItemArray[wTargetUser],m_cbWeaveItemCount[wTargetUser],GangCardResult,cbHuCardList);
			}
			else
			{
				m_cbUserAction[wTargetUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wTargetUser],
					m_WeaveItemArray[wTargetUser],m_cbWeaveItemCount[wTargetUser],GangCardResult);
			}
		}
		else if(m_cbLeftCardCount > 0)
		{
			tagGangCardResult GangCardResult;
			GangCardResult.cbCardData[0] = m_cbSendCardData;
			m_cbUserAction[wTargetUser] |= m_GameLogic.AnalyseGangCard(m_cbCardIndex[wTargetUser],
				m_WeaveItemArray[wTargetUser],m_cbWeaveItemCount[wTargetUser],GangCardResult);
		}

		//������
		WORD wProvideUser = (m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		if(cbTargetAction == WIK_GANG && m_bGameTypeIndex == 3)  // ���ݸܴ� ����ҡ����ץ��
		{
			BYTE cbNum1 = g_RandKiss.RandKiss() % 6 + 1;
			BYTE cbNum2 = g_RandKiss.RandKiss() % 6 + 1;
			WORD wSiceNum = MAKEWORD(cbNum1, cbNum2);
			BYTE bcSiceNumTemp = cbNum1 + cbNum2;
			if (bcSiceNumTemp == m_cbSiceCountPi && !m_bPiGang) 
			{
				DoSicePiGang();
			}
			SendOperateResulet(wTargetUser, wProvideUser, cbTargetAction, cbTargetCard, wSiceNum);
			for (BYTE i = 0; i < 16; i++)
			{
				if (m_cbGangSiceNum[i] == 0)
				{
					m_cbGangSiceNum[i] = bcSiceNumTemp;
					break;
				}
			}
		}
		else
		{
			SendOperateResulet(wTargetUser,wProvideUser,cbTargetAction,cbTargetCard);
		}
		bool bCanQFJ = false;
		if (m_bGameTypeIndex == 7 || m_bGameTypeIndex == 3 || (m_bGameTypeIndex == 8 && !m_bBaoIsOutCard) || m_bGameTypeIndex == 9) ;
		else if (cbTargetAction <= WIK_PENG && cbTargetAction >= WIK_LEFT)
		{
			bCanQFJ = true;
		}
		else if (cbTargetAction == WIK_GANG && wProvideUser != wTargetUser)
		{
			bCanQFJ = true;
		}
		if (bCanQFJ)
		{
			if (m_nUsefulOper[wTargetUser] == 2)
			{
				BYTE cbWeaveItemCount = m_cbWeaveItemCount[wTargetUser];
				tagWeaveItem * pWeaveItem = m_WeaveItemArray[wTargetUser];
				//if (m_bGameTypeIndex != 3)
				{
					m_nCheckQingFengJiang[wTargetUser] = m_GameLogic.AnalyseWave(pWeaveItem,cbWeaveItemCount);
					m_cbUserAction[wTargetUser] |= m_nCheckQingFengJiang[wTargetUser];
				}
			}
			if (m_nCheckQingFengJiang[wTargetUser] > 0 && m_nUsefulOper[wTargetUser]== 3)
			{
				BYTE cbWeaveItemCount = m_cbWeaveItemCount[wTargetUser];
				tagWeaveItem * pWeaveItem = m_WeaveItemArray[wTargetUser];
				DWORD dwType = m_GameLogic.AnalyseWave(pWeaveItem, cbWeaveItemCount);
				m_bOpenProvider[wTargetUser] = wProvideUser;
				if(m_bTalkQingFengQing[wTargetUser]==  false)
				{
					m_nCheckQingFengJiang[wTargetUser] = 0;
				}
				if ( (dwType & m_nCheckQingFengJiang[wTargetUser]) > 0 )
				{
					for (int i = 0 ;i < cbWeaveItemCount;i ++)
					{
						if (pWeaveItem->cbWeaveKind == (dwType & WIK_QING_BAO) || pWeaveItem->cbWeaveKind == (dwType & WIK_FENG_BAO) || pWeaveItem->cbWeaveKind == (dwType & WIK_JIANG_BAO))
						{
							pWeaveItem->wProvideUser = wProvideUser;
							SendOperateResulet(wTargetUser,wProvideUser,m_nCheckQingFengJiang[wTargetUser],1);
							break;
						}
						pWeaveItem ++;
					}
				}
			}
		}		
		
		// ����ǻ���  ������Ƥ�� ��� ��Ƥ�ܲ���
		if(m_bGameTypeIndex==3 && cbTargetAction <= WIK_GANG_PI && cbTargetAction >= WIK_GANG_HUN )
		{
			return  false;
		}
		// ����û����������
		if(m_bGameTypeIndex!=3 && cbTargetAction >= WIK_SHOW_BAO)
		{
			return  false;
		}
		if (m_bGameTypeIndex == 8 && cbTargetAction == WIK_PENG) // Ӣɽ���ƺ��ж�ȫ����
		{
			// ���ȫ����
			if (bCheckAllOther(m_cbCardIndex[m_wCurrentUser]))
			{
				m_cbUserAction[m_wCurrentUser] |= WIK_ALL_OTHER;
			}
		}
		SendOperateNotify();
		//�����û�
		m_wCurrentUser=wTargetUser;
		if (m_bGameTypeIndex == 7 && cbTargetAction == WIK_PENG) // ��÷�����ƴ���
		{
			if (m_cbCardIndex[m_wCurrentUser][32] == 4)
			{
				bool bPublic = false;
				if(!cheakSelfGang(m_wCurrentUser,0x36,bPublic))
				{
					return false;
				}
				SendOperateResulet(m_wCurrentUser,m_wCurrentUser,WIK_GANG,0x36);
				DoFirstGang();
				DispatchCardData(m_wCurrentUser,true);
				return true;
			}
			else if (m_cbCardIndex[m_wCurrentUser][32] > 0)
			{
				m_cbCardIndex[m_wCurrentUser][32]--;
				m_cbFaCaiNum[m_wCurrentUser]++;
				SendOperateResulet(m_wCurrentUser,m_wCurrentUser,WIK_FA_CAI,0x36);
				DoFirstGang();
				DispatchCardData(m_wCurrentUser,true);
				return true;
			}
		}
		else if (cbTargetAction==WIK_GANG)//���ƴ���
		{
			SCORE nScore = 1;
			sendGangScore(nScore,wTargetUser,m_wProvideUser,false);

			m_bGangStatus = true;
			if (hasRule(GAME_DIAN_GANG_HUA))
			{
				m_bGangHuaStatus = true;
				m_gProvideUser = m_wProvideUser;
			}
			if (m_bGameTypeIndex == 2 || m_bGameTypeIndex == 7) // ����ܴ�
			{
				DoFirstGang();
			}
			else if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8) // Ϫˮ�����ݺ��и�
			{
				m_cGangCountXS++;
				if (m_cGangCountXS == m_cbSiceCountPi && !m_bPiGang)
				{
					DoSicePiGangXS();
				}
			}
			DispatchCardData(wTargetUser,true);
		}
		else
		{
			m_pITableFrame->KillGameTimer(IDI_TIMER_CHUPAI);
			m_pITableFrame->SetGameTimer(IDI_TIMER_CHUPAI,(TIME_CHUPAI+2)*1000,1,0);
		}

		return true;
	}

	//��������
	if (m_wCurrentUser == wChairID)
	{
		//Ч�����
		if (cbOperateCode < WIK_GANG_PI)
		{
			if (cbOperateCode == WIK_NULL && (WIK_CHI_HU & m_cbUserAction[wChairID]))
			{
				if (m_bGameTypeIndex == 6) // ���ݺ��и�
				{
					m_iHuScore[wChairID] = AccountScoreHZHZG(wChairID, true);
					m_cbHuType[wChairID] = m_cbHuType[wChairID];
				}
				else if (m_bGameTypeIndex == 4) //�ˮ
				{
					m_iHuScore[wChairID] = XiShuiAccountScore(m_cbCardIndex[wChairID], wChairID, false, true);
					m_cbHuType[wChairID] = m_cbHuType[wChairID];
				}
				else if(m_bGameTypeIndex == 3)
				{
					int iBaseScore = 0;
					AccountGameScore(wChairID,false,m_ChiHuRightTemp[m_wCurrentUser],iBaseScore,true);
					m_iHuScore[m_wCurrentUser] = iBaseScore;
					// ����
					if(!(m_ChiHuRightTemp[m_wCurrentUser] & CHR_DA_HU).IsEmpty() )
					{
						m_cbHuType[m_wCurrentUser] = 2;
					}
					else 
					{
						m_cbHuType[m_wCurrentUser] = 1;
					}
				}
				else if(m_bGameTypeIndex == 1 || m_bGameTypeIndex == 2)
				{
					int iHuScore = 0;
					ProcessChiHuUser(wChairID, true, iHuScore, false,m_ChiHuRightTemp[m_wCurrentUser]);
					m_iHuScore[m_wCurrentUser] = m_iHuScore[m_wCurrentUser] < iHuScore ? iHuScore : m_iHuScore[m_wCurrentUser];
					if(!(m_ChiHuRightTemp[m_wCurrentUser] & CHR_DA_HU).IsEmpty() )
					{
						m_cbHuType[m_wCurrentUser] = 2;
					}
					else 
					{
						m_cbHuType[m_wCurrentUser] = 1;
					}
				}
			}
			if ((cbOperateCode==WIK_NULL) || ((m_cbUserAction[wChairID] & cbOperateCode) == 0))
			{	
				return true;
			}
		}

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL) || (cbOperateCode==WIK_CHI_HU) || (cbOperateCode == WIK_SHOW_BAO) || 
			 (cbOperateCode == WIK_ALL_OTHER) || (m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!= WIK_NULL) && (cbOperateCode!=WIK_CHI_HU) && (cbOperateCode != WIK_SHOW_BAO) && 
			(cbOperateCode != WIK_ALL_OTHER) && (m_GameLogic.IsValidCard(cbOperateCard)==false)) 
		{
			//������ʾ
			TCHAR szDescribe[64]=TEXT("");
			sprintf_s(szDescribe,TEXT("cbOperateCard = %02x"), cbOperateCard);
			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Normal);
			return false;
		}
		//���ñ���
		DWORD dwUserActionTemp[GAME_PLAYER]; //�û�������ʱ����
		ZeroMemory(dwUserActionTemp, sizeof(dwUserActionTemp));
		m_bSendStatus=true;
		m_bEnjoinChiHu[m_wCurrentUser]=false;
		memcpy(dwUserActionTemp, m_cbUserAction, sizeof(dwUserActionTemp));
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;
		m_bEnjoinChiPeng[m_wCurrentUser] = false;	// ��ֹ���ϼ�zyd
		m_vecEnjoinChiPeng[m_wCurrentUser].clear();

		bool bPublic=false;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_ALL_OTHER: // Ӣɽȫ����
			{
				if (m_bGameTypeIndex != 8) return false;
				m_cbTalkAllOther[wChairID] = 1;
				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				return true;
			}
		case WIK_QING_BAO:			//
			{
				m_bTalkQingFengQing[wChairID] = true;
				m_nUsefulQingFengJiang[wChairID] = WIK_QING_BAO;
				int cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_QING_BAO;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				if (dwUserActionTemp[wChairID] & WIK_GANG) // ���ܸ��Ʋ���
				{
					m_cbUserAction[wChairID] = WIK_GANG;
				}
				if (m_cbUserAction[wChairID])
				{
					//������ʾ
					SendOperateNotify();
				}
				return true;
			}
		case WIK_FENG_BAO:			//
			{
				m_bTalkQingFengQing[wChairID] = true;
				m_nUsefulQingFengJiang[wChairID] = WIK_FENG_BAO;
				int cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=m_bOpenProvider[wChairID];
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_FENG_BAO;			
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				if (dwUserActionTemp[wChairID] & WIK_GANG) // ���ܸ��Ʋ���
				{
					m_cbUserAction[wChairID] = WIK_GANG;
				}
				if (m_cbUserAction[wChairID])
				{
					//������ʾ
					SendOperateNotify();
				}
				return true;
			}
		case WIK_JIANG_BAO:			//
			{
				m_bTalkQingFengQing[wChairID] = true;
				m_nUsefulQingFengJiang[wChairID] = WIK_JIANG_BAO;
				int cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=m_bOpenProvider[wChairID];
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_JIANG_BAO;			
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				if (dwUserActionTemp[wChairID] & WIK_GANG) // ���ܸ��Ʋ���
				{
					m_cbUserAction[wChairID] = WIK_GANG;
				}
				if (m_cbUserAction[wChairID])
				{
					//������ʾ
					SendOperateNotify();
				}
				return true;
			}
		case WIK_GANG_HUN:			//���Ʋ���
			{
				bool bPublic = false;
				m_bGangStatus = true;
				if(!cheakSelfGangPi(3,wChairID,cbOperateCard,bPublic))
				{
					return false;
				}

				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);

				LONGLONG lScore = 1;
				sendGangScore(lScore,wChairID,wChairID,!bPublic);	
				if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8)
				{
					m_cGangCountXS++;
					if (m_cGangCountXS == m_cbSiceCountPi && !m_bPiGang)
					{
						DoSicePiGangXS();
					}
				}
				else
					DoFirstGang();
				DispatchCardData(wChairID,true);

				return true;
			}
		case WIK_GANG_LAIPI:			//���Ʋ���
			{
				bool bPublic = false;
				m_bGangStatus = true;
				if(!cheakSelfGangPi(2,wChairID,cbOperateCard,bPublic))
				{
					return false;
				}

				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);

				LONGLONG lScore = 1;
				sendGangScore(lScore,wChairID,wChairID,!bPublic);	

				if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8)
				{
					m_cGangCountXS++;
					if (m_cGangCountXS == m_cbSiceCountPi && !m_bPiGang)
					{
						DoSicePiGangXS();
					}
				}
				else
					DoFirstGang();
				DispatchCardData(wChairID,true);

				return true;
			}
		case WIK_GANG_PI:			//���Ʋ���
			{
				bool bPublic = false;
				m_bGangStatus = true;

				if(!cheakSelfGangPi(1,wChairID,cbOperateCard,bPublic))
				{
					return false;
				}
				
				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);

				LONGLONG lScore = 1;
				sendGangScore(lScore,wChairID,wChairID,!bPublic);	
				if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8)
				{
					m_cGangCountXS++;
					if (m_cGangCountXS == m_cbSiceCountPi && !m_bPiGang)
					{
						DoSicePiGangXS();
					}
				}
				else
					DoFirstGang();
		
				DispatchCardData(wChairID,true);
				
				return true;
			}	
		case WIK_SHOW_BAO: //��������
			{
				if (m_iBaoIndex != cbOperateCard)
				{
					return false;
				}
				m_bShowBao[wChairID] = true;
				
				CChiHuRight chr;
				DWORD dwoper = AnalyseChiHuCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbSendCardData, chr, wChairID, 1);
				m_cbUserAction[wChairID]|=dwoper;

				if (dwUserActionTemp[m_wCurrentUser] & WIK_GANG) // ���и�
				{
					m_cbUserAction[m_wCurrentUser] = WIK_GANG;
				}

				SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);

				//������ʾ
				SendOperateNotify();
				return  true;
			}
		
		case WIK_GANG:			//���Ʋ���
			{
				bool bPublic = false;
				if(!cheakSelfGang(wChairID,cbOperateCard,bPublic))
				{
					return false;
				}
				m_bGangStatus = true;
				
				if(m_bGameTypeIndex == 3)
				{
					BYTE cbNum1 = g_RandKiss.RandKiss() % 6 + 1;
					BYTE cbNum2 = g_RandKiss.RandKiss() % 6 + 1;
					WORD wSiceNum = MAKEWORD(cbNum1, cbNum2);
					BYTE bcSiceNumTemp = cbNum1 + cbNum2;
					if (bcSiceNumTemp == m_cbSiceCountPi && !m_bPiGang) 
					{
						DoSicePiGang();
					}
					SendOperateResulet(wChairID, wChairID, cbOperateCode, cbOperateCard, wSiceNum);
					for (BYTE i = 0; i < m_cbPlayerNum; i++)
					{
						if (m_cbGangSiceNum[i] == 0)
						{
							m_cbGangSiceNum[i] = bcSiceNumTemp;
							break;
						}
					}
					if (dwUserActionTemp[m_wCurrentUser] & WIK_SHOW_BAO) // ����������
					{
						m_cbUserAction[m_wCurrentUser] = WIK_SHOW_BAO;
						//������ʾ
						SendOperateNotify();
					}
				}
				else if (m_bGameTypeIndex == 4 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8)
				{
					m_cGangCountXS++;
					if (m_cGangCountXS == m_cbSiceCountPi && !m_bPiGang)
					{
						DoSicePiGangXS();
					}
					SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				}
				else 
				{
					SendOperateResulet(wChairID,wChairID,cbOperateCode,cbOperateCard);
				}

				//���Ƶ÷�
				LONGLONG lScore = 1;
				sendGangScore(lScore,wChairID,wChairID,!bPublic);

				//Ч�鶯��
				bool bAroseAction=false;
				if (bPublic == true)
				{
					bAroseAction=EstimateUserRespond(wChairID,cbOperateCard,EstimatKind_GangCard);
				}
				//�����˿�
				if (bAroseAction==false)
				{
					if (m_bGameTypeIndex == 2 || m_bGameTypeIndex == 7) //  ����ܴ�
					{
						DoFirstGang();
					}
					DispatchCardData(wChairID,true);
				}
				return true;
			}		
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				if (m_cbOutCardCount==0)
				{
					m_wProvideUser = m_wCurrentUser;
					m_cbProvideCard = m_cbSendCardData;
				}

				//��ͨ����
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1);

				m_dwChiHuKind[wChairID] = AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID],wChairID);

				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;
				if (m_bGameTypeIndex == 7) // ��÷
				{
					m_ChiHuRightHM[wChairID].SetEmpty();
					m_ChiHuRightHM[wChairID] = m_ChiHuRight[wChairID];
					ClearChiHuDataHM();
					ChiHuHM.resizeData();
					for(BYTE i = 0; i < ChiHuHM.wChiHuUser.size(); i++)
					{
						ChiHuHM.wChiHuUser[i] = 100;
					}
					AccountScoreHM(m_cbCardIndex[wChairID], wChairID);
					SendGameEndScore();
				}
				else if (m_bGameTypeIndex == 8) // Ӣɽ
				{
					AccountScoreYS(wChairID);
				}
				else if (m_bGameTypeIndex == 6) // ���ݺ��и�
				{
					AccountScoreHZHZG(wChairID, false);
				}
				else if(m_bGameTypeIndex == 3) // ����
				{
					int iBasecore = 0;
					AccountGameScore(wChairID,false,m_ChiHuRight[wChairID],iBasecore);
				}
				else if (m_bGameTypeIndex == 4) // Ϫˮ
				{
					XiShuiAccountScore(m_cbCardIndex[wChairID], wChairID, false, false);
				}
				else 
				{
					int  iScore = 0 ;
					ProcessChiHuUser( wChairID, false, iScore, false, m_ChiHuRight[wChairID]);
				}
				

				if (m_wBankerUserNext == INVALID_CHAIR)
				{
					m_wBankerUserNext = wChairID;
				}

				//�жϽ���
				BYTE cbPlayCount = 0;
				for( WORD i = 0; i < m_cbPlayerNum; i++ )
				{
					if( m_bPlayStatus[i] ) cbPlayCount++;
				}

				if( cbPlayCount < 2 || hasRule(GAME_RULE_DAODAO_HU)) 			
				{
					OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );
				}
				else
				{
					//����
					m_wCurrentUser = (m_wCurrentUser+GAME_PLAYER-1)%GAME_PLAYER;
					while( !m_bPlayStatus[m_wCurrentUser] )
						m_wCurrentUser = (m_wCurrentUser+GAME_PLAYER-1)%GAME_PLAYER;
					DispatchCardData( m_wCurrentUser,false );
				}

				return true;
			}		
		
		}

		return true;
	}

	return true;
}

bool CTableFrameSink::SendPiaoDang(WORD wChairID)
{
// 	//������ʾ
// 	for (WORD i=0;i<GAME_PLAYER;i++)
// 	{
// 		if (i == wChairID)
// 		{
// 			//��������
// 			GDMJ_CMD_S_OperatePiaoDang OperateNotify;
// 			OperateNotify.cbPiao=0;
// 			OperateNotify.cbDang=0;
// 			OperateNotify.cbUser=i;
// 
// 			//��������
// 			m_pITableFrame->SendTableData(i,SUB_S_PIAODANG,&OperateNotify,sizeof(OperateNotify));
// 			m_pITableFrame->SendLookonData(i,SUB_S_PIAODANG,&OperateNotify,sizeof(OperateNotify));
// 
// 			//m_pITableFrame->KillGameTimer(IDI_TIMER_ACTION);
// 			//m_pITableFrame->SetGameTimer(IDI_TIMER_ACTION,(TIME_ACTION+2)*1000,1,0);
// 
// 		}
// 		else
// 		{
// 			//��������
// 			GDMJ_CMD_S_OperatePiaoDang OperateNotify;
// 			OperateNotify.cbPiao=m_bPlayPiao[i];
// 			OperateNotify.cbDang=m_bPlayDang[i];
// 			OperateNotify.cbUser=i;
// 
// 			//��������
// 			m_pITableFrame->SendTableData(i,SUB_S_PIAODANG,&OperateNotify,sizeof(OperateNotify));
// 			m_pITableFrame->SendLookonData(i,SUB_S_PIAODANG,&OperateNotify,sizeof(OperateNotify));
// 
// 		}
// 	}


// 	for (WORD i=0;i<GAME_PLAYER;i++)
// 	{
		//��������
		GDMJ_CMD_S_OperateReqPiaoDang OperateNotify;
		OperateNotify.cbUser=1;


		//��������
		m_pITableFrame->SendTableData(wChairID,SUB_S_REQ_PIAODANG,&OperateNotify,sizeof(OperateNotify));
		m_pITableFrame->SendLookonData(wChairID,SUB_S_REQ_PIAODANG,&OperateNotify,sizeof(OperateNotify));

		//m_pITableFrame->SendGameStatus(&OperateNotify,sizeof(OperateNotify));
/*	}*/

	return true;

}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_cbPlayerNum;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			GDMJ_CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];
			OperateNotify.bHasHuPai = m_bHasHuPai[i];

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));

			m_pITableFrame->KillGameTimer(IDI_TIMER_ACTION);
			m_pITableFrame->SetGameTimer(IDI_TIMER_ACTION,(TIME_ACTION+2)*1000,1,0);

		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;

	m_bUserOperate = 0;
	m_bOperateCard=0;

	//��ׯ����
	int  liuJuCount = m_cbLeftCardCount - getLiuJuCount();
	if(liuJuCount == 0)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;

		if (m_wBankerUserNext == INVALID_CHAIR )
		{
			int nSize = m_kGameRecord.kAction.size();
			for (int i=nSize-1;i>=0;i--)
			{
				if (m_kGameRecord.kAction[i].cbActionType == GDMJGameRecordOperateResult::TYPE_SendCard)
				{
					m_wBankerUserNext = (m_wBankerUser+1+m_cbPlayerNum)%m_cbPlayerNum;
					break;
				}
			}
		}
		else if (m_wBankerUserNext == INVALID_CHAIR)
		{
			m_wBankerUserNext = (m_wBankerUser-1+m_cbPlayerNum)%m_cbPlayerNum;
		}
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//���ñ���
	m_wCurrentUser=wCurrentUser;
	m_bEnjoinChiHu[wCurrentUser]=false;
	m_iHuScore[wCurrentUser] = -1;
	m_cbHuType[wCurrentUser] = 0;

	//�ܺ���
	if( m_bGangOutStatus )
	{
		m_bGangOutStatus = false;
	}

	//���ƴ���
	if (m_bSendStatus==true)
	{
		//�����˿�
		m_cbSendCardCount++;
		m_cbSendCardData = getSendCardData(m_wCurrentUser);

		//���ñ���
		bool bCanHu = true;
		m_wProvideUser=wCurrentUser;
		//�ж�Ӣɽ��ȫ����
		if (m_bGameTypeIndex == 8)
		{
			if (bCheckAllOther(m_cbCardIndex[wCurrentUser]))
			{
				// ��ʾ��ȫ����
				m_cbUserAction[wCurrentUser] |= WIK_ALL_OTHER;
			}
		}
		//�����ж�
		if (m_bGameTypeQing == 1)
		{
			if (m_bOpenMouth[m_wCurrentUser] == 0)
			{
				bCanHu = false;
			}
			else if (m_bGameTypeIndex == 4 && m_cbCardIndex[m_wCurrentUser][m_nHunIndex] > 1)
			{
				bCanHu = false;
			}
		}
		if (m_bGameTypeIndex == 7 && (m_cbSendCardData == 0x36 || m_cbCardIndex[m_wCurrentUser][32] > 0)) bCanHu = false;
		if (bCanHu)
		{
			CChiHuRight chr;
			if(m_bGameTypeIndex == 3 && m_iBaoIndex == m_GameLogic.SwitchToCardIndex(m_cbSendCardData)) 
			{
				m_iBaoNum[wCurrentUser]++;
				if (m_bOpenMouth[wCurrentUser] == 0)  //û�п���
				{
					m_iStartBaoNum[wCurrentUser]++;
					if(m_iStartBaoNum[wCurrentUser] >= 2) 
					{
						m_bEnjoyGang[wCurrentUser]  = false;
					}
				}
				else // ������
				{
					m_iOpenMouthBaoNum[wCurrentUser]++;
					/* �����齫����1���� ���ҿ�����û������ʱ����ѡ��������
					����û�б���������������һ���������Բ����������˶���������ѡ�������߲���*/
					if (m_bOpenMouth[wCurrentUser] == 1 ) 
					{
						if (m_iStartBaoNum[wCurrentUser] == 1 && m_iOpenMouthBaoNum[wCurrentUser] > 0)
						{
							m_cbUserAction[wCurrentUser] |= WIK_SHOW_BAO;
						}
						else if (m_iStartBaoNum[wCurrentUser] == 0 && m_iOpenMouthBaoNum[wCurrentUser] >= 2)
						{
							m_cbUserAction[wCurrentUser] |= WIK_SHOW_BAO;
						}
					}
				}
			}
			if(m_bGameTypeIndex != 3 && m_bGameTypeIndex != 7 && (m_bGameTypeIndex == 8 && !m_bBaoIsOutCard) && m_bGameTypeIndex == 9)
			{
				if (m_nUsefulOper[wCurrentUser] >= 2 )
				{
					BYTE cbWeaveItemCount = m_cbWeaveItemCount[wCurrentUser];
					tagWeaveItem * pWeaveItem = m_WeaveItemArray[wCurrentUser];
					if(m_nCheckQingFengJiang[wCurrentUser] !=0 &&m_bTalkQingFengQing[wCurrentUser] !=true )
					{
						DWORD   dwtype= m_GameLogic.AnalyseWave(pWeaveItem,cbWeaveItemCount);
						m_cbUserAction[wCurrentUser] |= dwtype;
					}
				}
			}

			DWORD dwoper = AnalyseChiHuCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], m_cbSendCardData, chr, wCurrentUser);
			if (m_bGameTypeIndex == 7 && (dwoper & WIK_CHI_HU))
			{
				m_ChiHuRightHM[wCurrentUser].SetEmpty();
				m_ChiHuRightHM[wCurrentUser] = chr;
			}
			if(m_nUsefulQingFengJiang[wCurrentUser] != 0)
			{
				if( (!(chr&CHR_QING_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[wCurrentUser] == WIK_QING_BAO)
				{
					m_cbUserAction[wCurrentUser]|=dwoper;
				}
				if( (!(chr&CHR_FENG_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[wCurrentUser] == WIK_FENG_BAO)
				{
					m_cbUserAction[wCurrentUser]|=dwoper;
				}
				if( (!(chr&CHR_JIANG_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[wCurrentUser] == WIK_JIANG_BAO)
				{
					m_cbUserAction[wCurrentUser]|=dwoper;
				}			
			}
			else
			{
				m_cbUserAction[wCurrentUser]|=dwoper;
			}
			if (m_bGameTypeIndex != 7 && (m_cbUserAction[wCurrentUser] & WIK_CHI_HU))
			{
				m_ChiHuRightTemp[wCurrentUser].SetEmpty();
				m_ChiHuRightTemp[wCurrentUser] = chr;
				if (m_bGameTypeIndex == 9) // ���
				{
					if (AccountScoreMC(wCurrentUser, true) == -1)
					{
						m_cbUserAction[wCurrentUser] &= 0xFFFFFBF;
					}
				}
				else if (m_bGameTypeIndex == 8) // Ӣɽ
				{
					if (AccountScoreYS(wCurrentUser, true) == -1)
					{
						m_cbUserAction[wCurrentUser] &= 0xFFFFFBF;
					}
				}
				else if (m_bGameTypeIndex == 4) // Ϫˮ
				{
					if (XiShuiAccountScore(m_cbCardIndex[wCurrentUser], wCurrentUser, false, true) == -1)
					{
						m_cbUserAction[wCurrentUser] &= 0xFFBF;
					}
					else
					{
						if (m_cbUserAction[wCurrentUser] & WIK_CHI_HU)
						{
							if (!(chr & CHR_DA_HU).IsEmpty())
							{
								m_cbHuType[wCurrentUser] = 2;
							}
							else
								m_cbHuType[wCurrentUser] = 1;
						}
					}
				}
				else if (m_bGameTypeIndex == 6) // ���ݺ��и�
				{
					if (AccountScoreHZHZG(wCurrentUser, true) == -1)
					{
						m_cbUserAction[wCurrentUser] &= 0xFFBF;
					}
					else
					{
						if (!(chr & CHR_DA_HU).IsEmpty())
						{
							m_cbHuType[wCurrentUser] = 2;
						}
						else
							m_cbHuType[wCurrentUser] = 1;
					}
				}
			}
		}

		//����
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
		if (m_bGameTypeIndex == 7 && m_cbCardIndex[wCurrentUser][32] > 0);
		else
		{
			if ((m_bEnjoinChiPeng[wCurrentUser]==false)&&(m_cbLeftCardCount>0))
			{
				tagGangCardResult GangCardResult;
				GangCardResult.cbCardData[0] = m_cbSendCardData;
				if(m_bHasHuPai[wCurrentUser])
				{
					bool bHasYiChang = false;
					std::vector<BYTE> cbHuCardList;
					getChiHuCardList(wCurrentUser,cbHuCardList);
					m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard_HasHu(bHasYiChang,m_cbCardIndex[wCurrentUser],
						m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult,cbHuCardList);
				}
				else
				{
					m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
						m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
				}
			}
			else if(m_cbLeftCardCount>0)
			{
				tagGangCardResult GangCardResult;
				GangCardResult.cbCardData[0] = m_cbSendCardData;
				m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
					m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
			}
		}
	}

	//������Ϣ
	ASSERT( m_wHeapHand != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHand][0]++;
	}
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//��������
	GDMJ_CMD_S_SendCard SendCard;
	memset(&SendCard, 0, sizeof(GDMJ_CMD_S_SendCard));
	SendCard.wCurrentUser=wCurrentUser;	
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;
	SendCard.bHasHuPai = m_bHasHuPai[wCurrentUser];
	addGameRecordAction(SendCard);
	if (SendCard.cbActionMask == WIK_NULL)
	{
		SendCard.nLeftTime = TIME_CHUPAI;
	}
	else
	{
		SendCard.nLeftTime = TIME_ACTION;
	}

	m_bEnjoinChiPeng[m_wCurrentUser] = false;	// ��ֹ���ϼ�zyd

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	//m_pITableFrame->KillGameTimer(IDI_TIMER_CHUPAI);
	//m_pITableFrame->SetGameTimer(IDI_TIMER_CHUPAI,(SendCard.nLeftTime+2)*1000,1,0);
	//�жϻ�÷�ķ���
	if (m_bGameTypeIndex == 7 && m_cbCardIndex[wCurrentUser][32] > 0)
	{
		if (m_cbCardIndex[wCurrentUser][32] == 4)
		{
			bool bPublic = false;
			if(!cheakSelfGang(wCurrentUser,0x36,bPublic))
			{
				return false;
			}
			SendOperateResulet(wCurrentUser,wCurrentUser,WIK_GANG,0x36);
			DoFirstGang();
			DispatchCardData(wCurrentUser,true);
			return true;
		}
		else
		{
			m_cbCardIndex[wCurrentUser][32]--;
			m_cbFaCaiNum[wCurrentUser]++;
			SendOperateResulet(wCurrentUser,wCurrentUser,WIK_FA_CAI,0x36);
			DoFirstGang();
			DispatchCardData(wCurrentUser,true);
			return true;
		}
	}
	return true;
}

//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
	
	//�����ж�
	for (WORD i=0;i<m_cbPlayerNum;i++)
	{
		m_cbQiangGangHu[i] = 0;
		//�û�����
		if (wCenterUser==i||!m_bPlayStatus[i] ) continue;

		if (m_nQueColor[i] == m_GameLogic.GetCardColor(cbCenterCard))
		{
			//�����ȱ�� ֱ�������ж�
			continue;
		}
		//��������
		if (EstimatKind == EstimatKind_OutCard)
		{
			if (m_bEnjoinChiPeng[i]==false)
			{
				if(m_bGameTypeIndex != 7 && (i == wCenterUser + 1 || i == wCenterUser - (m_cbPlayerNum - 1)))
				{
					bool bCan_Chi = true;
					DWORD is_chi = 0;
					if (bCan_Chi)
					{
						is_chi = m_GameLogic.EstimateEatCard(m_cbCardIndex[i], cbCenterCard);
					}
					if(is_chi)
					{
						m_cbUserAction[i] |= is_chi;
					}
				}
				DWORD is_peng = m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);
				if(is_peng)
				{
					m_cbUserAction[i] |= is_peng;
				}

				//�����ж�
				if (m_cbLeftCardCount > 0 && !m_bHasHuPai[i]) 
				{
					if(m_bGameTypeIndex == 3 &&  m_bEnjoyGang == false)
					{

					}
					else 
					{
						m_cbUserAction[i] |= m_GameLogic.EstimateGangCard(m_cbCardIndex[i], cbCenterCard);
					}
				}
			}
		}

		//�����ж�
		CChiHuRight chr;
		BYTE cbWeaveCount = m_cbWeaveItemCount[i];
		// edit by wld
		bool bNeedCheck = m_bEnjoinChiHu[i];
		if (!bNeedCheck)
		{
			bool bCanHu = false;
			if(EstimatKind == EstimatKind_GangCard && m_bQiangGangHu == 1 && 
				(m_bGameTypeIndex == 3 || m_bGameTypeIndex == 6 || m_bGameTypeIndex == 7 || m_bGameTypeIndex == 8)) // ���ݻλκͻ��ݺ��и� ���ܺ��ж�
			{
				bCanHu = true;
			}
			else if(EstimatKind == EstimatKind_OutCard )
			{
				bCanHu = true; 
				if (m_bGameTypeQing == 1)
				{
					if (m_bOpenMouth[i] == 0)
					{
						bCanHu = false;
					}
					else if (m_bGameTypeIndex == 4 && m_cbCardIndex[i][m_nHunIndex] > 1)
					{
						bCanHu = false;
					}
				}
			}
			if (bCanHu)
			{
				DWORD dwAction = AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr,i);
				if (m_bGameTypeIndex == 7 && (dwAction & WIK_CHI_HU))
				{
					if (m_bGangStatus && m_bQiangGangHu && (chr&CHR_DA_HU).IsEmpty() && m_cbCardIndex[i][m_iBaoIndex] > 0)
						dwAction -= WIK_CHI_HU;
					else
					{
						m_ChiHuRightHM[i].SetEmpty();
						m_ChiHuRightHM[i] = chr;
					}
				}
				if(m_nUsefulQingFengJiang[i] != 0)
				{
					dwAction = WIK_NULL;
					if( (!(chr&CHR_QING_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[i] == WIK_QING_BAO)
					{
						dwAction = WIK_CHI_HU;
						m_cbUserAction[i]|=dwAction;
						
					}
					if( (!(chr&CHR_FENG_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[i] == WIK_FENG_BAO)
					{
						dwAction = WIK_CHI_HU;
						m_cbUserAction[i]|=dwAction;
					}
					if( (!(chr&CHR_JIANG_YI_SE).IsEmpty()) && m_nUsefulQingFengJiang[i] == WIK_JIANG_BAO)
					{
						dwAction = WIK_CHI_HU;
						m_cbUserAction[i]|=dwAction;
					}
				}
				else
				{
					if(!(chr&CHR_DA_HU).IsEmpty() || m_bGameTypeIndex == 7)
					{
						m_cbUserAction[i]|=dwAction;
					}
					else
					{
						if ( m_bGameTypeIndex == 3 || m_bOpenMouth[i] == 1)
						{
							m_cbUserAction[i]|=dwAction;
						}
					}
				}	
		
				if(dwAction != WIK_NULL && (m_cbUserAction[i] & WIK_CHI_HU) && EstimatKind == EstimatKind_GangCard)
				{
					chr |= CHR_QIANG_GANG;
					m_cbQiangGangHu[i] = 1;
				}
				int  iBaseScore = 0;
				if(m_cbUserAction[i] & WIK_CHI_HU)
				{
					if(!(chr & CHR_DA_HU).IsEmpty())
					{
						m_cbHuType[wCenterUser] = 2;
					}
					else
						m_cbHuType[wCenterUser] = 1;

					if (m_bGameTypeIndex == 6) // ���ݺ��и�
					{
						m_ChiHuRightTemp[i].SetEmpty();
						m_ChiHuRightTemp[i] = chr;
						int iScoreTemp = AccountScoreHZHZG(i, true);
						if (iScoreTemp == -1 || (iScoreTemp <= m_iHuScore[i] && m_cbHuType[i] == m_cbHuType[wCenterUser]) || 
							(iScoreTemp < m_iHuScore[i] && m_cbHuType[i] != 0 && m_cbHuType[i] < m_cbHuType[wCenterUser]))
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
					else if (m_bGameTypeIndex == 8) // Ӣɽ
					{
						m_ChiHuRightTemp[i].SetEmpty();
						m_ChiHuRightTemp[i] = chr;
						if (AccountScoreYS(i, true) == -1)
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
					else if (m_bGameTypeIndex == 9) // ���
					{
						m_ChiHuRightTemp[i].SetEmpty();
						m_ChiHuRightTemp[i] = chr;
						if (AccountScoreMC(i, true) == -1)
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
					else if (m_bGameTypeIndex == 4)
					{
						m_ChiHuRightTemp[i].SetEmpty();
						m_ChiHuRightTemp[i] = chr;
						int iFanShuTemp = XiShuiAccountScore(m_cbCardIndex[i], i, false, true);
						if (iFanShuTemp == -1 || (iFanShuTemp <= m_iHuScore[i] && m_cbHuType[i] == m_cbHuType[wCenterUser]) || 
							(iFanShuTemp < m_iHuScore[i] && m_cbHuType[i] != 0 && m_cbHuType[i] < m_cbHuType[wCenterUser]))
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
					else if (m_bGameTypeIndex == 3)
					{
						AccountGameScore(i,false,chr,iBaseScore,true);
						if (iBaseScore < m_iHuScore[i])
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
						if(iBaseScore == m_iHuScore[i] &&( m_cbHuType[i] != 1  || (chr&CHR_DA_HU).IsEmpty()))
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
					else if (m_bGameTypeIndex == 1 || m_bGameTypeIndex == 2)
					{
						ProcessChiHuUser(i, true, iBaseScore, false,chr);
						if(iBaseScore < m_iHuScore[i] )
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
						if(iBaseScore == m_iHuScore[i] &&( m_cbHuType[i] != 1  || (chr&CHR_DA_HU).IsEmpty()))
						{
							m_cbUserAction[i] -= WIK_CHI_HU;
						}
					}
				}
				if(m_cbUserAction[i]& WIK_CHI_HU)
				{
					m_ChiHuRightTemp[i].SetEmpty();
					m_ChiHuRightTemp[i]  = chr;
				}
			}
		}		
		
		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
	}
	
	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

//�������
void CTableFrameSink::ProcessChiHuUser( WORD wChairId, bool bJustScore, int &m_iGetScore, bool bGiveUp, CChiHuRight &chiHuPlayer)
{
	if( !bGiveUp )
	{
		//����Ȩλ
		CChiHuRight &chr = chiHuPlayer;

		int nFanNum = 0;
		int nBaseScore = 0;
		int nBaoPlayer = -1;
		ClearChiHuData();
		ChiHu.resizeData();
		ZeroMemory( m_cbGenCount, sizeof(m_cbGenCount) );
		ZeroMemory( m_cbPiGenCount, sizeof(m_cbPiGenCount) );
		ZeroMemory( m_cbLaiGenCount, sizeof(m_cbLaiGenCount) );
		ZeroMemory( m_nFanShu, sizeof(m_nFanShu) );
		ZeroMemory( m_lOtherGangCount, sizeof(m_lOtherGangCount) );
		ZeroMemory( m_lSelfGangCount, sizeof(m_lSelfGangCount) );
		m_bNeedCalGang = true;
		ChiHu.cbHuScoreOther = 0;

		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory( cbCardIndexTemp, m_cbCardIndex[wChairId], sizeof(cbCardIndexTemp) );

		nFanNum = FiltrateRightNew( wChairId, chr, nBaseScore, nFanNum);
		
		BYTE cbProvideIndex = m_GameLogic.SwitchToCardIndex(m_cbProvideCard);
		for( WORD j = 0; j < m_cbPlayerNum; j++ )
		{
			CopyMemory( cbCardIndexTemp,m_cbCardIndex[j],sizeof(cbCardIndexTemp) );
			for( BYTE i = 0; i < m_cbWeaveItemCount[j]; i++ )
			{
				if( m_WeaveItemArray[j][i].cbWeaveKind == WIK_GANG ) 
				{
					if( m_WeaveItemArray[j][i].cbPublicCard == 1 ) 
					{
						m_cbGenCount[j]++;
						if(m_WeaveItemArray[j][i].cbCenterCard == m_cbProvideCard)
						{
							if(m_WeaveItemArray[j][i].wProvideUser != wChairId)
							{
								continue;
							}
						}
						m_lOtherGangCount[j] ++;
					}
					else
					{
						m_cbGenCount[j] += 2;
						m_lSelfGangCount[j] ++;
					}
				}
				if( m_WeaveItemArray[j][i].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[j][i].cbWeaveKind == WIK_GANG_LAIPI) 
				{
					m_cbPiGenCount[j] += 1;
				}
				if( m_WeaveItemArray[j][i].cbWeaveKind == WIK_GANG_HUN) 
				{
					m_cbLaiGenCount[j] += 2;
				}
				if (m_WeaveItemArray[j][i].cbWeaveKind >= WIK_QING_BAO)
				{			
					if (wChairId != j) continue;
					if (wChairId != m_wProvideUser && m_nUsefulOper[wChairId] == 2)
					{						
						if( !(chr&CHR_QING_YI_SE).IsEmpty() )
						{
							if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
							{
								nBaoPlayer = m_wProvideUser;
							}
						}
						if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
						{
							if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
							{
								nBaoPlayer = m_wProvideUser;
							}
						}
						if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
						{
							if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
							{
								nBaoPlayer = m_wProvideUser;
							}
						}												
					}	
					else
					{
						if(m_nUsefulOper[wChairId] >= 3 && wChairId != m_WeaveItemArray[j][i].wProvideUser)
						{
							if( !(chr&CHR_QING_YI_SE).IsEmpty() )
							{
								if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
								{
									nBaoPlayer = m_WeaveItemArray[j][i].wProvideUser;
								}
							}
							if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
							{
								if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
								{
									nBaoPlayer = m_WeaveItemArray[j][i].wProvideUser;
								}
							}
							if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
							{
								if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
								{
									nBaoPlayer = m_WeaveItemArray[j][i].wProvideUser;
								}
							}						
						}							
					}	
				}		
			}				
		}
		
		bool bDahu = false;
		if( !(chr&CHR_DA_HU).IsEmpty() )
		{
			bDahu = true;
		}
		getHuScore(&ChiHu.lHuPaiScore,nBaseScore,nFanNum,wChairId,m_wProvideUser,bDahu, nBaoPlayer, m_iGetScore);
		bool bAllfifty = true;
		if(m_cbPlayerNum == 2)
		{
			bAllfifty = false;
		}
		for (int i = 0;i < m_cbPlayerNum && m_cbPlayerNum > 2 ;i ++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (ChiHu.lHuPaiScore[i] != -m_bGameMaxScore)
			{
				bAllfifty = false;
			}
			if (ChiHu.lHuPaiScore[i] == -m_bGameMaxScore * (m_cbPlayerNum - 1))
			{
				bAllfifty = true;
				break;
			}
		}
		if (bAllfifty)
		{
			if (nBaoPlayer != -1)
			{
				ChiHu.lHuPaiScore[wChairId] = m_bGameMaxScoreGold * (m_cbPlayerNum - 1);
				ChiHu.lHuPaiScore[nBaoPlayer] = -m_bGameMaxScoreGold * (m_cbPlayerNum - 1);
				ChiHu.lHuPaiScore[nBaoPlayer] |= CHR_BAO_HU;
			}
			else
			{
				for (int i = 0; i < GAME_PLAYER; i++)
				{
					if (i == wChairId)
					{
						ChiHu.lHuPaiScore[i] = m_bGameMaxScoreGold * (m_cbPlayerNum - 1);
					}
					else
					{
						ChiHu.lHuPaiScore[i] = -m_bGameMaxScoreGold;
					}				
				}
			}
		}

		if(bJustScore )
		{
			return  ;
		}

		SCORE lScoreTemp[GAME_PLAYER]={0};	

		//���ñ���
		m_wProvider[wChairId] = m_wProvideUser;
		m_wWinOrder[m_cbWinCount++] = wChairId;
		m_bGangStatus = false;
		m_bGangHuaStatus = false;
		m_bGangOutStatus = false;

		//������Ϣ		
		ChiHu.wChiHuUser = wChairId;
		ChiHu.wProviderUser = m_wProvideUser;
		ChiHu.cbChiHuCard = m_cbProvideCard;

		DWORD wChiHuRight[1]={0};
		m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0],MAX_RIGHT_COUNT );
		ChiHu.dwChiHuRight = wChiHuRight[0];
		ChiHu.cbGenCount = m_cbGenCount[wChairId];
		ChiHu.dwChiHuRight = wChiHuRight[0];
		ChiHu.cbGenCount = m_cbGenCount[wChairId];
		if (nBaoPlayer != -1)
		{
			ChiHu.wBaoPlayer = nBaoPlayer;
		}
		else
			ChiHu.wBaoPlayer = 10;
		for (int i=0;i<ChiHu.lMenHun.size();i++)
		{
			ChiHu.lMenHun[i]=m_bOpenMouth[i];
		}	
		for (int i=0;i<ChiHu.lPiao.size();i++)
		{
			ChiHu.lPiao[i]=m_cbPiGenCount[i];
		}	
		for (int i=0;i<ChiHu.lDang.size();i++)
		{
			ChiHu.lDang[i]=m_cbLaiGenCount[i];
		}	
		for (int i=0;i<ChiHu.lGang.size();i++)
		{
			ChiHu.lGang[i]=m_cbGenCount[i];
		}
		ChiHu.cbHuScore = (BYTE)nBaseScore;
		
		for (int i=0;i<ChiHu.lMingGang.size();i++)
		{
			ChiHu.lMingGang[i]=m_lOtherGangCount[i];
		}	
		for (int i=0;i<ChiHu.lAnGang.size();i++)
		{
			ChiHu.lAnGang[i]=m_lSelfGangCount[i];
		}		
		for (int i=0;i<ChiHu.lXiGang.size();i++)
		{
			ChiHu.lXiGang[i]=m_nFanShu[i];
		}	
			
// 		if ( hasRule(GAME_RULE_DAODAO_HU))
// 		{
// 			ChiHu.cbGenCount  = 0;
// 		}

		datastream kDataStream;
		ChiHu.StreamValue(kDataStream,true);

		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

		m_kChiHuInfoList.push_back(ChiHu);
		addGameRecordAction(ChiHu);
		//���ñ���

		m_bHasHuPai[wChairId] = true;
		{
			m_bPlayStatus[wChairId] = false;
		}
	}

	return;
}

//���ݻλλ��ֽ���
void  CTableFrameSink::AccountGameScore(WORD wChairId, bool bGiveUp, CChiHuRight &chiHuPlayer, int &m_iGetScore, bool bJustScore)
{
	if( !bGiveUp )
	{
		//����Ȩλ
		CChiHuRight &chr = chiHuPlayer;

		int nFanNum = 0;
		int nBaseScore = 0;
		int nPlayerScore = 0;
		int nBaoPlayer = -1;
		ClearChiHuData();
		ChiHu.resizeData();

		ZeroMemory( m_cbGenCount,sizeof(m_cbGenCount) );
		ZeroMemory( m_nFanShu,sizeof(m_nFanShu) );	
		ZeroMemory( m_lOtherGangCount,sizeof(m_lOtherGangCount));
		ZeroMemory( m_lSelfGangCount,sizeof(m_lSelfGangCount));

		m_bNeedCalGang = true;

		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory( cbCardIndexTemp, m_cbCardIndex[wChairId], sizeof(cbCardIndexTemp));

		StatisticalFraction( wChairId, chr, nBaseScore, nFanNum, nPlayerScore);

		BYTE cbProvideIndex =  m_GameLogic.SwitchToCardIndex(m_cbProvideCard);
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			CopyMemory(cbCardIndexTemp, m_cbCardIndex[j], sizeof(cbCardIndexTemp));
			for( BYTE i = 0; i < m_cbWeaveItemCount[j]; i++ )
			{
				if(m_WeaveItemArray[j][i].cbWeaveKind == WIK_GANG)
				{
					m_cbGenCount[j]++;
					if( m_WeaveItemArray[j][i].cbPublicCard == 1)
					{
						if(m_WeaveItemArray[j][i].cbCenterCard == m_cbProvideCard)
						{
							if(m_WeaveItemArray[j][i].wProvideUser != wChairId)
							{
								continue;
							}
						}
						m_lOtherGangCount[j]++;
					}
					else
					{
						m_lSelfGangCount[j]++;
					}
				}
			}
		}
		if(!(chr & CHR_QIANG_GANG).IsEmpty())
		{
			nBaoPlayer = m_wProvideUser;
			if (m_cbGenCount[m_wProvideUser] >= 1)
			{
				m_cbGenCount[m_wProvideUser]--;
			}
		}
		// �������ܰ���
		getHuScore(&ChiHu.lHuPaiScore, nBaseScore, nPlayerScore, nFanNum, wChairId, m_wProvideUser, nBaoPlayer, m_iGetScore);

		if(bJustScore)
		{
			return ;
		}

		//���ñ���
		m_wProvider[wChairId] = m_wProvideUser;
		m_wWinOrder[m_cbWinCount++] = wChairId;
		m_bGangStatus = false;
		m_bGangHuaStatus = false;
		m_bGangOutStatus = false;

		//������Ϣ		
		ChiHu.wChiHuUser = wChairId;
		ChiHu.wProviderUser = m_wProvideUser;
		ChiHu.cbChiHuCard = m_cbProvideCard;

		DWORD wChiHuRight[1]={0};
		m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0],MAX_RIGHT_COUNT );
		ChiHu.dwChiHuRight = wChiHuRight[0];
		ChiHu.cbGenCount = m_cbGenCount[wChairId];
		if (nBaoPlayer != -1)
		{
			ChiHu.wBaoPlayer = nBaoPlayer;
		}
		else
			ChiHu.wBaoPlayer = 10;
		for (int i=0;i<ChiHu.lGang.size();i++)
		{
			ChiHu.lGang[i]=m_cbGenCount[i];
		}
		ChiHu.cbHuScore = (BYTE)nBaseScore;
		if (nPlayerScore == 0)
		{
			ChiHu.cbHuScoreOther = (BYTE)nBaseScore;
		}
		else
			ChiHu.cbHuScoreOther = (BYTE)nPlayerScore;
		
		for (int i=0;i<ChiHu.lMingGang.size();i++)
		{
			ChiHu.lMingGang[i]=m_lOtherGangCount[i];
		}	
		for (int i=0;i<ChiHu.lAnGang.size();i++)
		{
			ChiHu.lAnGang[i]=m_lSelfGangCount[i];
		}			

		datastream kDataStream;
		ChiHu.StreamValue(kDataStream,true);
	

		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

		m_kChiHuInfoList.push_back(ChiHu);
		addGameRecordAction(ChiHu);
		//���ñ���

		m_bHasHuPai[wChairId] = true;
		{
			m_bPlayStatus[wChairId] = false;
		}
	}

	return;
}

//���ݻλ�ͳ�Ʒ���
int CTableFrameSink::StatisticalFraction(WORD wChairId, CChiHuRight &chr, int &nBaseScore, int &nBaseFan, int &nPlayerScore, DWORD bAction)
{
	nBaseFan = 0;
	nBaseScore = 0;
	nPlayerScore = 0;
	// ͳ��
	bool bMenQing = true;
	if (bAction == WIK_NULL)
	{
		for(int i = 0 ; i < m_cbWeaveItemCount[wChairId]; i++)
		{
			if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
			{
				if (m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
				{
					bMenQing = false;
					break;
				}
			}
			else if(m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG)
			{
				bMenQing = false; 
				break;
			}
		}
	}
	if (bMenQing == true)
	{
		chr |= CHR_MEN_QING;
	}
	//���ϻ�
	if (m_wCurrentUser == wChairId && m_bGangStatus)
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
	}
	else if (m_wProvideUser == wChairId)  // ���� 
	{
		chr |= CHR_ZI_MO;
	}
	else if (m_wProvideUser != wChairId)  // ����
	{
		chr |= CHR_FANG_PAO;
	}
	if(m_bGameMaxScore == 20)  //���׷�20��
	{
		// �ܿ�
		if (!(chr & CHR_GANG_KAI).IsEmpty())
		{
			if ((chr & CHR_DA_HU).IsEmpty() && (chr & CHR_NONE).IsEmpty() && (chr & CHR_MEN_QING).IsEmpty())
			{
				nBaseScore = 8;
			}
			else
				nBaseScore = 10;
			return nBaseFan;
		}
		if (!(chr & CHR_QIANG_GANG).IsEmpty()) // ���ܺ�
		{
			if (!(chr & CHR_NONE).IsEmpty()) // Ӳ��
			{
				nBaseScore = 20;
			}
			else
				nBaseScore = 16;
			return nBaseFan;
		}
		// ���
		if (!(chr & CHR_DA_HU).IsEmpty())
		{
			if ((!(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty()) && (chr & CHR_NONE).IsEmpty()) // ����7��
			{
				if (!(chr & CHR_ZI_MO).IsEmpty()) // ����
				{
					nBaseScore = 20;
				}
				else // ����
				{
					nBaseScore = 20;
					nPlayerScore = 16;
				}
			}
			else if (!(chr & CHR_NONE).IsEmpty() && !(chr & CHR_QI_DUI).IsEmpty())
			{
				if (!(chr & CHR_ZI_MO).IsEmpty()) // ����
				{
					nBaseScore = 20;
				}
				else // ����
				{
					nBaseScore = 20;
					nPlayerScore = 16;
				}
			}
			else if ((chr & CHR_NONE).IsEmpty()) // ���
			{
				if (!(chr & CHR_QI_DUI).IsEmpty()) // 7��
				{
					if (!(chr & CHR_ZI_MO).IsEmpty()) // ����
					{
						nBaseScore = 16;
					}
					else // ����
					{
						nBaseScore = 16;
						nPlayerScore = 8;
					}
				}
				else if (!(chr & CHR_PENGPENGHU).IsEmpty() || !(chr & CHR_QING_YI_SE).IsEmpty()) // ����������һɫ
				{
					if (!(chr & CHR_ZI_MO).IsEmpty()) // ����
					{
						nBaseScore = 16;
					}
					else // ����
					{
						nBaseScore = 16;
						nPlayerScore = 8;
					}
				}
			}
			else
				nBaseScore = 20;
		}
		else // С��
		{
			if (!(chr & CHR_NONE).IsEmpty() && !(chr & CHR_ZI_MO).IsEmpty())  // Ӳ����
			{
				if (!(chr & CHR_MEN_QING).IsEmpty())
				{
					nBaseScore = 16;
				}
				else
					nBaseScore = 8;
			}
			else  if (!(chr & CHR_NONE).IsEmpty() && (chr & CHR_ZI_MO).IsEmpty()) // Ӳ���� 
			{
				nBaseScore = 8;
				nPlayerScore = 4;
			}
			else if((chr & CHR_NONE).IsEmpty() && !(chr & CHR_ZI_MO).IsEmpty()) // ������ 
			{
				if (!(chr & CHR_MEN_QING).IsEmpty())
				{
					nBaseScore = 8;
				}
				else
					nBaseScore = 4;
			}
			else  // �����
			{
				nBaseScore = 4;
				nPlayerScore = 2;
			}	
		}
	}
	else if (m_bGameMaxScore == 30)  // �ⶥ30��
	{
		if (!(chr & CHR_GANG_KAI).IsEmpty()) // �ܿ�
		{
			if ((chr & CHR_NONE).IsEmpty() && (chr & CHR_DA_HU).IsEmpty() && (chr & CHR_MEN_QING).IsEmpty())
			{
				nBaseScore = 10;
			}
			else
				nBaseScore = 15;
			return nBaseFan;
		}
		if (!(chr & CHR_QIANG_GANG).IsEmpty()) // ���ܺ�
		{
			if (!(chr & CHR_NONE).IsEmpty()) // Ӳ��
			{
				nBaseScore = 30;
			}
			else
				nBaseScore = 20;
			return nBaseFan;
		}
		else if (!(chr & CHR_DA_HU).IsEmpty())
		{
			if (!(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty())
			{
				if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 30;
					nPlayerScore = 20;
				}
				else
					nBaseScore = 30;
			}
			else if (!(chr & CHR_QI_DUI).IsEmpty())
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 30;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 30;
					nPlayerScore = 20;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
					nPlayerScore = 10;
				}
			}
			else if(!(chr & CHR_PENGPENGHU).IsEmpty() || !(chr & CHR_QING_YI_SE).IsEmpty())
			{
				if(!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 30;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
					nPlayerScore = 10;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 30;
					nPlayerScore = 20;
				}
			}
		}
		else
		{
			if (!(chr & CHR_MEN_QING).IsEmpty()) //����
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
					nPlayerScore = 5;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 5;
					nPlayerScore = 5;
				}	
			}
			else 
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 5;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
					nPlayerScore = 5;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 5;
					nPlayerScore = 5;
				}
			}
		}
	}
	else if (m_bGameMaxScore == 50 || m_bGameMaxScore == 100)  // �ⶥ50��
	{
		if (!(chr & CHR_QIANG_GANG).IsEmpty()) // ���ܺ�
		{
			if (!(chr & CHR_NONE).IsEmpty()) // Ӳ��
			{
				nBaseScore = 50;
			}
			else
				nBaseScore = 40;
			return nBaseFan;
		}
		if (!(chr & CHR_DA_HU).IsEmpty()) // ���
		{
			if (!(chr & CHR_GANG_KAI).IsEmpty()) // �ܿ�
			{
				nBaseScore = 25;
			}
			else if (!(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty())
			{
				if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 50;
					nPlayerScore = 40;
				}
				else
					nBaseScore = 50;
			}
			else if (!(chr & CHR_QI_DUI).IsEmpty())
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 50;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 40;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 50;
					nPlayerScore = 40;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 40;
					nPlayerScore = 20;
				}
			}
			else if(!(chr & CHR_PENGPENGHU).IsEmpty() || !(chr & CHR_QING_YI_SE).IsEmpty())
			{
				if(!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 40;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 50;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 40;
					nPlayerScore = 20;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 50;
					nPlayerScore = 40;
				}
			}
		}
		else // С��
		{
			if (!(chr & CHR_GANG_KAI).IsEmpty()) // �ܿ�
			{
				if (!(chr & CHR_NONE).IsEmpty() || !(chr & CHR_MEN_QING).IsEmpty())
				{
					nBaseScore = 25;
				}
				else
					nBaseScore = 20;
				return nBaseFan;
			}
			if (!(chr & CHR_MEN_QING).IsEmpty()) //����
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 40;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
					nPlayerScore = 10;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
					nPlayerScore = 5;
				}	
			}
			else 
			{
				if (!(chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
				}
				else if (!(chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && !(chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 20;
					nPlayerScore = 10;
				}
				else if ((chr & CHR_ZI_MO).IsEmpty() && (chr & CHR_NONE).IsEmpty())
				{
					nBaseScore = 10;
					nPlayerScore = 5;
				}
			}
		}
		if (m_bGameMaxScore == 100)
		{
			nBaseScore *= 2;
			nPlayerScore *= 2;
		}
	}
	return nBaseFan;
}

// �ˮ���
int CTableFrameSink::XiShuiAccountScore(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, bool bGiveUp, bool bCheckScore)
{
	if( !bGiveUp )
	{
		//����Ȩλ
		CChiHuRight &chr = m_ChiHuRight[wChairId];
		if (bCheckScore)
		{
			chr = m_ChiHuRightTemp[wChairId];
		}
		BYTE nBaseScore = 1;
		BYTE nPlayerScore = 0;

		int nHuScore[GAME_PLAYER] = {0}; // ����
		int MaxScoreNum = 0;
		ClearChiHuData();
		ChiHu.resizeData();

		int nBaoPlayer = -1;
		int nPlayerFanShu[GAME_PLAYER] = {0};

		XiShuiAccountFanShu(cbCardIndex, wChairId, chr, nPlayerFanShu, nBaoPlayer); // ���㷬��

		bool bDaHu = false;
		if(!(chr & CHR_DA_HU).IsEmpty())
		{
			bDaHu = true;
			if (m_wProvideUser == wChairId) // ����
			{
				if (m_bGameMaxScore == 60) // ����20180929
				{
					nBaseScore = 8;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 4;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 4;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 4;
					}
				}
				else if (m_bGameMaxScore == 120)
				{
					nBaseScore = 16;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 8;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 8;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 8;
					}
				}
				else if (m_bGameTypeQing == 2) //������ �׷�
				{
					nBaseScore = 4;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 2;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 2;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 2;
					}
				}
				else if (m_bGameTypeQing == 1) // һ񮵽�� �׷�
				{
					nBaseScore = 4;
				}
			}
			else //����
			{
				if (m_bGameMaxScore == 60) // ����20180929
				{
					nBaseScore = 8;
					nPlayerScore = 4;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 4;
						nPlayerScore = 2;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 4;
						nPlayerScore = 2;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 4;
						nPlayerScore = 2;
					}
				}
				else if (m_bGameMaxScore == 120)
				{
					nBaseScore = 16;
					nPlayerScore = 8;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 8;
						nPlayerScore = 4;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 8;
						nPlayerScore = 4;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 8;
						nPlayerScore = 4;
					}
				}
				if (m_bGameTypeQing == 2) //������ �׷�
				{
					nBaseScore = 4;
					nPlayerScore = 2;
					if (!(chr & CHR_QI_DUI_3).IsEmpty())
					{
						nBaseScore = 2;
						nPlayerScore = 1;
						nPlayerFanShu[wChairId] += 2;
					}
					else if (!(chr & CHR_QI_DUI_2).IsEmpty())
					{
						nBaseScore = 2;
						nPlayerScore = 1;
						nPlayerFanShu[wChairId]++;
					}
					else if (!(chr & CHR_QI_DUI).IsEmpty())
					{
						nBaseScore = 2;
						nPlayerScore = 1;
					}
				}
				else if (m_bGameTypeQing == 1)// һ񮵽��
				{
					nBaseScore = 4;
					nPlayerScore = 2;
				}
			}
		}
		if (m_wProvideUser == wChairId) // ����
		{
			BYTE bTemp1 = 0;
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if(!m_bPlayStatus[i])
				{
					continue;
				}
				if(i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
				if (m_bGameMaxScore <= 30)
				{
					if (m_bGameTypeQing == 2) // ������
					{
						if (m_bGameMaxScore == 20 && nHuScore[i] < 4)
						{
							bTemp1++;
							nHuScore[i] = 0;
						}
						else if (m_bGameMaxScore == 30 && nHuScore[i] < 8)
						{
							bTemp1++;
							nHuScore[i] = 0;
						}
					}
					else// һ񮵽��
					{
						if (nHuScore[i] < 4)
						{
							bTemp1++;
							nHuScore[i] = 0;
						}
					}
				}
				else
				{
					if (m_bGameMaxScore == 60 && nHuScore[i] < 16)
					{
						bTemp1++;
						nHuScore[i] = 0;
					}
					else if (m_bGameMaxScore == 120 && nHuScore[i] < 32)
					{
						bTemp1++;
						nHuScore[i] = 0;
					}
				}
			}
			if (bTemp1 >= 3)
			{
				return -1;
			}
		}
		else if (m_wProvideUser != wChairId) // ����
		{
			BYTE iBigOrSmallHu = 0;
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if(!m_bPlayStatus[i])
				{
					continue;
				}
				if(i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				if (i == m_wProvideUser)
				{
					nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
					if ((chr & CHR_DA_HU).IsEmpty()) // С��
					{
						if (m_bGameMaxScore <= 30)
						{
							if (m_bGameTypeQing == 2) // ������
							{
								if (m_bGameMaxScore == 20 && nHuScore[i] < 4)
								{
									return -1;
								}
								else if (m_bGameMaxScore == 30 && nHuScore[i] < 8)
								{
									return -1;
								}
							}
							else // һ񮵽��
							{
								if (nHuScore[i] < 4)
								{
									return -1;
								}
							}
						}
						else
						{
							if (m_bGameMaxScore == 60 && nHuScore[i] < 16) // �¼ӹ���21080929
							{
								return -1;
							}
							else if (m_bGameMaxScore == 120 && nHuScore[i] < 32)
							{
								return -1;
							}
						}
					}
					else // ���
					{
						iBigOrSmallHu = 1;
					}
				}
				else
				{
					nHuScore[i] = nPlayerScore * (pow(2, m_nFanShu[i]));
					if (m_bGameMaxScore <= 30)
					{
						if (m_bGameTypeQing == 2) // ������
						{
							if (m_bGameMaxScore == 20 && nHuScore[i] < 4)
							{
								nHuScore[i] = 0;
							}
							else if (m_bGameMaxScore == 30 && nHuScore[i] < 8)
							{
								nHuScore[i] = 0;
							}
						}
						else // һ񮵽��
						{
							if (nHuScore[i] < 4)
							{
								nHuScore[i] = 0;
							}
						}
					}
					else
					{
						if (m_bGameMaxScore == 60 && nHuScore[i] < 16) // �¼ӹ���21080929
						{
							nHuScore[i] = 0;
						}
						else if (m_bGameMaxScore == 120 && nHuScore[i] < 32)
						{
							nHuScore[i] = 0;
						}
					}
				}
			}
			if (iBigOrSmallHu == 1)
			{
				BYTE iFlag = 0;
				if (m_bGameMaxScore <= 30)
				{
					if (m_bGameTypeQing == 2 && m_bGameMaxScore == 30)
					{
					for (BYTE j = 0; j < m_cbPlayerNum; j++)
						{
							if(j == wChairId)
							{
								continue;
							}
							if (nHuScore[j] < 8)
							{
								nHuScore[j] = 0;
								iFlag++;
							}
						}
						if (iFlag >= 3)
						{
							return -1;
						}
					}
					else
					{
					for (BYTE j = 0; j < m_cbPlayerNum; j++)
						{
							if(j == wChairId)
							{
								continue;
							}
							if (nHuScore[j] < 4)
							{
								nHuScore[j] = 0;
								iFlag++;
							}
						}
						if (iFlag >= 3)
						{
							return -1;
						}
					}
				}
				else
				{
					for (BYTE j = 0; j < m_cbPlayerNum; j++)
					{
						if(j == wChairId)
						{
							continue;
						}
						if (m_bGameMaxScore == 60 && nHuScore[j] < 16)
						{
							nHuScore[j] = 0;
							iFlag++;
						}
						else if (m_bGameMaxScore == 120 && nHuScore[j] < 32)
						{
							nHuScore[j] = 0;
							iFlag++;
						}
					}
					if (iFlag >= 3)
					{
						return -1;
					}	
				}
			}
		}
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if(m_bGameMaxScore == 20) // �ⶥ20
			{
				if (nHuScore[i] == 0) nHuScore[i] = 0;
				else if(nHuScore[i] <= 5) nHuScore[i] = 5;
				else if(nHuScore[i] <= 10) nHuScore[i] = 10;
				else if(nHuScore[i] <= 15) nHuScore[i] = 15;
				else if(nHuScore[i] > 15)
				{
					if (nHuScore[i] == 16)
					{
						nHuScore[i] = 15;
					}
					else
						nHuScore[i] = 20;
				}
				if(nHuScore[i] >= 20) MaxScoreNum++;
			}
			else //if (m_bGameMaxScore == 30)// �ⶥ30
			{
				//if (m_bGameTypeQing == 1) // һ񮵽��
				{
					if(nHuScore[i] == 0)		  nHuScore[i] = 0;
					else if (nHuScore[i] <= 5)	  nHuScore[i] = 5;
					else if (nHuScore[i] == 8)	  nHuScore[i] = 10;
					else if (nHuScore[i] == 16)
					{
						if (m_bGameTypeQing == 1 && m_bGameMaxScore <= 30)
						{
							nHuScore[i] = 15;
						}
						else nHuScore[i] = 20;
					}
					else if (nHuScore[i] == 32)	 nHuScore[i] = 40;
					else if (nHuScore[i] == 64)	 nHuScore[i] = 80;
					else if (nHuScore[i] >= 128) nHuScore[i] = 128;
					if( nHuScore[i] >= m_bGameMaxScore ) 
					{
						nHuScore[i] = m_bGameMaxScore;
						MaxScoreNum++;
					}
				}
				//else // ������
				//{
				//	if(nHuScore[i] == 0) nHuScore[i] = 0;
				//	else if( nHuScore[i] <= 10) nHuScore[i] = 10;
				//	else if( nHuScore[i] <= 15) nHuScore[i] = 15;
				//	else if( nHuScore[i] <= 20) nHuScore[i] = 20;
				//	else if( nHuScore[i] <= 25) nHuScore[i] = 25;
				//	else if( nHuScore[i] > 25 ) nHuScore[i] = 30;
				//	if( nHuScore[i] >= 30 ) MaxScoreNum++;
				//}
			}
		}
		if (m_bGameMaxScore == 20 && ((MaxScoreNum == 3 && m_cbPlayerNum == 4)||(MaxScoreNum == 2 && m_cbPlayerNum == 3)))
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 30;
			}
		}
		else
		{
			if (MaxScoreNum == 3)
			{
				for (int i = 0; i < m_cbPlayerNum; i++)
				{
					if (i == wChairId)
					{
						continue;
					}
					if (m_bGameMaxScore == 30) nHuScore[i] = 50;
					else if (m_bGameMaxScore == 60)  nHuScore[i] = 100;
					else if (m_bGameMaxScore == 120)  nHuScore[i] = 200;
				}
			}
			else if (MaxScoreNum == 2)
			{
				for (int i = 0; i < m_cbPlayerNum; i++)
				{
					if (i == wChairId)
					{
						continue;
					}
					if (m_bGameMaxScore == 30) nHuScore[i] = 40;
					else if (m_bGameMaxScore == 60)  nHuScore[i] = 80;
					else if (m_bGameMaxScore == 120)  nHuScore[i] = 160;
				}
			}	
		}

		if (nBaoPlayer != -1)
		{
			for(int i = 0; i < m_cbPlayerNum; i++)
			{
				if(i != wChairId)
				{
					ChiHu.lHuPaiScore[wChairId] += nHuScore[i];
					ChiHu.lHuPaiScore[nBaoPlayer] -= nHuScore[i];
				}
			}
			ChiHu.lHuPaiScore[nBaoPlayer] |= CHR_BAO_HU;
		}
		else 
		{
			for(int i = 0; i < m_cbPlayerNum; i++)
			{
				if(i != wChairId)
				{
					ChiHu.lHuPaiScore[wChairId] += nHuScore[i];
					ChiHu.lHuPaiScore[i] -= nHuScore[i];
				}
			}
		}
		if (bCheckScore)
		{
			return ChiHu.lHuPaiScore[wChairId];
		}
		//���ñ���
		m_wProvider[wChairId] = m_wProvideUser;
		m_wWinOrder[m_cbWinCount++] = wChairId;
		m_bGangStatus = false;
		m_bGangHuaStatus = false;
		m_bGangOutStatus = false;
		//������Ϣ		
		ChiHu.wChiHuUser = wChairId;
		ChiHu.wProviderUser = m_wProvideUser;
		ChiHu.cbChiHuCard = m_cbProvideCard;

		DWORD wChiHuRight[1] = {0};
		m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0], MAX_RIGHT_COUNT);
		ChiHu.dwChiHuRight = wChiHuRight[0];
		ChiHu.cbGenCount = m_cbGenCount[wChairId];
		if (nBaoPlayer != -1)
		{
			ChiHu.wBaoPlayer = nBaoPlayer;
		}
		else
			ChiHu.wBaoPlayer = 10;
		for (int i=0;i<ChiHu.lMenHun.size();i++)
		{
			ChiHu.lMenHun[i]=m_bOpenMouth[i];
		}	
		for (int i=0;i<ChiHu.lPiao.size();i++)
		{
			ChiHu.lPiao[i]=m_cbPiGenCount[i];
		}	
		for (int i=0;i<ChiHu.lDang.size();i++)
		{
			ChiHu.lDang[i]=m_cbLaiGenCount[i];
		}	
		for (int i=0;i<ChiHu.lGang.size();i++)
		{
			ChiHu.lGang[i]=m_cbGenCount[i];
		}
		ChiHu.cbHuScore = nBaseScore;
		ChiHu.cbHuScoreOther = nPlayerScore;
		for (int i=0;i<ChiHu.lMingGang.size();i++)
		{
			ChiHu.lMingGang[i]=m_lOtherGangCount[i];
		}	
		for (int i=0;i<ChiHu.lAnGang.size();i++)
		{
			ChiHu.lAnGang[i]=m_lSelfGangCount[i];
		}		
		for (int i=0;i<ChiHu.lXiGang.size();i++)
		{
			ChiHu.lXiGang[i]=m_nFanShu[i];
		}	
		datastream kDataStream;
		ChiHu.StreamValue(kDataStream,true);

		m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
		m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

		m_kChiHuInfoList.push_back(ChiHu);
		addGameRecordAction(ChiHu);
		//���ñ���

		m_bHasHuPai[wChairId] = true;
		{
			m_bPlayStatus[wChairId] = false;
		}
	}
	return 0;
}

//�ˮ�㷬
void  CTableFrameSink::XiShuiAccountFanShu(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId, CChiHuRight &chr, int nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer)
{
	bool bMenQing = true;
	ZeroMemory(m_cbGenCount, sizeof(m_cbGenCount));
	ZeroMemory(m_cbPiGenCount, sizeof(m_cbPiGenCount));	
	ZeroMemory(m_cbLaiGenCount, sizeof(m_cbLaiGenCount));
	ZeroMemory(m_nFanShu, sizeof(m_nFanShu));
	ZeroMemory(m_lOtherGangCount, sizeof(m_lOtherGangCount));
	ZeroMemory(m_lSelfGangCount, sizeof(m_lSelfGangCount));
	m_bNeedCalGang = true;

	//������Ʒ��ķ���  ���� ����  Ƥ��  ���
	for(int i = 0; i < m_cbWeaveItemCount[wChairId]; i++)
	{
		if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)    
		{
			if(m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
			{
				m_cbGenCount[wChairId]++;
				if(m_WeaveItemArray[wChairId][i].cbCenterCard == m_cbProvideCard)
				{
					if(m_WeaveItemArray[wChairId][i].wProvideUser != wChairId)
					{
						continue;
					}
				}
				m_lOtherGangCount[wChairId]++;
			}
			else
			{
				m_cbGenCount[wChairId] += 2;
				m_lSelfGangCount[wChairId]++;
			}
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_LAIPI) 
		{
			m_cbPiGenCount[wChairId] += 1;
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_HUN) 
		{
			m_cbLaiGenCount[wChairId] += 2;
		}
		else if (m_WeaveItemArray[wChairId][i].cbWeaveKind >= WIK_QING_BAO && m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_SHOW_BAO)
		{
			if (wChairId != m_wProvideUser && m_nUsefulOper[wChairId] == 2)
			{						
				if( !(chr&CHR_QING_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if (nBaoPlayer != -1 && m_bGameTypeIndex == 4 && m_bGameTypeQing == 1) // Ϫˮ��һ������
				{
					if (m_bOpenMouth[m_wProvideUser] == 0) // ���ڷ�û����
					{
						// ʲôҲ����
					}
					else if (m_GameLogic.GetTingTag_DaoDao(m_cbCardIndex[m_wProvideUser],m_WeaveItemArray[m_wProvideUser],m_cbWeaveItemCount[m_wProvideUser],false,m_bGameTypeIndex,m_bGameTypeQing))
					{
						nBaoPlayer = -1;
					}
				}
			}
			else
			{
				if(m_nUsefulOper[wChairId] >= 3 && wChairId != m_WeaveItemArray[wChairId][i].wProvideUser)
				{
					if( !(chr&CHR_QING_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}						
				}				
			}
		}	
		else if(m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG)
		{
			bMenQing = false;
		}
	}
	if(bMenQing == true) 
	{
		chr |= CHR_MEN_QING;
	}
	//���ϻ�
	if( m_wCurrentUser==wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
		nPlayerFanShu[wChairId]++;
	}
	if (m_wProvideUser == wChairId)  // ����
	{
		chr |= CHR_ZI_MO;
	}
	else if (m_wProvideUser != wChairId)  // ����
	{
		chr |= CHR_FANG_PAO;
	}
	if(!(chr & CHR_NONE).IsEmpty())  //Ӳ�� 
	{
		nPlayerFanShu[wChairId]++;
	}
	if (!(chr & CHR_PENGPENGHU).IsEmpty())
	{
		if (!(chr & CHR_QING_YI_SE).IsEmpty())
		{
			nPlayerFanShu[wChairId]++;
		}
		else
		{
			BYTE cbCardColor=0xFF;
			BYTE cbQingYiSe = 0;
			for (BYTE i = 0; i < MAX_INDEX; i++)
			{
				if(i==m_nHunIndex) continue;
				if (cbCardIndex[i]!=0)
				{
					//��ɫ�ж�
					if (cbCardColor!=0xFF)
					{
						cbQingYiSe = 1;
						break;
					}
					//���û�ɫ
					cbCardColor=(m_GameLogic.SwitchToCardData(i)&MASK_COLOR);

					//��������
					i=(i/9+1)*9-1;
				}
			}
			//�������ֻ������
			if( cbCardColor == 0xFF )
			{
				ASSERT( m_nHunIndex != MAX_INDEX && cbCardIndex[m_nHunIndex] > 0 );
				//������
				ASSERT( m_cbWeaveItemCount[wChairId] > 0 );
				cbCardColor = m_WeaveItemArray[wChairId][0].cbCenterCard&MASK_COLOR;
			}
			//����ж�
			for (BYTE i=0;i<m_cbWeaveItemCount[wChairId];i++)
			{
				if (m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG_PI)
				{
					BYTE cbCenterCard=m_WeaveItemArray[wChairId][i].cbCenterCard;
					if ((cbCenterCard&MASK_COLOR)!=cbCardColor)
					{
						cbQingYiSe = 1;
						break;
					}
				}
			}
			if (!cbQingYiSe)
			{
				chr |= CHR_QING_YI_SE;
				nPlayerFanShu[wChairId]++;
			}
		}
	}
	if(m_nUsefulOper[wChairId] >= 4 && m_wProvideUser != wChairId && (chr & CHR_DA_HU).IsEmpty() && m_bAllOther == 1)
	{
		chr |= CHR_ALL_OTHER;
		chr |= CHR_DA_HU;
		if (m_bOpenMouth[m_wProvideUser] == 0 && m_bGameTypeQing == 1) // ���ڷ�û����
		{
			nBaoPlayer = m_wProvideUser;
		}
		else if(false == m_GameLogic.GetTingTag_DaoDao(m_cbCardIndex[m_wProvideUser],m_WeaveItemArray[m_wProvideUser],m_cbWeaveItemCount[m_wProvideUser],false,m_bGameTypeIndex,m_bGameTypeQing))
		{
			nBaoPlayer = m_wProvideUser;
		}
	}
	nPlayerFanShu[wChairId] += (m_cbGenCount[wChairId] + m_cbPiGenCount[wChairId] + m_cbLaiGenCount[wChairId]); // �����ܷ���

	//�����мҷ��� 
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if(i == wChairId) continue;
		if(m_bOpenMouth[i]) nPlayerFanShu[i]++;
		for(int j = 0; j < m_cbWeaveItemCount[i]; j++)
		{
			if(m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG) 
			{
				if( m_WeaveItemArray[i][j].cbPublicCard == 1) 
				{
					m_cbGenCount[i]++;
					if(m_WeaveItemArray[i][j].cbCenterCard == m_cbProvideCard)
					{
						if(m_WeaveItemArray[i][j].wProvideUser != wChairId)
						{
							continue;
						}
					}
					m_lOtherGangCount[i]++;
				}
				else
				{
					m_cbGenCount[i] += 2;
					m_lSelfGangCount[i] ++;
				}
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_LAIPI) 
			{
				m_cbPiGenCount[i] += 1;
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_HUN)  //�����2��
			{
				m_cbLaiGenCount[i] += 2;
			}
		}
		nPlayerFanShu[i] += (m_cbGenCount[i] + m_cbPiGenCount[i] + m_cbLaiGenCount[i]); // �м��ܷ���
	}
}

int CTableFrameSink::FiltrateRightNew( WORD wChairId, CChiHuRight &chr, int &nBaseScore, int &nBaseFan )
{
	nBaseScore = 1;
	if (m_bGameMaxScore > 40)
	{
		nBaseScore = 2;
	}
	nBaseFan = 0;
	if(m_bGameTypeQing == 2)
	{
		bool bqing = true;
		for( BYTE i = 0; i < m_cbWeaveItemCount[wChairId]; i++ )
		{
			if( m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG_PI ) 
			{
				if (m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
				{
					if (m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
					{
						bqing = false;
					}
				}
				else
				{
					bqing = false;
				}				
			}
		}
		if(bqing)
		{
			chr |= CHR_MEN_QING;
		}
	}
	//���ϻ�
	if( m_wCurrentUser == wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
		nBaseFan ++;
	}
	if (m_wProvideUser == wChairId)
	{
		chr |= CHR_ZI_MO;
	}
	else if (m_wProvideUser != wChairId)
	{
		chr |= CHR_FANG_PAO;
	}

	if( !(chr & CHR_DA_HU).IsEmpty() )
	{
		if( !(chr & CHR_DOUBLE_HUN).IsEmpty() )
		{
			nBaseFan ++;
		}
		if( !(chr & CHR_QI_DUI).IsEmpty() )
		{
			if( !(chr & CHR_ZI_MO).IsEmpty() )
			{
				nBaseScore = nBaseScore * 3;
			}
			else
			{
				nBaseScore = nBaseScore;
			}
		}
		else
		{
			if (m_bOpenMouth[wChairId])
			{
				if( !(chr & CHR_ZI_MO).IsEmpty() )
				{
					nBaseScore = nBaseScore * 4;
				}
				else
				{
					nBaseScore = nBaseScore * 2;
				}
			}
			else
			{
				if( !(chr & CHR_ZI_MO).IsEmpty() )
				{
					nBaseScore = nBaseScore * 3;
				}
				else
				{
					nBaseScore = nBaseScore;
				}
			}			
		}
		if( !(chr & CHR_QI_DUI_2).IsEmpty() )
		{
			nBaseFan ++;
		}
		if( !(chr & CHR_QI_DUI_3).IsEmpty() )
		{
			nBaseFan += 2;
		}
		if( !(chr & CHR_PENGPENGHU).IsEmpty())
		{
			if( !(chr & CHR_QING_YI_SE).IsEmpty() )
			{
				nBaseFan ++;
			}			
		}
		if( !(chr & CHR_QI_DUI_2).IsEmpty())
		{
			if( !(chr & CHR_QING_YI_SE).IsEmpty() )
			{
				nBaseFan ++;
			}			
		}
		if( !(chr & CHR_NONE).IsEmpty() )
		{
			nBaseFan ++;
		}
	}
	else
	{
		if( !(chr & CHR_ZI_MO).IsEmpty() )
		{
			nBaseScore = nBaseScore;
		}
		else
		{
			nBaseScore = nBaseScore;
		}
		if( !(chr & CHR_NONE).IsEmpty() )
		{
			if( !(chr & CHR_MEN_QING).IsEmpty() )
			{
				nBaseScore = nBaseScore * 3;
			}
			else
			{
				nBaseFan ++;
			}			
		}
	}

	return nBaseFan;
}
//
int CTableFrameSink::FiltrateRight( WORD wChairId,CChiHuRight &chr )
{
	int ret = 0;
	//���ϻ�
	if( m_wCurrentUser==wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_DA_HU;
	}
	else
	{
		if (m_wProvideUser == wChairId)
		{
			m_ChiHuRight[wChairId] |= CHR_ZI_MO;
			//chr |= CHR_DA_HU;
		}
	}
	if (m_wProvideUser != wChairId)
	{
		m_ChiHuRight[wChairId] |= CHR_FANG_PAO;
	}

	if( !(chr&CHR_DA_HU).IsEmpty() )
	{
		ret ++;
		if( !(chr&CHR_QI_DUI_2).IsEmpty() )
		{
			ret ++;
		}
		if( !(chr&CHR_QI_DUI_3).IsEmpty() )
		{
			ret ++;
		}
	}
	else
	{

	}
	if( !(chr&CHR_ZI_MO).IsEmpty() )
	{
		ret ++;
	}
	if( !(chr&CHR_NONE).IsEmpty() )
	{
		ret ++;
	}
	if(m_bGameTypeQing == 2)
	{
		bool bqing = true;
		for( BYTE i = 0; i < m_cbWeaveItemCount[wChairId]; i++ )
		{
			if( m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG_PI ) 
			{
				bqing = false;
			}
		}
		if(bqing)
		{
			chr |= CHR_MEN_QING;
			ret ++;
		}
	}
	return ret;
}

//���ݺ��и� ���
int CTableFrameSink::AccountScoreHZHZG(WORD wChairId, bool bCheckScore)
{
	//����Ȩλ
	CChiHuRight &chr = m_ChiHuRight[wChairId];
	if (bCheckScore)
	{
		chr = m_ChiHuRightTemp[wChairId];
	}

	BYTE nBaseScore = 0;
	BYTE nPlayerScore = 0;
	int nBaoPlayer = -1;
	BYTE nPlayerFanShu[GAME_PLAYER] = {0}; // ���ҷ���
	int nHuScore[GAME_PLAYER] = {0}; // ����
	BYTE cbMaxScoreNum = 0;
	ClearChiHuData();
	ChiHu.resizeData();
	AccountFanShuHZHZG(wChairId, chr, nPlayerFanShu, nBaoPlayer); // ���㷬��
	if (m_bGameMaxScore == 15) // 15�ⶥ��20�𶥣�25����ӣ�30���ⶥ
	{
		if (!(chr & CHR_DA_HU).IsEmpty()) // ���
		{
			if (m_wProvideUser == wChairId) // ����
			{
				nBaseScore = 2;
				nPlayerScore = 2;
			}
			else // ����
				nBaseScore = 2;
				nPlayerScore = 1;
		}
		else // С��
		{
			if (m_wProvideUser == wChairId) // ����
			{
				// �׷�Ϊ0.5�� ��ʼ���1 ���ٳ���2
				nBaseScore = 1;
				nPlayerScore = 1;
			}
			else // ����
				// �׷�Ϊ0.5�� ��ʼ���1 ���ٳ���2
				nBaseScore = 1;
				nPlayerScore = 0;
		}
		if (m_wProvideUser == wChairId) // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (!m_bPlayStatus[i])
				{
					continue;
				}
				if (i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
			}
		}
		else // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if(!m_bPlayStatus[i])
				{
					continue;
				}
				if(i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				if (i == m_wProvideUser) // ���ڷ�
				{
					nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
				}
				else // �м�
				{
					nHuScore[i] = nPlayerScore * (pow(2, m_nFanShu[i]));
				}
			}
		}
		if ((chr & CHR_DA_HU).IsEmpty()) // С��
		{
			for (BYTE i = 0; i < m_cbPlayerNum; i++)
			{
				nHuScore[i] /= 2;
				if (nHuScore[i] < 4) nHuScore[i] = 0;
			}
		}
		int iScoreTemp = 0;
		for (BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if(i == wChairId)
			{
				continue;
			}
			iScoreTemp += nHuScore[i];
		}
		if (iScoreTemp < 4) // 3�Ҷ�����4�� ���ܺ�
		{
			return -1;
		}
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (nHuScore[i] == 0) nHuScore[i] = 0;
			else if (nHuScore[i] <= 5) nHuScore[i] = 5;
			else if (nHuScore[i] <= 10) nHuScore[i] = 10;
			else if (nHuScore[i] > 15) nHuScore[i] = 15;
			if(nHuScore[i] >= 15) cbMaxScoreNum++;
		}
		// �жϽ�
		if (m_cbPlayerNum == 4 && cbMaxScoreNum == 3)
		{
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 20;
			}
			// �ж����ⶥ
			if (m_wProvideUser == wChairId)
			{
				BYTE cbCheckNoMouthNum = 0;
				for (BYTE i = 0; i < GAME_PLAYER; i++)
				{
					if (i == wChairId)
					{
						continue;
					}
					if (!m_bOpenMouth[i]) // û����
					{
						cbCheckNoMouthNum++;
					}
				}
				if (cbCheckNoMouthNum == 3) // 3�Ҷ�û���� ���ⶥ
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						nHuScore[i] = 30;
					}
				}
				else
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						if (!m_bOpenMouth[i])
						{
							nHuScore[i] = 25;
						}
						else
							nHuScore[i] = 20;
					}
				}
			}
		}
		else if (m_cbPlayerNum == 3 && cbMaxScoreNum == 2)// ��
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 20;
			}
		}
	}
	else if (m_bGameMaxScore == 30) // 30�ⶥ��40�𶥣�50����ӣ�60���ⶥ
	{
		if (!(chr & CHR_DA_HU).IsEmpty()) // ���
		{
			if (m_wProvideUser == wChairId) // ����
			{
				nBaseScore = 4;
				nPlayerScore = 4;
			}
			else // ����
				nBaseScore = 4;
				nPlayerScore = 2;
		}
		else // С��
		{
			if (m_wProvideUser == wChairId) // ����
			{
				nBaseScore = 1;
				nPlayerScore = 1;
			}
			else // ����
				nBaseScore = 1;
				nPlayerScore = 0;
		}
		if (m_wProvideUser == wChairId) // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (!m_bPlayStatus[i])
				{
					continue;
				}
				if (i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
				if (nHuScore[i] < 8)
				{
					nHuScore[i] = 0;
				}
			}
		}
		else // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if(!m_bPlayStatus[i])
				{
					continue;
				}
				if(i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				if (i == m_wProvideUser) // ���ڷ�
				{
					nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
					if (nHuScore[i] < 8)
					{
						nHuScore[i] = 0;
					}
				}
				else // �м�
				{
					nHuScore[i] = nPlayerScore * (pow(2, m_nFanShu[i]));
					if (nHuScore[i] < 8)
					{
						nHuScore[i] = 0;
					}
				}
			}
		}
		int iScoreTemp = 0;
		for (BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if(i == wChairId)
			{
				continue;
			}
			iScoreTemp += nHuScore[i];
		}
		if (iScoreTemp < 8) // 3�Ҷ�����8�� ���ܺ�
		{
			return -1;
		}
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (nHuScore[i] == 0) nHuScore[i] = 0;
			else if(nHuScore[i] <= 10) nHuScore[i] = 10;
			else if(nHuScore[i] <= 20) nHuScore[i] = 20;
			else if(nHuScore[i] > 20) nHuScore[i] = 30;
			if(nHuScore[i] >= 30) cbMaxScoreNum++;
		}
		// �жϽ�
		if (m_cbPlayerNum == 4 && cbMaxScoreNum == 3)
		{
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 40;
			}
			// �ж����ⶥ
			if (m_wProvideUser == wChairId)
			{
				BYTE cbCheckNoMouthNum = 0;
				for (BYTE i = 0; i < GAME_PLAYER; i++)
				{
					if (i == wChairId)
					{
						continue;
					}
					if (!m_bOpenMouth[i]) // û����
					{
						cbCheckNoMouthNum++;
					}
				}
				if (cbCheckNoMouthNum == 3) // 3�Ҷ�û���� ���ⶥ
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						nHuScore[i] = 60;
					}
				}
				else
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						if (!m_bOpenMouth[i])
						{
							nHuScore[i] = 50;
						}
						else
							nHuScore[i] = 40;
					}
				}
			}
		}
		if (m_cbPlayerNum == 3 && cbMaxScoreNum == 2) // ��
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 40;
			}
		}
	}
	else if (m_bGameMaxScore == 50) // 50�ⶥ��60�𶥣�80����ӣ�1000���ⶥ
	{
		if (!(chr & CHR_DA_HU).IsEmpty()) // ���
		{
			if (m_wProvideUser == wChairId) // ����
			{
				nBaseScore = 8;
				nPlayerScore = 8;
			}
			else // ����
				nBaseScore = 8;
				nPlayerScore = 4;
		}
		else // С��
		{
			if (m_wProvideUser == wChairId) // ����
			{
				nBaseScore = 2;
				nPlayerScore = 2;
			}
			else // ����
				nBaseScore = 2;				
				nPlayerScore = 0;
		}
		if (m_wProvideUser == wChairId) // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (!m_bPlayStatus[i])
				{
					continue;
				}
				if (i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
				if (nHuScore[i] < 16)
				{
					nHuScore[i] = 0;
				}
			}
		}
		else // ����
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if(!m_bPlayStatus[i])
				{
					continue;
				}
				if(i == wChairId)
				{
					continue;
				}
				m_nFanShu[i] += nPlayerFanShu[i];
				m_nFanShu[i] += nPlayerFanShu[wChairId];
				if (i == m_wProvideUser) // ���ڷ�
				{
					nHuScore[i] = nBaseScore * (pow(2, m_nFanShu[i]));
					if (nHuScore[i] < 16)
					{
						nHuScore[i] = 0;
					}
				}
				else // �м�
				{
					nHuScore[i] = nPlayerScore * (pow(2, m_nFanShu[i]));
					if (nHuScore[i] < 16)
					{
						nHuScore[i] = 0;
					}
				}
			}
		}
		int iScoreTemp = 0;
		for (BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if(i == wChairId)
			{
				continue;
			}
			iScoreTemp += nHuScore[i];
		}
		if (iScoreTemp < 16) // 3�Ҷ�����16�� ���ܺ�
		{
			return -1;
		}
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (nHuScore[i] == 0) nHuScore[i] = 0;
			else if(nHuScore[i] <= 20) nHuScore[i] = 20;
			else if(nHuScore[i] <= 40) nHuScore[i] = 40;
			else if(nHuScore[i] > 40) nHuScore[i] = 50;
			if(nHuScore[i] >= 50) cbMaxScoreNum++;
		}
		// �жϽ�
		if (m_cbPlayerNum == 4 && cbMaxScoreNum == 3)
		{
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 60;
			}
			// �ж����ⶥ
			if (m_wProvideUser == wChairId)
			{
				BYTE cbCheckNoMouthNum = 0;
				for (BYTE i = 0; i < GAME_PLAYER; i++)
				{
					if (i == wChairId)
					{
						continue;
					}
					if (!m_bOpenMouth[i]) // û����
					{
						cbCheckNoMouthNum++;
					}
				}
				if (cbCheckNoMouthNum == 3) // 3�Ҷ�û���� ���ⶥ
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						nHuScore[i] = 100;
					}
				}
				else
				{
					for (BYTE i = 0; i < GAME_PLAYER; i++)
					{
						if (i == wChairId)
						{
							continue;
						}
						if (!m_bOpenMouth[i])
						{
							nHuScore[i] = 80;
						}
						else
							nHuScore[i] = 60;
					}
				}
			}
		}
		if (m_cbPlayerNum == 3 && cbMaxScoreNum == 2) // ��
		{
			for (int i = 0; i < m_cbPlayerNum; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				nHuScore[i] = 60;
			}
		}
	}

	if (nBaoPlayer != -1)
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += nHuScore[i];
				ChiHu.lHuPaiScore[nBaoPlayer] -= nHuScore[i];
			}
		}
		ChiHu.lHuPaiScore[nBaoPlayer] |= CHR_BAO_HU;
	}
	else 
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += nHuScore[i];
				ChiHu.lHuPaiScore[i] -= nHuScore[i];
			}
		}
	}
	if ((m_cbPantherTypeServer & TYPE_FENGBAO) || (m_cbPantherTypeServer & TYPE_JIANGBAO) || (m_cbPantherTypeServer & TYPE_DANBAO) || (m_cbPantherTypeServer & TYPE_SEBAO))
	{
		for (int j = 0; j < m_cbPlayerNum; j++)
		{
			ChiHu.lHuPaiScore[j] *= 2;
		}
	}
	if (bCheckScore)
	{
		return ChiHu.lHuPaiScore[wChairId];
	}
	//���ñ���
	m_wProvider[wChairId] = m_wProvideUser;
	m_wWinOrder[m_cbWinCount++] = wChairId;
	m_bGangStatus = false;
	m_bGangHuaStatus = false;
	m_bGangOutStatus = false;
	//������Ϣ		
	ChiHu.wChiHuUser = wChairId;
	ChiHu.wProviderUser = m_wProvideUser;
	ChiHu.cbChiHuCard = m_cbProvideCard;

	DWORD wChiHuRight[1] = {0};
	m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0], MAX_RIGHT_COUNT);
	ChiHu.dwChiHuRight = wChiHuRight[0];
	ChiHu.cbGenCount = m_cbGenCount[wChairId];
	if (nBaoPlayer != -1)
	{
		ChiHu.wBaoPlayer = nBaoPlayer;
	}
	else
		ChiHu.wBaoPlayer = 10;
	for (int i=0;i<ChiHu.lMenHun.size();i++)
	{
		ChiHu.lMenHun[i]=m_bOpenMouth[i];
	}	
	for (int i=0;i<ChiHu.lPiao.size();i++)
	{
		ChiHu.lPiao[i]=m_cbPiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lDang.size();i++)
	{
		ChiHu.lDang[i]=m_cbLaiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lGang.size();i++)
	{
		ChiHu.lGang[i]=m_cbGenCount[i];
	}
	ChiHu.cbHuScore = nBaseScore;
	ChiHu.cbHuScoreOther = nPlayerScore;
	for (int i=0;i<ChiHu.lMingGang.size();i++)
	{
		ChiHu.lMingGang[i]=m_lOtherGangCount[i];
	}	
	for (int i=0;i<ChiHu.lAnGang.size();i++)
	{
		ChiHu.lAnGang[i]=m_lSelfGangCount[i];
	}		
	for (int i=0;i<ChiHu.lXiGang.size();i++)
	{
		ChiHu.lXiGang[i]=m_nFanShu[i];
	}	

	datastream kDataStream;
	ChiHu.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

	m_kChiHuInfoList.push_back(ChiHu);
	addGameRecordAction(ChiHu);
	//���ñ���

	m_bHasHuPai[wChairId] = true;
	{
		m_bPlayStatus[wChairId] = false;
	}

	return 0;
}

//���ݺ��и� �㷬
int CTableFrameSink::AccountFanShuHZHZG(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer)
{
	ZeroMemory(m_cbGenCount, sizeof(m_cbGenCount));
	ZeroMemory(m_cbPiGenCount, sizeof(m_cbPiGenCount));	
	ZeroMemory(m_cbLaiGenCount, sizeof(m_cbLaiGenCount));
	ZeroMemory(m_nFanShu, sizeof(m_nFanShu));
	ZeroMemory(m_lOtherGangCount, sizeof(m_lOtherGangCount));
	ZeroMemory(m_lSelfGangCount, sizeof(m_lSelfGangCount));
	m_bNeedCalGang = true;

	//������Ʒ��ķ���  ���� ����  Ƥ��  ���
	for(int i = 0; i < m_cbWeaveItemCount[wChairId]; i++)
	{
		if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
		{
			if(m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
			{
				m_cbGenCount[wChairId]++;
				if(m_WeaveItemArray[wChairId][i].cbCenterCard == m_cbProvideCard)
				{
					if(m_WeaveItemArray[wChairId][i].wProvideUser != wChairId)
					{
						continue;
					}
				}
				m_lOtherGangCount[wChairId]++;
			}
			else
			{
				m_cbGenCount[wChairId] += 2;
				m_lSelfGangCount[wChairId]++;
			}
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_LAIPI) 
		{
			m_cbPiGenCount[wChairId] += 1;
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_HUN) 
		{
			m_cbLaiGenCount[wChairId] += 2;
		}
		else if (m_WeaveItemArray[wChairId][i].cbWeaveKind >= WIK_QING_BAO && m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_SHOW_BAO)
		{
			if (wChairId != m_wProvideUser && m_nUsefulOper[wChairId] == 2)
			{						
				if( !(chr&CHR_QING_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
			}
			else
			{
				if(m_nUsefulOper[wChairId] >= 3 && wChairId != m_WeaveItemArray[wChairId][i].wProvideUser)
				{
					if( !(chr&CHR_QING_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}						
				}				
			}
		}
	}
	//���ϻ�
	if( m_wCurrentUser==wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
		nPlayerFanShu[wChairId]++;
	}
	if (m_wProvideUser == wChairId)  // ����
	{
		chr |= CHR_ZI_MO;
	}
	else // ����
	{
		chr |= CHR_FANG_PAO;
	}
	if (!(chr & CHR_NONE).IsEmpty())  //Ӳ�� 
	{
		nPlayerFanShu[wChairId]++;
	}
	if (!(chr & CHR_PENGPENGHU).IsEmpty())
	{
		if (!(chr & CHR_QING_YI_SE).IsEmpty())
		{
			nPlayerFanShu[wChairId]++;
		}
		else
		{
			BYTE cbCardColor=0xFF;
			BYTE cbQingYiSe = 0;
			for (BYTE i = 0; i < MAX_INDEX; i++)
			{
				if(i==m_nHunIndex) continue;
				if (m_cbCardIndex[wChairId][i]!=0)
				{
					//��ɫ�ж�
					if (cbCardColor!=0xFF)
					{
						cbQingYiSe = 1;
						break;
					}
					//���û�ɫ
					cbCardColor=(m_GameLogic.SwitchToCardData(i)&MASK_COLOR);

					//��������
					i=(i/9+1)*9-1;
				}
			}
			//�������ֻ������
			if( cbCardColor == 0xFF )
			{
				ASSERT( m_nHunIndex != MAX_INDEX && cbCardIndex[m_nHunIndex] > 0 );
				//������
				ASSERT( m_cbWeaveItemCount[wChairId] > 0 );
				cbCardColor = m_WeaveItemArray[wChairId][0].cbCenterCard&MASK_COLOR;
			}
			//����ж�
			for (BYTE i=0;i<m_cbWeaveItemCount[wChairId];i++)
			{
				if (m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG_PI)
				{
					BYTE cbCenterCard=m_WeaveItemArray[wChairId][i].cbCenterCard;
					if ((cbCenterCard&MASK_COLOR)!=cbCardColor)
					{
						cbQingYiSe = 1;
						break;
					}
				}
			}
			if (!cbQingYiSe)
			{
				chr |= CHR_QING_YI_SE;
				nPlayerFanShu[wChairId]++;
			}
		}
	}
	if(m_nUsefulOper[wChairId] >= 4 && m_wProvideUser != wChairId && (chr & CHR_DA_HU).IsEmpty() && m_bAllOther == 1)
	{
		chr |= CHR_ALL_OTHER;
		chr |= CHR_DA_HU;
		nBaoPlayer = m_wProvideUser;
	}
	nPlayerFanShu[wChairId] += (m_cbGenCount[wChairId] + m_cbPiGenCount[wChairId] + m_cbLaiGenCount[wChairId]); // �����ܷ���

	//�����мҷ��� 
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if(i == wChairId) continue;
		if(m_bOpenMouth[i]) nPlayerFanShu[i]++;
		for(int j = 0; j < m_cbWeaveItemCount[i]; j++)
		{
			if(m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG) 
			{
				if( m_WeaveItemArray[i][j].cbPublicCard == 1) 
				{
					m_cbGenCount[i]++;
					if(m_WeaveItemArray[i][j].cbCenterCard == m_cbProvideCard)
					{
						if(m_WeaveItemArray[i][j].wProvideUser != wChairId)
						{
							continue;
						}
					}
					m_lOtherGangCount[i]++;
				}
				else
				{
					m_cbGenCount[i] += 2;
					m_lSelfGangCount[i] ++;
				}
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_LAIPI) 
			{
				m_cbPiGenCount[i] += 1;
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_HUN)  //�����2��
			{
				m_cbLaiGenCount[i] += 2;
			}
		}
		nPlayerFanShu[i] += (m_cbGenCount[i] + m_cbPiGenCount[i] + m_cbLaiGenCount[i]); // �м��ܷ���
	}
	return 0;
}

//��÷���
int CTableFrameSink::AccountScoreHM(const BYTE cbCardIndex[MAX_INDEX], WORD wChairId)
{
	//����Ȩλ
	CChiHuRight &chr = m_ChiHuRightHM[wChairId];
	BYTE cbFanShu = 1;
	int iPaoShu = 0;
	int iFaCaiScore = 0;
	int iBaiBanScore = 0;
	int iHongZhongScore = 0;
	int nHuScore[GAME_PLAYER] = {0}; // ����
	if (!(chr & CHR_NONE).IsEmpty()) cbFanShu = 2;
	AccountScoreHMFZ(cbCardIndex,wChairId,iPaoShu,iFaCaiScore,iBaiBanScore,iHongZhongScore,m_ChiHuRightHM[wChairId]);
	if (m_wProvideUser != wChairId)
	{
		if ((chr & CHR_DA_HU).IsEmpty())
		{
			iPaoShu += 1;
			if (m_bGangStatus)
			{
				iPaoShu += 4;
				chr |= CHR_QIANG_GANG;
			}
		}
		else
		{
			if (!(chr & CHR_QI_DUI).IsEmpty() || !(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty()) iPaoShu += 9;
			if (!(chr & CHR_PENGPENGHU).IsEmpty()) iPaoShu += 10;
			if (!(chr & CHR_QING_YI_SE).IsEmpty()) iPaoShu += 15;
		}
		if (m_bGangStatus)
		{
			iPaoShu += 5;
			chr |= CHR_QIANG_GANG;
		}
		nHuScore[wChairId] = (iPaoShu * cbFanShu + m_bPiaoShu) * m_bGameBaseScore;
		nHuScore[m_wProvideUser] = -(iPaoShu * cbFanShu + m_bPiaoShu) * m_bGameBaseScore;
	}
	else if (m_wProvideUser == wChairId)
	{
		if ((chr & CHR_DA_HU).IsEmpty())
		{
			iPaoShu += 2;
			if (m_bGangStatus)
			{
				iPaoShu += 3;
				chr |= CHR_GANG_KAI;
			}
		}
		else
		{
			if (!(chr & CHR_QI_DUI).IsEmpty() || !(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty()) iPaoShu += 10;
			if (!(chr & CHR_PENGPENGHU).IsEmpty()) iPaoShu += 11;
			if (!(chr & CHR_QING_YI_SE).IsEmpty()) iPaoShu += 16;
			if (m_bGangStatus)
			{
				iPaoShu += 5;
				chr |= CHR_GANG_KAI;
			}
		}
		for(BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId) continue;
			nHuScore[wChairId] += (iPaoShu * cbFanShu + m_bPiaoShu) * m_bGameBaseScore;
			nHuScore[i] = -(iPaoShu * cbFanShu + m_bPiaoShu) * m_bGameBaseScore;
		}
	}

	//���ñ���
	m_wProvider[wChairId] = m_wProvideUser;
	m_wWinOrder[m_cbWinCount++] = wChairId;
	m_bGangStatus = false;
	m_bGangHuaStatus = false;
	m_bGangOutStatus = false;

	ChiHuHM.cbChiHuCard = m_cbProvideCard;
	ChiHuHM.wProviderUser = m_wProvideUser;

	DWORD wChiHuRight[1] = {0};
	m_ChiHuRightHM[wChairId].GetRightData( &wChiHuRight[0], MAX_RIGHT_COUNT);
	ChiHuHM.dwChiHuRight[wChairId] = wChiHuRight[0];
	ChiHuHM.wChiHuUser[wChairId] = wChairId;
	if (m_dwChiHuUserTemp == 100) m_dwChiHuUserTemp = wChairId;
	for (int i=0;i<ChiHuHM.lMingGang.size();i++)
	{
		ChiHuHM.lMingGang[i] = m_lOtherGangCount[i];
	}
	for (int i=0;i<ChiHuHM.lAnGang.size();i++)
	{
		ChiHuHM.lAnGang[i] = m_lSelfGangCount[i];
	}
	for (int i=0;i<ChiHuHM.lHuPaiScore.size();i++)
	{
		ChiHuHM.lHuPaiScore[i] += nHuScore[i];
	}
	ChiHuHM.lFaCaiScore[wChairId] = iFaCaiScore;
	ChiHuHM.lHongZhongScore[wChairId] = iHongZhongScore;
	ChiHuHM.lBaiBanScore[wChairId] = iBaiBanScore;

	//���ñ���
	m_bHasHuPai[wChairId] = true;
	m_bPlayStatus[wChairId] = false;

	return 0;
}

//��÷��ָ���
void CTableFrameSink::AccountScoreHMFZ(const BYTE cbCardIndex[MAX_INDEX],WORD wChairId,int &iScore,int &iFaCaiScore,int &iBaiBanScore,int &iHongZhongScore,CChiHuRight &chr)
{
	iScore = 0;
	iFaCaiScore = 0;
	iBaiBanScore = 0;
	iHongZhongScore = 0;
	// ���㷢�Ƶĸ���
	if (m_cbFaCaiNum[wChairId] == 4)
	{
		iScore += 10;
		iFaCaiScore = 10;
	}
	else 
	{
		iScore += m_cbFaCaiNum[wChairId];
		iFaCaiScore = m_cbFaCaiNum[wChairId];
	}
	bool bMenQing = true;
	// ����ܷ֣����У��װ������
	for(int i = 0 ; i < m_cbWeaveItemCount[wChairId]; i++)
	{
		if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
		{
			if (m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
			{
				if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35 || m_WeaveItemArray[wChairId][i].cbCenterCard == 0x37)
				{
					iScore += 3;
					if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35) iHongZhongScore = 3;
					else iBaiBanScore = 3;
				}
				else
				{
					iScore += 1;
					m_lOtherGangCount[wChairId]++;
				}
				bMenQing = false;
			}
			else
			{
				if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x36)
				{
					iScore += 10;
					iFaCaiScore = 10;
				}
				else if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35 || m_WeaveItemArray[wChairId][i].cbCenterCard == 0x37)
				{
					iScore += 4;
					if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35) iHongZhongScore = 4;
					else iBaiBanScore = 4;
				}
				else
				{
					 iScore += 2;
					 m_lSelfGangCount[wChairId] += 2;
				}
			}
		}
		else if (m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_GANG)
		{
			if (m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_PENG && 
				(m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35 || m_WeaveItemArray[wChairId][i].cbCenterCard == 0x37))
			{
				iScore += 1;
				if (m_WeaveItemArray[wChairId][i].cbCenterCard == 0x35) iHongZhongScore = 1;
				else iBaiBanScore = 1;
			}
			bMenQing = false;
		}
	}
	//��������
	if (bMenQing)
	{
		iScore += 1;
		chr |= CHR_MEN_QING;
	}
	
	if (m_wProvideUser == wChairId)
	{
		chr |= CHR_ZI_MO;
	}
	//���������еĺ��кͰװ�
	if (m_iBaoIndex != 31 && cbCardIndex[31] >= 3)
	{
		iScore += 1;
		iHongZhongScore = 1;
	}
	if (m_iBaoIndex != 33 && cbCardIndex[33] >= 3)
	{
		iScore += 1;
		iBaiBanScore = 1;
	}
	//����Ƿ������
	BYTE cbDanZhang = 0;
	BYTE cbCardIndexTemp[MAX_INDEX] = {0};
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	cbCardIndexTemp[m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
	CAnalyseItemArray AnalyseItemArray;
	if (m_iBaoIndex == MAX_INDEX || cbCardIndexTemp[m_iBaoIndex] == 0)
	{
		do {
			if (!(chr & CHR_QI_DUI).IsEmpty() || !(chr & CHR_QI_DUI_2).IsEmpty() || !(chr & CHR_QI_DUI_3).IsEmpty())
			{
				cbDanZhang = 1;
				break;
			}
			for(BYTE i = 0; i < MAX_INDEX; i++)
			{
				if (i == m_iBaoIndex || i == m_GameLogic.SwitchToCardIndex(m_cbProvideCard)) continue;
				AnalyseItemArray.RemoveAll();
				cbCardIndexTemp[i]++;
				m_GameLogic.AnalyseCard(cbCardIndexTemp,NULL,0,AnalyseItemArray,false, 4);
				if (AnalyseItemArray.GetCount() > 0) 
				{
					cbDanZhang = 1;
					break;
				}
				cbCardIndexTemp[i]--;
			}
		}while(false);

	}
	else cbDanZhang = 1;
	if (!cbDanZhang)
	{
		iScore += 1;
		chr |= CHR_DAN_ZHANG;
	}
}

// ���ͽ�������(��÷)
void CTableFrameSink::SendGameEndScore()
{
	datastream kDataStream;
	ChiHuHM.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

	m_kChiHuInfoListHM.push_back(ChiHuHM);
	//���滹���ϵ�GDMJ_CMD_S_ChiHu
	GDMJ_CMD_S_ChiHu ChiHu;
	ChiHu.resizeData();
	ChiHu.wChiHuUser = m_dwChiHuUserTemp;
	ChiHu.wProviderUser = m_wProvideUser;
	ChiHu.cbChiHuCard = m_cbProvideCard;
	ChiHu.dwChiHuRight = ChiHuHM.dwChiHuRight[m_dwChiHuUserTemp];
	ChiHu.wBaoPlayer = 10;
	for (int i=0;i<ChiHu.lMenHun.size();i++)
	{
		ChiHu.lMenHun[i]=m_bOpenMouth[i];
	}	
	for (int i=0;i<ChiHu.lGang.size();i++)
	{
		ChiHu.lGang[i]=m_cbGenCount[i];
	}
	ChiHu.cbHuScore = 0;
	ChiHu.cbHuScoreOther = 0;
	for (int i=0;i<ChiHu.lMingGang.size();i++)
	{
		ChiHu.lMingGang[i]=m_lOtherGangCount[i];
	}	
	for (int i=0;i<ChiHu.lAnGang.size();i++)
	{
		ChiHu.lAnGang[i]=m_lSelfGangCount[i];
	}
	addGameRecordAction(ChiHu);
}

//Ӣɽ���
int CTableFrameSink::AccountScoreYS(WORD wChairId, bool bCheckScore)
{
	//����Ȩλ
	CChiHuRight &chr = m_ChiHuRight[wChairId];
	if (bCheckScore)
	{
		chr.SetEmpty();
		chr = m_ChiHuRightTemp[wChairId];
	}
	float fBaseScore = 0;
	float fPlayerScore = 0;
	int nBaoPlayer = -1;
	BYTE nPlayerFanShu[GAME_PLAYER] = {0}; // ���ҷ���
	float fHuScore[GAME_PLAYER] = {0}; // ����
	ClearChiHuData();
	ChiHu.resizeData();
	AccountFanShuYS(wChairId, chr, nPlayerFanShu, nBaoPlayer); // ���㷬��
	if (m_bGameTypeIndex == 8 && !m_bBaoIsOutCard) nBaoPlayer = -1;

	if (!(chr & CHR_DA_HU).IsEmpty()) // ���
	{
		if (m_wProvideUser == wChairId) // ����
		{
			if (m_bGameBaseScore == 1) // 0.2��
				fBaseScore = (fPlayerScore = 0.8);
			else if (m_bGameBaseScore == 2) // 0.625��
				fBaseScore = (fPlayerScore = 2);
			else // 1.25�� 
				fBaseScore = (fPlayerScore = 8);
		}
		else // ����
		{
			if (m_bGameBaseScore == 1) // 0.2��
				fBaseScore = (fPlayerScore = 0.4) * 2;
			else if (m_bGameBaseScore == 2) // 0.625��
				fBaseScore = (fPlayerScore = 1) * 2;
			else // 1.25��
				fBaseScore = (fPlayerScore = 4) * 2;
		}		
	}
	else // С��
	{
		if (m_wProvideUser == wChairId) // ����
		{
			if (m_bGameBaseScore == 1) fBaseScore = (fPlayerScore = 0.2);
			else if (m_bGameBaseScore == 2) fBaseScore = (fPlayerScore = 0.5);
			else fBaseScore = (fPlayerScore = 2);

		}
		else // С������
		{
			fPlayerScore = 0;
			if (m_bGameBaseScore == 1) fBaseScore = 0.2;
			else if (m_bGameBaseScore == 2) fBaseScore = 0.5;
			else fBaseScore = 2;
		}
	}
	float fScoreTemp = 0;
	if (m_wProvideUser == wChairId) // ����
	{
		for (int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			m_nFanShu[i] = nPlayerFanShu[i] + nPlayerFanShu[wChairId];
			fHuScore[i] = fBaseScore * (pow(2, m_nFanShu[i]));
			if (m_bGameBaseScore == 1 && fHuScore[i] < 1.6) fHuScore[i] = 0;
			else if (m_bGameBaseScore == 2 && fHuScore[i] < 4) fHuScore[i] = 0;
			else if (m_bGameBaseScore == 3 && fHuScore[i] < 8) fHuScore[i] = 0;
			fScoreTemp += fHuScore[i];
		}
	}
	else // ����
	{
		for (int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i == wChairId)
			{
				continue;
			}
			m_nFanShu[i] = nPlayerFanShu[i] + nPlayerFanShu[wChairId];
			if (i == m_wProvideUser) // ���ڷ�
			{
				fHuScore[i] = fBaseScore * (pow(2, m_nFanShu[i]));
			}
			else // �м�
			{
				fHuScore[i] = fPlayerScore * (pow(2, m_nFanShu[i]));
			}
			fScoreTemp += fHuScore[i];
		}
	}
	if (bCheckScore)
	{
		if (m_bGameBaseScore == 1 && fScoreTemp < 3.2) return -1;
		else if (m_bGameBaseScore == 2 && fScoreTemp < 8) return -1;
		else if (m_bGameBaseScore == 3 && fScoreTemp < 16) return -1;
		return 1;
	}
	BYTE cbMaxScoreNum = 0;
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if (i == wChairId)
		{
			continue;
		}
		if (m_bGameBaseScore == 1)
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] <= 2) fHuScore[i] = 2;
			else if (fHuScore[i] <= 4) fHuScore[i] = 4;
			else fHuScore[i] = 8;
			if(fHuScore[i] >= 8) cbMaxScoreNum++;
		}
		else if (m_bGameBaseScore == 2)
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] < 5) fHuScore[i] = 5;
			else if (fHuScore[i] < 10) fHuScore[i] = 10;
			else fHuScore[i] = 20;
			if(fHuScore[i] >= 20) cbMaxScoreNum++;
		}
		else
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] < 10) fHuScore[i] = 10;
			else if (fHuScore[i] < 20) fHuScore[i] = 20;
			else fHuScore[i] = 40;
			if(fHuScore[i] >= 40) cbMaxScoreNum++;
		}
	}
	if (m_cbPlayerNum == 4 && cbMaxScoreNum == 3) // ������
	{
		//������ⶥ
		BYTE cbCheckNoMouthNum = 0;
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (!m_bOpenMouth[i]) // û����
			{
				cbCheckNoMouthNum++;
			}
		}
		if (cbCheckNoMouthNum == 3) // 3�Ҷ�û���� ���ⶥ
		{
			for (BYTE i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				if (m_bGameBaseScore == 1) fHuScore[i] = 15;
				else if (m_bGameBaseScore == 2) fHuScore[i] = 50;
				else fHuScore[i] = 100;
			}
		}
		else // �𶥺���
		{
			for (BYTE i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				if (!m_bOpenMouth[i]) // ��
				{
					if (m_bGameBaseScore == 1) fHuScore[i] = 10;
					else if (m_bGameBaseScore == 2) fHuScore[i] = 30;
					else fHuScore[i] = 50;
				}
				else // ��
				{
					if (m_bGameBaseScore == 1) fHuScore[i] = 12;
					else if (m_bGameBaseScore == 2) fHuScore[i] = 40;
					else fHuScore[i] = 70;
				}
			}
		}
	}
	else if (m_cbPlayerNum == 3 && cbMaxScoreNum == 2) // ��
	{
		for (BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (m_bGameBaseScore == 1) fHuScore[i] = 10;
			else if (m_bGameBaseScore == 2) fHuScore[i] = 30;
			else fHuScore[i] = 50;
		}
	}
	if (nBaoPlayer != -1)
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += fHuScore[i];
				ChiHu.lHuPaiScore[nBaoPlayer] -= fHuScore[i];
			}
		}
		ChiHu.lHuPaiScore[nBaoPlayer] |= CHR_BAO_HU;
	}
	else 
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += fHuScore[i];
				ChiHu.lHuPaiScore[i] -= fHuScore[i];
			}
		}
	}

	//���ñ���
	m_wProvider[wChairId] = m_wProvideUser;
	m_wWinOrder[m_cbWinCount++] = wChairId;
	m_bGangStatus = false;
	m_bGangHuaStatus = false;
	m_bGangOutStatus = false;
	//������Ϣ		
	ChiHu.wChiHuUser = wChairId;
	ChiHu.wProviderUser = m_wProvideUser;
	ChiHu.cbChiHuCard = m_cbProvideCard;

	DWORD wChiHuRight[1] = {0};
	m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0], MAX_RIGHT_COUNT);
	ChiHu.dwChiHuRight = wChiHuRight[0];
	ChiHu.cbGenCount = m_cbGenCount[wChairId];
	if (nBaoPlayer != -1)
	{
		ChiHu.wBaoPlayer = nBaoPlayer;
	}
	else
		ChiHu.wBaoPlayer = 10;
	for (int i=0;i<ChiHu.lMenHun.size();i++)
	{
		ChiHu.lMenHun[i]=m_bOpenMouth[i];
	}	
	for (int i=0;i<ChiHu.lPiao.size();i++)
	{
		ChiHu.lPiao[i]=m_cbPiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lDang.size();i++)
	{
		ChiHu.lDang[i]=m_cbLaiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lGang.size();i++)
	{
		ChiHu.lGang[i]=m_cbGenCount[i];
	}
	for (int i=0;i<ChiHu.lMingGang.size();i++)
	{
		ChiHu.lMingGang[i]=m_lOtherGangCount[i];
	}	
	for (int i=0;i<ChiHu.lAnGang.size();i++)
	{
		ChiHu.lAnGang[i]=m_lSelfGangCount[i];
	}		
	for (int i=0;i<ChiHu.lXiGang.size();i++)
	{
		ChiHu.lXiGang[i]=m_nFanShu[i];
	}	

	datastream kDataStream;
	ChiHu.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

	m_kChiHuInfoList.push_back(ChiHu);
	addGameRecordAction(ChiHu);
	//���ñ���

	m_bHasHuPai[wChairId] = true;
	{
		m_bPlayStatus[wChairId] = false;
	}
}

//Ӣɽ�㷬
void CTableFrameSink::AccountFanShuYS(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER], int &nBaoPlayer)
{
	ZeroMemory(m_cbGenCount, sizeof(m_cbGenCount));
	ZeroMemory(m_cbPiGenCount, sizeof(m_cbPiGenCount));	
	ZeroMemory(m_cbLaiGenCount, sizeof(m_cbLaiGenCount));
	ZeroMemory(m_nFanShu, sizeof(m_nFanShu));
	ZeroMemory(m_lOtherGangCount, sizeof(m_lOtherGangCount));
	ZeroMemory(m_lSelfGangCount, sizeof(m_lSelfGangCount));
	m_bNeedCalGang = true;

	//������Ʒ��ķ���  ���� ����  Ƥ��  ���
	for(int i = 0; i < m_cbWeaveItemCount[wChairId]; i++)
	{
		if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
		{
			if(m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
			{
				m_cbGenCount[wChairId]++;
				if(m_WeaveItemArray[wChairId][i].cbCenterCard == m_cbProvideCard)
				{
					if(m_WeaveItemArray[wChairId][i].wProvideUser != wChairId)
					{
						continue;
					}
				}
				m_lOtherGangCount[wChairId]++;
			}
			else
			{
				m_cbGenCount[wChairId] += 2;
				m_lSelfGangCount[wChairId]++;
			}
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_LAIPI) 
		{
			m_cbPiGenCount[wChairId] += 1;
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_HUN) 
		{
			m_cbLaiGenCount[wChairId] += 2;
		}
		else if (m_WeaveItemArray[wChairId][i].cbWeaveKind >= WIK_QING_BAO && m_WeaveItemArray[wChairId][i].cbWeaveKind < WIK_SHOW_BAO)
		{
			if (wChairId != m_wProvideUser && m_nUsefulOper[wChairId] == 2)
			{						
				if( !(chr&CHR_QING_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
				if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
				{
					if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
					{
						nBaoPlayer = m_wProvideUser;
					}
				}
			}
			else
			{
				if(m_nUsefulOper[wChairId] >= 3 && wChairId != m_WeaveItemArray[wChairId][i].wProvideUser)
				{
					if( !(chr&CHR_QING_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_QING_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_FENG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_FENG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}
					if( !(chr&CHR_JIANG_YI_SE).IsEmpty() )
					{
						if (m_nUsefulQingFengJiang[wChairId] == WIK_JIANG_BAO)
						{
							nBaoPlayer = m_WeaveItemArray[wChairId][i].wProvideUser;
						}
					}						
				}				
			}
		}
	}
	//���ϻ�
	if( m_wCurrentUser==wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
		nPlayerFanShu[wChairId]++;
	}
	if (m_wProvideUser == wChairId)  // ����
	{
		chr |= CHR_ZI_MO;
	}
	else // ����
	{
		chr |= CHR_FANG_PAO;
	}
	if (!(chr & CHR_NONE).IsEmpty())  //Ӳ�� 
	{
		nPlayerFanShu[wChairId]++;
	}
	if (!(chr & CHR_PENGPENGHU).IsEmpty() && !(chr & CHR_QING_YI_SE).IsEmpty())
	{
		nPlayerFanShu[wChairId]++;
	}
	nPlayerFanShu[wChairId] += (m_cbGenCount[wChairId] + m_cbPiGenCount[wChairId] + m_cbLaiGenCount[wChairId]); // �����ܷ���

	//�����мҷ��� 
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if(i == wChairId) continue;
		if(m_bOpenMouth[i]) nPlayerFanShu[i]++;
		for(int j = 0; j < m_cbWeaveItemCount[i]; j++)
		{
			if(m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG) 
			{
				if( m_WeaveItemArray[i][j].cbPublicCard == 1) 
				{
					m_cbGenCount[i]++;
					if(m_WeaveItemArray[i][j].cbCenterCard == m_cbProvideCard)
					{
						if(m_WeaveItemArray[i][j].wProvideUser != wChairId)
						{
							continue;
						}
					}
					m_lOtherGangCount[i]++;
				}
				else
				{
					m_cbGenCount[i] += 2;
					m_lSelfGangCount[i] ++;
				}
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_LAIPI) 
			{
				m_cbPiGenCount[i] += 1;
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_HUN)  //�����2��
			{
				m_cbLaiGenCount[i] += 2;
			}
		}
		nPlayerFanShu[i] += (m_cbGenCount[i] + m_cbPiGenCount[i] + m_cbLaiGenCount[i]); // �м��ܷ���
	}
}

//������
int CTableFrameSink::AccountScoreMC(WORD wChairId, bool bCheckScore)
{
	//����Ȩλ
	CChiHuRight &chr = m_ChiHuRight[wChairId];
	if (bCheckScore)
	{
		chr.SetEmpty();
		chr = m_ChiHuRightTemp[wChairId];
	}
	float fBaseScore = 0;
	float fPlayerScore = 0;
	int nBaoPlayer = -1;
	BYTE nPlayerFanShu[GAME_PLAYER] = {0}; // ���ҷ���
	float fHuScore[GAME_PLAYER] = {0}; // ����
	ClearChiHuData();
	ChiHu.resizeData();
	AccountFanShuMC(wChairId, chr, nPlayerFanShu); // ���㷬��

	if (!(chr & CHR_DA_HU).IsEmpty()) // ���
	{
		if (m_wProvideUser == wChairId) // ����
		{
			if (m_bGameBaseScore == 1) // 0.2��
				fBaseScore = (fPlayerScore = 0.8);
			else if (m_bGameBaseScore == 2) // 0.625��
				fBaseScore = (fPlayerScore = 2);
			else // 1.25�� 
				fBaseScore = (fPlayerScore = 8);
		}
		else // ����
		{
			if (m_bGameBaseScore == 1) // 0.2��
				fBaseScore = (fPlayerScore = 0.4) * 2;
			else if (m_bGameBaseScore == 2) // 0.625��
				fBaseScore = (fPlayerScore = 1) * 2;
			else // 1.25��
				fBaseScore = (fPlayerScore = 4) * 2;
		}		
	}
	else // С��
	{
		if (m_wProvideUser == wChairId) // ����
		{
			if (m_bGameBaseScore == 1) fBaseScore = (fPlayerScore = 0.2);
			else if (m_bGameBaseScore == 2) fBaseScore = (fPlayerScore = 0.5);
			else fBaseScore = (fPlayerScore = 2);

		}
		else // С������
		{
			fPlayerScore = 0;
			if (m_bGameBaseScore == 1) fBaseScore = 0.2;
			else if (m_bGameBaseScore == 2) fBaseScore = 0.5;
			else fBaseScore = 2;
		}
	}
	float fScoreTemp = 0;
	if (m_wProvideUser == wChairId) // ����
	{
		for (int i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			m_nFanShu[i] = nPlayerFanShu[i] + nPlayerFanShu[wChairId];
			fHuScore[i] = fBaseScore * (pow(2, m_nFanShu[i]));
			if (m_bGameBaseScore == 1 && fHuScore[i] < 1.6) fHuScore[i] = 0;
			else if (m_bGameBaseScore == 2 && fHuScore[i] < 4) fHuScore[i] = 0;
			else if (m_bGameBaseScore == 3 && fHuScore[i] < 8) fHuScore[i] = 0;
			fScoreTemp += fHuScore[i];
		}
	}
	else // ����
	{
		for (int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i == wChairId)
			{
				continue;
			}
			m_nFanShu[i] = nPlayerFanShu[i] + nPlayerFanShu[wChairId];
			if (i == m_wProvideUser) // ���ڷ�
			{
				fHuScore[i] = fBaseScore * (pow(2, m_nFanShu[i]));
			}
			else // �м�
			{
				fHuScore[i] = fPlayerScore * (pow(2, m_nFanShu[i]));
			}
			fScoreTemp += fHuScore[i];
		}
	}
	if (bCheckScore)
	{
		if (m_bGameBaseScore == 1 && fScoreTemp < 3.2) return -1;
		else if (m_bGameBaseScore == 2 && fScoreTemp < 8) return -1;
		else if (m_bGameBaseScore == 3 && fScoreTemp < 16) return -1;
		return 1;
	}
	BYTE cbMaxScoreNum = 0;
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if (i == wChairId)
		{
			continue;
		}
		if (m_bGameBaseScore == 1)
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] <= 2) fHuScore[i] = 2;
			else if (fHuScore[i] <= 4) fHuScore[i] = 4;
			else fHuScore[i] = 8;
			if(fHuScore[i] >= 8) cbMaxScoreNum++;
		}
		else if (m_bGameBaseScore == 2)
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] < 5) fHuScore[i] = 5;
			else if (fHuScore[i] < 10) fHuScore[i] = 10;
			else fHuScore[i] = 20;
			if(fHuScore[i] >= 20) cbMaxScoreNum++;
		}
		else
		{
			if (fHuScore[i] == 0) fHuScore[i] = 0;
			else if (fHuScore[i] < 10) fHuScore[i] = 10;
			else if (fHuScore[i] < 20) fHuScore[i] = 20;
			else fHuScore[i] = 40;
			if(fHuScore[i] >= 40) cbMaxScoreNum++;
		}
	}
	if (m_cbPlayerNum == 4 && cbMaxScoreNum == 3) // ������
	{
		//������ⶥ
		BYTE cbCheckNoMouthNum = 0;
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (!m_bOpenMouth[i]) // û����
			{
				cbCheckNoMouthNum++;
			}
		}
		if (cbCheckNoMouthNum == 3) // 3�Ҷ�û���� ���ⶥ
		{
			for (BYTE i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				if (m_bGameBaseScore == 1) fHuScore[i] = 15;
				else if (m_bGameBaseScore == 2) fHuScore[i] = 50;
				else fHuScore[i] = 100;
			}
		}
		else // �𶥺���
		{
			for (BYTE i = 0; i < GAME_PLAYER; i++)
			{
				if (i == wChairId)
				{
					continue;
				}
				if (!m_bOpenMouth[i]) // ��
				{
					if (m_bGameBaseScore == 1) fHuScore[i] = 10;
					else if (m_bGameBaseScore == 2) fHuScore[i] = 30;
					else fHuScore[i] = 50;
				}
				else // ��
				{
					if (m_bGameBaseScore == 1) fHuScore[i] = 12;
					else if (m_bGameBaseScore == 2) fHuScore[i] = 40;
					else fHuScore[i] = 70;
				}
			}
		}
	}
	else if (m_cbPlayerNum == 3 && cbMaxScoreNum == 2) // ��
	{
		for (BYTE i = 0; i < m_cbPlayerNum; i++)
		{
			if (i == wChairId)
			{
				continue;
			}
			if (m_bGameBaseScore == 1) fHuScore[i] = 10;
			else if (m_bGameBaseScore == 2) fHuScore[i] = 30;
			else fHuScore[i] = 50;
		}
	}
	if (nBaoPlayer != -1)
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += fHuScore[i];
				ChiHu.lHuPaiScore[nBaoPlayer] -= fHuScore[i];
			}
		}
		ChiHu.lHuPaiScore[nBaoPlayer] |= CHR_BAO_HU;
	}
	else 
	{
		for(int i = 0; i < m_cbPlayerNum; i++)
		{
			if(i != wChairId)
			{
				ChiHu.lHuPaiScore[wChairId] += fHuScore[i];
				ChiHu.lHuPaiScore[i] -= fHuScore[i];
			}
		}
	}

	//���ñ���
	m_wProvider[wChairId] = m_wProvideUser;
	m_wWinOrder[m_cbWinCount++] = wChairId;
	m_bGangStatus = false;
	m_bGangHuaStatus = false;
	m_bGangOutStatus = false;
	//������Ϣ		
	ChiHu.wChiHuUser = wChairId;
	ChiHu.wProviderUser = m_wProvideUser;
	ChiHu.cbChiHuCard = m_cbProvideCard;

	DWORD wChiHuRight[1] = {0};
	m_ChiHuRight[wChairId].GetRightData( &wChiHuRight[0], MAX_RIGHT_COUNT);
	ChiHu.dwChiHuRight = wChiHuRight[0];
	ChiHu.cbGenCount = m_cbGenCount[wChairId];
	if (nBaoPlayer != -1)
	{
		ChiHu.wBaoPlayer = nBaoPlayer;
	}
	else
		ChiHu.wBaoPlayer = 10;
	for (int i=0;i<ChiHu.lMenHun.size();i++)
	{
		ChiHu.lMenHun[i]=m_bOpenMouth[i];
	}	
	for (int i=0;i<ChiHu.lPiao.size();i++)
	{
		ChiHu.lPiao[i]=m_cbPiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lDang.size();i++)
	{
		ChiHu.lDang[i]=m_cbLaiGenCount[i];
	}	
	for (int i=0;i<ChiHu.lGang.size();i++)
	{
		ChiHu.lGang[i]=m_cbGenCount[i];
	}
	for (int i=0;i<ChiHu.lMingGang.size();i++)
	{
		ChiHu.lMingGang[i]=m_lOtherGangCount[i];
	}	
	for (int i=0;i<ChiHu.lAnGang.size();i++)
	{
		ChiHu.lAnGang[i]=m_lSelfGangCount[i];
	}		
	for (int i=0;i<ChiHu.lXiGang.size();i++)
	{
		ChiHu.lXiGang[i]=m_nFanShu[i];
	}	

	datastream kDataStream;
	ChiHu.StreamValue(kDataStream,true);

	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_CHI_HU,&kDataStream[0],kDataStream.size() );

	m_kChiHuInfoList.push_back(ChiHu);
	addGameRecordAction(ChiHu);
	//���ñ���

	m_bHasHuPai[wChairId] = true;
	{
		m_bPlayStatus[wChairId] = false;
	}
}
//����㷬
void CTableFrameSink::AccountFanShuMC(WORD wChairId, CChiHuRight &chr, BYTE nPlayerFanShu[GAME_PLAYER])
{
	ZeroMemory(m_cbGenCount, sizeof(m_cbGenCount));
	ZeroMemory(m_cbPiGenCount, sizeof(m_cbPiGenCount));	
	ZeroMemory(m_cbLaiGenCount, sizeof(m_cbLaiGenCount));
	ZeroMemory(m_nFanShu, sizeof(m_nFanShu));
	ZeroMemory(m_lOtherGangCount, sizeof(m_lOtherGangCount));
	ZeroMemory(m_lSelfGangCount, sizeof(m_lSelfGangCount));
	m_bNeedCalGang = true;

	//������Ʒ��ķ���  ���� ����  Ƥ��  ���
	for(int i = 0; i < m_cbWeaveItemCount[wChairId]; i++)
	{
		if(m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG)
		{
			if(m_WeaveItemArray[wChairId][i].cbPublicCard == 1)
			{
				m_cbGenCount[wChairId]++;
				if(m_WeaveItemArray[wChairId][i].cbCenterCard == m_cbProvideCard)
				{
					if(m_WeaveItemArray[wChairId][i].wProvideUser != wChairId)
					{
						continue;
					}
				}
				m_lOtherGangCount[wChairId]++;
			}
			else
			{
				m_cbGenCount[wChairId] += 2;
				m_lSelfGangCount[wChairId]++;
			}
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_LAIPI) 
		{
			m_cbPiGenCount[wChairId] += 1;
		}
		else if( m_WeaveItemArray[wChairId][i].cbWeaveKind == WIK_GANG_HUN) 
		{
			m_cbLaiGenCount[wChairId] += 2;
		}
	}
	//���ϻ�
	if( m_wCurrentUser==wChairId && m_bGangStatus )
	{
		chr |= CHR_GANG_KAI;
		chr |= CHR_ZI_MO;
		nPlayerFanShu[wChairId]++;
	}
	if (m_wProvideUser == wChairId)  // ����
	{
		chr |= CHR_ZI_MO;
	}
	else // ����
	{
		chr |= CHR_FANG_PAO;
	}
	if (!(chr & CHR_NONE).IsEmpty())  //Ӳ�� 
	{
		nPlayerFanShu[wChairId]++;
	}
	if (!(chr & CHR_PENGPENGHU).IsEmpty() && !(chr & CHR_QING_YI_SE).IsEmpty())
	{
		nPlayerFanShu[wChairId]++;
	}
	nPlayerFanShu[wChairId] += (m_cbGenCount[wChairId] + m_cbPiGenCount[wChairId] + m_cbLaiGenCount[wChairId]); // �����ܷ���

	//�����мҷ��� 
	for(int i = 0; i < m_cbPlayerNum; i++)
	{
		if(i == wChairId) continue;
		if(m_bOpenMouth[i]) nPlayerFanShu[i]++;
		for(int j = 0; j < m_cbWeaveItemCount[i]; j++)
		{
			if(m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG) 
			{
				if( m_WeaveItemArray[i][j].cbPublicCard == 1) 
				{
					m_cbGenCount[i]++;
					if(m_WeaveItemArray[i][j].cbCenterCard == m_cbProvideCard)
					{
						if(m_WeaveItemArray[i][j].wProvideUser != wChairId)
						{
							continue;
						}
					}
					m_lOtherGangCount[i]++;
				}
				else
				{
					m_cbGenCount[i] += 2;
					m_lSelfGangCount[i] ++;
				}
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_PI ||  m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_LAIPI) 
			{
				m_cbPiGenCount[i] += 1;
			}
			if( m_WeaveItemArray[i][j].cbWeaveKind == WIK_GANG_HUN)  //�����2��
			{
				m_cbLaiGenCount[i] += 2;
			}
		}
		nPlayerFanShu[i] += (m_cbGenCount[i] + m_cbPiGenCount[i] + m_cbLaiGenCount[i]); // �м��ܷ���
	}
}

void CTableFrameSink::OnUserHuanPaiSiChuan(WORD wChairID, GDMJ_CMD_C_HuanPai* pHuanPai)
{
	BYTE mastKindCard[3];
	ZeroMemory(mastKindCard,sizeof(mastKindCard));
	memcpy(mastKindCard,pHuanPai->cbHuanCard,sizeof(mastKindCard));

	for (int n=0;n<3;n++)
	{
		if (!m_GameLogic.IsValidCard(mastKindCard[n]))
		{
			continue;
		}
		for (int i=0;i<m_cbLeftCardCount;i++)
		{
			int nColor = (((BYTE)m_cbRepertoryCard[i]&MASK_COLOR)>>4)+1;
			if (nColor == pHuanPai->nMasttKindColor )
			{
				BYTE nCardTemp = (BYTE)m_cbRepertoryCard[i];
				m_cbRepertoryCard[i] =mastKindCard[n];
				mastKindCard[n] = nCardTemp;
				break;
			}
		}
	}

	m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],pHuanPai->cbHuanCard,3);
	m_GameLogic.AddCard(m_cbCardIndex[wChairID],mastKindCard,3);
	int iCout = m_GameLogic.GetCardCount(m_cbCardIndex[wChairID]);

	GDMJ_CMD_S_HuanPai kInfo;
	ZeroMemory(kInfo.cbHuanCard,sizeof(kInfo.cbHuanCard));
	kInfo.wChairId = wChairID;
	memcpy(kInfo.cbHuanCard,mastKindCard,sizeof(mastKindCard));

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HUANPAI,&kInfo,sizeof(kInfo));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HUANPAI,&kInfo,sizeof(kInfo));

}
void CTableFrameSink::OnUserHuanPaiChengDu(WORD wChairID,GDMJ_CMD_C_HuanPai* pHuanPai)
{
	memcpy(m_cbHuanPaiData[wChairID],pHuanPai->cbHuanCard,sizeof(m_cbHuanPaiData[wChairID]));
}

void CTableFrameSink::OnUserHuanPaiEnd_SiChuan()
{
	for (int m = 0;m<GAME_PLAYER;m++)
	{
		if (!m_HuanPai[m])
		{
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.SwitchToCardData(m_cbCardIndex[m],cbCardData);
			std::vector<GDMJ_Card_Info> kCardInfoList;
			m_GameLogic.GetCardInfoList(cbCardData,MAX_COUNT-1,kCardInfoList);

			GDMJ_CMD_C_HuanPai kNetInfo;
			int iIdex = 0;
			for (int i = 0;i<kCardInfoList[0].nLen && iIdex < 3;i++)
			{
				kNetInfo.cbHuanCard[iIdex]=kCardInfoList[0].kCardList[i];
				iIdex++;
			}
			for (int i = 0;i<kCardInfoList[1].nLen && iIdex < 3;i++)
			{
				kNetInfo.cbHuanCard[iIdex]=kCardInfoList[1].kCardList[i];
				iIdex++;
			}
			for (int i = 0;i<kCardInfoList[2].nLen && iIdex < 3;i++)
			{
				kNetInfo.cbHuanCard[iIdex]=kCardInfoList[2].kCardList[i];
				iIdex++;
			}
			kNetInfo.nMasttKindColor = kCardInfoList[2].nColor;
			OnUserHuanPaiSiChuan(m,&kNetInfo);
			memcpy(m_cbHuanPaiData[m],cbCardData,sizeof(m_cbHuanPaiData[m]));
		}
	}
	sendXuanQueNotice();
}

void CTableFrameSink::OnUserHuanPaiEnd_ChengDu()
{
	for (int m = 0;m<GAME_PLAYER;m++)
	{
		if (!m_HuanPai[m])
		{
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.SwitchToCardData(m_cbCardIndex[m],cbCardData);
			std::vector<GDMJ_Card_Info> kCardInfoList;
			m_GameLogic.GetCardInfoList(cbCardData,MAX_COUNT-1,kCardInfoList);

			int nLen1 = kCardInfoList[0].nLen;			//���ٻ�ɫ
			int nLen2 = kCardInfoList[1].nLen;			//�м仨ɫ
			int nLen3 = kCardInfoList[2].nLen;			//��໨ɫ

			BYTE				         m_pHuanCards[3];
			GDMJ_CMD_C_HuanPai kNetInfo;
			int iIdex = 0;
			if (nLen1>=3)
			{
				m_pHuanCards[0]=kCardInfoList[0].kCardList[0];
				m_pHuanCards[1]=kCardInfoList[0].kCardList[1];
				m_pHuanCards[2]=kCardInfoList[0].kCardList[2];
			}
			else if (nLen2>=3)
			{
				m_pHuanCards[0]=kCardInfoList[1].kCardList[0];
				m_pHuanCards[1]=kCardInfoList[1].kCardList[1];
				m_pHuanCards[2]=kCardInfoList[1].kCardList[2];
			}
			else if (nLen3>=3)
			{
				m_pHuanCards[0]=kCardInfoList[2].kCardList[0];
				m_pHuanCards[1]=kCardInfoList[2].kCardList[1];
				m_pHuanCards[2]=kCardInfoList[2].kCardList[2];
			}
			kNetInfo.nMasttKindColor = kCardInfoList[2].nColor;
			OnUserHuanPaiChengDu(m,&kNetInfo);
			memcpy(m_cbHuanPaiData[m],m_pHuanCards,sizeof(m_cbHuanPaiData[m]));
		}
	}
	int nNum1 = g_RandKiss.RandKiss()%6+1;
	int nNum2 = g_RandKiss.RandKiss()%6+1;

	WORD wSiceNum = MAKEWORD(nNum1,nNum2);

	int nModel = (nNum1 + nNum2)%GAME_PLAYER;

	GDMJ_CMD_S_HuanPaiChengDu kInfo;
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.RemoveCard(m_cbCardIndex[i],m_cbHuanPaiData[i],3);
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		ZeroMemory(&kInfo,sizeof(kInfo));
		kInfo.wChairId = i;
		kInfo.wSice = wSiceNum;
		memcpy(kInfo.cbRemoveHuanCard,m_cbHuanPaiData[i],sizeof(m_cbHuanPaiData[i]));
		if (nModel == 0)
		{
			//4��8��12 ˳ʱ�뻻
			int iIdex = (i-1+GAME_PLAYER)%GAME_PLAYER;
			m_GameLogic.AddCard(m_cbCardIndex[i],m_cbHuanPaiData[iIdex],3);
			memcpy(kInfo.cbHuanCard,m_cbHuanPaiData[iIdex],sizeof(m_cbHuanPaiData[iIdex]));
		}
		else if (nModel == 2)
		{
			//2��6��10 ��ʱ�뻻	
			int iIdex = (i+1+GAME_PLAYER)%GAME_PLAYER;
			m_GameLogic.AddCard(m_cbCardIndex[i],m_cbHuanPaiData[iIdex],3);
			memcpy(kInfo.cbHuanCard,m_cbHuanPaiData[iIdex],sizeof(m_cbHuanPaiData[iIdex]));

		}else
		{
			//3��5��7��9��11�Լһ���
			int iIdex = (i+2+GAME_PLAYER)%GAME_PLAYER;
			m_GameLogic.AddCard(m_cbCardIndex[i],m_cbHuanPaiData[iIdex],3);
			memcpy(kInfo.cbHuanCard,m_cbHuanPaiData[iIdex],sizeof(m_cbHuanPaiData[iIdex]));
		}

		if(!m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			m_pITableFrame->SendTableData(i,SUB_S_HUANPAI_CHENGDU,&kInfo,sizeof(kInfo));
			m_pITableFrame->SendLookonData(i,SUB_S_HUANPAI_CHENGDU,&kInfo,sizeof(kInfo));
		}
	}
	sendXuanQueNotice();
}

BYTE CTableFrameSink::GetQueColor( WORD wChairID )
{
	BYTE cbCardData[MAX_COUNT-1];
	ZeroMemory(cbCardData,sizeof(cbCardData));
	m_GameLogic.SwitchToCardData(m_cbCardIndex[wChairID],cbCardData);


	std::vector<GDMJ_Card_Info> kCardInfoList;
	m_GameLogic.GetCardInfoList(cbCardData,MAX_COUNT-1,kCardInfoList);

	ASSERT(kCardInfoList.size() == CARD_COLOR_TIAO); //��༸�ֻ�ɫ

	return kCardInfoList[0].nColor;
}

// todo
void CTableFrameSink::gameStart()
{
	
}

void CTableFrameSink::getScoreByMa(WORD wchairID)
{
	
}

BYTE CTableFrameSink::getZiMianMaScore(BYTE card, WORD wchairID)
{
	BYTE score = 0;
	switch (card)
	{
	case 0x35:
		score = 1;
		break;
	case 0x36:
		score = 2;
		break;
	case 0x37:
		score = 3;
		break;
	default:
		score = card & 0x0F;
		break;
	}

	return score;
}

BYTE CTableFrameSink::getGuDingMaScore(BYTE card, WORD wchairID)
{
	BYTE score = 0;
	int huIdx = (4 + wchairID - m_wBankerUser) % 4;
	switch (huIdx)
	{
	case 0:
		{
			//if((card & 0x01) == 0x01 || (card & 0x05) == 0x05 || (card & 0x09) == 0x09)
			if(card == 0x01 || card == 0x11 || card == 0x21 || card == 0x31 || card == 0x05 || card == 0x15 || card == 0x25 || card == 0x35 || card == 0x09 || card == 0x19 || card == 0x29)
				score++;
		}
		break;

	case 1:
		{
			//if((card & 0x02) == 0x02 || (card & 0x06) == 0x06)
			if(card == 0x02 || card == 0x12 || card == 0x22 || card == 0x32 || card == 0x06 || card == 0x16 || card == 0x26 || card == 0x36)
				score++;
		}
		break;

	case 2:
		{
			//if((card & 0x03) == 0x03 || (card & 0x07) == 0x07)
			if(card == 0x03 || card == 0x13 || card == 0x23 || card == 0x33 || card == 0x07 || card == 0x17 || card == 0x27 || card == 0x37)
				score++;
		}
		break;

	case 3:
		{
			//if((card & 0x04) == 0x04 || (card & 0x08) == 0x08)
			if(card == 0x04 || card == 0x14 || card == 0x24 || card == 0x34 || card == 0x08 || card == 0x18 || card == 0x28)
				score++;
		}
		break;

	default:
		break;
	}
	return score;
}

void CTableFrameSink::setCurrentHuPlayer(BYTE wChairID)
{
	if(m_vHuPlayers.size() == 0)
		m_vHuPlayers.push_back(wChairID);
	else
	{
		std::vector<BYTE>::iterator it = m_vHuPlayers.end()-1;
		BYTE lastID = *it;
		if(lastID == wChairID)
		{
			m_vHuPlayers.push_back(wChairID);
		}
		else
		{
			m_vHuPlayers.clear();
			m_vHuPlayers.push_back(wChairID);
		}
	}
}
int CTableFrameSink::getLiuJuCount()
{
	//edit by wld
	//��ʯʣ��4��
	
	if (m_bGameTypeIndex == 3)// ����ʣ��24��
	{
		return 24;
	}
	return 10;
// 	if(m_bGameTypeHun==2)
// 	{
// 		return 1;
// 	}
// 	return 0;
// 
// 	if(hasRule(GAME_RULE_ZIMIANMA))
// 	{
// 		return 1;
// 	}
// 	else
// 	{
// 		int ma_count = GudingMaArray[m_bGameGuDingMa];
// 		int bubugao = (m_vHuPlayers.size()) * m_bGameBuBuGao;
// 		if(bubugao > 0)
// 			ma_count += bubugao;
// 
// 		return ma_count;
// 	}
}

// ���Ӣɽ��ȫ����
bool CTableFrameSink::bCheckAllOther(const BYTE cbCardIndex[MAX_INDEX])
{
	if (m_GameLogic.GetCardCount(cbCardIndex) != 2) return false;
	if (cbCardIndex[1] == 0 && cbCardIndex[4] == 0 && cbCardIndex[7] == 0 && cbCardIndex[10] == 0 && cbCardIndex[13] == 0 && 
		cbCardIndex[16] == 0 && cbCardIndex[19] == 0 && cbCardIndex[22] == 0 && cbCardIndex[25] == 0) return false;
	return true;
}
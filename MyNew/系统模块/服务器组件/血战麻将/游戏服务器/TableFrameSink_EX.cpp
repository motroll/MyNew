#include "StdAfx.h"
#include "TableFrameSink.h"
#include "FvMask.h"
#include "DlgCustomRule.h"
#include "Randkiss.h"
extern CRandKiss g_RandKiss;


bool CTableFrameSink::hasRule(DWORD cbRule)
{
	return FvMask::HasAny(m_bGameRuleIdex,_MASK_(cbRule));
}

void CTableFrameSink::sendHuanPaiNotice()
{
	BYTE kTemp = 0;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HUANPAI_NOTICE,&kTemp,1);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HUANPAI_NOTICE,&kTemp,1);

	if (m_pGameServiceOption->wServerType != GAME_GENRE_EDUCATE)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_HUANPAI,(TIME_HUANPAI)*1000,1,0);
	}
	m_pITableFrame->SetGameStatus(GS_MJ_HUANPAI);
}

void CTableFrameSink::sendXuanQueNotice()
{
	if (!hasRule(GAME_RULE_DIN_QUE))
	{
		startPlay();
		return;
	}
	m_pITableFrame->SetGameStatus(GS_MJ_XUANQUE);
	for (int i=0;i<GAME_PLAYER;i++)
	{
		GDMJ_CMD_S_XuanQueNotice kXuanQueNotice;

		kXuanQueNotice.wChairId = i;
		kXuanQueNotice.nLeftTime = TIME_XUANQUE;

		m_nQueColor[i] = GetQueColor(i);
		kXuanQueNotice.nQueColor = m_nQueColor[i];
		m_nQueColor[i] = 0;

		if(m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
		{
			GDMJ_CMD_AndroidCardIndex  kAndroidInfo;
			ZeroMemory(&kAndroidInfo,sizeof(kAndroidInfo));
			memcpy(kAndroidInfo.cbCardIndex,m_cbCardIndex,sizeof(m_cbCardIndex));
			m_pITableFrame->SendTableData(i,SUB_S_HUANPAI_ANDROID,&kAndroidInfo,sizeof(kAndroidInfo));
			m_pITableFrame->SendLookonData(i,SUB_S_HUANPAI_ANDROID,&kAndroidInfo,sizeof(kAndroidInfo));
		}

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_XUANQUE_NOTICE,&kXuanQueNotice,sizeof(kXuanQueNotice));
		m_pITableFrame->SendLookonData(i,SUB_S_XUANQUE_NOTICE,&kXuanQueNotice,sizeof(kXuanQueNotice));
	}

	m_pITableFrame->KillGameTimer(IDI_TIMER_HUANPAI);

	if (m_pGameServiceOption->wServerType != GAME_GENRE_EDUCATE)
	{
		m_pITableFrame->SetGameTimer(IDI_TIMER_XUANQUE,(TIME_XUANQUE+2)*1000,1,0);
	}
}
void CTableFrameSink::sendDingZhuang()
{
	m_pITableFrame->SetGameStatus(GS_MJ_DINGZHUANG);

	int nNum1 = g_RandKiss.RandKiss()%6+1;
	int nNum2 = g_RandKiss.RandKiss()%6+1;
	WORD wSiceNum = MAKEWORD(nNum1,nNum2);

	GDMJ_CMD_S_DingZhuang rollData;

	memset(&rollData,0,sizeof(rollData));
	rollData.wBankerUser = m_wBankerUser;
	rollData.wSice = wSiceNum;	

	m_pITableFrame->SendGameStatus(&rollData, sizeof(rollData));
	int banker = (nNum1 + nNum2 - 1) % m_cbPlayerNum;
	setZhuangID(banker);
	m_isDingZhuang = true;

}

void CTableFrameSink::sendRollDice()
{
	m_pITableFrame->SetGameStatus(GS_MJ_ROLLDICE);
// 
// 	int nNum1 = g_RandKiss.RandKiss()%6+1;
// 	int nNum2 = g_RandKiss.RandKiss()%6+1;
	WORD wSiceNum = MAKELONG(MAKEWORD(g_RandKiss.RandKiss()%6+1,g_RandKiss.RandKiss()%6+1),MAKEWORD(g_RandKiss.RandKiss()%6+1,g_RandKiss.RandKiss()%6+1));
	GDMJ_CMD_S_DingZhuang rollData;
	m_lSiceCount = wSiceNum;
	memset(&rollData,0,sizeof(rollData));
	rollData.wBankerUser = m_wBankerUserNext;
	rollData.wSice = wSiceNum;
	m_pITableFrame->SendGameStatus(&rollData, sizeof(rollData));

	m_isRollDice = true;
}

void CTableFrameSink::startPlay()
{
	if(hasRule(GAME_RULE_DIN_QUE))
	{
		if (m_pGameServiceOption->wServerType != GAME_GENRE_EDUCATE)
		{
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if (m_nQueColor[i] == 0)
				{
					m_nQueColor[i] = GetQueColor(i);
				}
			}
		}
		GDMJ_CMD_S_XuanQue kXuanQue;
		ZeroMemory(kXuanQue.nQueColor,sizeof(kXuanQue.nQueColor));
		memcpy(kXuanQue.nQueColor,m_nQueColor,sizeof(m_nQueColor));

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_XUANQUE,&kXuanQue,sizeof(kXuanQue));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_XUANQUE,&kXuanQue,sizeof(kXuanQue));
		m_pITableFrame->KillGameTimer(IDI_TIMER_XUANQUE);
	}
	m_pITableFrame->KillGameTimer(IDI_TIMER_HUANPAI);
	m_pITableFrame->KillGameTimer(IDI_TIMER_XUANQUE);
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);
	starGameRecord();
	DispatchCardData(m_wBankerUser);
}	

bool CTableFrameSink::cheakSelfGangPi(int nIsLaiPi,WORD wChairID,BYTE cbOperateCard,bool& bPublic)
{
	//��������
	BYTE cbWeaveIndex=0xFF;
	BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

	
	//�˿�Ч��
	if (nIsLaiPi == 1)
	{
		if (cbCardIndex!=m_nPiIndex )
		{ 
			return false;
		}
	}
	if (nIsLaiPi == 2)
	{
		if (cbCardIndex!=m_nLaiPiIndex)
		{ 
			return false;
		}
	}
	if (nIsLaiPi == 3)
	{
		if (cbCardIndex!=m_nHunIndex)
		{ 
			return false;
		}
	}
	
	if (m_cbCardIndex[wChairID][cbCardIndex]<=0) 
		return false;

	//���ñ���
	bPublic=false;


	cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
	m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
	m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
	if (nIsLaiPi == 1)
	{
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_GANG_PI;
	}
	if (nIsLaiPi == 2)
	{
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_GANG_LAIPI;
	}
	if (nIsLaiPi == 3)
	{
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_GANG_HUN;
	}
	m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;	

	//ɾ���˿�
	m_cbCardIndex[wChairID][cbCardIndex]-=1;
	return true;
}
bool CTableFrameSink::cheakSelfGang(WORD wChairID,BYTE cbOperateCard,bool& bPublic)
{
	//��������
	BYTE cbWeaveIndex=0xFF;
	BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

	//���ƴ���
	if (m_cbCardIndex[wChairID][cbCardIndex]==1)
	{
		//Ѱ�����
		for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
		{
			DWORD cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				bPublic=true;
				cbWeaveIndex=i;
				break;
			}
		}

		//Ч�鶯��
		ASSERT(cbWeaveIndex!=0xFF);
		if (cbWeaveIndex==0xFF) return false;

		//����˿�
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_GANG;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
		//m_WeaveItemArray[wChairID][cbWeaveIndex].cbIsSelfGang = 1;
	}
	else
	{
		//�˿�Ч��
		if(cbCardIndex == m_nPiIndex && m_bGameTypeIndex !=3)
		{
			ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==3);
			if (m_cbCardIndex[wChairID][cbCardIndex]!=3) 
				return false;
		}
		else
		{
			ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
			if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
				return false;
		}

		//���ñ���
		bPublic=false;
		cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
		m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=WIK_GANG;
		m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
	}

	//ɾ���˿�
	m_cbCardIndex[wChairID][cbCardIndex]=0;
	return true;
}

void CTableFrameSink::sendGangScoreReal(SCORE lScore,WORD wTargetID,WORD wProvideID,bool bAn)
{
	BYTE cbGangIndex = m_GangScore[wTargetID].cbGangCount++;
	//������Ϣ

	GDMJ_CMD_S_GangScore gs;
	gs.resizeData();
	gs.wChairId = wTargetID;
	gs.wPravadeId = wProvideID;

	
	int nTempScore = 0;

	if(bAn)
	{
		gs.cbXiaYu = bAn;
		nTempScore = lScore*2;
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			int nDang = 1;
			if( !m_bPlayStatus[i] || i == wTargetID ) continue;

			if(m_bPlayDang[i] == 1)
			{
				nDang = 2;
			}
			if(m_bPlayDang[wTargetID] == 1)
			{
				nDang = 2;
			}

			m_nGangScoreNow[i]  -= (nTempScore*nDang);
			m_nGangScoreNow[wTargetID] += nTempScore*nDang;

			gs.lGangScore[i]  -= (nTempScore*nDang);
			gs.lGangScore[wTargetID] += nTempScore*nDang;
		}

		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			int nDang = 1;
			if( !m_bPlayStatus[i] || i == wTargetID ) continue;

			if(m_bPlayDang[i] == 1)
			{
				nDang = 2;
			}
			if(m_bPlayDang[wTargetID] == 1)
			{
				nDang = 2;
			}
			m_GangScore[wTargetID].lScore[cbGangIndex][i] -= nTempScore*nDang;
			m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += nTempScore*nDang;
		}

		m_GangScore[wTargetID].cbGangCount_Single +=2;
		m_GangScore[wTargetID].bMingGang = false;
	}
	else if((wTargetID == wProvideID && !bAn))
	{
		gs.cbXiaYu = bAn;
		nTempScore = lScore;

		
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			int nDang = 1;
			if( !m_bPlayStatus[i] || i == wTargetID ) continue;

			if(m_bPlayDang[i] == 1)
			{
				nDang = 2;
			}
			if(m_bPlayDang[wTargetID] == 1)
			{
				nDang = 2;
			}

			gs.lGangScore[i] -= nTempScore*nDang;
			gs.lGangScore[wTargetID] += nTempScore*nDang;

			m_nGangScoreNow[i]  -= (nTempScore*nDang);
			m_nGangScoreNow[wTargetID] += nTempScore*nDang;
		}

		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			int nDang = 1;
			if( !m_bPlayStatus[i] || i == wTargetID ) continue;

			if(m_bPlayDang[i] == 1)
			{
				nDang = 2;
			}
			if(m_bPlayDang[wTargetID] == 1)
			{
				nDang = 2;
			}

			m_GangScore[wTargetID].lScore[cbGangIndex][i] -= nTempScore*nDang;
			m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += nTempScore*nDang;
		}

		{
			m_GangScore[wTargetID].cbGangCount_Single++;
			m_GangScore[wTargetID].bMingGang = true;
		}
	}
	else
	{
		nTempScore = 3*lScore;
		gs.wChairId = wTargetID;
		gs.cbXiaYu = FALSE;

		int nDang = 1;
		if(m_bPlayDang[wProvideID] == 1)
		{
			nDang = 2;
		}
		if(m_bPlayDang[wTargetID] == 1)
		{
			nDang = 2;
		}

		gs.lGangScore[wTargetID] += nTempScore*nDang;
		gs.lGangScore[wProvideID] -= nTempScore*nDang;

		m_nGangScoreNow[wTargetID]  += (nTempScore*nDang);
		m_nGangScoreNow[wTargetID] -= nTempScore*nDang;

		m_GangScore[wTargetID].lScore[cbGangIndex][wTargetID] += nTempScore*nDang;
		m_GangScore[wTargetID].lScore[cbGangIndex][wProvideID] -= nTempScore*nDang;

		m_GangScore[wTargetID].cbGangCount_Single++;
		m_GangScore[wTargetID].bMingGang = true;
	}

	datastream kDataStream;
	gs.StreamValue(kDataStream,true);

	for(int i = 0; i < GAME_PLAYER; i++)
		m_pITableFrame->SendTableData( i,SUB_S_GANG_SCORE,&kDataStream[0],kDataStream.size() );


	m_kGangInfoList.push_back(gs);

}

void CTableFrameSink::sendGangScore(SCORE lScore,WORD wTargetID,WORD wProvideID,bool bAn /*= false*/)
{
	BYTE cbGangIndex = m_GangScore[wTargetID].cbGangCount++;
	//������Ϣ
	
	GDMJ_CMD_S_GangScore gs;
	gs.resizeData();
	gs.wChairId = wTargetID;
	gs.wPravadeId = wProvideID;

	
	int nTempScore = 0;
	lScore = m_nGangScore;

	gs.lGangScore[wTargetID] += nTempScore;

	if(bAn)
	{
		gs.cbXiaYu = bAn;		
		m_GangScore[wTargetID].cbGangCount_Single +=1;
		m_GangScore[wTargetID].bMingGang = false;
	}
	else if((wTargetID == wProvideID && !bAn))
	{
		gs.cbXiaYu = bAn;
		m_GangScore[wTargetID].cbGangCount_Single++;
		m_GangScore[wTargetID].bMingGang = true;
	}
	else
	{		
		m_GangScore[wTargetID].cbGangCount_Single++;
		m_GangScore[wTargetID].bMingGang = true;
	}

	datastream kDataStream;
	gs.StreamValue(kDataStream,true);

	for(int i = 0; i < GAME_PLAYER; i++)
		m_pITableFrame->SendTableData( i,SUB_S_GANG_SCORE,&kDataStream[0],kDataStream.size() );


	m_kGangInfoList.push_back(gs);
	
}

void CTableFrameSink::SendOperateResulet(WORD wChairID,WORD wProvideID,DWORD cbOperateCode, BYTE cbOperateCard, DWORD dGangSiceNum)
{
	//������
	GDMJ_CMD_S_OperateResult OperateResult;
	OperateResult.wOperateUser=wChairID;
	OperateResult.wProvideUser=wProvideID;
	OperateResult.cbOperateCode=cbOperateCode;
	OperateResult.cbOperateCard=cbOperateCard;
	OperateResult.wGangSiceNum = dGangSiceNum;
	addGameRecordAction(OperateResult);
	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
}

//���ر�����ť��֪ͨ
void  CTableFrameSink::SendHideQfjNotice(WORD  wChairID)
{
	GDMJ_CMD_S_HideQFJNotice     HideQFJNotice;
	memset(&HideQFJNotice,0,sizeof(GDMJ_CMD_S_HideQFJNotice));
	HideQFJNotice.wPeratorUser =  wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HIDEQFJ_NOTICE,&HideQFJNotice,sizeof(HideQFJNotice));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HIDEQFJ_NOTICE,&HideQFJNotice,sizeof(HideQFJNotice));
}

bool CTableFrameSink::removeCardByAction(WORD wTargetUser,DWORD cbAction,BYTE cbTargetCard)
{
	//ɾ���˿�
	switch (cbAction)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_LEFT,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );
			if (m_cbDiscardCount>0)
			{
				m_cbDiscardCount[m_wProvideUser]--;
			}
			break;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_RIGHT,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );
			if (m_cbDiscardCount>0)
			{
				m_cbDiscardCount[m_wProvideUser]--;
			}
			break;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[3];
			m_GameLogic.GetWeaveCard(WIK_CENTER,cbTargetCard,cbRemoveCard);
			VERIFY( m_GameLogic.RemoveCard(cbRemoveCard,3,&cbTargetCard,1) );
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );
			if (m_cbDiscardCount>0)
			{
				m_cbDiscardCount[m_wProvideUser]--;
			}
			break;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) );
			if (m_cbDiscardCount>0)
			{
				m_cbDiscardCount[m_wProvideUser]--;
			}
//			UpdateTingCardTag(wTargetUser,cbTargetCard,true);
			break;
		}
	case WIK_GANG:		//���Ʋ���
		{
			//ɾ���˿�,��������ֻ���ڷŸ�
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
			VERIFY( m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) );
			if (m_cbDiscardCount>0)
			{
				m_cbDiscardCount[m_wProvideUser]--;
			}
			break;

		}
	default:
		ASSERT( FALSE );
		return false;
	}
	return true;
}

int CTableFrameSink::FloorScore5(int nScore)
{
	int ret = 5;
	if (nScore <= 5 )
	{
		ret = 5;
	}
	else if (nScore <= 10 )
	{
		ret = 10;
	}
	else if (nScore <= 15 )
	{
		ret = 15;
	}
	else if (nScore <= 20 )
	{
		ret = 20;
	}
	else if (nScore <= 25 )
	{
		ret = 25;
	}
	else if (nScore <= 30 )
	{
		ret = 30;
	}
	else if (nScore <= 35 )
	{
		ret = 35;
	}
	else if (nScore <= 40 )
	{
		ret = 40;
	}
	else if (nScore <= 45 )
	{
		ret = 45;
	}
	else if (nScore <= 50 )
	{
		ret = 50;
	}
	else if (nScore <= 55 )
	{
		ret = 55;
	}
	else if (nScore <= 60 )
	{
		ret = 60;
	}
	else if (nScore <= 65 )
	{
		ret = 65;
	}
	else
	{
		ret = m_bGameMaxScore;
	}
	if (ret > m_bGameMaxScore)
	{
		ret = m_bGameMaxScore;
	}
	return ret;
}

int CTableFrameSink::FloorScore10(int nScore)
{
	int ret = 10;
	if (nScore <= 10 )
	{
		ret = 10;
	}
	else if (nScore <= 20 )
	{
		ret = 20;
	}
	else if (nScore <= 30 )
	{
		ret = 30;
	}
	else if (nScore <= 40 )
	{
		ret = 40;
	}
	else if (nScore <= 50 )
	{
		ret = 50;
	}
	else if (nScore <= 60 )
	{
		ret = 60;
	}
	else
	{
		ret = m_bGameMaxScore;
	}
	if (ret > m_bGameMaxScore )
	{
		ret = m_bGameMaxScore;
	}
	return ret;
}

int CTableFrameSink::FloorScore(int nScore)
{
	if (m_bGameMaxScore <= 20)
	{
		if (nScore > m_bGameMaxScore)
		{
			nScore = m_bGameMaxScore;
		}
		return nScore ;
	}
	else if (m_bGameMaxScore <= 30)
	{
		return FloorScore5(nScore);
	}
	else 
	{
		return FloorScore10(nScore);
	}
}
void CTableFrameSink::getHuScore(std::vector<LONGLONG> * pVec,int nScore,int nBaseFan,WORD wChairId,WORD ProvideUser,bool bIsDahu,int nBaoPlayer, int &iHuScore)
{
	if (nBaoPlayer<-1)
	{

	}
	int nTempScore =0;
	if( ProvideUser == wChairId || bIsDahu )
	{
		for( WORD i = 0; i < m_cbPlayerNum; i++ )
		{
			if(!m_bPlayStatus[i] )
			{
				continue;
			}
			if( i == wChairId )
			{
				continue;
			}
			m_nFanShu[i] += nBaseFan;
			m_nFanShu[i] += m_cbPiGenCount[i];
			m_nFanShu[i] += m_cbLaiGenCount[i];
			m_nFanShu[i] += m_cbPiGenCount[wChairId];
			m_nFanShu[i] += m_cbLaiGenCount[wChairId];
			//if ( i == ProvideUser)
			{
				m_nFanShu[i] += m_bOpenMouth[i];
				//nScore *= 2;
			}	
			if (bIsDahu && i == ProvideUser)
			{
				m_nFanShu[i] += 1;
				//nScore *= 2;
			}	
			m_nFanShu[i] += m_cbGenCount[wChairId];
			m_nFanShu[i] += m_cbGenCount[i];

			nTempScore = nScore * (pow(2,m_nFanShu[i]));		
			nTempScore = FloorScore(nTempScore);
			if (nBaoPlayer != -1)
			{
				pVec->at(nBaoPlayer) -=  (nTempScore);
			}
			else
			{
				pVec->at(i) -=  (nTempScore);
			}			
			pVec->at(wChairId) +=  (nTempScore);
			//����������� ���·��ڷ��ķ���   
			if(ProvideUser != wChairId && i == ProvideUser) 
			{
				iHuScore = nTempScore;
			}
			else if(ProvideUser == wChairId)
			{
				iHuScore += nTempScore;
			}
		}
	}
	//����
	else
	{
		m_ChiHuRight[ProvideUser] |= CHR_FANG_PAO;

		m_nFanShu[ProvideUser] += nBaseFan;
		m_nFanShu[ProvideUser] += m_cbPiGenCount[ProvideUser];
		m_nFanShu[ProvideUser] += m_cbLaiGenCount[ProvideUser];
		m_nFanShu[ProvideUser] += m_cbPiGenCount[wChairId];
		m_nFanShu[ProvideUser] += m_cbLaiGenCount[wChairId];
		m_nFanShu[ProvideUser] += m_bOpenMouth[ProvideUser];
		m_nFanShu[ProvideUser] += m_cbGenCount[wChairId];
		m_nFanShu[ProvideUser] += m_cbGenCount[ProvideUser];

		nTempScore = nScore * (pow(2,m_nFanShu[ProvideUser]));	
		nTempScore = FloorScore(nTempScore);
		pVec->at(ProvideUser) -=  (nTempScore);
		pVec->at(wChairId) += (nTempScore);

		iHuScore = nTempScore;

	}
}

//���ݻλ����
void CTableFrameSink::getHuScore(std::vector<LONGLONG> * pVec,int nScore, int nPlayerScore,int nBaseFan,WORD wChairId,WORD ProvideUser,int nBaoPlayer, int &iHuScore)
{
	if (nBaoPlayer < -1)
	{

	}
	int nTempScore = 0;

	for(WORD i = 0; i < m_cbPlayerNum; i++)
	{
		if(!m_bPlayStatus[i])
		{
			continue;
		}
		if(i == wChairId)
		{
			continue;
		}
		m_nFanShu[i] += m_cbGenCount[wChairId];
		m_nFanShu[i] += m_cbGenCount[i];
		if (wChairId == ProvideUser)
		{
			nTempScore = nScore * (pow(2, m_nFanShu[i]));
		}
		else
		{
			if (ProvideUser == i || nPlayerScore == 0)
			{
				nTempScore = nScore * (pow(2, m_nFanShu[i]));
			}
			else
			{
				nTempScore = nPlayerScore * (pow(2, m_nFanShu[i]));
			}
		}
		if(nTempScore > m_bGameMaxScore) nTempScore = m_bGameMaxScore;
		if (nBaoPlayer != -1)
		{
			pVec->at(nBaoPlayer) -= (nTempScore);
			pVec->at(nBaoPlayer) |= CHR_BAO_HU;
		}
		else
		{
			pVec->at(i) -= (nTempScore);
		}
		pVec->at(wChairId) += (nTempScore);
		if(i == ProvideUser && wChairId != ProvideUser)
		{
			iHuScore =  nTempScore;
		}
		else if(wChairId  == ProvideUser)
		{
			iHuScore += nTempScore;
		}
	}
}

void CTableFrameSink::chaJiaoAndHuaZhu(int cbLeftUserCount,LONGLONG lHuaZhuScore[],LONGLONG lChaJiaoScore[],WORD	 lChaJiaoFanShuMax[],LONGLONG lGangScore[])
{
	
}

void CTableFrameSink::chaJiaoAndHuaZhu_YiChang(int cbLeftUserCount,LONGLONG lHuaZhuScore[],LONGLONG lChaJiaoScore[],WORD	 lChaJiaoFanShuMax[],LONGLONG lGangScore[])
{
	
}

DWORD CTableFrameSink::AnalyseMagicInexCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	DWORD ret = 1;
	BYTE cardIndex = m_GameLogic.SwitchToCardIndex(cbCurrentCard);
	if (cbCardIndex[m_nHunIndex] >= 3) // ���첻�ɺ���
	{
		return 0;
	}



	//�����ܣ����������и�3����һ��������

// 	if (cardIndex > 27)
// 	{
// 
// 	}
// 	else
// 	{
// 		int nCurrentCardIndex = cardIndex % 9;
// 		if(nCurrentCardIndex>1)
// 		{
// 			if(cbCardIndex[cardIndex-1]==3)
// 			{
// 				return 0;
// 			}
// 		}
// 		if(nCurrentCardIndex>2)
// 		{
// 			if(cbCardIndex[cardIndex-2]==3)
// 			{
// 				return 0;
// 			}
// 		}
// 		if(nCurrentCardIndex<7)
// 		{
// 			if(cbCardIndex[cardIndex+2]==3)
// 			{
// 				return 0;
// 			}
// 		}
// 		if(nCurrentCardIndex<8)
// 		{
// 			if(cbCardIndex[cardIndex+1]==3)
// 			{
// 				return 0;
// 			}
// 		}
// 	}



	//һ�ŵ���Ҳû�в��ܺ���
	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	BYTE cbMagicCardCount = 0;

	int nMagicCardCount = cbCardIndex[m_nHunIndex];
	
	//����7��
	int nCardCount = 0;
	int nSingleCardCount = 0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbMagicCardIndex[i];
		nCardCount+=cbCardCount;
		//���ƹ���
		if( i == m_nHunIndex ) continue;

		//����ͳ��
		if( cbCardCount == 1 || cbCardCount == 3 ) 	nSingleCardCount++;
	}
	if(nCardCount==13 && nSingleCardCount ==0)
	{
		return 0;
	}

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//�����ж�
		if ((i<(MAX_INDEX-9))&&((i%9)<7))
		{
			if(cbMagicCardIndex[i]!= m_nHunIndex&& cbMagicCardIndex[i+1]!= m_nHunIndex&&cbMagicCardIndex[i+2]!= m_nHunIndex)
			{
				if (cbMagicCardIndex[i] == 1 && cbMagicCardIndex[i + 1] == 1 && cbMagicCardIndex[i + 2] == 1)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
				if(cbMagicCardIndex[i] == 3 && cbMagicCardIndex[i + 1] == 1 && cbMagicCardIndex[i + 2] == 1)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
				if(cbMagicCardIndex[i] == 1 && cbMagicCardIndex[i + 1] == 3 && cbMagicCardIndex[i + 2] == 1)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
				if(cbMagicCardIndex[i] == 1 && cbMagicCardIndex[i + 1] == 1 && cbMagicCardIndex[i + 2] == 3)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
			}

		}
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//ͬ���ж�
		if (i == m_nHunIndex) 
		{
			continue;
		}
		if (cbMagicCardIndex[i]==3)
		{
			cbMagicCardIndex[i] =0;
		}
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//�����ж�
		if ((i<(MAX_INDEX-9))&&((i%9)<7))
		{
			if(cbMagicCardIndex[i]!= m_nHunIndex&& cbMagicCardIndex[i+1]!= m_nHunIndex&&cbMagicCardIndex[i+2]!= m_nHunIndex)
			{
				if (cbMagicCardIndex[i] >0 && cbMagicCardIndex[i + 1] >0 && cbMagicCardIndex[i + 2] >0)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
				if (cbMagicCardIndex[i] >0 && cbMagicCardIndex[i + 1] >0 && cbMagicCardIndex[i + 2] >0)
				{
					cbMagicCardIndex[i] -=1;
					cbMagicCardIndex[i+1] -=1;
					cbMagicCardIndex[i+2] -=1;
				}
			}

		}
	}
	int nKey = 0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//ͬ���ж�
		if (i == m_nHunIndex) 
		{
			continue;
		}
		if(nKey == 1)
		{
			continue;
		}
		if (cbMagicCardIndex[i]==2)
		{
			nKey = 1;
			cbMagicCardIndex[i] =0;
		}
	}
	int nLeftNum = 0;
	int nLeftFeng = 0;
	bool bAllUse = false;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i != m_nHunIndex)
		{
			if(cbMagicCardIndex[i] != 0)
			{
				nLeftNum+=cbMagicCardIndex[i];
				if(i>=27)
				{
					nLeftFeng += cbMagicCardIndex[i];;
				}

			}
		}
	}

	if (nMagicCardCount == 1)
	{
		if (nKey == 1)
		{
			if(nLeftNum <=1&&nLeftFeng == 0)
			{
				bAllUse = true;
			}
		}
		else
		{
			if(nLeftNum <=0)
			{
				bAllUse = true;
			}
		}
	}	

	if (bAllUse)
	{
		return 0;
	}

	 bAllUse = false;
	 // ������ͱ�����
	int nLeftMagicIndex = nMagicCardCount -1; //���Ҫ��һ���챧����		
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(nLeftMagicIndex == 0)
		{
			break;
		}
		//ͬ���ж�
		if (i == m_nHunIndex) 
		{
			continue;
		}	
		if (cbMagicCardIndex[i]==2)
		{
			cbMagicCardIndex[i] =0;
			nLeftMagicIndex --;
		}
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//�����ж�
		if ((i<(MAX_INDEX-9))&&((i%9)<7))
		{
			if(nLeftMagicIndex == 0)
			{
				break;
			}		
			if(cbMagicCardIndex[i]!= m_nHunIndex&& cbMagicCardIndex[i+1]!= m_nHunIndex&&cbMagicCardIndex[i+2]!= m_nHunIndex)
			{
				if (cbMagicCardIndex[i] + cbMagicCardIndex[i + 1] + cbMagicCardIndex[i + 2] == 2)
				{					
					if(cbMagicCardIndex[i] > 0)
					{
						cbMagicCardIndex[i] -=1;
					}
					if(cbMagicCardIndex[i+1] > 0)
					{
						cbMagicCardIndex[i+1] -=1;
					}
					if(cbMagicCardIndex[i+2] > 0)
					{
						cbMagicCardIndex[i+2] -=1;
					}
					nLeftMagicIndex --;
				}
			}
		}
	}


	nLeftFeng = 0;
	nLeftNum=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i != m_nHunIndex)
		{
			if(cbMagicCardIndex[i] != 0)
			{
				nLeftNum+=cbMagicCardIndex[i];
				if(i>=27)
				{
					nLeftFeng += cbMagicCardIndex[i];;
				}
			}
		}
	}
	if (nKey == 1)
	{
		if(nLeftNum <=1&&nLeftFeng == 0)
		{
			bAllUse = true;
		}		
	}
	else
	{
		if(nLeftNum <=0)
		{
			bAllUse = true;
		}
	}

	if (bAllUse)
	{
		return 0;
	}

	return ret;
}

// edit by wld
DWORD CTableFrameSink::AnalyseQiangGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	DWORD ret = 1;
	BYTE cardIndex = m_GameLogic.SwitchToCardIndex(cbCurrentCard);
//	if(cbCardIndex[cardIndex]>0)
//	{
//		return ret;
//	}
	if (cardIndex > 27)
	{
		return 0;
	}
	int nCurrentCardIndex = cardIndex % 9;
	
	if(nCurrentCardIndex>=2)
	{
		if(cbCardIndex[cardIndex-1]>0 &&cbCardIndex[cardIndex-2]>0)
		{
			return ret;
		}
	}
	if(nCurrentCardIndex<7)
	{
		if(cbCardIndex[cardIndex+1]>0 &&cbCardIndex[cardIndex+2]>0)
		{
			return ret;
		}
	}
	if(nCurrentCardIndex<8 &&nCurrentCardIndex>=1)
	{
		if(cbCardIndex[cardIndex+1]>0 &&cbCardIndex[cardIndex-1]>0)
		{
			return ret;
		}
	}
	return 0;
}

DWORD CTableFrameSink::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight, int nCurrentUser, BYTE ucBaoHou, BYTE QFJ)
{
	DWORD ret =  0 ;
	if(m_bGameTypeIndex == 3)
	{
		bool bHasBighu = true ;
		bool bHasSmallhu = true;
		if(m_bOpenMouth[nCurrentUser] == 0 || (m_iStartBaoNum[nCurrentUser] == 0 && m_iBaoNum[nCurrentUser] <= 1)) //û�п��� ����û�б�
		{
				//���Ժ���С��
		}
		else  if(m_iStartBaoNum[nCurrentUser] == 0 && m_bOpenMouth[nCurrentUser] == 1 && m_iBaoNum[nCurrentUser] >= 2)
		{
			if(m_bShowBao[nCurrentUser] == true) //���˱�
			{
			
			}
			else  // û������
			{
				bHasSmallhu = false;
			}
		}
		else if(m_iStartBaoNum[nCurrentUser] == 1  && m_bOpenMouth[nCurrentUser] == 1 && m_iBaoNum[nCurrentUser] >= 2) //����һ���� ���˿� 
		{
			if(m_bShowBao[nCurrentUser] == true) //���˱�
			{
				//���Ժ���С�� 
			}
			else  // û������
			{
				bHasSmallhu = false;
			}
		}
		else if (m_iStartBaoNum[nCurrentUser] >= 2 && m_bOpenMouth[nCurrentUser] == true) //���������� ���˿� 
		{
			//ֻ�ܺ����
			bHasSmallhu = false;
		}
		ret = m_GameLogic.AnalyseChiHuCard_DaoDao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,
		bHasBighu,bHasSmallhu, m_iBaoNum[nCurrentUser], ucBaoHou);
	}
	else if (m_bGameTypeIndex == 7) // ��÷
	{
		BYTE cbZiMoDianPao = 0; // ����
		if (nCurrentUser == m_wProvideUser)//����
		{
			cbZiMoDianPao = 1;
		}
		ret = m_GameLogic.AnalyseChiHuCard_HM(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,m_cbPeng[nCurrentUser],cbZiMoDianPao);
	}
	else 
	{
		bool bGangKai = false;
		if( m_wCurrentUser==nCurrentUser && m_bGangStatus )
		{
			bGangKai = true;
		}
		bool bQFJ = true;
		if (!m_bTalkQingFengQing[nCurrentUser] && m_nUsefulOper[nCurrentUser] >= 2)
		{
			bQFJ = false;
		}
		BYTE ucflag = 0;
		if (m_bGameTypeIndex == 4 && m_bGameTypeQing == 2)
		{
			ucflag = 1;
		}
		else if (m_bGameTypeIndex == 4 && m_bGameTypeQing == 1)
		{
			ucflag = 2;
		}
		if (m_bGameTypeIndex == 8) // Ӣɽ
		{
			// ��鱨������罫
			if (!m_bBaoIsOutCard) bQFJ = true;
			bool bZiMo = false;
			if (nCurrentUser == m_wCurrentUser) bZiMo = true;
			ret = m_GameLogic.AnalyseChiHuCard_YS(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,bGangKai,bQFJ,bZiMo,m_cbTalkAllOther[nCurrentUser]);
		}
		else if (m_bGameTypeIndex == 9) // ���
		{
			ret = m_GameLogic.AnalyseChiHuCard_MC(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,m_bGameTypeQing == 2?1:0,bGangKai);
		}
		else
			ret = m_GameLogic.AnalyseChiHuCard_DaoDao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,
			m_wProvideUser == m_wCurrentUser,m_bGameTypeQing == 2?1:0,bGangKai,bQFJ, ucflag, QFJ);	
	}

	return ret;
}

DWORD CTableFrameSink::AnalyseChiHuCardBD(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	DWORD ret = m_GameLogic.AnalyseChiHuCard_DaoDao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,
		m_wProvideUser == m_wCurrentUser,m_bGameTypeQing == 2?1:0,false);
	return ret;
// 	if (hasRule(GAME_RULE_DAODAO_HU))
// 	{
// 		if (hasRule(GAME_RULE_ZIMOHU) && INVALID_CHAIR != m_wCurrentUser)
// 		{
// 			return WIK_NULL;
// 		}
// 		return m_GameLogic.AnalyseChiHuCard_DaoDao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,ChiHuRight,m_wProvideUser == m_wCurrentUser,hasRule(GAME_RULE_QIDUI));
// 	}
// 	return WIK_NULL;
}

void CTableFrameSink::getChiHuCardList(WORD wChairID,std::vector<BYTE>& cbChiHuCardList)
{
	for (int i=0;i<m_kChiHuInfoList.size();i++)
	{
		if (m_kChiHuInfoList[i].wChiHuUser == wChairID)
		{
			cbChiHuCardList.push_back(m_kChiHuInfoList[i].cbChiHuCard);
		}
	}
}

//////////////////////////////////////////////////////////////////////////


void CTableFrameSink::RandCard()
{

	if(m_cbPlayerNum == 4)
	{
		if(m_bGameTypeIndex == 2 || m_bGameTypeIndex == 4 ||  m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8 || m_bGameTypeIndex == 9)
		{
			m_cbLeftCardCount = m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard),m_bGameTypeIndex,m_bGameTypeQing);

		}	
		else if(m_bGameTypeIndex == 3 || m_bGameTypeIndex == 7) // ���ݻλΣ���÷
		{
			m_cbLeftCardCount = m_GameLogic.RandCardDataHZHH(m_cbRepertoryCard,CountArray(m_cbRepertoryCard),m_lSiceCount);
		}
	}
	else 
	{
		if (m_bGameTypeIndex == 2 || m_bGameTypeIndex == 4 ||  m_bGameTypeIndex == 6 || m_bGameTypeIndex == 8 || m_bGameTypeIndex == 9)
		{
			m_cbLeftCardCount = m_GameLogic.RandCardDataTwoThree(m_cbRepertoryCard,CountArray(m_cbRepertoryCard),m_bGameTypeIndex,m_bGameTypeQing);
		}
		else if (m_bGameTypeIndex == 3) // ���ݻλ�
		{
			m_cbLeftCardCount = m_GameLogic.RandCardDataHzhhTwoThree(m_cbRepertoryCard,CountArray(m_cbRepertoryCard),m_lSiceCount);
		}
		else if (m_bGameTypeIndex == 7) // ��÷
		{
			m_cbLeftCardCount = m_GameLogic.RandCardDataTwoThreeHM(m_cbRepertoryCard, CountArray(m_cbRepertoryCard));
		}
	}

}

bool CTableFrameSink::IsLaiPi(int nIndex)
{
	if(m_nHunIndex == nIndex)
	{
		return true;
	}
	return false;
}
bool CTableFrameSink::IsHun(int nIndex)
{
	if(m_nPiIndex == nIndex && m_bGameTypeIndex !=3)
	{
		return true;
	}
	if(m_nLaiPiIndex == nIndex)
	{
		return true;
	}
	return false;

}

bool CTableFrameSink::CheckRandCard()
{
	bool ret= true;

	BYTE tpCardIndex[MAX_INDEX];	//�û��˿�

	m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,tpCardIndex);
	for(int i = 0;i < MAX_INDEX;i++)
	{
		if(tpCardIndex[i] > 4)
		{
			ret = false;
			return ret;
		}
	}
	return ret;
}

void CTableFrameSink::DoFirstGang()
{
	if(m_bFirstGang)
	{
		int nTemp =	m_cbRepertoryCard[m_cbLeftCardCount - 1] ;
		m_cbRepertoryCard[m_cbLeftCardCount - 1] = m_cbRepertoryCard[0];
		m_cbRepertoryCard[0] = nTemp;
		m_bFirstGang = false;
	}
}

// ���ݻλ��ж�Ƥ�����ӵĵ���
void CTableFrameSink::DoSicePiGang()
{
	int nTemp =	m_cbRepertoryCard[m_cbLeftCardCount - 1];
	m_cbRepertoryCard[m_cbLeftCardCount - 1] = m_cbRepertoryCard[m_cbSiceCountPi * 2 - 2];
	m_cbRepertoryCard[m_cbSiceCountPi * 2 - 2] = nTemp;
	m_bPiGang = true;
}

// Ϫˮ�ж�Ƥ�����ӵĵ���
void CTableFrameSink::DoSicePiGangXS()
{
	int nTemp =	m_cbRepertoryCard[m_cbLeftCardCount - 1];
	m_cbRepertoryCard[m_cbLeftCardCount - 1] = m_cbRepertoryCard[0];
	m_cbRepertoryCard[0] = nTemp;
	m_bPiGang = true;
}
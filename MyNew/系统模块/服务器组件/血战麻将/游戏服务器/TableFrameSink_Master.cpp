#include "StdAfx.h"
#include "TableFrameSink.h"

void CTableFrameSink::sendMasterHandCard(int nChairdID)
{
	GDMJ_MasterHandCard kMasterHandCard;

	for (int i=0;i<GAME_PLAYER;i++)
	{
		GDMJ_MasterHandCardInfo kCardInfo;
		kCardInfo.nChairId = i;
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],kCardInfo.kMasterHandCard);

		kMasterHandCard.kMasterHandCardList.push_back(kCardInfo);
	}
	datastream kDataStream;
	kMasterHandCard.StreamValue(kDataStream,true);
	m_pITableFrame->SendTableData( nChairdID,SUB_S_MASTER_HANDCARD,&kDataStream[0],kDataStream.size());
}


void CTableFrameSink::sendMasterLeftCard(int nChairdID)
{
	sendMasterHandCard(nChairdID);

	std::vector<BYTE> LeftCardData;
	
	for (int i=0;i<m_cbLeftCardCount;i++)
	{
		LeftCardData.push_back(m_cbRepertoryCard[i]);
	}

	GDMJ_MasterLeftCard kMasterLeftCard;
	ZeroMemory(&kMasterLeftCard,sizeof(kMasterLeftCard));
	m_GameLogic.SwitchToCardIndex(LeftCardData,kMasterLeftCard.kMasterLeftIndex);
	kMasterLeftCard.kMasterCheakCard = m_cbMasterCheckCard[nChairdID];

	m_pITableFrame->SendTableData( nChairdID,SUB_S_MASTER_LEFTCARD,&kMasterLeftCard,sizeof(kMasterLeftCard));
}
BYTE CTableFrameSink::getSendCardData(WORD dwSendUser)
{
	if(m_bGameCardType == GAME_TYPE_ITEM_WITH_FENG)
	{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

		BYTE MasterCard = m_cbMasterCheckCard[dwSendUser];
		if (MasterCard == 0)
		{
			return m_cbRepertoryCard[--m_cbLeftCardCount];
		}

		bool bValue = false;
		for (int i=m_cbLeftCardCount-1;i>=0;i--)
		{
			if (m_cbRepertoryCard[i] == MasterCard)
			{
				bValue = true;
				swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
				break;
			}
		}

		m_cbMasterCheckCard[dwSendUser]= 0;
		BYTE nCard = m_cbRepertoryCard[--m_cbLeftCardCount];
		if (!bValue)
		{
			m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(dwSendUser),"你选择的牌已经不存在！",SMT_EJECT|SMT_CHAT);
		}
		return nCard;
	}
	else if(m_bGameCardType == GAME_TYPE_ITEM_NO_FENG)
	{
		if(hasRule(GAME_RULE_GUANGDONG))
		{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

			BYTE MasterCard = m_cbMasterCheckCard[dwSendUser];
			if (MasterCard == 0)
			{
				return m_cbRepertoryCard[--m_cbLeftCardCount];
			}

			bool bValue = false;
			for (int i=m_cbLeftCardCount-1;i>=0;i--)
			{
				if (m_cbRepertoryCard[i] == MasterCard)
				{
					bValue = true;
					swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
					break;
				}
			}

			m_cbMasterCheckCard[dwSendUser]= 0;
			BYTE nCard = m_cbRepertoryCard[--m_cbLeftCardCount];
			if (!bValue)
			{
				m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(dwSendUser),"你选择的牌已经不存在！",SMT_EJECT|SMT_CHAT);
			}
			return nCard;
		}
		else if(hasRule(GAME_RULE_HONGZHONG))
		{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

			BYTE MasterCard = m_cbMasterCheckCard[dwSendUser];
			if (MasterCard == 0)
			{
				return m_cbRepertoryCard[--m_cbLeftCardCount];
			}

			bool bValue = false;
			for (int i=m_cbLeftCardCount-1;i>=0;i--)
			{
				if (m_cbRepertoryCard[i] == MasterCard)
				{
					bValue = true;
					swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
					break;
				}
			}

			m_cbMasterCheckCard[dwSendUser]= 0;
			BYTE nCard = m_cbRepertoryCard[--m_cbLeftCardCount];
			if (!bValue)
			{
				m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(dwSendUser),"你选择的牌已经不存在！",SMT_EJECT|SMT_CHAT);
			}
			return nCard;
		}
	}
	return 0;
}

bool CTableFrameSink::checkIsGuoShouPeng(WORD chairID, BYTE card)
{
	bool bFound = false;
	for (int i=0;i <m_vecEnjoinChiPeng[chairID].size(); i ++ )
	{
		if (m_vecEnjoinChiPeng[chairID][i] == card)
		{
			bFound = true;
			return bFound;
		}
	}
	return bFound;
}
#include "StdAfx.h"
#include "TableFrameSink.h"

#define PrivateAction_ZiMo 0
#define PrivateAction_JiePao 1
#define PrivateAction_FangPao 2
#define PrivateAction_An_Gang 3 // 自己杠
#define PrivateAction_Ming_Gang 4 //杠别人

void CTableFrameSink::starGameRecord()
{
	m_kGameRecord = GDMJGameRecord();

	m_kGameRecord.cbHunIndex= m_nHunIndex;	// 混
	m_kGameRecord.cbLaiPiIndex= m_nLaiPiIndex;// 皮
	m_kGameRecord.cbPiIndex= m_nPiIndex;// 皮
	m_kGameRecord.cbBaoIndex = m_iBaoIndex; // 宝

	for (WORD i=0;i<m_cbPlayerNum;i++)
	{
		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pUserItem == NULL)
		{
			continue;
		}
		GDMJGameRecordPlayer kReocrdPlayer;
		kReocrdPlayer.dwUserID = pUserItem->GetUserID();
		kReocrdPlayer.kHead = "";
		kReocrdPlayer.kNickName = pUserItem->GetNickName();
		BYTE cbCardData[MAX_COUNT];
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],cbCardData);
		for (int i = 0;i<MAX_COUNT;i++)
		{
			kReocrdPlayer.cbCardData.push_back(cbCardData[i]);
		}
		m_kGameRecord.kPlayers.push_back(kReocrdPlayer);

	}
}
void CTableFrameSink::addGameRecordAction(GDMJ_CMD_S_OperateResult kNetInfo)
{
	GDMJGameRecordOperateResult kOperateRecord;
	kOperateRecord.cbActionType = GDMJGameRecordOperateResult::TYPE_OperateResult;
	kOperateRecord.cbOperateCard = kNetInfo.cbOperateCard;
	kOperateRecord.cbOperateCode = kNetInfo.cbOperateCode;
	kOperateRecord.wOperateUser = kNetInfo.wOperateUser;
	kOperateRecord.wProvideUser = kNetInfo.wProvideUser;
	m_kGameRecord.kAction.push_back(kOperateRecord);
}
void CTableFrameSink::addGameRecordAction(GDMJ_CMD_S_SendCard kNetInfo)
{
	GDMJGameRecordOperateResult kOperateRecord;
	kOperateRecord.cbActionType = GDMJGameRecordOperateResult::TYPE_SendCard;
	kOperateRecord.cbOperateCard = kNetInfo.cbCardData;
	kOperateRecord.cbOperateCode = kNetInfo.cbActionMask;
	kOperateRecord.wOperateUser = kNetInfo.wCurrentUser;
	m_kGameRecord.kAction.push_back(kOperateRecord);
}
void CTableFrameSink::addGameRecordAction(GDMJ_CMD_S_OutCard kNetInfo)
{
	GDMJGameRecordOperateResult kOperateRecord;
	kOperateRecord.cbActionType = GDMJGameRecordOperateResult::TYPE_OutCard;
	kOperateRecord.cbOperateCard = kNetInfo.cbOutCardData;
	kOperateRecord.wOperateUser = kNetInfo.wOutCardUser;
	m_kGameRecord.kAction.push_back(kOperateRecord);
}
void CTableFrameSink::addGameRecordAction(GDMJ_CMD_S_ChiHu kNetInfo)
{
	GDMJGameRecordOperateResult kOperateRecord;
	kOperateRecord.cbActionType = GDMJGameRecordOperateResult::TYPE_ChiHu;
	kOperateRecord.cbOperateCard = kNetInfo.cbChiHuCard;
	kOperateRecord.wProvideUser = kNetInfo.wProviderUser;
	kOperateRecord.wOperateUser = kNetInfo.wChiHuUser;
	m_kGameRecord.kAction.push_back(kOperateRecord);

	if (kNetInfo.wChiHuUser == kNetInfo.wProviderUser)
	{
		m_pITableFrame->AddPrivateAction(kNetInfo.wChiHuUser,PrivateAction_ZiMo);
	}
	else
	{
		m_pITableFrame->AddPrivateAction(kNetInfo.wProviderUser,PrivateAction_FangPao);
		m_pITableFrame->AddPrivateAction(kNetInfo.wChiHuUser,PrivateAction_JiePao);
	}
}
#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//�������
struct tagWeaveItem
{
	DWORD							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
/*	BYTE							cbIsSelfGang;*/
};
#pragma pack(1)
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_LISTEN					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����
#define WIK_ZI_MO					0x80								//����
#define WIK_GANG_PI					0x100								//��Ƥ
#define WIK_GANG_LAIPI				0x200								//����Ƥ
#define WIK_GANG_HUN				0x400								//�ܻ�
#define WIK_QING_BAO				0x800								//��һɫ������
#define WIK_FENG_BAO				0x1000								//��һɫ������
#define WIK_JIANG_BAO				0x2000								//��һɫ������
#define WIK_SHOW_BAO                0x4000                              //����
#define WIK_FA_CAI					0x8000								//����(��÷�淨)
#define WIK_ALL_OTHER				0x10000								//ȫ����


//////////////////////////////////////////////////////////////////////////
//���ƶ���

//����
#define CHK_NULL					0x00										//�Ǻ�����
#define CHK_CHI_HU					0x01										//������

#define CHR_NONE					0x00000001									//��ˮ��
#define CHR_MEN_QING				0x00000002									//����
#define CHR_PENGPENGHU				0x00000004									//��	
#define CHR_QING_YI_SE				0x00000008									//��һɫ
#define CHR_FENG_YI_SE				0x00000010									//��һɫ
#define CHR_JIANG_YI_SE				0x00000020									//��һɫ
#define CHR_QI_DUI					0x00000040									//�߶�		
#define CHR_QI_DUI_2				0x00000080									//���߶�
#define CHR_QI_DUI_3				0x00000100									//˫���߶�

#define CHR_GANG_KAI				0x00000200									//�ܿ�		
#define CHR_PI_KAI					0x00000400									//Ƥ��
#define CHR_LAI_KAI					0x00000800									//����

#define CHR_ZI_MO					0x00001000									//����		
#define CHR_FANG_PAO				0x00002000									//����
#define CHR_DA_HU					0x00004000									//���
#define CHR_DOUBLE_HUN				0x00008000									//˫��
#define CHR_QIANG_GANG              0x00010000                                  //����
#define CHR_ALL_OTHER				0x00020000									//����
#define CHR_BAO_HU					0x00040000									//����
#define CHR_DAN_ZHANG				0x00080000									//����

//////////////////////////////////////////////////////////////////////////

#define ZI_PAI_COUNT	7


#define  MAX_CARD_NUM   34

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbCardIndex[3];						//�˿�����
	BYTE							cbValidIndex[3];					//ʵ���˿�����
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[4];						//�˿�����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	bool                            bMagicEye;                          //�����Ƿ�������
	DWORD							cbWeaveKind[4];						//�������
	BYTE							cbCenterCard[4];					//�����˿�
	BYTE                            cbCardData[4][4];                   //ʵ���˿�
};

//������
struct tagPengItem
{
	WORD							wProvider;
	WORD							wCurrent;
	BYTE							cbCardData;
	bool							bIsStart;
};
//////////////////////////////////////////////////////////////////////////

#define MASK_CHI_HU_RIGHT			0xffffffff

/*
//	Ȩλ�ࡣ
//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//  ֻ�ܵ�������:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//��̬����
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//Ȩλ����
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//���캯��
	CChiHuRight();

	//���������
public:
	//��ֵ��
	CChiHuRight & operator = ( DWORD dwRight );

	//�����
	CChiHuRight & operator &= ( DWORD dwRight );
	//�����
	CChiHuRight & operator |= ( DWORD dwRight );

	//��
	CChiHuRight operator & ( DWORD dwRight );
	CChiHuRight operator & ( DWORD dwRight ) const;

	//��
	CChiHuRight operator | ( DWORD dwRight );
	CChiHuRight operator | ( DWORD dwRight ) const;

	//���ܺ���
public:
	//�Ƿ�ȨλΪ��
	bool IsEmpty();

	//����ȨλΪ��
	void SetEmpty();

	//��ȡȨλ��ֵ
	BYTE GetRightData( DWORD dwRight[], BYTE cbMaxCount );

	//����Ȩλ��ֵ
	bool SetRightData( const DWORD dwRight[], BYTE cbRightCount );

private:
	//���Ȩλ�Ƿ���ȷ
	bool IsValidRight( DWORD dwRight );

};

typedef std::vector<tagAnalyseItem> AnalyseItemList;

//////////////////////////////////////////////////////////////////////////
struct TingCardResult
{
	int nOutCard;
	std::vector<BYTE>  kTingCardList;
};
//����˵��
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;
typedef std::vector<TingCardResult>          TingCardResultList;

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����
	static const BYTE				m_cbCardDataArrayWF[MAX_REPERTORY_WUFENG]; // �޷�
	static const BYTE				m_cbCardDataArrayHZWF[MAX_REPERTORY_HZWUFENG];//�����޷�

	// ���ݻλ�
	static const BYTE               m_cbCardDataArrayHZHH[MAX_REPERTORY_HUANGZHOU];//���ݻλ�

	//2-3���˿�����
	static const BYTE				m_cbCardDataArrayTWTH[TWO_THREE_REPERTORY];						
	//2-3�˻��ݻλ��˿�����
	static const BYTE   		    m_cbCardDataArrayTWTHHZHH[TWO_THREE_HUANGZHOU];			

	BYTE							m_cbMagicIndex;						//��������
	static const BYTE				m_cbCardWord[MAX_CARD_NUM];	//�˿�����
	BYTE							m_cbPiIndex;						//��������
	BYTE							m_cbLaiPiIndex;						//��������
	BYTE                            m_iBaoIndex;                        //������

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	int RandCardData(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing);
	int RandCardDataNoFeng(BYTE cbCardData[], BYTE cbMaxCount);
	int RandCardDataHZNoFeng(BYTE cbCardData[], BYTE cbMaxCount);
	int RandCardDataHZHH(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount);

	//2-3�˻����˿�
	int RandCardDataTwoThree(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing);
	int RandCardDataHzhhTwoThree(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount);
	int RandCardDataTwoThreeHM(BYTE cbCardData[], BYTE cbMaxCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//��������
	void SetMagicIndex( BYTE cbMagicIndex ) { if(cbMagicIndex<=MAX_INDEX)m_cbMagicIndex = cbMagicIndex; }

	//����Pi
	void SetPiIndex( BYTE cbPiIndex ) { if(cbPiIndex<=MAX_INDEX)m_cbPiIndex = cbPiIndex; }
	//����Pi
	void SetLaiPiIndex( BYTE cbLaiPiIndex ) { if(cbLaiPiIndex<=MAX_INDEX)m_cbLaiPiIndex = cbLaiPiIndex; }
	//���ñ�
	void SetBaoIndex(BYTE cbBaoIndex){if(cbBaoIndex<=MAX_INDEX) m_iBaoIndex = cbBaoIndex; }
	//����
	bool IsMagicCard( BYTE cbCardData );

	//�����
	bool AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount);

	bool IsJiangByIndex(BYTE cbCardIndex);
	bool IsJiangByCardData(BYTE cbCardData);

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
	//����˿�
	BYTE GetWeaveCard(DWORD cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(DWORD cbUserAction);
	//���Ƶȼ�
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);
	//���Ƶȼ�
	WORD GetChaHuActionRank(const CChiHuRight & ChiHuRight);
	//���Ƶȼ�
	WORD GetChiHuActionRank_YiChang(const CChiHuRight & ChiHuRight);

	//�����ж�
public:
	//�����ж�
	DWORD EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	DWORD EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	DWORD EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	DWORD AnalyseGangCard_HasHu(bool bIsYiChang ,const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult,const std::vector<BYTE>& cbHuCardList);
	//�����ж�
public:
	//���Ʒ���
	DWORD AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//��ϲ����
	DWORD AnalyseLiangXiCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//ϲ�ܷ���
	DWORD AnalyseXiGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//�Ժ�����
	DWORD AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bHasYao9JiangDui = false);

	DWORD AnalyseChiHuCard_YiChang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend);
	
	//�����齫���Ʒ���
	DWORD AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
										  bool bHasBigHu, bool  bHasSmallHu, int iBaoNum, BYTE ucBaoHou = 0);
	DWORD AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bSelfSend,bool bHasQiDui,bool bGangkai,bool canQFJ = true, BYTE ucFlag = 0, BYTE QFJ = 0);
	
	//��÷�齫���Ʒ���
	DWORD AnalyseChiHuCard_HM(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		BYTE cbPeng, BYTE cbZiMoDianPao);

	//Ӣɽ�齫���Ʒ���
	DWORD AnalyseChiHuCard_YS(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bGangkai,bool canQFJ,bool bZiMo,BYTE cbTalkAllOther);

	//����齫���Ʒ���
	DWORD AnalyseChiHuCard_MC(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bHasQiDui,bool bGangkai);

	
	DWORD AnalyseChiHuTAG_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend,bool bHasQiDui);

	//���Ʒ���
	DWORD AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	std::vector<BYTE> GetTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng);
	std::vector<BYTE> GetTing_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng);
	bool GetTingTag_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng,BYTE bGameTypeIndex=0,BYTE bGameTypeQing=0);
	DWORD AnalyseTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//�Ƿ�����
	bool IsTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	bool IsTingCard_YiChang( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//�Ƿ���
	bool IsHuaZhu( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//�˿�ת��
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], std::vector<BYTE>& cbCardData);
	BYTE SwitchToCardIndex(const std::vector<BYTE>& cbCardData, BYTE cbCardIndex[MAX_INDEX]);

	//��С����
	DWORD IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);

	//�����˿�
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, 
		CAnalyseItemArray & AnalyseItemArray , bool bCheckJiang = false, int maxHun = 1);

	//�����˿�
	bool AnalyseCardPingHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray,int nMaxHun = 1);

	DWORD AnalyseWave(const tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//��������
protected:
public:
	bool IsJiangHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	bool IsFengYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);

	//�����
	bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
	//��һɫ��
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard, int * color);

	//����
	bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
	//����
	bool IsJiangDui( const tagAnalyseItem *pAnalyseItem );

	//���߶�
	bool isJiangQiDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	bool IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard);
	
	//�ڲ�����
private:
	
	bool AnalyseTAG(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//����,������ֵ����
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
public:

	BYTE GetCardColor(BYTE cbCardDat);
	BYTE GetCardValue(BYTE cbCardDat);

	void GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<GDMJ_Card_Info>& kCardInfoList);
	BYTE GetAutoDiscardData(const BYTE cbCardIndex[MAX_INDEX],BYTE nQueColor,int nWeaveCont);
	//�����аװ�
	bool CheckHZBB(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//��������
	bool CheckDanZhang(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
};

//////////////////////////////////////////////////////////////////////////

#pragma pack()
#endif

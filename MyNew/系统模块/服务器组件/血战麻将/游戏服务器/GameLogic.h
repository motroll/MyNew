#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//组合子项
struct tagWeaveItem
{
	DWORD							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
/*	BYTE							cbIsSelfGang;*/
};
#pragma pack(1)
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

//动作标志
#define WIK_NULL					0x00								//没有类型
#define WIK_LEFT					0x01								//左吃类型
#define WIK_CENTER					0x02								//中吃类型
#define WIK_RIGHT					0x04								//右吃类型
#define WIK_PENG					0x08								//碰牌类型
#define WIK_GANG					0x10								//杠牌类型
#define WIK_LISTEN					0x20								//吃牌类型
#define WIK_CHI_HU					0x40								//吃胡类型
#define WIK_ZI_MO					0x80								//自摸
#define WIK_GANG_PI					0x100								//杠皮
#define WIK_GANG_LAIPI				0x200								//杠赖皮
#define WIK_GANG_HUN				0x400								//杠混
#define WIK_QING_BAO				0x800								//清一色包三家
#define WIK_FENG_BAO				0x1000								//风一色包三家
#define WIK_JIANG_BAO				0x2000								//将一色包三家
#define WIK_SHOW_BAO                0x4000                              //亮宝
#define WIK_FA_CAI					0x8000								//发财(黄梅玩法)
#define WIK_ALL_OTHER				0x10000								//全求人


//////////////////////////////////////////////////////////////////////////
//胡牌定义

//胡牌
#define CHK_NULL					0x00										//非胡类型
#define CHK_CHI_HU					0x01										//胡类型

#define CHR_NONE					0x00000001									//清水胡
#define CHR_MEN_QING				0x00000002									//门清
#define CHR_PENGPENGHU				0x00000004									//碰	
#define CHR_QING_YI_SE				0x00000008									//清一色
#define CHR_FENG_YI_SE				0x00000010									//风一色
#define CHR_JIANG_YI_SE				0x00000020									//将一色
#define CHR_QI_DUI					0x00000040									//七对		
#define CHR_QI_DUI_2				0x00000080									//豪七对
#define CHR_QI_DUI_3				0x00000100									//双豪七对

#define CHR_GANG_KAI				0x00000200									//杠开		
#define CHR_PI_KAI					0x00000400									//皮开
#define CHR_LAI_KAI					0x00000800									//赖开

#define CHR_ZI_MO					0x00001000									//自摸		
#define CHR_FANG_PAO				0x00002000									//放炮
#define CHR_DA_HU					0x00004000									//大胡
#define CHR_DOUBLE_HUN				0x00008000									//双倍
#define CHR_QIANG_GANG              0x00010000                                  //抢杠
#define CHR_ALL_OTHER				0x00020000									//求人
#define CHR_BAO_HU					0x00040000									//包胡
#define CHR_DAN_ZHANG				0x00080000									//单张

//////////////////////////////////////////////////////////////////////////

#define ZI_PAI_COUNT	7


#define  MAX_CARD_NUM   34

//类型子项
struct tagKindItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbCardIndex[3];						//扑克索引
	BYTE							cbValidIndex[3];					//实际扑克索引
};

//杠牌结果
struct tagGangCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[4];						//扑克数据
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	bool                            bMagicEye;                          //牌眼是否是王霸
	DWORD							cbWeaveKind[4];						//组合类型
	BYTE							cbCenterCard[4];					//中心扑克
	BYTE                            cbCardData[4][4];                   //实际扑克
};

//过手碰
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
//	权位类。
//  注意，在操作仅位时最好只操作单个权位.例如
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)，这样结果是无定义的。
//  只能单个操作:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//静态变量
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//权位变量
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//构造函数
	CChiHuRight();

	//运算符重载
public:
	//赋值符
	CChiHuRight & operator = ( DWORD dwRight );

	//与等于
	CChiHuRight & operator &= ( DWORD dwRight );
	//或等于
	CChiHuRight & operator |= ( DWORD dwRight );

	//与
	CChiHuRight operator & ( DWORD dwRight );
	CChiHuRight operator & ( DWORD dwRight ) const;

	//或
	CChiHuRight operator | ( DWORD dwRight );
	CChiHuRight operator | ( DWORD dwRight ) const;

	//功能函数
public:
	//是否权位为空
	bool IsEmpty();

	//设置权位为空
	void SetEmpty();

	//获取权位数值
	BYTE GetRightData( DWORD dwRight[], BYTE cbMaxCount );

	//设置权位数值
	bool SetRightData( const DWORD dwRight[], BYTE cbRightCount );

private:
	//检查权位是否正确
	bool IsValidRight( DWORD dwRight );

};

typedef std::vector<tagAnalyseItem> AnalyseItemList;

//////////////////////////////////////////////////////////////////////////
struct TingCardResult
{
	int nOutCard;
	std::vector<BYTE>  kTingCardList;
};
//数组说明
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;
typedef std::vector<TingCardResult>          TingCardResultList;

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据
	static const BYTE				m_cbCardDataArrayWF[MAX_REPERTORY_WUFENG]; // 无风
	static const BYTE				m_cbCardDataArrayHZWF[MAX_REPERTORY_HZWUFENG];//红中无风

	// 黄州晃晃
	static const BYTE               m_cbCardDataArrayHZHH[MAX_REPERTORY_HUANGZHOU];//黄州晃晃

	//2-3人扑克数据
	static const BYTE				m_cbCardDataArrayTWTH[TWO_THREE_REPERTORY];						
	//2-3人黄州晃晃扑克数据
	static const BYTE   		    m_cbCardDataArrayTWTHHZHH[TWO_THREE_HUANGZHOU];			

	BYTE							m_cbMagicIndex;						//钻牌索引
	static const BYTE				m_cbCardWord[MAX_CARD_NUM];	//扑克数据
	BYTE							m_cbPiIndex;						//钻牌索引
	BYTE							m_cbLaiPiIndex;						//钻牌索引
	BYTE                            m_iBaoIndex;                        //宝索引

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//混乱扑克
	int RandCardData(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing);
	int RandCardDataNoFeng(BYTE cbCardData[], BYTE cbMaxCount);
	int RandCardDataHZNoFeng(BYTE cbCardData[], BYTE cbMaxCount);
	int RandCardDataHZHH(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount);

	//2-3人混乱扑克
	int RandCardDataTwoThree(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing);
	int RandCardDataHzhhTwoThree(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount);
	int RandCardDataTwoThreeHM(BYTE cbCardData[], BYTE cbMaxCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//设置钻牌
	void SetMagicIndex( BYTE cbMagicIndex ) { if(cbMagicIndex<=MAX_INDEX)m_cbMagicIndex = cbMagicIndex; }

	//设置Pi
	void SetPiIndex( BYTE cbPiIndex ) { if(cbPiIndex<=MAX_INDEX)m_cbPiIndex = cbPiIndex; }
	//设置Pi
	void SetLaiPiIndex( BYTE cbLaiPiIndex ) { if(cbLaiPiIndex<=MAX_INDEX)m_cbLaiPiIndex = cbLaiPiIndex; }
	//设置宝
	void SetBaoIndex(BYTE cbBaoIndex){if(cbBaoIndex<=MAX_INDEX) m_iBaoIndex = cbBaoIndex; }
	//钻牌
	bool IsMagicCard( BYTE cbCardData );

	//添加牌
	bool AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount);

	bool IsJiangByIndex(BYTE cbCardIndex);
	bool IsJiangByCardData(BYTE cbCardData);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//扑克数目
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
	//组合扑克
	BYTE GetWeaveCard(DWORD cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	BYTE GetUserActionRank(DWORD cbUserAction);
	//胡牌等级
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);
	//胡牌等级
	WORD GetChaHuActionRank(const CChiHuRight & ChiHuRight);
	//胡牌等级
	WORD GetChiHuActionRank_YiChang(const CChiHuRight & ChiHuRight);

	//动作判断
public:
	//吃牌判断
	DWORD EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//碰牌判断
	DWORD EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//杠牌判断
	DWORD EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	DWORD AnalyseGangCard_HasHu(bool bIsYiChang ,const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult,const std::vector<BYTE>& cbHuCardList);
	//动作判断
public:
	//杠牌分析
	DWORD AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//亮喜分析
	DWORD AnalyseLiangXiCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//喜杠分析
	DWORD AnalyseXiGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//吃胡分析
	DWORD AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bHasYao9JiangDui = false);

	DWORD AnalyseChiHuCard_YiChang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend);
	
	//黄州麻将胡牌分析
	DWORD AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
										  bool bHasBigHu, bool  bHasSmallHu, int iBaoNum, BYTE ucBaoHou = 0);
	DWORD AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bSelfSend,bool bHasQiDui,bool bGangkai,bool canQFJ = true, BYTE ucFlag = 0, BYTE QFJ = 0);
	
	//黄梅麻将胡牌分析
	DWORD AnalyseChiHuCard_HM(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		BYTE cbPeng, BYTE cbZiMoDianPao);

	//英山麻将胡牌分析
	DWORD AnalyseChiHuCard_YS(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bGangkai,bool canQFJ,bool bZiMo,BYTE cbTalkAllOther);

	//麻城麻将胡牌分析
	DWORD AnalyseChiHuCard_MC(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bHasQiDui,bool bGangkai);

	
	DWORD AnalyseChiHuTAG_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend,bool bHasQiDui);

	//听牌分析
	DWORD AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	std::vector<BYTE> GetTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng);
	std::vector<BYTE> GetTing_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng);
	bool GetTingTag_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool isWuFeng,BYTE bGameTypeIndex=0,BYTE bGameTypeQing=0);
	DWORD AnalyseTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//是否听牌
	bool IsTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	bool IsTingCard_YiChang( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//是否花猪
	bool IsHuaZhu( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//扑克转换
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], std::vector<BYTE>& cbCardData);
	BYTE SwitchToCardIndex(const std::vector<BYTE>& cbCardData, BYTE cbCardIndex[MAX_INDEX]);

	//七小对牌
	DWORD IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);

	//分析扑克
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, 
		CAnalyseItemArray & AnalyseItemArray , bool bCheckJiang = false, int maxHun = 1);

	//分析扑克
	bool AnalyseCardPingHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray,int nMaxHun = 1);

	DWORD AnalyseWave(const tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//胡法分析
protected:
public:
	bool IsJiangHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	bool IsFengYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);

	//大对子
	bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
	//清一色牌
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard, int * color);

	//带幺
	bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
	//将对
	bool IsJiangDui( const tagAnalyseItem *pAnalyseItem );

	//将七对
	bool isJiangQiDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	bool IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard);
	
	//内部函数
private:
	
	bool AnalyseTAG(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//排序,根据牌值排序
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
public:

	BYTE GetCardColor(BYTE cbCardDat);
	BYTE GetCardValue(BYTE cbCardDat);

	void GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<GDMJ_Card_Info>& kCardInfoList);
	BYTE GetAutoDiscardData(const BYTE cbCardIndex[MAX_INDEX],BYTE nQueColor,int nWeaveCont);
	//检查红中白板
	bool CheckHZBB(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//检查胡单张
	bool CheckDanZhang(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
};

//////////////////////////////////////////////////////////////////////////

#pragma pack()
#endif

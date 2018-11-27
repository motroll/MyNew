#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录服务
protected:
	//I D 登录
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游客登录
	bool OnRequestLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//手机登录
protected:
	//I D 登录
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//账号服务
protected:
	//修改机器
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改帐号
	bool OnRequestModifyAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改推荐人
	bool OnRequestModifySpreader(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改签名
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//评分
	bool OnRequestModifyStarValue(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//分享朋友圈获得奖励
	bool OnRequestShareAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获得游戏状态
	bool OnRequestUserInGameServerID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestPublicNotic(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	// 俱乐部
protected:
	// 会长创建俱乐部
	bool OnRequestCreateClub(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 申请加入俱乐部
	bool OnRequestJoinClub(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 会长同意或不同意加入俱乐部
	bool OnRequestJoinClubComfirm(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 离开俱乐部
	bool OnRequestLeaveClub(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 踢出俱乐部
	bool OnRequestKickClub(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 邀请玩家加入俱乐部
	bool OnRequestInviteClub(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 获取自己的俱乐部请求
	bool OnRequestGetMyClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 俱乐部会员列表请求
	bool OnRequestSubMmemberListReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 会长更新俱乐部公告请求
	bool OnRequestSubUpdateClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 解散俱乐部请求
	bool OnRequestSubDismissClubReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 转让会长请求
	bool OnRequestSubChangeMasterReq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 会长设置管理员请求
	bool OnRequestSubSetManagerReq(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//银行服务
protected:
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转账游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 查询充值列表，并充值
	bool OnCheckAllPayList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// 充值换元宝
	DWORD PidToInsureScore(DWORD pid);
	// 绑定后的元宝
	DWORD SrcInsureToBandInsure(DWORD srcInsure);
	//充值结果通知
	VOID OnPayDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString);
	//信息查询
protected:
	//查询个人信息
	bool OnRequestQueryAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询资料
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询充值历史
	bool OnRequestQueryInsureHistory(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//签到
protected:
	//加载奖励
	bool OnRequestCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询签到
	bool OnRequestCheckInQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//执行签到
	bool OnRequestCheckInDone(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取签到奖励
	bool OnRequestCheckAward(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//轮盘
protected:
	//加载轮盘奖励
	bool OnRequestRouletteReward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//用户轮盘信息
	bool OnRequestUserRouletteAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//执行轮盘
	bool OnRequestRouletteDone(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//新手活动
protected:
	//加载新手活动
	bool OnRequestBeginnerConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//
	bool OnRequestBeginnerQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//
	bool OnRequestBeginnerDone(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//赚金榜
protected:
	//
	bool OnRequestAddBankAwardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//
	bool OnRequestAddBackUpAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//
	bool OnRequestGetAddBank(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//低保服务
protected:
	bool OnRequestGameConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//低保服务
protected:
	//加载低保
	bool OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取低保
	bool OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//比赛服务
protected:
	//比赛报名
	bool OnRequestMatchSignup(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//取消报名
	bool OnRequestMatchUnSignup(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载奖励
	bool OnRequestLoadMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//游戏记录
protected:
	//游戏记录
	bool OnRequestGameRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏回放
	bool OnRequestGameChildRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏回放
	bool OnRequestGameTotalRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);


	//兑换话费
protected:
	//兑换话费列表
	bool OnRequestGetExchangeHuaFeiList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//兑换
	bool OnRequestExchangeHuaFeiDone(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//商城
protected:
	//商城列表
	bool OnRequestGetShopInfoList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//购买
	bool OnRequestGetBuyOder(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//出售
protected:
	//出售
	bool OnRequestSellItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//购买
	bool OnRequestGetSellRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//操作结果
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
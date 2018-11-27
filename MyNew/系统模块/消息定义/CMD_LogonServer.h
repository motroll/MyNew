#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "..\..\全局定义\datastream.h"


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_GP_LOGON_VISITOR		5									//游客登录

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_MATCH_SIGNUPINFO		106									//报名信息

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//I D 登录
struct CMD_GP_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbValidateFlags;			        //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	BYTE							cbValidateFlags;			        //校验标识
};

//游客登录
struct CMD_GP_VisitorLogon
{
	WORD								wFaceID;						//头像标识
	BYTE								cbGender;						//用户性别
	DWORD								dwPlazaVersion;					//广场版本
	BYTE								cbValidateFlags;			    //校验标识
	TCHAR								szNickName[LEN_NICKNAME];		//用户昵称
	TCHAR								szSpreader[LEN_NICKNAME];		//推广人名
	TCHAR								szPassWord[LEN_PASSWORD];		//登录密码
	TCHAR								szPassWordBank[LEN_PASSWORD];	//登录密码
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	BYTE							cbValidateFlags;			        //校验标识
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	DWORD							dwSpreaderID;						//推广ID
	BYTE							cbInsureEnabled;					//银行开通
	
	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态
	CHAR							szPictureName[128];                 //活动图片
	DWORD                           dwPresent[6];                       //

};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//机器序列
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
	TCHAR							szDownLoadHttp[128];				//下载地址
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_LIST_MATCH			105									//比赛列表
#define SUB_GP_VIDEO_OPTION			106									//视频配置

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE	103									//修改签名
#define SUB_GP_MODIFY_ACCOUNTS		104									//修改帐号
#define SUB_GP_MODIFY_SPREADER		105									//修改推荐人

//修改头像
#define SUB_GP_USER_FACE_INFO		120									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO		121									//系统头像
#define SUB_GP_CUSTOM_FACE_INFO		122									//自定头像

//比赛服务
#define SUB_GP_MATCH_SIGNUP			200									//比赛报名
#define SUB_GP_MATCH_UNSIGNUP		201									//取消报名
#define SUB_GP_MATCH_SIGNUP_RESULT	202									//报名结果
#define SUB_GP_MATCH_AWARD_LIST		203									//比赛奖励

//签到服务
#define SUB_GP_CHECKIN_QUERY		220									//查询签到
#define SUB_GP_CHECKIN_INFO			221									//签到信息
#define SUB_GP_CHECKIN_DONE			222									//执行签到
#define SUB_GP_CHECKIN_RESULT		223									//签到结果
#define SUB_GP_CHECKIN_AWARD		224									//签到奖励

//新手引导
#define SUB_GP_BEGINNER_QUERY		240									//新手引导签到
#define SUB_GP_BEGINNER_INFO		241									//新手引导信息
#define SUB_GP_BEGINNER_DONE		242									//新手引导执行
#define SUB_GP_BEGINNER_RESULT		243									//新手引导结果

//轮盘服务
#define SUB_GP_ROULETTE_USERINFO	250									//轮盘信息
#define SUB_GP_ROULETTE_DONE		251									//执行


//低保服务
#define SUB_GP_BASEENSURE_LOAD			260								//加载低保
#define SUB_GP_BASEENSURE_TAKE			261								//领取低保
#define SUB_GP_BASEENSURE_PARAMETER		262								//低保参数
#define SUB_GP_BASEENSURE_RESULT		263								//低保结果

//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料
#define SUB_GP_QUERY_ACCOUNTINFO	304									//个人信息
#define SUB_GP_QUERY_INGAME_SEVERID	305									//游戏状态
#define SUB_GP_MODIFY_STARVALUE		306									//评分
#define SUB_GP_SHARE_AWARD			307									//分享朋友圈奖励
#define SUB_GP_AWARD_RETURN			308									//返回

//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE	402									//转账操作
#define SUB_GP_USER_INSURE_INFO		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//用户信息
#define SUB_GP_QUERY_USER_INSURE_HESTORY 409							//充值记录
#define SUB_GP_QUERY_USER_INSURE_HESTORY_SUCCESS 410					//充值记录回复

//自定义字段查询 公告
#define SUB_GP_QUERY_PUBLIC_NOTICE		500									//自定义字段查询
#define	SUB_GP_PUBLIC_NOTICE			501

//设置推荐人结果
#define SUB_GP_SPREADER_RESOULT			520									//设置推荐人结果

//赚金排行榜
#define SUB_GP_ADDRANK_GET_AWARD_INFO	540									//获得奖励信息
#define SUB_GP_ADDRANK_BACK_AWARD_INFO	541									//返回奖励信息
#define SUB_GP_ADDRANK_GET_RANK			542									//获得排行
#define SUB_GP_ADDRANK_BACK_RANK		543									//返回排行

//游戏记录
#define SUB_GP_GAME_RECORD_LIST			550			
#define SUB_GP_GAME_RECORD_TOTAL		551			
#define SUB_GP_GAME_RECORD_CHILD		552									


//操作结果
#define SUB_GP_OPERATE_SUCCESS			900									//操作成功
#define SUB_GP_OPERATE_FAILURE			901									//操作失败

//话费兑换
#define  SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO    1000                    //获取兑换话费列表
#define  SUB_GP_EXCHANGEHUAFEI_BACK				1001					//兑换话费列表返回
#define  SUB_GP_EXCHANGE_DONE					1002					//兑换

//商城数据
#define  SUB_GP_SHOPINFO_GET_LIST_INFO		1100                    //获取商城列表
#define  SUB_GP_SHOPINFO_BACK				1101					//商城列表返回
#define  SUB_GP_SHOPINFO_BUYODER			1102					//购买
#define  SUB_GP_SELLITEM					1103					//
#define  SUB_GP_SELLLIST_BACK				1104					//

//俱乐部信息
#define SUB_GR_CREATE_CLUB			    1200		// 创建俱乐部
#define SUB_GR_JOIN_CLUB				1201		// 申请加入俱乐部
#define SUB_GR_JOIN_CLUB_COMFIRM		1203		// 会长同意或不同意加入俱乐部
#define SUB_GR_LEAVE_CLUB				1204		// 离开俱乐部
#define SUB_GR_KICK_CLUB				1205		// 踢出俱乐部
#define SUB_GR_INVITE_CLUB				1206		// 邀请玩家加入俱乐部
#define SUB_GR_CREATE_JOIN_RES          1209        // 创建或者申请加入俱乐部回应
#define SUB_GR_GET_MY_CLUB_REQ          1211        // 获取自己的俱乐部请求
#define SUB_GR_GET_MY_CLUB_RES          1212        // 获取自己的俱乐部回应
#define SUB_GR_CLUB_MEMBER_LIST_REQ     1213        // 会长俱乐部会员列表请求
#define SUB_GR_CLUB_MEMBER_LIST_RES     1214        // 会长俱乐部会员列表回应
#define SUB_GR_UPDATE_CLUB_REQ			1215        // 会长更新俱乐部请求
#define SUB_GR_DISMISS_CLUB_REQ         1216		// 解散俱乐部请求
#define SUB_GR_CLUB_CHANGE_MASTER_REQ   1217		// 转让会长请求
#define SUB_GR_CLUB_SET_MANAGER_REQ		1218		// 会长设置管理员请求
#define SUB_GR_CREATE_FLOOR_STRESS_REQ  1219        // 新开楼层




// 创建俱乐部  SUB_GR_CREATE_CLUB
struct SUB_GR_Create_Club
{	
	DWORD dwUserID;					// 用户ID
	TCHAR stClubName[LEN_NICKNAME]; // 俱乐部名称
	BYTE bGameTypeIndex;			// 游戏类型 2罗田，3黄州晃晃，4溪水
	BYTE bGameTypeQing;				// 2有门清 ，1 没门清
	BYTE bPlayCoutIdex;				// 游戏局数
	BYTE bGamePayType;				// 游戏付费类型
	BYTE bQiangGangHu;				// 黄州的抢杠胡 0没有，1有
	BYTE bAllOther;					// 溪水的全求人 0没有，1有
	BYTE bMaxScore;				    // 封顶分数
	BYTE bPantherType;              // 风豹 将豹 单豹  色豹 
	BYTE bBaoIsOutCard;             // 0 有宝不可打 1 有宝可打 2 无宝
	BYTE bPiaoShu;				    // 飘数 0 不要 1 飘1 2 飘2... 10 飘10	
	BYTE bPlayerNum;                // 游戏人数
	BYTE bGameBaseScore;            // 游戏底分
};

// 申请加入俱乐部 SUB_GR_JOIN_CLUB
struct SUB_GR_Join_Club
{
	DWORD dwUserID; // 用户ID
	DWORD dwClubID; // 要申请的俱乐部ID
};

// 会长同意或不同意加入俱乐部 SUB_GR_JOIN_CLUB_COMFIRM
struct SUB_GR_Join_Club_Comfirm 
{	
	DWORD dwUserID; // 待审核的用户ID
	DWORD dwClubID; // 申请俱乐部的ID
	BYTE bComfirm;  // 1同意，2拒绝	
};

// 离开俱乐部 SUB_GR_LEAVE_CLUB
struct SUB_GR_Leave_Club
{	
	DWORD dwUserID; // 申请者的用户ID
	DWORD dwClubID; // 离开俱乐部的ID
};

// 踢出俱乐部 SUB_GR_KICK_CLUB
struct SUB_GR_Kick_Club
{	
	DWORD dwRequestID; // 申请者ID
	DWORD dwUserID; // 踢出用户的ID
	DWORD dwClubID; // 俱乐部ID	
};

// 邀请玩家加入俱乐部 SUB_GR_INVITE_CLUB
struct SUB_GR_Invite_Club
{	
	DWORD dwRequestID; // 申请者ID
	DWORD dwUserID; // 被邀请的用户ID
	DWORD dwClubID; // 俱乐部ID
};

//获取自己的俱乐部请求  SUB_GR_GET_MY_CLUB_REQ
struct   SUB_GR_Get_My_Club_Req
{
	DWORD                       dwUserID;                     // 自己的用户ID
};

//获取自己的俱乐部回应  SUB_GR_GET_MY_CLUB_RES
struct   SUB_GR_Get_My_Club_Res
{
	DWORD				cbAgent;					   // 是否代理 不是0，是1
	DWORD               dwUserId;                      // 用户ID
	DWORD               dwClubNum;                     // 自己的俱乐部数量 
	DWORD               dwClubId[5];                   // 俱乐部ID
	TCHAR               szClubName[5][LEN_NICKNAME];   // 俱乐部名字
	DWORD               dwMemberNum[5];                // 成员数量
	DWORD               dwMasterId[5];                 // 会长ID
	DWORD				dwClubManagerId[5][3];         // 管理员ID
	TCHAR               szMaterName[5][LEN_NICKNAME];  // 会长名字
	TCHAR               szPatritUrl[5][256];           // 会长头像
	TCHAR				stClubNotice[5][124];		   // 俱乐部公告
	BYTE				cbGameTypeIndex[5];            // 游戏类型
	BYTE				bGameTypeQing[5];			   // 2有门清 ，1 没门清
	BYTE				bPlayCoutIdex[5];			   // 游戏局数
	BYTE				bGamePayType[5];			   // 游戏付费类型
	BYTE				bQiangGangHu[5];			   // 黄州的抢杠胡 0没有，1有
	BYTE				bAllOther[5];				   // 溪水的全求人 0没有，1有
	BYTE				cbPantherType[5];			   // 风 将 单 色 豹
	BYTE				cbPlayerNum[5];			       // 游戏人数
	BYTE                bBaoIsOutCard[5];              //0 有宝不可打 1 有宝可打 2 无宝
	BYTE                bPiaoShu[5];				    //飘数 0 不要 1 飘1 2 飘2... 10 飘10
	BYTE                cbGameBaseScore[5];            // 黄梅底分
	BYTE				cbGameMaxScore[5];             // 封顶分数
	SYSTEMTIME			dCreateTime[5];				   // 创建俱乐部的时间
	DWORD				dwOnlineNum[5];				   // 成员在线人数

};

// SUB_GR_CREATE_JOIN_RES             1209         创建或者加入俱乐部的回应
struct  SUB_GR_Create_Join_Res 
{
	DWORD dwCreateJoin; // 1 创建，2 申请加入
	DWORD dwError; // 错误标识 0 成功，其他数失败
	TCHAR szDescribeString[128]; //描述消息
}; 

// SUB_GR_CLUB_MEMBER_LIST_REQ     1213        会长俱乐部会员列表请求
struct  SUB_GR_Club_Member_List_Req 
{
	DWORD dwUserID; // 会长ID
	DWORD dwClubID; // 俱乐部ID
	DWORD dwFlag;   // 0请求申请列表，1请求会员列表
};


// SUB_GR_CLUB_MEMBER_LIST_RES     1214        会长俱乐部会员列表回应
struct SUB_GR_Club_Member_List_Res
{
	DWORD bFlag;                                 // 结束标记，0没结束 1结束
	std::vector<int>                iUserID;     // 用户ID; 
	std::vector<std::string>        szHeadHttp0; // 用户头像; 
	std::vector<std::string>		strClubName; // 俱乐部名称; 
	std::vector<std::string>		strUserName; // 用户名称
	std::vector<std::string>		strJoinTime; // 加入时间

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(bFlag);
		Stream_VECTOR(iUserID);
		Stream_VECTOR(szHeadHttp0);
		Stream_VECTOR(strClubName);
		Stream_VECTOR(strUserName);
		Stream_VECTOR(strJoinTime);
	}
};

// SUB_GR_UPDATE_CLUB_REQ			1215         会长更新俱乐部请求
struct SUB_GR_Update_Club_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	BYTE bGameTypeIndex;			// 游戏类型 2罗田，3黄州晃晃，4溪水
	BYTE bGameTypeQing;				// 2有门清 ，1 没门清
	BYTE bPlayCoutIdex;				// 游戏局数
	BYTE bGamePayType;				// 游戏付费类型
	BYTE bMaxScore;				    // 封顶分数
	BYTE bQiangGangHu;				// 黄州的抢杠胡 0没有，1有
	BYTE bAllOther;					// 溪水的全求人 0没有，1有
	BYTE bPantherType;				// 风 将 单 色 豹
	BYTE bPlayerNum;                // 游戏人数
	BYTE bBaoIsOutCard;             // 0 有宝不可打 1 有宝可打 2 无宝
	BYTE bPiaoShu;				    // 飘数 0 不要 1 飘1 2 飘2... 10 飘10	
	BYTE bGameBaseScore;            // 底分
	TCHAR stClubName[LEN_NICKNAME]; // 俱乐部名称
	TCHAR stClubNotice[124];        // 公告
};

// SUB_GR_DISMISS_CLUB_REQ          1216		 解散俱乐部请求
struct SUB_GR_Dismiss_Club_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
};

// SUB_GR_CLUB_CHANGE_MASTER_REQ   1217		 转让会长请求
struct SUB_GR_Club_Change_Master_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	DWORD dwMasterID;				// 下个会长ID(转让后的)
};

// SUB_GR_CLUB_SET_MANAGER_REQ	  1218		 会长设置管理员请求
struct SUB_GR_Club_Set_Manager
{	
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	DWORD dwManagerID;				// 管理员ID
};
// 新建俱乐部楼层  SUB_GR_CREATE_FLOOR_STRESS_REQ
struct SUB_GR_Create_Floor_Stress
{	
	DWORD dwUserID;					// 用户ID 创建者ID
	DWORD dwClubID;                 // 俱乐部ID
	BYTE  bGameTypeIndex;			// 游戏类型 2罗田，3黄州晃晃，4溪水
	BYTE  bGameTypeQing;			// 2有门清 ，1 没门清
	BYTE  bPlayCoutIdex;			// 游戏局数
	BYTE  bGamePayType;				// 游戏付费类型
	BYTE  bQiangGangHu;				// 黄州的抢杠胡 0没有，1有
	BYTE  bAllOther;				// 溪水的全求人 0没有，1有
	BYTE  bMaxScore;				// 封顶分数
	BYTE  bPantherType;             // 风豹 将豹 单豹  色豹 
	BYTE  bBaoIsOutCard;            // 0 有宝不可打 1 有宝可打 2 无宝
	BYTE  bPiaoShu;				    // 飘数 0 不要 1 飘1 2 飘2... 10 飘10	
	BYTE  bPlayerNum;               // 游戏人数
	BYTE  bGameBaseScore;           // 游戏底分
};


//游戏记录
struct CMD_GP_GetGameRecord_List
{
	DWORD							dwUserID;							
};

//游戏记录
struct CMD_GP_GetGameTotalRecord
{
	DWORD							dwUserID;			
	DWORD							dwRecordID;						
};

//游戏记录
struct CMD_GP_GetGameChildRecord
{
	DWORD							dwUserID;			
	DWORD							dwRecordID;						
};



struct CMD_GP_BackAddBankAwardInfo
{
	INT							kRewardGold[3][LEN_ADDRANK];				
	INT							kRewardType[3][LEN_ADDRANK];				
};

struct CMD_GP_GetAddBank
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	INT								iRankIdex;
};


struct CMD_GP_BackAddBank
{
	int iRankIdex;
	std::vector<std::string>		kNickName;
	std::vector<int>				kUserID;
	std::vector<int>				kFaceID;
	std::vector<int>				kCustomID;
	std::vector<int>				kUserPoint;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(iRankIdex);
		Stream_VECTOR(kNickName);
		Stream_VECTOR(kUserID);
		Stream_VECTOR(kFaceID);
		Stream_VECTOR(kCustomID);
		Stream_VECTOR(kUserPoint);
	}
};

//话费兑换

struct CMD_GP_GetExchangeHuaFei
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};


struct CMD_GP_ExchangeDone
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	DWORD							dwExchangeID;						//兑换 I D
};

struct CMD_GP_BackExchangeHuaFei
{
	std::vector<int>				kExchangeID;            //兑换id
	std::vector<int>				kUseType;			   //兑换道具类型
	std::vector<int>				kUseNum;				   //兑换道具个数
	std::vector<int>				kGetType;			   //兑换商品类型
	std::vector<int>				kGetNum;				   //兑换商品个数
	std::vector<std::string>		kGoodsName;			   //兑换商品名称
	std::vector<std::string>		kExchangeDesc;		   //兑换描述
	std::vector<std::string>		kImgIcon;			   //图标名称
	std::vector<int>				kFlag;				   //标记

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kExchangeID);
		Stream_VECTOR(kUseType);
		Stream_VECTOR(kUseNum);
		Stream_VECTOR(kGetType);
		Stream_VECTOR(kGetNum);
		Stream_VECTOR(kGoodsName);
		Stream_VECTOR(kExchangeDesc);
		Stream_VECTOR(kImgIcon);
		Stream_VECTOR(kFlag);
	}
};

// 商城列表
struct CMD_GP_GetShopInfo
{
	DWORD						dwUserID;						//用户 I D
	char							szPassword[LEN_PASSWORD];			//用户密码
};

struct CMD_GP_UpShopOder
{
	DWORD						dwUserID;						//用户 I D
	DWORD						dwItemID;						//商品id
	DWORD						total_fee;						//金额
	std::string					appid;							//微信id
	std::string					mch_id;							//商户号
	std::string					prepay_id;						//准备订单号
	std::string					strOSType;						//系统号

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwUserID);
		Stream_VALUE(dwItemID);
		Stream_VALUE(total_fee);
		Stream_VALUE(appid);
		Stream_VALUE(mch_id);
		Stream_VALUE(prepay_id);
		Stream_VALUE(strOSType);
	}
};

//出售
struct CMD_GP_SellItem
{
	DWORD						dwSrcUserID;					
	std::string					strSrcPassword;					
	DWORD						dwDestUserID;					
	DWORD						dwItemType;						
	DWORD						dwItemValue;					

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwSrcUserID);
		Stream_VALUE(strSrcPassword);
		Stream_VALUE(dwDestUserID);
		Stream_VALUE(dwItemType);
		Stream_VALUE(dwItemValue);
	}
};

struct CMD_GP_BackShopInfo
{
	std::vector<int>				kItemID;					 //商品id
	std::vector<int>				kItemType;				 //商品类型
	std::vector<int>				kOrderID_IOS;				 //商品订单号 苹果
	std::vector<int>				kOrderID_Android;			 //商品订单号 安卓
	std::vector<int>				kPrice;					 //商品价格
	std::vector<int>				kGoodsNum;				 //商品数量
	std::vector<std::string>		kItemTitle;				 //标题
	std::vector<std::string>		kItemDesc;				 //描述
	std::vector<std::string>		kItemIcon;				 //图标名称
	std::vector<std::string>		kItemName;				 //商品名称

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VECTOR(kItemID);
		Stream_VECTOR(kItemType);
		Stream_VECTOR(kOrderID_IOS);
		Stream_VECTOR(kOrderID_Android);
		Stream_VECTOR(kPrice);
		Stream_VECTOR(kGoodsNum);
		Stream_VECTOR(kItemTitle);
		Stream_VECTOR(kItemDesc);
		Stream_VECTOR(kItemIcon);
		Stream_VECTOR(kItemName);
	}
};

//////////////////////////////////////////////////////////////////////////////////
//比赛报名
struct CMD_GP_MatchSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取消报名
struct CMD_GP_MatchUnSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//报名结果
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//报名标识
	bool							bSuccessed;							//成功标识
	TCHAR							szDescribeString[128];				//描述信息
};


//报名结果
struct CMD_GP_MatchGetAward
{
	DWORD							dwUserID;	
	DWORD							dwMatchID;		//比赛标识
	DWORD							dwMatchNO;		//比赛场次	
};

//排行信息
struct tagMatchAwardkInfo
{
	WORD							MatchRank;							//比赛名次
	DWORD							RewardGold;					//奖励金币
	DWORD							RewardMedal;						//奖励元宝
	DWORD							RewardExperience;					//奖励经验
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(MatchRank);
		Stream_VALUE(RewardGold);
		Stream_VALUE(RewardMedal);
		Stream_VALUE(RewardExperience);
	}
};

//比赛奖励
struct CMD_GR_MatchAwardList
{
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次
	std::vector<tagMatchAwardkInfo> kAwards;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwMatchID);
		Stream_VALUE(dwMatchNO);
		StructVecotrMember(tagMatchAwardkInfo,kAwards);
	}
};
//////////////////////////////////////////////////////////////////////////////////


//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改推荐人
struct CMD_GP_ModifySpreader
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szSpreader[LEN_NICKNAME];		//用户密码
};

//修改帐号
struct CMD_GP_ModifyAccounts
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//用户帐号
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwUserStarValue;					//评分
	DWORD							dwUserStarCout;						//评分
};

//个人信息
struct CMD_GP_UserAccountInfo
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定索引
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	DWORD							dwSpreaderID;						//推广ID
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
	TCHAR							szLogonIp[LEN_ACCOUNTS];			//登录IP

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//赠送返回
struct CMD_GP_AWARD_RETURN
{
	DWORD							dwUserID;
	DWORD							dwInsure;
};

//操作成功
struct CMD_GP_InGameSeverID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;			
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//游戏状态
struct CMD_GP_UserInGameServerID
{
	DWORD							dwUserID;							//用户 I D
};


//查询信息
struct CMD_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//用户 I D
};


//修改资料
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};
//修改评分
struct  CMD_GP_ModifyStarValue
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwStarValue;						//连接地址
};

//分享朋友圈奖励
struct CMD_GP_ShareAward
{
	DWORD							dwUserID;

};

//////////////////////////////////////////////////////////////////////////////////
//查询签到
struct CMD_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//签到信息
struct CMD_GP_CheckInInfo
{	
	WORD							wSeriesDate;						//连续日期
	WORD							wAwardDate;							//物品日期
	bool							bTodayChecked;						//签到标识
	SCORE							lRewardGold[LEN_SIGIN];				//奖励金额
	BYTE							lRewardType[LEN_SIGIN];				//奖励类型 1金币 2道具
	BYTE							lRewardDay[LEN_SIGIN];				//奖励天数 
};

//执行签到
struct CMD_GP_CheckInDone
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//签到结果
struct CMD_GP_CheckInResult
{
	bool							bType;								//是否是达到天数领取物品
	bool							bSuccessed;							//成功标识
	SCORE							lScore;								//当前金币
	TCHAR							szNotifyContent[128];				//提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//轮盘
//签到结果
struct CMD_GP_ReqRouletteUeserInfo
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwVaildDay;							//有效时间
};
struct CMD_GP_BackRouletteUeserInfo
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwHaveDone;
	BYTE							cbCout;
	SCORE							lRewardGold[LEN_ROULETTE];			//奖励金额
	BYTE							cbRewardType[LEN_ROULETTE];			//奖励类型 1金币 2房卡
	BYTE							cbRouletteIdex[LEN_ROULETTE];		//奖励索引
	BYTE							cbAwardPercent[LEN_ROULETTE];		//奖励索引
};
struct CMD_GP_ReqRouletteDone
{
	DWORD							dwUserID;							//用户标识
};
struct CMD_GP_BackRouletteDone
{
	DWORD							dwUserID;							//用户标识
	SCORE							lRewardGold;						//奖励金额
	BYTE							cbRewardType;						//奖励类型 1金币 2房卡
	BYTE							cbRewardIdex;						//奖励类型 1金币 2房卡

};
//////////////////////////////////////////////////////////////////////////////////
//新手活动

struct CMD_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};
struct CMD_GP_BeginnerInfo
{	
	WORD							wSeriesDate;						//连续日期
	bool							bTodayChecked;						//签到标识
	bool							bLastCheckIned;						//签到标识
	SCORE							lRewardGold[LEN_BEGINNER];			//奖励金额
	BYTE							lRewardType[LEN_BEGINNER];			//奖励类型 1金币 2道具
};
struct CMD_GP_BeginnerDone
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

struct CMD_GP_BeginnerResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lAwardCout;							//奖励数量
	SCORE							lAwardType;							//奖励类型
	TCHAR							szNotifyContent[128];				//提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//低保服务

//领取低保
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//低保参数
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	TCHAR							szNotifyContent[128];				//提示内容
};


//自定义字段查询 公告
struct CMD_GP_QueryNotice
{
	TCHAR							szKeyName[LEN_NICKNAME];			//关键字
};
struct CMD_GP_PublicNotice
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[512];				//成功消息
};
//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址
#define DTP_GP_UI_HEAD_HTTP			10									//头像
#define DTP_GP_UI_IP				11									//IP
#define DTP_GP_UI_CHANNEL			12									//渠道号
#define DTP_GP_UI_GPS				13									//GPS

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

// 充值记录
struct RechargeInsure
{
	DWORD							m_rechargeid;	// 充值记录id
	DWORD							m_insure_type;	// 购买商品类型 0:6 1:18...
	DWORD							m_add_insure;	// 充值金额		100豆
	DWORD							m_get_insure;	// 实际获得金额	120豆
	SYSTEMTIME						m_insure_time;	// 充值时间		 
	TCHAR							szRechargeKey[128];	//充值流水号 
};

struct CMD_GP_UserInsureHistory
{
	CMD_GP_UserInsureHistory()
	{
		memset(this, 0, sizeof(*this));
	}
	RechargeInsure					infos[100];
};



//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

// 查询充值信息SUB_GP_QUERY_USER_INSURE_HESTORY
struct CMD_GP_QueryInsureHestory
{
	DWORD							dwUserID;							//用户 I D
};


//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

//操作成功
struct CMD_GP_SpreaderResoult
{
	LONG							lResultCode;						//操作代码
	SCORE							lScore;
	TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_MB_LogonGameID
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};


//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////


//服务器di
struct    CMD_MB_Server_Address_Req
{
	DWORD          dwUserID;                                           //用户ID
	DWORD          dwClubID;										   //俱乐部ID
};
#pragma pack()

#endif
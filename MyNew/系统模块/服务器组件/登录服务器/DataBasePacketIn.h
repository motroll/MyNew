#ifndef DATA_BASE_PACKETIN_HEAD_FILE
#define DATA_BASE_PACKETIN_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//登录命令
#define	DBR_GP_LOGON_GAMEID			1									//I D 登录
#define	DBR_GP_LOGON_ACCOUNTS		2									//帐号登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号

//账号服务
#define DBR_GP_MODIFY_MACHINE		10									//修改机器
#define DBR_GP_MODIFY_LOGON_PASS	11									//修改密码
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_UNDER_WRITE	13									//修改签名
#define DBR_GP_MODIFY_INDIVIDUAL	14									//修改资料
#define DBR_GP_MODIFY_ACCOUNTS		15									//修改帐号
#define DBR_GP_MODIFY_SPREADER		16									//修改推荐人
#define DBR_GP_MODIFY_STARVALUE		17									//评分
#define DBR_GP_SHARE_AWARD			18									//分享朋友圈奖励

//头像命令
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//修改头像
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//修改头像

//银行命令
#define DBR_GP_USER_SAVE_SCORE		30									//存入游戏币
#define DBR_GP_USER_TAKE_SCORE		31									//提取游戏币
#define DBR_GP_USER_TRANSFER_SCORE	32									//转账游戏币
#define DBR_GR_CHECK_PAY_INFO	    33									// 支付

//查询命令
#define DBR_GP_QUERY_INDIVIDUAL		40									//查询资料
#define DBR_GP_QUERY_INSURE_INFO	41									//查询银行
#define DBR_GP_QUERY_USER_INFO	    42									//查询用户
#define DBR_GP_QUERY_ACCOUNTINFO	44									//查询资料
#define DBR_GP_USER_INGAME_SERVERID	45									//游戏状态
#define DBR_GP_USER_INSURE_HISTORY	46									//查询充值记录


//系统命令
#define DBR_GP_LOAD_GAME_LIST		50									//加载列表
#define DBR_GP_ONLINE_COUNT_INFO	51									//在线信息

//比赛命令
#define DBR_GP_MATCH_SIGNUP			55									//比赛报名
#define DBR_GP_MATCH_UNSIGNUP		56									//取消报名
#define DBR_GP_MATCH_AWARD			57									//比赛奖励

//签到命令
#define DBR_GP_LOAD_CHECKIN_REWARD	60									//签到奖励
#define DBR_GP_CHECKIN_DONE			61									//执行签到
#define DBR_GP_CHECKIN_QUERY_INFO	62									//查询信息
#define DBR_GP_CHECKIN_AWARD		64									//查询信息


#define DBR_GP_LOAD_BASEENSURE		70									//加载低保

#define DBR_GP_LOAD_BEGINNER		80									//加载新手引导
#define DBR_GP_BEGINNER_QUERY_INFO	81									//查询新手引导
#define DBR_GP_BEGINNER_DONE		82									//领取新手引导

	//轮盘命令
#define DBR_GP_LOAD_ROULETE_AWARD	90									//轮盘奖励
#define DBR_GP_ROULETE_DONE			91									//获取轮盘奖励
#define DBR_GP_ROULETE_USERINFO		92									//轮盘状态信息

//游戏设置
#define DBR_GP_GAME_CONFIG			95									//游戏设置

//低保命令
#define DBR_GP_BASEENSURE_TAKE		100									//领取低保

//赚金排行
#define DBR_GP_LOAD_ADDRANK_CONFIG	110									//更新
#define DBO_GP_ADDRANK_UP_AWARD		111									//是否领取排行奖励		
#define DBR_GP_GET_ADDRANK			112									//获得排行

//游戏记录
#define DBR_GP_GAME_RECORD_LIST		120									//游戏记录
#define DBR_GP_GAME_TOTAL_RECORD	121									//游戏回放
#define DBR_GP_GAME_TOTAL_CHILD		122									//游戏回放

//兑换话费	
#define DBR_GP_GET_EXCHANGE_HUAFEI	130							//获得兑换话费列表
#define DBR_GP_EXCHANGE_DONE		131							//兑换

//商城	
#define DBR_GP_GET_SHOPINFO			140									//获得商城列表
#define DBR_GP_GET_BUYODER			141	

//玩家出售	
#define DBR_GP_SELL_ITEM			150									//获得商城列表
#define DBR_GP_GET_SELL_RECORD		151										//购买

//俱乐部信息 phw
#define DBR_GR_CREATE_CLUB			    160		// 创建俱乐部
#define DBR_GR_JOIN_CLUB				161		// 申请加入俱乐部
#define DBR_GR_JOIN_CLUB_COMFIRM		163		// 会长同意或不同意加入俱乐部
#define DBR_GR_LEAVE_CLUB				164		// 离开俱乐部
#define DBR_GR_KICK_CLUB				165		// 踢出俱乐部
#define DBR_GR_INVITE_CLUB				166		// 邀请玩家加入俱乐部
#define DBR_GR_CLUB_INFO_MEMBER			167		// 自己俱乐部的信息
#define DBR_GR_GET_MY_CLUB_REQ          168     // 获取自己的俱乐部请求
#define DBR_GR_CLUB_MEMBER_LIST_REQ     169     // 俱乐部会员列表请求
#define DBR_GR_UPDATE_CLUB_REQ		    170     // 会长更新俱乐部请求
#define DBR_GR_DISMISS_CLUB_REQ			171		// 解散俱乐部请求
#define DBR_GR_CLUB_CHANGE_MASTER_REQ	172		// 解散俱乐部请求
#define DBR_GR_CLUB_SET_MANAGER_REQ		173		// 会长设置管理员请求

//自定义字段查询
#define DBR_GP_PUBLIC_NOTIC			200									//自定义字段查询

//////////////////////////////////////////////////////////////////////////////////

// 创建俱乐部
struct DBR_GR_Create_Club
{	
	DWORD dwUserID;					// 用户ID
	TCHAR stClubName[LEN_NICKNAME]; // 俱乐部名称
	BYTE bGameTypeIndex;			// 游戏类型 2罗田，3黄州晃晃，4溪水
	BYTE bGameTypeQing;				// 2有门清 ，1 没门清
	BYTE bPlayCoutIdex;				// 游戏局数
	BYTE bGamePayType;				// 游戏付费类型
	BYTE bQiangGangHu;				// 黄州的抢杠胡 0没有，1有
	BYTE bAllOther;					// 溪水的全求人 0没有，1有
	BYTE bMaxScore;					// 封顶分数
	BYTE bPantherType;				// 将豹 风豹 单豹  色豹 
	BYTE bBaoIsOutCard;             // 0 有宝不可打 1 有宝可打 2 无宝
	BYTE bPiaoShu;				    // 飘数 0 不要 1 飘1 2 飘2... 10 飘10	
	BYTE bPlayerNum;                // 游戏人数
	BYTE bGameBaseScore;            // 游戏底分
};

// 申请加入俱乐部
struct DBR_GR_Join_Club
{
	DWORD dwUserID; // 用户ID
	DWORD dwClubID; // 俱乐部ID
};

// 会长同意或不同意加入俱乐部
struct DBR_GR_Jion_Club_Comfirm 
{	
	DWORD dwOperateID; //操作者ID
	DWORD dwUserID; // 待审核的用户ID
	DWORD dwClubID; // 申请俱乐部的ID
	BYTE  bComfirm; // 1同意，2拒绝							
};

// 离开俱乐部
struct DBR_GR_Leave_Club
{	
	DWORD dwUserID; // 申请者的用户ID
	DWORD dwClubID; // 离开俱乐部的ID						
};

// 踢出俱乐部
struct DBR_GR_Kick_Club
{	
	DWORD dwRequestID; // 申请者ID
	DWORD dwUserID; // 踢出用户的ID
	DWORD dwClubID; // 俱乐部ID							
};

// 邀请玩家加入俱乐部
struct DBR_GR_Invite_Club
{	
	DWORD dwRequestID; // 申请者ID
	DWORD dwUserID; // 被邀请的用户ID
	DWORD dwClubID; // 俱乐部ID					
};

// 自己俱乐部的信息
struct DBR_GR_Club_Info_Member
{	
	DWORD dwClubID; // 俱乐部ID
};
// 获取自己的俱乐部信息请求
struct  DBR_GR_Get_My_Club_Req
{
	DWORD                           dwUserID;                          // 俱乐部ID
};
// 获取自己的俱乐部信息回应
struct  DBO_GR_Get_My_Club_Res
{
	DWORD				cbAgent;					   // 是否代理 不是0，是1
	DWORD               dwUserID;                      // 用户ID
	DWORD               dwClubNum;                     // 自己的俱乐部数量 
	DWORD               dwClubId[5];                   // 俱乐部ID
	TCHAR               szClubName[5][LEN_NICKNAME];   // 俱乐部名字
	DWORD               dwMemberNum[5];                // 成员数量
	DWORD               dwMasterId[5];                 // 会长ID
	DWORD				dwClubManagerId[5][3];         // 自己是否是俱乐部里面的成员
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
	BYTE				cbBaoIsOutCard[5];			   // 宝 0:不可打  1:可打 2:无宝
	BYTE				cbPiaoShu[5];                  // 飘数 0 不要 1 飘1 2 飘2... 10 飘10
	BYTE                cbGameBaseScore[5];            // 黄梅底分
	BYTE				cbGameMaxScore[5];			   // 封顶分数
	SYSTEMTIME			dCreateTime[5];		           // 创建俱乐部的时间
	DWORD				dwOnlineNum[5];				   // 成员在线人数
	
};

// DBR_GR_CLUB_MEMBER_LIST_REQ     169     // 俱乐部会员列表请求
struct  DBO_GR_Club_Member_List_Req 
{
	DWORD dwUserID; // 会长ID
	DWORD dwClubID; // 俱乐部ID
	DWORD dwFlag; // 0请求申请列表，1请求会员列表
};

// DBR_GR_UPDATE_CLUB_REQ       170     会长更新俱乐部请求
struct  DBR_GR_Update_Club_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	BYTE bGameTypeIndex;			// 游戏类型 2罗田，3黄州晃晃，4溪水
	BYTE bGameTypeQing;				// 2有门清 ，1 没门清
	BYTE bPlayCoutIdex;				// 游戏局数
	BYTE bGamePayType;				// 游戏付费类型
	BYTE bMaxScore;                 // 封顶分数
	BYTE bQiangGangHu;				// 黄州的抢杠胡 0没有，1有
	BYTE bAllOther;					// 溪水的全求人 0没有，1有
	BYTE bPantherType;				// 风 将 单 色 豹
	BYTE bPlayerNum;				// 游戏人数
	BYTE bBaoIsOutCard;             // 宝 0:不可打  1:可打 2:无宝
	BYTE bPiaoShu;                  // 飘数 0 不要 1 飘1 2 飘2... 10 飘10	
	BYTE bGameBaseScore;            // 游戏底分
	TCHAR stClubName[LEN_NICKNAME]; // 俱乐部名称
	TCHAR stClubNotice[124];        // 公告
};

// DBR_GR_DISMISS_CLUB_REQ			171		 解散俱乐部请求
struct DBR_GR_Dismiss_Club_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
};

// DBR_GR_CLUB_CHANGE_MASTER_REQ			172		 转让会长请求
struct DBR_GR_Club_Change_Master_Req 
{
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	DWORD dwMasterID;				// 下个会长ID(转让后的)
};

// DBR_GR_CLUB_SET_MANAGER_REQ		173		 会长设置管理员请求
struct DBR_GR_Club_Set_Manager
{	
	DWORD dwUserID;					// 会长ID
	DWORD dwClubID;					// 俱乐部ID
	DWORD dwManagerID;				// 管理员ID
};


//ID 登录
struct DBR_GP_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//游客登录
struct DBR_GP_LogonVisitor
{
	DWORD							dwClientIP;							//连接地址
	TCHAR							szComputerID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szNickName[LEN_ACCOUNTS];			//人名
	TCHAR							szPassWord[LEN_MD5];				//登录密码
	TCHAR							szPassWordBank[LEN_MD5];			//登录密码
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	WORD							wFaceID;							//注册头像
	BYTE							cbGender;							//用户性别
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//修改机器
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyAccounts
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//用户帐号
};

//修改推荐人
struct DBR_GP_ModifySpreader
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szSpreader[LEN_ACCOUNTS];			//用户帐号
};

//修改签名
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//修改头像
struct DBR_GP_ModifySystemFace
{
	//用户信息
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct DBR_GP_ModifyCustomFace
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改资料
struct DBR_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码

	//帐号资料
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//用户信息
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
	
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//头像HTTP
	TCHAR							szUserChannel[LEN_NICKNAME];		//渠道号
	
	TCHAR							szUserGps[LEN_NICKNAME];			//GPS
};

//修改资料
struct DBR_GP_ModifyStarValue
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwStarValue;						//连接地址
};

//分享朋友圈奖励
struct DBR_GP_ShareAward
{
	DWORD							dwUserID;
};

//查询用户游戏状态
struct DBR_GP_QueryUserInGameServerID
{
	DWORD							dwUserID;							//用户 I D
};

//查询资料
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询个人信息
struct DBR_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//存入游戏币
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账游戏币
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账游戏币
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询银行
struct DBR_GP_QueryInsureHestory
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询用户
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户资料
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};

//查询签到
struct DBR_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//执行签到
struct DBR_GP_CheckInDone
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

////////////////////////////////////////////////////////////////////////////////////////////
//新手引导查询
struct DBR_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};
struct DBR_GP_BeginnerDone
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

////////////////////////////////////////////////////////////////////////////////////////////

//低保参数
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//低保结果
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//成功标识
	SCORE							lGameScore;							//当前游戏币
	TCHAR							szNotifyContent[128];				//提示内容
};

//领取低保
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//报名结果
struct DBO_GP_MatchSignupResult
{
	bool							bSignup;							//报名标识
	bool							bSuccessed;							//成功标识
	TCHAR							szDescribeString[128];				//描述信息
};


//加载奖励
struct DBR_GR_LoadMatchReward
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次	
};

//修改机器
struct DBR_GP_PublicNotic
{
	TCHAR							szKeyName[LEN_NICKNAME];			//关键字
};

/////////////////////////////////////
//轮盘

//加载奖励
struct DBR_GR_ReqUserRouletteAward
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwVaildDay;							//有效时间
};

//轮盘信息
struct DBR_GP_RouletteDone
{
	DWORD							dwUserID;					//用户 I D
	SCORE							lRewardGold;				//奖励金额
	BYTE							cbRewardType;				//奖励类型 1金币 2房卡
	BYTE							cbRewardIdex;				//奖励类型 1金币 2房卡
};

/////////////////////////////////////
//赚金榜
struct DBR_GP_GetAddBankConfig
{
	INT								iIdex;
};
//获得排行榜
struct DBR_GP_GetAddBank
{
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	INT								iIdex;
};


//比赛报名
struct DBR_GP_MatchSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛报名
struct DBR_GP_MatchUnSignup
{
	//比赛信息
	WORD							wServerID;							//房间标识
	DWORD							dwMatchID;							//比赛标识
	DWORD							dwMatchNO;							//比赛场次

	//用户信息
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};


//游戏记录
struct DBR_GP_GameRecordList
{
	DWORD							dwUserID;							
};


//游戏回放
struct DBR_GP_GetGameTotalRecord
{
	DWORD							dwUserID;		
	DWORD							dwRecordID;							
};

//游戏回放
struct DBR_GP_GetGameChildRecord
{	
	DWORD							dwRecordID;							
};

//话费兑换列表
struct DBR_GP_GetExchangeHuaFei
{
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
};

//话费兑换
struct DBR_GP_ExchangeDone
{
	//登录信息
	DWORD							dwUserID;							//用户 ID
	DWORD							dwExchangeID;						//兑换 ID
	TCHAR							szPassword[LEN_MD5];				//登录密码
};

//商城
struct DBR_GP_GetShopInfo
{
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
};
//商城
struct DBR_GP_UpShopOder
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
struct DBR_GP_SellItem
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

//出售
struct DBR_GP_GetSellRecordList
{
	DWORD						dwUserID;
};



#endif
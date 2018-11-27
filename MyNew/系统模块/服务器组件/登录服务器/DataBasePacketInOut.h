#ifndef DATA_BASE_PACKETOUT_HEAD_FILE
#define DATA_BASE_PACKETOUT_HEAD_FILE

#include "..\..\ȫ�ֶ���\datastream.h"

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBR_GP_VALIDATE_MBCARD		102									//��¼ʧ��

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����
#define DBO_GP_USER_ACCOUNTINFO		112									//�û���Ϣ
#define DBO_GP_INSURE_RETURN		114									//��������

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO   123								//�û�����
#define DBO_GP_USER_INGAME_SERVER_ID   124								//�û���Ϸ״̬
#define DBO_GR_USER_PAY_SUCCESS   125								//֧��
#define DBO_GR_USER_INSURE_HISTORY   126								// ��ֵ��¼

//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM		132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM		133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT		134									//���ؽ��

//ǩ������
#define DBO_GP_CHECKIN_REWARD		160									//ǩ������
#define DBO_GP_CHECKIN_INFO			161									//ǩ����Ϣ
#define DBO_GP_CHECKIN_RESULT		162									//ǩ�����


#define DBO_GP_BASEENSURE_PARAMETER	180									//�ͱ�����
#define DBO_GP_BASEENSURE_RESULT	181									//��ȡ���
//�������
#define DBO_GP_MATCH_SIGNUP_RESULT	190									//�������	
#define DBO_GP_MATCH_AWARD			191									//��������

//������
#define DBO_GP_PUBLIC_NOTICE		200									//�����ɹ�

//���ֻ����
#define DBO_GP_BEGINNER_CONFIG		210									//���ֻ����
#define DBO_GP_BEGINNER_INFO		211									//���ֻ��Ϣ
#define DBO_GP_BEGINNER_RESULT		212									//���ֻ���

//��������
#define DBO_GP_ROULETE_AWAED		90									//���̽���
#define DBO_GP_ROULETE_USERINFO		91									//����״̬��Ϣ

//��Ϸ����
#define DBO_GP_GAME_CONFIG			95									//��Ϸ����

//�����Ƽ��˽��
#define DBO_GP_SPREADER_RESOULT		220									//�����Ƽ��˽��

//׬���
#define DBO_GP_ADDRANK_AWARD_CONFIG	240									//����							
#define DBO_GP_ADDRANK_RANK_BACK	241									//���н��						

//��Ϸ��¼
#define DBO_GP_GAME_RECORD_LIST		250									//��¼�б�							
#define DBO_GP_GAME_RECORD_TOTAL	251									//						
#define DBO_GP_GAME_RECORD_CHILD	252									//

//�һ�����
#define DBO_GP_EXCHAGE_HUAFEI_BACK	260									//�һ������б�

//�̳���Ϣ
#define DBO_GP_SHOPINFO_BACK	270									//�̳��б�		

//������Ϣ
#define DBO_GP_SELLLIST_BACK	280									//�̳��б�	

//������
#define DBO_GP_OPERATE_SUCCESS		500									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE		501									//����ʧ��

//���ֲ����
#define DBO_GP_CLUB_INFO_MEMBER           701                   //�Լ����ֲ�����Ϣ
#define DBO_GP_CREATE_JOIN_RES            702                   //������������ֲ���Ӧ
#define DBO_GP_GET_MY_CLUB_RES            703                   //��ȡ���ֲ���Ӧ��Ϣ
#define DBO_GP_CLUB_MEMBER_LIST_RES       704                   //�᳤���ֲ���Ա�б��Ӧ

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	BYTE							cbInsureEnabled;					//����ʹ��
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szPictureName[LEN_PICTURE];			//�ͼƬ
	DWORD							dwPresent[LEN_PRESENT];				//����͵���

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//������Ϣ
	WORD							wSignupCount;						//��������
	tagSignupMatchInfo				SignupMatchInfo[32];				//�����б�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ�����
};

//���ͷ���
struct DBO_GP_AWARD_RETURN
{
	DWORD							dwUserID;
	DWORD							dwInsure;
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//ͷ��
	TCHAR							szLogonIP[LEN_NICKNAME];			//IP
	TCHAR							szUserChannel[LEN_NICKNAME];		//������
	TCHAR							szUserGps[LEN_NICKNAME];			//GPS
	DWORD							dwUserStarValue;			//����
	DWORD							dwUserStarCout;			//����
};


//������Ϣ
struct DBO_GP_UserAccountInfo
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szLogonIp[LEN_ACCOUNTS];			//��¼IP

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};
//��������
struct DBO_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lDestScore;							//��Ϸ�ұ仯
	SCORE							lDestInsure;					    //���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����Ƽ��˽��
struct DBO_GP_SpreaderResoult
{
	LONG							lResultCode;						//��������
	SCORE							lScore;							//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

// ��ֵ��¼
struct SRechargeInsure
{
	DWORD							m_rechargeid;	// ��ֵ��¼id
	DWORD							m_insure_type;	// ������Ʒ���� 0:6 1:18...
	DWORD							m_add_insure;	// ��ֵ���		100��
	DWORD							m_get_insure;	// ʵ�ʻ�ý��	120��
	SYSTEMTIME						m_insure_time;	// ��ֵʱ��		 
	TCHAR							szRechargeKey[128];	//��ֵ��ˮ��
};

struct DBO_GP_UserInsureHistory
{
	DBO_GP_UserInsureHistory()
	{
		memset(this, 0, sizeof(*this));
	}
	SRechargeInsure					infos[100];
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

// ��ֵ��¼
struct tagPayInfo
{
	WORD							rid;								//���ID
	DWORD							pid;								//��ƷID
	DWORD							isband;								//�Ƿ��
	TCHAR							szOrderId[100];						//����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

//ǩ������
struct DBO_GP_CheckInReward
{
	SCORE							lRewardGold[LEN_SIGIN];				//�������
	BYTE							lRewardType[LEN_SIGIN];				//�������� 1��� 2����
	BYTE							lRewardDay[LEN_SIGIN];				//�������� 
};

//ǩ����Ϣ
struct DBO_GP_CheckInInfo
{
	WORD							wSeriesDate;						//��������
	WORD							wAwardDate;							//��Ʒ����
	bool							bTodayChecked;						//ǩ����ʶ
};

//ǩ�����
struct DBO_GP_CheckInResult
{
	bool							bType;								//�Ƿ��Ǵﵽ������ȡ��Ʒ
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lScore;								//��ǰ����
	TCHAR							szNotifyContent[128];				//��ʾ����
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//����������Ϣ
struct DBO_GP_BeginnerInfo
{
	WORD							wSeriesDate;						//��������
	bool							bTodayChecked;						//ǩ����ʶ
	bool							bLastCheckIned;						//ǩ����ʶ
};

//��ѯ���
struct DBO_GP_UserInGameServerID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;							
};

//��ѯ���
struct DBO_GP_PublicNotice
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[512];				//�ɹ���Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//������Ϣ
struct DBO_GP_RouletteAward
{
	BYTE							cbCout;
	SCORE							lRewardGold[LEN_ROULETTE];				//�������
	BYTE							cbRewardType[LEN_ROULETTE];				//�������� 1��� 2����
	BYTE							cbRouletteIdex[LEN_ROULETTE];			//��������
	BYTE							cbAwardPercent[LEN_ROULETTE];			//��������
};

struct DBO_GP_RouletteUserInfo
{
	DWORD							dwUserID;					//�û� I D
	DWORD							dwHaveDone;
};
//////////////////////////////////////////////////////////////////////////////////

//���ֻ����
struct DBO_GP_BeginnerCofig
{
	SCORE							lRewardGold[LEN_BEGINNER];				//�������
	BYTE							lRewardType[LEN_BEGINNER];				//�������� 1��� 2����
};
//���ֻ���
struct DBO_GP_BeginnerResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lAwardCout;							//��������
	SCORE							lAwardType;							//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//׬���
struct DBO_GP_AddRankAwardConfig
{
	INT								iIdex;
	INT								kRewardGold[LEN_ADDRANK];
	INT								kRewardType[LEN_ADDRANK];
};

struct DBO_GP_BackAddBank
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



//�һ�����
struct DBO_GP_BackExchangeHuaFei
{
	std::vector<int>				kExchangeID;            //�һ�id
	std::vector<int>				kUseType;			   //�һ���������
	std::vector<int>				kUseNum;				   //�һ����߸���
	std::vector<int>				kGetType;			   //�һ���Ʒ����
	std::vector<int>				kGetNum;				   //�һ���Ʒ����
	std::vector<std::string>		kGoodsName;			   //�һ���Ʒ����
	std::vector<std::string>		kExchangeDesc;		   //�һ�����
	std::vector<std::string>		kImgIcon;			   //ͼ������
	std::vector<int>				kFlag;				   //���

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


//�̳��б�
struct DBO_GP_BackShopInfo
{
	std::vector<int>				kItemID;					 //��Ʒid
	std::vector<int>				kItemType;				 //��Ʒ����
	std::vector<int>				kOrderID_IOS;				 //��Ʒ������ ƻ��
	std::vector<int>				kOrderID_Android;			 //��Ʒ������ ��׿
	std::vector<int>				kPrice;					 //��Ʒ�۸�
	std::vector<int>				kGoodsNum;				 //��Ʒ����
	std::vector<std::string>		kItemTitle;				 //����
	std::vector<std::string>		kItemDesc;				 //����
	std::vector<std::string>		kItemIcon;				 //ͼ������
	std::vector<std::string>		kItemName;				 //��Ʒ����

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

//������Ϣ
struct tagDBMatchAwardkInfo
{
	WORD							MatchRank;							//��������
	SCORE						RewardGold;					//�������
	DWORD							RewardMedal;						//����Ԫ��
	DWORD							RewardExperience;					//��������
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(MatchRank);
		Stream_VALUE(RewardGold);
		Stream_VALUE(RewardMedal);
		Stream_VALUE(RewardExperience);
	}
};

//��������
struct DBO_GR_MatchAwardList
{
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
	std::vector<tagDBMatchAwardkInfo> kAwards;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwMatchID);
		Stream_VALUE(dwMatchNO);
		StructVecotrMember(tagDBMatchAwardkInfo,kAwards);
	}
};


//�̳��б�
struct DBO_GP_BackSellOderItem
{
	int							DestUserID;					 
	int							kItemType;				 
	int							kItemValue;				
	std::string					DestUserName;		
	SYSTEMTIME					kPlayTime;		 

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(DestUserID);
		Stream_VALUE(kItemType);
		Stream_VALUE(kItemValue);
		Stream_VALUE(DestUserName);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
	}
};
struct DBO_GP_BackSellOderList
{			
	std::vector<DBO_GP_BackSellOderItem>	kItems;		 

	void StreamValue(datastream& kData,bool bSend)
	{
		StructVecotrMember(DBO_GP_BackSellOderItem,kItems);
	}
};

//���ֲ����

// DBO_GP_CREATE_JOIN_RES     702    ������������ֲ���Ӧ
struct  DBO_GP_Create_Join_Res
{
	DWORD dwCreateJoin; // 1 ������2 �������
	DWORD dwError; // �����ʶ ������ʧ�ܣ�0�ɹ�
	TCHAR szDescribeString[128]; //������Ϣ
};

// DBO_GP_CLUB_MEMBER_LIST_RES      704       //�᳤���ֲ���Ա�б��Ӧ
struct DBO_GP_Club_Mmember_List_Res
{
	DWORD bFlag;                                 // ������ǣ�0û���� 1����
	std::vector<int>                iUserID;     // �û�id; 
	std::vector<std::string>        szHeadHttp0; // �û�ͷ��; 
	std::vector<std::string>		strClubName; // ���ֲ�����; 
	std::vector<std::string>		strUserName; // �û�����
	std::vector<std::string>		strJoinTime; // ����ʱ��

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

#pragma pack()
#endif
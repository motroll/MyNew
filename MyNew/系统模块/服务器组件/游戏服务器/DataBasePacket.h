#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//�û��¼�
#define	DBR_GR_LOGON_USERID			100									//I D ��¼
#define	DBR_GR_LOGON_MOBILE			101									//�ֻ���¼
#define	DBR_GR_LOGON_ACCOUNTS		102									//�ʺŵ�¼

//ϵͳ�¼�
#define DBR_GR_WRITE_GAME_SCORE		200									//��Ϸ����
#define DBR_GR_LEAVE_GAME_SERVER	201									//�뿪����
#define DBR_GR_GAME_SCORE_RECORD	202									//���ּ�¼
#define DBR_GR_MANAGE_USER_RIGHT	203									//Ȩ�޹���
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//ϵͳ��Ϣ
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//���д�
#define DBR_GR_PRIVATE_GAME_RECORD	206									//���д�

//�����¼�
#define DBR_GR_LOAD_PARAMETER		300									//���ز���
#define DBR_GR_LOAD_GAME_COLUMN		301									//�����б�
#define DBR_GR_LOAD_ANDROID_USER	302									//���ػ���
#define DBR_GR_LOAD_GAME_PROPERTY	303									//���ص���

//�����¼�
#define DBR_GR_USER_SAVE_SCORE		400									//������Ϸ��
#define DBR_GR_USER_TAKE_SCORE		401									//��ȡ��Ϸ��
#define DBR_GR_USER_TRANSFER_SCORE	402									//ת����Ϸ��
#define DBR_GR_QUERY_INSURE_INFO	403									//��ѯ����
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//��ѯ�û�
#define DBR_GR_CHECK_PAY_INFO	    405									// ֧��

//��Ϸ�¼�
#define DBR_GR_PROPERTY_REQUEST		500									//��������
#define DBR_GR_GAME_FRAME_REQUEST	502									//��Ϸ����

//�����¼�
#define DBR_GR_MATCH_SIGNUP			600									//��������
#define DBR_GR_MATCH_UNSIGNUP		601									//�˳�����
#define DBR_GR_MATCH_START			602									//������ʼ
#define DBR_GR_MATCH_OVER			603									//��������	
#define DBR_GR_MATCH_REWARD			604									//��������
#define DBR_GR_MATCH_ENTER_FOR		605									//��������
#define DBR_GR_MATCH_ELIMINATE      606									//������̭

//��Ϸ�¼�
#define DBR_GR_BEGINNER_GAME_TIME	700		

//˽�˳��¼�
#define DBR_GR_PRIVATE_INFO					800			   //˽�˳���Ϣ
#define DBR_GR_CREATE_PRIVAT				801			   //����˽�˳�
#define DBR_GR_CREATE_PRIVAT_COST			802			   //����˽�˳�

//���ֲ��¼�
#define  DBR_GR_JOIN_CLUB_ROM               810            // ������ֲ�����
#define  DBR_GR_JOIN_CLUB_ROM_PRIVATE       811            // ͨ��˽�˳���ʽ������ֲ�����
#define  DBR_GR_CLUB_ROM_PRIVATE_RES        812            // ͨ��˽�˳���ʽ������ֲ����䷵��0
#define	 DBR_GR_CLUB_JOIN_CLUB_ROM_REQ		813			   // ����Ƿ���Խ�����ֲ�����
#define	DBR_GR_CREATE_FLOOR_STRESS_REQ      814			   // �������ֲ�¥��
#define  DBR_GR_CLUB_UPDATE_FLOOR_REQ       815            // ����

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�߼��¼�
#define DBO_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GR_LOGON_FAILURE		101									//��¼ʧ��

//�����¼�
#define DBO_GR_GAME_PARAMETER		200									//������Ϣ
#define DBO_GR_GAME_COLUMN_INFO		201									//�б���Ϣ
#define DBR_GR_GAME_ANDROID_INFO	202									//������Ϣ
#define DBO_GR_GAME_PROPERTY_INFO	203									//������Ϣ

//��������
#define DBO_GR_USER_INSURE_INFO		300									//��������
#define DBO_GR_USER_INSURE_SUCCESS	301									//���гɹ�
#define DBO_GR_USER_INSURE_FAILURE	302									//����ʧ��
#define DBO_GR_USER_INSURE_USER_INFO   303								//�û�����
#define DBO_GR_USER_PAY_SUCCESS	304									    //��ֵ�ɹ�

//��Ϸ�¼�
#define DBO_GR_PROPERTY_SUCCESS		400									//���߳ɹ�
#define DBO_GR_PROPERTY_FAILURE		401									//����ʧ��
#define DBO_GR_GAME_FRAME_RESULT	402									//��Ϸ���

//�����¼�
#define DBO_GR_MATCH_EVENT_START	500									//������ʶ
#define DBO_GR_MATCH_SIGNUP_RESULT	500									//�������
#define DBO_GR_MATCH_UNSIGNUP_RESULT 501								//�������
#define DBO_GR_MATCH_RANK_LIST		502									//��������
#define DBO_GR_MATCH_REWARD_RESULT  503									//�������
#define DBO_GR_MATCH_EVENT_END		599									//������ʶ

//ϵͳ�¼�
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600								//��Ϣ���
#define DBO_GR_SENSITIVE_WORDS		    601								//���д�

//˽�˳��¼�
#define DBO_GR_PRIVATE_EVENT_START	700									//˽�˳���ʶ
#define DBO_GR_PRIVATE_INFO			701									//˽�˳���Ϣ	
#define DBO_GR_CREATE_PRIVATE	    702									//˽�˳���Ϣ	
#define DBO_GR_PRIVATE_EVENT_END	799									//˽�˳���ʶ

//���ֲ��¼�
#define  DBO_GR_JOIN_CLUB_ROM           710                              //������ֲ�������Ϣ
#define  DBO_GR_CLUB_ROM_PRIVATE_RES    711                              //ͨ��˽�˳���ʽ������ֲ������ж�
#define  DBR_GR_CLUB_JOIN_CLUB_ROM_RES  712                              //����Ƿ��ܽ��뷿��Ļ�Ӧ
#define  DBR_GR_CLUB_CREATE_FLOOR_RES   713                              //�������ֲ�¥���Ӧ
#define  DBR_GR_CLUB_UPDATE_FLOOR_RES   714                              //���¾��ֲ�¥���Ӧ

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GR_LogonUserID
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//�������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//ID ��¼
struct DBR_GR_LogonMobile
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//�������
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct DBR_GR_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ����
struct DBR_GR_WriteGameScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//�û���Ϣ
	DWORD							dwDBQuestID;						//�����ʶ
	DWORD							dwInoutIndex;						//��������

	//�ɼ�����
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//������Ϣ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
};

//����������¼
struct DBR_GR_Beginner_InGame
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
};

//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwLeaveReason;						//�뿪ԭ��
	DWORD							dwOnLineTimeCount;					//����ʱ��

	//�ɼ�����
	tagVariationInfo				RecordInfo;							//��¼��Ϣ
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//ϵͳ��Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

struct DBR_GR_GameScoreRecord
{
	DBR_GR_GameScoreRecord()
	{
		wTableID = 0;
		wUserCount = 0;
		wAndroidCount = 0;
		lWasteCount = 0;
		lRevenueCount = 0;
		dwUserMemal = 0;
		dwPlayTimeCount = 0;
		wRecordCount = 0;
	}
	//������Ϣ
	WORD							wTableID;							//���Ӻ���
	WORD							wUserCount;							//�û���Ŀ
	WORD							wAndroidCount;						//������Ŀ

	DWORD							dwPrivateID;						//˽�˷���ʶ
	
	//���˰��
	SCORE							lWasteCount;						//�����Ŀ
	SCORE							lRevenueCount;						//˰����Ŀ

	//ͳ����Ϣ
	DWORD							dwUserMemal;						//������Ŀ
	DWORD							dwPlayTimeCount;					//��Ϸʱ��

	//ʱ����Ϣ
	SYSTEMTIME						SystemTimeStart;					//��ʼʱ��
	SYSTEMTIME						SystemTimeConclude;					//����ʱ��

	//���ּ�¼
	WORD							wRecordCount;						//��¼��Ŀ
	datastream						dataGameDefine;
	std::vector<tagGameScoreRecord>	GameScoreRecord;			//��Ϸ��¼
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wTableID);
		Stream_VALUE(wUserCount);
		Stream_VALUE(wAndroidCount);
		Stream_VALUE(dwPrivateID);
		Stream_VALUE(lWasteCount);
		Stream_VALUE(lRevenueCount);
		Stream_VALUE(dwUserMemal);
		Stream_DATA(dataGameDefine);
		Stream_VALUE_SYSTEMTIME(SystemTimeStart);
		Stream_VALUE_SYSTEMTIME(SystemTimeConclude);
		StructVecotrMember(tagGameScoreRecord,GameScoreRecord);
	}
};

//˽�˳���¼
struct BR_GR_PrivateRandTotalRecord
{
	DWORD						dwPrivateID;	//˽�˷���ʶ
	int							iRoomNum;
	std::vector<int>			kUserID;
	std::vector<std::string>	kNickName;
	std::vector<int>			kToatlScore;
	SYSTEMTIME					kPlayTime;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwPrivateID);
		Stream_VALUE(iRoomNum);
		Stream_VECTOR(kUserID);
		Stream_VECTOR(kNickName);
		Stream_VECTOR(kToatlScore);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
	}
};
//������Ϸ��
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	DWORD							dwUserID;							//�û� I D
};

//��������
struct DBR_GR_PropertyRequest
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;			            //ʹ�÷�Χ 
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//�û�Ȩ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//ϵͳ��Ϣ
	WORD							wTableID;							//���Ӻ���
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��¼�ɹ�
struct DBO_GR_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	DWORD							dwCustomID;							//�Զ�ͷ��
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR                           szHeadHttp[256];                    //΢��ͷ��

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	
	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	LONG							lLoveLiness;						//�û�����

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//��������
	DWORD							dwInoutIndex;						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//�ֻ�����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct DBO_GR_GameParameter
{
	//������Ϣ
	WORD							wMedalRate;							//���ƻ���
	WORD							wRevenueRate;						//����˰��

	//�汾��Ϣ
	DWORD							dwClientVersion;					//�ͻ��汾
	DWORD							dwServerVersion;					//����汾
};

//�б���Ϣ
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//�б���Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//������Ϣ
};

//������Ϣ
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//������Ϣ
};

//��������
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lFrozenedScore;						//�������
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û�����
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//���߳ɹ�
struct DBO_GR_S_PropertySuccess
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//��ԱȨ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//�����Ϣ
	SCORE							lConsumeGold;						//������Ϸ��
	LONG							lSendLoveLiness;					//��������
	LONG							lRecvLoveLiness;					//��������

	//��Ա��Ϣ
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//����ʧ��
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//����Χ
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�������
struct DBO_GR_MatchSingupResult
{
	bool							bResultCode;						//�������
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lCurrGold;							//��ǰ���
	SCORE							lCurrIngot;							//��ǰԪ��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct tagMatchRankInfo
{
	WORD							wRankID;							//��������
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lMatchScore;						//��������
	SCORE							lRewardGold;						//�������
	DWORD							dwRewardIngot;						//����Ԫ��
	DWORD							dwRewardExperience;					//��������
};

// ��ֵ��¼
struct tagPayInfo
{
	WORD							rid;								//���ID
	DWORD							pid;								//��ƷID
	TCHAR							szOrderId[100];						//����
};


//��������
struct DBO_GR_MatchRankList
{
	WORD							wUserCount;							//�û���Ŀ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
	tagMatchRankInfo				MatchRankInfo[128];					//������Ϣ
};

//Ȩ�޹���
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��
	bool							bGameRight;							//��ϷȨ��
};

//Ȩ�޹���
struct DBR_GR_ManageMatchRight
{	
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��	
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
};

//��������
struct DBR_GR_MatchSignup
{
	//��������
	DWORD							dwUserID;							//�û�ID
	SCORE							lMatchFee;							//��������

	//ϵͳ��Ϣ
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwMatchID;							//����ID
	DWORD							dwMatchNO;							//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������ʼ
struct DBR_GR_MatchStart
{
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������	
};

//��������
struct DBR_GR_MatchOver
{
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������	
	SYSTEMTIME						MatchStartTime;						//��ʼʱ��
	SYSTEMTIME						MatchEndTime;						//����ʱ��
};

//������̭
struct DBR_GR_MatchEliminate
{
	DWORD							dwUserID;							//�û���ʶ
	WORD							wServerID;							//�����ʶ
	BYTE							cbMatchType;						//��������
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������	
};


//��������
struct DBR_GR_MatchReward
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lRewardGold;						//�������
	DWORD							dwRewardIngot;						//����Ԫ��
	DWORD							dwRewardExperience;					//�û�����	
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//�������
struct DBR_GR_MatchRewardResult
{
	bool							bResultCode;						//�������
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lCurrGold;							//��ǰ���
	SCORE							lCurrIngot;							//��ǰԪ��
	DWORD							dwCurrExperience;					//��ǰ����
};



//ϵͳ��Ϣ
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //��ϢID
	BYTE							cbMessageType;						//��Ϣ����
	BYTE                            cbAllRoom;                          //ȫ�巿��
	DWORD							dwTimeRate;						    //ʱ��Ƶ��
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};
//////////////////////////////////////////////////////////////////////////////////

//˽�˳���Ϣ
struct DBR_GR_Private_Info
{
	WORD							wKindID;
};


//����˽�˳�
struct DBR_GR_Create_Private
{
	WORD							wKindID;
	DWORD							dwUserID;
	DWORD							dwCost;
	DWORD							dwCostType;
	BYTE							cbRoomType;
	DWORD							dwAgaginTable;
	BYTE							bPlayCoutIdex;							//��Ϸ����
	BYTE							bGameTypeIdex;							//��Ϸ����
	DWORD							bGameRuleIdex;							//��Ϸ����
	BYTE							bGamePayType;							//��Ϸ��������
	BYTE							bGameBaseScore;							//�׷֣���÷��
	BYTE							bGameGuDingMa;							//�̶���
	BYTE							bQiangGangHu;							//���ܺ���0û�����ܺ���1�����ܺ�����÷��
	BYTE							bGameCardType;							//���е�
	BYTE							bAllOther;	
	BYTE							cbPantherType;							//�� ��  �� ɫ ��
	BYTE                            cbPlayerNum;                            //��Ϸ����

	BYTE							bGameTypeIndex;						    //
	BYTE							bGameTypeQing;						    //

	BYTE							bGameMaxScore;							//�ⶥ
	BYTE							bGameMaxScoreGold;						//��
	BYTE							bBaoIsOutCard;							//0�б����ɴ�1�б��ɴ򣨻�÷��
	BYTE							bPiaoShu;								//Ʈ����0��Ư��1Ư1��2Ư2 ...10Ư10������÷��
	BYTE							bFendDing;								//ֻ�Ϸⶥ
	BYTE							bJingDing;								//��
	BYTE                            bFloorIndex;                            //¥��
	DWORD							dwClubID;								//���ֲ�ID
	BYTE							bLimitIP;	//
};

//����˽�˳�
struct DBR_GR_Create_Private_Cost
{
	DWORD							dwUserID;
	DWORD							dwCost;
	DWORD							dwCostType;
};


//˽�˳���Ϣ
struct DBO_GR_Private_Info
{
	WORD							wKindID;
	SCORE							lCostGold;
	BYTE							bPlayCout[4];							//��Ҿ���
	SCORE							lPlayCost[4];							//���ĵ���
};

//��������
struct DBO_GR_CreatePrivateResoult
{
	BOOL							bSucess;
	BYTE							cbRoomType;
	TCHAR							szDescribeString[128];					//������Ϣ
	SCORE							lCurSocre;								//��ǰʣ��
	DWORD							dwAgaginTable;							//���¼�������
	BYTE							bPlayCoutIdex;							//��Ҿ���
	BYTE							bGameTypeIdex;							//��Ϸ����
	DWORD							bGameRuleIdex;							//��Ϸ����
	BYTE							bGamePayType;							//��Ϸ��������
	BYTE							bGameBaseScore;							//�׷֣���÷��
	BYTE							bGameGuDingMa;							//�̶���
	BYTE							bQiangGangHu;							//���ܺ���0û�����ܺ���1�����ܺ�
	BYTE							bGameCardType;							//����
	BYTE							bAllOther;
	BYTE							cbPantherType;							//1�籪��2������3������
	BYTE                            cbPlayerNum;                            //��Ϸ����
	//fushun
	BYTE							bGameTypeIndex;							//
	BYTE							bGameTypeQing;							//

	BYTE							bGameMaxScore;							//�ⶥ
	BYTE							bGameMaxScoreGold;						//��
	BYTE							bBaoIsOutCard;							//0�б����ɴ�1�б��ɴ򣨻�÷��
	BYTE							bPiaoShu;								//Ʈ����0��Ư��1Ư1��2Ư2 ...10Ư10������÷��
	BYTE							bFendDing;								//ֻ�Ϸⶥ
	BYTE							bJingDing;								//��
	BYTE                            bFloorIndex;                            //¥��
	DWORD							dwClubID;								//�������ֲ�
	DWORD							dwMasterID;								//Ⱥ��ID
	DWORD                           dwManagerID;                            //����ԱID
	
	BYTE							bLimitIP;

};

//��Ҽ�����ֲ�    
struct  DBR_GR_Join_Club_Rom
{
	DWORD             dwUserID;            //���ID
	DWORD             dwClubID;			   //���ֲ�ID
	DWORD             dwRoomNum;           //�����
	DWORD             dwSeatNum;           //�����
	DWORD			  dwPlayCost;		   // ��Ҫ֧������ʯ
	DWORD             dwUpdateStatus;      //���¾��ֲ�������Ϣ
};
struct  FloorStressGameInfo
{
	BYTE                            bGameTypeIndex;                         //2�з磬1û��
	BYTE                            bGameTypeQing;                          //2������ ��1 û����
	BYTE							bPlayCoutIdex;							//��Ϸ����
	BYTE							bGamePayType;							//֧����ʽ
	BYTE							bGameBaseScore;							//�׷�
	BYTE							bQiangGangHu;							//���ܺ���0û�����ܺ���1�����ܺ�
	BYTE                            bAllOther;                              //ȫ���� 0 ��ȫ���� 1 ��ȫ����
	//����
	BYTE                            bGameMaxScore;							//�ⶥ
	BYTE                            bBaoIsOutCard;					        //0 �б����ɴ� 1 �б��ɴ򣨻�÷��
	BYTE                            bPiaoShu;                               //Ʈ��0 ��Ҫ  1 Ʈ1  2 Ʈ2 ... 10 Ʈ10
	BYTE                            bPlayerNum;								//��Ϸ����
	BYTE                            bPatherType;                            //����
};
struct   RoomInfo 
{
	BYTE                            bFloorIndex;                           //  ¥��
	DWORD							dwCreateUser;							// ������
	DWORD							dwPlayCost;								// ��Ҫ֧������ʯ
	FloorStressGameInfo             lGameInfo;                              //
};


//��Ҽ�����ֲ�  
struct  DBR_GR_Join_Club_RomResult
{
	DWORD                           dwUserID;								// ���ID
	DWORD							dwClubID;								// ���ֲ�ID
	DWORD                           dwMasterID;                             // Ⱥ��ID
	DWORD                           dwManageId;                             // ����ԱID
	DWORD                           dwCostScore;                            // ��ʯ����
	DWORD							dwRepairTimeBegin;						// ��ʼά��ʱ��
	DWORD							dwRepairTimeEnd;						// ����ά��ʱ��
	TCHAR							szDescribeString[128];					// ������Ϣ
	BYTE							bSucess;								// ��ȡ�Ƿ�ɹ�
	DWORD                           dwUpdateState;                          // ����״̬(���¾��ֲ����淨)
	std::vector<RoomInfo>           vecRoomInfo;
};
//˽�˳���ʽ������ֲ�����
struct   DBR_GR_Club_RomPrivateRes
{
	DWORD                           bUserID;								//���ID
	DWORD							dwClubID;								//���ֲ�ID
	DWORD                           dwTableID;                              //����ID
	DWORD                           dwSeatNum;                              //��λ��  
	BYTE							bSucess;								//��ȡ�Ƿ�ɹ�
};

//DBR_GR_CLUB_JOIN_CLUB_ROM_REQ		   813			 ����Ƿ���Խ�����ֲ�����
struct  DBR_GR_Into_Club_Table_Req
{
	DWORD							dwRoomNum;						  //����ID
	DWORD                           dwUserID;                         //�û�ID
};
// DBR_GR_CLUB_JOIN_CLUB_ROM_RES  712                 ����Ƿ��ܽ��뷿��Ļ�Ӧ
struct  DBR_GR_Into_Club_Table_Res
{
	BYTE							bSucess;								//��ȡ�Ƿ�ɹ�
	DWORD							dwRoomNum;								//����ID
};

// DBR_GR_CREATE_FLOOR_STRESS_REQ       814			 �������ֲ�¥��
struct DB_GR_Create_Floor_Stress            
{
	DWORD							dwUserID;						    //�û�ID ������ID
	DWORD							dwClubID;						    //���ֲ�ID
	FloorStressGameInfo             lGameInfo;
};

// 
struct  DB_GR_Create_Floor_Stress_Res
{
	BYTE                            bSucess;
	char                            strErrorDecrible[128];                  //��������
	BYTE							bFloorIndex;                            //¥��ID
	FloorStressGameInfo             lGameInfo;
	DWORD                           dwClubID;                               //���ֲ���
	DWORD                           dwUserID;                               //���ID
};
// DBR_GR_CLUB_UPDATE_FLOOR_REQ			815          ����Ƿ��ܽ��뷿��Ļ�Ӧ
struct  DBR_GR_Update_Club_Floor_Req
{
	BYTE							bFloorIndex;                            //¥��ID
	FloorStressGameInfo             lGameInfo;
	DWORD                           dwClubID;                               //���¾��ֲ���¥��
	DWORD                           dwUserID;                               //�û�ID
};
//  DBR_GR_CLUB_UPDATE_FLOOR_RES      714                ���¾��ֲ�¥���Ӧ
struct  DBR_GR_Update_Club_Floor_Res
{
	BYTE                            bSucess;
	BYTE							bFloorIndex;                            //¥��ID
	FloorStressGameInfo				lGameInfo;
	DWORD                           dwClubID;                               //���¾��ֲ���¥��
};

#endif
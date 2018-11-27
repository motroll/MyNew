#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "..\..\ȫ�ֶ���\datastream.h"


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_GP_LOGON_VISITOR		5									//�ο͵�¼

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_MATCH_SIGNUPINFO		106									//������Ϣ

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ο͵�¼
struct CMD_GP_VisitorLogon
{
	WORD								wFaceID;						//ͷ���ʶ
	BYTE								cbGender;						//�û��Ա�
	DWORD								dwPlazaVersion;					//�㳡�汾
	BYTE								cbValidateFlags;			    //У���ʶ
	TCHAR								szNickName[LEN_NICKNAME];		//�û��ǳ�
	TCHAR								szSpreader[LEN_NICKNAME];		//�ƹ�����
	TCHAR								szPassWord[LEN_PASSWORD];		//��¼����
	TCHAR								szPassWordBank[LEN_PASSWORD];	//��¼����
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	BYTE							cbInsureEnabled;					//���п�ͨ
	
	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
	CHAR							szPictureName[128];                 //�ͼƬ
	DWORD                           dwPresent[6];                       //

};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//��������
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
	TCHAR							szDownLoadHttp[128];				//���ص�ַ
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_LIST_MATCH			105									//�����б�
#define SUB_GP_VIDEO_OPTION			106									//��Ƶ����

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��
#define SUB_GP_MODIFY_ACCOUNTS		104									//�޸��ʺ�
#define SUB_GP_MODIFY_SPREADER		105									//�޸��Ƽ���

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		120									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		121									//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO		122									//�Զ�ͷ��

//��������
#define SUB_GP_MATCH_SIGNUP			200									//��������
#define SUB_GP_MATCH_UNSIGNUP		201									//ȡ������
#define SUB_GP_MATCH_SIGNUP_RESULT	202									//�������
#define SUB_GP_MATCH_AWARD_LIST		203									//��������

//ǩ������
#define SUB_GP_CHECKIN_QUERY		220									//��ѯǩ��
#define SUB_GP_CHECKIN_INFO			221									//ǩ����Ϣ
#define SUB_GP_CHECKIN_DONE			222									//ִ��ǩ��
#define SUB_GP_CHECKIN_RESULT		223									//ǩ�����
#define SUB_GP_CHECKIN_AWARD		224									//ǩ������

//��������
#define SUB_GP_BEGINNER_QUERY		240									//��������ǩ��
#define SUB_GP_BEGINNER_INFO		241									//����������Ϣ
#define SUB_GP_BEGINNER_DONE		242									//��������ִ��
#define SUB_GP_BEGINNER_RESULT		243									//�����������

//���̷���
#define SUB_GP_ROULETTE_USERINFO	250									//������Ϣ
#define SUB_GP_ROULETTE_DONE		251									//ִ��


//�ͱ�����
#define SUB_GP_BASEENSURE_LOAD			260								//���صͱ�
#define SUB_GP_BASEENSURE_TAKE			261								//��ȡ�ͱ�
#define SUB_GP_BASEENSURE_PARAMETER		262								//�ͱ�����
#define SUB_GP_BASEENSURE_RESULT		263								//�ͱ����

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����
#define SUB_GP_QUERY_ACCOUNTINFO	304									//������Ϣ
#define SUB_GP_QUERY_INGAME_SEVERID	305									//��Ϸ״̬
#define SUB_GP_MODIFY_STARVALUE		306									//����
#define SUB_GP_SHARE_AWARD			307									//��������Ȧ����
#define SUB_GP_AWARD_RETURN			308									//����

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE	402									//ת�˲���
#define SUB_GP_USER_INSURE_INFO		403									//��������
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//�û���Ϣ
#define SUB_GP_QUERY_USER_INSURE_HESTORY 409							//��ֵ��¼
#define SUB_GP_QUERY_USER_INSURE_HESTORY_SUCCESS 410					//��ֵ��¼�ظ�

//�Զ����ֶβ�ѯ ����
#define SUB_GP_QUERY_PUBLIC_NOTICE		500									//�Զ����ֶβ�ѯ
#define	SUB_GP_PUBLIC_NOTICE			501

//�����Ƽ��˽��
#define SUB_GP_SPREADER_RESOULT			520									//�����Ƽ��˽��

//׬�����а�
#define SUB_GP_ADDRANK_GET_AWARD_INFO	540									//��ý�����Ϣ
#define SUB_GP_ADDRANK_BACK_AWARD_INFO	541									//���ؽ�����Ϣ
#define SUB_GP_ADDRANK_GET_RANK			542									//�������
#define SUB_GP_ADDRANK_BACK_RANK		543									//��������

//��Ϸ��¼
#define SUB_GP_GAME_RECORD_LIST			550			
#define SUB_GP_GAME_RECORD_TOTAL		551			
#define SUB_GP_GAME_RECORD_CHILD		552									


//�������
#define SUB_GP_OPERATE_SUCCESS			900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE			901									//����ʧ��

//���Ѷһ�
#define  SUB_GP_EXCHANGEHUAFEI_GET_LIST_INFO    1000                    //��ȡ�һ������б�
#define  SUB_GP_EXCHANGEHUAFEI_BACK				1001					//�һ������б���
#define  SUB_GP_EXCHANGE_DONE					1002					//�һ�

//�̳�����
#define  SUB_GP_SHOPINFO_GET_LIST_INFO		1100                    //��ȡ�̳��б�
#define  SUB_GP_SHOPINFO_BACK				1101					//�̳��б���
#define  SUB_GP_SHOPINFO_BUYODER			1102					//����
#define  SUB_GP_SELLITEM					1103					//
#define  SUB_GP_SELLLIST_BACK				1104					//

//���ֲ���Ϣ
#define SUB_GR_CREATE_CLUB			    1200		// �������ֲ�
#define SUB_GR_JOIN_CLUB				1201		// ���������ֲ�
#define SUB_GR_JOIN_CLUB_COMFIRM		1203		// �᳤ͬ���ͬ�������ֲ�
#define SUB_GR_LEAVE_CLUB				1204		// �뿪���ֲ�
#define SUB_GR_KICK_CLUB				1205		// �߳����ֲ�
#define SUB_GR_INVITE_CLUB				1206		// ������Ҽ�����ֲ�
#define SUB_GR_CREATE_JOIN_RES          1209        // �����������������ֲ���Ӧ
#define SUB_GR_GET_MY_CLUB_REQ          1211        // ��ȡ�Լ��ľ��ֲ�����
#define SUB_GR_GET_MY_CLUB_RES          1212        // ��ȡ�Լ��ľ��ֲ���Ӧ
#define SUB_GR_CLUB_MEMBER_LIST_REQ     1213        // �᳤���ֲ���Ա�б�����
#define SUB_GR_CLUB_MEMBER_LIST_RES     1214        // �᳤���ֲ���Ա�б��Ӧ
#define SUB_GR_UPDATE_CLUB_REQ			1215        // �᳤���¾��ֲ�����
#define SUB_GR_DISMISS_CLUB_REQ         1216		// ��ɢ���ֲ�����
#define SUB_GR_CLUB_CHANGE_MASTER_REQ   1217		// ת�û᳤����
#define SUB_GR_CLUB_SET_MANAGER_REQ		1218		// �᳤���ù���Ա����
#define SUB_GR_CREATE_FLOOR_STRESS_REQ  1219        // �¿�¥��




// �������ֲ�  SUB_GR_CREATE_CLUB
struct SUB_GR_Create_Club
{	
	DWORD dwUserID;					// �û�ID
	TCHAR stClubName[LEN_NICKNAME]; // ���ֲ�����
	BYTE bGameTypeIndex;			// ��Ϸ���� 2���3���ݻλΣ�4Ϫˮ
	BYTE bGameTypeQing;				// 2������ ��1 û����
	BYTE bPlayCoutIdex;				// ��Ϸ����
	BYTE bGamePayType;				// ��Ϸ��������
	BYTE bQiangGangHu;				// ���ݵ����ܺ� 0û�У�1��
	BYTE bAllOther;					// Ϫˮ��ȫ���� 0û�У�1��
	BYTE bMaxScore;				    // �ⶥ����
	BYTE bPantherType;              // �籪 ���� ����  ɫ�� 
	BYTE bBaoIsOutCard;             // 0 �б����ɴ� 1 �б��ɴ� 2 �ޱ�
	BYTE bPiaoShu;				    // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10	
	BYTE bPlayerNum;                // ��Ϸ����
	BYTE bGameBaseScore;            // ��Ϸ�׷�
};

// ���������ֲ� SUB_GR_JOIN_CLUB
struct SUB_GR_Join_Club
{
	DWORD dwUserID; // �û�ID
	DWORD dwClubID; // Ҫ����ľ��ֲ�ID
};

// �᳤ͬ���ͬ�������ֲ� SUB_GR_JOIN_CLUB_COMFIRM
struct SUB_GR_Join_Club_Comfirm 
{	
	DWORD dwUserID; // ����˵��û�ID
	DWORD dwClubID; // ������ֲ���ID
	BYTE bComfirm;  // 1ͬ�⣬2�ܾ�	
};

// �뿪���ֲ� SUB_GR_LEAVE_CLUB
struct SUB_GR_Leave_Club
{	
	DWORD dwUserID; // �����ߵ��û�ID
	DWORD dwClubID; // �뿪���ֲ���ID
};

// �߳����ֲ� SUB_GR_KICK_CLUB
struct SUB_GR_Kick_Club
{	
	DWORD dwRequestID; // ������ID
	DWORD dwUserID; // �߳��û���ID
	DWORD dwClubID; // ���ֲ�ID	
};

// ������Ҽ�����ֲ� SUB_GR_INVITE_CLUB
struct SUB_GR_Invite_Club
{	
	DWORD dwRequestID; // ������ID
	DWORD dwUserID; // ��������û�ID
	DWORD dwClubID; // ���ֲ�ID
};

//��ȡ�Լ��ľ��ֲ�����  SUB_GR_GET_MY_CLUB_REQ
struct   SUB_GR_Get_My_Club_Req
{
	DWORD                       dwUserID;                     // �Լ����û�ID
};

//��ȡ�Լ��ľ��ֲ���Ӧ  SUB_GR_GET_MY_CLUB_RES
struct   SUB_GR_Get_My_Club_Res
{
	DWORD				cbAgent;					   // �Ƿ���� ����0����1
	DWORD               dwUserId;                      // �û�ID
	DWORD               dwClubNum;                     // �Լ��ľ��ֲ����� 
	DWORD               dwClubId[5];                   // ���ֲ�ID
	TCHAR               szClubName[5][LEN_NICKNAME];   // ���ֲ�����
	DWORD               dwMemberNum[5];                // ��Ա����
	DWORD               dwMasterId[5];                 // �᳤ID
	DWORD				dwClubManagerId[5][3];         // ����ԱID
	TCHAR               szMaterName[5][LEN_NICKNAME];  // �᳤����
	TCHAR               szPatritUrl[5][256];           // �᳤ͷ��
	TCHAR				stClubNotice[5][124];		   // ���ֲ�����
	BYTE				cbGameTypeIndex[5];            // ��Ϸ����
	BYTE				bGameTypeQing[5];			   // 2������ ��1 û����
	BYTE				bPlayCoutIdex[5];			   // ��Ϸ����
	BYTE				bGamePayType[5];			   // ��Ϸ��������
	BYTE				bQiangGangHu[5];			   // ���ݵ����ܺ� 0û�У�1��
	BYTE				bAllOther[5];				   // Ϫˮ��ȫ���� 0û�У�1��
	BYTE				cbPantherType[5];			   // �� �� �� ɫ ��
	BYTE				cbPlayerNum[5];			       // ��Ϸ����
	BYTE                bBaoIsOutCard[5];              //0 �б����ɴ� 1 �б��ɴ� 2 �ޱ�
	BYTE                bPiaoShu[5];				    //Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10
	BYTE                cbGameBaseScore[5];            // ��÷�׷�
	BYTE				cbGameMaxScore[5];             // �ⶥ����
	SYSTEMTIME			dCreateTime[5];				   // �������ֲ���ʱ��
	DWORD				dwOnlineNum[5];				   // ��Ա��������

};

// SUB_GR_CREATE_JOIN_RES             1209         �������߼�����ֲ��Ļ�Ӧ
struct  SUB_GR_Create_Join_Res 
{
	DWORD dwCreateJoin; // 1 ������2 �������
	DWORD dwError; // �����ʶ 0 �ɹ���������ʧ��
	TCHAR szDescribeString[128]; //������Ϣ
}; 

// SUB_GR_CLUB_MEMBER_LIST_REQ     1213        �᳤���ֲ���Ա�б�����
struct  SUB_GR_Club_Member_List_Req 
{
	DWORD dwUserID; // �᳤ID
	DWORD dwClubID; // ���ֲ�ID
	DWORD dwFlag;   // 0���������б�1�����Ա�б�
};


// SUB_GR_CLUB_MEMBER_LIST_RES     1214        �᳤���ֲ���Ա�б��Ӧ
struct SUB_GR_Club_Member_List_Res
{
	DWORD bFlag;                                 // ������ǣ�0û���� 1����
	std::vector<int>                iUserID;     // �û�ID; 
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

// SUB_GR_UPDATE_CLUB_REQ			1215         �᳤���¾��ֲ�����
struct SUB_GR_Update_Club_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	BYTE bGameTypeIndex;			// ��Ϸ���� 2���3���ݻλΣ�4Ϫˮ
	BYTE bGameTypeQing;				// 2������ ��1 û����
	BYTE bPlayCoutIdex;				// ��Ϸ����
	BYTE bGamePayType;				// ��Ϸ��������
	BYTE bMaxScore;				    // �ⶥ����
	BYTE bQiangGangHu;				// ���ݵ����ܺ� 0û�У�1��
	BYTE bAllOther;					// Ϫˮ��ȫ���� 0û�У�1��
	BYTE bPantherType;				// �� �� �� ɫ ��
	BYTE bPlayerNum;                // ��Ϸ����
	BYTE bBaoIsOutCard;             // 0 �б����ɴ� 1 �б��ɴ� 2 �ޱ�
	BYTE bPiaoShu;				    // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10	
	BYTE bGameBaseScore;            // �׷�
	TCHAR stClubName[LEN_NICKNAME]; // ���ֲ�����
	TCHAR stClubNotice[124];        // ����
};

// SUB_GR_DISMISS_CLUB_REQ          1216		 ��ɢ���ֲ�����
struct SUB_GR_Dismiss_Club_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
};

// SUB_GR_CLUB_CHANGE_MASTER_REQ   1217		 ת�û᳤����
struct SUB_GR_Club_Change_Master_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	DWORD dwMasterID;				// �¸��᳤ID(ת�ú��)
};

// SUB_GR_CLUB_SET_MANAGER_REQ	  1218		 �᳤���ù���Ա����
struct SUB_GR_Club_Set_Manager
{	
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	DWORD dwManagerID;				// ����ԱID
};
// �½����ֲ�¥��  SUB_GR_CREATE_FLOOR_STRESS_REQ
struct SUB_GR_Create_Floor_Stress
{	
	DWORD dwUserID;					// �û�ID ������ID
	DWORD dwClubID;                 // ���ֲ�ID
	BYTE  bGameTypeIndex;			// ��Ϸ���� 2���3���ݻλΣ�4Ϫˮ
	BYTE  bGameTypeQing;			// 2������ ��1 û����
	BYTE  bPlayCoutIdex;			// ��Ϸ����
	BYTE  bGamePayType;				// ��Ϸ��������
	BYTE  bQiangGangHu;				// ���ݵ����ܺ� 0û�У�1��
	BYTE  bAllOther;				// Ϫˮ��ȫ���� 0û�У�1��
	BYTE  bMaxScore;				// �ⶥ����
	BYTE  bPantherType;             // �籪 ���� ����  ɫ�� 
	BYTE  bBaoIsOutCard;            // 0 �б����ɴ� 1 �б��ɴ� 2 �ޱ�
	BYTE  bPiaoShu;				    // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10	
	BYTE  bPlayerNum;               // ��Ϸ����
	BYTE  bGameBaseScore;           // ��Ϸ�׷�
};


//��Ϸ��¼
struct CMD_GP_GetGameRecord_List
{
	DWORD							dwUserID;							
};

//��Ϸ��¼
struct CMD_GP_GetGameTotalRecord
{
	DWORD							dwUserID;			
	DWORD							dwRecordID;						
};

//��Ϸ��¼
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
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
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

//���Ѷһ�

struct CMD_GP_GetExchangeHuaFei
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};


struct CMD_GP_ExchangeDone
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	DWORD							dwExchangeID;						//�һ� I D
};

struct CMD_GP_BackExchangeHuaFei
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

// �̳��б�
struct CMD_GP_GetShopInfo
{
	DWORD						dwUserID;						//�û� I D
	char							szPassword[LEN_PASSWORD];			//�û�����
};

struct CMD_GP_UpShopOder
{
	DWORD						dwUserID;						//�û� I D
	DWORD						dwItemID;						//��Ʒid
	DWORD						total_fee;						//���
	std::string					appid;							//΢��id
	std::string					mch_id;							//�̻���
	std::string					prepay_id;						//׼��������
	std::string					strOSType;						//ϵͳ��

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

//����
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

//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ������
struct CMD_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//�������
struct CMD_GP_MatchGetAward
{
	DWORD							dwUserID;	
	DWORD							dwMatchID;		//������ʶ
	DWORD							dwMatchNO;		//��������	
};

//������Ϣ
struct tagMatchAwardkInfo
{
	WORD							MatchRank;							//��������
	DWORD							RewardGold;					//�������
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
struct CMD_GR_MatchAwardList
{
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������
	std::vector<tagMatchAwardkInfo> kAwards;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(dwMatchID);
		Stream_VALUE(dwMatchNO);
		StructVecotrMember(tagMatchAwardkInfo,kAwards);
	}
};
//////////////////////////////////////////////////////////////////////////////////


//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸��Ƽ���
struct CMD_GP_ModifySpreader
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szSpreader[LEN_NICKNAME];		//�û�����
};

//�޸��ʺ�
struct CMD_GP_ModifyAccounts
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//�û��ʺ�
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwUserStarValue;					//����
	DWORD							dwUserStarCout;						//����
};

//������Ϣ
struct CMD_GP_UserAccountInfo
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

//���ͷ���
struct CMD_GP_AWARD_RETURN
{
	DWORD							dwUserID;
	DWORD							dwInsure;
};

//�����ɹ�
struct CMD_GP_InGameSeverID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;			
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��Ϸ״̬
struct CMD_GP_UserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};


//��ѯ��Ϣ
struct CMD_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//�û� I D
};


//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};
//�޸�����
struct  CMD_GP_ModifyStarValue
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwStarValue;						//���ӵ�ַ
};

//��������Ȧ����
struct CMD_GP_ShareAward
{
	DWORD							dwUserID;

};

//////////////////////////////////////////////////////////////////////////////////
//��ѯǩ��
struct CMD_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ǩ����Ϣ
struct CMD_GP_CheckInInfo
{	
	WORD							wSeriesDate;						//��������
	WORD							wAwardDate;							//��Ʒ����
	bool							bTodayChecked;						//ǩ����ʶ
	SCORE							lRewardGold[LEN_SIGIN];				//�������
	BYTE							lRewardType[LEN_SIGIN];				//�������� 1��� 2����
	BYTE							lRewardDay[LEN_SIGIN];				//�������� 
};

//ִ��ǩ��
struct CMD_GP_CheckInDone
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ǩ�����
struct CMD_GP_CheckInResult
{
	bool							bType;								//�Ƿ��Ǵﵽ������ȡ��Ʒ
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lScore;								//��ǰ���
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//����
//ǩ�����
struct CMD_GP_ReqRouletteUeserInfo
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwVaildDay;							//��Чʱ��
};
struct CMD_GP_BackRouletteUeserInfo
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwHaveDone;
	BYTE							cbCout;
	SCORE							lRewardGold[LEN_ROULETTE];			//�������
	BYTE							cbRewardType[LEN_ROULETTE];			//�������� 1��� 2����
	BYTE							cbRouletteIdex[LEN_ROULETTE];		//��������
	BYTE							cbAwardPercent[LEN_ROULETTE];		//��������
};
struct CMD_GP_ReqRouletteDone
{
	DWORD							dwUserID;							//�û���ʶ
};
struct CMD_GP_BackRouletteDone
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lRewardGold;						//�������
	BYTE							cbRewardType;						//�������� 1��� 2����
	BYTE							cbRewardIdex;						//�������� 1��� 2����

};
//////////////////////////////////////////////////////////////////////////////////
//���ֻ

struct CMD_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};
struct CMD_GP_BeginnerInfo
{	
	WORD							wSeriesDate;						//��������
	bool							bTodayChecked;						//ǩ����ʶ
	bool							bLastCheckIned;						//ǩ����ʶ
	SCORE							lRewardGold[LEN_BEGINNER];			//�������
	BYTE							lRewardType[LEN_BEGINNER];			//�������� 1��� 2����
};
struct CMD_GP_BeginnerDone
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

struct CMD_GP_BeginnerResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lAwardCout;							//��������
	SCORE							lAwardType;							//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�ͱ�����

//��ȡ�ͱ�
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ͱ�����
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};


//�Զ����ֶβ�ѯ ����
struct CMD_GP_QueryNotice
{
	TCHAR							szKeyName[LEN_NICKNAME];			//�ؼ���
};
struct CMD_GP_PublicNotice
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[512];				//�ɹ���Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ
#define DTP_GP_UI_HEAD_HTTP			10									//ͷ��
#define DTP_GP_UI_IP				11									//IP
#define DTP_GP_UI_CHANNEL			12									//������
#define DTP_GP_UI_GPS				13									//GPS

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

// ��ֵ��¼
struct RechargeInsure
{
	DWORD							m_rechargeid;	// ��ֵ��¼id
	DWORD							m_insure_type;	// ������Ʒ���� 0:6 1:18...
	DWORD							m_add_insure;	// ��ֵ���		100��
	DWORD							m_get_insure;	// ʵ�ʻ�ý��	120��
	SYSTEMTIME						m_insure_time;	// ��ֵʱ��		 
	TCHAR							szRechargeKey[128];	//��ֵ��ˮ�� 
};

struct CMD_GP_UserInsureHistory
{
	CMD_GP_UserInsureHistory()
	{
		memset(this, 0, sizeof(*this));
	}
	RechargeInsure					infos[100];
};



//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

// ��ѯ��ֵ��ϢSUB_GP_QUERY_USER_INSURE_HESTORY
struct CMD_GP_QueryInsureHestory
{
	DWORD							dwUserID;							//�û� I D
};


//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//�����ɹ�
struct CMD_GP_SpreaderResoult
{
	LONG							lResultCode;						//��������
	SCORE							lScore;
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};


//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����

//////////////////////////////////////////////////////////////////////////////////


//������di
struct    CMD_MB_Server_Address_Req
{
	DWORD          dwUserID;                                           //�û�ID
	DWORD          dwClubID;										   //���ֲ�ID
};
#pragma pack()

#endif
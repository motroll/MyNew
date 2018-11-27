#ifndef DATA_BASE_PACKETIN_HEAD_FILE
#define DATA_BASE_PACKETIN_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����
#define DBR_GP_MODIFY_ACCOUNTS		15									//�޸��ʺ�
#define DBR_GP_MODIFY_SPREADER		16									//�޸��Ƽ���
#define DBR_GP_MODIFY_STARVALUE		17									//����
#define DBR_GP_SHARE_AWARD			18									//��������Ȧ����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת����Ϸ��
#define DBR_GR_CHECK_PAY_INFO	    33									// ֧��

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�
#define DBR_GP_QUERY_ACCOUNTINFO	44									//��ѯ����
#define DBR_GP_USER_INGAME_SERVERID	45									//��Ϸ״̬
#define DBR_GP_USER_INSURE_HISTORY	46									//��ѯ��ֵ��¼


//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ

//��������
#define DBR_GP_MATCH_SIGNUP			55									//��������
#define DBR_GP_MATCH_UNSIGNUP		56									//ȡ������
#define DBR_GP_MATCH_AWARD			57									//��������

//ǩ������
#define DBR_GP_LOAD_CHECKIN_REWARD	60									//ǩ������
#define DBR_GP_CHECKIN_DONE			61									//ִ��ǩ��
#define DBR_GP_CHECKIN_QUERY_INFO	62									//��ѯ��Ϣ
#define DBR_GP_CHECKIN_AWARD		64									//��ѯ��Ϣ


#define DBR_GP_LOAD_BASEENSURE		70									//���صͱ�

#define DBR_GP_LOAD_BEGINNER		80									//������������
#define DBR_GP_BEGINNER_QUERY_INFO	81									//��ѯ��������
#define DBR_GP_BEGINNER_DONE		82									//��ȡ��������

	//��������
#define DBR_GP_LOAD_ROULETE_AWARD	90									//���̽���
#define DBR_GP_ROULETE_DONE			91									//��ȡ���̽���
#define DBR_GP_ROULETE_USERINFO		92									//����״̬��Ϣ

//��Ϸ����
#define DBR_GP_GAME_CONFIG			95									//��Ϸ����

//�ͱ�����
#define DBR_GP_BASEENSURE_TAKE		100									//��ȡ�ͱ�

//׬������
#define DBR_GP_LOAD_ADDRANK_CONFIG	110									//����
#define DBO_GP_ADDRANK_UP_AWARD		111									//�Ƿ���ȡ���н���		
#define DBR_GP_GET_ADDRANK			112									//�������

//��Ϸ��¼
#define DBR_GP_GAME_RECORD_LIST		120									//��Ϸ��¼
#define DBR_GP_GAME_TOTAL_RECORD	121									//��Ϸ�ط�
#define DBR_GP_GAME_TOTAL_CHILD		122									//��Ϸ�ط�

//�һ�����	
#define DBR_GP_GET_EXCHANGE_HUAFEI	130							//��öһ������б�
#define DBR_GP_EXCHANGE_DONE		131							//�һ�

//�̳�	
#define DBR_GP_GET_SHOPINFO			140									//����̳��б�
#define DBR_GP_GET_BUYODER			141	

//��ҳ���	
#define DBR_GP_SELL_ITEM			150									//����̳��б�
#define DBR_GP_GET_SELL_RECORD		151										//����

//���ֲ���Ϣ phw
#define DBR_GR_CREATE_CLUB			    160		// �������ֲ�
#define DBR_GR_JOIN_CLUB				161		// ���������ֲ�
#define DBR_GR_JOIN_CLUB_COMFIRM		163		// �᳤ͬ���ͬ�������ֲ�
#define DBR_GR_LEAVE_CLUB				164		// �뿪���ֲ�
#define DBR_GR_KICK_CLUB				165		// �߳����ֲ�
#define DBR_GR_INVITE_CLUB				166		// ������Ҽ�����ֲ�
#define DBR_GR_CLUB_INFO_MEMBER			167		// �Լ����ֲ�����Ϣ
#define DBR_GR_GET_MY_CLUB_REQ          168     // ��ȡ�Լ��ľ��ֲ�����
#define DBR_GR_CLUB_MEMBER_LIST_REQ     169     // ���ֲ���Ա�б�����
#define DBR_GR_UPDATE_CLUB_REQ		    170     // �᳤���¾��ֲ�����
#define DBR_GR_DISMISS_CLUB_REQ			171		// ��ɢ���ֲ�����
#define DBR_GR_CLUB_CHANGE_MASTER_REQ	172		// ��ɢ���ֲ�����
#define DBR_GR_CLUB_SET_MANAGER_REQ		173		// �᳤���ù���Ա����

//�Զ����ֶβ�ѯ
#define DBR_GP_PUBLIC_NOTIC			200									//�Զ����ֶβ�ѯ

//////////////////////////////////////////////////////////////////////////////////

// �������ֲ�
struct DBR_GR_Create_Club
{	
	DWORD dwUserID;					// �û�ID
	TCHAR stClubName[LEN_NICKNAME]; // ���ֲ�����
	BYTE bGameTypeIndex;			// ��Ϸ���� 2���3���ݻλΣ�4Ϫˮ
	BYTE bGameTypeQing;				// 2������ ��1 û����
	BYTE bPlayCoutIdex;				// ��Ϸ����
	BYTE bGamePayType;				// ��Ϸ��������
	BYTE bQiangGangHu;				// ���ݵ����ܺ� 0û�У�1��
	BYTE bAllOther;					// Ϫˮ��ȫ���� 0û�У�1��
	BYTE bMaxScore;					// �ⶥ����
	BYTE bPantherType;				// ���� �籪 ����  ɫ�� 
	BYTE bBaoIsOutCard;             // 0 �б����ɴ� 1 �б��ɴ� 2 �ޱ�
	BYTE bPiaoShu;				    // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10	
	BYTE bPlayerNum;                // ��Ϸ����
	BYTE bGameBaseScore;            // ��Ϸ�׷�
};

// ���������ֲ�
struct DBR_GR_Join_Club
{
	DWORD dwUserID; // �û�ID
	DWORD dwClubID; // ���ֲ�ID
};

// �᳤ͬ���ͬ�������ֲ�
struct DBR_GR_Jion_Club_Comfirm 
{	
	DWORD dwOperateID; //������ID
	DWORD dwUserID; // ����˵��û�ID
	DWORD dwClubID; // ������ֲ���ID
	BYTE  bComfirm; // 1ͬ�⣬2�ܾ�							
};

// �뿪���ֲ�
struct DBR_GR_Leave_Club
{	
	DWORD dwUserID; // �����ߵ��û�ID
	DWORD dwClubID; // �뿪���ֲ���ID						
};

// �߳����ֲ�
struct DBR_GR_Kick_Club
{	
	DWORD dwRequestID; // ������ID
	DWORD dwUserID; // �߳��û���ID
	DWORD dwClubID; // ���ֲ�ID							
};

// ������Ҽ�����ֲ�
struct DBR_GR_Invite_Club
{	
	DWORD dwRequestID; // ������ID
	DWORD dwUserID; // ��������û�ID
	DWORD dwClubID; // ���ֲ�ID					
};

// �Լ����ֲ�����Ϣ
struct DBR_GR_Club_Info_Member
{	
	DWORD dwClubID; // ���ֲ�ID
};
// ��ȡ�Լ��ľ��ֲ���Ϣ����
struct  DBR_GR_Get_My_Club_Req
{
	DWORD                           dwUserID;                          // ���ֲ�ID
};
// ��ȡ�Լ��ľ��ֲ���Ϣ��Ӧ
struct  DBO_GR_Get_My_Club_Res
{
	DWORD				cbAgent;					   // �Ƿ���� ����0����1
	DWORD               dwUserID;                      // �û�ID
	DWORD               dwClubNum;                     // �Լ��ľ��ֲ����� 
	DWORD               dwClubId[5];                   // ���ֲ�ID
	TCHAR               szClubName[5][LEN_NICKNAME];   // ���ֲ�����
	DWORD               dwMemberNum[5];                // ��Ա����
	DWORD               dwMasterId[5];                 // �᳤ID
	DWORD				dwClubManagerId[5][3];         // �Լ��Ƿ��Ǿ��ֲ�����ĳ�Ա
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
	BYTE				cbBaoIsOutCard[5];			   // �� 0:���ɴ�  1:�ɴ� 2:�ޱ�
	BYTE				cbPiaoShu[5];                  // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10
	BYTE                cbGameBaseScore[5];            // ��÷�׷�
	BYTE				cbGameMaxScore[5];			   // �ⶥ����
	SYSTEMTIME			dCreateTime[5];		           // �������ֲ���ʱ��
	DWORD				dwOnlineNum[5];				   // ��Ա��������
	
};

// DBR_GR_CLUB_MEMBER_LIST_REQ     169     // ���ֲ���Ա�б�����
struct  DBO_GR_Club_Member_List_Req 
{
	DWORD dwUserID; // �᳤ID
	DWORD dwClubID; // ���ֲ�ID
	DWORD dwFlag; // 0���������б�1�����Ա�б�
};

// DBR_GR_UPDATE_CLUB_REQ       170     �᳤���¾��ֲ�����
struct  DBR_GR_Update_Club_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	BYTE bGameTypeIndex;			// ��Ϸ���� 2���3���ݻλΣ�4Ϫˮ
	BYTE bGameTypeQing;				// 2������ ��1 û����
	BYTE bPlayCoutIdex;				// ��Ϸ����
	BYTE bGamePayType;				// ��Ϸ��������
	BYTE bMaxScore;                 // �ⶥ����
	BYTE bQiangGangHu;				// ���ݵ����ܺ� 0û�У�1��
	BYTE bAllOther;					// Ϫˮ��ȫ���� 0û�У�1��
	BYTE bPantherType;				// �� �� �� ɫ ��
	BYTE bPlayerNum;				// ��Ϸ����
	BYTE bBaoIsOutCard;             // �� 0:���ɴ�  1:�ɴ� 2:�ޱ�
	BYTE bPiaoShu;                  // Ʈ�� 0 ��Ҫ 1 Ʈ1 2 Ʈ2... 10 Ʈ10	
	BYTE bGameBaseScore;            // ��Ϸ�׷�
	TCHAR stClubName[LEN_NICKNAME]; // ���ֲ�����
	TCHAR stClubNotice[124];        // ����
};

// DBR_GR_DISMISS_CLUB_REQ			171		 ��ɢ���ֲ�����
struct DBR_GR_Dismiss_Club_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
};

// DBR_GR_CLUB_CHANGE_MASTER_REQ			172		 ת�û᳤����
struct DBR_GR_Club_Change_Master_Req 
{
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	DWORD dwMasterID;				// �¸��᳤ID(ת�ú��)
};

// DBR_GR_CLUB_SET_MANAGER_REQ		173		 �᳤���ù���Ա����
struct DBR_GR_Club_Set_Manager
{	
	DWORD dwUserID;					// �᳤ID
	DWORD dwClubID;					// ���ֲ�ID
	DWORD dwManagerID;				// ����ԱID
};


//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ο͵�¼
struct DBR_GP_LogonVisitor
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szComputerID[LEN_MACHINE_ID];		//��������
	TCHAR							szNickName[LEN_ACCOUNTS];			//����
	TCHAR							szPassWord[LEN_MD5];				//��¼����
	TCHAR							szPassWordBank[LEN_MD5];			//��¼����
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyAccounts
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesAccount[LEN_ACCOUNTS];			//�û��ʺ�
};

//�޸��Ƽ���
struct DBR_GP_ModifySpreader
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�û��ʺ�
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
	
	TCHAR							szHeadHttp[LEN_USER_NOTE];			//ͷ��HTTP
	TCHAR							szUserChannel[LEN_NICKNAME];		//������
	
	TCHAR							szUserGps[LEN_NICKNAME];			//GPS
};

//�޸�����
struct DBR_GP_ModifyStarValue
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwStarValue;						//���ӵ�ַ
};

//��������Ȧ����
struct DBR_GP_ShareAward
{
	DWORD							dwUserID;
};

//��ѯ�û���Ϸ״̬
struct DBR_GP_QueryUserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ������Ϣ
struct DBR_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ����
struct DBR_GP_QueryInsureHestory
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//��ѯǩ��
struct DBR_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ִ��ǩ��
struct DBR_GP_CheckInDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

////////////////////////////////////////////////////////////////////////////////////////////
//����������ѯ
struct DBR_GP_BeginnerQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};
struct DBR_GP_BeginnerDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

////////////////////////////////////////////////////////////////////////////////////////////

//�ͱ�����
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��ȡ�ͱ�
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct DBO_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//���ؽ���
struct DBR_GR_LoadMatchReward
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������	
};

//�޸Ļ���
struct DBR_GP_PublicNotic
{
	TCHAR							szKeyName[LEN_NICKNAME];			//�ؼ���
};

/////////////////////////////////////
//����

//���ؽ���
struct DBR_GR_ReqUserRouletteAward
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwVaildDay;							//��Чʱ��
};

//������Ϣ
struct DBR_GP_RouletteDone
{
	DWORD							dwUserID;					//�û� I D
	SCORE							lRewardGold;				//�������
	BYTE							cbRewardType;				//�������� 1��� 2����
	BYTE							cbRewardIdex;				//�������� 1��� 2����
};

/////////////////////////////////////
//׬���
struct DBR_GP_GetAddBankConfig
{
	INT								iIdex;
};
//������а�
struct DBR_GP_GetAddBank
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	INT								iIdex;
};


//��������
struct DBR_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��������
struct DBR_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};


//��Ϸ��¼
struct DBR_GP_GameRecordList
{
	DWORD							dwUserID;							
};


//��Ϸ�ط�
struct DBR_GP_GetGameTotalRecord
{
	DWORD							dwUserID;		
	DWORD							dwRecordID;							
};

//��Ϸ�ط�
struct DBR_GP_GetGameChildRecord
{	
	DWORD							dwRecordID;							
};

//���Ѷһ��б�
struct DBR_GP_GetExchangeHuaFei
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
};

//���Ѷһ�
struct DBR_GP_ExchangeDone
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� ID
	DWORD							dwExchangeID;						//�һ� ID
	TCHAR							szPassword[LEN_MD5];				//��¼����
};

//�̳�
struct DBR_GP_GetShopInfo
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
};
//�̳�
struct DBR_GP_UpShopOder
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

//����
struct DBR_GP_GetSellRecordList
{
	DWORD						dwUserID;
};



#endif
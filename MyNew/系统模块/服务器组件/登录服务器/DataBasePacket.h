#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#include "DataBasePacketIn.h"
#include "DataBasePacketInOut.h"


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			500									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		501									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	502									//ע���ʺ�
#define DBR_GP_VISITOR_ACCOUNTS		503									//�ο͵�¼

//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif
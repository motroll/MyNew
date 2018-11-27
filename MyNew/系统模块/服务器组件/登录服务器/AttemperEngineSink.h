#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"

//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������

//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwUserID;							//���id
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾
};

//�ͱ�����
struct tagBaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//////////////////////////////////////////////////////////////////////////////////

//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬

	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������

	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����
	CMD_GP_CheckInInfo				m_kCheckInInfo;						//ǩ������
	CMD_GP_BeginnerInfo				m_kBeginnerInfo;					//���ֻ
	CMD_GP_BackAddBankAwardInfo		m_BackAddBankAwardInfo;				//׬�����	
	CMD_GP_BackRouletteUeserInfo	m_kBackRouletteUeserInfo;			//���̽���
	tagBaseEnsureParameter			m_BaseEnsureParameter;				//�ͱ�����	
	tagGameConfig					m_kGameConfig;						//��Ϸ����
	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���Ӵ���
protected:
	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼����
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�����¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubPCVisitor(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	
	//�ֻ��¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��Ϸ�¼�
protected:
	//��¼�ɹ�
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ͷ��
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�������Ϣ
	bool OnDBPCUserAccountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnDBPCUserInsureReturn(DWORD dwContextID, VOID* pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ�ɹ�
	bool OnDBUserPaySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�Զ������ݲ�ѯ
	bool OnDBPCPublicNotice(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ״̬
	bool OnDBPCInGameSevrerID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����Ƽ��˽��
	bool OnDBPCOSpreaderResoult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ɹ�
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	VOID SendUserSignupInfo(DWORD dwSocketID,WORD wSignupCount,tagSignupMatchInfo * pSignupMatchInfo);

	VOID SendOperateFailure(DWORD dwContextID, LONG lResultCode, LPCTSTR pszDescribe);
	// ��ѯ��ҳ�ֵ��¼
	bool OnDBUserInsuerHistory(DWORD dwContextID, VOID * pData, WORD wDataSize);

public:
	//�������
	bool OnDBPCUserMatchSignupResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��������
	bool OnDBPCMacthAwardList( DWORD dwContextID, VOID * pData, WORD wDataSize );

	//�ֻ��¼�
protected:
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�б��¼�
protected:
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ڵ�
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ������
	bool OnDBPCCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ����Ϣ
	bool OnDBPCUserCheckInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ�����
	bool OnDBPCUserCheckInResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������������
	bool OnDBPCBeginnerConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserBeginnerInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����������
	bool OnDBPCUserBeginnerResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���̽�������
	bool OnDBPCUserRouletteAward(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������Ϣ
	bool OnDBPCUserRouletteUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϸ����
	bool OnDBPCGameConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//׬�����
	bool OnDBPCAddBankAwardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//׬�������
	bool OnDBPCAddBankBack(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ͱ�����
	bool OnDBPCBaseEnsureParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ͱ����
	bool OnDBPCBaseEnsureResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//�һ������б�
	bool OnDBPCExchangeHuaFeiBack(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�̳��б�
	bool OnDBPCShopInfoBack(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�̳��б�
	bool OnDBPCSellListBack(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϸ��¼
	bool OnDBPCGameRecordList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ��¼
	bool OnDBPCGameRecordTotal(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ��¼
	bool OnDBPCGameRecordChild(DWORD dwContextID, VOID * pData, WORD wDataSize);

	// ���ֲ��¼���Ӧ
protected:
	// �����򴴽����ֲ���Ӧ
	bool  OnDBPCCreateJoinRes(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// ��ȡ�Լ��ľ��ֲ���Ӧ
	bool  OnDBPGetMyClubRes(DWORD dwContextID, VOID * pData, WORD wDataSize);
	// �᳤���ֲ���Ա�б��Ӧ
	bool OnDBPCClubMasterListRes(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ǩ������
protected:
	//��ѯǩ��
	bool OnTCPNetworkSubPCCheckinQuery(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//ִ��ǩ��
	bool OnTCPNetworkSubPCCheckInDone(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//ǩ������
	bool OnTCPNetworkSubPCCheckAward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ͱ�
	bool OnTCPNetworkSubPCBaseensureLoad(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	bool OnTCPNetworkSubPCBaseensureTake(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���̷���
protected:
	
	//������Ϣ
	bool OnTCPNetworkSubPCRouletteUserInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//ִ������
	bool OnTCPNetworkSubPCRouletteDone(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���ֻ
protected:
	//��ѯ���ֻ
	bool OnTCPNetworkSubPCBiggerQuery(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//ִ�����ֻ
	bool OnTCPNetworkSubPCBiggerAward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//׬���
protected:
	//׬�������
	bool OnTCPNetworkSubGetAddBank(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	bool OnTCPNetworkSubAddBankAwardInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�һ�����
protected:
	//�һ������б�
	bool  OnTCPNetworkSubGetExchangeHuaFei(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�һ�
	bool  OnTCPNetworkSubExchangeDone(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	
	//�̳�
protected:
	//�̳��б�
	bool  OnTCPNetworkSubGetShopInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//����
	bool  OnTCPNetworkSubBuyOder(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ҳ���
	bool OnTCPNetworkSubSellItem(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ҳ��ۼ�¼
	bool OnTCPNetworkSubGetSellItemRecord(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	// ���ֲ�
protected:
	// �᳤�������ֲ�
	bool OnTCPNetworkSubCreatClub(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	// ���������ֲ�
	bool OnTCPNetworkSubJoinClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �᳤ͬ���ͬ�������ֲ�
	bool OnTCPNetworkSubJoinClubComfirm(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ��ȡ�Լ��ľ��ֲ�����
	bool OnTCPNetworkSubGetMyClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �뿪���ֲ�
	bool OnTCPNetworkSubLeaveClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �߳����ֲ�
	bool OnTCPNetworkSubKickClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ������Ҽ�����ֲ�
	bool OnTCPNetworkSubInviteClub(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �᳤���ֲ���Ա�б�����
	bool OnTCPNetworkSubMmmberListReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �᳤���¾��ֲ���������
	bool OnTCPNetworkSubUpdateClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ��ɢ���ֲ�����
	bool OnTCPNetworkSubDismissClubReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ת�û᳤����
	bool OnTCPNetworkSubChangeMasterReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// �᳤���ù���Ա����
	bool OnTCPNetworkSubSetManagerReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ��������ַ����
	bool OnTCPNetworkServerAdressReq(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	// ��������ַ��Ӧ
	bool OnTCPNetworkServerAdressRes(WORD wSubCmdID, VOID *pData, WORD wDataSize, DWORD dwSocketID);
	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͽڵ�
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//���Ͷ���
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID);

	//�ֻ��б�
protected:
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//���ͷ���
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);

};

//////////////////////////////////////////////////////////////////////////////////

#endif
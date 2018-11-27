#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef IServerUserItem * CTableUserItemArray[MAX_CHAIR];				//��Ϸ����
typedef CWHArray<IServerUserItem *> CLookonUserItemArray;				//�Թ�����
typedef CWHArray<tagGameScoreRecord *> CGameScoreRecordArray;			//��¼����
interface IMatchTableFrameNotify;
//////////////////////////////////////////////////////////////////////////////////

//���ӿ��
class CTableFrame : public ITableFrame
{
	//��Ϸ����
protected:
	WORD							m_wTableID;							//���Ӻ���
	WORD							m_wChairCount;						//������Ŀ
	BYTE							m_cbStartMode;						//��ʼģʽ
	WORD							m_wUserCount;						//�û���Ŀ
	BYTE							m_bLimitIP;
	BYTE                            m_cbPlayerNum;                      //��Ϸ����

	//״̬����
protected:
	bool							m_bGameStarted;						//��Ϸ��־
	bool							m_bDrawStarted;						//��Ϸ��־
	bool							m_bTableStarted;					//��Ϸ��־

	bool							m_bDingZhuang;						//��Ϸ���
	bool							m_bRollDice;						//������
	int								m_DiceTimes;						//ѡׯ���һ�ο�ʼ
	int								m_readyNum;
	int								m_iRollCount;
	BYTE							m_nSitDownSt[MAX_CHAIR];
	BYTE							m_nZhuang[MAX_CHAIR];				//ׯ���
	BYTE							m_nDice[MAX_CHAIR];					//���ӱ��
	//״̬����
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//�Թ۱�־
	SCORE							m_lFrozenedScore[MAX_CHAIR];		//�������

	//��Ϸ����
protected:
	LONG							m_lCellScore;						//��Ԫ����
	BYTE							m_cbGameStatus;						//��Ϸ״̬

	//ʱ�����
protected:
	DWORD							m_dwDrawStartTime;					//��ʼʱ��
	SYSTEMTIME						m_SystemTimeStart;					//��ʼʱ��

	//��̬����
protected:
	DWORD							m_dwTableOwnerID;					//�����û�
	TCHAR							m_szEnterPassword[LEN_PASSWORD];	//��������

	//���߱���
protected:
	WORD							m_wOffLineCount[MAX_CHAIR];			//���ߴ���
	DWORD							m_dwOffLineTime[MAX_CHAIR];			//����ʱ��

	//�û�����
protected:
	CTableUserItemArray				m_TableUserItemArray;				//��Ϸ�û�
	CLookonUserItemArray			m_LookonUserItemArray;				//�Թ��û�

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	ITableFrameSink	*				m_pITableFrameSink;					//���ӽӿ�
	IMainServiceFrame *				m_pIMainServiceFrame;				//����ӿ�
	IAndroidUserManager *			m_pIAndroidUserManager;				//�����ӿ�

	//��չ�ӿ�
protected:
	ITableUserAction *				m_pITableUserAction;				//�����ӿ�
	ITableUserRequest *				m_pITableUserRequest;				//����ӿ�

	//���ݽӿ�
protected:
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//�ں�����
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//��¼����

	//������Ϣ
protected:
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//��Ϸ��¼
protected:
	CGameScoreRecordArray			m_GameScoreRecordActive;			//��Ϸ��¼
	static CGameScoreRecordArray	m_GameScoreRecordBuffer;			//��Ϸ��¼
	datastream						m_UserDefine;						//�û��Զ�������

	//�����ӿ�
protected:
	ITableFrameHook	*				m_pITableFrameHook;					//�����ӿ�
	ITableUserAction *				m_pIMatchTableAction;				//�����ӿ�

	//˽�˳��ӿ�
protected:
	ITableFramePrivate	*			m_pITableFramePrivate;					//˽�˳��ӿ�
	ITableUserAction *				m_pIPrivateTableAction;				//˽�˳������ӿ�


	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���Խӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID() { return m_wTableID; }
	//��Ϸ����
	virtual WORD GetChairCount() { return m_wChairCount; }
	//��λ����Ŀ
	virtual WORD GetNullChairCount(){return m_wChairCount - m_wUserCount;}
	//������Ŀ
	virtual WORD GetOffLineUserCount();
	//�û���Ŀ
	virtual WORD GetSitUserCount();

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//���ýӿ�
public:
	//��ʼģʽ
	virtual BYTE GetStartMode() { return m_cbStartMode; }
	//��ʼģʽ
	virtual VOID SetStartMode(BYTE cbStartMode) { m_cbStartMode=cbStartMode; }

	//��Ԫ����
public:
	//��Ԫ����
	virtual LONG GetCellScore() { return m_lCellScore; }
	//��Ԫ����
	virtual VOID SetCellScore(LONG lCellScore);


	//��Ϣ�ӿ�
public:
	//��Ϸ״̬
	virtual bool IsGameStarted() { return m_bGameStarted; }
	//��Ϸ״̬
	virtual bool IsDrawStarted() { return m_bDrawStarted; }
	//��Ϸ״̬
	virtual bool IsTableStarted() { return m_bTableStarted; }
	//����״̬
	virtual bool IsTableLocked() { return (m_szEnterPassword[0]!=0); }

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }

	//���ƽӿ�
public:
	//������
	virtual bool RollDice();
	//��ʼ��Ϸ
	virtual bool StartGame(WORD chairID);
	//��ɢ��Ϸ
	virtual bool DismissGame();
	//������Ϸ
	virtual bool ConcludeGame(BYTE cbGameStatus);
	//��������
	virtual bool ConcludeTable();
	//����������Ϸ
	virtual bool TableFrameEnd();
	//�û��ӿ�
public:
	//Ѱ���û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChairID);
	//�Թ��û�
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//д�ֽӿ�
public:
	//д�����
	virtual bool WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo);
	//д�����
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount,datastream kDataStream = datastream());

	//����ӿ�
public:
	//����˰��
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore);
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);

	//ʱ��ӿ�
public:
	//����ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//ɾ��ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID);

	//��Ϸ�û�
public:
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID=MDM_GF_GAME,bool needReady=true);

	//�Թ��û�
public:
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�������������Ϣ
	virtual bool GetTableChairInfo(IServerUserItem * pIServerUserItem);
	//�����û�
public:
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//ϵͳ��Ϣ
public:
	//������Ϣ
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//��������
public:
	//��������
	virtual bool PerformStandUpAction(IServerUserItem * pIServerUserItem);
	//��������
	virtual bool PerformStandUpActionEx(IServerUserItem * pIServerUserItem);
	//�Թ۶���
	virtual bool PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//���¶���
	virtual int PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword=NULL);
	//����ѡ��
	virtual bool PerformSelectChairAction(IServerUserItem * pIServerUserItem);
	//δѡ��״̬���л�λ��
	virtual bool performExchangeChairAction(IServerUserItem* pIServerUserItem, WORD exchangeChairID);
	//���ܽӿ�
public:
	//���ͳ���
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����ӿ�
public:
	//���ýӿ�
	virtual bool SetTableFrameHook(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual IUnknownEx * GetTableFrameHook(){ return m_pITableFrameHook;}

	//˽�˳��ӿ�
public:
	//���ýӿ�
	virtual bool SetTableFramePrivate(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual IUnknownEx * GetTableFramePrivate(){ return m_pITableFramePrivate;}
	//����¼�
	virtual void AddPrivateAction(DWORD dwChairID,BYTE	bActionIdex);
	//����˽�˳���Ϣ
	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD	bGameRuleIdex, BYTE bBaseScore, BYTE bGuDingMa, BYTE bQiangGangHu, BYTE bGameType, BYTE bLimitIP);
	virtual void SetPrivateFushunInfo(BYTE bGameTypeIndex,BYTE	bGameTypeQing, BYTE bLimitIP,
		BYTE bGameMaxScore,BYTE bGameMaxScoreGold,BYTE bPiaoShu,BYTE bBaoIsOutCard,BYTE bFendDing,BYTE bJingDing,BYTE bAllOther, BYTE	cbPantherType,BYTE cbPlayerNum, DWORD dwClubID = 0);
	virtual void SetCreateUserID(DWORD dwUserID);
	//��λ������Ϣ
	virtual VOID  ResAccountInfo();
	
	//���ܺ���
public:
	//��ȡ��λ
	WORD GetNullChairID();
	//�����λ
	WORD GetRandNullChairID();
	//�Թ���Ŀ
	WORD GetLookonUserCount();

	//���ܺ���
public:
	//�û�״��
	WORD GetTableUserInfo(tagTableUserInfo & TableUserInfo);
	//��������
	bool InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter);

	//�û��¼�
public:
	//�����¼�
	bool OnEventUserOffLine(IServerUserItem * pIServerUserItem);
	//�����¼�
	bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//ϵͳ�¼�
public:
	//ʱ���¼�
	bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ�¼�
	bool OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//����¼�
	bool OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��������
public:
	//����״̬
	bool SendTableStatus();
	//����ʧ��
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);

	//Ч�麯��
public:
	//��ʼЧ��
	bool EfficacyStartGame(WORD wReadyChairID);
	//��ַЧ��
	bool EfficacyIPAddress(IServerUserItem * pIServerUserItem);
	//����Ч��
	bool EfficacyScoreRule(IServerUserItem * pIServerUserItem);

public :
	bool OnChangeRoomServiceNotify(int iRoomID);

public:
	virtual void SendGameStatus(VOID* pData, WORD wDataSize);
	bool updatePlayerStatus();

};

//////////////////////////////////////////////////////////////////////////////////

#endif
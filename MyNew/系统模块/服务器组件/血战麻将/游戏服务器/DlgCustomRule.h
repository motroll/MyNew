#pragma once
#include "Stdafx.h"
#include "TableFrameSink.h"

//�Զ�������
struct tagCustomConfig
{
	//��ׯ��Ϣ	
	BOOL						cbGameType1;	
	BOOL						cbGameType2;			
	BOOL						cbGameRule[20];					
	//���캯��
	tagCustomConfig()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		cbGameType1 = false;
		cbGameType2 = false;
		ZeroMemory(cbGameRule,0);
	}
};


class CDlgCustomRule : public CDialog
{
	//���ñ���
protected:
	WORD							m_wCustomSize;						//���ô�С
	LPBYTE							m_pcbCustomRule;					//���û���

	//���ýṹ
protected:
	tagCustomConfig					m_CustomConfig;						//�Զ�����

	//��������
public:
	//���캯��
	CDlgCustomRule();
	//��������
	virtual ~CDlgCustomRule();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//��������
	bool SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	//��������
	bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	//Ĭ������
	bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize);
	DECLARE_MESSAGE_MAP()
};

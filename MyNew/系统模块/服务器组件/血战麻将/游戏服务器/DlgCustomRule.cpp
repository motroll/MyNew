// DlgCustomRule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"


// CDlgCustomRule �Ի���

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
}

//��������
CDlgCustomRule::~CDlgCustomRule()
{
}

//�ؼ���
VOID CDlgCustomRule::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_CHECK_SC,m_CustomConfig.cbGameType1);
	DDX_Check(pDX,IDC_CHECK_CD,m_CustomConfig.cbGameType2);

	DDX_Check(pDX,IDC_CHECK_XZ,m_CustomConfig.cbGameRule[1]);
	DDX_Check(pDX,IDC_CHECK_XL,m_CustomConfig.cbGameRule[2]);
	DDX_Check(pDX,IDC_CHECK_HUAN3,m_CustomConfig.cbGameRule[3]);
	DDX_Check(pDX,IDC_DING_QUE,m_CustomConfig.cbGameRule[4]);

}

//��ʼ������
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	return FALSE;
}

//ȷ������
VOID CDlgCustomRule::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//ȡ����Ϣ
VOID CDlgCustomRule::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//��������
bool CDlgCustomRule::SetCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return false;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	memcpy(&m_CustomConfig, pCustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateData(TRUE);

	return true;
}

//��������
bool CDlgCustomRule::SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���½���
	if( m_hWnd )
		UpdateData(TRUE);

	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	return true;
}

//��������
bool CDlgCustomRule::DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustomSize)
{
	//���ñ���
	m_wCustomSize=wCustomSize;
	m_pcbCustomRule=pcbCustomRule;

	//���ñ���
	ASSERT(m_pcbCustomRule);
	if( !m_pcbCustomRule ) return true;
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pcbCustomRule;

	m_CustomConfig.DefaultCustomRule();
	memcpy(pCustomConfig, &m_CustomConfig, sizeof(tagCustomConfig));

	//���½���
	if( m_hWnd )
		UpdateData(TRUE);

	return true;
}

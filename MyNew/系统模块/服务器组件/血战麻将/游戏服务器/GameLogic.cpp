#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"
#include "Randkiss.h"
#include <algorithm>
extern CRandKiss g_RandKiss;

//////////////////////////////////////////////////////////////////////////

//��̬����
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//���캯��
CChiHuRight::CChiHuRight()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );

	if( !m_bInit )
	{
		m_bInit = true;
		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( 0 == i )
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (DWORD(pow((float)2,(float)(i-1))))<<28;
		}
	}
}

//��ֵ������
CChiHuRight & CChiHuRight::operator = ( DWORD dwRight )
{
	DWORD dwOtherRight = 0;
	//��֤Ȩλ
	if( !IsValidRight( dwRight ) )
	{
		//��֤ȡ��Ȩλ
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator &= ( DWORD dwRight )
{
	bool bNavigate = false;
	//��֤Ȩλ
	if( !IsValidRight( dwRight ) )
	{
		//��֤ȡ��Ȩλ
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		//����Ȩλ
		DWORD dwHeadRight = (~dwRight)&0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight|dwTailRight;
		bNavigate = true;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if( !bNavigate )
			m_dwRight[i] = 0;
	}

	return *this;
}

//�����
CChiHuRight & CChiHuRight::operator |= ( DWORD dwRight )
{
	//��֤Ȩλ
	if( !IsValidRight( dwRight ) ) return *this;

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
	}

	return *this;
}

//��
CChiHuRight CChiHuRight::operator & ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator & ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//��
CChiHuRight CChiHuRight::operator | ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//��
CChiHuRight CChiHuRight::operator | ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//�Ƿ�ȨλΪ��
bool CChiHuRight::IsEmpty()
{
	for( BYTE i = 0; i < CountArray(m_dwRight); i++ )
		if( m_dwRight[i] ) return false;
	return true;
}

//����ȨλΪ��
void CChiHuRight::SetEmpty()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	return;
}

//��ȡȨλ��ֵ
BYTE CChiHuRight::GetRightData( DWORD dwRight[], BYTE cbMaxCount )
{
	ASSERT( cbMaxCount >= CountArray(m_dwRight) );
	if( cbMaxCount < CountArray(m_dwRight) ) return 0;

	CopyMemory( dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight) );
	return CountArray(m_dwRight);
}

//����Ȩλ��ֵ
bool CChiHuRight::SetRightData( const DWORD dwRight[], BYTE cbRightCount )
{
	ASSERT( cbRightCount <= CountArray(m_dwRight) );
	if( cbRightCount > CountArray(m_dwRight) ) return false;

	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	CopyMemory( m_dwRight,dwRight,sizeof(DWORD)*cbRightCount );
	
	return true;
}

//����λ�Ƿ���ȷ
bool CChiHuRight::IsValidRight( DWORD dwRight )
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		if( m_dwRightMask[i] == dwRightHead ) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//��̬����

//�˿�����
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
};

const BYTE CGameLogic::m_cbCardDataArrayWF[MAX_REPERTORY_WUFENG]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
};

const BYTE CGameLogic::m_cbCardDataArrayHZWF[MAX_REPERTORY_HZWUFENG] =
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��

	0x35,																//����
	0x35,																//����
	0x35,																//����
	0x35,																//����
};

//���ݻλ��˿�����
const BYTE CGameLogic::m_cbCardDataArrayHZHH[MAX_REPERTORY_HUANGZHOU]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��

	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����

	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
};
//�˿�����
const BYTE CGameLogic::m_cbCardDataArrayTWTH[TWO_THREE_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��


	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����

	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//���������з���
};
const BYTE CGameLogic::m_cbCardDataArrayTWTHHZHH[TWO_THREE_HUANGZHOU]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//ͬ��

	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//����

	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
	0x35,0x36,0x37,														//�з���
};


//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
	m_cbLaiPiIndex =  MAX_INDEX;
	m_iBaoIndex = MAX_INDEX;
	m_cbPiIndex = MAX_INDEX;
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
int CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArray);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 4;

	BYTE cardDatas[MAX_REPERTORY];
	ZeroMemory(cardDatas,sizeof(cardDatas));

	if(nrand <= 1)
	{
		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{ 
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount); 
			cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);  //��ǰ�����

		for(int i = 0; i < cbMaxCount; i++ ) //��ֵ����
		{
			cbPosition=g_RandKiss.RandKiss()%cbMaxCount;  //�����һ��λ��
			BYTE  bCardValue = cardDatas[cbPosition];
			cardDatas[cbPosition] =   cardDatas[i];
			cardDatas[i] =   bCardValue;
		}
		cbRandCount = cbMaxCount;
		ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
		CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		ZeroMemory(cardDatas,sizeof(cardDatas));
		do
		{ 
			cbPosition=g_RandKiss.RandKiss()% cbRandCount;  
			cardDatas[cbRandCount - 1]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbRandCount - 1];
			cbRandCount--;
		} while (cbRandCount>0);  //�Ӻ���ǰ��

		//ǰһ��ͺ�һ���ƽ����������
		for(int i =0 ;i < cbMaxCount/2; i++)
		{
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount/2);  //�����һ��λ��
			BYTE  cbCardValue = cardDatas[i+cbMaxCount/2];
			cardDatas[i+cbMaxCount/2] = cardDatas[cbPosition];
			cardDatas[cbPosition] = cbCardValue;
		}
	}
	else
	{
		for (int i = 0;i < nrand; i ++)
		{
			//�����˿�
			BYTE cbRandCount=0,cbPosition=0;
			ZeroMemory(cardDatas,sizeof(cardDatas));
			do
			{
				cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
				cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);

			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
			cbRandCount = cbMaxCount;
			do
			{ 
				cbPosition=g_RandKiss.RandKiss()% cbRandCount;  
				cardDatas[cbRandCount - 1]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbRandCount - 1];
				cbRandCount--;
			} while (cbRandCount>0);  //�Ӻ���ǰ��
			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		}
	}
	BYTE cbPiTemp = 0xFF;
	BYTE cbLaiPiTemp = 0xFF;
	//BYTE cbHunTemp = 0xFF;
	cbPiTemp = cardDatas[0];
	//cbHunTemp = cbPiTemp + 1;
	cbLaiPiTemp = 0x35;
	//if(cbPiTemp<=0x29)
	//{
	//	if(cbPiTemp==0x09)
	//	{
	//		//cbHunTemp=0x01;
	//	}
	//	else if(cbPiTemp==0x19)
	//	{
	//		//cbHunTemp=0x11;
	//	}
	//	else if(cbPiTemp==0x29)
	//	{
	//		//cbHunTemp=0x21;
	//	}		
	//}
	//else
	{
		if (cbGameTypeIndex == 4)
		{
			if(cbPiTemp==0x34)
			{
				//cbHunTemp=0x31;
			}
			else if(cbPiTemp==0x35)
			{
				//cbHunTemp=0x36;
				cbLaiPiTemp=0x37;
			}
			else if(cbPiTemp==0x36)
			{
				//cbHunTemp=0x37;
			}
			else if(cbPiTemp==0x37)
			{
				//cbHunTemp=0x35;
				cbLaiPiTemp=0x36;
			}
		}
		else
		{
			if(cbPiTemp==0x34)
			{
				//cbHunTemp=0x35;
				cbLaiPiTemp=0x36;
			}
			else if(cbPiTemp==0x35)
			{
				//cbHunTemp=0x36;
				if (cbGameTypeIndex == 6)
				{
					cbLaiPiTemp=0x34;
				}
				else
					cbLaiPiTemp=0x37;
			}
			else if(cbPiTemp==0x36)
			{
				//cbHunTemp=0x37;
			}
			else if(cbPiTemp==0x37)
			{
				//cbHunTemp=0x31;
			}
		}
	}
	BYTE cbRandTemp = g_RandKiss.RandKiss() % 100;
	if (cbRandTemp < 40)
	{
		BYTE cbIndexTemp = 8;
		//BYTE cbIndexTemp1 = 12;
		BYTE cbTemp = 0;
		BYTE cbTemp1 = 0;
		for (BYTE j = 135; j >= 1; j--)
		{
			if (cbTemp < 2 && (cardDatas[j] == cbPiTemp || cardDatas[j] == cbLaiPiTemp))
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp];
				cardDatas[cbIndexTemp] = cbCardValueTemp;
				cbIndexTemp++;
				cbTemp++;
			}
			/*else if (cbTemp1 < 2 && cardDatas[j] == cbHunTemp)
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp1];
				cardDatas[cbIndexTemp1] = cbCardValueTemp;
				cbIndexTemp1 = 40;
				cbTemp1++;
			}*/
			if (cbTemp >= 2/* && cbTemp1 >= 2*/)
			{
				break;
			}
		}
	}
	else
	{
		BYTE cbIndexTemp = 8;
		//BYTE cbIndexTemp1 = 12;
		BYTE cbTemp = 0;
		//BYTE cbTemp1 = 0;
		for (BYTE j = 135; j >= 1; j--)
		{
			if (cbTemp < 3 && (cardDatas[j] == cbPiTemp || cardDatas[j] == cbLaiPiTemp))
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp];
				cardDatas[cbIndexTemp] = cbCardValueTemp;
				cbIndexTemp++;
				cbTemp++;
			}
			/*else if (cbTemp1 < 1 && cardDatas[j] == cbHunTemp)
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp1];
				cardDatas[cbIndexTemp1] = cbCardValueTemp;
				cbIndexTemp1++;
				cbTemp1++;
			}*/
			if (cbTemp >= 3 /*&& cbTemp1 >= 1*/)
			{
				break;
			}
		}
	}
	CopyMemory(cbCardData, cardDatas, sizeof(cardDatas));

	return cbMaxCount;
}
//�����˿�
int CGameLogic::RandCardDataTwoThree(BYTE cbCardData[], BYTE cbMaxCount, BYTE cbGameTypeIndex, BYTE cbGameTypeQing)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayTWTH);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayTWTH)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayTWTH,sizeof(m_cbCardDataArrayTWTH));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 4;

	BYTE cardDatas[TWO_THREE_REPERTORY];
	ZeroMemory(cardDatas,sizeof(cardDatas));

	if(nrand <= 1)
	{
		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{ 
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount); 
			cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);  //��ǰ�����

		for(int i = 0; i < cbMaxCount; i++ ) //��ֵ����
		{
			cbPosition=g_RandKiss.RandKiss()%cbMaxCount;  //�����һ��λ��
			BYTE  bCardValue = cardDatas[cbPosition];
			cardDatas[cbPosition] =   cardDatas[i];
			cardDatas[i] =   bCardValue;
		}
		cbRandCount = cbMaxCount;
		ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
		CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		ZeroMemory(cardDatas,sizeof(cardDatas));
		do
		{ 
			cbPosition=g_RandKiss.RandKiss()% cbRandCount;  
			cardDatas[cbRandCount - 1]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbRandCount - 1];
			cbRandCount--;
		} while (cbRandCount>0);  //�Ӻ���ǰ��

		//ǰһ��ͺ�һ���ƽ����������
		for(int i =0 ;i < cbMaxCount/2; i++)
		{
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount/2);  //�����һ��λ��
			BYTE  cbCardValue = cardDatas[i+cbMaxCount/2];
			cardDatas[i+cbMaxCount/2] = cardDatas[cbPosition];
			cardDatas[cbPosition] = cbCardValue;
		}
	}
	else
	{
		for (int i = 0;i < nrand; i ++)
		{
			//�����˿�
			BYTE cbRandCount=0,cbPosition=0;
			ZeroMemory(cardDatas,sizeof(cardDatas));
			do
			{
				cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
				cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);

			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
			cbRandCount = cbMaxCount;
			do
			{ 
				cbPosition=g_RandKiss.RandKiss()% cbRandCount;  
				cardDatas[cbRandCount - 1]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbRandCount - 1];
				cbRandCount--;
			} while (cbRandCount>0);  //�Ӻ���ǰ��
			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		}
	}
	BYTE cbPiTemp = 0xFF;
	BYTE cbLaiPiTemp = 0xFF;
	BYTE cbHunTemp = 0xFF;
	cbPiTemp = cardDatas[0];
	cbHunTemp = cbPiTemp + 1;
	cbLaiPiTemp = 0x35;
	if(cbPiTemp<=0x29)
	{
		if(cbPiTemp==0x09)
		{
			cbHunTemp=0x01;
		}
		else if(cbPiTemp==0x29)
		{
			cbHunTemp=0x21;
		}		
	}
	else
	{
		if (cbGameTypeIndex == 4)
		{
			if(cbPiTemp==0x34)
			{
				cbHunTemp=0x31;
			}
			else if(cbPiTemp==0x35)
			{
				cbHunTemp=0x36;
				cbLaiPiTemp=0x37;
			}
			else if(cbPiTemp==0x36)
			{
				cbHunTemp=0x37;
			}
			else if(cbPiTemp==0x37)
			{
				cbHunTemp=0x35;
				cbLaiPiTemp=0x36;
			}
		}
		else
		{
			if(cbPiTemp==0x34)
			{
				cbHunTemp=0x35;
				cbLaiPiTemp=0x36;
			}
			else if(cbPiTemp==0x35)
			{
				cbHunTemp=0x36;
				if (cbGameTypeIndex == 6)
				{
					cbLaiPiTemp=0x34;
				}
				else
					cbLaiPiTemp=0x37;
			}
			else if(cbPiTemp==0x36)
			{
				cbHunTemp=0x37;
			}
			else if(cbPiTemp==0x37)
			{
				cbHunTemp=0x31;
			}
		}
	}
	BYTE cbRandTemp = g_RandKiss.RandKiss() % 100;
	if (cbRandTemp < 40)
	{
		BYTE cbIndexTemp = 8;
		BYTE cbIndexTemp1 = 12;
		BYTE cbTemp = 0;
		BYTE cbTemp1 = 0;
		for (BYTE j = TWO_THREE_REPERTORY - 1; j >= 1; j--)
		{
			if (cbTemp < 2 && (cardDatas[j] == cbPiTemp || cardDatas[j] == cbLaiPiTemp))
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp];
				cardDatas[cbIndexTemp] = cbCardValueTemp;
				cbIndexTemp++;
				cbTemp++;
			}
			else if (cbTemp1 < 2 && cardDatas[j] == cbHunTemp)
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp1];
				cardDatas[cbIndexTemp1] = cbCardValueTemp;
				cbIndexTemp1 = 40;
				cbTemp1++;
			}
			if (cbTemp >= 2 && cbTemp1 >= 2)
			{
				break;
			}
		}
	}
	else
	{
		BYTE cbIndexTemp = 8;
		BYTE cbIndexTemp1 = 12;
		BYTE cbTemp = 0;
		BYTE cbTemp1 = 0;
		for (BYTE j = TWO_THREE_REPERTORY - 1; j >= 1; j--)
		{
			if (cbTemp < 3 && (cardDatas[j] == cbPiTemp || cardDatas[j] == cbLaiPiTemp))
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp];
				cardDatas[cbIndexTemp] = cbCardValueTemp;
				cbIndexTemp++;
				cbTemp++;
			}
			else if (cbTemp1 < 1 && cardDatas[j] == cbHunTemp)
			{
				BYTE cbCardValueTemp = cardDatas[j];
				cardDatas[j] = cardDatas[cbIndexTemp1];
				cardDatas[cbIndexTemp1] = cbCardValueTemp;
				cbIndexTemp1++;
				cbTemp1++;
			}
			if (cbTemp >= 3 && cbTemp1 >= 1)
			{
				break;
			}
		}
	}
	CopyMemory(cbCardData, cardDatas, sizeof(cardDatas));

	return cbMaxCount;
}
int CGameLogic::RandCardDataHzhhTwoThree(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayTWTHHZHH);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayTWTHHZHH)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayTWTHHZHH,sizeof(m_cbCardDataArrayTWTHHZHH));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 5;
	BYTE cardDatas[TWO_THREE_HUANGZHOU];
	ZeroMemory(cardDatas,sizeof(cardDatas));

	if(nrand <= 1)
	{
		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
			cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);
	}
	else
	{
		for (int i = 0;i < nrand; i ++)
		{
			//�����˿�
			BYTE cbRandCount=0,cbPosition=0;
			ZeroMemory(cardDatas,sizeof(cardDatas));
			do
			{
				cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
				cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);
			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		}
	}
	BYTE cbRandTemp = g_RandKiss.RandKiss() % 100;
	//������ʾ
	TCHAR szDescribe3[32]=TEXT("");
	sprintf_s(szDescribe3,TEXT("cbRandTemp = %d"),cbRandTemp);
	//��ʾ��Ϣ
	CTraceService::TraceString(szDescribe3,TraceLevel_Normal);
	if (cbRandTemp < 5)
	{
		// ʲôҲ����
	}
	else
	{
		BYTE cbPiTemp = 0xFF;
		BYTE cbBaoTemp = 0xFF;
		WORD wSiceTemp = WORD(cbSiceCount & 0xffff);
		BYTE cbSiceCountPiTemp = HIBYTE(wSiceTemp) + LOBYTE(wSiceTemp);
		cbPiTemp = cardDatas[cbSiceCountPiTemp * 2 - 2]; // Ƥ
		cbBaoTemp = cbPiTemp + 1; // Ƥ��һΪ��
		//������ʾ
		TCHAR szDescribe3[64]=TEXT("");
		sprintf_s(szDescribe3,TEXT("cbSiceCount = %d, cbSiceCountPiTemp * 2 - 2 = %d"),cbSiceCount, cbSiceCountPiTemp * 2 - 2);
		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe3,TraceLevel_Normal);
		BYTE cbIndexTemp = cbSiceCountPiTemp * 2 - 1;
		if (cbPiTemp == 0x09)
		{
			cbBaoTemp = 0x01;
		}
		else if (cbPiTemp == 0x29)
		{
			cbBaoTemp = 0x21;
		}
		else if (cbPiTemp == 0x37)
		{
			cbBaoTemp = 0x35;
		}
		//������ʾ
		TCHAR szDescribe4[32]=TEXT("");
		sprintf_s(szDescribe4,TEXT("cbBaoTemp = %02x"),cbBaoTemp);
		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe4,TraceLevel_Normal);
		if (cbRandTemp < 50)
		{
			for (BYTE i = TWO_THREE_HUANGZHOU - 1; i >= 0; i--)
			{
				if (cardDatas[i] == cbBaoTemp)
				{
					BYTE cbCardValueTemp = cardDatas[i];
					cardDatas[i] = cardDatas[cbIndexTemp];
					cardDatas[cbIndexTemp] = cbCardValueTemp;
					break;
				}
			}
		}
		else
		{
			BYTE cbTemp = 0;
			for (BYTE i = TWO_THREE_HUANGZHOU - 1; i >= 0; i--)
			{
				if (cardDatas[i] == cbBaoTemp)
				{
					BYTE cbCardValueTemp = cardDatas[i];
					cardDatas[i] = cardDatas[cbIndexTemp];
					cardDatas[cbIndexTemp] = cbCardValueTemp;
					cbIndexTemp++;
					cbTemp++;
					if (cbTemp >= 2) break;
				}
			}
		}
	}
	CopyMemory(cbCardData, cardDatas, sizeof(cardDatas));

	return cbMaxCount;
}

int CGameLogic::RandCardDataTwoThreeHM(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayTWTHHZHH);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayTWTHHZHH)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayTWTHHZHH,sizeof(m_cbCardDataArrayTWTHHZHH));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 5;
	BYTE cardDatas[TWO_THREE_HUANGZHOU];
	ZeroMemory(cardDatas,sizeof(cardDatas));

	if(nrand <= 1)
	{
		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
			cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);
	}
	else
	{
		for (int i = 0;i < nrand; i ++)
		{
			//�����˿�
			BYTE cbRandCount=0,cbPosition=0;
			ZeroMemory(cardDatas,sizeof(cardDatas));
			do
			{
				cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
				cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);
			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		}
	}
	CopyMemory(cbCardData, cardDatas, sizeof(cardDatas));
	return cbMaxCount;
}

int CGameLogic::RandCardDataNoFeng(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayWF);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayWF)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayWF,sizeof(m_cbCardDataArrayWF));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 5;
	for (int i = 0;i < nrand; i ++)
	{
		srand((unsigned int)time(NULL));
	}
	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);
	return cbMaxCount;
}

int CGameLogic::RandCardDataHZNoFeng(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayHZWF);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayHZWF)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayHZWF,sizeof(m_cbCardDataArrayHZWF));

	srand((unsigned int)time(NULL));
	int nrand = rand() % 4;
	for (int i = 0;i < nrand; i ++)
	{
		srand((unsigned int)time(NULL));
	}
	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);
	return cbMaxCount;
}

// ���ݻλ�ϴ��
int CGameLogic::RandCardDataHZHH(BYTE cbCardData[], BYTE cbMaxCount, LONG cbSiceCount)
{
	//����׼��
	cbMaxCount = CountArray(m_cbCardDataArrayHZHH);
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArrayHZHH)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArrayHZHH,sizeof(m_cbCardDataArrayHZHH));

	srand((unsigned int)time(NULL));
	int nrand = g_RandKiss.RandKiss() % 5;
	BYTE cardDatas[MAX_REPERTORY_HUANGZHOU];
	ZeroMemory(cardDatas,sizeof(cardDatas));

	if(nrand <= 1)
	{
		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
			cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);
	}
	else
	{
		for (int i = 0;i < nrand; i ++)
		{
			//�����˿�
			BYTE cbRandCount=0,cbPosition=0;
			ZeroMemory(cardDatas,sizeof(cardDatas));
			do
			{
				cbPosition=g_RandKiss.RandKiss()%(cbMaxCount-cbRandCount);
				cardDatas[cbRandCount++]=cbCardDataTemp[cbPosition];
				cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
			} while (cbRandCount<cbMaxCount);
			ZeroMemory(cbCardDataTemp,sizeof(cbCardDataTemp));
			CopyMemory(cbCardDataTemp, cardDatas, sizeof(cardDatas));
		}
	}
	//BYTE cbRandTemp = g_RandKiss.RandKiss() % 100;
	//if (cbRandTemp < 5)
	//{
	//	// ʲôҲ����
	//}
	//else
	//{
	//	BYTE cbPiTemp = 0xFF;
	//	BYTE cbBaoTemp = 0xFF;
	//	WORD wSiceTemp = WORD(cbSiceCount & 0xffff);
	//	BYTE cbSiceCountPiTemp = HIBYTE(wSiceTemp) + LOBYTE(wSiceTemp);
	//	cbPiTemp = cardDatas[cbSiceCountPiTemp * 2 - 2]; // Ƥ
	//	cbBaoTemp = cbPiTemp + 1; // Ƥ��һΪ��
	//	BYTE cbIndexTemp = cbSiceCountPiTemp * 2 - 1;
	//	if (cbPiTemp == 0x09)
	//	{
	//		cbBaoTemp = 0x01;
	//	}
	//	else if (cbPiTemp == 0x19)
	//	{
	//		cbBaoTemp = 0x11;
	//	}
	//	else if (cbPiTemp == 0x29)
	//	{
	//		cbBaoTemp = 0x21;
	//	}
	//	else if (cbPiTemp == 0x37)
	//	{
	//		cbBaoTemp = 0x35;
	//	}
	//	if (cbRandTemp < 50)
	//	{
	//		for (BYTE i = 119; i >= 0; i--)
	//		{
	//			if (cardDatas[i] == cbBaoTemp)
	//			{
	//				BYTE cbCardValueTemp = cardDatas[i];
	//				cardDatas[i] = cardDatas[cbIndexTemp];
	//				cardDatas[cbIndexTemp] = cbCardValueTemp;
	//				break;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		BYTE cbTemp = 0;
	//		for (BYTE i = 119; i >= 0; i--)
	//		{
	//			if (cardDatas[i] == cbBaoTemp)
	//			{
	//				BYTE cbCardValueTemp = cardDatas[i];
	//				cardDatas[i] = cardDatas[cbIndexTemp];
	//				cardDatas[cbIndexTemp] = cbCardValueTemp;
	//				cbIndexTemp++;
	//				cbTemp++;
	//				if (cbTemp >= 2) break;
	//			}
	//		}
	//	}
	//}
	CopyMemory(cbCardData, cardDatas, sizeof(cardDatas));

	return cbMaxCount;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���˿�
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���˿�
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=14);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//�ɹ��ж�
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

bool CGameLogic::AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount)
{
	//����˿�
	for (BYTE i=0;i<cbAddCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbAddCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbAddCard[i])]<4);

		//ɾ���˿�
		BYTE cbAddIndex=SwitchToCardIndex(cbAddCard[i]);
		if (cbCardIndex[cbAddIndex]<4)
		{
			//ɾ���˿�
			++cbCardIndex[cbAddIndex];
		}
		else 
		{
			//�������
			ASSERT(FALSE);
			//��ԭ���
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbAddCard[j]));
				cbCardIndex[SwitchToCardIndex(cbAddCard[j])]--;
			}
			return false;
		}
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(DWORD cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard-2;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(DWORD cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&WIK_GANG) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }


	return 0;
}

//���Ƶȼ�
WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;

// 	if( !(ChiHuRight&CHR_PIN_HU).IsEmpty() )
// 		wFanShu = 0;															 //ƽ��		0��
// 	if( !(ChiHuRight&CHR_DA_DUI_ZI).IsEmpty() )									 //�����	1��
// 		wFanShu = 1;
// 																				 //��һɫ	2��
// 	if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() )								 //�߶�		2��
// 		wFanShu = 2;															 //���߶�	3��
// 	if( !(ChiHuRight&CHR_QI_XIAO_DUI).IsEmpty() )								 //���		3��
// 		wFanShu = 2;
// 																				 //ȫ�۾�	3��
// 	if( !(ChiHuRight&CHR_LONG_QI_DUI).IsEmpty() )								 //����		3��
// 		wFanShu = 3;
// 	if( !(ChiHuRight&CHR_QING_DUI).IsEmpty() )									 //���߶�	4��
// 		wFanShu = 3;															 //�����߶�	5��
// 	if( !(ChiHuRight&CHR_DAI_YAO).IsEmpty() )									 //�����߶�	5��
// 		wFanShu = 3;															 //���		5��
// 	if( !(ChiHuRight&CHR_JIANG_DUI).IsEmpty() )									 //�غ�		5��
// 		wFanShu = 3;															 //���۾�	5��
// 	if( !(ChiHuRight&CHR_QING_QI_DUI).IsEmpty() )
// 		wFanShu = 4;															 //���ܺ�		1��
// 	if( !(ChiHuRight&CHR_QING_LONG_QI_DUI).IsEmpty() )							 //������		1��
// 		wFanShu = 5;															 //���ϻ�		1��
// 	if( !(ChiHuRight&CHR_JIANG_QI_DUI).IsEmpty() )								 //�𹳵�		1��
// 		wFanShu = 5;															 //������		1��
// 	if( !(ChiHuRight&CHR_TIAN_HU).IsEmpty() )									 //������		1��
// 		wFanShu = 5;
// 	if( !(ChiHuRight&CHR_DI_HU).IsEmpty() )
// 		wFanShu = 5;
// 	if( !(ChiHuRight&CHR_QING_YAO_JIU).IsEmpty() )
// 		wFanShu = 5;
// 
// 	//���ܺ�
// 	if( !(ChiHuRight&CHR_QIANG_GANG).IsEmpty() )
// 		wFanShu += 1;
// 	//������
// 	if( !(ChiHuRight&CHR_GANG_SHANG_PAO).IsEmpty() )
// 		wFanShu += 1;
// 	//���ϻ�
// 	if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() )
// 		wFanShu += 1;
// 	//��ܻ�
// 	if( !(ChiHuRight&CHR_DIANGANGHUA).IsEmpty())
// 		wFanShu += 1;
// 	//�𹳵�
// 	if( !(ChiHuRight&CHR_JIN_GOU_DAO).IsEmpty() )
// 		wFanShu += 1;
// 	//������
// 	if( !(ChiHuRight&CHR_HAI_DI_HU).IsEmpty() )
// 		wFanShu += 1;
	return wFanShu;
}
//���к��Ƶȼ�
WORD CGameLogic::GetChaHuActionRank(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;

// 	if( !(ChiHuRight&CHR_PIN_HU).IsEmpty() )
// 		wFanShu = 0;															 //ƽ��		0��
// 	if( !(ChiHuRight&CHR_DA_DUI_ZI).IsEmpty() )									 //�����	1��
// 		wFanShu = 1;
// 																				 //��һɫ	2��
// 	if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() )								 //�߶�		2��
// 		wFanShu = 2;															 //���߶�	3��
// 	if( !(ChiHuRight&CHR_QI_XIAO_DUI).IsEmpty() )								 //���		3��
// 		wFanShu = 2;
// 																				 //ȫ�۾�	3��
// 	if( !(ChiHuRight&CHR_LONG_QI_DUI).IsEmpty() )								 //����		3��
// 		wFanShu = 3;
// 	if( !(ChiHuRight&CHR_QING_DUI).IsEmpty() )									 //���߶�	4��
// 		wFanShu = 3;															 //�����߶�	5��
// 	if( !(ChiHuRight&CHR_DAI_YAO).IsEmpty() )									 //�����߶�	5��
// 		wFanShu = 3;															 //���		5��
// 	if( !(ChiHuRight&CHR_JIANG_DUI).IsEmpty() )									 //�غ�		5��
// 		wFanShu = 3;															 //���۾�	5��
// 	if( !(ChiHuRight&CHR_QING_QI_DUI).IsEmpty() )
// 		wFanShu = 4;															 //���ܺ�		1��
// 	if( !(ChiHuRight&CHR_QING_LONG_QI_DUI).IsEmpty() )							 //������		1��
// 		wFanShu = 5;															 //���ϻ�		1��
// 	if( !(ChiHuRight&CHR_JIANG_QI_DUI).IsEmpty() )								 //�𹳵�		1��
// 		wFanShu = 5;															 //������		1��
// 	if( !(ChiHuRight&CHR_TIAN_HU).IsEmpty() )									 //������		1��
// 		wFanShu = 5;
// 	if( !(ChiHuRight&CHR_DI_HU).IsEmpty() )
// 		wFanShu = 5;
// 	if( !(ChiHuRight&CHR_QING_YAO_JIU).IsEmpty() )
// 		wFanShu = 5;
// 
// 	//���ܺ�
// 	if( !(ChiHuRight&CHR_QIANG_GANG).IsEmpty() )
// 		wFanShu += 1;
// 	//������
// 	if( !(ChiHuRight&CHR_GANG_SHANG_PAO).IsEmpty() )
// 		wFanShu += 1;
// 	//���ϻ�
// 	if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() )
// 		wFanShu += 1;
// 	//��ܻ�
// 	if( !(ChiHuRight&CHR_DIANGANGHUA).IsEmpty())
// 		wFanShu += 1;
// 	//�𹳵�
// 	if( !(ChiHuRight&CHR_JIN_GOU_DAO).IsEmpty() )
// 		wFanShu += 1;
	return wFanShu;
}


WORD CGameLogic::GetChiHuActionRank_YiChang(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;

	return wFanShu;
}

//�����ж�
DWORD CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
	{
		return WIK_NULL;
	}
		
	//��������
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//�����ж�
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);

	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//���Ʋ��ܰ���������
			if( m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

			if(m_cbPiIndex >= cbFirstIndex && m_cbPiIndex <= cbFirstIndex+2 ) continue;
			if(m_cbLaiPiIndex >= cbFirstIndex && m_cbLaiPiIndex <= cbFirstIndex+2 ) continue;

			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
				continue;

			//��������
			cbEatKind|=cbItemKind[i];
		}
	}
	return cbEatKind;
}

//�����ж�
DWORD CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if ( IsMagicCard(cbCurrentCard) ) 
		return WIK_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//�����ж�
DWORD CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if ( IsMagicCard(cbCurrentCard) ) 
		return WIK_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

DWORD CGameLogic::AnalyseGangCard_HasHu(bool bIsYiChang,const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult,const std::vector<BYTE>& cbHuCardList)
{
	BYTE cbCurrentCard = GangCardResult.cbCardData[0];
	DWORD cbActionTemp =  AnalyseGangCard(cbCardIndex,WeaveItem,cbWeaveCount,GangCardResult);
	if (cbActionTemp == WIK_GANG)
	{
		BYTE cbCardIndexTemp[MAX_INDEX] ={0};
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		BYTE cbGangCardIndex = SwitchToCardIndex(cbCurrentCard);

		bool bPublic = true;
		if (cbCardIndexTemp[cbGangCardIndex] == 4)
		{
			bPublic = false;
		}

		cbCardIndexTemp[cbGangCardIndex] = 0;

		tagWeaveItem tagWeaveItemTemp[MAX_WEAVE]={0};
		ZeroMemory(tagWeaveItemTemp,sizeof(tagWeaveItemTemp));
		CopyMemory(tagWeaveItemTemp,WeaveItem,sizeof(tagWeaveItemTemp));

		int cbWeaveCountTemp = cbWeaveCount;
		if (bPublic == false)
		{
			WORD wIndex= cbWeaveCount;
			tagWeaveItemTemp[wIndex].cbPublicCard=bPublic;
			tagWeaveItemTemp[wIndex].cbCenterCard=cbCurrentCard;
			tagWeaveItemTemp[wIndex].cbWeaveKind=WIK_GANG;
			tagWeaveItemTemp[wIndex].wProvideUser=INVALID_CHAIR;
			//tagWeaveItemTemp[wIndex].cbIsSelfGang = 0;
			cbWeaveCountTemp = cbWeaveCount +1;
		}

		DWORD cbChiHu = WIK_NULL;
		
		CChiHuRight chr;
		for (int i=0;i<(int)cbHuCardList.size();i++)
		{
			if (bIsYiChang)
			{
				cbChiHu = AnalyseChiHuCard_YiChang(cbCardIndexTemp,tagWeaveItemTemp,cbWeaveCountTemp,cbHuCardList[i],chr,true);
			}
			else
			{
				cbChiHu = AnalyseChiHuCard(cbCardIndexTemp,tagWeaveItemTemp,cbWeaveCountTemp,cbHuCardList[i],chr);
			}
			if(cbChiHu == WIK_NULL)
			{
				break;
			}
		}
		if (cbChiHu == WIK_NULL)
		{
			cbActionTemp = WIK_NULL;
		}
	}
	return cbActionTemp;
}

//ϲ�ܷ���
DWORD CGameLogic::AnalyseXiGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{

	DWORD cbActionMask=WIK_NULL;
	
	return cbActionMask;
}
DWORD CGameLogic::AnalyseLiangXiCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{

	//���ñ���
	DWORD cbActionMask=WIK_NULL;

	return cbActionMask;

}
//���Ʒ���
DWORD CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	BYTE cbSendCard = GangCardResult.cbCardData[0];

	//���ñ���
	DWORD cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//���ϸ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex ) continue;
		if( i == m_cbPiIndex ) 
		{
			continue;
			if (cbCardIndex[i] > 0)
			{
				cbActionMask|=WIK_GANG_PI;
			}
		}
		if( i == m_cbLaiPiIndex ) 
		{
			continue;
			if (cbCardIndex[i] > 0)
			{
				cbActionMask|=WIK_GANG_LAIPI;
			}
		}
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
		if(i == m_iBaoIndex )
		{
			cbActionMask|= WIK_SHOW_BAO;
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			BYTE cbCardIdex = SwitchToCardIndex(WeaveItem[i].cbCenterCard);
			if (cbCardIndex[cbCardIdex]==1 
				/*&& cbSendCard !=0 && cbSendCard == WeaveItem[i].cbCenterCard */)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

//�Ժ�����
DWORD CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bHasYao9JiangDui)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCardһ����Ϊ0			!!!!!!!!!
	ASSERT( cbCurrentCard != 0 );
	if( cbCurrentCard == 0 ) return WIK_NULL;

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;


	//��С����
	if( IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) ) 
	{
		ChiHuRight |= CHR_QI_DUI;
		// 		if(isJiangQiDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
		// 			ChiHuRight = CHR_JIANG_QI_DUI;
	}

	if( !ChiHuRight.IsEmpty() )
		cbChiHuKind = WIK_CHI_HU;


	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//
		cbChiHuKind = WIK_CHI_HU;

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

// 			//������
// 			if( IsPengPeng(pAnalyseItem) ) 
// 				ChiHuRight |= CHR_DA_DUI_ZI;
// 
// 			//����
// 			if( IsDaiYao(pAnalyseItem) && bHasYao9JiangDui )
// 				ChiHuRight |= CHR_DAI_YAO;


// 			//����
// 			if( IsJiangDui(pAnalyseItem))
// 				ChiHuRight |= 0x00000020;						
		}
	}


	return cbChiHuKind;
}

DWORD CGameLogic::AnalyseChiHuCard_YiChang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCardһ����Ϊ0			!!!!!!!!!
	ASSERT( cbCurrentCard != 0 );
	if( cbCurrentCard == 0 ) return WIK_NULL;


	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//����ȱһ��
	if( IsHuaZhu(cbCardIndexTemp,WeaveItem,cbWeaveCount) ) return WIK_NULL;

	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//
		cbChiHuKind = WIK_CHI_HU;

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			/*
			//	�жϷ���
			*/
			//������
			if( IsPengPeng(pAnalyseItem) ) 
				ChiHuRight |= CHR_PENGPENGHU;
		}
	}


	//����
	if (IsMenQing(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard)&& bSelfSend)
	{
		ChiHuRight |= CHR_MEN_QING;
	}
	return cbChiHuKind;
}

//�����齫���Ʒ���
DWORD CGameLogic::AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
										  bool bHasBigHu, bool  bHasSmallHu, int iBaoNum, BYTE ucBaoHou)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	int nTempIndex = SwitchToCardIndex(cbCurrentCard);

	bool  IsDahu = false ;
	if(bHasBigHu )
	{
		DWORD dwRight = IsQiXiaoDui(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard);
		if(dwRight) 
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(iBaoNum == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			int nColor = -1;
			if( IsQingYiSe(cbCardIndex, WeaveItem, cbWeaveCount, cbCurrentCard, &nColor))
			{
				ChiHuRight |= CHR_QING_YI_SE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= dwRight;
			ChiHuRight |= CHR_DA_HU;	
			ChiHuRight |= CHR_QI_DUI;	
			IsDahu = true;
			return cbChiHuKind;
		}
		if (cbCurrentCard > 0 && !ucBaoHou)
		{
			cbCardIndexTemp[nTempIndex] ++;
		}
		//�����˿�
		AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false,4);

		if (AnalyseItemArray.GetCount()<= 0)
		{
			return cbChiHuKind;
		}
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//������
			if( IsPengPeng(pAnalyseItem) ) 	
			{
				if(iBaoNum == 0)
				{
					ChiHuRight |= CHR_NONE;
				}
				cbChiHuKind = WIK_CHI_HU;
				ChiHuRight |= CHR_PENGPENGHU;
				ChiHuRight |= CHR_DA_HU;	
				// return cbChiHuKind;
			}						
		}
		int nColor = -1;
		if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
		{
			if(iBaoNum == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_QING_YI_SE;
			ChiHuRight |= CHR_DA_HU;	
			IsDahu = true; 
			return cbChiHuKind;
		}
	}

	if(IsDahu == false  && bHasSmallHu == true)
	{
		CAnalyseItemArray AnalyseItemArray1;
		AnalyseItemArray1.RemoveAll();
		if (cbCurrentCard > 0 && !bHasBigHu && !ucBaoHou)
		{
			cbCardIndexTemp[nTempIndex] ++;
		}
		//�����˿�
		AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,false,4);
		//���Ʒ���
		if (AnalyseItemArray1.GetCount() > 0)
		{
			cbChiHuKind = WIK_CHI_HU;
		}
		if(iBaoNum == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		else
		{
			int nOldMagicCard = m_cbMagicIndex;
			m_cbMagicIndex = MAX_INDEX;
			AnalyseItemArray1.RemoveAll();
			AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,false,4);
			if (AnalyseItemArray1.GetCount() > 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			m_cbMagicIndex = nOldMagicCard;
		}
	}
	return cbChiHuKind;
}

DWORD CGameLogic::AnalyseChiHuCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
										  bool bSelfSend,bool bHasQiDui,bool bGangkai,bool canQFJ, BYTE ucFlag, BYTE QFJ)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//��Ƥ����Ƥ���ܺ�
	if(cbCardIndexTemp[m_cbLaiPiIndex] > 0 || cbCardIndexTemp[m_cbPiIndex] > 0)
	{
		return false;
	}
	int nTempIndex = SwitchToCardIndex(cbCurrentCard);
	//��Ƥ����Ƥ���ܺ�
	if(nTempIndex == m_cbLaiPiIndex || nTempIndex == m_cbPiIndex)
	{
		return false;
	}
	if (ucFlag == 2) // Ϫˮ�����岻����2�������ϵ����
	{
		if (nTempIndex == m_cbMagicIndex && cbCardIndexTemp[m_cbMagicIndex] >= 1)
		{
			return false;
		}
	}
	bool isDahu = false;

	//��С����
	if (bHasQiDui)
	{
		DWORD dwRight =  IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard);
		if(dwRight) 
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			int nColor = -1;
			if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
			{
				ChiHuRight |= CHR_QING_YI_SE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= dwRight;
			ChiHuRight |= CHR_DA_HU;
			ChiHuRight |= CHR_QI_DUI;	
			isDahu = true;
			return cbChiHuKind;
		}
	}
	if (canQFJ)
	{
		//��һɫ
		if( IsJiangHu(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
			{
				if (IsJiangByIndex(m_cbMagicIndex))
				{
					ChiHuRight |= CHR_DOUBLE_HUN;
				}				
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_JIANG_YI_SE;	
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
			return cbChiHuKind;
		}
		//��һɫ
		if( IsFengYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
			{
				if (m_cbMagicIndex >= 27)
				{
					ChiHuRight |= CHR_DOUBLE_HUN;
				}	
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_FENG_YI_SE;	
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
			return cbChiHuKind;
		}
	}
	
	if (cbCurrentCard > 0 && !QFJ) // ���Ǵӱ���罫�������ж�
	{
		cbCardIndexTemp[nTempIndex] ++;
	}
	int maxHun = 1;
	if (bGangkai && ucFlag == 1)
	{
		// ��������
	}
	else if (bGangkai)
	{
		maxHun = 4;
	}
	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false, maxHun);

	if (AnalyseItemArray.GetCount()<= 0)
	{
		return cbChiHuKind;
	}
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//��������
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//������
		if( IsPengPeng(pAnalyseItem) ) 	
		{
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_PENGPENGHU;
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
		}						
	}

	int nColor = -1;
	if (canQFJ)
	{
		if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
		{
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
			{
	// 			int cbCardColor=(SwitchToCardData(m_cbMagicIndex)&MASK_COLOR);
	// 			if (cbCardColor == nColor)
	// 			{
	// 				ChiHuRight |= CHR_DOUBLE_HUN;
	// 			}	
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_QING_YI_SE;
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
			return cbChiHuKind;
		}	
	}
	if (!isDahu)
	{
		CAnalyseItemArray AnalyseItemArray1;
		AnalyseItemArray1.RemoveAll();
		//�����˿�
		AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);

		//���Ʒ���
		if (AnalyseItemArray1.GetCount()>0)
		{
			cbChiHuKind = WIK_CHI_HU;	
		}
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		else
		{
			int nOldMagicCard = m_cbMagicIndex;
			m_cbMagicIndex = MAX_INDEX;
			AnalyseItemArray1.RemoveAll();
			AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);
			if (AnalyseItemArray1.GetCount()>0)
			{
				ChiHuRight |= CHR_NONE;
			}
			m_cbMagicIndex = nOldMagicCard;
		}
	}
	
	return cbChiHuKind;
}

//��÷�齫���Ʒ���
DWORD CGameLogic::AnalyseChiHuCard_HM(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
									  BYTE cbPeng, BYTE cbZiMoDianPao)
{
	//��������
	BYTE cbChiHuKind = WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	int nTempIndex = SwitchToCardIndex(cbCurrentCard);

	bool bDaHu = false;
	//��С����
	DWORD dwRight = IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard);
	if(dwRight) 
	{
		if (cbCurrentCard > 0)
		{
			cbCardIndexTemp[nTempIndex] ++;
		}
		if(m_cbMagicIndex == MAX_INDEX || cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		int nColor = -1;
		if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
		{
			ChiHuRight |= CHR_QING_YI_SE;
		}
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= dwRight;
		ChiHuRight |= CHR_DA_HU;
		ChiHuRight |= CHR_QI_DUI;
		return cbChiHuKind;
	}

	if (cbCurrentCard > 0)
	{
		cbCardIndexTemp[nTempIndex] ++;
	}
	if(m_cbMagicIndex == MAX_INDEX || cbCardIndexTemp[m_cbMagicIndex] == 0)
	{
		ChiHuRight |= CHR_NONE;
	}
	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false, 4);

	if (AnalyseItemArray.GetCount()<= 0)
	{
		return cbChiHuKind;
	}
	cbChiHuKind = WIK_CHI_HU;
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//��������
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//������
		if( IsPengPeng(pAnalyseItem) ) 	
		{
			ChiHuRight |= CHR_PENGPENGHU;
			bDaHu = true;
		}						
	}
	int nColor = -1;
	if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
	{
		ChiHuRight |= CHR_QING_YI_SE;
		bDaHu = true;
	}
	if (bDaHu)
	{
		ChiHuRight |= CHR_DA_HU;
		if(m_cbMagicIndex == MAX_INDEX || cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
	}
	else // С��
	{
		if (cbCardIndexTemp[m_cbMagicIndex] > 0 && !cbZiMoDianPao)
		{
			int nOldMagicCard = m_cbMagicIndex;
			m_cbMagicIndex = MAX_INDEX;
			AnalyseItemArray.RemoveAll();
			AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false, 4);
			if (AnalyseItemArray.GetCount()>0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
				cbChiHuKind = WIK_NULL;
			m_cbMagicIndex = nOldMagicCard;
		}
		if (cbChiHuKind != WIK_NULL && cbPeng)//С�������Ʋ��ܺ����Ǻ��Ʒ������з��ƻ�����ţ������˺��аװ壬��3�����аװ�����
		{
			if (cbCardIndexTemp[32] > 0);
			else if (cbCardIndexTemp[31] >= 3 || cbCardIndexTemp[33] >= 3);
			else if (CheckHZBB(WeaveItem, cbWeaveCount));
			else if (CheckDanZhang(cbCardIndexTemp, cbCurrentCard));
			else cbChiHuKind = WIK_NULL;
		}
		
		if (cbChiHuKind != WIK_NULL && (m_cbMagicIndex == MAX_INDEX || cbCardIndexTemp[m_cbMagicIndex] == 0)) ChiHuRight |= CHR_NONE;
	}
	return cbChiHuKind;
}

//Ӣɽ�齫���Ʒ���
DWORD CGameLogic::AnalyseChiHuCard_YS(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
		bool bGangkai,bool canQFJ,bool bZiMo,BYTE cbTalkAllOther)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	int nTempIndex = SwitchToCardIndex(cbCurrentCard);
	//��Ƥ����Ƥ���ܺ�
	if(cbCardIndexTemp[m_cbLaiPiIndex] > 0 || cbCardIndexTemp[m_cbPiIndex] > 0)
	{
		return false;
	}
	if(nTempIndex == m_cbLaiPiIndex || nTempIndex == m_cbPiIndex)
	{
		return false;
	}
	//����ȫ���˲���������
	if (cbTalkAllOther && bZiMo) return false;
	bool isDahu = false;
	if (canQFJ)
	{
		//��һɫ
		if( IsJiangHu(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
			{
				if (IsJiangByIndex(m_cbMagicIndex))
				{
					ChiHuRight |= CHR_DOUBLE_HUN;
				}				
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_JIANG_YI_SE;	
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
			return cbChiHuKind;
		}
		//��һɫ
		if( IsFengYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			else
			{
				if (m_cbMagicIndex >= 27)
				{
					ChiHuRight |= CHR_DOUBLE_HUN;
				}	
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_FENG_YI_SE;	
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
			return cbChiHuKind;
		}
	}
	
	if (cbCurrentCard > 0)
	{
		cbCardIndexTemp[nTempIndex] ++;
	}
	int maxHun = 1;
	if (bGangkai)  maxHun = 4;
	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false, maxHun);

	if (AnalyseItemArray.GetCount()<= 0)
	{
		return cbChiHuKind;
	}
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//��������
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//������
		if( IsPengPeng(pAnalyseItem) ) 	
		{
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_PENGPENGHU;
			ChiHuRight |= CHR_DA_HU;	
			isDahu = true;
		}						
	}

	int nColor = -1;

	if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
	{
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= CHR_QING_YI_SE;
		ChiHuRight |= CHR_DA_HU;
		isDahu = true;
		return cbChiHuKind;
	}	

	if (!isDahu)
	{
		CAnalyseItemArray AnalyseItemArray1;
		AnalyseItemArray1.RemoveAll();
		//�����˿�
		AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);

		//���Ʒ���
		if (AnalyseItemArray1.GetCount()>0)
		{
			cbChiHuKind = WIK_CHI_HU;	
		}
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		else
		{
			int nOldMagicCard = m_cbMagicIndex;
			m_cbMagicIndex = MAX_INDEX;
			AnalyseItemArray1.RemoveAll();
			AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);
			if (AnalyseItemArray1.GetCount()>0)
			{
				ChiHuRight |= CHR_NONE;
			}
			m_cbMagicIndex = nOldMagicCard;
		}
	}
	
	return cbChiHuKind;
}

//����齫���Ʒ���
DWORD CGameLogic::AnalyseChiHuCard_MC(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,
									  bool bHasQiDui,bool bGangkai)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	int nTempIndex = SwitchToCardIndex(cbCurrentCard);
	//��Ƥ����Ƥ���ܺ�
	if(cbCardIndexTemp[m_cbLaiPiIndex] > 0 || cbCardIndexTemp[m_cbPiIndex] > 0)
	{
		return false;
	}
	if(nTempIndex == m_cbLaiPiIndex || nTempIndex == m_cbPiIndex)
	{
		return false;
	}
	//��С����
	if (bHasQiDui)
	{
		DWORD dwRight =  IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard);
		if(dwRight) 
		{
			if (cbCurrentCard > 0)
			{
				cbCardIndexTemp[nTempIndex] ++;
			}
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			int nColor = -1;
			if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
			{
				ChiHuRight |= CHR_QING_YI_SE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= dwRight;
			ChiHuRight |= CHR_DA_HU;
			ChiHuRight |= CHR_QI_DUI;	
			return cbChiHuKind;
		}
	}
	bool isDahu = false;
	//��һɫ
	if( IsJiangHu(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
	{
		if (cbCurrentCard > 0)
		{
			cbCardIndexTemp[nTempIndex]++;
		}
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= CHR_JIANG_YI_SE;	
		ChiHuRight |= CHR_DA_HU;	
		return cbChiHuKind;
	}
	//��һɫ
	if( IsFengYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
	{
		if (cbCurrentCard > 0)
		{
			cbCardIndexTemp[nTempIndex] ++;
		}
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= CHR_FENG_YI_SE;
		ChiHuRight |= CHR_DA_HU;
		return cbChiHuKind;
	}
	if (cbCurrentCard > 0)
	{
		cbCardIndexTemp[nTempIndex] ++;
	}
	int maxHun = 1;
	if (bGangkai)  maxHun = 4;
	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray,false, maxHun);

	if (AnalyseItemArray.GetCount()<= 0)
	{
		return cbChiHuKind;
	}
	for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
	{
		//��������
		tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

		//������
		if( IsPengPeng(pAnalyseItem) ) 	
		{
			if(cbCardIndexTemp[m_cbMagicIndex] == 0)
			{
				ChiHuRight |= CHR_NONE;
			}
			cbChiHuKind = WIK_CHI_HU;
			ChiHuRight |= CHR_PENGPENGHU;
			ChiHuRight |= CHR_DA_HU;
			isDahu = true;
		}						
	}
	int nColor = -1;
	if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,&nColor))
	{
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= CHR_QING_YI_SE;
		ChiHuRight |= CHR_DA_HU;
		isDahu = true;
		return cbChiHuKind;
	}
	if (!isDahu)
	{
		CAnalyseItemArray AnalyseItemArray1;
		AnalyseItemArray1.RemoveAll();
		//�����˿�
		AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);

		//���Ʒ���
		if (AnalyseItemArray1.GetCount()>0)
		{
			cbChiHuKind = WIK_CHI_HU;
		}
		if(cbCardIndexTemp[m_cbMagicIndex] == 0)
		{
			ChiHuRight |= CHR_NONE;
		}
		else
		{
			int nOldMagicCard = m_cbMagicIndex;
			m_cbMagicIndex = MAX_INDEX;
			AnalyseItemArray1.RemoveAll();
			AnalyseCardPingHu(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray1,maxHun);
			if (AnalyseItemArray1.GetCount()>0)
			{
				ChiHuRight |= CHR_NONE;
			}
			m_cbMagicIndex = nOldMagicCard;
		}
	}
	
	return cbChiHuKind;
}


DWORD CGameLogic::AnalyseChiHuTAG_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight,bool bSelfSend,bool bHasQiDui)
{
	//��������
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCardһ����Ϊ0			!!!!!!!!!
	ASSERT( cbCurrentCard != 0 );
	if( cbCurrentCard == 0 ) return WIK_NULL;


	//�ж��Ƿ���У��Ƿ���4������
	/*if(m_cbMagicIndex != MAX_INDEX)
	{
		if(cbCardIndex[m_cbMagicIndex] == 4)
			return WIK_CHI_HU;
	}*/

	/*
	//	���ⷬ��
	*/
	//��С����
	if( IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) && bHasQiDui) 
	{
		cbChiHuKind = WIK_CHI_HU;
		ChiHuRight |= CHR_QI_DUI;
	}

	//�����˿�
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	//�����˿�
	//AnalyseTAG(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount() > 0)
	{
		//
		cbChiHuKind = WIK_CHI_HU;
	}
	return cbChiHuKind;
}

DWORD CGameLogic::AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	//��������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		for( BYTE i = 0; i < MAX_INDEX-ZI_PAI_COUNT; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					return WIK_LISTEN;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		for( BYTE j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
				return WIK_LISTEN;
		}
	}

	return WIK_NULL;
}

std::vector<BYTE> CGameLogic::GetTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool bWufeng)
{
	std::vector<BYTE> vec;

	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;

		for( BYTE i = 0; i < count; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < count; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false,false) )
					vec.push_back(cbCurrentCard);
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;
		for( BYTE j = 0; j < count; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr,false, false,false) )
				vec.push_back(cbCurrentCard);
		}
	}
	return vec;
}

std::vector<BYTE> CGameLogic::GetTing_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool bWufeng)
{
	std::vector<BYTE> vec;

	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;

		for( BYTE i = 0; i < count; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < count; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false,false) )
				{
					vec.push_back(cbCurrentCard);
					return vec;
				}
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;
		for( BYTE j = 0; j < count; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr,false, false,false) )
			{
				vec.push_back(cbCurrentCard);
				return vec;
			}
		}
	}
	return vec;
}

bool CGameLogic::GetTingTag_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount,bool bWufeng,BYTE bGameTypeIndex,BYTE bGameTypeQing)
{
	if (bGameTypeIndex == 4)
	{
		//��Ƥ����Ƥ��������
		if(cbCardIndex[m_cbLaiPiIndex] > 0 || cbCardIndex[m_cbPiIndex] > 0)
		{
			return false;
		}
		else if (bGameTypeQing == 1 && cbCardIndex[m_cbMagicIndex] > 1) // ������
		{
			return false;
		}
	}

	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;

		for( BYTE i = 0; i < count; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < count; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				/*if( WIK_CHI_HU == AnalyseChiHuTAG_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false) )
				return true;*/

				if (WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false,false,true,2,false)) // �޸�20180911
					return true;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		int count = 0;
		if(bWufeng)
			count = MAX_INDEX - ZI_PAI_COUNT;
		else
			count = MAX_INDEX;
		for( BYTE j = 0; j < count; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			/*if( WIK_CHI_HU == AnalyseChiHuTAG_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr,false, false) )
			return true;*/
			if (WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false,false,true,2,false))// �޸�20180911
				return true;
		}
	}
	return false;
}

DWORD CGameLogic::AnalyseTingCard_DaoDao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		for( BYTE i = 0; i < MAX_INDEX-ZI_PAI_COUNT; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr, false, false,false) )
					return WIK_LISTEN;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		for( BYTE j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard_DaoDao(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr,false, false,false) )
				return WIK_LISTEN;
		}
	}

	return WIK_NULL;
}

//�Ƿ�����
bool CGameLogic::IsTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	//��������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	CChiHuRight chr;
	for( BYTE i = 0; i < MAX_INDEX-7; i++ )
	{
		BYTE cbCurrentCard = SwitchToCardData( i );
		if( WIK_CHI_HU == AnalyseChiHuCard( cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr ) )
			return true;
	}
	return false;
}

bool CGameLogic::IsTingCard_YiChang( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	//��������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	CChiHuRight chr;
	for( BYTE i = 0; i < MAX_INDEX-7; i++ )
	{
		BYTE cbCurrentCard = SwitchToCardData( i );
		if( WIK_CHI_HU == AnalyseChiHuCard_YiChang( cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr,false) )
			return true;
	}
	return false;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	if(cbCardIndex==MAX_INDEX)
	{
		return 128;
	}
	ASSERT(cbCardIndex<34);
	return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
}

BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], std::vector<BYTE>& cbCardData)
{
	//ת���˿�
// 	//����
// 	if( m_cbMagicIndex != MAX_INDEX )
// 	{
// 		for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
// 			cbCardData.push_back(SwitchToCardData(m_cbMagicIndex)) ;
// 	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//if( i == m_cbMagicIndex ) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbCardData.size()<MAX_COUNT);
				cbCardData.push_back(SwitchToCardData(i));
			}
		}
	}

	return cbCardData.size();
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

BYTE CGameLogic::SwitchToCardIndex(const std::vector<BYTE>& cbCardData, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	int cbCardCount = cbCardData.size();
	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���˿�
	BYTE cbPosition=0;
// 	//����
// 	if( m_cbMagicIndex != MAX_INDEX )
// 	{
// 		for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
// 			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
// 	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//if( i == m_cbMagicIndex ) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//�����˿�
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, 
							 CAnalyseItemArray & AnalyseItemArray,bool bCheckJiang, int maxHun)
{
	//������Ŀ
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+7+14+14*3];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	//ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		//ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if(bCheckJiang)
			{
				if(!IsJiangByIndex(i))
				{
					continue;
				}
			}		
			if (cbCardIndex[i]==2 || 
				( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{

				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				int nStart = 0;
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					if (WeaveItem[j].cbWeaveKind < WIK_LISTEN)
					{
						AnalyseItem.cbWeaveKind[nStart]=WeaveItem[j].cbWeaveKind;
						AnalyseItem.cbCenterCard[nStart]=WeaveItem[j].cbCenterCard;
						GetWeaveCard( WeaveItem[j].cbWeaveKind,WeaveItem[j].cbCenterCard,AnalyseItem.cbCardData[nStart] );
						nStart ++;
					}
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}	

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	//CopyMemory(cbMagicCardIndex, cbTempCardIndex, sizeof(cbMagicCardIndex));
	BYTE cbMagicCardCount = 0;
	if( m_cbMagicIndex != MAX_INDEX )
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		if( cbMagicCardIndex[m_cbMagicIndex] ) cbMagicCardIndex[m_cbMagicIndex] = 1;		//��С�������
	}
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbMagicCardIndex[i] == 0) //edit by wld ������һ�Ŷ�û�У����������
			{
				
			}
			else if (cbMagicCardIndex[i]+cbMagicCardCount>=3)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>0?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[1] = cbMagicCardIndex[i]>1?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[2] = cbMagicCardIndex[i]>2?i:m_cbMagicIndex;
				cbKindItemCount++;
				if(cbMagicCardIndex[i]+cbMagicCardCount>=6)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>3?i:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = m_cbMagicIndex;
					cbKindItemCount++;
				}
			}

			//�����ж�
			if ((i<(MAX_INDEX-9))&&((i%9)<7))
			{
				//ֻҪ������������3��˳���������������ڵ���3,��������
				if (cbMagicCardIndex[i] == 0 && cbMagicCardIndex[i + 1] == 0 && cbMagicCardIndex[i + 2] == 0)//edit by wld ������һ�Ŷ�û�У����������
				{
					
				}
				else if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
				{
					BYTE cbIndex[3] = { i==m_cbMagicIndex?0:cbMagicCardIndex[i],(i+1)==m_cbMagicIndex?0:cbMagicCardIndex[i+1],
						(i+2)==m_cbMagicIndex?0:cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
					{
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if( nMagicCountTemp >= 0 )
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
							cbKindItemCount++;
							if(cbMagicCardIndex[i]>0&&cbMagicCardIndex[i+1]>0&&cbMagicCardIndex[i+2]>0&&cbMagicCardCount>0)
							{
								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=m_cbMagicIndex;
								KindItem[cbKindItemCount].cbValidIndex[1]=i+1;
								KindItem[cbKindItemCount].cbValidIndex[2]=i+2;
								cbKindItemCount++;

								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=i;
								KindItem[cbKindItemCount].cbValidIndex[1]=m_cbMagicIndex;
								KindItem[cbKindItemCount].cbValidIndex[2]=i+2;
								cbKindItemCount++;

								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=i;
								KindItem[cbKindItemCount].cbValidIndex[1]=i+1;
								KindItem[cbKindItemCount].cbValidIndex[2]=m_cbMagicIndex;
								cbKindItemCount++;

							}
						}
						else break;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else 
					cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if(bCheckJiang)
					{
						if(!IsJiangByIndex(i))
						{
							continue;
						}
					}					
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						if( i == m_cbMagicIndex ) bMagicEye = true;
						break;
					}
					else if( i!=m_cbMagicIndex && 
						m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
					{
						cbCardEye = SwitchToCardData(i);
						bMagicEye = true;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�������
					int nStart = 0;
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						if (WeaveItem[i].cbWeaveKind < WIK_LISTEN)
						{
							AnalyseItem.cbWeaveKind[nStart]=WeaveItem[i].cbWeaveKind;
							AnalyseItem.cbCenterCard[nStart]=WeaveItem[i].cbCenterCard;
							GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,
								AnalyseItem.cbCardData[nStart] );
							nStart ++;
						}						
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[nStart]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[nStart]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[nStart][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[nStart][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[nStart][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
						nStart ++;
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);

					// return (AnalyseItemArray.GetCount()>0);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=cbLessKindItem-1;
				for (;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

DWORD CGameLogic::AnalyseWave(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
//#define WIK_QING_BAO				0x800								//��һɫ������
//#define WIK_FENG_BAO				0x1000								//��һɫ������
//#define WIK_JIANG_BAO				0x2000								//��һɫ������
	DWORD ret = WIK_NULL;
	bool isjiang = true;
	for (int i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_LEFT 
			|| WeaveItem[i].cbWeaveKind == WIK_CENTER 
			|| WeaveItem[i].cbWeaveKind == WIK_RIGHT )
		{
			isjiang = false;
		}
		if (WeaveItem[i].cbWeaveKind >= WIK_GANG_PI )
		{
			continue;
		}
		if (!IsJiangByCardData(WeaveItem[i].cbCenterCard))
		{
			isjiang = false;
		}		
	}
	if (isjiang)
	{
		ret = ret | WIK_JIANG_BAO;
	}
	bool isqing = true;
	BYTE cbCardColor = 0xFF;
	bool isfeng = true;
	//����ж�
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if (WeaveItem[i].cbWeaveKind < WIK_GANG_PI)
		{
			if (SwitchToCardIndex(WeaveItem[i].cbCenterCard) < 27)
			{
				isfeng =  false;
			}
		}
	}
	if (isfeng)
	{
		ret = ret | WIK_FENG_BAO;
	}
	//����ж�
	for (BYTE i = 0; i < cbWeaveCount; i++)
	{
		if (WeaveItem[i].cbWeaveKind < WIK_GANG_PI)
		{
			BYTE cbCenterCard = WeaveItem[i].cbCenterCard;
			if( cbCardColor == 0xFF )
			{
				cbCardColor = cbCenterCard & MASK_COLOR;
			}		
			if (cbCardColor == 0x30)
			{
				isqing = false;
				break;
			}
			int nTempColor = cbCenterCard&MASK_COLOR;
			if (nTempColor != cbCardColor)	
			{
				isqing = false;
				break;
			}
		}
	}
	if (isqing)
	{
		ret = ret |  WIK_QING_BAO;
	}
	
	return ret;
}

//�����˿�
bool CGameLogic::AnalyseCardPingHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray,int nMaxHun)
{
	//������Ŀ
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;
	//��Ƥ����Ƥ���ܺ�
	if(cbCardIndex[m_cbLaiPiIndex] > 0 || cbCardIndex[m_cbPiIndex] > 0)
	{
		return false;
	}
	//�������� ������һ�����ϻ�
	if (m_cbMagicIndex != MAX_INDEX && !IsMenQing(cbCardIndex,WeaveItem,cbWeaveCount,0))
	{
		int nMagicCount = cbCardIndex[m_cbMagicIndex];
		if(nMagicCount > nMaxHun)
		{
			return false;
		}
	}

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+7+14+14*3];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	//ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		//ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if(!IsJiangByIndex(i))
			{
				continue;
			}
			if (cbCardIndex[i]==2 || 
				( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					GetWeaveCard( WeaveItem[j].cbWeaveKind,WeaveItem[j].cbCenterCard,AnalyseItem.cbCardData[j] );
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}	

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	//CopyMemory(cbMagicCardIndex, cbTempCardIndex, sizeof(cbMagicCardIndex));
	BYTE cbMagicCardCount = 0;
	if( m_cbMagicIndex != MAX_INDEX )
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		if( cbMagicCardIndex[m_cbMagicIndex] ) cbMagicCardIndex[m_cbMagicIndex] = 1;		//��С�������
	}
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbMagicCardIndex[i] == 0) //edit by wld ������һ�Ŷ�û�У����������
			{

			}
			else if (cbMagicCardIndex[i]+cbMagicCardCount>=3)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>0?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[1] = cbMagicCardIndex[i]>1?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[2] = cbMagicCardIndex[i]>2?i:m_cbMagicIndex;
				cbKindItemCount++;
				if(cbMagicCardIndex[i]+cbMagicCardCount>=6)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>3?i:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = m_cbMagicIndex;
					cbKindItemCount++;
				}
			}

			//�����ж�
			if ((i<(MAX_INDEX-9))&&((i%9)<7))
			{
				//ֻҪ������������3��˳���������������ڵ���3,��������
				if (cbMagicCardIndex[i] == 0 && cbMagicCardIndex[i + 1] == 0 && cbMagicCardIndex[i + 2] == 0)//edit by wld ������һ�Ŷ�û�У����������
				{

				}
				else if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
				{
					BYTE cbIndex[3] = { i==m_cbMagicIndex?0:cbMagicCardIndex[i],(i+1)==m_cbMagicIndex?0:cbMagicCardIndex[i+1],
						(i+2)==m_cbMagicIndex?0:cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
					{
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if( nMagicCountTemp >= 0 )
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
							cbKindItemCount++;
							if(cbMagicCardIndex[i]>0&&cbMagicCardIndex[i+1]>0&&cbMagicCardIndex[i+2]>0&&cbMagicCardCount>0)
							{
								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=m_cbMagicIndex;
								KindItem[cbKindItemCount].cbValidIndex[1]=i+1;
								KindItem[cbKindItemCount].cbValidIndex[2]=i+2;
								cbKindItemCount++;

								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=i;
								KindItem[cbKindItemCount].cbValidIndex[1]=m_cbMagicIndex;
								KindItem[cbKindItemCount].cbValidIndex[2]=i+2;
								cbKindItemCount++;

								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								KindItem[cbKindItemCount].cbValidIndex[0]=i;
								KindItem[cbKindItemCount].cbValidIndex[1]=i+1;
								KindItem[cbKindItemCount].cbValidIndex[2]=m_cbMagicIndex;
								cbKindItemCount++;

							}
						}
						else break;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else 
					cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if(!IsJiangByIndex(i))
					{
						continue;
					}
					if (cbCardIndexTemp[i]==2)
					{								
						cbCardEye=SwitchToCardData(i);						
						if( i == m_cbMagicIndex ) bMagicEye = true;
						break;
					}
					else if( i!=m_cbMagicIndex && 
						m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
					{
						cbCardEye = SwitchToCardData(i);
						bMagicEye = true;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�������
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,
							AnalyseItem.cbCardData[i] );
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);

					return (AnalyseItemArray.GetCount()>0);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=cbLessKindItem-1;
				for (;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//�����˿�
bool CGameLogic::AnalyseTAG(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//������Ŀ
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+7+14];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2 || 
				( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					GetWeaveCard( WeaveItem[j].cbWeaveKind,WeaveItem[j].cbCenterCard,AnalyseItem.cbCardData[j] );
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	BYTE cbMagicCardCount = 0;
	if( m_cbMagicIndex != MAX_INDEX )
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		if( cbMagicCardIndex[m_cbMagicIndex] ) cbMagicCardIndex[m_cbMagicIndex] = 1;		//��С�������
	}
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbMagicCardIndex[i]+cbMagicCardCount>=3)
			{
				ASSERT( cbKindItemCount < CountArray(KindItem) );
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
				KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
				KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>0?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[1] = cbMagicCardIndex[i]>1?i:m_cbMagicIndex;
				KindItem[cbKindItemCount].cbValidIndex[2] = cbMagicCardIndex[i]>2?i:m_cbMagicIndex;
				cbKindItemCount++;
				if(cbMagicCardIndex[i]+cbMagicCardCount>=6)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>3?i:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = m_cbMagicIndex;
					cbKindItemCount++;
				}
			}

			//�����ж�
			if ((i<(MAX_INDEX-9))&&((i%9)<7))
			{
				//ֻҪ������������3��˳���������������ڵ���3,��������
				if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
				{
					BYTE cbIndex[3] = { i==m_cbMagicIndex?0:cbMagicCardIndex[i],(i+1)==m_cbMagicIndex?0:cbMagicCardIndex[i+1],
						(i+2)==m_cbMagicIndex?0:cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
					{
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if( nMagicCountTemp >= 0 )
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbCardIndex[0]=i;
							KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
							KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
							cbKindItemCount++;
						}
						else break;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else 
					cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						if( i == m_cbMagicIndex ) bMagicEye = true;
						break;
					}
					else if( i!=m_cbMagicIndex && 
						m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
					{
						cbCardEye = SwitchToCardData(i);
						bMagicEye = true;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�������
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,
							AnalyseItem.cbCardData[i] );
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);
					return true;
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=cbLessKindItem-1;
				for (;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//����
bool CGameLogic::IsMagicCard( BYTE cbCardData )
{
	if( m_cbMagicIndex != MAX_INDEX )
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//����,������ֵ����
bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
{
	//��Ŀ����
	if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if (cbCardData[i]>cbCardData[i+1])
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

/*
// ������������
*/

//�����
bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{

		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			return false;
	}
	return true;
}

bool CGameLogic::IsFengYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	BYTE cbCardDataTemp[MAX_INDEX];
	CopyMemory(cbCardDataTemp,cbCardIndex,MAX_INDEX);
	if (cbCurrentCard != -1)
	{
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardDataTemp[cbCurrentIndex]++;
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i==m_cbMagicIndex) continue;
		if (cbCardDataTemp[i] > 0 && i < 27)
		{
			return false;
		}
	}	
	//����ж�
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if (WeaveItem[i].cbWeaveKind < WIK_GANG_PI)
		{
			if (SwitchToCardIndex(WeaveItem[i].cbCenterCard)  < 27)
			{
				return false;
			}
		}
	}
	return true;
}

//��һɫ��
bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard, int * color)
{
	BYTE cbCardDataTemp[MAX_INDEX];
	CopyMemory(cbCardDataTemp,cbCardIndex,MAX_INDEX);

	if (cbCurrentCard != -1)
	{
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardDataTemp[cbCurrentIndex]++;
	}

	//�����ж�
	BYTE cbCardColor=0xFF;

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i==m_cbMagicIndex) continue;
		if (cbCardDataTemp[i]!=0)
		{
			//��ɫ�ж�
			if (cbCardColor!=0xFF)
				return false;

			//���û�ɫ
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//��������
			i=(i/9+1)*9-1;
		}
	}

	//�������ֻ������
	if( cbCardColor == 0xFF )
	{
		ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardDataTemp[m_cbMagicIndex] > 0 );
		//������
		ASSERT( cbItemCount > 0 );
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind < WIK_GANG_PI)
		{
			BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
			if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
		}
	}
	*color = cbCardColor;
	return true;
}

//��С����
DWORD CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	int nFourCount = 0;
	//����ж�
	// if (cbWeaveCount!=0) return 0;

	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//��������
	int nCardCount = 0;
	
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		nCardCount += cbCardIndexTemp[i];
	}
	if (nCardCount < 13) return 0;
	
	if (nCardCount!= 14)
	{
		BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;
	}
	//������Ŀ
	int cbReplaceCount = cbCardIndexTemp[m_cbMagicIndex];
	int nMagicCardCount = cbReplaceCount;
	//���㵥��
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//���ƹ���
		if( i == m_cbMagicIndex ) continue;

		//����ͳ��
		if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount--;

		if(cbReplaceCount < 0)
		{
			return 0;
		}		
		if (cbCardCount >= 4)
		{
			nFourCount ++;
		}
	}	

	if(nFourCount >= 2)
	{
		return CHR_QI_DUI_3;
	}
	if(nFourCount == 1)
	{
		return CHR_QI_DUI_2;
	}

	return  CHR_QI_DUI;

}

//����
bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
{
	//�������
	BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
	if( cbCardValue != 1 && cbCardValue != 9 ) return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			BYTE j = 0;
			for(; j < 3; j++ )
			{
				cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
				if( cbCardValue == 1 || cbCardValue == 9 ) break;
			}
			if( j == 3 ) return false;
		}
		else
		{
			cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
			if( cbCardValue != 1 && cbCardValue != 9 ) return false;
		}
	}
	return true;
}

//����
bool CGameLogic::IsJiangHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	for (int i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_LEFT 
			|| WeaveItem[i].cbWeaveKind == WIK_CENTER 
			|| WeaveItem[i].cbWeaveKind == WIK_RIGHT )
		{
			return false;
		}
		if (WeaveItem[i].cbWeaveKind >= WIK_GANG_PI )
		{
			continue;
		}
		if (!IsJiangByCardData(WeaveItem[i].cbCenterCard))
		{
			return false;
		}		
	}

	//������Ŀ
	BYTE cbReplaceCount = 0;

	//��ʱ����
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//��������
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//���㵥��
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (i == m_cbMagicIndex)
		{
			continue;
		}
		if (cbCardIndexTemp[i] == 0)
		{
			continue;
		}
		if (!IsJiangByIndex(i))
		{
			return false;
		}	
	}
	return true;
}

bool CGameLogic::IsJiangByCardData(BYTE cbCardData)
{
	if (cbCardData >= 0x30)
	{
		return false;
	}
	BYTE cbValue =(cbCardData & MASK_VALUE);

	//����ͳ��
	if( cbValue != 2 && cbValue != 5 && cbValue != 8 )
	{
		return false;
	}
	return true;
}

bool CGameLogic::IsJiangByIndex(BYTE cbCardIndex)
{
	if (cbCardIndex >= 27)
	{
		return false;
	}
	BYTE cbValue =(SwitchToCardData(cbCardIndex) & MASK_VALUE);

	//����ͳ��
	if( cbValue != 2 && cbValue != 5 && cbValue != 8 )
	{
		return false;
	}
	return true;
}


bool CGameLogic::IsMenQing(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
// 	BYTE cbCardCount=GetCardCount(cbCardIndex);
// 	if (cbCardCount < 13)
// 	{
// 		return false;
// 	}
// 	return true;
	//����ж�
	if (cbWeaveCount==0) return true;

	bool bValue = true;
	for (int i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbPublicCard != FALSE)
		{
			bValue = false;
		}
	}
	return bValue;
}

//�Ƿ���
bool CGameLogic::IsHuaZhu( const BYTE cbCardIndex[], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	BYTE cbColor[3] = { 0,0,0 };
	for( BYTE i = 0; i < MAX_INDEX; i++ )
	{
		if( cbCardIndex[i] > 0 )
		{
			BYTE cbCardColor = SwitchToCardData(i)&MASK_COLOR;
			cbColor[cbCardColor>>4]++;

			i = (i/9+1)*9-1;
		}
	}
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		BYTE cbCardColor = WeaveItem[i].cbCenterCard&MASK_COLOR;
		cbColor[cbCardColor>>4]++;
	}
	//ȱһ�žͲ��ǻ���
	for( BYTE i = 0; i < CountArray(cbColor); i++ )
		if( cbColor[i] == 0 ) return false;

	return true;
}

BYTE CGameLogic::GetCardColor(BYTE cbCardDat)
{
	ASSERT(IsValidCard(cbCardDat));
	return ((cbCardDat&MASK_COLOR)>>4)+1;
}

BYTE CGameLogic::GetCardValue(BYTE cbCardDat)
{
	ASSERT(IsValidCard(cbCardDat));
	return (cbCardDat&MASK_VALUE);
}
void CGameLogic::GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<GDMJ_Card_Info>& kCardInfoList )
{
	for (int m=1;m<=CARD_COLOR_TIAO;m++)
	{	
		GDMJ_Card_Info kInfo;
		kInfo.nColor = m;
		kInfo.nLen = 0;
		for(int i=0;i<nHandCardCount;i++)
		{
			BYTE cbColor= GetCardColor(pHandCardData[i]);
			if (cbColor== kInfo.nColor )
			{
				kInfo.kCardList.push_back(pHandCardData[i]);
				kInfo.nLen++;
			}
		}
		kCardInfoList.push_back(kInfo);
	}
	std::sort(kCardInfoList.begin(),kCardInfoList.end());
}
BYTE CGameLogic::GetAutoDiscardData(const BYTE cbCardIndex[MAX_INDEX],BYTE nQueColor,int nWeaveCont)
{
	BYTE cbCardData[MAX_COUNT];
	ZeroMemory(cbCardData,sizeof(cbCardData));
	SwitchToCardData(cbCardIndex,cbCardData);

	std::vector<GDMJ_Card_Info> kCardInfoList;
	GetCardInfoList(cbCardData,MAX_COUNT - nWeaveCont ,kCardInfoList);

	BYTE cbOutCard = 0;
	for (int i=0;i<(int)kCardInfoList.size();i++)
	{
		if (kCardInfoList[i].nColor == nQueColor && (int)kCardInfoList[i].kCardList.size() >0)
		{
			cbOutCard = kCardInfoList[i].kCardList[0];
			break;
		}
		else if( (int)kCardInfoList[i].kCardList.size()>0 )
		{
			cbOutCard = kCardInfoList[i].kCardList[0];
			break;
		}
	}
	ASSERT(cbOutCard !=0 );
	return cbOutCard;
}

//�����аװ�
bool CGameLogic::CheckHZBB(const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	for (int i = 0; i < cbWeaveCount; i++)
	{
		if ((WeaveItem[i].cbWeaveKind == WIK_PENG || WeaveItem[i].cbWeaveKind == WIK_GANG) && 
			(WeaveItem[i].cbCenterCard == 53 || WeaveItem[i].cbCenterCard == 55)) return true;
	}
	return false;
}

//��������
bool CGameLogic::CheckDanZhang(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//����Ƿ������
	BYTE cbDanZhang = 0;
	BYTE cbCardIndexTemp[MAX_INDEX] = {0};
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]--;
	CAnalyseItemArray AnalyseItemArray;
	if (m_cbMagicIndex == MAX_INDEX || cbCardIndexTemp[m_cbMagicIndex] == 0)
	{
		for(BYTE i = 0; i < MAX_INDEX; i++)
		{
			if (i == m_cbMagicIndex || i == SwitchToCardIndex(cbCurrentCard)) continue;
			AnalyseItemArray.RemoveAll();
			cbCardIndexTemp[i]++;
			AnalyseCard(cbCardIndexTemp,NULL,0,AnalyseItemArray,false, 4);
			if (AnalyseItemArray.GetCount() > 0) 
			{
				return false;
			}
			cbCardIndexTemp[i]--;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

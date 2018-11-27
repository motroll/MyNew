#include "StdAfx.h"
#include "DataBaseAide.h"
#include "DataBase.h"

//���캯��
CDataBaseAide::CDataBaseAide(IUnknownEx * pIUnknownEx)
{
}
//��������
CDataBaseAide::~CDataBaseAide()
{
}

bool CDataBaseAide::SetDataBase(IUnknownEx *pIUnKnownEx)
{
	m_pIDataBase = (IDataBase *)pIUnKnownEx;
	if (m_pIDataBase != NULL)
		return true;
	else
		return false;
}

VOID *CDataBaseAide::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	if (m_pIDataBase != NULL)
		return m_pIDataBase;
	return NULL;
}
/////////////////////////////////
////ȡ����

INT CDataBaseAide::GetValue_INT(LPCTSTR pszItem)
{
	INT nValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, nValue);
		return nValue;
	}
	return 0;
}

UINT CDataBaseAide::GetValue_UINT(LPCTSTR pszItem)
{
	UINT ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

DOUBLE CDataBaseAide::GetValue_DOUBLE(LPCTSTR pszItem)
{
	DOUBLE  dbValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, dbValue);
		return dbValue;

	}
	return 0;
}

BYTE CDataBaseAide::GetValue_BYTE(LPCTSTR pszItem)
{
	BYTE  bValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, bValue);
		return bValue;
	}

	return 0;
}

LONG CDataBaseAide::GetValue_LONG(LPCTSTR pszItem)
{
	LONG  lValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, lValue);
		return lValue;

	}
	return 0;

}

WORD CDataBaseAide::GetValue_WORD(LPCTSTR pszItem)
{
	WORD  wValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, wValue);
		return wValue;

	}
	return 0;

}

DWORD CDataBaseAide::GetValue_DWORD(LPCTSTR pszItem)
{
	DWORD  ulValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, ulValue);
		return ulValue;

	}
	return 0;
}

FLOAT CDataBaseAide::GetValue_FLOAT(LPCTSTR pszItem)
{

	return 0;

}

LONGLONG CDataBaseAide::GetValue_LONGLONG(LPCTSTR pszItem)
{
	__int64 llValue;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, llValue);
		return llValue;

	}
	return 0;
}

VOID CDataBaseAide::GetValue_VarValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	ASSERT(false);
	return;
}

VOID CDataBaseAide::GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime)
{
	DOUBLE fOut = GetValue_DOUBLE(pszItem);
	VariantTimeToSystemTime(fOut,&SystemTime);
	return;
}


//��ȡ����
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetParameter(pszItem, DBVarValue);
	}

}
//��ȡ����
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize)
{
	CDBVarValue temp;
	GetParameter(pszItem, temp);
	CopyMemory(pszString,(char*)_bstr_t( temp ),uSize);
}
//��ȡ����
VOID CDataBaseAide::GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize)
{
	CDBVarValue temp;
	GetParameter(pszItem, temp);
	CopyMemory(pszString,(char*)_bstr_t( temp ),uSize);
}

//���ò���
VOID CDataBaseAide::ResetParameter()
{
	lReturnValue = -1;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ClearParameters();
		((CDataBase*)m_pIDataBase)->AddParameter(TEXT("RETURN_VALUE"), adInteger, adParamReturnValue, sizeof(long), _variant_t((long)0));
	}

}

//��ȡ�ַ�
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, (char *)pszString, uMaxCount);
	}
}

//��ȡ�ַ�
VOID CDataBaseAide::GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem, (char *)pszString, uMaxCount);
	}
}

//�洢����
LONG CDataBaseAide::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	lReturnValue = -1;
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->ExecuteProcess(pszSPName, bRecordset);
		lReturnValue = ((CDataBase*)m_pIDataBase)->GetReturnValue();
		return lReturnValue;
	}
	return lReturnValue;

}
LONG CDataBaseAide::GetReturnValue()
{
	return lReturnValue;
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, INT nValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)nValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE * pData,ULONG lSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		//��������
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

		//���ñ���
		SafeArrayBound.lLbound=0L;
		SafeArrayBound.cElements=lSize;
		//��������
		SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

		//��������
		for (LONG nIndex=0;nIndex<(LONG)lSize;nIndex++)
		{
			SafeArrayPutElement(pSafeArray,&nIndex,pData+nIndex);
		}

		//���ñ���
		VARIANT VarChunk;
		VariantClear(&VarChunk);
		VarChunk.parray=pSafeArray;
		VarChunk.vt=VT_ARRAY|VT_UI1;

		m_pIDataBase->AddParameter(pszItem,adLongVarBinary,adParamInput,lSize,CDBVarValue(VarChunk));
	}
}
//��ȡ����
void  CDataBaseAide::GetParameter(LPCTSTR pszItem, BYTE* szBuffer, UINT& uSize)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->GetFieldValue(pszItem,szBuffer,uSize);
	}
}
VOID CDataBaseAide::GetParameter(LPCTSTR lpFieldName, datastream& kDataStream)
{
	static BYTE s_TempData[MAX_ASYNCHRONISM_DATA];
	UINT	uGetSize = MAX_ASYNCHRONISM_DATA;
	GetParameter(lpFieldName,s_TempData,uGetSize);
	if (uGetSize != MAX_ASYNCHRONISM_DATA && uGetSize > 0)
	{
		kDataStream.pushValue((char*)s_TempData,uGetSize);
	}
}
//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, UINT uValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)uValue));
	}

}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)lValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LONGLONG lValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)lValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, BYTE cbValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)cbValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, WORD wValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)wValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DWORD dwValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)dwValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, FLOAT fValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)fValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, DOUBLE dValue, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adInteger, ParameterDirection, sizeof(long), _variant_t((long)dValue));
	}
}

//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCTSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		if (strlen(pszString) != 0)
		{
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, ParameterDirection, lstrlen(pszString), _variant_t(pszString));
		}
		else
		{
			//TCHAR szString[64]=" ";
			((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adChar, ParameterDirection, lstrlen(pszString) + 1, _variant_t(pszString));
		}
	}
}
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ParameterDirectionEnum ParameterDirection)
{

	if (m_pIDataBase != NULL)
	{
		double fOut;
		SystemTimeToVariantTime(&SystemTime,&fOut);
		AddParameter(pszItem, fOut,ParameterDirection);
	}
}

#if 0  //
//�������
VOID CDataBaseAide::AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		((CDataBase*)m_pIDataBase)->AddParameter(pszItem, adParamInput, adVarChar, lstrlen(pszString), _variant_t(pszString));
	}
}
#endif

//�������
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{
	if (m_pIDataBase != NULL)
	{
		m_pIDataBase->AddParameter(pszItem,adVarChar,ParameterDirection,uSize,CDBVarValue(pszString));
	}
}
//�������
VOID CDataBaseAide::AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ParameterDirectionEnum ParameterDirection)
{

	if (m_pIDataBase != NULL)
	{
		m_pIDataBase->AddParameter(pszItem,adVarChar,ParameterDirection,uSize,CDBVarValue(pszString));
	}
}













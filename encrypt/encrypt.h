#pragma once

#include <WinCrypt.h>

namespace encrypt
{
	typedef enum 
	{
		BIN_BASE64,
		BIN_HEX
	}BIN_TYPE;
}
// ����Hash���ɹ�����0��ʧ�ܷ���GetLastError()
//  CONST BYTE *pbData, ��������
//  DWORD dwDataLen,    ���������ֽڳ���
//  ALG_ID algId        Hash �㷨��CALG_MD5,CALG_SHA
DWORD  GetHash(BYTE *pbData, DWORD dwDataLen, ALG_ID algId,CAtlStringA &out);

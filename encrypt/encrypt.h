#pragma once

#include <WinCrypt.h>
//aes 128����
void AesEncode(CAtlStringA strIn,CAtlStringA &strOut,CAtlStringA strKey);
//aes 128����
void AesDecode(CAtlStringA strIn,CAtlStringA &strOut,CAtlStringA strKey);

// ����Hash���ɹ�����0��ʧ�ܷ���GetLastError()
//  CONST BYTE *pbData, ��������
//  DWORD dwDataLen,    ���������ֽڳ���
//  ALG_ID algId        Hash �㷨��CALG_MD5,CALG_SHA
DWORD  GetHash(BYTE *pbData, DWORD dwDataLen, ALG_ID algId,CAtlStringA &out);

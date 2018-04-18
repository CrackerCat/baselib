// ***************************************************************
//  ģ�����ƣ� sqlite��װ  
//  �汾�ţ� 1.0
//  �������ڣ� 2012-07-11
//  �����ߣ�hwz
//  -------------------------------------------------------------
//  ���������� ��sqlite�ӿڽ��з�װ
//  -------------------------------------------------------------
//  ��Ȩ��Ϣ��Copyright (C) 2012 - All Rights Reserved
// ***************************************************************
//  �޸����� 2013-04-18 �汾��  v1.0.016
//  �޸����� ȥ���׳��쳣  modify by hwz
// ***************************************************************

#include "stdafx.h"
#include "CppSQLite3U.h"
#include "SQLiteTool.h"
#include <string>
#include <atlconv.h>


CppSQLite3ExceptionU::CppSQLite3ExceptionU(const int nErrCode,
									LPTSTR szErrMess,
									bool bDeleteMsg/*=true*/) :
									mnErrCode(nErrCode)
{
	int ss = szErrMess ? _tcslen(szErrMess)+50 : 50;
	mpszErrMess=new TCHAR[ss];
	_stprintf_s(mpszErrMess, ss, _T("%s[%d]: %s"),
								errorCodeAsString(nErrCode),
								nErrCode,
								szErrMess ? szErrMess : _T(""));

	if (bDeleteMsg && szErrMess)
	{
		CSQLiteTool::Get()->SqliteFree(szErrMess);
	}
}


CppSQLite3ExceptionU::CppSQLite3ExceptionU(const CppSQLite3ExceptionU&  e) :
									mnErrCode(e.mnErrCode)
{
	mpszErrMess = 0;
	if (e.mpszErrMess)
	{
		int ss = _tcslen(e.mpszErrMess)+10;
		mpszErrMess=new TCHAR[ss];
		_stprintf_s(mpszErrMess, ss, _T("%s"), e.mpszErrMess);
	}
}


LPCTSTR CppSQLite3ExceptionU::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
		case SQLITE_OK          : return _T("SQLITE_OK");
		case SQLITE_ERROR       : return _T("SQLITE_ERROR");
		case SQLITE_INTERNAL    : return _T("SQLITE_INTERNAL");
		case SQLITE_PERM        : return _T("SQLITE_PERM");
		case SQLITE_ABORT       : return _T("SQLITE_ABORT");
		case SQLITE_BUSY        : return _T("SQLITE_BUSY");
		case SQLITE_LOCKED      : return _T("SQLITE_LOCKED");
		case SQLITE_NOMEM       : return _T("SQLITE_NOMEM");
		case SQLITE_READONLY    : return _T("SQLITE_READONLY");
		case SQLITE_INTERRUPT   : return _T("SQLITE_INTERRUPT");
		case SQLITE_IOERR       : return _T("SQLITE_IOERR");
		case SQLITE_CORRUPT     : return _T("SQLITE_CORRUPT");
		case SQLITE_NOTFOUND    : return _T("SQLITE_NOTFOUND");
		case SQLITE_FULL        : return _T("SQLITE_FULL");
		case SQLITE_CANTOPEN    : return _T("SQLITE_CANTOPEN");
		case SQLITE_PROTOCOL    : return _T("SQLITE_PROTOCOL");
		case SQLITE_EMPTY       : return _T("SQLITE_EMPTY");
		case SQLITE_SCHEMA      : return _T("SQLITE_SCHEMA");
		case SQLITE_TOOBIG      : return _T("SQLITE_TOOBIG");
		case SQLITE_CONSTRAINT  : return _T("SQLITE_CONSTRAINT");
		case SQLITE_MISMATCH    : return _T("SQLITE_MISMATCH");
		case SQLITE_MISUSE      : return _T("SQLITE_MISUSE");
		case SQLITE_NOLFS       : return _T("SQLITE_NOLFS");
		case SQLITE_AUTH        : return _T("SQLITE_AUTH");
		case SQLITE_FORMAT      : return _T("SQLITE_FORMAT");
		case SQLITE_RANGE       : return _T("SQLITE_RANGE");
		case SQLITE_ROW         : return _T("SQLITE_ROW");
		case SQLITE_DONE        : return _T("SQLITE_DONE");
		case CPPSQLITE_ERROR    : return _T("CPPSQLITE_ERROR");
		default: return _T("UNKNOWN_ERROR");
	}
}


CppSQLite3ExceptionU::~CppSQLite3ExceptionU()
{
	if (mpszErrMess)
	{
 		delete [] mpszErrMess;
		mpszErrMess = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CppSQLite3DBU

CppSQLite3DBU::CppSQLite3DBU()
{
	mpDB = 0;
	mnBusyTimeoutMs = 60000; // 60 seconds
	mpVM = NULL;
	autoShutDown = true;
}

CppSQLite3DBU::CppSQLite3DBU(const CppSQLite3DBU& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
	mpVM=NULL;
	autoShutDown = false;
}
void CppSQLite3DBU::setAutoShutDown(bool autoShutDown)
{
	this->autoShutDown = autoShutDown;
}
CppSQLite3DBU::~CppSQLite3DBU()
{
	close();
	if(autoShutDown)
		CSQLiteTool::Get()->Shutdown();
}

////////////////////////////////////////////////////////////////////////////////

CppSQLite3DBU& CppSQLite3DBU::operator=(const CppSQLite3DBU& db)
{
	mpDB = db.mpDB;
	mnBusyTimeoutMs = 60000; // 60 seconds
	return *this;
}

bool CppSQLite3DBU::open(LPCTSTR szFile)
{
    int nRet = CSQLiteTool::Get()->OpenDB(CT2CA(szFile,CP_UTF8), &mpDB); 
	if (nRet != SQLITE_OK)
	{
		GetLastMsg();
		return false;
	}
	InitParam();
	return true;
}
bool CppSQLite3DBU::openEx( LPCTSTR pszMemory )
{
	int nRet;

	nRet = CSQLiteTool::Get()->OpenDB(CT2A(pszMemory,CP_UTF8), &mpDB); 
	if (nRet != SQLITE_OK)
	{
		GetLastMsg();
		return false;
	}
	InitParam();
	return true;
}
void CppSQLite3DBU::InitParam()
{
	CSQLiteTool::Get()->SetBusyTimeout(mpDB,mnBusyTimeoutMs);
	//CSQLiteTool::Get()->ExecuteSQL(mpDB, "PRAGMA journal_mode = WAL;", NULL, 0, 0); 
	CSQLiteTool::Get()->ExecuteSQL(mpDB, "PRAGMA encoding=UTF-8;", NULL, 0, 0); 
}
void CppSQLite3DBU::GetLastMsg()
{
	LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
	lastMsg = szError;
	//CSQLiteTool::Get()->SqliteFree(szError);
}
bool CppSQLite3DBU::SetKeyDB(LPCTSTR szKey)
{
	if(!mpDB)
	{
		return false;
	}
	std::string pwd = (char*)CT2CA(szKey,CP_UTF8);
	int nRet=CSQLiteTool::Get()->SetKey(mpDB,pwd.c_str(),pwd.length());
	if(nRet != SQLITE_OK)
	{
		GetLastMsg();
		close();
		return false;
	}
	return true;
}

bool CppSQLite3DBU::EncryptDB(LPCTSTR szDBFile,LPCTSTR szKey)
{
	int nRet = CSQLiteTool::Get()->OpenDB(CT2CA(szDBFile,CP_UTF8),&mpDB);
	if (nRet != SQLITE_OK)
	{
		GetLastMsg();
		return false;	
	}
	else
	{
		std::string pwd = (char*)CT2CA(szKey,CP_UTF8);
		nRet=CSQLiteTool::Get()->SetKey(mpDB,pwd.c_str(),pwd.length());
		if(nRet != SQLITE_OK)
		{
			GetLastMsg();
			close();
			return false;
		}
	}
	InitParam();
	return true;
}
bool CppSQLite3DBU::ResetKeyDB(LPCTSTR szKey)
{
	int nRet = 0;
	std::string key = (char*)CT2CA(szKey,CP_UTF8);
	if( nRet != CSQLiteTool::Get()->ResetKey(mpDB,key.c_str(),key.length()) )
	{
		GetLastMsg();
		close();
		return false;
	}
	return true;
}

//�ͷ�������Դ
void CppSQLite3DBU::Release()
{
	CSQLiteTool::Get()->Release();

	CSQLiteTool::Destroy();
}

void CppSQLite3DBU::close()
{
	if (mpDB)
	{
		finalize();
		
		int nRet = CSQLiteTool::Get()->CloseDB(mpDB);
		if (nRet != SQLITE_OK)
		{
			GetLastMsg();
		}
		mpDB = 0;
	}
}

CppSQLite3StatementU CppSQLite3DBU::compileStatement(LPCTSTR szSQL)
{
	if (!mpDB)
	{
		return CppSQLite3StatementU();
	}	

	sqlite3_stmt* pVM = compile(szSQL);

	return CppSQLite3StatementU(mpDB, pVM);
}


bool CppSQLite3DBU::tableExists(LPCTSTR szTable)
{
	TCHAR szSQL[128];

	_stprintf_s(szSQL, _T("select count(*) from sqlite_master where type='table' and name='%s'"),	szTable);

	int nRet = execScalar(szSQL);

	return (nRet > 0);
}

bool CppSQLite3DBU::finalize()
{
	if (mpVM)
	{
		int nRet = CSQLiteTool::Get()->Finalize(mpVM);
		mpVM = 0;
		if (nRet != SQLITE_OK)
		{
			GetLastMsg();
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

int CppSQLite3DBU::execDML(LPCTSTR szSQL)
{
	int nRet;
	sqlite3_stmt* pVM; 

	if (!mpDB)
	{
		return SQLITE_ERROR;
	}

	CString strSQL=szSQL;

	if(strSQL.IsEmpty())
	{
		return SQLITE_ERROR;
	}
	do{ 
		pVM = compile(szSQL);
		if(pVM==NULL)
		{
			nRet=SQLITE_ERROR;
			break;
		}

		nRet = CSQLiteTool::Get()->DBStep(pVM);
		if (nRet == SQLITE_ERROR)
		{
			GetLastMsg();
			//��¼��־
			if(NULL!=pVM)
			{
				CSQLiteTool::Get()->Finalize(pVM);
				pVM=NULL;
			}			
			//close();
			nRet=SQLITE_ERROR;
			break;
		}
		nRet = CSQLiteTool::Get()->Finalize(pVM);

		//LOG_FUNC_P0("sleep");
		//Sleep(1);
	} 
	while( nRet == SQLITE_SCHEMA );
	if(nRet != SQLITE_OK)
		GetLastMsg();
	return nRet;
}

CppSQLite3QueryU CppSQLite3DBU::execQuery(LPCTSTR szSQL)
{
	if (!mpDB)
	{
		return CppSQLite3QueryU();
	}

	int nRet;
	sqlite3_stmt* pVM; 
	do{ 
		pVM = compile(szSQL);
		if(pVM==NULL)
		{
			nRet=SQLITE_ERROR;
			break;
		}
		nRet = CSQLiteTool::Get()->DBStep(pVM);
		if (nRet == SQLITE_DONE)
		{	
			return CppSQLite3QueryU(mpDB, pVM, true/*eof*/);
		}
		else if (nRet == SQLITE_ROW)
		{	// at least 1 row
			return CppSQLite3QueryU(mpDB, pVM, false/*eof*/);
		}
		else if(nRet==SQLITE_ERROR){
			GetLastMsg();
		}
		nRet = CSQLiteTool::Get()->Finalize(pVM);
		Sleep(1);
	} 
	while( nRet == SQLITE_SCHEMA ); 
	GetLastMsg();
	return CppSQLite3QueryU();
}


int CppSQLite3DBU::execScalar(LPCTSTR szSQL)
{
	CppSQLite3QueryU q = execQuery(szSQL);

	//��¼��־

	if (q.eof() || q.numFields() < 1)
	{
		//��¼��־

		return -1;
	}		

	return _tstoi(q.fieldValue(0));
}

CString CppSQLite3DBU::execScalarStr(LPCTSTR szSQL)
{
	CppSQLite3QueryU q = execQuery(szSQL);

	//��¼��־

	if (q.eof() || q.numFields() < 1)
	{
		//��¼��־

		return _T("");
	}	
	
	return (CString)q.getStringField(0);
}

sqlite_int64 CppSQLite3DBU::lastRowId()
{
	return CSQLiteTool::Get()->GetLastInsertRowID(mpDB);
}


void CppSQLite3DBU::setBusyTimeout(int nMillisecs)
{
	mnBusyTimeoutMs = nMillisecs;
	CSQLiteTool::Get()->SetBusyTimeout(mpDB, mnBusyTimeoutMs);
}


bool CppSQLite3DBU::checkDB()
{
	if (!mpDB)
	{
		//��¼��־

		return false;
	}	

	return true;
}


sqlite3_stmt* CppSQLite3DBU::compile(LPCTSTR szSQL)
{	
	if (!mpDB)
	{
		return NULL;
	}
	sqlite3_stmt* pVM;
	int nRet=CSQLiteTool::Get()->Prepare16_V2(mpDB,szSQL,-1,&pVM,NULL);
	if (nRet != SQLITE_OK)
	{		
		GetLastMsg();
		//��¼��־
		if(NULL!=pVM)
		{
			CSQLiteTool::Get()->Finalize(pVM);
		}
		//close();
		pVM=NULL;
	}

	return pVM;
}

int CppSQLite3DBU::execSys(LPCTSTR szSQL,CAtlString & stRet )
{
	int nRet=0;

	std::string  strtemp;
#ifdef _UNICODE
	strtemp = (char*)CT2CA(szSQL,CP_UTF8);
#else
	strtemp=szSQL;
#endif
	char *msg = NULL;
	nRet=CSQLiteTool::Get()->ExecuteSQL(mpDB, strtemp.c_str(), NULL, 0, &msg); 
    stRet = msg;
	if( msg )
	{
		CSQLiteTool::Get()->SqliteFree(msg);
	}
	if(nRet!=SQLITE_OK)
	{
		lastMsg = stRet;
	}
	return nRet;
}

//////////////////////// CppSQLite3StatementU  ///////////////////////////////////////////
CppSQLite3StatementU::CppSQLite3StatementU()
{
	mpDB = 0;
	mpVM = 0;
}

CppSQLite3StatementU::CppSQLite3StatementU(const CppSQLite3StatementU& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;
	// Only one object can own VM
	const_cast<CppSQLite3StatementU&>(rStatement).mpVM = 0;
}

CppSQLite3StatementU::CppSQLite3StatementU(sqlite3* pDB, sqlite3_stmt* pVM)
{
	mpDB = pDB;
	mpVM = pVM;
}

CppSQLite3StatementU::~CppSQLite3StatementU()
{
	try
	{
		finalize();
	}
	catch (...)	{}
}

CppSQLite3StatementU& CppSQLite3StatementU::operator=(const CppSQLite3StatementU& rStatement)
{
	mpDB = rStatement.mpDB;
	mpVM = rStatement.mpVM;

	const_cast<CppSQLite3StatementU&>(rStatement).mpVM = 0;
	return *this;
}

int CppSQLite3StatementU::execDML()
{
	if (!mpDB || mpVM == 0)
	{
		return SQLITE_ERROR;
	}

	int nRet = CSQLiteTool::Get()->DBStep(mpVM);

	if (nRet == SQLITE_DONE)
	{
		int nRowsChanged = CSQLiteTool::Get()->SqliteChanges(mpDB);

		nRet = CSQLiteTool::Get()->Reset(mpVM);
		if (nRet != SQLITE_OK)
		{
			LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
			lastMsg = szError;
			CSQLiteTool::Get()->SqliteFree(szError);
			return SQLITE_ERROR;
		}
		return nRowsChanged;
	}
	else
	{
		nRet = CSQLiteTool::Get()->Reset(mpVM);

		LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
		lastMsg = szError;
		CSQLiteTool::Get()->SqliteFree(szError);

		return SQLITE_ERROR;
	}

	return SQLITE_ERROR;
}


bool CppSQLite3StatementU::bind(int nParam, LPCTSTR szValue)
{
	if(mpVM==NULL)
	{
		return false;
	}

	int nRes = CSQLiteTool::Get()->BindText16(mpVM, nParam, szValue, -1, SQLITE_TRANSIENT);

	if (nRes != SQLITE_OK)
		return false;

	return true;
}


bool CppSQLite3StatementU::bind(int nParam, const int nValue)
{
	if(mpVM==NULL)
	{
		return false;
	}

	int nRes = CSQLiteTool::Get()->BindInt(mpVM, nParam, nValue);
	if (nRes != SQLITE_OK)
		return false;

	return true;
}


bool CppSQLite3StatementU::bind(int nParam, const double dValue)
{
	if(mpVM==NULL)
	{
		return false;
	}
	int nRes = CSQLiteTool::Get()->BindDouble(mpVM, nParam, dValue);
	if (nRes != SQLITE_OK)
		return false;

	return true;
}


bool CppSQLite3StatementU::bind(int nParam, const unsigned char* blobValue, int nLen)
{
	if(mpVM==NULL)
	{
		return false;
	}
	int nRes = CSQLiteTool::Get()->BindBlob(mpVM, nParam,(const void*)blobValue, nLen, SQLITE_TRANSIENT);
	if (nRes != SQLITE_OK)
	{
		LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
		lastMsg = szError;
		CSQLiteTool::Get()->SqliteFree(szError);

		return false;
	}
	return true;
}

bool CppSQLite3StatementU::bindNull(int nParam)
{
	if(mpVM==NULL)
	{
		return false;
	}
	int nRes = CSQLiteTool::Get()->BindNull(mpVM, nParam);
	if (nRes != SQLITE_OK)
	{
		LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
		lastMsg = szError;
		CSQLiteTool::Get()->SqliteFree(szError);

  		return false;
	}
	return true;
}


bool CppSQLite3StatementU::reset()
{
	if (mpVM)
	{
		int nRet = CSQLiteTool::Get()->Reset(mpVM);

		if (nRet != SQLITE_OK)
		{
			LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
			lastMsg = szError;
			CSQLiteTool::Get()->SqliteFree(szError);

			return false;
		}
	}

	return true;
}

bool CppSQLite3StatementU::finalize()
{
	if (mpVM)
	{
		int nRet = CSQLiteTool::Get()->Finalize(mpVM);
		mpVM = 0;

		if (nRet != SQLITE_OK)
		{
			LPCTSTR szError = (LPCTSTR) CSQLiteTool::Get()->GetErrMsg16(mpDB);
			lastMsg = szError;
			CSQLiteTool::Get()->SqliteFree(szError);

			return false;
		}
	}

	return true;
}


bool CppSQLite3StatementU::checkDB()
{
	if (mpDB == 0) 
		return false;

	return true;
}

bool CppSQLite3StatementU::checkVM()
{
	if (mpVM == 0)
		return false;

	return true;
}


/////////////////////  CppSQLite3QueryU  //////////////////////////////////////////////////
CppSQLite3QueryU::CppSQLite3QueryU()
{
	mpVM = 0;
	mbEof = true;
	mnCols = 0;
	mbOwnVM = false;
}


CppSQLite3QueryU::CppSQLite3QueryU(const CppSQLite3QueryU& rQuery)
{
	mpVM = rQuery.mpVM;
	const_cast<CppSQLite3QueryU&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
}


CppSQLite3QueryU::CppSQLite3QueryU(sqlite3* pDB, sqlite3_stmt* pVM,
								 bool bEof,	 bool bOwnVM/*=true*/)
{
	mpDB = pDB;
	mpVM = pVM;
	mbEof = bEof;
	mnCols = CSQLiteTool::Get()->GetColumnCount(mpVM);
	mbOwnVM = bOwnVM;
}

CppSQLite3QueryU::~CppSQLite3QueryU()
{
	try
	{
		finalize();
	}
	catch (...) {}
}


CppSQLite3QueryU& CppSQLite3QueryU::operator=(const CppSQLite3QueryU& rQuery)
{
	try
	{
		finalize();
	}
	catch (...)	{ }

	mpVM = rQuery.mpVM;
	const_cast<CppSQLite3QueryU&>(rQuery).mpVM = 0;
	mbEof = rQuery.mbEof;
	mnCols = rQuery.mnCols;
	mbOwnVM = rQuery.mbOwnVM;
	return *this;
}


int CppSQLite3QueryU::numFields()
{
	if(mpVM==NULL)
	{
		return 0;
	}

	return mnCols;
}


LPCTSTR CppSQLite3QueryU::fieldValue(int nField)
{
	if(mpVM==NULL)
	{
		return _T("");
	}

	if (nField < 0 || nField > mnCols-1)
	{
		//��¼��־

		return _T("");
	}		

	return (LPCTSTR)CSQLiteTool::Get()->GetColumnUnicodeText(mpVM, nField);
}


LPCTSTR CppSQLite3QueryU::fieldValue(LPCTSTR szField)
{
	int nField = fieldIndex(szField);

	return (LPCTSTR)CSQLiteTool::Get()->GetColumnUnicodeText(mpVM, nField);
}


int CppSQLite3QueryU::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return CSQLiteTool::Get()->GetColumnInt(mpVM, nField);
	}
}


int CppSQLite3QueryU::getIntField(LPCTSTR szField, int nNullValue/*=0*/)
{
	int nField = fieldIndex(szField);
	return getIntField(nField, nNullValue);
}

__int64 CppSQLite3QueryU::getInt64Field(LPCTSTR szField,__int64 nNullValue)
{
	int nField = fieldIndex(szField);
	return getInt64Field(nField, nNullValue);
}
__int64 CppSQLite3QueryU::getInt64Field(int nField,__int64 nNullValue)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return nNullValue;
	}
	else
	{
		return CSQLiteTool::Get()->GetColumnInt64(mpVM, nField);
	}
}


double CppSQLite3QueryU::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return fNullValue;
	}
	else
	{
		return CSQLiteTool::Get()->GetColumnDouble(mpVM, nField);
	}
}


double CppSQLite3QueryU::getFloatField(LPCTSTR szField, double fNullValue/*=0.0*/)
{
	int nField = fieldIndex(szField);

	return getFloatField(nField, fNullValue);
}


LPCTSTR CppSQLite3QueryU::getStringField(int nField, LPCTSTR szNullValue/*=""*/)
{
	if (fieldDataType(nField) == SQLITE_NULL)
	{
		return szNullValue;
	}
	else
	{
		return (LPCTSTR)CSQLiteTool::Get()->GetColumnUnicodeText(mpVM, nField);
	}
}

LPCTSTR CppSQLite3QueryU::getStringField(LPCTSTR szField, LPCTSTR szNullValue/*=""*/)
{
	int nField = fieldIndex(szField);
	return getStringField(nField, szNullValue);
}


const unsigned char* CppSQLite3QueryU::getBlobField(int nField, int& nLen)
{
	if(mpVM==NULL)
	{
		return NULL;
	}

	if (nField < 0 || nField > mnCols-1)
	{
		return NULL;
	}		

	nLen = CSQLiteTool::Get()->GetColumnByteLen16(mpVM, nField);
	return (const unsigned char*)CSQLiteTool::Get()->GetColumnBlob(mpVM, nField);
}


const unsigned char* CppSQLite3QueryU::getBlobField(LPCTSTR szField, int& nLen)
{
	int nField = fieldIndex(szField);
	return getBlobField(nField, nLen);
}


bool CppSQLite3QueryU::fieldIsNull(int nField)
{
	return (fieldDataType(nField) == SQLITE_NULL);
}


bool CppSQLite3QueryU::fieldIsNull(LPCTSTR szField)
{
	int nField = fieldIndex(szField);
	return (fieldDataType(nField) == SQLITE_NULL);
}
int CppSQLite3QueryU::fieldIndex(LPCTSTR szField)
{
	if(mpVM==NULL)
	{
		return -1;
	}
	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			LPCTSTR szTemp = (LPCTSTR)CSQLiteTool::Get()->GetColumnUnicodeName(mpVM, nField);

			if (_tcscmp(szField, szTemp) == 0)
			{
				return nField;
			}
		}
	}
	//��¼��־
	return -1;
}


LPCTSTR CppSQLite3QueryU::fieldName(int nCol)
{
	if(mpVM==NULL)
	{
		return _T("");
	}
	if (nCol < 0 || nCol > mnCols-1)
	{
		//��¼��־
		
		return _T("");
	}
	return (LPCTSTR)CSQLiteTool::Get()->GetColumnUnicodeName(mpVM, nCol);
}


LPCTSTR CppSQLite3QueryU::fieldDeclType(int nCol)
{
	if(mpVM==NULL)
	{
		return _T("");
	}
	if (nCol < 0 || nCol > mnCols-1)
	{
		//��¼��־
		
		return _T("");
	}
	return (LPCTSTR)CSQLiteTool::Get()->GetColumnDeclType16(mpVM, nCol);
}


int CppSQLite3QueryU::fieldDataType(int nCol)
{
	if(mpVM==NULL)
	{
		return -1;
	}
	if (nCol < 0 || nCol > mnCols-1)
	{
		//��¼��־
		
		return -1;
	}
	return CSQLiteTool::Get()->GetColumnType(mpVM, nCol);
}


bool CppSQLite3QueryU::eof()
{
	if(mpVM==NULL)
	{
		return true;
	}

	return mbEof;
}


void CppSQLite3QueryU::nextRow()
{
	if(mpVM==NULL)
	{
		mbEof = true;
		return;
	}

	int nRet = CSQLiteTool::Get()->DBStep(mpVM);

	if (nRet == SQLITE_DONE)
	{
		mbEof = true;
	}
	else if (nRet == SQLITE_ROW)
	{
		// more rows, nothing to do
	}
	else
	{
		nRet = CSQLiteTool::Get()->Finalize(mpVM);
		mpVM = 0;
		LPCTSTR szError = (LPCTSTR)CSQLiteTool::Get()->GetErrMsg16(mpDB);
		lastMsg = szError;
		CSQLiteTool::Get()->SqliteFree(szError);

		mbEof = true;
	}
}


bool CppSQLite3QueryU::finalize()
{
	if (mpVM && mbOwnVM)
	{
		int nRet = CSQLiteTool::Get()->Finalize(mpVM);
		mpVM = 0;
		if (nRet != SQLITE_OK)
		{
			LPCTSTR szError = (LPCTSTR)CSQLiteTool::Get()->GetErrMsg16(mpDB);
			lastMsg = szError;
			CSQLiteTool::Get()->SqliteFree(szError);

			return false;
		}
	}

	return true;
}

bool CppSQLite3QueryU::checkVM()
{
	if (mpVM == 0)
	{
		//��¼��־
		
		return false;
	}

	return true;
}

CString DoubleQuotes(CString in)
{
	in.Replace(_T("\'"),_T("\'\'"));

	return in;
}

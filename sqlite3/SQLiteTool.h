#pragma once

#include "sqlite3.h"
#include "../SingleCpp.h"
#include "../baseX.h"
class CSQLiteTool
{
public:
	GarboSingle(CSQLiteTool);
public:
	int Initialize();
	int Shutdown();

	//�����ݿ�
	int  OpenDB16( const void *filename, sqlite3 **ppDb );

	//�����ݿ�
	int  OpenDB( const char *filename, sqlite3 **ppDb);

	//��ȡ������Ϣ
	const void *  GetErrMsg16(sqlite3 *);

	//��ȡ������Ϣ
	const char *  GetErrMsg(sqlite3 *);

	//��ȡ�������
	int  GetErrCode(sqlite3 *);

	//ִ�����
	int  ExecuteSQL(sqlite3*, const char *sql, 
		int (*callback)(void*,int,char**,char**), 
		void *,                                   
		char **errmsg);

	//�ر����ݿ�
	int  CloseDB(sqlite3 *);

	//�ر����ݿ�
	int  CloseDBV2(sqlite3 *);

	//�ͷ�
	int  Finalize(sqlite3_stmt *pStmt);

	//ִ����һ��
	int  DBStep(sqlite3_stmt *pStmt);

	//��ȡ���һ��������б�ʶ
	sqlite3_int64  GetLastInsertRowID(sqlite3*);

	//���ó�ʱ
	int  SetBusyTimeout(sqlite3*, int ms);

	int Prepare_V2(
		sqlite3 *db,           
		const char *zSql,       
		int nByte,             
		sqlite3_stmt **ppStmt,  
		const char **pzTail    
		);

	//Ԥ����
	int  Prepare16_V2(
		sqlite3 *db,           
		const void *zSql,      
		int nByte,             
		sqlite3_stmt **ppStmt, 
		const void **pzTail     
		);

	//Ԥ����
	int Prepare(sqlite3 *db,const char *zSql,int nByte,sqlite3_stmt **ppStmt,const char **pzTail);

	//�����仯
	int  SqliteChanges(sqlite3*);

	//����
	int  Reset(sqlite3_stmt *pStmt);

	//���ı�
	int  BindText(sqlite3_stmt*, int, const char*, int n, void(*)(void*));

	//���ı�
	int  BindText16(sqlite3_stmt*, int, const void*, int, void(*)(void*));

	//������
	int  BindInt(sqlite3_stmt*, int, int);

	//�󶨸�����
	int  BindDouble(sqlite3_stmt*, int, double);

	//��blob
	int  BindBlob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));

	//��nullֵ
	int  BindNull(sqlite3_stmt*, int);

	//��ȡ����
	int  GetColumnCount(sqlite3_stmt *pStmt);

	//��ȡ���ı�
	const unsigned char *  GetColumnText(sqlite3_stmt*, int iCol);

	//��ȡ���ı�
	const void *  GetColumnUnicodeText(sqlite3_stmt*, int iCol);

	//��ȡ��doubleֵ
	double  GetColumnDouble(sqlite3_stmt*, int iCol);

	//��ȡ������ֵ
	int  GetColumnInt(sqlite3_stmt*, int iCol);

	__int64 GetColumnInt64(sqlite3_stmt*,int iCol);

	//��ȡ����
	int  GetColumnByteLen(sqlite3_stmt*, int iCol);

	//��ȡ����
	int  GetColumnByteLen16(sqlite3_stmt*, int iCol);

	//��ȡ��blobֵ
	const void *  GetColumnBlob(sqlite3_stmt*, int iCol);

	//��ȡ����
	const char *  GetColumnName(sqlite3_stmt*, int N);

	//��ȡ����
	const void *  GetColumnUnicodeName(sqlite3_stmt*, int N);

	//��ȡdecl����
	const char *  GetColumnDeclType(sqlite3_stmt*,int);

	//��ȡdecl����
	const void *  GetColumnDeclType16(sqlite3_stmt*,int);

	//��ȡ������
	int  GetColumnType(sqlite3_stmt*, int iCol);

	//�����ڴ�
	void *  SqliteMalloc(int);

	//���·����ڴ�
	void *  SqliteRealloc(void*, int);

	//�ͷ��ڴ�
	void   SqliteFree(void*);
	void   SqliteFree(const char*);
	void   SqliteFree(const wchar_t*);

	//������������
	int ResetKey(sqlite3 *db, const void *pKey, int nKey);

	//��������
	int SetKey( sqlite3 *db, const void *pKey, int nKey);

	//�������
	char * MPrintf(const char* pszFormat,const char* pszErrMsg,const int nErrCode,char* pszMess);

	//�������
	char * MPrintf(const char* pszFormat,char* pszMsg);

	//�������
	char * VMPrintf(const char* pszFormat,va_list& va);

	//��ȡ��
	int GetTable(sqlite3 * pDB, const char * pszSql,char *** pszResult, int * pnRow,int * pnColumn, char **pzErrmsg );

	//�ͷű����
	void FreeTable(char **result);

	void SqliteInterrupt(sqlite3*);

	//����û� utf-8
	int UserAdd(sqlite3 * pDB,const char* nm,const char* pwd,bool isAdmin);
	int UserLogin(sqlite3 * pDB,const char* nm,const char* pwd);
	//�ͷ���Դ
	void Release();
private:
	base::DLL	dll;
};

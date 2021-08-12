/*
	sqlite3 ���ݿ���ع���
	author�� jiayh
*/
#pragma once
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#include "AsyncUtils/AsyncUtils.h"
#include "sqlite3/CppSQLite3U.h"
#include "List.h"

namespace SqliteOpt{
	struct Column
	{
		Column(CAtlString n,CAtlString t){
			name = n;
			type = t;
		}
		Column(char n,CAtlString t){
			name = n;
			type = t;
		}
		Column(){

		}
		CAtlString name;
		CAtlString type;
	};
	typedef std::map<CAtlString,Column> Columns;
	
	struct PairSql
	{
		CAtlString run1;
		CAtlString run2;
	};
	typedef std::vector<PairSql> PairSqls;
	
	/*
		�����������sqlת��
		@inParam
			�������
		@outParam
			ת����
		@return
			ת����
	*/
	CAtlString FormatSql(CAtlString& in);
	/*
		�����������sql��ԭ
		@inParam
			�������
		@outParam
			��ԭ���
		@return
			��ԭ���
	*/
	CAtlString TurnRawSql(CAtlString& in);

	/*
		ִ��DML sql��insert��delete��update��
		@inParam
			*db ���ݿ�ʵ��
			pszSQL sql���
		@return
			true-�ɹ���false-ʧ��
	*/
	bool ExecuteDML(CppSQLite3DBU *db,LPCTSTR pszSQL);
	/*
		ִ��sqlite����
		@inParam
			*db ���ݿ�ʵ��
			pszSQL sqlite����
		@outParam
			&strRet ����쳣��Ϣ
		@return
			true-�ɹ���false-ʧ��
	*/
	bool ExecuteSys(CppSQLite3DBU *db,LPCTSTR pszSQL,CAtlString &strRet);
	/*
		ִ�����ݲ�ѯ
		@inParam
			*db ���ݿ�ʵ��
			pszSQL sql���
		@outParam
			&result ���ؼ�¼����
		@return
			true-�ɹ���false-ʧ��
	*/
	bool ExecuteQuery(CppSQLite3DBU *db,LPCTSTR pszSQL,CppSQLite3QueryU& result);
	/*
		��������
		@inParam
			*db ���ݿ�ʵ��
	*/
	void BeginTransaction(CppSQLite3DBU *db);
	/*
		��������
		@inParam
			*db ���ݿ�ʵ��
			bSubmit true-�ύ��false-�ع�
	*/
	void EndTransaction(CppSQLite3DBU *db,bool bSubmit);
	/*
		��ȡ���ֶμ���
		@inParam
			*db ���ݿ�ʵ��
			table ������
		@return
			���ֶμ���
	*/
	Columns GetColumns(CppSQLite3DBU* db,LPCTSTR table);
	/*
		bool ����ת���ݴ洢���ͣ��磺true-1,false-0
		@inParam
			b ת��������
		@return
			ת��������
	*/
	CAtlString Bool2Save(bool b);
	/*
		�洢����תbool�������ݣ�֧��
		true - 1,true,yes,suc,success,y
		false - ����֮������
		@inParam
			v ת��������
		@return
			ת��������
	*/
	bool Save2Bool(CAtlString v);
	
	//��֤���������ݿ�
	bool IsNoPwdDB(CAtlString file,CSQLiteTool* driver);
	/*
		�첽ִ��sql�߼�����
	*/
	class Job
	{
	public:
		Job(){ mSync = NULL;  }
		virtual ~Job(){ 
			if(mSync){
				::CloseHandle(mSync);
				mSync = NULL;
			}
		}
		// ͬ���ȴ�
		inline void WantSync(){
			if(mSync==NULL)
			{
				mSync = ::CreateEvent(NULL,FALSE,FALSE,NULL);
				return ;
			}
			::WaitForSingleObject(mSync,INFINITE);
			Release();
		}
		// �ж��Ƿ����UI����
		virtual bool IsNeedUI() { 
			if(mSync==NULL)
				return true;
			return false;
		}
		// ִ��sqlҵ��
		virtual bool Execute(CppSQLite3DBU *sqlite){ 
			return true;
		}
		// ����ͬ�����
		virtual bool FireSync(){
			if(mSync)
			{
				::SetEvent(mSync);
				return true;
			}
			return false;
		}
		// �����ͷ�
		virtual void Release(){	}
		CAtlString mMyNsrsh;	// ��˰��˰��
	protected:
		HANDLE mSync; // ͬ���¼�
	};
	typedef base::IListImpl<Job,CLock> Jobs;

	
	//�򵥵ò�ѯ����
	struct SimpleWhere
	{
		SimpleWhere(){
			level = 0;
			like = false;
			bIN = false;
		}
		SimpleWhere(size_t l,CAtlString k,CAtlString v,bool b){
			level = l;
			key = k;
			val = v;
			like =b;
			bIN = false;
		}
		SimpleWhere(size_t l,CAtlString k,char v,bool b){
			level = l;
			key = k;
			val = v;
			like =b;
			bIN = false;
		}
		size_t level;	//����
		CAtlString key;	//�����ؼ���
		CAtlString val;	//����ֵ
		bool like;	//�Ƿ�ģ��
		bool bIN;	//��Χ����
	};
	//���ڶ�where���ݽ����ع�����
	class ReWhereX
	{
	public:
		virtual ~ReWhereX(){ }
		virtual void Release(){	}
		/*
			@inParam 
				k �ؼ���
				v ֵ
				like �Ƿ�ģ��ƥ��
			@return
				�ع����
		*/
		virtual CAtlString OnReDoX(const SimpleWhere& sw,bool &handled){ 
 			return L""; 
		}
	};
	typedef std::vector<SimpleWhere> SIMPLEWHERES;
	/*
		���ɳ���where���
		@inParam
			sw where�ṹ��
		@return
			where�Ӿ�
	*/
	CAtlString BuildUsualWhere(const SIMPLEWHERES& sw);
	/*
		����json����where�ṹ��
		@inParam
			json��ʽ�ַ�������ʽ���£�[{"key": "�ֶ�����", "value": "ƥ��ֵ", "level": "����", "like": "�Ƿ�ģ��"}]
		@outParam
			sw where�ṹ��
		@return
			where�Ӿ�
	*/
	size_t BuildWhereFromJson(const std::string& sJson,SIMPLEWHERES& sw);
	/*
		����where�ṹ�Լ��޸���������where sql�����,
		@inParam
			sw where�ṹ��
			rw �޸���ָ��
		@return
			where �Ӿ�			
	*/
	CAtlString BuildReWhereX(const SIMPLEWHERES& sw,ReWhereX *rw);
	/*
		����insert sql���
		@inParam 
			tbname ������
			*col �ֶ��б�
		@return
			����sql���
	*/
	CAtlString BuildInsertSql(CAtlString tbname,Columns *col);
	//��ҳ��������
	struct Pager
	{
		Pager(){
			index = -1;
			size = -1;
		}
		inline bool IsAll(){
			if(index==-1 && size==-1)
				return true;
			return false;
		}
		inline CAtlString Sql(){
			CAtlString offset;
			if(index<0 || size<0)
				return offset;
			offset.Format(L"limit %d offset %d",size,index);
			return offset;
		}
		long index;	//����
		long size;	//����
	};
	/*
		��json[pager]�ֶη����л���pager����
	@inParam 
		sJson ������ҳjson������ʽ���£�{"index": "����", "size": "ҳ���С"}			
	@return
		pager ����
	*/
	SqliteOpt::Pager Json2Pager(const std::string& sJson);
	
	//sqlite3���ݿ���ʶ���
	class Access : public base::BackLogicBase
	{
	public:
		Access(HWND win);
		virtual ~Access();
		void SetNoAsync();
		CAtlString GetFileDB();
		void TestNoPwd(CAtlString file,CSQLiteTool* driver);
		void UsePwd(CAtlString pwd);
		void SetBackupDB(CAtlString backfile);
		bool Open(CAtlString file,CAtlString driver);
		bool Open(CAtlString file,CSQLiteTool* driver);
		bool OpenClear(CAtlString file,CSQLiteTool* driver);
		bool OpenCheck(CAtlString file,CAtlString driver,Job *check);
		bool OpenCheck(CAtlString file,CSQLiteTool* driver,Job *check);
		void Append(std::tr1::shared_ptr<Job> job);
		void SyncRun(std::tr1::shared_ptr<Job> job);
		void Quit();
		void HandleDone();
		void GlobalName(CAtlString nm);
		bool IsOpen();
		void StartWAL();
		void SetNsrsh(CAtlString nsrsh);
	protected:
		void* Query(LPCTSTR pClass)
		{
			CAtlString tmpnm;
			{
				CLockGuard guard(&m_lkDBFILE);
				tmpnm = m_gnm;
			}
			if(tmpnm == pClass)
				return this;
			return NULL;
		}
		bool Run();
		void Done();
		bool CheckValid();
		void DriverInit();
	private:
		Jobs m_jobsW;	//�ȴ�ִ���������
		Jobs m_jobsD;	//ִ����ɶ���
		CLock	m_lkDBFILE;	//�������ݳ�Ա������
		CAtlString m_dbBack; //�����ļ�
		CAtlString m_dbfile;  //���ݿ��ļ�
		CAtlString m_pwd;	//��������
		CAtlString m_gnm;	//ȫ������
		CAtlString m_nsrsh; //��˰������
		CppSQLite3DBU *m_db; //sqlite3���ݶ���
		bool m_opened;	//���ݿ��״̬
		bool m_init;	//���ݿ��ʼ״̬
		bool m_noasync;	//ͬ���첽���
	};
};

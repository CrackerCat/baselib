#include "StdAfx.h"
#include "AsyncUtils.h"

namespace base
{
	CTaskJob::CTaskJob():m_nTag(-1),m_hnotifyWnd(NULL)
	{

	}

	CTaskJob::~CTaskJob()
	{

	}

	void CTaskJob::SetTag(int ntag)
	{
		m_nTag = ntag;
	}

	void CTaskJob::SetHwnd( HWND hWnd )
	{
		m_hnotifyWnd = hWnd;
	}

	void CTaskJob::SetPid( LPCTSTR pszPid )
	{
		m_strPid = pszPid;
	}
	/*-------------------------------------------------------------------------------------*/
	Back2Front::Back2Front()
	{

	}
	Back2Front::~Back2Front()
	{

	}
	bool Back2Front::pushStack(classLife *c,long nT)
	{
		long key = nT;
		CLockGuard lock(&m_mutex);
		if(nT==0)
			key = (long)c;	//���û��������ô�õ�ַ������
		KEY_STACK::iterator it = m_stack.find(key);
		if( it!=m_stack.end() )
			return false;
		c->AddRef();
		m_stack[key] = c;
		return true;
	}
	void Back2Front::clear()
	{
		CLockGuard lock(&m_mutex);
		KEY_STACK::iterator it = m_stack.begin();
		for(it;it!=m_stack.end();it++)
			it->second->RelRef();
		m_stack.clear();
	}
	bool Back2Front::postFront(HWND win,UINT msg,classLife *param)
	{
		if(win==NULL)
			return false;
		return (::PostMessage(win,msg,0,(LPARAM)(void*)param)?true:false);
	}
	/*-------------------------------------------------------------------------------------*/
	BackLogicBase::BackLogicBase(HWND win)
	{
		m_id = 0;
		m_thread = NULL;
		m_quit = NULL;
		m_run  = NULL;
		m_win    = win;
		m_curStats = UNSTART;
		Back2Front::Get()->pushStack(this);
	}
	BackLogicBase::~BackLogicBase(void)
	{
		if(m_quit)
		{
			::CloseHandle(m_quit);
			m_quit = NULL;
		}
		if(m_run)
		{
			::CloseHandle(m_run);
			m_run = NULL;
		}
	}
	void BackLogicBase::resetWin(HWND win)
	{
		m_win = win;
	}
	void BackLogicBase::start()
	{
		if(m_thread==NULL)
		{
			m_quit = ::CreateEvent(NULL,FALSE,FALSE,NULL);
			m_run = ::CreateEvent(NULL,FALSE,FALSE,NULL);
			//m_thread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE),0,NULL);
			m_thread = (HANDLE)_beginthreadex(NULL,0,&BackLogicBase::ThreadProc,this,0,&m_id);
		}
		::SetEvent(m_run);
	}
	void BackLogicBase::stop()
	{
		if(m_thread)
		{
			::ResetEvent(m_run);
			UpdateStatus(STOP);
		}
	}
	void BackLogicBase::close(DWORD waitTM)
	{
		DWORD tid = GetCurrentThreadId();
		if (m_thread) 
		{
			::ResetEvent(m_run);
			::SetEvent(m_quit);		//������̨
			if(m_id!=tid)
			{
				::WaitForSingleObject(m_thread,waitTM);
			}
			::CloseHandle(m_thread);
			UpdateStatus(FINISH);
			m_thread = NULL;
		}
	}
	BackLogicBase::Status  BackLogicBase::getStatus()
	{
		BackLogicBase::Status ret;
		CLockGuard lock(&m_lockStatus);
		ret = m_curStats;
		return ret;
	}
	void BackLogicBase::UpdateStatus(BackLogicBase::Status v)
	{
		CLockGuard lock(&m_lockStatus);
		m_curStats = v;
	}
	CAtlString BackLogicBase::getLastError()
	{
		CAtlString ret;
		CLockGuard lock(&m_lockError);
		ret = m_error;
		return ret;
	}
	void BackLogicBase::UpdateLastError(const CAtlString& v)
	{
		CLockGuard lock(&m_lockError);
		m_error = v;
	}
	unsigned int BackLogicBase::ThreadProc(void* p1)
	{
		BackLogicBase *pT = reinterpret_cast<BackLogicBase*>(p1);
		HANDLE events[10]={pT->m_quit,pT->m_run};
		::CoInitialize(NULL);
		::OleInitialize(NULL);
		while(1)
		{
			bool bPost=false;
			DWORD dwWait = WaitForMultipleObjects(2,events,FALSE,INFINITE);
			if(dwWait==WAIT_OBJECT_0)
			{
				break;
			}
			pT->UpdateStatus(WORKING);
			bPost = pT->Run();
			pT->UpdateStatus(STOP);
			if( bPost && pT->m_win)
				Back2Front::Get()->postFront(pT->m_win,LogicEvent<>::UM_LOGIC,pT);
		}
		pT->UpdateStatus(FINISH);
		pT->Done();
		::OleUninitialize();
		::CoUninitialize();		
		_endthreadex(0xdead);
		return 0xdead;
	}
}

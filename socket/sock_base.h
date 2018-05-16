#pragma once

#include "jadesocketiocp.h"
#include "Interface.h"
#include "tools.h"
class CheckAlive : public NETCHECK__
{
public:
	enum State
	{
		Connecting,		//��������
		Connected,		//����״̬
		Disconnectd,	//�ѶϿ�����
		Error			//�쳣
	};
	class Service
	{
	public:
		CAtlString SvrIp;	//��������ַ
		UINT  unPort;		//�������˿ں�
		UINT  unType;		//0---tcp,1---udp
		UINT  unTimout;		//��ʱ���õ�λ��
		char  TestPack[100];	//�������ݰ�
		Service& operator=(const Service& s)
		{
			Service *pT = const_cast<Service*>(&s);
			this->SvrIp   = pT->SvrIp;
			this->unPort  = pT->unPort;
			this->unType  = pT->unType;
			this->unTimout= pT->unTimout;
			memcpy(this->TestPack,pT->TestPack,sizeof(this->TestPack));
			return (*this);
		}
	};
	class Proxy : public base::IRef
	{
	public:
		virtual DWORD onStart(JadeSocketIocp<> *socket,CheckAlive *pbase) = 0;
		virtual void  onRecv(PRECV pRecv,DWORD &dwID) = 0;
		virtual void  onSend(PSEND pSend,DWORD &dwID) = 0;
		virtual void  onConnect(PCONNECTEX pConnect,DWORD &dwID) = 0;
		virtual void  onDisconnect(DWORD &dwID) = 0;
		virtual bool  IsConnected(void) = 0;
	};
	CheckAlive(const Service &svr,DWORD dwUser,PNETCHECK_CALLBACK fn);
	virtual ~CheckAlive();
	DWORD		Startup(Proxy *proxy);
	DWORD		PostEndService(void);
	Service*	GetService(void);
	int			DoCallBack(LPCTSTR pMsg,bool bSuc,bool bDis);
protected:
	static DWORD WINAPI BackService(LPVOID lpParameter);
	DWORD  Connect(void);
	void   OnSendCmd(PSEND,DWORD);
	void   OnSendCmd(PSENDTO,DWORD);
	CAtlString	State2String(State);
private:
	PNETCHECK_CALLBACK  m_fn;			//�¼��ص�����
	DWORD				m_dwUser;
	Service				m_svr;			//����������
	Proxy				*m_pProxy;
	JadeSocketIocp<>	m_socket;		//socket����
	HANDLE				m_hThread;		//�̶߳���
	int					m_suspended;	//�߳�SUSPENDED
	int					m_bEnd;			//�߳��ڽ������
	volatile State		m_state;		//״̬���
	DWORD				m_threadId;		//�߳�id
};


#include "proxysock5.h"
#include "proxyhttp.h"

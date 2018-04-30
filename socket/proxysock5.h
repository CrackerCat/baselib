#pragma once

#include "sock_base.h"
#include "../base.h"
class JadeSocket;
class ProxySock5 : public base::Ref<CheckAlive::Proxy>
{
public:
	enum Socks5State
	{
		S5StateDisconnected,
		S5StateConnecting,
		S5StateNegotiating,
		S5StateUpdConfirm,
		S5StateAuthenticating,
		S5StateConnected,
		S5StateConnectionRefused,
		S5StateProxyNoSupportedAuth,
		S5StateProxyAuthRequired,
		S5StateProxyAuthFailed,
		S5StateError
	};
	class Proxy
	{
	public:
		CAtlString SvrIp;	//��������ַ
		UINT  unType;		//��������0-sock4,1-sock5,2-http
		UINT  unPort;		//�˿�
		CAtlString name;	//�û�����
		CAtlString pass;	//����
		Proxy& operator=(const Proxy& s)
		{
			Proxy *pT = const_cast<Proxy*>(&s);
			this->SvrIp   = pT->SvrIp;
			this->unPort  = pT->unPort;
			this->unType  = pT->unType;
			this->name	  = pT->name;
			this->pass    = pT->pass;
			return (*this);
		}
	};
	ProxySock5(const Proxy& proxy);
	virtual ~ProxySock5();

	virtual DWORD onStart(JadeSocketIocp<> *socket,CheckAlive *pbase);
	virtual void  onRecv(PRECV pRecv,DWORD &dwID);
	virtual void  onSend(PSEND pSend,DWORD &dwID);
	virtual void  onConnect(PCONNECTEX pConnect,DWORD &dwID);
	virtual void  onDisconnect(DWORD &dwID);
	virtual bool  IsConnected(void);
	void	negotiate(void);
protected:
	bool		IsIp(const CAtlString& strV);
	CAtlString	State2String(Socks5State n);
	bool		SendIo(char *pBuf,UINT unLen);
	bool		SendUdpPack(int ip,u_short unPort);
private:
	CheckAlive			*m_pbase;
	Proxy				m_proxy;
	JadeSocketIocp<>	*m_socket;		//socket����
	Socks5State			m_state;
	UINT				m_unRetry;

	std::string			m_udpIp;		//����udp������Ҫ�ڽ�������һ��ͨ·��������
	std::string			m_udpPort;		//
	JadeSocket			*m_udpSock;		//����ʵ��udpͨ�ŵ�sock
};
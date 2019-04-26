#if !defined(__JADE_SOCKET_IOCP_H_)
#define __JADE_SOCKET_IOCP_H_

#include <Winsock2.h>
#include <mswsock.h>
#include <Ws2tcpip.h>
#include <vector>
#include <algorithm>
#if !defined(BASE_CODE)
	#if ( _WIN32_WINNT >= 0x0500 )
		#define BASE_CODE	(16000L)	//����Windows2000�Ժ�ϵͳ����������Ϣ��16000֮��ʼ
	#else
		#define BASE_CODE	(6010L)		//����Windows2000֮ǰϵͳ����������Ϣ��6010֮��ʼ
	#endif
#endif

#define ERROR_OOBDATA	(BASE_CODE+100)	//

#define	ADDRRESS_LENGTH	(sizeof(SOCKADDR_IN)+16)
/************************************************************************\
					�������ƣ�SOCKETKEY
					�������ܣ���ʾIO����Key����ֵ
					����������
						Input:
							DWORD	dwOperateType	��������
							LPVOID	lpValue			���ͽṹָ��
						Output��
						
					�� �� ֵ��
						��
\************************************************************************/
typedef struct _tgSocketKey
{
	OVERLAPPED	tgOverlap;
	INT			nOperateType;
	DWORD		dwID;			//����ǩ
	SOCKET		sock;
	LPVOID		lpValue;		//
}SOCKETKEY,*PSOCKETKEY;
/************************************************************************\
					�Y�����Q��AcceptEx
					�Y��������
							LPVOID		lpDataBuf		�������ݻ�����
							DWORD		dwBufLen		�������ݻ�����
\************************************************************************/
typedef struct _tgAcceptEx
{
	LPVOID		lpDataBuf;
	DWORD		dwBufLen;
	SOCKET		wSocket;
	BOOL		bInherit;
}ACCEPTEX,*PACCEPTEX;
/************************************************************************\
					�Y�����Q��AcceptExInfo
					�Y��������
						SOCKADDR	tgRemoteAddr	Զ�̵�ַ
						SOCKADDR	tgLocalAddr		���ص�ַ
\************************************************************************/
typedef struct _tgAcceptExInfo
{
	SOCKADDR_IN	tgRemoteAddr;
	SOCKADDR_IN	tgLocalAddr;
	char*		lpBuf;
	DWORD		dwBufLen;
	SOCKET		wSocket;
}ACCEPTEXINFO,*PACCEPTEXINFO;
/************************************************************************\
					�Y�����Q��ConnectEx
					�Y��������
						DWORD	dwBytesTranslate �Ѿ����͵����ݳ���							
\************************************************************************/
typedef struct _tgConnectEx
{
	DWORD	dwBytesTranslate;
}CONNECTEX,*PCONNECTEX,FAR *LPCONNECTEX;
/************************************************************************\
					�Y�����Q��Recv
					�Y��������
						PVOID	pBuf	ʵ�ʽ������ݻ�����
						DWORD	dwLen	ʵ�ʽ������ݳ���
\************************************************************************/
typedef struct _tgRecv
{
	char	*pBuf;
	DWORD	dwLen;
	SOCKET  sock;
}RECV,*PRECV,FAR *LPRECV;
/************************************************************************\
					�Y�����Q��Send
					�Y��������
						DWORD	dwNumBytes	ʵ�ʷ������ݳ���
\************************************************************************/
typedef struct _tgSend
{
	DWORD	dwNumBytes;		//�ѷ����ֽ���
	char	*pBuf;			//���ͻ�����
	DWORD	dwBufLen;		//���ͻ���������
	DWORD	dwPos;			//��ǰ�α�λ��
	SOCKET  sock;
}SEND,*PSEND,FAR *LPSEND;
/************************************************************************\
					�Y�����Q��RecvFrom
					�Y��������
						PVOID			pBuf		�����������ݻ�����
						DWORD			dwLen		�������ݳ���
						SOCKADDR_IN		tgAddress	������Դ
\************************************************************************/
typedef struct _tgRecvFrom
{
	char*			pBuf;
	DWORD			dwLen;
	SOCKADDR_IN		tgAddress;
}RECVFROM,*PRECVFROM,FAR *LPRECVFROM;
/************************************************************************\
					�Y�����Q��SendTo
					�Y��������
						DWORD	dwNumBytes	ʵ�ʷ������ݳ���
\************************************************************************/
typedef struct _tgSendTo
{
	SOCKADDR_IN	tgAddress;
	DWORD		dwNumBytes;
	char		*pBuf;			//���ͻ�����
	DWORD		dwBufLen;		//���ͻ���������
	DWORD		dwPos;			//��ǰ�α�λ��
}SENDTO,*PSENDTO,FAR *LPSENDTO;

template < typename BlockX = int >
class JadeSocketIocp
{
public:
	JadeSocketIocp(void);
	JadeSocketIocp(UINT unlow,UINT unHigh,BOOL bAutoClean);
	virtual ~JadeSocketIocp(void);
	/************************************************************************\
								�������Ͷ���			
	\************************************************************************/
	static	const UINT	SOCKET_ACCEPTEX		= 0;
	static	const UINT	SOCKET_SENDTO		= 1;
	static	const UINT	SOCKET_RECVFROM		= 2;
	static	const UINT	SOCKET_SEND			= 3;
	static	const UINT	SOCKET_RECV			= 4;
	static	const UINT	SOCKET_CONNECTEX	= 5;
	static	const UINT	SOCKET_DISCONNECTEX	= 6;
	static	const UINT	UNKNOWN_USERDATA	= 7;
	DWORD	m_dwKey;
	SOCKET	m_socket;					//Socket
public:
	int		Startup(WORD wVersion,WSADATA* lpWsaData,BOOL bAutoClean = TRUE);
	
	DWORD	InitResource(void);

	DWORD	GetSockName(char* rSocketAddress,UINT& rSocketPort,UINT &unAdrSize);
	
	DWORD	GetSockName(SOCKADDR* lpSockAddr,int* lpSockAddrLen );

	DWORD	GetSockOpt(int nOptionName,void* lpOptionValue,int* lpOptionLen,int nLevel = SOL_SOCKET);
	
	DWORD	SetSockOpt(int nOptionName,const void* lpOptionValue,int nOptionLen,int nLevel = SOL_SOCKET);

	DWORD	Create(UINT nSocketPort = 0,const char* lpszSocketAddress = NULL,int nSocketType = SOCK_STREAM,
					DWORD	dwFlags = WSA_FLAG_OVERLAPPED,LPWSAPROTOCOL_INFO lpProtocolInfo = NULL,
					BOOL bBind = TRUE,UINT unAf = AF_INET,UINT unProtocol = IPPROTO_IP);

	DWORD	CreateEx(SOCKADDR_IN *pSockAddr,DWORD dwSockAddrLen,LPWSAPROTOCOL_INFO lpProtocolInfo
					,int nSocketType = SOCK_STREAM,DWORD dwFlags = WSA_FLAG_OVERLAPPED
					,BOOL bBind = TRUE,UINT unAf = AF_INET,UINT unProtocol = IPPROTO_IP);

	DWORD	Listen(int nBacklog = SOMAXCONN);
	
	DWORD	Accept(IN DWORD dwID,IN DWORD dwDataBufLen,IN BOOL bInherit = TRUE,OUT DWORD *dwIdentify = NULL);
	DWORD	AcceptEx(IN DWORD dwID,IN SOCKET ConnectedSocket,IN DWORD dwDataBufLen,IN BOOL bInherit = TRUE,OUT DWORD *dwIdentify = NULL);

	DWORD	ConnectEx(DWORD dwID,const SOCKADDR *cpName,PVOID pSendBuffer,DWORD dwSendDataLen,DWORD *dwIdentify);

	DWORD	ConnectEx(DWORD dwID,const char* lpszHostAddress,UINT nHostPort,PVOID pSendBuffer,DWORD dwSendDataLen,
					DWORD *dwIdentify = NULL,UINT unAf = AF_INET);
 
	DWORD ClientSend(char* pBuf,DWORD dwBufLen,DWORD dwPos,DWORD dwFlags,DWORD dwID,DWORD *dwIdentify)
	{
		return Send(pBuf,dwBufLen,dwPos,dwFlags,dwID,dwIdentify,m_socket);
	}
	DWORD ClientReceive(DWORD dwBufLen,DWORD &dwFlags,DWORD dwID,DWORD *dwIdentify)
	{
		return Receive(dwBufLen,dwFlags,dwIdentify,dwID,dwIdentify,m_socket);
	}
	DWORD ClientDisconnect(DWORD dwFlags,DWORD dwID,DWORD *dwIdentify)
	{
		return Disconnect(dwFlags,dwID,dwIdentify,m_socket);
	}
	DWORD ClientIsConnect(long lOvertime)
	{
		return IsConnect(lOvertime,m_socket);
	}
	DWORD ClientPeekInputQueue(char* const pBuf,DWORD dwBufLen,DWORD &dwRecvLen,long lOvertime)
	{
		return PeekInputQueue(pBuf,dwBufLen,dwRecvLen,lOvertime,m_socket);
	}
	DWORD Send(char* pBuf,DWORD dwBufLen,DWORD dwPos,DWORD dwFlags,DWORD dwID,DWORD *dwIdentify,SOCKET s);
	DWORD Receive(DWORD dwBufLen,DWORD &dwFlags,DWORD dwID,DWORD *dwIdentify,SOCKET s);
	DWORD Disconnect(DWORD dwFlags,DWORD dwID,DWORD *dwIdentify,SOCKET s);
	DWORD IsConnect(long lOvertime,SOCKET s);
	DWORD PeekInputQueue(char* const pBuf,DWORD dwBufLen,DWORD &dwRecvLen,long lOvertime,SOCKET s);
	SOCKET indexSock(int i);

	DWORD	ReceiveFrom(DWORD dwID,DWORD dwLength,DWORD &dwFlags,const char* szAddress,int nPort,DWORD *dwIdentify);
	DWORD	ReceiveFrom(DWORD dwID,DWORD dwLength,DWORD &dwFlags,SOCKADDR *lpFrom,int &nFromLen,DWORD *dwIdentify);

	DWORD	SendTo(DWORD dwID,char *pBuffer,DWORD dwLength,DWORD dwPos,DWORD dwFlags,
				   SOCKADDR *lpTo,int nToLen,DWORD *dwIdentify);

	DWORD	SendTo(DWORD dwID,char *pBuffer,DWORD dwLength,DWORD dwPos,DWORD dwFlags,
				   const char* lpAddress,int nPort,short nAf);

	DWORD	CloseRemote(DWORD dwID,DWORD idx,DWORD dwFlags,DWORD Reserved,DWORD *dwIdentify);

	DWORD	WaitSocketComplete(DWORD &dwID,UINT &nOperateType,PVOID *lpOptVal,DWORD dwMilliseconds);

	void	ParserAcceptEx(HANDLE hHeap,PACCEPTEX lpData,PACCEPTEXINFO pInfo);

	char *	Ipv4AddrToString(SOCKADDR_IN *SockAddr);
	
	DWORD	Attach(SOCKET tgSock);

	DWORD	FreeMemory(PVOID lpData,UINT nOperateType);

	DWORD	Release(DWORD dwExitCode);

	DWORD	Associate(HANDLE hIocp,DWORD dwKey);
	DWORD	Associate(SOCKET nSock);

	DWORD	PostUserDefined(DWORD dwID,const BlockX &pUserX,DWORD *dwIdentify);

	DWORD	IsComplete(DWORD dwIdentify);
	
	DWORD	RecvDisconnect(LPCTSTR lpBuf,DWORD dwBufLen);

	DWORD	CancelAll(void);

	DWORD	CancelPending(DWORD dwIdentify);

	DWORD	ReleaseAll(void);
protected:
	DWORD Associate(HANDLE hIocp,SOCKET nSock);
	DWORD OnFailedFreeMemory(PVOID lpData,UINT nOperateType);
	void  closeAllSock();
	void  pushSock(SOCKET s);
	void  popSock(SOCKET s);
protected:
	virtual DWORD OnAcceptEx(HANDLE hHeap,PACCEPTEX pAcceptEx,PACCEPTEXINFO *lpInfo,DWORD dwNumOfBytesTransfer);
	virtual DWORD OnConnectEx(HANDLE hHeap,PCONNECTEX pConnectEx,DWORD dwNumOfBytesTransfer);
private:
	LPFN_ACCEPTEX				m_lpfnAcceptEx;				//AcceptEx Function Pointer
	LPFN_GETACCEPTEXSOCKADDRS	m_lpfnGetAcceptExSockaddrs;	//GetAcceptExSockaddrs Function Pointer
	LPFN_CONNECTEX				m_lpfnConnectEx;			//ConnectEx Function Pointer
	LPFN_DISCONNECTEX			m_lpfnDisconnectEx;			//DisconnectEx Function Pointer
	HANDLE						m_hIocp;					//Socket io complete Handle
	BOOL						m_bAutoClean;				//Auto Clean flag
	HANDLE						m_hHeap;					//Memory Heap
	int m_sockType;
	
	std::vector<SOCKET>  m_rcvSocks; //remote socket
	CRITICAL_SECTION  m_lkRcvSocks;
};

#include "JadeSocketIocp.inl"

#endif /* __JADE_SOCKET_IOCP_H_ */
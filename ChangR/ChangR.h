#pragma once

//����ӿ�
#include "cryp_ctrl.tlh"

class ChangRuan
{
public:
	typedef enum{
		BEIJING,	//����
		TIANJIN,	//���
		HEBEI,		//�ӱ�
		SANXI,		//ɽ��
		NEIMENG,	//���ɹ�
		LIAONING,	//����
		DALIAN,		//����
		JILIN,		//����
		HEILJ,		//������
		SHANGHAI,	//�Ϻ�
		JIANGSHU,	//����
		ZHEJIANG,	//�㽭
		NINGBO,		//����
		ANHUI,		//����
		FUJIAN,		//����
		XIAMEN,		//����
		GUIZHOU,	//����
		YUNNAN,		//����
		XIZANG,		//����
		SHANXI,		//����
		GANSU,		//����
		QINGHAI,	//�ຣ
		NINGXIA,	//����
		XINJIANG,	//�½�
		JIANGXI,	//����
		SANDONG,	//ɽ��
		QINGDAO,	//�ൺ
		HENAN,		//����
		HUBEI,		//����
		HUNAN,		//����
		GUANGDONG,	//�㶫
		SHENZHEN,	//����
		GUANGXI,	//����
		HAINAN,		//����
		CHONGQING,	//����
		SHICHUAN	//�Ĵ�
	}AREA;
	class Pager
	{
	public:
		Pager()
		{
			nMax= _T("50");
			page=_T("0");
		}
		CAtlString nMax;
		CAtlString page;	//��ǰҳ��
	};
	class Query : public Pager
	{
	public:
		Query()
		{
			rz = RZ_ALL;
			
		}
		typedef enum{
			RZ_YES,
			RZ_NO,
			RZ_ALL
		}ZTRZ;
		ZTRZ rz;	//��֤
		CAtlString ksrq;	//��ʼ����
		CAtlString jsrq;	//��������
	};
	//��ѡ
	class Gx
	{
	public:
		//��ѡ״̬
		typedef enum{
			GX_YES,
			GX_NO
		}ZT;
		Gx()
		{
			zt = GX_NO;
		}
		CAtlString dm;
		CAtlString hm;
		CAtlString kprq;
		ZT zt;
	};
	class Rz : public Pager
	{
	public:
		typedef enum{
			RZ_GX,	//�ѹ�ѡδȷ��
			RZ_QR	//�Ѿ�ȷ��
		}RZZT;
		Rz()
		{
			zt = RZ_GX;
		}
		RZZT zt;	//��֤״̬
	};
	//ͳ�Ʋ�ѯ
	class Tj : public Pager
	{
	public:
		CAtlString tjyf;	//ֵ��201802
		CAtlString fpdm;	//null
		CAtlString fphm;	//null
		CAtlString xfsbh;	//null
		CAtlString qrrzrq_q;	//ȷ����֤������
		CAtlString qrrzrq_z;	//ȷ����֤����ֹ
	};
	class Log
	{
	public:
		virtual ~Log(){	}
		virtual void Release(){	 delete this; }		
		virtual void OnHttpTrace(const std::stringstream& ss)=0;
		virtual void OnOperator(const std::string& method){ }
	};
	typedef struct __mm
	{
		std::string ts;
		std::string pubkey;
	}MM;

	ChangRuan();
	virtual ~ChangRuan();
	bool CheckEnv();
	void SetLog(Log *log);
	bool IsInitAndPwd();
	void SetPwd(const CAtlString& pwd);
	void SetArea(AREA area);
	bool Init();
	bool CheckPwd(const CAtlString& pwd);
	bool ReInitAndPwd();
	bool ReLogin();
	bool Login(AREA area);
	bool Quit();
	CAtlString GetUserInfo();
	//��ѯ��Ʊ��Ϣ
	bool GetFpFromGx(const Query& q,std::string &out);
	//������ݻ�ȡ��֤��Ϣ
	bool GetRzTjByNf(const CAtlString& nf,std::string &out);
	//��������·ݻ�ȡ�ֿ�ͳ��
	//date��ʽ"201702"
	bool GetDkTjByDate(const CAtlString& date,std::string &out);
	//����˰�������ڲ�ѯ
	bool GetQrGxBySsq(const CAtlString& ssq,std::string &out);
	//ȷ�Ϲ�ѡ
	bool ConfirmGx(std::string &out);
	bool ConfirmGxEnd();

	//���湴ѡ״̬
	bool SubmitGx(const std::vector<Gx>& gx);
	//��ѯ��ѡ��֤��Ʊ
	bool QueryRzfp(const Rz& rz,std::string& out);
	//ͳ�Ʋ�ѯ
	bool QueryDkcx(const Tj& tj,std::string& out);
	//��Ʊȷ�ϻ��ܲ�ѯ
	bool QueryQrHzFp(const CAtlString& ssq,std::string& out);
	//��ȡ��ҵ��Ϣ
	bool QueryQy(std::string& out);	
	//
	const CAtlString& GetLastMsg();
	void CopyData(const ChangRuan& cr);
	void Release();
	void EnableAutoQuit(bool b);
	bool QueryQrgx();

	const CAtlString& GetTaxNo() const	{
		return m_tax;
	}
	const std::string& GetDqrq() const {
		return m_dqrq;
	}
	const CAtlString& GetLjrzs() const {
		return m_ljrzs;
	}
	const std::string& GetToken() const {
		return m_token;
	}
protected:
	bool SecondLogin();
	bool ThirdLogin(const std::string& ts,const std::string& pubkey);
	bool SecondConfirmGx(std::string &out);
	bool ThirdConfirmGx(const std::string& p1,const std::string& p2);
	MM QueryPubKey(const char *p="checkInvConf");
	void BlankMM();

	CAtlString MakeClientAuthCode(const CAtlString& svrPacket);
	CAtlString MakeClientHello();
	bool OpenDev();
	bool CloseDev();
	CAtlString GetTickCount();
	bool QueryHqssq();
	std::string MakeCookie();
public:
	CAtlString   m_ip;
	CAtlString   m_pwd;
	CAtlString   m_authCode;
	//ͨ��QueryQrgx�ӿڻ�ȡ
	CAtlString   m_ljrzs;	//�ۼ���֤����
	//
	CAtlString	 m_nsrmc;
	CAtlString   m_svrPacket;
	CAtlString   m_svrRandom;
	CAtlString	 m_tax;
	//����ȷ�Ϲ�ѡ
	//
	AREA m_area;
private:
	Log	*m_log;
	CAtlString   m_lastMsg;
	std::string	 m_response;

	CComPtr<_CryptCtl>  crypCtrl;

	//��ѡȷ�Ϻ󷵻ص�����
	std::string  m_qrgxData;

	bool m_atuoQuit;
	bool m_hasInitPwd;
	std::string m_ljhzxxfs;
	std::string m_signature;		
	std::string m_cookie;
	std::string m_gxrqfw;	//��ѡ��Χ
	std::string m_skssq;
	std::string m_token;
	std::string	m_dqrq;		//��ǰʱ��
	std::string m_Ymbb;
};
//��ѡƽ̨
namespace GxPt
{
	//�û���Ϣ
	class Usr
	{
	public:
		CAtlString level; //�û�����
		CAtlString qylx;  //��ҵ����
		CAtlString qymc;  //��ҵ����
		CAtlString sbzq;  //�걨����
		CAtlString qysh;  //��ҵ˰��
		CAtlString oldsh; //��˰��
		CAtlString nsdq;  //��˰����
	};
	//��ѡ��֤����
	class RzGx
	{
	public:
		class Zu
		{
		public:
			CAtlString label;	//��ǩ
			CAtlString sl;		//����
			CAtlString se;		//˰��
			CAtlString je;		//���
		};
		void push(const RzGx::Zu& zu)
		{
			fnZu.push_back(zu);
		}
		CAtlString ssq;			//������
		CAtlString qrgxsl;		//�ڼ���ȷ�Ϲ�ѡ
		CAtlString bcqrfpsl;	//��ѡ��Ʊ��
		CAtlString bcyxgxsl;	//��Ч��ѡ��Ʊ
		CAtlString bcqrgxqrz;	//��ѡ��ɨ����֤��Ʊ
		CAtlString bcqrgxbkdk;	//��ѡ���ɵֿ۷�Ʊ
		std::vector<Zu>	fnZu;	//��ֵ˰ר�÷�Ʊ,��������Ʊ,���˷�Ʊ ��		
	};

	//�ֿ�ͳ������
	class DkTj
	{
	public:
		class Zu
		{
		public:
			CAtlString label;	//��ǩ
			CAtlString sl;		//����
			CAtlString se;		//˰��
			CAtlString je;		//���
		};
		void push(const Zu& zu){
			fnZu.push_back(zu);
		}
		std::vector<Zu>	fnZu;	//��ѡ��ɨ�裬�ϼ�
		CAtlString label;		//��ǩ
	};
	typedef std::vector<DkTj> DkTjs; //�ֿ�ͳ������
	//��ҳ��֤ͳ������
	class RzTj
	{
	public:
		CAtlString tm;		//ʱ��
		CAtlString zhangs;	//��Ʊ����
		CAtlString sehj;	//˰��ϼ�
		CAtlString zt;		//״̬ 1-���걨 0-��ǰ������ 2-δ�걨
		void clear(){
			tm.Empty();
			zhangs.Empty();
			sehj.Empty();
		}
	};
	class Ssq
	{
	public:
		CAtlString curSSq;	//��ǰ������
		CAtlString curJzRq;	//�����ڽ�������
	};
	class ValidDate
	{
	public:
		CAtlString begin;	//��Чʱ�俪ʼ
		CAtlString end;		//��Чʱ�����
	};
	//ȷ�ϻ�������
	class QrHzFp
	{
	public:
		static const TCHAR* ZP(){
			return _T("��ֵ˰ר�÷�Ʊ");
		}
		static const TCHAR* JDC(){
			return _T("��������Ʊ");
		}
		static const TCHAR* HY(){
			return _T("���˷�Ʊ");
		}
		static const TCHAR* TX(){
			return _T("ͨ�зѷ�Ʊ");
		}
		static const TCHAR* HJ(){
			return _T("�ϼ�");
		}
		class Hz
		{
		public:
			CAtlString column;
			CAtlString sl;	//����
			CAtlString je;	//���
			CAtlString se;	//˰��
		};
		typedef std::map<CAtlString,Hz> HashHz;	//��������
		class Qr
		{
		public:
			CAtlString cs;	//�ڼ���ȷ��
			CAtlString tm;	//ȷ��ʱ��
			HashHz curGxTj;	//������Ч��ѡͳ��
			HashHz countGxTj;  //�ۼ���Ч��ѡͳ��
			inline void pushCurGxTj(const Hz& hz){
				curGxTj.insert( std::make_pair(hz.column,hz) );
			}
			inline void pushCountGxTj(const Hz& hz){
				countGxTj.insert( std::make_pair(hz.column,hz) );
			}
			Hz getHzByKey(const TCHAR *key){
				Hz ret;
				HashHz::iterator it = curGxTj.find(key);
				if(it != curGxTj.end())
					ret = it->second;
				return ret;
			}
		};
		inline void push(const Qr& qr){
			qrs.push_back(qr);
		}		
	public:
		std::vector<Qr> qrs;
	
	};
	//
	struct SortDesc
	{
		bool operator()(const CAtlString& k1,const CAtlString& k2)const
		{
			return k1>k2;
		}
	};
	typedef std::map<CAtlString,RzTj,SortDesc> RzTjs;
	//����GetRzTjByNf ���ص�key3ֵ
	void HandleRzTjByNf(const std::string& json,RzTjs &out,Ssq &ssq,ValidDate& validDate);
	//����QueryQrHzFp ���ص�key2ֵ
	void HandleQrHzFp(const std::string& key2,QrHzFp &out);
	//����ֿ�ͳ������
	void HandleDkTj(const std::string& key2,DkTjs &out);
	//
	void HandleFirstConfirm(const std::string& json,RzGx &cur,RzGx &dq);
	//
	void HandleQy(const std::string& json,const std::string& token,Usr &out);
	//
	size_t SplitBy(const std::string& src,char delim,std::vector<std::string> &ret);
	
	class Encrypt
	{
	public:
		static Encrypt* Get()
		{
			static Encrypt encrypt;
			return &encrypt;
		}
		void AddJsCode(HMODULE hInst,LPCTSTR sType,LPCTSTR sName);
		void AddJsCode(const CAtlString& pathFile);
		std::string checkTaxno(const std::string& a,const std::string& b,const std::string& c,const std::string& d,const std::string& e);
		std::string checkOneInv(const std::string& a,const std::string& b,const std::string& c,const std::string& d,const std::string& e,const std::string& f,const std::string& g);
		std::string checkInvConf(const std::string& a,const std::string& b,const std::string& c,const std::string& d,const std::string& e);
		std::string checkDeduDown(const std::string& a,const std::string& b,const std::string& c,const std::string& d,const std::string& e,const std::string& f);
		std::string BuildCall(const char *fun,const char *p1,...);
	private:
		Encrypt(){}
		virtual ~Encrypt(){}
	private:
		typedef std::map<CAtlString,std::string> FileMapCode;
		FileMapCode jsCode;
	};
}
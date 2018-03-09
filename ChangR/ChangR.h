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
			nMax= _T("500");
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
		CAtlString tjyf;
		CAtlString fpdm;
		CAtlString fphm;
		CAtlString xfsbh;
		CAtlString qrrzrq_q;
		CAtlString qrrzrq_z;
	};
	class Log
	{
	public:
		virtual ~Log(){	}
		virtual void Release(){	 delete this; }		
		virtual void OnHttpTrace(const std::stringstream& ss)=0;
		virtual void OnOperator(const std::string& method){ }
	};

	ChangRuan();
	virtual ~ChangRuan();
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
	bool ConfirmGx();
	//���湴ѡ״̬
	bool SubmitGx(const std::vector<Gx>& gx);
	//��ѯ��ѡ��֤��Ʊ
	bool QueryRzfp(const Rz& rz,std::string& out);
	//ͳ�Ʋ�ѯ
	bool QueryDkcx(const Tj& tj,std::string& out);

	const CAtlString& GetLastMsg();
	void CopyData(const ChangRuan& cr);
	void Release();
	void EnableAutoQuit(bool b);
	bool QueryQrgx();

	const CAtlString& GetTaxNo() const	{
		return m_tax;
	}
	const CAtlString& GetDqrq() const {
		return m_dqrq;
	}
	const CAtlString& GetLjrzs() const {
		return m_ljrzs;
	}
protected:
	bool SecondLogin();
	bool SecondConfirmGx();
	bool ThirdConfirmGx(const std::string& p1,const std::string& p2);

	CAtlString MakeClientAuthCode(const CAtlString& svrPacket);
	CAtlString MakeClientHello();
	bool OpenDev();
	bool CloseDev();
	CAtlString GetTickCount();
	bool BeforeConfirmGx();
public:
	CAtlString   m_token;
	CAtlString   m_ip;
	CAtlString   m_pwd;
	CAtlString   m_authCode;
	//ͨ��QueryQrgx�ӿڻ�ȡ
	CAtlString   m_ljrzs;	//�ۼ���֤����
	CAtlString	 m_dqrq;	//��ǰʱ��
	//
	CAtlString   m_Ymbb;
	CAtlString	 m_nsrmc;
	CAtlString   m_svrPacket;
	CAtlString   m_svrRandom;
	CAtlString   m_skssq;
	CAtlString	 m_tax;
	//����ȷ�Ϲ�ѡ
	CAtlString   m_cookssq;	//��ѡ������
	CAtlString   m_gxrqfw;	//��ѡ��Χ
	//
	AREA m_area;
private:
	Log	*m_log;
	CAtlString   m_lastMsg;

	CComPtr<_CryptCtl>  crypCtrl;
	
	//��ѡȷ�Ϻ󷵻ص�����
	std::string  m_qrgxData;

	bool m_atuoQuit;
	bool m_hasInitPwd;
};
//��ѡƽ̨
namespace GxPt
{
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
	struct SortDesc
	{
		bool operator()(const CAtlString& k1,const CAtlString& k2)const
		{
			return k1>k2;
		}
	};
	typedef std::map<CAtlString,RzTj,SortDesc> RzTjs;
	//����GetRzTjByNf ���ص�key3ֵ
	void HandleRzTjByNf(const std::string& key3,RzTjs &out);

	size_t SplitBy(const std::string& src,char delim,std::vector<std::string> &ret);
}
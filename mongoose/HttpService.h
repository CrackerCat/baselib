#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace HttpService {
	//���в���
	class ExternMG;
	class RunParam
	{
	public:
		RunParam();
		~RunParam();
		ExternMG* GetMG();
		void CloseMG();
 		void SetPort(CAtlString port);
		void SetSSL(CAtlString name,CAtlString pem,CAtlString key,CAtlString cert);
		void Stop();
	public:
		HANDLE	evReady;	//������������ɣ�֪ͨ���������¼�
		HANDLE  evRun;		//���б��
		HANDLE  mThread;	//����
		bool    mStop;		//�˳��������
		ExternMG *mMG;
 	};

	typedef std::map<std::string,std::string> PARAMS;
	struct ReqHead
	{
		std::string type;
		std::string charset;
		std::string method;
		std::string accept;
		std::string origin;
		PARAMS	xValue;
	};
	struct ReqParam
	{
		PARAMS params;
		ReqHead head;
		std::string body;
		inline std::string GetParamVal(const std::string& key){
			std::string blank;
			PARAMS::iterator it = params.find(key);
			if(it==params.end())
				return blank;
			return it->second;
		}
	};
	struct Result
	{
		bool bNext;	//�Ƿ�ִ����һ�������߼����磺OnRequest
		std::string rpType;	//���ر��ĸ�ʽ
		std::string response; //���ر���
	};

	//uri �����
	std::string UriEncode(const std::string& str);
	std::string UriDecode(const std::string& str);

	class Handler
	{
	public:
		virtual ~Handler(){	}
		virtual void OnInit(RunParam *runParam){ }
		virtual Result OnHeader(const std::string& uri,const ReqHead& type)
		{
			Result ret;
			ret.bNext = true;
			return ret;
		}
		virtual Result OnRequest(const std::string& uri,const ReqParam& param)=0;
		virtual void OnIdle(){ };
	};
	class Service
	{
	public:
		bool BindHander(Handler *handler);
		bool Run(void);
		void Stop();
		RunParam gRunParam;
	};
};
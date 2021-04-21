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
	};
	struct ReqParam
	{
		PARAMS params;
		ReqHead head;
		std::string body;
	};
	struct Result
	{
		bool bNext;	//�Ƿ�ִ����һ�������߼����磺OnRequest
		std::string type;	//���ر��ĸ�ʽ
		std::string response; //���ر���
	};

	class Handler
	{
	public:
		virtual ~Handler(){	}
		virtual Result OnHeader(const std::string& uri,const ReqHead& type)
		{
			Result ret;
			ret.bNext = true;
			return ret;
		}
		virtual Result OnRequest(const std::string& uri,const ReqParam& param)=0;
		virtual void OnIdle(){ };
	};
	bool BindHander(Handler *handler);
	bool Run(void);

	extern RunParam gRunParam;
};
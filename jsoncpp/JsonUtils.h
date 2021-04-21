#pragma once
#include "inc/json.h"
#include <sstream>
namespace JsonUtils{
	/**
	json��ʽ�����ַ���

	@author jiayh

	@inParam doc json����

	@outParam ��

	@throw ��

	@return std::string ���

	@since 1.0
	**/
	std::string FmtJson(const Json::Value &doc);
	/**
	����json�ַ�����json����

	@author jiayh

	@inParam v json�ַ���

	@outParam doc json����

	@throw ��

	@return bool true-�ɹ���false-ʧ��

	@since 1.0
	**/
	bool ParseJson(const std::string& v,Json::Value &doc);
	/**
	��ȫ��ȡjson������keyֵ

	@author jiayh

	@inParam doc json����
	@inParam k json����key�ؼ���

	@outParam ��

	@throw ��

	@return string keyֵ���ַ���

	@since 1.0
	**/
	std::string SafeJsonValue(const Json::Value &doc,const std::string& k);
	long SafeJsonValueINT(const Json::Value &doc,const std::string& k);
	bool SafeJsonValueBOOL(const Json::Value &doc,const std::string& k);
	std::string SafeJsonValueStr(const std::string &json,const std::string& k);
};
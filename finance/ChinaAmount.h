#pragma once

namespace ChinaAmount
{
	//rmb Сдת��д
	bool CapitalRMB(const CAtlString &inRmb,CAtlString &out);
	//����Сдת��д
	CAtlString CapitalNumber(const CAtlString &inNum);
	//��λת��ֵ
	UINT Unit2Number(const CAtlString& unit);
};
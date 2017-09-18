#include "stdafx.h"
#include "ChinaAmount.h"

namespace base
{
	bool ChinaAmount::CapitalRMB(const CAtlString &inRmb,CAtlString &out)
	{
		static TCHAR CAPITAL_NUM[]=_T("��Ҽ��������½��ƾ�");
		static TCHAR CAPITAL_UNIT[]=_T("�ֽ�Ԫʰ��Ǫ����");

		CAtlString integer;	//��������
		CAtlString decimal;	//С������
		int dot = inRmb.ReverseFind('.');
		if(inRmb.IsEmpty())
			return false;
		if(dot==-1 || dot==(inRmb.GetLength()-1) )
		{
			integer = inRmb;
		}
		else
		{
			integer = inRmb.Mid(0,dot);
			decimal = inRmb.Mid(dot+1);
		}
		//ȥ���������ֵ�����ұ�ʶ��
		integer.Remove('��');
		//����ת����Χ
		if(decimal.GetLength()>2)
			return false;
		if(integer.GetLength()>10)
			return false;
		//ȥ��С������β����0ֵ
		if( !decimal.IsEmpty() )
		{
			if(decimal==_T("00"))
				decimal.Empty();
			else if(decimal==_T("0"))
				decimal.Empty();
			else if( '0'==decimal[decimal.GetLength()-1] )
				decimal.Delete(decimal.GetLength()-1);
		}
		CAtlString tmp1,tmp2;
		int nUnit = 1;		//��λ����
		//����С������
		for(int nNum=0,nUnit=1;nNum<decimal.GetLength();nNum++,nUnit--)
		{
			int price = decimal[nNum] - _T('0');
			if(price==0)	continue;	//����0
			TCHAR capital = CAPITAL_NUM[price];
			TCHAR unit = CAPITAL_UNIT[nUnit];
			tmp2 += capital;
			tmp2 += unit;
		}
		for(int nNum=0,nUnit=2;nNum<integer.GetLength();nNum++)
		{
			CAtlString tmp;
			int price = integer[integer.GetLength()-nNum-1] - _T('0');
			if(price==0)	continue;	//����0
			TCHAR capital = CAPITAL_NUM[price];
			TCHAR unit;
			if(nNum<=5)		//�ڵ�λ
				unit = CAPITAL_UNIT[nUnit+nNum];
			else
			{	//�����ڱ�ʾ��Χ
				int nU = nNum-5;
				unit = CAPITAL_UNIT[nUnit+nU];
			}
			tmp += capital;
			tmp += unit;
			tmp1.Insert(0,tmp);
		}
		if( tmp1[tmp1.GetLength()-1]!=_T('Ԫ') )
			tmp1 += _T("Ԫ");
		tmp2 += _T("��");
		out = tmp1 + tmp2;
		return true;
	}
}
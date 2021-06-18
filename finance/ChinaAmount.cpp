#include "stdafx.h"
#include "ChinaAmount.h"

namespace base
{
	bool ChinaAmount::CapitalRMB(const CAtlString &inRmb,CAtlString &out)
	{
		static TCHAR CAPITAL_NUM[]=_T("��Ҽ��������½��ƾ�");
		//static TCHAR CAPITAL_UNIT[]=_T("�ֽ�Ԫʰ��Ǫ����");
		
		static TCHAR radices[] = _T(" ʰ��Ǫ");
		static TCHAR bigRadices[] = _T(" ����");
		static TCHAR decimals[] = _T("�ֽ�");

		CAtlString integer;	//��������
		CAtlString decimal;	//С������
		bool fu = false;
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
		if( integer[0]==_T('-') )
		{
			fu = true;
		}
		//ȥ���������ֵ�����ұ�ʶ��
		integer.Remove('��');
		integer.Remove(_T('-'));
		integer.Remove(_T('+'));
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
			TCHAR unit = decimals[nUnit];
			tmp2 += capital;
			tmp2 += unit;
		}
		long quotient=0,modulus=0;
		long zeroCount = 0;
		for(int nNum=0,nUnit=2;nNum<integer.GetLength();nNum++)
		{
			long p = integer.GetLength() - nNum - 1; 
			CAtlString d = integer.Mid(nNum, 1); 
			quotient = p / 4; 
			modulus  = p % 4; 
			if(d == _T("0"))
			{ 
				zeroCount++; 
			} 
			else
			{ 
				if (zeroCount > 0) 
				{ 
					tmp1 += CAPITAL_NUM[0]; 
				} 
				zeroCount = 0; 
				tmp1 += CAPITAL_NUM[ _ttoi(d) ];
				tmp1 += radices[modulus]; 
			} 
			if (modulus==0 && zeroCount < 4) 
			{ 
				tmp1 += bigRadices[quotient]; 
				zeroCount = 0; 
			} 			
		}
		tmp1 = tmp1.Trim();
		if(fu)
			tmp1.Insert(0,_T("��������"));
		if( tmp1[tmp1.GetLength()-1]!=_T('Ԫ') )
			tmp1 += _T("Ԫ");
		tmp2 += _T("��");
		out = tmp1 + tmp2;
		return true;
	}
}
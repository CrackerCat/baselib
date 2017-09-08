// calendar.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "calendar.h"

namespace base
{

	Calendar::Calendar() throw()
	{

	}
	Calendar::Calendar( const ATL::CTime & tm )
		:ATL::CTime(tm.GetTime())
	{

	}
	Calendar::Calendar( __time64_t time )
		: ATL::CTime(time)
	{

	}
	Calendar::Calendar( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST)
		: ATL::CTime(nYear,nMonth,nDay,nHour,nMin,nSec,nDST)
	{

	}
	Calendar::Calendar( WORD wDosDate, WORD wDosTime, int nDST)
		: ATL::CTime(wDosTime,wDosTime,nDST)
	{

	}
	Calendar::Calendar( const SYSTEMTIME& st, int nDST)
		: ATL::CTime(st,nDST)
	{

	}
	Calendar::Calendar( const FILETIME& ft, int nDST)
		: ATL::CTime(ft,nDST)
	{

	}
	int Calendar::GetMonthDays()
	{
		return Calendar::GetMonthDays(ATL::CTime::GetYear(), ATL::CTime::GetMonth());
	}
	int Calendar::GetMonthDays(int y,int m)
	{
		int nRet = 0;
		static UINT dayNum[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		if (2 == m)
		{
			nRet = (((0 == y % 4) && (0 != y % 100) || (0 == y % 400)) ? 29 : 28);
		}
		else
		{
			nRet = dayNum[m - 1];
		}
		return nRet;
	}
	int Calendar::DayOfWeek(int y,int m,int d)
	{
		//ע�⣺�ڹ�ʽ���и���������ʽ��ͬ�ĵط���
		//��һ�ºͶ��¿�������һ���ʮ���º�ʮ���£����������2004 - 1 - 10����ɣ�2003 - 13 - 10��
		//�Թ�ԪԪ��Ϊ�ο�����ԪԪ��1��1��Ϊ����һ
		//W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7 
		if (m == 1 || m == 2) {
			m += 12;
			y--;
		}
		int iWeek = (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
		iWeek += 1;	//0 -- ��һ��7 -- ������
		return iWeek;
	}
	Calendar& Calendar::operator=( const ATL::CTime& time )
	{
		ATL::CTime *superTM = static_cast<ATL::CTime*>(this);
		(*superTM) = time.GetTime();
		return (*this);
	}
	Calendar& Calendar::operator=( const Calendar& time)
	{
		ATL::CTime *superTM = static_cast<ATL::CTime*>(this);
		(*superTM) = time.GetTime();
		return (*this);
	}
	int Calendar::GetDayOfWeekByChina()
	{
		if( ATL::CTime::GetDayOfWeek()==1 )
			return 7;
		return (ATL::CTime::GetDayOfWeek()-1);
	}
	bool Calendar::IsValid()
	{
		return (GetTime()>0);
	}
	bool Calendar::PlusYear(int v)
	{
		int nSelV = GetYear()+v;
		if( nSelV < 1900 )
			return false;
		base::Calendar canlendarTmp(nSelV,GetMonth(),GetDay(),GetHour(),GetMinute(),GetSecond());
		*this= canlendarTmp;
		return true;
	}
	bool Calendar::PlusMonth(int v)
	{
		int nY = 0;	//��������
		int nM = 0;	//��������
		if(v>=12)
		{
			nY = v/12;
			nM = v%12;
		}
		else
			nM = v;
		int nSelV = GetMonth()+nM;
		if( nSelV<=0 )
		{
			nY -= 1;
			nSelV = 12 + nSelV;
		}
		else if( nSelV>12 )
		{
			nY += 1;
			nSelV -= 12;
		}
		if( (GetYear()+nY) < 1900 )
			return false;
		base::Calendar canlendarTmp(GetYear()+nY,nSelV,GetDay(),GetHour(),GetMinute(),GetSecond());
		*this = canlendarTmp;
		return true;
	}
}

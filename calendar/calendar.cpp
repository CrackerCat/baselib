// calendar.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "calendar.h"
#include <sys/timeb.h>
#include <atlcomtime.h>

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
	time_t Calendar::CurrentTimeLarge()
	{
		long long time_last;    
		time_last = time(NULL);     //������  
		struct __timeb64 t1;    
		_ftime(&t1);
		return (t1.time * 1000 + t1.millitm);
	}
	bool Calendar::MinusDays(int v)
	{
		*this -= ATL::CTimeSpan(v,0,0,0);
		return true;
	}
	bool Calendar::PlusMonth(int v)
	{
		int nowDay = GetDay();
		int nOldDays = GetMonthDays(GetYear(),GetMonth());
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
		int nNewDays = GetMonthDays(GetYear()+nY,nSelV);
		if(nowDay>nNewDays)
			nowDay = nNewDays;
		base::Calendar canlendarTmp(GetYear()+nY,nSelV,nowDay,GetHour(),GetMinute(),GetSecond());
		*this = canlendarTmp;
		return true;
	}
	bool Calendar::ParseDateTime(LPCTSTR lpszDate)
	{
		COleDateTime oleDt;
		if( oleDt.ParseDateTime(lpszDate) )
		{
			(*this) = ATL::CTime(oleDt.m_dt);
			return true;
		}
		return false;
	}
	SYSTEMTIME Calendar::ParseDateTime(const std::string& dt,const std::string& temp)
	{
		SYSTEMTIME sysTM;
		memset(&sysTM,0,sizeof(SYSTEMTIME));
		if(temp.size()==0)	return sysTM;
		std::string year;
		std::string month;
		std::string day;
		std::string hour;
		std::string minut;
		std::string second;	
		int npos = 0;
		for(size_t t=0;t<dt.size();t++)
		{
			char v = dt[t];
			if( t > (temp.length()-1) )	break;
			char f = temp.at(t);
			if(f=='y')
				year += v;
			else if(f=='M')
				month += v;
			else if(f=='m')
				minut += v;
			else if(f=='d')
				day += v;
			else if(f=='h')
				hour += v;
			else if(f=='s')
				second += v;
			if(v==f)
			{
				npos += 1;
				continue;
			}
		}
		if(year.size())
			sysTM.wYear = atol(year.c_str());
		if(month.size())
			sysTM.wMonth= atol(month.c_str());
		if(day.size())
			sysTM.wDay  = atol(day.c_str());
		if(hour.size())
			sysTM.wHour  = atol(hour.c_str());
		if(hour.size())
			sysTM.wMinute  = atol(minut.c_str());
		if(second.size())
			sysTM.wSecond = atol(second.c_str());
		if(sysTM.wDay==0)
			sysTM.wDay=1;
		return sysTM;
	}

}

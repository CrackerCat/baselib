// calendar.cpp : �������̨Ӧ�ó������ڵ㡣
//
#pragma once
#include <atltime.h>

namespace base{

	class Calendar : public ATL::CTime
	{
	public:
		Calendar() throw();
		Calendar( __time64_t time ) throw();
		Calendar( const ATL::CTime & tm ) throw();
		Calendar( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST = -1 );
		Calendar( WORD wDosDate, WORD wDosTime, int nDST = -1 );
		Calendar( const SYSTEMTIME& st, int nDST = -1 );
		Calendar( const FILETIME& ft, int nDST = -1 );
		//��ȡ�й�ϰ�ߵ����ڼ�
		//��һ 1 ���� ���� 7
		int GetDayOfWeekByChina();
		bool IsValid();
		Calendar& operator=( const ATL::CTime& time );
		Calendar& operator=( const Calendar& time);
		bool PlusYear(int v);
		bool PlusMonth(int v);
		bool MinusDays(int v);
		//��ȡ��ǰʱ�侫ȷ������
		static time_t CurrentTimeLarge();
		bool ParseDateTime(LPCTSTR lpszDate);
		//dt - ����������
		//temp - ����ģ�� ��ѡֵ��
		//				yyyy-MM-dd hh:mm:ss
		//				yy-MM-dd hh:mm:ss
		//				hh:mm:ss
		//				yyyy-MM-dd
		//				yy-MM-dd
		//				yyyyMMdd
		//	
		static SYSTEMTIME ParseDateTime(const std::string& dt,const std::string& temp);
	public:
		//��ȡ��ǰ������
		int GetMonthDays();
		//��ȡĳ��ĳ������
		static int GetMonthDays(int y,int m);
		//���ڻ�ķ����ɭ���㹫ʽ
		//@param y ��� �磺1990��
		//@param m �·� �磺1��
		//@param d ���� �磺20��
		static int DayOfWeek(int y,int m,int d);
	};
}
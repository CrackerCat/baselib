#pragma once

#include "../base/MacroX.h"
namespace CustomUI
{
	class EditUI : public DuiLib::CEditUI 
	{
	public:
		EditUI(void);
		~EditUI(void);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetBkTextColor(DWORD dwColor);
		void SetBkFont(int nI);
		void SetBkTextPadding(RECT rc);
		void SetText(LPCTSTR pstrValue);
		void SetBkText(LPCTSTR pstrValue);
		virtual void Init();
		DUI_DECLARE(CustomUI::EditUI,DuiLib::CEditUI)
	protected:
		void DoEvent(DuiLib::TEventUI& event);
		void PaintText(HDC hDC);
		void PaintBorder(HDC hDC);
	protected:
		DuiLib::CDuiString m_bkText;//�����ı�
		DWORD m_bkTextColor;		//�����ı���ɫ
		RECT  m_bkTextPadding;		//�����ı�λ��
		int	  m_bkFont;				//�����ı�����
		UINT  m_bkTextStyle;		//�����ı���ʽ
		bool  m_needbktext;
	};
}

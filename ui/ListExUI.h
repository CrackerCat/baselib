#pragma once

#include "../Base/MacroX.h"
namespace CustomUI
{
	class ListUI : public DuiLib::CListUI
	{
	public:
		ListUI(void);
		//���� ���ȼ���listbody item������ʱ��û�м���Column����listbody item pos���㲻��ȷ
		void SetPos(RECT rc);
		void ScrollAt(DuiLib::CControlUI *item);
		DUI_DECLARE(CustomUI::ListUI,DuiLib::CListUI)
	protected:

	};
}
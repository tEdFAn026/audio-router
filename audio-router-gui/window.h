#pragma once

#include "dialog_main.h"
#include "formview.h"
#include "bootstrapper.h"
#include "licensing.h"
#include <memory>
#include <shellapi.h>
//#pragma comment(lib,"mousehook.lib")
//#include "..\mousehook\mousehook.h"

#define WIN_WIDTH 970//400
#define WIN_HEIGHT 670//360
#define GET(type, item) reinterpret_cast<type&>(this->GetDlgItem(item))
#define WM_SHOWTASK (WM_USER +1)

	class window : public CFrameWindowImpl<window>
	{
	private:
		bool dlg_main_b;
	protected:
		//CmousehookApp m_hook;
	public:
		dialog_main* dlg_main;
		formview* form_view;
		/*dialog_licensing* license;*/
		/*bootstrapper* bootstrap;*/

		explicit window(/*bootstrapper**/);
		~window();

		DECLARE_FRAME_WND_CLASS(L"Audio Router", IDR_MAINFRAME);

		BEGIN_MSG_MAP(window)
			MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
			MESSAGE_HANDLER(WM_SHOWTASK, OnShowTask)
			MSG_WM_CREATE(OnCreate)
			/*MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)*/
			CHAIN_MSG_MAP(CFrameWindowImpl<window>)
			COMMAND_ID_HANDLER(ID_FILE_REFRESHLIST, OnFileRefreshlist)
			COMMAND_ID_HANDLER(ID_ABOUT, OnAbout)
			COMMAND_ID_HANDLER(ID_FILE_SWITCHVIEW, OnFileSwitchview)
			COMMAND_ID_HANDLER(ID_MINIMIZE,OnMinimize)
			COMMAND_ID_HANDLER(ID_RESTORE,OnRestore)
			COMMAND_ID_HANDLER(ID_EXIT,OnExit)
			/*MSG_WM_NCHITTEST(OnNcHitTest)*/
		END_MSG_MAP()

		int OnCreate(LPCREATESTRUCT);
		NOTIFYICONDATA nid;
		LRESULT OnFileRefreshlist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnFileSwitchview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnMinimize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnRestore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnShowTask(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	};
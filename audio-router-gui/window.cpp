#include "window.h"

telemetry* telemetry_m = NULL;

window::window(/*bootstrapper* bootstrap*/) : dlg_main_b(true)/*, license(NULL)*//*, bootstrap(bootstrap)*/
{
    this->dlg_main = new dialog_main(*this);
    this->form_view = new formview(*this);
}

window::~window()
{
    if(this->dlg_main_b)
        delete this->dlg_main;
    delete this->form_view;

    delete telemetry_m;
    telemetry_m = NULL;
}

int window::OnCreate(LPCREATESTRUCT lpcs)
{
    telemetry_m = new telemetry;

    /*this->license = new dialog_licensing(*this);*/

    this->m_hWndClient = this->dlg_main->Create(this->m_hWnd);
    this->dlg_main->ShowWindow(SW_SHOW);

    return 0;
}

LRESULT window::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   
	if (wParam == SC_MINIMIZE)
	{
		for (dialog_main::dialog_arrays_t::iterator it = this->dlg_main->dialog_arrays.begin();
			it != this->dlg_main->dialog_arrays.end();
			it++)
		{
			for (dialog_array::dialog_controls_t::iterator jt = (*it)->dialog_controls.begin();
				jt != (*it)->dialog_controls.end();
				jt++)
			{
				(*jt)->set_display_name(false, true);
			}
		}

		this->ShowWindow(SW_HIDE);

		return 0;
	}
    else if(wParam == SC_RESTORE)
    {
        for(dialog_main::dialog_arrays_t::iterator it = this->dlg_main->dialog_arrays.begin();
            it != this->dlg_main->dialog_arrays.end();
            it++)
        {
            for(dialog_array::dialog_controls_t::iterator jt = (*it)->dialog_controls.begin();
                jt != (*it)->dialog_controls.end();
                jt++)
            {
                (*jt)->set_display_name(false, false);
            }
        }
    }

    bHandled = FALSE;
    return 0;
}

LRESULT window::OnFileRefreshlist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if(!this->dlg_main_b)
    {
        this->form_view->refresh_list();
    }
    return 0;
}

LRESULT window::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    this->MessageBoxW(
        L"Audio Router version 0.10.2.\n" \
        L"\nIf you come across any bugs(especially relating to routing or duplicating), " \
        L"or just have an idea for a new feature, " \
        L"please send a PM to the developer on reddit: reddit.com/user/audiorouterdev/\n" \
		L"\nAdd Hide in task bar by tEdFAn in this vision.", 
        L"About", MB_ICONINFORMATION);
    return 0;
}

LRESULT window::OnFileSwitchview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    RECT rc;
    this->GetClientRect(&rc);

    if(this->dlg_main_b)
    {
        this->dlg_main->DestroyWindow();
        delete this->dlg_main;

        this->m_hWndClient = this->form_view->Create(*this);
        //this->form_view->ShowWindow(SW_SHOW);
        this->form_view->SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_SHOWWINDOW);
    }
    else
    {
        this->form_view->DestroyWindow();

        this->dlg_main = new dialog_main(*this);
        this->m_hWndClient = this->dlg_main->Create(*this);
        //this->dlg_main->ShowWindow(SW_SHOW);
        this->dlg_main->SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_SHOWWINDOW);
    }

    this->dlg_main_b = !this->dlg_main_b;

    return 0;
}

LRESULT window::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return HTCLOSE;
}

LRESULT window::OnMinimize(WORD , WORD , HWND , BOOL& )
{
	for (dialog_main::dialog_arrays_t::iterator it = this->dlg_main->dialog_arrays.begin();
		it != this->dlg_main->dialog_arrays.end();
		it++)
	{
		for (dialog_array::dialog_controls_t::iterator jt = (*it)->dialog_controls.begin();
			jt != (*it)->dialog_controls.end();
			jt++)
		{
			(*jt)->set_display_name(false, true);
		}
	}
	
	this->ShowWindow(SW_HIDE);

	return 0;
}

LRESULT window::OnRestore(WORD, WORD, HWND, BOOL&)
{
	for (dialog_main::dialog_arrays_t::iterator it = this->dlg_main->dialog_arrays.begin();
		it != this->dlg_main->dialog_arrays.end();
		it++)
	{
		for (dialog_array::dialog_controls_t::iterator jt = (*it)->dialog_controls.begin();
			jt != (*it)->dialog_controls.end();
			jt++)
		{
			(*jt)->set_display_name(false, false);
		}
	}

	this->ShowWindow(SW_RESTORE);

	return 0;
}

LRESULT window::OnExit(WORD , WORD , HWND , BOOL& )
{
	PostMessage(WM_CLOSE, 0, 0);
	return 0;
}

LRESULT window::OnShowTask(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单
							   //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已
							   //隐藏），将程序结束。
		menu.AppendMenu(MF_STRING, ID_ABOUT, _T("About"));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_MINIMIZE, _T("Minimize"));
		menu.AppendMenu(MF_STRING, ID_RESTORE, _T("Restore"));
		menu.AppendMenu(MF_SEPARATOR);
		//menu.AppendMenu(MF_STRING, WM_CLOSE, _T("WM_CLOSE"));
		//menu.AppendMenu(MF_STRING, WM_DESTROY, _T("WM_DESTROY"));
		menu.AppendMenu(MF_STRING, ID_EXIT, _T("Exit"));
		//确定弹出式菜单的位置
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this->m_hWnd);
		//资源回收
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK://双击左键的处理
	{
		this->ShowWindow(SW_RESTORE);//简单的显示主窗口完事儿
		for (dialog_main::dialog_arrays_t::iterator it = this->dlg_main->dialog_arrays.begin();
			it != this->dlg_main->dialog_arrays.end();
			it++)
		{
			for (dialog_array::dialog_controls_t::iterator jt = (*it)->dialog_controls.begin();
				jt != (*it)->dialog_controls.end();
				jt++)
			{
				(*jt)->set_display_name(false, false);
			}
		}
	}
	break;
	}
	return 0;
}
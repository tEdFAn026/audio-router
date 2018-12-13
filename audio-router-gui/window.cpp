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

	//this->m_hWndClient = this->form_view->Create(this->m_hWnd);
	//this->form_view->ShowWindow(SW_HIDE);
	this->m_hWndClient = this->dlg_main->Create(this->m_hWnd);
	this->dlg_main->ShowWindow(SW_SHOW);
	//m_hook.Starthook(this->m_hWnd);
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
	else 
	{
		this->dlg_main->refresh_dialog_arrays();
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
		L"\nAdd Hide On Tray by tEdFAn in this vision.", 
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

		//this->dlg_main->ShowWindow(SW_HIDE);
        //this->form_view->ShowWindow(SW_SHOW);
		//this->form_view->refresh_list();

        this->form_view->SetWindowPos(NULL, &rc, SWP_NOZORDER | SWP_SHOWWINDOW);
    }
    else
    {
        this->form_view->DestroyWindow();
        this->dlg_main = new dialog_main(*this);
        this->m_hWndClient = this->dlg_main->Create(*this);

		//this->form_view->ShowWindow(SW_HIDE);
		//this->dlg_main->refresh_dialog_arrays();
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
	SetForegroundWindow(this->m_hWnd);

	return 0;
}

LRESULT window::OnExit(WORD , WORD , HWND , BOOL& )
{
	PostMessage(WM_CLOSE, 0, 0);
	return 0;
}

LRESULT window::OnShowTask(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ
{
	
	if (wParam != IDR_MAINFRAME)
		return 1;

	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ��
		CMenu menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵�
		menu.AppendMenu(MF_STRING, ID_ABOUT, _T("About"));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_MINIMIZE, _T("Minimize"));
		menu.AppendMenu(MF_STRING, ID_RESTORE, _T("Restore"));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_EXIT, _T("Exit"));
		SetForegroundWindow(this->m_hWnd);//�������Ϊ������������ط�ʱ,�����Ĳ˵��ܹ���ʧ
		//ȷ������ʽ�˵���λ��
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this->m_hWnd);
		//��Դ����
		HMENU hMenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK:
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
		this->ShowWindow(SW_RESTORE);//�򵥵���ʾ���������¶�
		SetForegroundWindow(this->m_hWnd);
	}
	break;
	}
	return 0;
}
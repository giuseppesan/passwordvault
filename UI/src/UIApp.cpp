#include "UIApp.hpp"
#include "LoginDialog.hpp"
#include "MainFrame.hpp"

wxIMPLEMENT_APP(UIApp);

bool UIApp::OnInit()
{
    LoginDialog loginDlg(wxT("Login Screen"));
    if (loginDlg.ShowModal() == wxID_OK)
    {
        MainFrame *frame = new MainFrame("Password Manager");
        frame->Show(true);
        return true;
    }
    return false;
}

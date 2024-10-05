#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP

#include <wx/wx.h>

class LoginDialog : public wxDialog
{
public:
    LoginDialog(const wxString &title);
    ~LoginDialog();

private:
    void OnLogin(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
    void OnRegister(wxCommandEvent &event);

    wxTextCtrl *usernameCtrl;
    wxTextCtrl *passwordCtrl;

    wxDECLARE_EVENT_TABLE();
};

#endif // LOGINDIALOG_HPP

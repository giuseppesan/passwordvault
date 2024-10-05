#ifndef REGISTERDIALOG_HPP
#define REGISTERDIALOG_HPP

#include <wx/wx.h>

class RegisterDialog : public wxDialog
{
public:
    RegisterDialog(const wxString &title);

private:
    void OnRegister(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

    wxTextCtrl *newUsernameCtrl;
    wxTextCtrl *newPasswordCtrl;

    wxDECLARE_EVENT_TABLE();
};

#endif // REGISTERDIALOG_HPP

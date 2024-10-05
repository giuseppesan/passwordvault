#ifndef CREDENTIALSDIALOG_HPP
#define CREDENTIALSDIALOG_HPP

#include <wx/wx.h>

class CredentialsDialog : public wxDialog
{
public:
    CredentialsDialog(const wxString &title);

private:
    void OnSave(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

    wxTextCtrl *usernameCtrl;
    wxTextCtrl *passwordCtrl;
    wxTextCtrl *tagCtrl;

    wxDECLARE_EVENT_TABLE();
};

#endif // CREDENTIALSDIALOG_HPP

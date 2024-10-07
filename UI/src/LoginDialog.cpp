#include "LoginDialog.hpp"
#include "RegisterDialog.hpp"
#include "hash.hpp"
#include "CLInterface.hpp"

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_BUTTON(wxID_OK, LoginDialog::OnLogin)
    EVT_BUTTON(wxID_CANCEL, LoginDialog::OnCancel)
    EVT_BUTTON(wxID_ANY + 1, LoginDialog::OnRegister)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(const wxString &title)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText *usernameLabel = new wxStaticText(this, wxID_ANY, wxT("Username:"));
    usernameCtrl = new wxTextCtrl(this, wxID_ANY);
    vbox->Add(usernameLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(usernameCtrl, 0, wxALL | wxEXPAND, 10);

    wxStaticText *passwordLabel = new wxStaticText(this, wxID_ANY, wxT("Password:"));
    passwordCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    vbox->Add(passwordLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(passwordCtrl, 0, wxALL | wxEXPAND, 10);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxButton *loginBtn = new wxButton(this, wxID_OK, wxT("Login"));
    wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    wxButton *registerBtn = new wxButton(this, wxID_ANY + 1, wxT("Register"));
    hbox->Add(loginBtn, 1);
    hbox->Add(cancelBtn, 1, wxLEFT, 10);
    hbox->Add(registerBtn, 1, wxLEFT, 10);

    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    SetSizerAndFit(vbox);
    Centre();
}

LoginDialog::~LoginDialog(){}

void LoginDialog::OnLogin(wxCommandEvent &WXUNUSED(event))
{
    wxString Username = usernameCtrl->GetValue();
    wxString Password = passwordCtrl->GetValue();

    if (Username.IsEmpty() || Password.IsEmpty())
    {
        wxMessageBox(wxT("Please enter both username and password!"), wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    std::string username = Username.ToStdString();
    std::string password = Password.ToStdString();

    hash hash_obj;
    CLInterface cli;

    utils::sanitize_input(username);
    hash_obj.set_user(username);
    utils::sanitize_input(password);
    hash_obj.set_password(password);

    int ret = hash_obj.check_login(hash_obj.get_user(), hash_obj.get_password());

    if (ret == 0)
    {
        wxMessageBox(wxT("Login successful!"), wxT("Info"), wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox(wxT("Login failed!"), wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    EndModal(wxID_OK);
}

void LoginDialog::OnRegister(wxCommandEvent &WXUNUSED(event))
{
    RegisterDialog registerDlg(wxT("Register"));
    registerDlg.ShowModal();
}

void LoginDialog::OnCancel(wxCommandEvent &WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}
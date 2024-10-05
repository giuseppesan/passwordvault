#include "RegisterDialog.hpp"
#include "hash.hpp"
#include "CLInterface.hpp"

wxBEGIN_EVENT_TABLE(RegisterDialog, wxDialog)
    EVT_BUTTON(wxID_OK, RegisterDialog::OnRegister)
    EVT_BUTTON(wxID_CANCEL, RegisterDialog::OnCancel)
wxEND_EVENT_TABLE()

RegisterDialog::RegisterDialog(const wxString &title)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
{
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText *newUsernameLabel = new wxStaticText(this, wxID_ANY, wxT("New Username:"));
    newUsernameCtrl = new wxTextCtrl(this, wxID_ANY);
    vbox->Add(newUsernameLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(newUsernameCtrl, 0, wxALL | wxEXPAND, 10);

    wxStaticText *newPasswordLabel = new wxStaticText(this, wxID_ANY, wxT("New Password:"));
    newPasswordCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    vbox->Add(newPasswordLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(newPasswordCtrl, 0, wxALL | wxEXPAND, 10);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxButton *registerBtn = new wxButton(this, wxID_OK, wxT("Register"));
    wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    hbox->Add(registerBtn, 1);
    hbox->Add(cancelBtn, 1, wxLEFT, 10);

    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    SetSizerAndFit(vbox);
    Centre();
}

void RegisterDialog::OnRegister(wxCommandEvent &WXUNUSED(event))
{
    wxString newUsername = newUsernameCtrl->GetValue();
    wxString newPassword = newPasswordCtrl->GetValue();

    if (newUsername.IsEmpty() || newPassword.IsEmpty())
    {
        wxMessageBox(wxT("Please fill out all fields!"), wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    std::string username = newUsername.ToStdString();
    std::string password = newPassword.ToStdString();

    hash hash_obj;
    CLInterface cli;

    cli.sanitize_input(username);
    hash_obj.set_user(username);
    cli.sanitize_input(password);
    hash_obj.set_password(password);

    int ret = hash_obj.register_user(hash_obj.get_user(), hash_obj.get_password());

    if (ret == 0)
    {
        wxMessageBox(wxT("Registration successful!"), wxT("Info"), wxOK | wxICON_INFORMATION);
        EndModal(wxID_OK);
    }
    else
    {
        wxMessageBox(wxT("Registration failed! Username may already exist."), wxT("Error"), wxOK | wxICON_ERROR);
    }
}

void RegisterDialog::OnCancel(wxCommandEvent &WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

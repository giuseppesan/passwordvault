#include "CredentialsDialog.hpp"
#include "hash.hpp"
#include "CLInterface.hpp"

wxBEGIN_EVENT_TABLE(CredentialsDialog, wxDialog)
    EVT_BUTTON(wxID_OK, CredentialsDialog::OnSave)
    EVT_BUTTON(wxID_CANCEL, CredentialsDialog::OnCancel)
wxEND_EVENT_TABLE()

CredentialsDialog::CredentialsDialog(const wxString &title)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
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

    wxStaticText *tagLabel = new wxStaticText(this, wxID_ANY, wxT("Tag (optional):"));
    tagCtrl = new wxTextCtrl(this, wxID_ANY);
    vbox->Add(tagLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);
    vbox->Add(tagCtrl, 0, wxALL | wxEXPAND, 10);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    wxButton *saveBtn = new wxButton(this, wxID_OK, wxT("Save"));
    wxButton *cancelBtn = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    hbox->Add(saveBtn, 1);
    hbox->Add(cancelBtn, 1, wxLEFT, 10);

    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    SetSizerAndFit(vbox);
    Centre();
}

void CredentialsDialog::OnSave(wxCommandEvent &WXUNUSED(event))
{
    wxString Username = usernameCtrl->GetValue();
    wxString Password = passwordCtrl->GetValue();
    wxString Tag = tagCtrl->GetValue();

    if (Username.IsEmpty() || Password.IsEmpty() || Tag.IsEmpty())
    {
        wxMessageBox(wxT("Please fill out all required fields!"), wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    std::string username = Username.ToStdString();
    std::string password = Password.ToStdString();
    std::string tag = Tag.ToStdString();

    CLInterface cli;
    hash hash_obj;

    cli.sanitize_input(username);
    cli.sanitize_input(password);
    cli.sanitize_input(tag);
       
    if (utils::find_entry(username, credentials_path) == found)
    {
        std::cout << "Tag is taken. Choose a new Tag:\n";
        return;
    }

    encryption enc_obj;
    enc_obj.add_new_entry(tag, username, password);

    wxMessageBox(wxT("Credentials saved successfully!"), wxT("Info"), wxOK | wxICON_INFORMATION);
    EndModal(wxID_OK);
}

void CredentialsDialog::OnCancel(wxCommandEvent &WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

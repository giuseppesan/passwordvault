#include "MainFrame.hpp"
#include "CredentialsDialog.hpp"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_NEW, MainFrame::OnNewEntry)
    EVT_MENU(wxID_HELP, MainFrame::OnHelp)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title)
{
    wxMenu *menuFile = new wxMenu;
    wxMenu *menuHelp = new wxMenu;
    
    menuFile->Append(wxID_NEW);
    menuHelp->Append(wxID_HELP);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
}

void MainFrame::OnNewEntry(wxCommandEvent &WXUNUSED(event))
{
    CredentialsDialog credentialsDlg(wxT("New Entry - Add Credentials"));
    credentialsDlg.ShowModal();
}

void MainFrame::OnHelp(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox("This is the help section.", "Help", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}

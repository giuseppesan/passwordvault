#include "MainFrame.hpp"
#include "CredentialsDialog.hpp"
#include "utils.hpp"
#include "encryption.hpp"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_NEW, MainFrame::OnNewEntry)
        EVT_MENU(wxID_HELP, MainFrame::OnHelp)
            EVT_LIST_ITEM_RIGHT_CLICK(wxID_ANY, MainFrame::OnItemRightClick)
                wxEND_EVENT_TABLE()

                    MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    CreateMenuBar();
    CreateStatusBar();
    CreateCredentialsList();
    LoadCredentials();
}

void MainFrame::CreateMenuBar()
{
    auto *menuFile = new wxMenu;
    auto *menuHelp = new wxMenu;

    menuFile->Append(wxID_NEW, "&New Entry\tCtrl-N", "Add a new credential entry");
    menuHelp->Append(wxID_HELP, "&Help\tF1", "Show help information");

    auto *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
}

void MainFrame::CreateCredentialsList()
{
    credentialsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_SUNKEN);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(credentialsList, 1, wxEXPAND | wxALL, 10);
    SetSizer(sizer);

    // Set up columns
    credentialsList->InsertColumn(0, "Username", wxLIST_FORMAT_LEFT, 150);
    credentialsList->InsertColumn(1, "Password", wxLIST_FORMAT_LEFT, 150);
    credentialsList->InsertColumn(2, "Website", wxLIST_FORMAT_LEFT, 200);
}

void MainFrame::LoadCredentials()
{
    credentialsList->DeleteAllItems(); // Clear previous items
    realPasswords.clear(); // Clear previous real passwords

    encryption obj;
    std::string out, username, password, tag;
    std::ifstream my_file(credentials_path);

    if (!my_file.is_open())
    {
        std::cerr << "Unable to open & read file\n";
        return;
    }

    while (getline(my_file, out))
    {
        obj.decrypt_credentials(out, username, password, tag);
        long index = credentialsList->InsertItem(0, username);

        // Insert the masked password (dots) for display
        credentialsList->SetItem(index, 1, wxString("******")); // Masked password for display
        
        // Set the real password in the realPasswords vector
        realPasswords.push_back(password);  // Store the real password
        credentialsList->SetItem(index, 2, tag);
    }
    my_file.close();
}

void MainFrame::OnNewEntry(wxCommandEvent &WXUNUSED(event))
{
    CredentialsDialog credentialsDlg(wxT("New Entry - Add Credentials"));
    if (credentialsDlg.ShowModal() == wxID_OK)
    {
        LoadCredentials(); // Reload credentials if new entry was added
    }
}

void MainFrame::OnItemRightClick(wxListEvent &WXUNUSED(event))
{
    wxMenu contextMenu;
    contextMenu.Append(wxID_COPY, "Copy Password");
    Bind(wxEVT_MENU, &MainFrame::OnCopyPassword, this, wxID_COPY);
    PopupMenu(&contextMenu);
}

void MainFrame::OnCopyPassword(wxCommandEvent &WXUNUSED(event))
{
    long selectedRow = credentialsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (selectedRow != -1)
    {
        // Retrieve the actual password from the vector using the selected row index
        wxString password = wxString(realPasswords[selectedRow]); // Get the real password

        // Copy the password to the clipboard
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(new wxTextDataObject(password));
            wxTheClipboard->Close();
            SetStatusText("Password copied to clipboard.");
        }
        else
        {
            SetStatusText("Failed to open the clipboard.");
        }
    }
    else
    {
        SetStatusText("No row selected.");
    }
}

void MainFrame::OnHelp(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox("This is the help section.", "Help", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}

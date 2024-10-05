#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/clipbrd.h>
#include <vector>
#include <string>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);
    void LoadCredentials();
    
private:
    void CreateMenuBar();
    void CreateCredentialsList();
    wxListCtrl *credentialsList; // Pointer to the credentials list control

    void OnNewEntry(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    void OnItemRightClick(wxListEvent &event);
    void OnCopyPassword(wxCommandEvent &event);

    std::vector<std::string> realPasswords; // Store the actual passwords

private:
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_HPP

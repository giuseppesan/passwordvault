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

private:
    void CreateMenuBar();
    void CreateCredentialsList();
    void LoadCredentials();

    void OnNewEntry(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    void OnItemRightClick(wxListEvent &event);
    void OnCopyPassword(wxCommandEvent &event);

    wxListCtrl *credentialsList; // Pointer to the credentials list control

private:
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_HPP

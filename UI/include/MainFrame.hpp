#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnNewEntry(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);

private:
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_HPP

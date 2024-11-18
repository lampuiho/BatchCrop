#include <wx/wx.h>

class FolderPicker : public wxPanel {
    wxTextCtrl *pathDisplay;
public:
    enum Ctrl { SELF, OPEN, TEXT, CLOSE };
    FolderPicker(wxWindow *parent, int baseId, std::string title);
    void OpenFolder(wxString folder);
    void CloseFolder();
    wxString GetOpened() { return pathDisplay->GetValue(); }
protected:
    void OnOpenFolder(wxCommandEvent& event);
    void OnCloseFolder(wxCommandEvent& event);
};
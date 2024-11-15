#include <wx/wx.h>

class FolderPicker : public wxPanel {
    wxTextCtrl *pathDisplay;
public:
    enum Ctrl { OPEN, TEXT, CLOSE };
    FolderPicker(wxWindow *parent, int baseId, std::string title);
    void OpenFolder(wxString folder);
    void CloseFolder();
    wxString GetOpened() { return pathDisplay->GetValue(); }
};
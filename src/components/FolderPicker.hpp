#include <wx/wx.h>

class FolderPicker : public wxPanel {
public:
    enum Ctrl { OPEN, TEXT, CLOSE };
    FolderPicker(wxFrame *parent, int baseId, std::string title);
};
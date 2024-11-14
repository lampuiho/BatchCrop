#include <wx/wx.h>

class FilePanel : public wxPanel {
public:
    enum Picker { SOURCE=2110, OUTPUT=2120  };
    FilePanel(wxFrame *parent);
};
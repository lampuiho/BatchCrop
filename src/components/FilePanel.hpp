#include <wx/wx.h>

class FilePanel : public wxPanel {
public:
    enum Picker { SOURCE=2110, OUTPUT=2120 };
    static const int FILE_LIST = 2130;
    FilePanel(wxFrame *parent);
    wxString GetOpenedSrc();
    wxString GetOpenedOut();
    void Invalidate(int idx);
private:
    FolderPicker *srcPicker;
    FolderPicker *outPicker;
    wxListBox *fileList;

    void OpenSrcFolder(wxString folder, std::vector<wxString> files);
    void CloseSrcFolder();
    void OpenOutFolder(wxString folder);
    void CloseOutFolder();
    void OnOpenSrcFolder(wxCommandEvent& event);
    void OnCloseSrcFolder(wxCommandEvent& event);
    void OnOpenOutFolder(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

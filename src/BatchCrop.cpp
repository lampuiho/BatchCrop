#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>
#include "utils/events.hpp"
#include "components/CropOptPanel.hpp"
#include "components/FolderPicker.hpp"
#include "components/FilePanel.hpp"
#include "components/PictureFrame.hpp"

class BatchCropApp : public wxApp {
public:
    virtual bool OnInit();
};
class BatchCropFrame : public wxFrame {
public:
    BatchCropFrame(const wxString& title);

private:
    // Event handlers
    void OnCloseSrcFolder(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnNextFile(wxCommandEvent& event);
    void OnPreviousFile(wxCommandEvent& event);
    void OnSaveImage(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnCropBoxAdjust(wxCommandEvent& event);
    void OnCropBoxColorChange(wxCommandEvent& event);
    void OnTransparencyChange(wxCommandEvent& event);

    // Helper methods
    // void LoadImage(const wxString& filepath);
    // void UpdateCropBox();

    // Controls
    FilePanel *filePanel;
    PictureFrame *picFrame;

    // Image and crop box parameters
    wxImage currentImage;
    wxRect cropBox;
    bool positionLocked, sizeLocked;
    int zoomLevel, transparency;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(BatchCropFrame, wxFrame)
    EVT_BUTTON(FilePanel::Picker::SOURCE+FolderPicker::Ctrl::CLOSE, BatchCropFrame::OnCloseSrcFolder)
    EVT_LISTBOX(FilePanel::FILE_LIST, BatchCropFrame::OnOpenFile)
    EVT_BUTTON(CropOptPanel::Ctrl::SAVE, BatchCropFrame::OnSaveImage)
    EVT_COMMAND(CROP_OPT_PANEL_ID, DEFINE_CROP, BatchCropFrame::OnCropBoxAdjust)
    // EVT_BUTTON(1007, BatchCropFrame::OnZoomIn)
    // EVT_BUTTON(1008, BatchCropFrame::OnZoomOut)
wxEND_EVENT_TABLE()
IMPLEMENT_APP(BatchCropApp)

bool BatchCropApp::OnInit() {
    BatchCropFrame *frame = new BatchCropFrame("Image Crop Tool");
    frame->Show(true);
    return true;
}
BatchCropFrame::BatchCropFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    // Initial states
    zoomLevel = 100;
    transparency = 20;
    positionLocked = false;
    sizeLocked = true;
    cropBox = wxRect(0, 0, 0, 0);

    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    // Left Panel
    // ------------------- //
    auto leftSizer = new wxBoxSizer(wxVERTICAL);
    // === TOP-Left Panel ===
    filePanel = new FilePanel(this);
    leftSizer->Add(filePanel, 1, wxEXPAND);
    // === Bottom-Left Panel ===
    auto optPanel = new CropOptPanel(this);
    leftSizer->Add(optPanel, 0, wxEXPAND);
    // ------------------- //
    mainSizer->Add(leftSizer, 0, wxEXPAND);
    // === Right Panel ===
    picFrame = new PictureFrame(this, wxID_ANY);
    mainSizer->Add(picFrame, 1, wxEXPAND);

    SetSizer(mainSizer);
    Centre();
}

// Event handlers (open folder, close, save, zoom, etc.)
void BatchCropFrame::OnCloseSrcFolder(wxCommandEvent& event) { picFrame->CloseImage(); }
void BatchCropFrame::OnOpenFile(wxCommandEvent& event) { 
    auto filename = event.GetString(), root = filePanel->GetOpenedSrc();
    wxFileName fname; fname.SetPath(root); fname.SetFullName(filename);
    auto fullPath = fname.GetFullPath();
    picFrame->LoadImageW(fullPath);
}
void BatchCropFrame::OnNextFile(wxCommandEvent& event) { /* Next file code */ }
void BatchCropFrame::OnPreviousFile(wxCommandEvent& event) { /* Previous file code */ }
void BatchCropFrame::OnSaveImage(wxCommandEvent& event) { /* Save cropped image code */ }
void BatchCropFrame::OnZoomIn(wxCommandEvent& event) { /* Zoom in functionality */ }
void BatchCropFrame::OnZoomOut(wxCommandEvent& event) { /* Zoom out functionality */ }
void BatchCropFrame::OnCropBoxAdjust(wxCommandEvent &event) {
    auto &rect = *(wxRect*)event.GetClientData();
    wxLogInfo("%d,%d,%d,%d",rect.x,rect.y,rect.width,rect.height);
}
void BatchCropFrame::OnCropBoxColorChange(wxCommandEvent& event) { /* Crop box color change */ }
void BatchCropFrame::OnTransparencyChange(wxCommandEvent& event) { /* Transparency adjustment */ }

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/dcbuffer.h>
#include <wx/dir.h>
#include "components/CropOptPanel.hpp"
#include "components/FolderPicker.hpp"
#include "components/FilePanel.hpp"


class BatchCropApp : public wxApp {
public:
    virtual bool OnInit();
};

class BatchCropFrame : public wxFrame {
public:
    BatchCropFrame(const wxString& title);

private:
    // Event handlers
    void OnOpenSrcFolder(wxCommandEvent& event);
    void OnCloseSrcFolder(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnNextFile(wxCommandEvent& event);
    void OnPreviousFile(wxCommandEvent& event);
    void OnOpenOutFolder(wxCommandEvent& event);
    void OnSaveImage(wxCommandEvent& event);
    void OnZoomIn(wxCommandEvent& event);
    void OnZoomOut(wxCommandEvent& event);
    void OnCropBoxAdjust(wxCommandEvent& event);
    void OnCropBoxColorChange(wxCommandEvent& event);
    void OnTransparencyChange(wxCommandEvent& event);

    // Helper methods
    void LoadImage(const wxString& filepath);
    void UpdateCropBox();
    
    // Image and crop box parameters
    wxImage currentImage;
    wxBitmap currentBitmap;
    wxRect cropBox;
    bool positionLocked, sizeLocked;
    int zoomLevel, transparency;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(BatchCropFrame, wxFrame)
    EVT_BUTTON(FilePanel::Picker::SOURCE+FolderPicker::Ctrl::OPEN, BatchCropFrame::OnOpenSrcFolder)
    EVT_BUTTON(FilePanel::Picker::SOURCE+FolderPicker::Ctrl::CLOSE, BatchCropFrame::OnCloseSrcFolder)
    EVT_BUTTON(FilePanel::Picker::OUTPUT+FolderPicker::Ctrl::OPEN, BatchCropFrame::OnOpenOutFolder)
    EVT_BUTTON(1006, BatchCropFrame::OnSaveImage)
    EVT_BUTTON(1007, BatchCropFrame::OnZoomIn)
    EVT_BUTTON(1008, BatchCropFrame::OnZoomOut)
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
    cropBox = wxRect(50, 50, 200, 200);

    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    // in top left
    auto filePanel = new FilePanel(this);
    mainSizer->Add(filePanel, 1, wxEXPAND | wxALL, 5);

    // === Bottom-Left Panel ===
    auto optPanel = new CropOptPanel(this);
    mainSizer->Add(optPanel, 1, wxEXPAND | wxALL, 5);

    // === Right Panel ===
    auto canvasPanel = new wxPanel(this, wxID_ANY);
    canvasPanel->SetBackgroundColour(*wxWHITE); // Placeholder for image display
    mainSizer->Add(canvasPanel, 3, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    Centre();
}

// Event handlers (open folder, close, save, zoom, etc.)
void BatchCropFrame::OnOpenSrcFolder(wxCommandEvent& event) { /* Open folder code */ }
void BatchCropFrame::OnCloseSrcFolder(wxCommandEvent& event) { /* Close folder code */ }
void BatchCropFrame::OnOpenFile(wxCommandEvent& event) { /* Next file code */ }
void BatchCropFrame::OnNextFile(wxCommandEvent& event) { /* Next file code */ }
void BatchCropFrame::OnPreviousFile(wxCommandEvent& event) { /* Previous file code */ }
void BatchCropFrame::OnOpenOutFolder(wxCommandEvent& event) { /* Output folder selection */ }
void BatchCropFrame::OnSaveImage(wxCommandEvent& event) { /* Save cropped image code */ }
void BatchCropFrame::OnZoomIn(wxCommandEvent& event) { /* Zoom in functionality */ }
void BatchCropFrame::OnZoomOut(wxCommandEvent& event) { /* Zoom out functionality */ }
void BatchCropFrame::OnCropBoxAdjust(wxCommandEvent& event) { /* Crop box adjust */ }
void BatchCropFrame::OnCropBoxColorChange(wxCommandEvent& event) { /* Crop box color change */ }
void BatchCropFrame::OnTransparencyChange(wxCommandEvent& event) { /* Transparency adjustment */ }

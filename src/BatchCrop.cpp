#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/tglbtn.h>
#include <wx/filedlg.h>
#include <wx/popupwin.h>
#include <wx/filename.h>
#include "common/events.hpp"
#include "components/NotificationPopup.hpp"
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
    void OnResetZoom(wxCommandEvent& event);
    void OnZoomUpdate(wxCommandEvent& event);
    void OnCropToggle(wxCommandEvent& event);
    void OnCropEnd(wxCommandEvent& event);
    void OnCropBoxInput(wxCommandEvent& event);
    void OnCropBoxColorChange(wxCommandEvent& event);
    void OnTransparencyChange(wxCommandEvent& event);
    // Controls
    FilePanel *filePanel;
    CropOptPanel *optPanel;
    PictureFrame *picFrame;
    // Managers
    PopupManager popupManager;
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(BatchCropFrame, wxFrame)
    EVT_COMMAND(FilePanel::Picker::SOURCE, CLOSE_FOLDER, BatchCropFrame::OnCloseSrcFolder)
    EVT_LISTBOX(FilePanel::FILE_LIST, BatchCropFrame::OnOpenFile)
    EVT_BUTTON(CropOptPanel::Ctrl::SAVE, BatchCropFrame::OnSaveImage)
    EVT_TOGGLEBUTTON(CropOptPanel::Ctrl::CROP, BatchCropFrame::OnCropToggle)
    EVT_COMMAND(CROP_OPT_PANEL_ID, DEFINE_CROP, BatchCropFrame::OnCropBoxInput)
    EVT_COMMAND(PICTURE_FRAME_ID, DEFINE_CROP, BatchCropFrame::OnCropEnd)
    EVT_BUTTON(CropOptPanel::Ctrl::ZOOM_IN, BatchCropFrame::OnZoomIn)
    EVT_BUTTON(CropOptPanel::Ctrl::ZOOM_OUT, BatchCropFrame::OnZoomOut)
    EVT_BUTTON(CropOptPanel::Ctrl::ZOOM_LEVEL, BatchCropFrame::OnResetZoom)
    EVT_COMMAND(PICTURE_FRAME_ID, UPDATE_ZOOM, BatchCropFrame::OnZoomUpdate)
    EVT_COMMAND(wxID_ANY, REQ_SAVE, BatchCropFrame::OnSaveImage)
wxEND_EVENT_TABLE()
IMPLEMENT_APP(BatchCropApp)

bool BatchCropApp::OnInit() {
    BatchCropFrame *frame = new BatchCropFrame("Image Crop Tool");
    frame->Show(true);
    return true;
}
BatchCropFrame::BatchCropFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    auto mainSizer = new wxBoxSizer(wxHORIZONTAL);
    // Left Panel
    // ------------------- //
    auto leftSizer = new wxBoxSizer(wxVERTICAL);
    // === TOP-Left Panel ===
    filePanel = new FilePanel(this);
    leftSizer->Add(filePanel, 1, wxEXPAND);
    // === Bottom-Left Panel ===
    optPanel = new CropOptPanel(this);
    leftSizer->Add(optPanel, 0, wxEXPAND);
    // ------------------- //
    mainSizer->Add(leftSizer, 0, wxEXPAND);
    // === Right Panel ===
    picFrame = new PictureFrame(this, PICTURE_FRAME_ID);
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
void BatchCropFrame::OnSaveImage(wxCommandEvent& event) {
    wxString error;
    auto root = filePanel->GetOpenedOut();
    if (root.IsEmpty()) error += "Have not specified output folder.\n";
    auto file = filePanel->GetOpenedFile();
    if (file.IsEmpty()) error += "Have not opened a file.\n";

    wxString message;
    if (error.IsEmpty()) {
        wxFileName fname; fname.SetPath(root); fname.SetFullName(file);
        auto fullPath = fname.GetFullPath();
        message = picFrame->SaveCrop(fullPath, error) ? "File saved to: " + fullPath : file + " not saved.";
        if (!error.IsEmpty()) message += '\n' + error;
    } else {
        message = "No file saved.\n" + error;
    }
    auto popup = new NotificationPopup(this, &popupManager, message);
}
void BatchCropFrame::OnZoomIn(wxCommandEvent& event) { picFrame->ZoomIn(); }
void BatchCropFrame::OnZoomOut(wxCommandEvent& event) { picFrame->ZoomOut(); }
void BatchCropFrame::OnResetZoom(wxCommandEvent& event) { picFrame->SetZoom(-1, true); }
void BatchCropFrame::OnZoomUpdate(wxCommandEvent& event) { optPanel->UpdateZoom(event.GetInt()); }
void BatchCropFrame::OnCropToggle(wxCommandEvent& event) {
    if (event.IsChecked()) picFrame->StartCrop();
}
void BatchCropFrame::OnCropEnd(wxCommandEvent &event) {
    auto &rect = *(wxRect*)event.GetClientData();
    optPanel->EndCrop(rect);
}
void BatchCropFrame::OnCropBoxInput(wxCommandEvent &event) {
    auto &rect = *(wxRect*)event.GetClientData();
    picFrame->SetCrop(rect);
}
void BatchCropFrame::OnCropBoxColorChange(wxCommandEvent& event) { /* Crop box color change */ }
void BatchCropFrame::OnTransparencyChange(wxCommandEvent& event) { /* Transparency adjustment */ }

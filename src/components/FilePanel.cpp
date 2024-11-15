#include <wx/dir.h>
#include "FolderPicker.hpp"
#include "FilePanel.hpp"

FilePanel::FilePanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    // === Top-Left Panel ===
    auto topLeftSizer = new wxBoxSizer(wxVERTICAL);

    // Source folder selection
    srcPicker = new FolderPicker(this, Picker::SOURCE, "Source Folder:");
    topLeftSizer->Add(srcPicker, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    // File list
    fileList = new wxListBox(this, FILE_LIST);
    topLeftSizer->Add(fileList, 1, wxEXPAND | wxALL, 5);

    // Output folder selection
    outPicker = new FolderPicker(this, Picker::OUTPUT, "Output Folder:");
    topLeftSizer->Add(outPicker, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    this->SetSizer(topLeftSizer);
}
void FilePanel::OpenSrcFolder(wxString folder, std::vector<wxString> files) { 
    srcPicker->OpenFolder(folder);
    fileList->Clear();
    fileList->Append(files);
}
void FilePanel::CloseSrcFolder() {
    fileList->Clear();
    srcPicker->CloseFolder();
}
void FilePanel::OpenOutFolder(wxString folder) { outPicker->OpenFolder(folder); }
void FilePanel::CloseOutFolder() { outPicker->CloseFolder(); }
wxString FilePanel::GetOpenedSrc() { return srcPicker->GetOpened(); }
wxString FilePanel::GetOpenedOut() { return outPicker->GetOpened(); }
void FilePanel::Invalidate(int idx) { fileList->Delete(idx); }
void FilePanel::OnOpenSrcFolder(wxCommandEvent& event) { 
    auto opened = GetOpenedSrc();
    wxDirDialog dialog(this, wxT("Choose a directory"), opened);
    if (dialog.ShowModal() == wxID_OK) { 
        wxString path = dialog.GetPath();
        std::vector<wxString> files;
        wxDir dir(path);
        wxString filename;
        bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES|wxDIR_HIDDEN);
        while(cont) {
            if (filename.EndsWith(".png")||filename.EndsWith(".jpg")||filename.EndsWith(".jpeg")) {
                files.push_back(filename);
            }
            cont = dir.GetNext(&filename);
        }
        OpenSrcFolder(path, files);
    }
}
void FilePanel::OnCloseSrcFolder(wxCommandEvent& event) {  CloseSrcFolder(); }
void FilePanel::OnOpenOutFolder(wxCommandEvent& event) { /* Output folder selection */ }

wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
    EVT_BUTTON(FilePanel::Picker::SOURCE+FolderPicker::Ctrl::OPEN, FilePanel::OnOpenSrcFolder)
    EVT_BUTTON(FilePanel::Picker::SOURCE+FolderPicker::Ctrl::CLOSE, FilePanel::OnCloseSrcFolder)
    EVT_BUTTON(FilePanel::Picker::OUTPUT+FolderPicker::Ctrl::OPEN, FilePanel::OnOpenOutFolder)
wxEND_EVENT_TABLE()
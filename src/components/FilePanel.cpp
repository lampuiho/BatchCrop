#include <wx/dir.h>
#include "../common/events.hpp"
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
    wxEvtHandler::Bind(wxEVT_CHAR_HOOK, FilePanel::OnKeyDown, this, FILE_LIST);

    // Output folder selection
    outPicker = new FolderPicker(this, Picker::OUTPUT, "Output Folder:");
    topLeftSizer->Add(outPicker, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    this->SetSizer(topLeftSizer);
}
std::vector<wxString> FilePanel::OpenFolder(wxString folder) {
    std::vector<wxString> files;
    wxDir dir(folder);
    wxString filename;
    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES|wxDIR_HIDDEN);
    while(cont) {
        if (filename.EndsWith(".png")||filename.EndsWith(".jpg")||filename.EndsWith(".jpeg")) {
            files.push_back(filename);
        }
        cont = dir.GetNext(&filename);
    }
    return files;
}
void FilePanel::OpenSrcFolder(wxString folder) {
    auto files = OpenFolder(folder);
    fileList->Clear();
    fileList->Append(files);
}
void FilePanel::CloseSrcFolder() { fileList->Clear(); }
void FilePanel::OpenOutFolder(wxString folder) { outPicker->OpenFolder(folder); }
void FilePanel::CloseOutFolder() { outPicker->CloseFolder(); }
wxString FilePanel::GetOpenedSrc() { return srcPicker->GetOpened(); }
wxString FilePanel::GetOpenedOut() { return outPicker->GetOpened(); }
wxString FilePanel::GetOpenedFile() { 
    try {
        return fileList->GetString(fileList->GetSelection());
    } catch(...) {
        return "";
    }
}
void FilePanel::Invalidate(int idx) { fileList->Delete(idx); }
void FilePanel::OnOpenSrcFolder(wxCommandEvent& event) { OpenSrcFolder(event.GetString()); event.Skip(); }
void FilePanel::OnCloseSrcFolder(wxCommandEvent& event) {  CloseSrcFolder(); event.Skip(); }
void FilePanel::OnKeyDown(wxKeyEvent &event) {
    if (event.GetKeyCode()==WXK_RETURN || event.GetKeyCode()==WXK_NUMPAD_ENTER) {
        wxCommandEvent event(REQ_SAVE, this->GetId());
        wxPostEvent(this, event);
    } else {
        event.Skip();
    }
}

wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
    EVT_COMMAND(FilePanel::Picker::SOURCE, OPEN_FOLDER, FilePanel::OnOpenSrcFolder)
    EVT_COMMAND(FilePanel::Picker::SOURCE, CLOSE_FOLDER, FilePanel::OnCloseSrcFolder)
wxEND_EVENT_TABLE()
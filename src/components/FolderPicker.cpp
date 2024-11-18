#include <wx/artprov.h>
#include <wx/bmpbuttn.h>
#include <wx/dir.h>
#include "../common/events.hpp"
#include "FolderPicker.hpp"

FolderPicker::FolderPicker(wxWindow *parent, int baseId, std::string title) : wxPanel(parent, baseId) {
    auto panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(new wxStaticText(this, wxID_ANY, title));

    auto pickerSizer = new wxBoxSizer(wxHORIZONTAL);
    auto openBtn = new wxBitmapButton(this, baseId+Ctrl::OPEN, wxArtProvider::GetIcon(wxART_FOLDER_OPEN));
    pickerSizer->Add(openBtn);
    pathDisplay = new wxTextCtrl(this, baseId+Ctrl::TEXT);
    pathDisplay->SetEditable(false);
    pickerSizer->Add(pathDisplay, 1, wxEXPAND);
    auto closeBtn = new wxBitmapButton(this, baseId+Ctrl::CLOSE, wxArtProvider::GetIcon(wxART_CLOSE));
    pickerSizer->Add(closeBtn, 0, wxEXPAND);

    panelSizer->Add(pickerSizer, 0, wxEXPAND);

    wxEvtHandler::Bind(wxEVT_BUTTON, OnOpenFolder, this, openBtn->GetId());
    wxEvtHandler::Bind(wxEVT_BUTTON, OnCloseFolder, this, closeBtn->GetId());

    this->SetSizer(panelSizer);
}
void FolderPicker::OpenFolder(wxString folder) {
    pathDisplay->Clear();
    pathDisplay->AppendText(folder);
    wxCommandEvent openEvt(OPEN_FOLDER, this->GetId());
    openEvt.SetString(folder);
    wxPostEvent(this, openEvt);
}
void FolderPicker::CloseFolder() {
    pathDisplay->Clear();
    wxCommandEvent closeEvt(CLOSE_FOLDER, this->GetId());
    wxPostEvent(this, closeEvt);
}
void FolderPicker::OnOpenFolder(wxCommandEvent& event) {
    auto opened = GetOpened();
    wxDirDialog dialog(this, wxT("Choose a directory"), opened);
    if (dialog.ShowModal() == wxID_OK) { 
        wxString path = dialog.GetPath();
        OpenFolder(path);
    }
}
void FolderPicker::OnCloseFolder(wxCommandEvent& event) {
    CloseFolder();
}
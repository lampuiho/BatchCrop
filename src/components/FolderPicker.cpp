#include <wx/artprov.h>
#include <wx/bmpbuttn.h>
#include "FolderPicker.hpp"

FolderPicker::FolderPicker(wxWindow *parent, int baseId, std::string title) : wxPanel(parent, wxID_ANY) {
    auto panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(new wxStaticText(this, wxID_ANY, title));

    auto pickerSizer = new wxBoxSizer(wxHORIZONTAL);
    pickerSizer->Add(new wxBitmapButton(this, baseId+Ctrl::OPEN, wxArtProvider::GetIcon(wxART_FOLDER_OPEN)));
    pathDisplay = new wxTextCtrl(this, baseId+Ctrl::TEXT);
    pathDisplay->SetEditable(false);
    pickerSizer->Add(pathDisplay, 1, wxEXPAND);
    pickerSizer->Add(new wxBitmapButton(this, baseId+Ctrl::CLOSE, wxArtProvider::GetIcon(wxART_CLOSE)), 0, wxEXPAND);

    panelSizer->Add(pickerSizer, 0, wxEXPAND);

    this->SetSizer(panelSizer);
}
void FolderPicker::OpenFolder(wxString folder) {
    pathDisplay->Clear();
    pathDisplay->AppendText(folder);
}
void FolderPicker::CloseFolder() {
    pathDisplay->Clear();
}
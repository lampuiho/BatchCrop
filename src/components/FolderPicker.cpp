#include "FolderPicker.hpp"

FolderPicker::FolderPicker(wxFrame *parent, int baseId, std::string title) {
    wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
    panelSizer->Add(new wxStaticText(this, wxID_ANY, title));

    wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
    pickerSizer->Add(new wxButton(this, baseId+Ctrl::OPEN, "📁"));
    pickerSizer->Add(new wxTextCtrl(this, baseId+Ctrl::TEXT), 1, wxEXPAND | wxALL, 5);
    pickerSizer->Add(new wxButton(this, baseId+Ctrl::CLOSE, "❌"));

    panelSizer->Add(pickerSizer);
}

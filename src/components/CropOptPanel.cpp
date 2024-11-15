#include "wxToggleButton.hpp"
#include "CropOptPanel.hpp"

CropOptPanel::CropOptPanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    auto bottomLeftSizer = new wxBoxSizer(wxVERTICAL);

    // Crop options (x, y, w, h inputs)
    auto cropInputSizer = new wxGridSizer(2, 4, 0, 5);
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "X"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "Y"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "W"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "H"));
    for (int c=Ctrl::X_INPUT; c<=Ctrl::H_INPUT; ++c) {
        cropInputSizer->Add(new wxTextCtrl(this, Ctrl::X_INPUT, "", wxDefaultPosition, {48,-1}));
    }
    bottomLeftSizer->Add(cropInputSizer, 0, wxEXPAND | wxALL, 5);

    // Crop settings toggle buttons
    auto toggleSizer = new wxBoxSizer(wxHORIZONTAL);
    toggleSizer->Add(new wxToggleButton(this, Ctrl::POS_LOCK, "Position Lock"), 0, wxEXPAND | wxRIGHT, 5);
    toggleSizer->Add(new wxToggleButton(this, Ctrl::SIZE_LOCK, "Size Lock"), 0, wxEXPAND | wxRIGHT, 5);
    toggleSizer->Add(new wxButton(this, Ctrl::SAVE, "Save Image"), 0, wxEXPAND | wxRIGHT, 5);
    bottomLeftSizer->Add(toggleSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizer(bottomLeftSizer);
}
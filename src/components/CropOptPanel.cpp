#include "wxToggleButton.hpp"
#include "CropOptPanel.hpp"

CropOptPanel::CropOptPanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    auto bottomLeftSizer = new wxBoxSizer(wxVERTICAL);

    // Crop options (x, y, w, h inputs)
    auto cropInputSizer = new wxGridSizer(2, 4, 5, 5);
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "X"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "Y"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "W"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "H"));
    cropInputSizer->Add(new wxTextCtrl(this, Ctrl::X_INPUT, "50"));
    cropInputSizer->Add(new wxTextCtrl(this, Ctrl::Y_INPUT, "50"));
    cropInputSizer->Add(new wxTextCtrl(this, Ctrl::W_INPUT, "200"));
    cropInputSizer->Add(new wxTextCtrl(this, Ctrl::H_INPUT, "200"));
    bottomLeftSizer->Add(cropInputSizer, 0, wxEXPAND | wxALL, 5);

    // Crop settings toggle buttons
    auto toggleSizer = new wxBoxSizer(wxHORIZONTAL);
    toggleSizer->Add(new wxToggleButton(this, Ctrl::POS_LOCK, "Position Lock"));
    toggleSizer->Add(new wxToggleButton(this, Ctrl::SIZE_LOCK, "Size Lock"));
    toggleSizer->Add(new wxButton(this, Ctrl::SAVE, "Save Image"));
    bottomLeftSizer->Add(toggleSizer, 0, wxEXPAND | wxALL, 5);

    this->SetSizer(bottomLeftSizer);
}
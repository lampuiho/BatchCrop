#include <wx/artprov.h>
#include <wx/tglbtn.h>
#include "../utils/events.hpp"
#include "CropOptPanel.hpp"

CropOptPanel::CropOptPanel(wxFrame *parent) : wxPanel(parent, CROP_OPT_PANEL_ID) {
    auto bottomLeftSizer = new wxBoxSizer(wxVERTICAL);

    // Crop options (x, y, w, h inputs)
    auto cropInputSizer = new wxGridSizer(2, 5, 0, 5);
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "X"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "Y"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "W"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, "H"));
    cropInputSizer->Add(new wxStaticText(this, wxID_ANY, ""));
    for (int c=Ctrl::X_INPUT; c<=Ctrl::H_INPUT; ++c) {
        cropInputSizer->Add(new wxTextCtrl(this, c, "", wxDefaultPosition, {48,-1}));
    }
    cropInputSizer->Add(new wxBitmapToggleButton(this, Ctrl::CROP, wxArtProvider::GetIcon(wxART_FULL_SCREEN), wxDefaultPosition, {20,20}));
    bottomLeftSizer->Add(cropInputSizer, 0, wxEXPAND | wxALL, 5);

    // Crop settings toggle buttons
    auto toggleSizer = new wxBoxSizer(wxHORIZONTAL);
    toggleSizer->Add(new wxBitmapToggleButton(this, Ctrl::POS_LOCK, wxArtProvider::GetIcon(wxART_STOP), wxDefaultPosition, {20,20}), 0, wxEXPAND | wxRIGHT, 5);
    toggleSizer->Add(new wxBitmapToggleButton(this, Ctrl::SIZE_LOCK, wxArtProvider::GetIcon(wxART_STOP), wxDefaultPosition, {20,20}), 0, wxEXPAND | wxRIGHT, 5);
    toggleSizer->Add(new wxButton(this, Ctrl::SAVE, "Save Image"), 0, wxEXPAND | wxRIGHT, 5);
    bottomLeftSizer->Add(toggleSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizer(bottomLeftSizer);
}
void CropOptPanel::OnEnterCropParam(wxCommandEvent &event) {
    auto id = event.GetId();
    auto txt = event.GetString();
    try {
        cropBox[id - Ctrl::X_INPUT] = std::stoi(txt.ToStdString());
    } catch (...) {
        cropBox[id - Ctrl::X_INPUT] = -1;
    }
    for (auto c: cropBox) if (c < 0) return;
    wxCommandEvent cropEvent(DEFINE_CROP);
    cropEvent.SetClientData(new wxRect(cropBox[0],cropBox[1],cropBox[2],cropBox[3]));
    wxPostEvent(this, cropEvent);
}
BEGIN_EVENT_TABLE(CropOptPanel, wxPanel)
EVT_TEXT(Ctrl::X_INPUT, CropOptPanel::OnEnterCropParam)
EVT_TEXT(Ctrl::Y_INPUT, CropOptPanel::OnEnterCropParam)
EVT_TEXT(Ctrl::W_INPUT, CropOptPanel::OnEnterCropParam)
EVT_TEXT(Ctrl::H_INPUT, CropOptPanel::OnEnterCropParam)
END_EVENT_TABLE()
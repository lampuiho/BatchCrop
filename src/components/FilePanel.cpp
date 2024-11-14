#include "FolderPicker.hpp"
#include "FilePanel.hpp"

FilePanel::FilePanel(wxFrame *parent) : wxPanel(parent, wxID_ANY) {
    // === Top-Left Panel ===
    auto topLeftSizer = new wxBoxSizer(wxVERTICAL);

    // Source folder selection
    auto srcPicker = new FolderPicker(parent, Picker::SOURCE, "Source Folder:");
    topLeftSizer->Add(srcPicker, 0);

    // File list
    auto fileList = new wxListBox(this, wxID_ANY);
    topLeftSizer->Add(fileList, 1, wxEXPAND | wxALL, 5);

    // Output folder selection
    auto outPicker = new FolderPicker(parent, Picker::OUTPUT, "Output Folder:");
    topLeftSizer->Add(outPicker, 0);

    this->SetSizer(topLeftSizer);
}
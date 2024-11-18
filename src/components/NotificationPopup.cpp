#include "NotificationPopup.hpp"

NotificationPopup::NotificationPopup(wxWindow* parent, PopupManager* manager, const wxString& message)
    : wxPopupWindow(parent, wxBORDER_NONE), m_timer(this), m_manager(manager) {
    m_manager->RegisterPopup(this);
// Create a sizer and a label to show the message
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_label = new wxStaticText(this, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
    m_label->SetBackgroundColour(*wxWHITE);
    sizer->Add(m_label, wxSizerFlags().Expand().Border(wxALL, 5));
    SetSizerAndFit(sizer);

    // Bind the timer event to close the popup
    Bind(wxEVT_TIMER, &NotificationPopup::OnTimer, this);

    // Calculate display time based on message length
    size_t wordCount = std::count(message.begin(), message.end(), ' ') + 1;
    size_t charCount = message.length();
    double duration = std::max(0.4 * wordCount, 0.08 * charCount) * 1000; // in milliseconds

    ShowAtTopCenter(duration);
}
void NotificationPopup::ShowAtTopCenter(double duration) {
    // Get parent size and calculate maximum width (80% of parent width)
    wxSize parentSize = GetParent()->GetSize();
    int maxWidth = static_cast<int>(parentSize.GetWidth() * 0.8);

    // Constrain label width and adjust layout
    m_label->Wrap(maxWidth);
    SetSizerAndFit(GetSizer());
    wxPoint parentPos = GetParent()->GetPosition();
    wxSize popupSize = GetSize();

    int x = parentPos.x + (parentSize.x - popupSize.x) / 2;
    int y = parentPos.y + 10; // Offset from the top
    Position(wxPoint(x, y), wxSize());
    Show();
    m_timer.StartOnce(static_cast<int>(duration));
}
void NotificationPopup::OnTimer(wxTimerEvent&) { m_manager->RemovePopup(this); }
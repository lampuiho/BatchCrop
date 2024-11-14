#include <wx/wx.h>
#include "wxToggleButton.hpp"

wxToggleButton::wxToggleButton(wxWindow* parent, wxWindowID id, const wxString& label, bool initialState)
    : wxButton(parent, id, label), toggled(initialState) {
    UpdateAppearance();
    Bind(wxEVT_BUTTON, &wxToggleButton::OnToggle, this);
}
bool wxToggleButton::IsToggled() const {
    return toggled;
}
void wxToggleButton::OnToggle(wxCommandEvent& event) {
    toggled = !toggled;
    UpdateAppearance();
    event.Skip();  // Propagate the event further if needed.
}
void  wxToggleButton::UpdateAppearance() {
    if (toggled) {
        SetBackgroundColour(*wxBLUE);
        SetLabel("On");
    } else {
        SetBackgroundColour(*wxLIGHT_GREY);
        SetLabel("Off");
    }
    Refresh();
}
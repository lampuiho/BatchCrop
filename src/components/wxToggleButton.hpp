#include <wx/wx.h>
#include <wx/sizer.h>

class wxToggleButton : public wxButton {
public:
    wxToggleButton(wxWindow* parent, wxWindowID id, const wxString& label, bool initialState=false);
    bool IsToggled() const;

private:
    bool toggled;

    void OnToggle(wxCommandEvent& event);
    void UpdateAppearance();
};
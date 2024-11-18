#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/timer.h>
#include "../common/PopupManager.hpp"

class NotificationPopup : public wxPopupWindow {
public:
    NotificationPopup(wxWindow* parent, PopupManager* manager, const wxString& message);
private:
    void ShowAtTopCenter(double duration);
    void OnTimer(wxTimerEvent&);
    wxTimer m_timer;
    PopupManager *m_manager;
    wxStaticText *m_label;
};
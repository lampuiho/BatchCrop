#include <unordered_set>
#include <wx/wx.h>

class wxPopupWindow;
class PopupManager {
public:
    void RegisterPopup(wxPopupWindow* popup);
    void RemovePopup(wxPopupWindow* popup);
    ~PopupManager();
private:
    std::unordered_set<wxPopupWindow*> m_popups;
};
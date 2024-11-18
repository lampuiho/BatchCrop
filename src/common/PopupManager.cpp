#include <wx/popupwin.h>
#include "PopupManager.hpp"

PopupManager::~PopupManager() {
    // Ensure all popups are cleaned up
    for (auto* popup : m_popups) {
        delete popup;
    }
}
void PopupManager::RegisterPopup(wxPopupWindow* popup) {
    m_popups.insert(popup);
}
void PopupManager::RemovePopup(wxPopupWindow* popup) {
    m_popups.erase(popup);
    delete popup; // Safely delete the popup
}
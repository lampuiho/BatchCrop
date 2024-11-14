#include <wx/wx.h>

#define CROP_OPT_PANEL_ID 3000

class CropOptPanel : public wxPanel {
public:
    enum Ctrl { X_INPUT=CROP_OPT_PANEL_ID, Y_INPUT, W_INPUT, H_INPUT, POS_LOCK, SIZE_LOCK, SAVE, ZOOM_IN, ZOOM_OUT };
    CropOptPanel(wxFrame *parent);
};
#include <wx/wx.h>

#define CROP_OPT_PANEL_ID 3000
class wxToggleButton;
class CropOptPanel : public wxPanel {
public:
    enum Ctrl { X_INPUT=CROP_OPT_PANEL_ID+1, Y_INPUT, W_INPUT, H_INPUT, CROP, POS_LOCK, SIZE_LOCK, SAVE, ZOOM_IN, ZOOM_OUT, ZOOM_LEVEL };
    CropOptPanel(wxFrame *parent);
    void EndCrop(wxRect cropRect);
    void UpdateZoom(int perc);
protected:
    wxDECLARE_EVENT_TABLE();
private:
    void EndCrop();
    std::array<wxTextCtrl*,4> cropInputs;
    std::array<int,4> cropBox = {-1, -1, -1 ,-1};
    void OnEnterCropParam(wxCommandEvent &event);
    wxButton *zoomLvl;
    wxToggleButton *cropToggle;
};
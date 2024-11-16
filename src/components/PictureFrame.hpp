#include <wx/wx.h>
#include <wx/scrolwin.h>
#include "../utils/enums.hpp"

// A scrolled window for showing an image.
class PictureFrame: public wxScrolledWindow {
public:
    PictureFrame(wxWindow *parent, wxWindowID id);;
    void LoadImage(const wxString &fullPath);
    void CloseImage();
    void SetZoom(int percentage);
    void ZoomIn(int rot=1);
    void ZoomOut(int rot=1);
    void StartCrop();
protected:
    void OnDraw(wxDC& dc) override;
    void OnEraseBackGround(wxEraseEvent& event);
    void OnMouseDrag(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    void OnMouseScroll(wxMouseEvent& event);
    double GetFitImageZoom();

    wxDECLARE_EVENT_TABLE();
private:
    enum CropState { START, NEXT, DONE, N_CROP_STATE };
    void UpdateScaleOffset(double newScale, double oldScale);
    void ClampScrollOffset();
    void UpdateScale();
    void OnWindowSizeChange(wxSizeEvent& event);

    wxImage image;          // The wxImage object
    wxBitmap bitmap;        // Renderable bitmap
    double scale = 1;
    int zoom = -1;          // Current zoom level
    wxPoint panStart;       // Starting point for panning
    wxPoint scrollOffset;   // Scroll position for panning
    wxSize lastRendered = { 0, 0 };
    wxRect cropBox;
    bool needRedraw = true;
    bool lockZoom;
    bool isPanning;         // Flag to indicate panning state
    CropState cropState = DONE;

    DEFINE_INCREMENT(PictureFrame::CropState)
};
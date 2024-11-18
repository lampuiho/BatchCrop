#include <wx/wx.h>
#include <wx/scrolwin.h>
#include "../utils/enums.hpp"

#define PICTURE_FRAME_ID 4000

// A scrolled window for showing an image.
class PictureFrame: public wxScrolledWindow {
public:
    PictureFrame(wxWindow *parent, wxWindowID id);
    void LoadImage(const wxString &fullPath);
    void CloseImage();
    bool SaveCrop(const wxString &fullPath, wxString &error);
    void SetZoom(int percentage, bool refresh=false);
    void ZoomIn(int rot=1);
    void ZoomOut(int rot=1);
    void StartCrop();
    void SetCrop(wxRect crop);
protected:
    void OnDraw(wxDC& dc) override;
    void OnEraseBackGround(wxEraseEvent &event);
    void OnMouseDrag(wxMouseEvent &event);
    void OnMouseLeftDown(wxMouseEvent &event);
    void OnMouseScroll(wxMouseEvent &event);
    double GetFitImageZoom();

    wxDECLARE_EVENT_TABLE();
private:
    enum CropState { START, NEXT, DONE, N_CROP_STATE };
    wxSize GetSize();
    void UpdateScaleOffset(double newScale, double oldScale);
    void ClampScrollOffset();
    bool ClampCropBox();
    void ClampCropBoxSize();
    void UpdateScale();
    bool HitCrop(wxPoint pt);
    void OnWindowSizeChange(wxSizeEvent &event);
    void OnCropBoxUpdate();
    wxPoint ToImageSpace(wxPoint pt);

    wxImage image;          // The wxImage object
    wxBitmap bitmap;        // Renderable bitmap
    double scale = 1;
    int zoom = -1;          // Current zoom level
    wxPoint panStart;       // Starting point for panning
    wxPoint scrollOffset;   // Scroll position for panning
    wxRect cropBox;
    bool needRedraw = true;
    bool lockZoom = false;
    bool isPanning = false;         // Flag to indicate panning state
    bool isDraggingCrop = false;
    CropState cropState = DONE;

    DEFINE_INCREMENT(PictureFrame::CropState)
};
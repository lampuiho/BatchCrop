#include <wx/wx.h>
#include <wx/scrolwin.h>

// A scrolled window for showing an image.
class PictureFrame: public wxScrolledWindow {
public:
    PictureFrame(wxWindow *parent, wxWindowID id);;
    void LoadImage(const wxString &fullPath);
    void CloseImage();
    void SetZoom(int percentage);
protected:
    void OnDraw(wxDC& dc) override;
    void OnMouseDrag(wxMouseEvent& event);
    void OnMouseLeftDown(wxMouseEvent& event);
    double GetFitImageZoom();

    wxDECLARE_EVENT_TABLE();
private:
    void OnWindowSizeChange(wxSizeEvent& event);
    wxSize CalVSize();
    bool NeedReDrawing(const wxSize newSize);

    wxImage image;          // The wxImage object
    wxBitmap bitmap;        // Renderable bitmap
    int zoom;            // Current zoom level
    wxPoint panStart;       // Starting point for panning
    wxPoint scrollOffset;   // Scroll position for panning
    bool newPic;
    bool lockZoom;
    bool isPanning;         // Flag to indicate panning state
};

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/scrolwin.h>
#include "../utils/events.hpp"
#include "CropOptPanel.hpp"
#include "PictureFrame.hpp"

PRE_AND_POST_INCREMENT(PictureFrame::CropState)

PictureFrame::PictureFrame(wxWindow *parent, wxWindowID id)
        : wxScrolledWindow(parent, id) {
    SetScrollRate(5, 5);
    wxInitAllImageHandlers();
}
void PictureFrame::LoadImage(const wxString &fullPath) {
    if (image.LoadFile(fullPath)) {
        bitmap = wxBitmap(image);
        if (!lockZoom) SetZoom(-1);
        Refresh();
    } else {
        wxLogError("Failed to load image: %s", fullPath);
        CloseImage();
    }
}
double PictureFrame::GetFitImageZoom() {
    if (!bitmap.IsOk()) return 1;

    wxSize clientSize = GetClientSize();
    double scaleX = static_cast<double>(clientSize.GetWidth()) / bitmap.GetWidth();
    double scaleY = static_cast<double>(clientSize.GetHeight()) / bitmap.GetHeight();

    return std::min(scaleX, scaleY);  // Fit both width and height
}
void PictureFrame::SetZoom(int percentage) {
    zoom = percentage;
    UpdateScale();
}
void PictureFrame::ZoomIn(int rot) {
    SetZoom((static_cast<int>(std::floor(scale*4))+rot)*25);
}
void PictureFrame::ZoomOut(int rot) {
    SetZoom(std::max(25, ((static_cast<int>(std::ceil(scale*4)))-rot)*25));
}
void PictureFrame::UpdateScale() {
    if (!bitmap.IsOk()) return;
    auto oldScale = scale;
    scale = zoom<=0 ? GetFitImageZoom() : static_cast<double>(zoom)/100.;
    UpdateScaleOffset(scale, oldScale);
    if (oldScale != scale) needRedraw = true;
}
void PictureFrame::StartCrop() { cropState = START; }
void PictureFrame::OnDraw(wxDC& dc) {
    auto gc = wxGraphicsContext::CreateFromUnknownDC(dc);
    if (!gc) return;
    if (!bitmap.IsOk()) {
        dc.Clear();
        lastRendered = {0,0};
    } else {
        wxSize newSize = { static_cast<int>(bitmap.GetWidth()*scale),  static_cast<int>(bitmap.GetHeight()*scale) };
        if (newSize.GetX()<lastRendered.GetX() || newSize.GetY()<lastRendered.GetY()) dc.Clear();
        gc->Scale(scale, scale);
        gc->Translate(-scrollOffset.x, -scrollOffset.y);
        gc->DrawBitmap(bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
        lastRendered = newSize;
    }
    delete gc;
    needRedraw = false;
}
void PictureFrame::CloseImage() {
    if (bitmap.IsOk()) {
        bitmap = wxBitmap();
        SetVirtualSize(GetClientSize());
        Refresh();
    }
}
void PictureFrame::ClampScrollOffset() {
    if (!bitmap.IsOk()) return;

    wxSize clientSize = GetClientSize();

    int scaledWidth = static_cast<int>(bitmap.GetWidth() * scale);
    int scaledHeight = static_cast<int>(bitmap.GetHeight() * scale);

    // Ensure scrollOffset is within bounds
    scrollOffset.x = std::clamp(scrollOffset.x, 0, std::max(0, scaledWidth - clientSize.GetWidth()));
    scrollOffset.y = std::clamp(scrollOffset.y, 0, std::max(0, scaledHeight - clientSize.GetHeight()));
}
void PictureFrame::UpdateScaleOffset(double newScale, double oldScale) {
    auto s = newScale/oldScale;
    scrollOffset.x = static_cast<int>(s*scrollOffset.x);
    scrollOffset.y = static_cast<int>(s*scrollOffset.y);
    ClampScrollOffset();
}
void PictureFrame::OnMouseLeftDown(wxMouseEvent &event) {
    if (cropState == DONE) {
        isPanning = true;
        panStart = event.GetPosition();
        CaptureMouse();  // Capture mouse for dragging
    } else {
        auto [x, y] = event.GetPosition();
        if (cropState++ == START) {
            cropBox.x = x; cropBox.y = y;
        } else {
            cropBox.width = x - cropBox.x + 1; cropBox.height = y - cropBox.y + 1;
            wxCommandEvent event(DEFINE_CROP);
            event.SetClientData(&cropBox);
            wxPostEvent(this, event);
        }
    }
}
void PictureFrame::OnMouseDrag(wxMouseEvent& event) {
    if (isPanning && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentPos = event.GetPosition();
        wxPoint delta = panStart - currentPos;
        panStart = currentPos;
        // Adjust scroll position based on drag delta
        auto backup = scrollOffset;
        scrollOffset += delta;
        ClampScrollOffset();
        if (scrollOffset != backup) {
            needRedraw = true;
            Refresh();
        }
    }
    if (isPanning && event.LeftUp()) {
        isPanning = false;
        ReleaseMouse();
    }
}
void PictureFrame::OnMouseScroll(wxMouseEvent& event) {
    auto r = event.GetWheelRotation()/event.GetWheelDelta();
    if (r > 0) ZoomIn(r);
    else if (r < 0) ZoomOut(-r);
    if (needRedraw) Refresh();
}
void PictureFrame::OnWindowSizeChange(wxSizeEvent& event) {
    UpdateScale();
    Refresh();
}
void PictureFrame::OnEraseBackGround(wxEraseEvent& event) { event.Skip(); }

BEGIN_EVENT_TABLE(PictureFrame,wxScrolledWindow)
    EVT_ERASE_BACKGROUND(PictureFrame::OnEraseBackGround)
    EVT_MOUSEWHEEL(PictureFrame::OnMouseScroll)
    EVT_LEFT_DOWN(PictureFrame::OnMouseLeftDown)
    EVT_MOTION(PictureFrame::OnMouseDrag)
    EVT_LEFT_UP(PictureFrame::OnMouseDrag)
    EVT_SIZE(PictureFrame::OnWindowSizeChange)
END_EVENT_TABLE()
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/scrolwin.h>
#include "PictureFrame.hpp"

PictureFrame::PictureFrame(wxWindow *parent, wxWindowID id)
        : wxScrolledWindow(parent, id)
        , bitmap(0,0) {
    SetScrollRate(5, 5);
    wxInitAllImageHandlers();
}
void PictureFrame::LoadImage(const wxString &fullPath) {
    newPic = true;
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
}
wxSize PictureFrame::CalVSize() {
    if (!bitmap.IsOk()) return GetClientSize();
    // if (!bitmap.IsOk()) return {2500,2000};

    int virtualWidth = zoom<=0 ? static_cast<int>(GetFitImageZoom()*bitmap.GetWidth()) : bitmap.GetWidth()*zoom/100;
    int virtualHeight = zoom<=0 ? static_cast<int>(GetFitImageZoom()*bitmap.GetHeight()) : bitmap.GetHeight()*zoom/100;

    return wxSize(virtualWidth, virtualHeight);
}
void PictureFrame::OnDraw(wxDC& dc) {
    auto newSize = CalVSize();
    if (NeedReDrawing(newSize)) {
        auto gc = wxGraphicsContext::CreateFromUnknownDC(dc);
        if (!gc) return;

        if (!bitmap.IsOk()) {
            gc->SetBrush(*wxWHITE_BRUSH);
            gc->DrawRectangle(0, 0, newSize.GetWidth(), newSize.GetHeight());
        } else {
            auto sx = static_cast<double>(newSize.GetX())/bitmap.GetWidth(), sy = static_cast<double>(newSize.GetY())/bitmap.GetHeight();
            gc->Scale(sx, sy);
            gc->DrawBitmap(bitmap, 0, 0, image.GetWidth(), image.GetHeight());
        }
        SetVirtualSize(newSize);
        delete gc;
    }
    newPic = false;
}
void PictureFrame::CloseImage() {
    if (bitmap.IsOk()) {
        newPic = true;
        bitmap = wxBitmap();
        SetVirtualSize(GetClientSize());
        Refresh();
    }
}
bool PictureFrame::NeedReDrawing(const wxSize newSize) {
    wxLogInfo("newPic: %s\nvsize: %d,%d\nnew size: %d,%d"
        , newPic ? "Y" : "N"
        , GetVirtualSize().GetX(), GetVirtualSize().GetY()
        , newSize.GetX(), newSize.GetY()
    );
    return newPic || GetVirtualSize() != newSize;
}
void PictureFrame::OnMouseLeftDown(wxMouseEvent &event) {
    isPanning = true;
    panStart = event.GetPosition();
    scrollOffset = GetViewStart();  // Get current scroll position
    CaptureMouse();  // Capture mouse for dragging
}
void PictureFrame::OnMouseDrag(wxMouseEvent& event) {
    if (isPanning && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentPos = event.GetPosition();
        wxPoint delta = panStart - currentPos;

        // Adjust scroll position based on drag delta
        int x = std::max(0, scrollOffset.x + delta.x);
        int y = std::max(0, scrollOffset.y + delta.y);

        Scroll(x, y);  // Scroll to new position
    }
    if (isPanning && event.LeftUp()) {
        isPanning = false;
        ReleaseMouse();
    }
}
void PictureFrame::OnWindowSizeChange(wxSizeEvent& event) {
    if (zoom <= 0 || !bitmap.IsOk()) Refresh();
}

BEGIN_EVENT_TABLE(PictureFrame,wxScrolledWindow)
    EVT_LEFT_DOWN(PictureFrame::OnMouseLeftDown)
    EVT_MOTION(PictureFrame::OnMouseDrag)
    EVT_SIZE(PictureFrame::OnWindowSizeChange)
END_EVENT_TABLE()
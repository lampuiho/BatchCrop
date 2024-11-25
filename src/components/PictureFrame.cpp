#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/scrolwin.h>
#include "../common/events.hpp"
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
        if (ClampCropBox()) OnCropBoxUpdate();
        else Refresh();
    } else {
        wxLogError("Failed to load image: %s", fullPath);
        CloseImage();
    }
}
bool PictureFrame::SaveCrop(const wxString &fullPath, wxString &error) {
    if (image.IsOk()) {
        // checking cropBox
        if (cropBox.width<=2 || cropBox.height<=2) {
            error += "Crop dimension too small. Width: " + std::to_string(cropBox.width) + " Height: " + std::to_string(cropBox.height);
            if (cropBox.width <= 0 || cropBox.height <= 0) return false;
        }
        auto res = image.GetSubImage(cropBox).SaveFile(fullPath);
        if (!res) error += "wx failed to save image file.";
        // figure out jpeg lossless cropping in the future
        return res;
    } else {
        if (error.IsEmpty()) error += "No image opened.";
        return false;
    }
}
double PictureFrame::GetFitImageZoom() {
    if (!bitmap.IsOk()) return 1;

    wxSize clientSize = GetClientSize();
    double scaleX = static_cast<double>(clientSize.GetWidth()) / bitmap.GetWidth();
    double scaleY = static_cast<double>(clientSize.GetHeight()) / bitmap.GetHeight();

    return std::min(scaleX, scaleY);  // Fit both width and height
}
void PictureFrame::SetZoom(int percentage, bool refresh) {
    zoom = percentage;
    UpdateScale();
    if (refresh && needRedraw) Refresh();
}
void PictureFrame::ZoomIn(int rot) {
    SetZoom((static_cast<int>(std::floor(scale*4))+rot)*25, true);
}
void PictureFrame::ZoomOut(int rot) {
    SetZoom(std::max(25, ((static_cast<int>(std::ceil(scale*4)))-rot)*25), true);
}
void PictureFrame::UpdateScale() {
    if (!bitmap.IsOk()) return;
    auto oldScale = scale;
    scale = zoom<=0 ? GetFitImageZoom() : static_cast<double>(zoom)/100.;
    UpdateScaleOffset(scale, oldScale);
    if (oldScale != scale) {
        needRedraw = true;
        wxCommandEvent event(UPDATE_ZOOM, this->GetId());
        event.SetInt(scale*100);
        wxPostEvent(this, event);
    }
}
void PictureFrame::StartCrop() {
    if (bitmap.IsOk()) {
        cropBox = {0,0,0,0};
        cropState = START;
        SetFocus();
        OnCropBoxUpdate();
    }
}
void PictureFrame::SetCrop(wxRect box) {
    auto oldCrop = cropBox;
    cropBox = box;
    ClampCropBox();
    needRedraw = oldCrop != cropBox;
    cropState = DONE;
    if (box != cropBox) OnCropBoxUpdate();
    else if (needRedraw) Refresh();
}
void PictureFrame::OnDraw(wxDC& dc) {
    if (!bitmap.IsOk()) {
        dc.Clear();
    } else {
        auto clientSize = GetClientSize();
        wxBitmap tmpBitmap(clientSize);
        wxMemoryDC tmpDc(tmpBitmap);
        auto gc = wxGraphicsContext::Create(tmpDc);
        if (!gc) return;
        gc->Translate(-scrollOffset.x, -scrollOffset.y);
        gc->Scale(scale, scale);
        gc->DrawBitmap(bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
        gc->SetPen( wxColor(10,25,237,55) );
        gc->SetBrush( wxColor(10,25,237,55) );
        gc->DrawRectangle(cropBox.x, cropBox.y, cropBox.width, cropBox.height);
        dc.Blit(0,0,clientSize.x,clientSize.y,&tmpDc,0,0);
        delete gc;
    }
    needRedraw = false;
}
void PictureFrame::CloseImage() {
    if (bitmap.IsOk()) {
        cropState = DONE;
        image.Destroy();
        bitmap = wxBitmap();
        SetVirtualSize(GetClientSize());
        Refresh();
    }
}
void PictureFrame::ClampScrollOffset() {
    if (!bitmap.IsOk()) return;

    wxSize clientSize = GetClientSize();

    auto vSize = GetSize();
    // Ensure scrollOffset is within bounds
    scrollOffset.x = std::clamp(scrollOffset.x, 0, std::max(0, vSize.x-clientSize.x));
    scrollOffset.y = std::clamp(scrollOffset.y, 0, std::max(0, vSize.y-clientSize.y));
}
bool PictureFrame::ClampCropBox() {
    if (!bitmap.IsOk()) return false;
    auto backup = cropBox;
    auto size = bitmap.GetSize();
    cropBox.width = std::min(size.x, cropBox.width);
    cropBox.height = std::min(size.y, cropBox.height);
    cropBox.x = std::clamp(cropBox.x, 0, size.x - cropBox.width);
    cropBox.y = std::clamp(cropBox.y, 0, size.y - cropBox.height);
    if (backup != cropBox) return true;
    return false;
}
void PictureFrame::ClampCropBoxSize() {
    if (!bitmap.IsOk()) return;
    auto size = bitmap.GetSize();
    cropBox.width = std::clamp(cropBox.width, 0, size.x - cropBox.x); // x+w <= x_image
    cropBox.height = std::clamp(cropBox.height, 0, size.y - cropBox.y);
}
wxSize PictureFrame::GetSize() {
    return wxSize(static_cast<int>(bitmap.GetWidth()*scale), static_cast<int>(bitmap.GetHeight()*scale));
}
void PictureFrame::UpdateScaleOffset(double newScale, double oldScale) {
    auto s = newScale/oldScale;
    scrollOffset.x = static_cast<int>(s*scrollOffset.x);
    scrollOffset.y = static_cast<int>(s*scrollOffset.y);
    ClampScrollOffset();
}
bool PictureFrame::HitCrop(wxPoint pt) {
    wxPoint scaled(pt.x/scale+scrollOffset.x, pt.y/scale+scrollOffset.y);
    return cropBox.Contains(scaled);
}
void PictureFrame::OnMouseLeftDown(wxMouseEvent &event) {
    if (cropState == DONE) {
        if (HitCrop(event.GetPosition())) isDraggingCrop = true;
        else isPanning = true;
        panStart = event.GetPosition();
        CaptureMouse();  // Capture mouse for dragging
    } else {
        auto [x,y] = ToImageSpace(event.GetPosition());
        if (cropState++ == START) {
            panStart = {x,y};
            cropBox.x = x; cropBox.y = y;
            // Capture mouse position update for refreshing the crop size
            CaptureMouse();
        } else {
            ReleaseMouse();
            auto backup = cropBox;
            cropBox.width = x-panStart.x; cropBox.height = y-panStart.y;
            if (cropBox.width < 0) {
                cropBox.width = -cropBox.width;
                cropBox.x = panStart.x-cropBox.width;
            }
            if (cropBox.height < 0) {
                cropBox.height = -cropBox.height;
                cropBox.y = panStart.y-cropBox.height;
            }
            ClampCropBoxSize();
            if (cropBox!=backup) OnCropBoxUpdate();
        }
    }
}
void PictureFrame::OnMouseDrag(wxMouseEvent &event) {
    if ((isPanning || isDraggingCrop) && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentPos = event.GetPosition();
        wxPoint delta = isPanning ? panStart-currentPos : currentPos-panStart;
        panStart = currentPos;
        if (isPanning) {
            // Adjust scroll position based on drag delta
            auto backup = scrollOffset;
            scrollOffset += delta;
            ClampScrollOffset();
            if (scrollOffset != backup) Refresh();
        } else if (isDraggingCrop) {
            auto backup = wxPoint(cropBox.x, cropBox.y);
            // adjust delta to be image space;
            cropBox.x += delta.x/scale; cropBox.y += delta.y/scale;
            ClampCropBox();
            if (wxPoint(cropBox.x, cropBox.y)!=backup) OnCropBoxUpdate();
        }
    } else if ((isPanning || isDraggingCrop) && event.LeftUp()) {
        isPanning = false;
        isDraggingCrop = false;
        ReleaseMouse();
    }
    if (cropState == NEXT) {
        auto backup = cropBox;
        auto [x,y] = ToImageSpace(event.GetPosition());
        cropBox.width = x-panStart.x; cropBox.height = y-panStart.y;
        if (cropBox.width < 0) {
            cropBox.width = -cropBox.width;
            cropBox.x = panStart.x-cropBox.width;
        }
        if (cropBox.height < 0) {
            cropBox.height = -cropBox.height;
            cropBox.y = panStart.y-cropBox.height;
        }
        ClampCropBoxSize();
        if (cropBox!=backup) OnCropBoxUpdate();
    }
}
void PictureFrame::OnMouseScroll(wxMouseEvent &event) {
    auto r = event.GetWheelRotation()/event.GetWheelDelta();
    if (r > 0) ZoomIn(r);
    else if (r < 0) ZoomOut(-r);
}
void PictureFrame::OnWindowSizeChange(wxSizeEvent &event) {
    UpdateScale();
    Refresh();
}
void PictureFrame::OnEraseBackGround(wxEraseEvent &event) {}
void PictureFrame::OnCropBoxUpdate() {
    Refresh();
    wxCommandEvent event(DEFINE_CROP, this->GetId());
    event.SetClientData(&cropBox);
    wxPostEvent(this, event);
}
wxPoint PictureFrame::ToImageSpace(wxPoint pt) {
    pt -= scrollOffset;
    pt.x /= scale; pt.y /= scale;
    return pt;
}

BEGIN_EVENT_TABLE(PictureFrame,wxScrolledWindow)
    EVT_ERASE_BACKGROUND(PictureFrame::OnEraseBackGround)
    EVT_MOUSEWHEEL(PictureFrame::OnMouseScroll)
    EVT_LEFT_DOWN(PictureFrame::OnMouseLeftDown)
    EVT_MOTION(PictureFrame::OnMouseDrag)
    EVT_LEFT_UP(PictureFrame::OnMouseDrag)
    EVT_SIZE(PictureFrame::OnWindowSizeChange)
END_EVENT_TABLE()
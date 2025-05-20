#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <cstdio>

#include "ImGizmo.h"

#include "imgui.h"
#include "imgui_internal.h"

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

struct ImGizmoSpace {
    ImGuiID ID;
    ImGizmoMatrix viewMatrix;
    ImGizmoMatrix projMatrix;
    ImDrawList* drawList;
    ImRect frameRect;

    bool initialized;
};

struct ImGizmoContext {
    ImGizmoSpace currentSpace;
    // TODO: add global styling options, like in imgui
};

void InitializeContext(ImGizmoContext* context) {};

void FPU_MatrixF_x_MatrixF(const float* a, const float* b, float* r)
{
  r[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
  r[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
  r[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
  r[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

  r[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
  r[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
  r[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
  r[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

  r[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
  r[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
  r[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
  r[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

  r[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
  r[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
  r[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
  r[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

ImGizmoVec3 ImGizmoMatrix::Pos() const {
    ImGizmoVec3 vec;

    vec.x = this->m4x4[3][0];
    vec.y = this->m4x4[3][1];
    vec.z = this->m4x4[3][2];

    return vec;
}

ImGizmoMatrix ImGizmoMatrix::operator* (const ImGizmoMatrix& _matrix) const {
    ImGizmoMatrix out;
    FPU_MatrixF_x_MatrixF(this->m16, _matrix.m16, out.m16);
    return out;
}

ImGizmoVec3 ImGizmoVec3::Transform(const ImGizmoMatrix& _matrix) {
    ImGizmoVec4 in;
    ImGizmoVec3 out;

    in.x = this->x * _matrix.m4x4[0][0] + this->y * _matrix.m4x4[1][0] + this->z * _matrix.m4x4[2][0] + _matrix.m4x4[3][0];
    in.y = this->x * _matrix.m4x4[0][1] + this->y * _matrix.m4x4[1][1] + this->z * _matrix.m4x4[2][1] + _matrix.m4x4[3][1];
    in.z = this->x * _matrix.m4x4[0][2] + this->y * _matrix.m4x4[1][2] + this->z * _matrix.m4x4[2][2] + _matrix.m4x4[3][2];
    in.w = this->x * _matrix.m4x4[0][3] + this->y * _matrix.m4x4[1][3] + this->z * _matrix.m4x4[2][3] + _matrix.m4x4[3][3];

    in.x /= in.w;
    in.y /= in.w;
    in.z /= in.w;

    this->x = in.x;
    this->y = in.y;
    this->z = in.z;

    return *this;
}

ImGizmoVec4 ImGizmoVec4::Transform(const ImGizmoMatrix& _matrix) {
    ImGizmoVec4 out;

    out.x = this->x * _matrix.m4x4[0][0] + this->y * _matrix.m4x4[1][0] + this->z * _matrix.m4x4[2][0] + _matrix.m4x4[3][0];
    out.y = this->x * _matrix.m4x4[0][1] + this->y * _matrix.m4x4[1][1] + this->z * _matrix.m4x4[2][1] + _matrix.m4x4[3][1];
    out.z = this->x * _matrix.m4x4[0][2] + this->y * _matrix.m4x4[1][2] + this->z * _matrix.m4x4[2][2] + _matrix.m4x4[3][2];
    out.w = this->x * _matrix.m4x4[0][3] + this->y * _matrix.m4x4[1][3] + this->z * _matrix.m4x4[2][3] + _matrix.m4x4[3][3];

    out.x /= out.w;
    out.y /= out.w;
    out.z /= out.w;

    this->x = out.x;
    this->y = out.y;
    this->z = out.z;
    this->w = out.w;

    return *this;
}

namespace IMGIZMO_NAMESPACE {

    // ----------------------------------------------------------------------------------------------------------------------------------
    // CONTEXT SECTION
    // ----------------------------------------------------------------------------------------------------------------------------------

    ImGizmoContext* CreateContext() {
        ImGizmoContext* context = IM_NEW(ImGizmoContext)();
        InitializeContext(context);
        context->currentSpace.initialized = false;
        if (GImGizmo == nullptr) {
            SetCurrentContext(context);
        }
        return context;
    }

    void DestroyContext(ImGizmoContext* context) {
        if (context == nullptr) {
            context = GImGizmo;
        }
        if (context == GImGizmo) {
            SetCurrentContext(nullptr);
        }
        IM_DELETE(GImGizmo);
    }

    ImGizmoContext* GetCurrentContext() {
        return GImGizmo;
    }

    void SetCurrentContext(ImGizmoContext* context) {
        GImGizmo = context;
    }

    // ----------------------------------------------------------------------------------------------------------------------------------
    // FUNCTIONS SECTION
    // ----------------------------------------------------------------------------------------------------------------------------------

    bool Begin(const char* _id, float* _view, float* _proj, const ImVec2& _size) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == false, "You are trying to create a ImGizmo space inside of an ImGizmo space, which isn't allowed");

        ImGuiContext& g = *GImGui;
        ImGizmoContext& gz = *GImGizmo;
        ImGizmoSpace& current = gz.currentSpace;

        const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
                            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);

        if(ImGui::Begin(_id, NULL, flags)) {
            ImGuiWindow* window = g.CurrentWindow;

            current.initialized = true;
            current.viewMatrix = _view;
            current.projMatrix = _proj;
            current.drawList = window->DrawList;
            current.frameRect = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImGui::GetMainViewport()->Size);

            ImGui::End();
        }

        return current.initialized;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "You are trying to end the current ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;

        // Reset current ImGizmo space
        gz.currentSpace.initialized = false;
    }

    // ----------------------------------------------------------------------------------------------------------------------------------
    // DRAW CALLS
    // ----------------------------------------------------------------------------------------------------------------------------------

    bool DrawPoint(const char* _id, ImGizmoVec3 point) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImGizmoMatrix& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImGizmoMatrix& projMatrix = GImGizmo->currentSpace.projMatrix;

        // Push translation gizmo bounding boxes data
        // Push translation gizmo rending data

        //TODO: move rendering to end, because of depth. (We can't know if something is behind or worse infront when we call a draw function);
        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;

        // Draw debug point
        point.Transform(viewMatrix);
        point.Transform(projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;

        point.x = ((point.x + 1.f) / 2.f ) * max.x;
        point.y = (1 - ((point.y + 1.f) / 2.f )) * max.y;

        bool hovering = false;
        auto mousePos = ImGui::GetIO().MousePos;
        auto distance = pow(mousePos.x - point.x, 2.f) + pow(mousePos.y - point.y,2.f);
        if (distance < 5.f)
        {
            hovering = true;
        }

        if (hovering) {
            drawList.AddCircle({point.x, point.y}, 1.f, ImGui::GetColorU32({.25f, .25f, .25f, 1.f}));
        }
        else {
            drawList.AddCircle({point.x, point.y}, 1.f, ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));
        }

        return hovering;
    }

    bool DrawLine(const char* _id, ImGizmoVec3 point1, ImGizmoVec3 point2) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImGizmoMatrix& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImGizmoMatrix& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;

        // Draw debug point
        point1.Transform(viewMatrix);
        point1.Transform(projMatrix);
        point2.Transform(viewMatrix);
        point2.Transform(projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;

        ImVec2 screenPoint1 = {};
        ImVec2 screenPoint2 = {};
        screenPoint1.x = ((point1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((point1.y + 1.f) / 2.f )) * max.y;
        screenPoint2.x = ((point2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((point2.y + 1.f) / 2.f )) * max.y;

        bool hovering = false;
        auto mousePos = ImGui::GetIO().MousePos;
        auto point1ToMouse = mousePos - screenPoint1;
        auto point1ToPoint2 = screenPoint2 - screenPoint1;

        auto ADot = point1ToMouse.x * point1ToPoint2.x + point1ToMouse.y * point1ToPoint2.y;
        auto BDot = point1ToPoint2.x * point1ToPoint2.x + point1ToPoint2.y * point1ToPoint2.y;
        auto value = (ADot / BDot) > 1.f ? 1.f : (ADot / BDot) < 0.f ? 0.f : (ADot / BDot);
        auto projection = ImVec2(value * point1ToPoint2.x, value * point1ToPoint2.y) + screenPoint1;

        auto distance = pow(mousePos.x - projection.x, 2.f) + pow(mousePos.y - projection.y,2.f);
        if (distance < 5.f)
        {
            hovering = true;
        }

        if (hovering) {
            drawList.AddLine({screenPoint1.x, screenPoint1.y}, {screenPoint2.x, screenPoint2.y}, ImGui::GetColorU32({.25f, .25f, .25f, 1.f}));
        }
        else {
            drawList.AddLine({screenPoint1.x, screenPoint1.y}, {screenPoint2.x, screenPoint2.y}, ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));
        }

        return hovering;
    }
};

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
    ImMat44 viewMatrix;
    ImMat44 projMatrix;
    ImDrawList* drawList;
    ImRect frameRect;

    bool initialized;
};

struct ImGizmoContext {
    ImGizmoSpace currentSpace;
    // TODO: add global styling options, like in imgui
};

void InitializeContext(ImGizmoContext* context) {};

static inline ImMat44 FPU_MatrixF_x_MatrixF(const ImMat44& _lhs, const ImMat44& _rhs)
{
    ImMat44 out;

    out.m16[0] = _lhs.m16[0] * _rhs.m16[0] + _lhs.m16[1] * _rhs.m16[4] + _lhs.m16[2] * _rhs.m16[8] + _lhs.m16[3] * _rhs.m16[12];
    out.m16[1] = _lhs.m16[0] * _rhs.m16[1] + _lhs.m16[1] * _rhs.m16[5] + _lhs.m16[2] * _rhs.m16[9] + _lhs.m16[3] * _rhs.m16[13];
    out.m16[2] = _lhs.m16[0] * _rhs.m16[2] + _lhs.m16[1] * _rhs.m16[6] + _lhs.m16[2] * _rhs.m16[10] + _lhs.m16[3] * _rhs.m16[14];
    out.m16[3] = _lhs.m16[0] * _rhs.m16[3] + _lhs.m16[1] * _rhs.m16[7] + _lhs.m16[2] * _rhs.m16[11] + _lhs.m16[3] * _rhs.m16[15];

    out.m16[4] = _lhs.m16[4] * _rhs.m16[0] + _lhs.m16[5] * _rhs.m16[4] + _lhs.m16[6] * _rhs.m16[8] + _lhs.m16[7] * _rhs.m16[12];
    out.m16[5] = _lhs.m16[4] * _rhs.m16[1] + _lhs.m16[5] * _rhs.m16[5] + _lhs.m16[6] * _rhs.m16[9] + _lhs.m16[7] * _rhs.m16[13];
    out.m16[6] = _lhs.m16[4] * _rhs.m16[2] + _lhs.m16[5] * _rhs.m16[6] + _lhs.m16[6] * _rhs.m16[10] + _lhs.m16[7] * _rhs.m16[14];
    out.m16[7] = _lhs.m16[4] * _rhs.m16[3] + _lhs.m16[5] * _rhs.m16[7] + _lhs.m16[6] * _rhs.m16[11] + _lhs.m16[7] * _rhs.m16[15];

    out.m16[8] = _lhs.m16[8] * _rhs.m16[0] + _lhs.m16[9] * _rhs.m16[4] + _lhs.m16[10] * _rhs.m16[8] + _lhs.m16[11] * _rhs.m16[12];
    out.m16[9] = _lhs.m16[8] * _rhs.m16[1] + _lhs.m16[9] * _rhs.m16[5] + _lhs.m16[10] * _rhs.m16[9] + _lhs.m16[11] * _rhs.m16[13];
    out.m16[10] = _lhs.m16[8] * _rhs.m16[2] + _lhs.m16[9] * _rhs.m16[6] + _lhs.m16[10] * _rhs.m16[10] + _lhs.m16[11] * _rhs.m16[14];
    out.m16[11] = _lhs.m16[8] * _rhs.m16[3] + _lhs.m16[9] * _rhs.m16[7] + _lhs.m16[10] * _rhs.m16[11] + _lhs.m16[11] * _rhs.m16[15];

    out.m16[12] = _lhs.m16[12] * _rhs.m16[0] + _lhs.m16[13] * _rhs.m16[4] + _lhs.m16[14] * _rhs.m16[8] + _lhs.m16[15] * _rhs.m16[12];
    out.m16[13] = _lhs.m16[12] * _rhs.m16[1] + _lhs.m16[13] * _rhs.m16[5] + _lhs.m16[14] * _rhs.m16[9] + _lhs.m16[15] * _rhs.m16[13];
    out.m16[14] = _lhs.m16[12] * _rhs.m16[2] + _lhs.m16[13] * _rhs.m16[6] + _lhs.m16[14] * _rhs.m16[10] + _lhs.m16[15] * _rhs.m16[14];
    out.m16[15] = _lhs.m16[12] * _rhs.m16[3] + _lhs.m16[13] * _rhs.m16[7] + _lhs.m16[14] * _rhs.m16[11] + _lhs.m16[15] * _rhs.m16[15];

    return out;
}

// Helpers: ImVec3 Functions
static inline ImVec3 Transform(ImVec3& _vec, const ImMat44& _matrix) {
    ImVec4 in;

    in.x = _vec.x * _matrix.m4x4[0][0] + _vec.y * _matrix.m4x4[1][0] + _vec.z * _matrix.m4x4[2][0] + _matrix.m4x4[3][0];
    in.y = _vec.x * _matrix.m4x4[0][1] + _vec.y * _matrix.m4x4[1][1] + _vec.z * _matrix.m4x4[2][1] + _matrix.m4x4[3][1];
    in.z = _vec.x * _matrix.m4x4[0][2] + _vec.y * _matrix.m4x4[1][2] + _vec.z * _matrix.m4x4[2][2] + _matrix.m4x4[3][2];
    in.w = _vec.x * _matrix.m4x4[0][3] + _vec.y * _matrix.m4x4[1][3] + _vec.z * _matrix.m4x4[2][3] + _matrix.m4x4[3][3];

    in.x /= in.w;
    in.y /= in.w;
    in.z /= in.w;

    _vec.x = in.x;
    _vec.y = in.y;
    _vec.z = in.z;

    return _vec;
}

namespace ImGizmo {

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

    bool Begin(const char* _id, ImMat44 _view, ImMat44 _proj, const ImVec2& _size) {
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

    bool DrawPoint(const char* _id, ImVec3 point) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        Transform(point, viewMatrix);
        Transform(point, projMatrix);

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

    bool DrawLine(const char* _id, ImVec3 point1, ImVec3 point2) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        bool mouseDown = ImGui::GetIO().MouseDown[ImGuiMouseButton_Left];

        Transform(point1, viewMatrix);
        Transform(point1, projMatrix);
        Transform(point2, viewMatrix);
        Transform(point2, projMatrix);

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

        auto color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f});
        if (hovering) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        drawList.PathLineTo(screenPoint1);
        drawList.PathLineTo(screenPoint2);
        drawList.AddPolyline(drawList._Path.Data, drawList._Path.Size, color, ImDrawFlags_None, 1.f);
        drawList.PathClear();

        return hovering && mouseDown;
    }

    bool DrawTriangle(const char* _id, ImVec3 point1, ImVec3 point2, ImVec3 point3) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        bool mouseDown = ImGui::GetIO().MouseDown[ImGuiMouseButton_Left];

        Transform(point1, viewMatrix);
        Transform(point1, projMatrix);
        Transform(point2, viewMatrix);
        Transform(point2, projMatrix);
        Transform(point3, viewMatrix);
        Transform(point3, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;

        ImVec2 screenPoint1 = {};
        ImVec2 screenPoint2 = {};
        ImVec2 screenPoint3 = {};
        screenPoint1.x = ((point1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((point1.y + 1.f) / 2.f )) * max.y;
        screenPoint2.x = ((point2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((point2.y + 1.f) / 2.f )) * max.y;
        screenPoint3.x = ((point3.x + 1.f) / 2.f ) * max.x;
        screenPoint3.y = (1 - ((point3.y + 1.f) / 2.f )) * max.y;

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

        auto color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f});
        if (hovering) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        drawList.PathLineTo(screenPoint1);
        drawList.PathLineTo(screenPoint2);
        drawList.PathLineTo(screenPoint3);
        drawList.PathLineTo(screenPoint1);
        drawList.AddPolyline(drawList._Path.Data, drawList._Path.Size, color, ImDrawFlags_None, 1.f);
        drawList.PathClear();

        return hovering && mouseDown;
    }

    bool TranslateGizmo(const char *_id, ImVec3& _translate) {
        bool active = false;

        bool xAxis = DrawLine("x_axis", _translate, _translate + ImVec3(1.f, 0.f, 0.f));
        xAxis = xAxis ^ DrawTriangle("x_blade_1", _translate + ImVec3(1.f, 0.f, 0.f), _translate + ImVec3(1.2f, 0.f, 0.f), _translate + ImVec3(1.f, 0.1f, 0.f));
        xAxis = xAxis ^ DrawTriangle("x_blade_2", _translate + ImVec3(1.f, 0.f, 0.f), _translate + ImVec3(1.2f, 0.f, 0.f), _translate + ImVec3(1.f, 0.f, 0.1f));
        bool yAxis = DrawLine("y_axis", _translate, _translate + ImVec3(0.f, 1.f, 0.f));
        yAxis = yAxis ^ DrawTriangle("y_blade_1", _translate + ImVec3(0.f, 1.f, 0.f), _translate + ImVec3(0.f, 1.2f, 0.f), _translate + ImVec3(0.f, 1.f, 0.1f));
        yAxis = yAxis ^ DrawTriangle("y_blade_2", _translate + ImVec3(0.f, 1.f, 0.f), _translate + ImVec3(0.f, 1.2f, 0.f), _translate + ImVec3(0.1f, 1.f, 0.f));
        bool zAxis = DrawLine("z_axis", _translate, _translate + ImVec3(0.f, 0.f, 1.f));
        zAxis = zAxis ^ DrawTriangle("z_blade_1", _translate + ImVec3(0.f, 0.f, 1.f), _translate + ImVec3(0.f, 0.f, 1.2f), _translate + ImVec3(0.1f, 0.f, 1.f));
        zAxis = zAxis ^ DrawTriangle("z_blade_2", _translate + ImVec3(0.f, 0.f, 1.f), _translate + ImVec3(0.f, 0.f, 1.2f), _translate + ImVec3(0.f, 0.1f, 1.f));

        if (xAxis) {
            _translate.x += ImGui::GetIO().MouseDelta.x;
        }
        if (yAxis) {
            _translate.y += ImGui::GetIO().MouseDelta.y;
        }
        if (zAxis) {
            _translate.z += ImGui::GetIO().MouseDelta.y;
        }

        active = active ^ xAxis;
        active = active ^ yAxis;
        active = active ^ zAxis;

        return active;
    }
};

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "ImGizmo.h"
#include "ImGizmo_Internal.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

void InitializeContext(ImGizmoContext* context) {};

namespace IMGIZMO_NAMESPACE {
    ImGizmoContext* CreateContext() {
        ImGizmoContext* context = IM_NEW(ImGizmoContext)();
        InitializeContext(context);
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

    bool Begin(const char *_id, float *_view, float *_proj, const ImVec2& _size) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;

        const ImGuiID ID = window->GetID(_id);
        ImGizmoSpace* current = &gz.gizmoSpaces.emplace();

        current->projMatrix = _proj;
        current->viewMatrix = _view;

        ImVec2 frameSize = _size;
        if(fabsf(frameSize.x + frameSize.y) < 0.0001f) {
            // We take the full size of the window if no size (or a zero vector) is given
            frameSize = window->ContentSize;
        }

        current->frameRect = ImRect(window->DC.CursorPos, window->DC.CursorPos + frameSize);
        ImGui::ItemSize(current->frameRect);
        if (!ImGui::ItemAdd(current->frameRect, current->ID, &current->frameRect)) {
            return false;
        }

        ImGui::PushID(ID);

        return true;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(!GImGizmo->gizmoSpaces.empty(), "Current ImGizmo Scope is empty. Did you call ImGizmo::Begin()?");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;

        ImDrawList& drawList = *window->DrawList;
        ImGizmoSpace* current = &gz.gizmoSpaces.back();

        ImGui::PushClipRect(current->frameRect.Min, current->frameRect.Max, true);

        //TODO: add the rendering of object
    
        gz.gizmoSpaces.pop();
    }

    // -----------------
    // RENDERING SECTION
    // -----------------

    bool DrawPosition(float* _matrix) {
        return false;
    }

    bool DrawPosition(float* _x, float* _y, float* _z) {
        ImGizmoMatrix matrix = ImGizmoMatrix();
        matrix.m4x4[3][0] = *_x;
        matrix.m4x4[3][1] = *_y;
        matrix.m4x4[3][2] = *_z;
        if(DrawPosition(&matrix.m16[0])) {
            *_x = matrix.m4x4[3][0];
            *_y = matrix.m4x4[3][1];
            *_z = matrix.m4x4[3][2];
            return true;
        } else {
            return false;
        }
    }
};

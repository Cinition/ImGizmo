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
        gz.currentGizmoSpace = gz.gizmoSpaces.GetOrAddByKey(ID);
        ImGizmoSpace* current = gz.currentGizmoSpace;

        current->projMatrix = convertToMatrix(_proj);
        current->viewMatrix = convertToMatrix(_view);

        // add imgizmo scope window to imgui
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

        return true;
    }
};

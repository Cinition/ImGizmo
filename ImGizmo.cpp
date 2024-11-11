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
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying to create a ImGizmo space inside of an ImGizmo space, which isn't allowed");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;

        const ImGuiID ID = window->GetID(_id);
        ImGizmoSpace* current = IM_NEW(ImGizmoSpace);
        gz.currentSpace = current;

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

        return true;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying to end the current ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;

        ImDrawList& drawList = *window->DrawList;
        ImGizmoSpace* current = &gz.currentSpace;

        ImGui::PushClipRect(current->frameRect.Min, current->frameRect.Max, true);

        //TODO: add the rendering of object


        // Reset current ImGizmo space
        IM_DELETE(&gz.currentSpace);
        gz.currentSpace = nullptr;
    }

    // -----------------
    // RENDERING SECTION
    // -----------------

    bool DrawTranslation(const char* _id, float* _matrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        // 1 => Calculate/Create hitbox around translation gizmo.
        // 2 => Detect mouse interaction (hovering & pressing).
        // 3 => Push data to space context for rendering at end.
        GImGizmo->scopeContext.drawTranslation = true;

        return false;
    }

    bool DrawScaling(const char* _id, float* _matrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        return false;
    }

    bool DrawRotation(const char* _id, float* _matrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        return false;
    }

    bool DrawPoint(const char* _id, float* _matrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        return false;
    }
};

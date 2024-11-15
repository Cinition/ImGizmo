#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cstdio>
#endif

#include "ImGizmo.h"
#include "ImGizmo_Math.h"
#include "ImGizmo_Internal.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <algorithm>

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

void InitializeContext(ImGizmoContext* context) {};

namespace IMGIZMO_NAMESPACE {

    // ---------------
    // CONTEXT SECTION
    // ---------------

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

    // -----------------
    // FUNCTIONS SECTION
    // -----------------

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

    // -----------------
    // RENDERING SECTION
    // -----------------

    bool DrawTranslation(const char* _id, float* _matrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImGizmoMatrix matrix = ImGizmoMatrix(_matrix);

        // TODO: Move to global context styling
        const float handleHeight = 10.f;
        const float handleWidth = 2.f;

        const ImGizmoMatrix& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImGizmoMatrix& projMatrix = GImGizmo->currentSpace.projMatrix;
        const ImGizmoMatrix modelView = viewMatrix * projMatrix;

        for(int i = 0; i < 3; ++i) {
            // Create axis aligned bounding box, for fast coarse initial check
            // Create axis bounding box, for slower accurate secondary check
        }

        // Push translation gizmo bounding boxes data
        // Push translation gizmo rending data

        //TODO: move rendering to end, because of hovering. (We can't know if something is behind or worse infront when we call a draw function);
        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;

        // Draw debug point
        ImGizmoVec point1 = ImGizmoVec(1.f, 2.f, 10.f, 0.f);
        point1.Transform(viewMatrix);
        point1.Transform(projMatrix);

        //point1.x += GImGizmo->currentSpace.frameRect.Max.x * 0.5f;
        //point1.y += GImGizmo->currentSpace.frameRect.Max.y * 0.5f;

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        point1.x = (point1.x * 0.5f * max.y);
        point1.y = (point1.y * 0.5f * max.x);

        drawList.AddCircle({point1.x, point1.y}, 1.f, ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f}));

        return false;
    }
};

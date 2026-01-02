#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cstring>
#endif

#include "imgizmo.h"
#include "imgizmo_internal.h"
#include "imgui.h"
#include "imgui_internal.h"

//-----------------------------------------------------------------------------
// [SECTION] Math constants
//-----------------------------------------------------------------------------
constexpr float PI = 3.1415926536;

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

namespace ImGizmo
{
    ImGizmoContext* CreateContext() {
        ImGizmoContext* context = IM_NEW(ImGizmoContext)();
        //InitializeContext(context);
        context->currentSpace.initialized = false;
        if (GImGizmo == nullptr)
        {
            SetCurrentContext(context);
        }
        return context;
    }

    void DestroyContext(ImGizmoContext* context) {
        if (context == nullptr)
        {
            context = GImGizmo;
        }
        if (context == GImGizmo)
        {
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

    bool Begin(const char* id, const float* viewMatrix, const float* projectionMatrix) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == false, "You are trying to create a ImGizmo space inside of an ImGizmo space, which isn't allowed");

        ImGuiContext& g = *GImGui;
        ImGizmoContext& gz = *GImGizmo;
        ImGizmoSpace& currentSpace = gz.currentSpace;

        const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
                            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);

        if(ImGui::Begin(id, NULL, flags)) {
            ImGuiWindow* window = g.CurrentWindow;

            currentSpace.initialized = true;
            currentSpace.viewMatrix = (ImGizmoMatrix*)viewMatrix;
            currentSpace.projMatrix = (ImGizmoMatrix*)projectionMatrix;
            currentSpace.drawList = window->DrawList;
            currentSpace.frameRect = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImGui::GetMainViewport()->Size);

            ImGui::End();
        }

        return currentSpace.initialized;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "You are trying to end the current ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;
        ImDrawList& drawList = *gz.currentSpace.drawList;

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mousePos = io.MousePos;
        bool mouseDown = io.MouseDown[ImGuiMouseButton_Left];

        const char* activeID = gz.currentSpace.activeID;
        const char* hoverID = (activeID != nullptr ? gz.currentSpace.hoverID : nullptr);

        // Reset current ImGizmo space
        gz.currentSpace.initialized = false;

        gz.currentSpace.lastMousePos = mousePos;
    }

    void DrawPoint(float x, float y, float z) {
        ImGizmoContext& gz = *GImGizmo;
        ImDrawList& drawList = *gz.currentSpace.drawList;

        drawList.   
    }
};


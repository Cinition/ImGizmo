#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cmath>
#include <cstring>
#endif

#include "imgizmo.h"
#include "imgizmo_internal.h"
#include "imgizmo_draw.h"
#include "imgui.h"
#include "imgui_internal.h"

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

namespace ImGizmo
{
    ImGizmoContext* CreateContext() {
        ImGizmoContext* context = IM_NEW(ImGizmoContext)();
        //InitializeContext(context);
        context->currentSpace = nullptr;
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
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace == nullptr, "You are trying to create a ImGizmo space inside of an ImGizmo space, which isn't allowed ATM");

        ImGuiContext& g = *GImGui;
        ImGizmoContext& gz = *GImGizmo;

        const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
                            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;

        ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);

        if(ImGui::Begin(id, NULL, flags)) {
            ImGizmoSpace* currentSpace = new ImGizmoSpace();
            currentSpace->viewMatrix = (ImGizmoMatrix*)viewMatrix;
            currentSpace->projMatrix = (ImGizmoMatrix*)projectionMatrix;
            currentSpace->drawList = ImGui::GetCurrentWindow()->DrawList;
            currentSpace->frameRect = ImRect(ImGui::GetCurrentWindow()->DC.CursorPos, ImGui::GetCurrentWindow()->DC.CursorPos + ImGui::GetMainViewport()->Size);

            gz.currentSpace = currentSpace;

            currentSpace->imGizmoDrawList._drawListFlags = ImGui::GetCurrentWindow()->DrawList->Flags;
            currentSpace->imGizmoDrawList._sharedData = ImGui::GetDrawListSharedData();

            ImGui::End();
            return true;
        }

        return false;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying to end the current ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;
        ImGizmoDrawList& drawList = gz.currentSpace->imGizmoDrawList;

        const char* activeID = gz.currentSpace->activeID;
        const char* hoverID = (activeID != nullptr ? gz.currentSpace->hoverID : nullptr);

        drawList.PushToDrawList(gz.currentSpace->drawList);
        drawList.Reset();

        delete GImGizmo->currentSpace;
        GImGizmo->currentSpace = nullptr;
    }

    void PushPositionOffset(const float x, const float y, const float z)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        GImGizmo->positionOffsets.push_back(ImGizmoVec3(x, y, z));
    }

    void PopPositionOffset()
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        IM_ASSERT_USER_ERROR(!GImGizmo->positionOffsets.empty(), "You are trying to pop on empty PositionOffsets vector");

        GImGizmo->positionOffsets.pop_back();
    }

    void PushEulerRotationOffset(const float x, const float y, const float z)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        GImGizmo->rotationOffsets.push_back(ImGizmoQuaternion::FromEuler(x, y, z));
    }

    void PushQuatRotationOffset(const float x, const float y, const float z, const float w)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        GImGizmo->rotationOffsets.push_back(ImGizmoQuaternion(x, y, z, w));
    }

    void PopRotationOffset()
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        IM_ASSERT_USER_ERROR(!GImGizmo->rotationOffsets.empty(), "You are trying to pop on empty RotationOffsets vector");

        GImGizmo->rotationOffsets.pop_back();
    }

    void GizmoTranslate(const char* id, float* x, float* y, float* z)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");

        ImRect rect = GImGizmo->currentSpace->frameRect;
        ImGizmoDrawList& drawlist = GImGizmo->currentSpace->imGizmoDrawList;

        // point

        // arrow   Up
        // arrow Left
        // arrow   At
        
        ImVec2 mid = {rect.GetWidth() / 2.f, rect.GetHeight() / 2.f};
        drawlist.ReserveMesh(3,3);
        drawlist._vertWritePtr[0].pos.x = mid.x + -100.f;
        drawlist._vertWritePtr[0].pos.y = mid.y + 100.f;
        drawlist._vertWritePtr[0].uv = drawlist._sharedData->TexUvWhitePixel;
        drawlist._vertWritePtr[0].col = ImGui::GetColorU32({0, 0, 1, 1});
        drawlist._vertWritePtr[1].pos.x = mid.x + 100.f;
        drawlist._vertWritePtr[1].pos.y = mid.y + 100.f;
        drawlist._vertWritePtr[1].uv = drawlist._sharedData->TexUvWhitePixel;
        drawlist._vertWritePtr[1].col = ImGui::GetColorU32({0, 1, 0, 1});
        drawlist._vertWritePtr[2].pos.x = mid.x;
        drawlist._vertWritePtr[2].pos.y = mid.y + -100.f;
        drawlist._vertWritePtr[2].uv = drawlist._sharedData->TexUvWhitePixel;
        drawlist._vertWritePtr[2].col = ImGui::GetColorU32({1, 0, 0, 1});
        drawlist._vertWritePtr += 3;
        drawlist._idxWritePtr[0] = (ImDrawIdx)(drawlist._vertCurrentIdx);
        drawlist._idxWritePtr[1] = (ImDrawIdx)(drawlist._vertCurrentIdx + 1);
        drawlist._idxWritePtr[2] = (ImDrawIdx)(drawlist._vertCurrentIdx + 2);
        drawlist._idxWritePtr += 3;
        drawlist._vertCurrentIdx += 3;
        drawlist._zWritePtr[0] = 1.f;
        drawlist._zWritePtr += 1;
    }

    void GizmoRotateEuler(const char* id, float* x, float* y, float* z)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");
    }

    void GizmoRotateQuat(const char* id, float* x, float* y, float* z, float* w)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");
    }

    void GizmoScale(const char* id, float* x, float* y, float* z)
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace != nullptr, "You are trying call a function inside an ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");
    }
};

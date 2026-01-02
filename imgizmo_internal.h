#include "imgizmo.h"
#include "imgui_internal.h"

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoSpace
//-----------------------------------------------------------------------------

struct ImGizmoSpace
{
    ImGuiID ID;
    ImGizmoMatrix* viewMatrix;
    ImGizmoMatrix* projMatrix;
    ImDrawList* drawList;
    ImRect frameRect;
    const char* hoverID = nullptr;
    const char* activeID = nullptr;
    ImVec2 initialMousePos = {};
    ImVec2 lastMousePos = {};
    bool firstMouseDownFrame = true;
    bool initialized = false;
    float nearClip = 0.1f;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoContext
//-----------------------------------------------------------------------------

struct ImGizmoContext
{
    ImGizmoSpace currentSpace;
};

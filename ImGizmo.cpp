#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cstring>
#include <vector>
#endif

#include <cstdio>
#include "ImGizmo.h"
#include "imgui.h"
#include "imgui_internal.h"

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

enum ImGizmoDrawType {
    ImGizmoDrawType_Point,
    ImGizmoDrawType_Line,
    ImGizmoDrawType_COUNT,
};

struct ImGizmoDraw {
    union {
        struct {
            ImVec2 pos;
            float radius;
        } Point;
        struct {
            ImVec2 pos1;
            ImVec2 pos2;
        } Line;
    };

    const char* id;
    ImU32 col;
    ImU32 flags;
    ImGizmoDrawType type;
    float depth;
};

struct ImGizmoSpace {
    ImGuiID ID;
    ImMat44 viewMatrix;
    ImMat44 projMatrix;
    ImDrawList* drawList;
    ImRect frameRect;
    std::vector<ImGizmoDraw> renderList;

    const char* hoverID = nullptr;
    const char* activeID = nullptr;
    bool initialized = false;
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
static inline ImVec3 ImVec3Transform(ImVec3& _vec, const ImMat44& _matrix) {
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

static inline ImVec3 ImVec3Cross(const ImVec3& _vec1, const ImVec3& _vec2) {
    ImVec3 out;
    out.x = _vec1.y * _vec2.z - _vec1.z * _vec2.y;
    out.y = _vec1.z * _vec2.x - _vec1.x * _vec2.z;
    out.z = _vec1.x * _vec2.y - _vec1.y * _vec2.x;
    return out;
}

static inline ImVec3 ImVec3Normalize(const ImVec3& _vec) {
    ImVec3 out = _vec;
    float mag = sqrt(pow(_vec.x, 2.f) + pow(_vec.y, 2.f) + pow(_vec.z, 2.f));
    if (mag != 0.f) {
        out.x /= mag;
        out.y /= mag;
        out.z /= mag;
    }
    return out;
}

static inline float ImVec2Dot(const ImVec2& _vec1, const ImVec2& _vec2) {
    return _vec1.x * _vec2.x + _vec1.y * _vec2.y;
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
        ImDrawList& drawList = *gz.currentSpace.drawList;

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mousePos = io.MousePos;
        bool mouseDown = io.MouseDown[ImGuiMouseButton_Left];

        const char* hoverID = nullptr;
        for(const auto& object : gz.currentSpace.renderList) {
            IM_ASSERT_USER_ERROR(object.type != ImGizmoDrawType_COUNT, "ImGizmo is trying to render an unknown object.");

            switch (object.type) {
                case ImGizmoDrawType_Point:
                    drawList.AddCircle(object.Point.pos, object.Point.radius, object.col);
                    break;
                case ImGizmoDrawType_Line:
                    drawList.AddLine(object.Line.pos1, object.Line.pos2, object.col);
                    break;
                default: break;
            }

            if (hoverID != nullptr) {
                continue;
            }

            switch (object.type) {
                case ImGizmoDrawType_Point: {
                        float mag = pow(mousePos.x - object.Point.pos.x, 2.f) + pow(mousePos.y - object.Point.pos.y, 2.f);

                        if (mag < 5.f) {
                            hoverID = object.id;
                        }
                        break;
                    }
                case ImGizmoDrawType_Line: {
                        ImVec2 pos1ToMouse = mousePos - object.Line.pos1;
                        ImVec2 pos1ToPos2 = object.Line.pos2 - object.Line.pos1;

                        float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
                        float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
                        float value = dot1 / dot2;
                        float clamp = value > 1.f ? 1.f : value < 0.f ? 0.f : value;
                        ImVec2 proj = pos1ToPos2 * clamp + object.Line.pos1;
                        float mag = pow(mousePos.x - proj.x, 2.f) + pow(mousePos.y - proj.y, 2.f);

                        if (mag < 5.f) {
                            hoverID = object.id;
                        }
                        break;
                    }
                default: break;
            }
        }

        // Reset current ImGizmo space
        gz.currentSpace.initialized = false;
        gz.currentSpace.renderList.clear();
        gz.currentSpace.hoverID = hoverID;
        gz.currentSpace.activeID = (mouseDown && hoverID != nullptr ? hoverID : nullptr);
    }

    bool IsOver() {
        return (GImGizmo->currentSpace.hoverID == nullptr ? false : true);
    }

    bool DrawPoint(const char* _id, ImVec3 point) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3Transform(point, viewMatrix);
        ImVec3Transform(point, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec2 screenPoint = {};
        screenPoint.x = ((point.x + 1.f) / 2.f ) * max.x;
        screenPoint.y = (1 - ((point.y + 1.f) / 2.f )) * max.y;

        auto color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f});
        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == _id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = _id;
        object.type = ImGizmoDrawType_Point;
        object.Point.pos = screenPoint;
        object.Point.radius = 2.f;
        object.col = color;
        object.depth = point.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == _id);
    }

    bool DrawLine(const char* _id, ImVec3 point1, ImVec3 point2) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3Transform(point1, viewMatrix);
        ImVec3Transform(point1, projMatrix);
        ImVec3Transform(point2, viewMatrix);
        ImVec3Transform(point2, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec2 screenPoint1 = {};
        ImVec2 screenPoint2 = {};
        screenPoint1.x = ((point1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((point1.y + 1.f) / 2.f )) * max.y;
        screenPoint2.x = ((point2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((point2.y + 1.f) / 2.f )) * max.y;

        auto color = ImGui::GetColorU32({1.f, 1.f, 1.f, 1.f});
        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == _id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = _id;
        object.type = ImGizmoDrawType_Line;
        object.Line.pos1 = screenPoint1;
        object.Line.pos2 = screenPoint2;
        object.col = color;
        object.depth = point1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == _id);
    }

    bool TranslateGizmo(const char *_id, ImMat44& _matrix) {
        bool active = false;

        auto drawArrow = [_matrix](const char* id, const ImVec3& pos, const ImVec3& dir, const ImVec3& left, ImVec3 up) -> bool {
            ImVec3 p1 = pos;
            ImVec3 p2 = pos + dir * 1.f; // center tip
            ImVec3 p3 = p2 + dir * 0.2f; // tip
            ImVec3 p4 = p2 + ImVec3Normalize(ImVec3Cross(dir, left)) * 0.1f;
            ImVec3 p5 = p2 + ImVec3Normalize(ImVec3Cross(dir, up)) * 0.1f;

            bool active = false;
            active = active ^ DrawLine(id, p1, p3);
            active = active ^ DrawLine(id, p3, p4);
            active = active ^ DrawLine(id, p4, p2);
            active = active ^ DrawLine(id, p3, p5);
            active = active ^ DrawLine(id, p5, p2);

            return active;
        };

        ImVec3 left = ImVec3(_matrix.m16[0], _matrix.m16[1], _matrix.m16[2]);
        ImVec3 up = ImVec3(_matrix.m16[4], _matrix.m16[5], _matrix.m16[6]);
        ImVec3 at = ImVec3(_matrix.m16[8], _matrix.m16[9], _matrix.m16[10]);
        ImVec3 pos = ImVec3(_matrix.m16[12], _matrix.m16[13], _matrix.m16[14]);

        bool xAxis = drawArrow("x_axis", pos, left, up, -at);
        bool yAxis = drawArrow("y_axis", pos, up, at, -left);
        bool zAxis = drawArrow("z_axis", pos, at, left, -up);

        active = active ^ xAxis;
        active = active ^ yAxis;
        active = active ^ zAxis;

        return active;
    }
};

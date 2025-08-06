#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cmath>
#include <cstring>
#include <vector>
#endif

#include "ImGizmo.h"
#include "imgui.h"
#include "imgui_internal.h"

#ifndef GImGizmo
ImGizmoContext* GImGizmo = nullptr;
#endif

enum ImGizmoDrawType {
    ImGizmoDrawType_Point,
    ImGizmoDrawType_Point_Outline,
    ImGizmoDrawType_Line,
    ImGizmoDrawType_Line_Outline,
    ImGizmoDrawType_Triangle,
    ImGizmoDrawType_Triangle_Outline,
    ImGizmoDrawType_Quad,
    ImGizmoDrawType_Quad_Outline,
    ImGizmoDrawType_COUNT,
};

typedef int ImGizmoNextItemFlags;
enum ImGizmoNextItemFlags_ {
    ImGizmoNextItemFlags_None = 0,
    ImGizmoNextItemFlags_HasRotation = 1 << 0,
};

struct ImGizmoDraw {
    union {
        struct {
            ImVec3 pos;
            float radius;
        } Point;
        struct {
            ImVec3 pos1;
            ImVec3 pos2;
        } Line;
        struct {
            ImVec3 pos1;
            ImVec3 pos2;
            ImVec3 pos3;
        } Triangle;
        struct {
            ImVec3 pos1;
            ImVec3 pos2;
            ImVec3 pos3;
            ImVec3 pos4;
        } Quad;
    };

    const char* id;
    ImU32 color;
    ImU32 color_out;
    ImU32 flags;
    ImGizmoDrawType type;
    float thickness;
    float depth;
};

struct ImGizmoSpace {
    ImGuiID ID;
    ImMat44 viewMatrix;
    ImMat44 projMatrix;
    ImMat44 transMatrix;
    ImDrawList* drawList;
    ImRect frameRect;
    std::vector<ImGizmoDraw> renderList;
    const char* hoverID = nullptr;
    const char* activeID = nullptr;
    ImVec3 cameraPos = {};
    ImVec3 hoverPos = {};
    ImVec2 initialMousePos = {};
    ImVec2 lastMousePos = {};
    bool firstMouseDownFrame = true;
    bool initialized = false;
};

struct ImGizmoNextItem {
    ImGizmoNextItemFlags flags;
    ImMat44 rotation;

    inline void Clear() { flags = ImGizmoNextItemFlags_None; }
};

struct ImGizmoContext {
    ImGizmoSpace currentSpace;
    ImGizmoNextItem nextItem;
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

// Helpers: ImMat44 Math Functions
static inline ImVec3 ImMat44Left(const ImMat44& _mat) {
    return ImVec3(_mat.m16[0], _mat.m16[1], _mat.m16[2]);
}

static inline ImVec3 ImMat44Up(const ImMat44& _mat) {
    return ImVec3(_mat.m16[4], _mat.m16[5], _mat.m16[6]);
}

static inline ImVec3 ImMat44At(const ImMat44& _mat) {
    return ImVec3(_mat.m16[8], _mat.m16[9], _mat.m16[10]);
}

static inline ImVec3 ImMat44Pos(const ImMat44& _mat) {
    return ImVec3(_mat.m16[12], _mat.m16[13], _mat.m16[14]);
}

// Helpers: ImVec3 Math Functions
static inline ImVec3 ImVec3Transform(const ImVec3& _vec, const ImMat44& _matrix) {
    ImVec4 in;
    ImVec3 out;

    in.x = _vec.x * _matrix.m4x4[0][0] + _vec.y * _matrix.m4x4[1][0] + _vec.z * _matrix.m4x4[2][0] + _matrix.m4x4[3][0];
    in.y = _vec.x * _matrix.m4x4[0][1] + _vec.y * _matrix.m4x4[1][1] + _vec.z * _matrix.m4x4[2][1] + _matrix.m4x4[3][1];
    in.z = _vec.x * _matrix.m4x4[0][2] + _vec.y * _matrix.m4x4[1][2] + _vec.z * _matrix.m4x4[2][2] + _matrix.m4x4[3][2];
    in.w = _vec.x * _matrix.m4x4[0][3] + _vec.y * _matrix.m4x4[1][3] + _vec.z * _matrix.m4x4[2][3] + _matrix.m4x4[3][3];

    in.x /= in.w;
    in.y /= in.w;
    in.z /= in.w;

    out.x = in.x;
    out.y = in.y;
    out.z = in.z;

    return out;
}

static inline ImVec3 ImVec3Cross(const ImVec3& _vec1, const ImVec3& _vec2) {
    ImVec3 out;
    out.x = _vec1.y * _vec2.z - _vec1.z * _vec2.y;
    out.y = _vec1.z * _vec2.x - _vec1.x * _vec2.z;
    out.z = _vec1.x * _vec2.y - _vec1.y * _vec2.x;
    return out;
}

static inline float ImVec3Magnitude(const ImVec3& _vec) {
    return sqrt(pow(_vec.x, 2.f) + pow(_vec.y, 2.f) + pow(_vec.z, 2.f));
}

static inline ImVec3 ImVec3Normalize(const ImVec3& _vec) {
    ImVec3 out = _vec;
    const float mag = ImVec3Magnitude(out);
    if (mag != 0.f) {
        out.x /= mag;
        out.y /= mag;
        out.z /= mag;
    }
    return out;
}

// Helpers: ImVec2 Math Functions
static inline float ImVec2Dot(const ImVec2& _vec1, const ImVec2& _vec2) {
    return _vec1.x * _vec2.x + _vec1.y * _vec2.y;
}

static inline ImVec2 ImVec2Perpendicular(const ImVec2& _vec) {
    return ImVec2(_vec.y, -_vec.x);
}

static inline float ImVec2SignedTriangleArea(const ImVec2& _edgeA, const ImVec2& _edgeB, const ImVec2& _edgeC)
{
    ImVec2 ac = _edgeC - _edgeA;
    ImVec2 abPerp = ImVec2Perpendicular(_edgeB - _edgeA);
    return ImVec2Dot(ac, abPerp) / 2.f;
}

static bool PointInTriangle(const ImVec2& _edgeA, const ImVec2& _edgeB, const ImVec2& _edgeC, const ImVec2& _point) {
    float areaABP = ImVec2SignedTriangleArea(_edgeA, _edgeB, _point);
    float areaBCP = ImVec2SignedTriangleArea(_edgeB, _edgeC, _point);
    float areaCAP = ImVec2SignedTriangleArea(_edgeC, _edgeA, _point);
    return areaABP > 0 && areaBCP > 0 && areaCAP > 0;
}

const float PI = 3.1415926536;

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

    bool Begin(const char* id, const ImMat44& viewMatrix, const ImMat44& projectionMatrix, const ImVec3& pos) {
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

        if(ImGui::Begin(id, NULL, flags)) {
            ImGuiWindow* window = g.CurrentWindow;

            current.initialized = true;
            current.viewMatrix = viewMatrix;
            current.projMatrix = projectionMatrix;
            current.drawList = window->DrawList;
            current.cameraPos = pos;
            current.frameRect = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImGui::GetMainViewport()->Size);

            ImGui::End();
        }

        return current.initialized;
    }

    void End() {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "You are trying to end the current ImGizmo space, but its empty. You can create a ImGizmo space with ImGizmo::Begin");
        IM_ASSERT_USER_ERROR(GImGizmo->nextItem.flags == 0, "Next item struct still contains data, remember to 'Pop' your settings before 'End' function call");

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGizmoContext& gz = *GImGizmo;
        ImDrawList& drawList = *gz.currentSpace.drawList;

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 mousePos = io.MousePos;
        bool mouseDown = io.MouseDown[ImGuiMouseButton_Left];

        const char* activeID = gz.currentSpace.activeID;
        const char* hoverID = (activeID != nullptr ? gz.currentSpace.hoverID : nullptr);

        for(const auto& object : gz.currentSpace.renderList) {
            IM_ASSERT_USER_ERROR(object.type != ImGizmoDrawType_COUNT, "ImGizmo is trying to render an unknown object.");

            switch (object.type) {
                case ImGizmoDrawType_Point:
                    drawList._PathArcToFastEx(object.Point.pos.xy(), object.Point.radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
                    drawList.PathFillConvex(object.color);
                    break;
                case ImGizmoDrawType_Point_Outline:
                    drawList._PathArcToFastEx(object.Point.pos.xy(), object.Point.radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.color_out, ImDrawFlags_Closed, object.thickness);
                    break;
                case ImGizmoDrawType_Line:
                    drawList.AddLine(object.Line.pos1.xy(), object.Line.pos2.xy(), object.color, object.thickness);
                    break;
                case ImGizmoDrawType_Line_Outline:
                    drawList.AddLine(object.Line.pos1.xy(), object.Line.pos2.xy(), object.color_out, object.thickness * 2.f);
                    drawList.AddLine(object.Line.pos1.xy(), object.Line.pos2.xy(), object.color, object.thickness);
                    break;
                case ImGizmoDrawType_Triangle:
                    drawList.PathLineTo(object.Triangle.pos1.xy());
                    drawList.PathLineTo(object.Triangle.pos2.xy());
                    drawList.PathLineTo(object.Triangle.pos3.xy());
                    drawList.PathFillConvex(object.color);
                    break;
                case ImGizmoDrawType_Triangle_Outline:
                    drawList.PathLineTo(object.Triangle.pos1.xy());
                    drawList.PathLineTo(object.Triangle.pos2.xy());
                    drawList.PathLineTo(object.Triangle.pos3.xy());
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.color_out, ImDrawFlags_Closed, object.thickness);
                    break;
                case ImGizmoDrawType_Quad:
                    drawList.PathLineTo(object.Quad.pos1.xy());
                    drawList.PathLineTo(object.Quad.pos2.xy());
                    drawList.PathLineTo(object.Quad.pos3.xy());
                    drawList.PathLineTo(object.Quad.pos4.xy());
                    drawList.PathFillConvex(object.color);
                    break;
                case ImGizmoDrawType_Quad_Outline:
                    drawList.PathLineTo(object.Quad.pos1.xy());
                    drawList.PathLineTo(object.Quad.pos2.xy());
                    drawList.PathLineTo(object.Quad.pos3.xy());
                    drawList.PathLineTo(object.Quad.pos4.xy());
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.color_out, ImDrawFlags_Closed, object.thickness);
                    break;
                default: break;
            }

            if (hoverID != nullptr && hoverID != object.id) {
                continue;
            }

            switch (object.type) {
                case ImGizmoDrawType_Point:
                case ImGizmoDrawType_Point_Outline: {
                    float mag = abs(pow(mousePos.x - object.Point.pos.x, 2.f) + pow(mousePos.y - object.Point.pos.y, 2.f));

                    if (mag < object.Point.radius) {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Line:
                case ImGizmoDrawType_Line_Outline: {
                    ImVec2 pos1ToMouse = mousePos - object.Line.pos1.xy();
                    ImVec2 pos1ToPos2 = object.Line.pos2.xy() - object.Line.pos1.xy();

                    float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
                    float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
                    float value = dot1 / dot2;
                    float clamp = value > 1.f ? 1.f : value < 0.f ? 0.f : value;
                    ImVec2 proj = pos1ToPos2 * clamp + object.Line.pos1.xy();
                    float mag = pow(mousePos.x - proj.x, 2.f) + pow(mousePos.y - proj.y, 2.f);

                    if (mag < 5.f) {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Triangle:
                case ImGizmoDrawType_Triangle_Outline: {
                    bool inTriangle = PointInTriangle(object.Triangle.pos1.xy(), object.Triangle.pos2.xy(), object.Triangle.pos3.xy(), mousePos);

                    if(inTriangle) {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Quad:
                case ImGizmoDrawType_Quad_Outline: {
                    bool inTriangle1 = PointInTriangle(object.Quad.pos1.xy(),object.Quad.pos2.xy(), object.Quad.pos3.xy(), mousePos);
                    bool inTriangle2 = PointInTriangle(object.Quad.pos3.xy(),object.Quad.pos4.xy(), object.Quad.pos1.xy(), mousePos);

                    if(inTriangle1 || inTriangle2) {
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

        if(mouseDown == true && gz.currentSpace.firstMouseDownFrame == true)
        {
            gz.currentSpace.initialMousePos = mousePos;
            gz.currentSpace.activeID = hoverID;
            gz.currentSpace.firstMouseDownFrame = false;
        }
        else if (mouseDown == false) {
            gz.currentSpace.hoverID = hoverID;
            gz.currentSpace.activeID = nullptr;
            gz.currentSpace.firstMouseDownFrame = true;
        }

        gz.currentSpace.lastMousePos = mousePos;
    }

    bool IsOver() {
        return (GImGizmo->currentSpace.hoverID == nullptr ? false : true);
    }

    bool IsUsing() {
        return (GImGizmo->currentSpace.activeID == nullptr ? false : true);
    }

    const char* GetHoveredID() {
        if(GImGizmo->currentSpace.hoverID == nullptr)
            return nullptr;

        return GImGizmo->currentSpace.hoverID;
    }

    const char* GetActiveID() {
        if(GImGizmo->currentSpace.activeID == nullptr)
            return nullptr;

        return GImGizmo->currentSpace.activeID;
    }

    ImVec3 GetHoveredPos() {
        return GImGizmo->currentSpace.hoverPos;
    }

    ImVec2 GetUsingStartPos() {
        return GImGizmo->currentSpace.initialMousePos;
    }

    ImVec2 GetLastMousePos() {
        return GImGizmo->currentSpace.lastMousePos;
    }

    ImVec2 ConvertTo2DCoords(const ImVec3& pos) {
        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 point;
        point = ImVec3Transform(pos, viewMatrix);
        point = ImVec3Transform(point, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec2 out = {};
        out.x = ((point.x + 1.f) / 2.f ) * max.x;
        out.y = (1 - ((point.y + 1.f) / 2.f )) * max.y;

        return out;
    }

    bool DrawPoint(const char* id, const ImVec3& point, float radius, ImU32 flags, ImU32 color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint;
        transPoint = ImVec3Transform(point, viewMatrix);
        transPoint = ImVec3Transform(transPoint, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint = {};
        screenPoint.x = ((transPoint.x + 1.f) / 2.f ) * max.x;
        screenPoint.y = (1 - ((transPoint.y + 1.f) / 2.f )) * max.y;
        screenPoint.z = transPoint.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Point;
        object.Point.pos = screenPoint;
        object.Point.radius = radius;
        object.color = color;
        object.depth = 20.f;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawPointWithOutline(const char* id, const ImVec3& point, float radius, float thickness, ImU32 flags, ImU32 color, ImU32 out_color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint;
        transPoint = ImVec3Transform(point, viewMatrix);
        transPoint = ImVec3Transform(transPoint, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint = {};
        screenPoint.x = ((transPoint.x + 1.f) / 2.f ) * max.x;
        screenPoint.y = (1 - ((transPoint.y + 1.f) / 2.f )) * max.y;
        screenPoint.z = transPoint.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Point_Outline;
        object.Point.pos = screenPoint;
        object.Point.radius = radius;
        object.color = color;
        object.color_out = out_color;
        object.thickness = thickness;
        object.depth = 20.f;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawLine(const char* id, const ImVec3& point1, const ImVec3& point2, ImU32 flags, ImU32 color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Line;
        object.Line.pos1 = screenPoint1;
        object.Line.pos2 = screenPoint2;
        object.color = color;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawLineWithOutline(const char* id, const ImVec3& point1, const ImVec3& point2, float thickness, ImU32 flags, ImU32 color, ImU32 out_color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Line_Outline;
        object.Line.pos1 = screenPoint1;
        object.Line.pos2 = screenPoint2;
        object.color = color;
        object.color_out = out_color;
        object.thickness = thickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawTriangle(const char* id, const ImVec3& point1, const ImVec3& point2, const ImVec3& point3, ImU32 flags, ImU32 color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        ImVec3 transPoint3;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);
        transPoint3 = ImVec3Transform(point3, viewMatrix);
        transPoint3 = ImVec3Transform(transPoint3, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        ImVec3 screenPoint3 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;
        screenPoint3.x = ((transPoint3.x + 1.f) / 2.f ) * max.x;
        screenPoint3.y = (1 - ((transPoint3.y + 1.f) / 2.f )) * max.y;
        screenPoint3.z = transPoint3.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Triangle;
        object.Triangle.pos1 = screenPoint1;
        object.Triangle.pos2 = screenPoint2;
        object.Triangle.pos3 = screenPoint3;
        object.color = color;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawTriangleWithOutline(const char* id, const ImVec3& point1, const ImVec3& point2, const ImVec3& point3, float thickness, ImU32 flags, ImU32 color, ImU32 out_color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        ImVec3 transPoint3;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);
        transPoint3 = ImVec3Transform(point3, viewMatrix);
        transPoint3 = ImVec3Transform(transPoint3, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        ImVec3 screenPoint3 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;
        screenPoint3.x = ((transPoint3.x + 1.f) / 2.f ) * max.x;
        screenPoint3.y = (1 - ((transPoint3.y + 1.f) / 2.f )) * max.y;
        screenPoint3.z = transPoint3.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Triangle_Outline;
        object.Triangle.pos1 = screenPoint1;
        object.Triangle.pos2 = screenPoint2;
        object.Triangle.pos3 = screenPoint3;
        object.color = color;
        object.color_out = out_color;
        object.thickness = thickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawQuad(const char* id, const ImVec3& point1, const ImVec3& point2, const ImVec3& point3, const ImVec3& point4, ImU32 flags, ImU32 color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        ImVec3 transPoint3;
        ImVec3 transPoint4;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);
        transPoint3 = ImVec3Transform(point3, viewMatrix);
        transPoint3 = ImVec3Transform(transPoint3, projMatrix);
        transPoint4 = ImVec3Transform(point4, viewMatrix);
        transPoint4 = ImVec3Transform(transPoint4, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        ImVec3 screenPoint3 = {};
        ImVec3 screenPoint4 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;
        screenPoint3.x = ((transPoint3.x + 1.f) / 2.f ) * max.x;
        screenPoint3.y = (1 - ((transPoint3.y + 1.f) / 2.f )) * max.y;
        screenPoint3.z = transPoint3.z;
        screenPoint4.x = ((transPoint4.x + 1.f) / 2.f ) * max.x;
        screenPoint4.y = (1 - ((transPoint4.y + 1.f) / 2.f )) * max.y;
        screenPoint4.z = transPoint4.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Quad;
        object.Quad.pos1 = screenPoint1;
        object.Quad.pos2 = screenPoint2;
        object.Quad.pos3 = screenPoint3;
        object.Quad.pos4 = screenPoint4;
        object.color = color;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawQuadWithOutline(const char* id, const ImVec3& point1, const ImVec3& point2, const ImVec3& point3, const ImVec3& point4, float thickness, ImU32 flags, ImU32 color, ImU32 out_color) {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint1;
        ImVec3 transPoint2;
        ImVec3 transPoint3;
        ImVec3 transPoint4;
        transPoint1 = ImVec3Transform(point1, viewMatrix);
        transPoint1 = ImVec3Transform(transPoint1, projMatrix);
        transPoint2 = ImVec3Transform(point2, viewMatrix);
        transPoint2 = ImVec3Transform(transPoint2, projMatrix);
        transPoint3 = ImVec3Transform(point3, viewMatrix);
        transPoint3 = ImVec3Transform(transPoint3, projMatrix);
        transPoint4 = ImVec3Transform(point4, viewMatrix);
        transPoint4 = ImVec3Transform(transPoint4, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint1 = {};
        ImVec3 screenPoint2 = {};
        ImVec3 screenPoint3 = {};
        ImVec3 screenPoint4 = {};
        screenPoint1.x = ((transPoint1.x + 1.f) / 2.f ) * max.x;
        screenPoint1.y = (1 - ((transPoint1.y + 1.f) / 2.f )) * max.y;
        screenPoint1.z = transPoint1.z;
        screenPoint2.x = ((transPoint2.x + 1.f) / 2.f ) * max.x;
        screenPoint2.y = (1 - ((transPoint2.y + 1.f) / 2.f )) * max.y;
        screenPoint2.z = transPoint2.z;
        screenPoint3.x = ((transPoint3.x + 1.f) / 2.f ) * max.x;
        screenPoint3.y = (1 - ((transPoint3.y + 1.f) / 2.f )) * max.y;
        screenPoint3.z = transPoint3.z;
        screenPoint4.x = ((transPoint4.x + 1.f) / 2.f ) * max.x;
        screenPoint4.y = (1 - ((transPoint4.y + 1.f) / 2.f )) * max.y;
        screenPoint4.z = transPoint4.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id) {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
            out_color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Quad_Outline;
        object.Quad.pos1 = screenPoint1;
        object.Quad.pos2 = screenPoint2;
        object.Quad.pos3 = screenPoint3;
        object.Quad.pos4 = screenPoint4;
        object.color = color;
        object.color_out = out_color;
        object.thickness = thickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    void PushNextItemRotation(ImVec3* left, ImVec3* up, ImVec3* at) {
        ImMat44 rotation = {};

        rotation.m4x4[0][0] = left->x;
        rotation.m4x4[0][1] = left->y;
        rotation.m4x4[0][2] = left->z;
        rotation.m4x4[1][0] = up->x;
        rotation.m4x4[1][0] = up->x;
        rotation.m4x4[1][1] = up->y;
        rotation.m4x4[2][2] = at->z;
        rotation.m4x4[2][1] = at->y;
        rotation.m4x4[2][2] = at->z;

        GImGizmo->nextItem.rotation = rotation;
        GImGizmo->nextItem.flags |= ImGizmoNextItemFlags_HasRotation;
    }

    void PushNextItemRotation(ImVec3* euler) {
        ImMat44 rotation = {};

        const float HalfPI = PI / 180.f;
        float cosX = std::cos(euler->x * HalfPI);
        float sinX = std::sin(euler->x * HalfPI);
        float cosY = std::cos(euler->y * HalfPI);
        float sinY = std::sin(euler->y * HalfPI);
        float cosZ = std::cos(euler->z * HalfPI);
        float sinZ = std::sin(euler->z * HalfPI);

        rotation.m4x4[0][0] = cosY * cosZ;
        rotation.m4x4[0][1] = cosY * sinZ;
        rotation.m4x4[0][2] = -sinY;

        rotation.m4x4[1][0] = -cosX * sinZ + sinX * sinY * cosZ;
        rotation.m4x4[1][1] = cosX * cosZ + sinX * sinY * sinZ;
        rotation.m4x4[1][2] = sinX * cosY;

        rotation.m4x4[2][0] = sinX * sinZ + cosX * sinY * cosZ;
        rotation.m4x4[2][1] = -sinX * cosZ + cosX * sinY * sinZ;
        rotation.m4x4[2][2] = cosX * cosY;

        GImGizmo->nextItem.rotation = rotation;
        GImGizmo->nextItem.flags |= ImGizmoNextItemFlags_HasRotation;
    }

    void PushNextItemRotation(ImVec4* quat) {
        ImMat44 rotation = {};

        IM_ASSERT(false && "TODO");

        GImGizmo->nextItem.rotation = rotation;
        GImGizmo->nextItem.flags |= ImGizmoNextItemFlags_HasRotation;
    }

    void PopNextItemRotation() {
        IM_ASSERT_USER_ERROR(GImGizmo->nextItem.flags & ImGizmoNextItemFlags_HasRotation, "Next item rotation is empty. Either you are trying to pop without calling 'PushNextItemRotation' or it has already been poped.");

        GImGizmo->nextItem.flags ^= ImGizmoNextItemFlags_HasRotation;
    }

    bool Translate(const char *_id, ImVec3* position) {
        bool active = false;

        auto drawAxis = [](const char* id, const ImVec3& pos, const ImVec3& at, const ImVec3& axis1, const ImVec3& axis2, bool inverted, ImU32 color) -> bool {
            ImVec3 p1 = pos;
            ImVec3 p2 = pos + (at * 1.f);
            ImVec3 p3 = p2 + (at * 0.2f);
            ImVec3 p4 = p2 + (axis1 * 0.1f);
            ImVec3 p5 = p2 + (axis2 * 0.1f);

            const auto transp = ImGui::GetColorU32({0.f,0.f,0.f,0.f});

            bool active = false;
            if(inverted) {
                active = active ^ DrawLineWithOutline(id, p1, p2, 1.f, 0, transp, color);
                active = active ^ DrawTriangleWithOutline(id, p3, p4, p2, 1.f, 0, transp, color);
                active = active ^ DrawTriangleWithOutline(id, p3, p2, p5, 1.f, 0, transp, color);
            } else {
                active = active ^ DrawLine(id, p1, p2, 0, color);
                active = active ^ DrawTriangle(id, p3, p4, p2, 0, color);
                active = active ^ DrawTriangle(id, p3, p2, p5, 0, color);
            }

            return active;
        };

        auto drawPlane = [](const char* id, const ImVec3& pos, const ImVec3& axis1, const ImVec3& axis2, bool inverted, ImU32 color) -> bool {
            ImVec3 p1 = pos + (axis1 * 0.25f) + (axis2 * 0.25f); 
            ImVec3 p2 = p1 + (axis1 * 0.25f);
            ImVec3 p3 = p1 + (axis1 * 0.25f) + (axis2 * 0.25f);
            ImVec3 p4 = p1 + (axis2 * 0.25f);

            const auto transp = ImGui::GetColorU32({0.f,0.f,0.f,0.f});

            if (inverted) {
                return DrawQuadWithOutline(id, p1, p4, p3, p2, 0.2f, 0, transp, color);
            }
            else {
                return DrawQuad(id, p1, p2, p3, p4, 0, color);
            }
        };

        ImVec3 left = ImVec3(1.f, 0.f, 0.f);
        ImVec3 up = ImVec3(0.f, 1.f, 0.f);
        ImVec3 at = ImVec3(0.f, 0.f, 1.f);
        if(GImGizmo->nextItem.flags & ImGizmoNextItemFlags_HasRotation) {
            auto& rotation = GImGizmo->nextItem.rotation;
            left = ImMat44Left(rotation);
            up = ImMat44Up(rotation);
            at = ImMat44At(rotation);
        }

        bool xAxisInv = false;
        bool yAxisInv = false;
        bool zAxisInv = false;

        const auto& camPos = GImGizmo->currentSpace.cameraPos;

        const float xPlaneDistance = left.x * camPos.x + left.y * camPos.y + left.z * camPos.z;
        const float yPlaneDistance = up.x * camPos.x + up.y * camPos.y + up.z * camPos.z;
        const float zPlaneDistance = at.x * camPos.x + at.y * camPos.y + at.z * camPos.z;

        if(xPlaneDistance < 0.f) {
            left = -left;
            xAxisInv = true;
        }
        if(yPlaneDistance < 0.f) {
            up = -up;
            yAxisInv = true;
        }
        if(zPlaneDistance < 0.f) {
            at = -at;
            zAxisInv = true;
        }

        bool xAxis = drawAxis("x_axis", *position, left, up, at, xAxisInv, ImGui::GetColorU32({1.f,0.f,0.f,1.f}));
        bool yAxis = drawAxis("y_axis", *position, up, at, left, yAxisInv, ImGui::GetColorU32({0.f,1.f,0.f,1.f}));
        bool zAxis = drawAxis("z_axis", *position, at, left, up, zAxisInv, ImGui::GetColorU32({0.f,0.f,1.f,1.f}));

        bool xyAxis = drawPlane("xy_axis", *position, left, up, xAxisInv, ImGui::GetColorU32({0.f,0.f,1.f,1.f}));
        bool yzAxis = drawPlane("yz_axis", *position, up, at, yAxisInv, ImGui::GetColorU32({1.f,0.f,0.f,1.f}));
        bool zxAxis = drawPlane("zx_axis", *position, at, left, zAxisInv, ImGui::GetColorU32({0.f,1.f,0.f,1.f}));

        xAxis = xAxis ^ (xyAxis || zxAxis);
        yAxis = yAxis ^ (yzAxis || xyAxis);
        zAxis = zAxis ^ (zxAxis || yzAxis);

        DrawPoint("center_axis", *position, 5.f);

        ImVec3 pos = *position;
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 deltaPos = GetLastMousePos();
        ImVec2 pos1 = ConvertTo2DCoords(*position);
        ImVec2 pos1ToMouse = mousePos - pos1;
        ImVec2 pos1ToDelta = deltaPos - pos1;
        if (xAxis) {
            ImVec2 pos2 = ConvertTo2DCoords(pos + ImVec3Normalize(left));
            ImVec2 pos1ToPos2 = pos2 - pos1;

            float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
            float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
            float dot3 = ImVec2Dot(pos1ToDelta, pos1ToPos2);
            float value1 = dot1 / dot2;
            float value2 = dot3 / dot2;
            float diff = value1 - value2;

            *position += ImVec3Normalize(left) * diff;
        }
        if (yAxis) {
            ImVec2 pos2 = ConvertTo2DCoords(pos + ImVec3Normalize(up));
            ImVec2 pos1ToPos2 = pos2 - pos1;

            float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
            float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
            float dot3 = ImVec2Dot(pos1ToDelta, pos1ToPos2);
            float value1 = dot1 / dot2;
            float value2 = dot3 / dot2;
            float diff = value1 - value2;

            *position += ImVec3Normalize(up) * diff;
        }
        if (zAxis) {
            ImVec2 pos2 = ConvertTo2DCoords(pos + ImVec3Normalize(at));
            ImVec2 pos1ToPos2 = pos2 - pos1;

            float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
            float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
            float dot3 = ImVec2Dot(pos1ToDelta, pos1ToPos2);
            float value1 = dot1 / dot2;
            float value2 = dot3 / dot2;
            float diff = value1 - value2;

            *position += ImVec3Normalize(at) * diff;
        }

        active = active ^ xAxis;
        active = active ^ yAxis;
        active = active ^ zAxis;

        return active;
    }
};

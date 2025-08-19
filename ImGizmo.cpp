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

struct ImMat44
{
    union
    {
        float m4x4[4][4];
        float m16[16];
    };

    ImMat44()
    {
        m4x4[0][0] = m4x4[0][1] = m4x4[0][2] = m4x4[0][3] =
        m4x4[1][0] = m4x4[1][1] = m4x4[1][2] = m4x4[1][3] =
        m4x4[2][0] = m4x4[2][1] = m4x4[2][2] = m4x4[2][3] =
        m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = m4x4[3][3] = 0.f;
    };

    ImMat44(float* matrix)
    {
        m16[0] = matrix[0];
        m16[1] = matrix[1];
        m16[2] = matrix[2];
        m16[3] = matrix[3];
        m16[4] = matrix[4];
        m16[5] = matrix[5];
        m16[6] = matrix[6];
        m16[7] = matrix[7];
        m16[8] = matrix[8];
        m16[9] = matrix[9];
        m16[10] = matrix[10];
        m16[11] = matrix[11];
        m16[12] = matrix[12];
        m16[13] = matrix[13];
        m16[14] = matrix[14];
        m16[15] = matrix[15];
    }

#ifdef IM_MAT44_CLASS_EXTRA
    IM_MAT44_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imgizmoconfig.h to convert back and forth between your matrices and ImMat44.
#endif
};

enum ImGizmoDrawType
{
    ImGizmoDrawType_Point,
    ImGizmoDrawType_Line,
    ImGizmoDrawType_Triangle,
    ImGizmoDrawType_Quad,
    ImGizmoDrawType_COUNT,
};

struct ImGizmoDraw
{
    union
    {
        struct
        {
            ImVec3 pos;
            float radius;
        } Point;
        struct
        {
            ImVec3 pos1;
            ImVec3 pos2;
        } Line;
        struct
        {
            ImVec3 pos1;
            ImVec3 pos2;
            ImVec3 pos3;
        } Triangle;
        struct
        {
            ImVec3 pos1;
            ImVec3 pos2;
            ImVec3 pos3;
            ImVec3 pos4;
        } Quad;
    };

    const char* id;
    ImU32 color;
    ImU32 borderColor;
    ImU32 flags;
    ImGizmoDrawType type;
    float borderThickness;
    float depth;
};

struct ImGizmoSpace
{
    ImGuiID ID;
    ImMat44* viewMatrix;
    ImMat44* projMatrix;
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

struct ImGizmoContext
{
    ImGizmoSpace currentSpace;
};

void InitializeContext(ImGizmoContext* context) {};

static inline ImVec3  operator*(const ImVec3& lhs, const float rhs)     { return ImVec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
static inline ImVec3  operator/(const ImVec3& lhs, const float rhs)     { return ImVec3(lhs.x / rhs, lhs.y / rhs, lhs.y / rhs); }
static inline ImVec3  operator+(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
static inline ImVec3  operator-(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
static inline ImVec3  operator*(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
static inline ImVec3  operator/(const ImVec3& lhs, const ImVec3& rhs)   { return ImVec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
static inline ImVec3  operator-(const ImVec3& lhs)                      { return ImVec3(-lhs.x, -lhs.y, -lhs.z); }
static inline ImVec3& operator*=(ImVec3& lhs, const float rhs)          { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
static inline ImVec3& operator/=(ImVec3& lhs, const float rhs)          { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
static inline ImVec3& operator+=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z;return lhs; }
static inline ImVec3& operator-=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z;return lhs; }
static inline ImVec3& operator*=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z;return lhs; }
static inline ImVec3& operator/=(ImVec3& lhs, const ImVec3& rhs)        { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z;return lhs; }
static inline bool    operator==(const ImVec3& lhs, const ImVec3& rhs)  { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
static inline bool    operator!=(const ImVec3& lhs, const ImVec3& rhs)  { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; }

static inline ImMat44 FPU_MatrixF_x_MatrixF(const ImMat44& lhs, const ImMat44& rhs)
{
    ImMat44 out;

    out.m16[0] = lhs.m16[0] * rhs.m16[0] + lhs.m16[1] * rhs.m16[4] + lhs.m16[2] * rhs.m16[8] + lhs.m16[3] * rhs.m16[12];
    out.m16[1] = lhs.m16[0] * rhs.m16[1] + lhs.m16[1] * rhs.m16[5] + lhs.m16[2] * rhs.m16[9] + lhs.m16[3] * rhs.m16[13];
    out.m16[2] = lhs.m16[0] * rhs.m16[2] + lhs.m16[1] * rhs.m16[6] + lhs.m16[2] * rhs.m16[10] + lhs.m16[3] * rhs.m16[14];
    out.m16[3] = lhs.m16[0] * rhs.m16[3] + lhs.m16[1] * rhs.m16[7] + lhs.m16[2] * rhs.m16[11] + lhs.m16[3] * rhs.m16[15];

    out.m16[4] = lhs.m16[4] * rhs.m16[0] + lhs.m16[5] * rhs.m16[4] + lhs.m16[6] * rhs.m16[8] + lhs.m16[7] * rhs.m16[12];
    out.m16[5] = lhs.m16[4] * rhs.m16[1] + lhs.m16[5] * rhs.m16[5] + lhs.m16[6] * rhs.m16[9] + lhs.m16[7] * rhs.m16[13];
    out.m16[6] = lhs.m16[4] * rhs.m16[2] + lhs.m16[5] * rhs.m16[6] + lhs.m16[6] * rhs.m16[10] + lhs.m16[7] * rhs.m16[14];
    out.m16[7] = lhs.m16[4] * rhs.m16[3] + lhs.m16[5] * rhs.m16[7] + lhs.m16[6] * rhs.m16[11] + lhs.m16[7] * rhs.m16[15];

    out.m16[8] = lhs.m16[8] * rhs.m16[0] + lhs.m16[9] * rhs.m16[4] + lhs.m16[10] * rhs.m16[8] + lhs.m16[11] * rhs.m16[12];
    out.m16[9] = lhs.m16[8] * rhs.m16[1] + lhs.m16[9] * rhs.m16[5] + lhs.m16[10] * rhs.m16[9] + lhs.m16[11] * rhs.m16[13];
    out.m16[10] = lhs.m16[8] * rhs.m16[2] + lhs.m16[9] * rhs.m16[6] + lhs.m16[10] * rhs.m16[10] + lhs.m16[11] * rhs.m16[14];
    out.m16[11] = lhs.m16[8] * rhs.m16[3] + lhs.m16[9] * rhs.m16[7] + lhs.m16[10] * rhs.m16[11] + lhs.m16[11] * rhs.m16[15];

    out.m16[12] = lhs.m16[12] * rhs.m16[0] + lhs.m16[13] * rhs.m16[4] + lhs.m16[14] * rhs.m16[8] + lhs.m16[15] * rhs.m16[12];
    out.m16[13] = lhs.m16[12] * rhs.m16[1] + lhs.m16[13] * rhs.m16[5] + lhs.m16[14] * rhs.m16[9] + lhs.m16[15] * rhs.m16[13];
    out.m16[14] = lhs.m16[12] * rhs.m16[2] + lhs.m16[13] * rhs.m16[6] + lhs.m16[14] * rhs.m16[10] + lhs.m16[15] * rhs.m16[14];
    out.m16[15] = lhs.m16[12] * rhs.m16[3] + lhs.m16[13] * rhs.m16[7] + lhs.m16[14] * rhs.m16[11] + lhs.m16[15] * rhs.m16[15];

    return out;
}

// Helpers: ImMat44 Math Functions
static inline ImVec3 ImMat44Left(const ImMat44& matrix)
{
    return ImVec3(matrix.m16[0], matrix.m16[1], matrix.m16[2]);
}

static inline ImVec3 ImMat44Up(const ImMat44& matrix)
{
    return ImVec3(matrix.m16[4], matrix.m16[5], matrix.m16[6]);
}

static inline ImVec3 ImMat44At(const ImMat44& matrix)
{
    return ImVec3(matrix.m16[8], matrix.m16[9], matrix.m16[10]);
}

static inline ImVec3 ImMat44Pos(const ImMat44& matrix)
{
    return ImVec3(matrix.m16[12], matrix.m16[13], matrix.m16[14]);
}

// Helpers: ImVec3 Math Functions

const float PI = 3.1415926536;

static inline ImMat44 EulerToRotationMatrix(const ImVec3& euler)
{
    ImMat44 rotation = {};

    const float HalfPI = PI / 180.f;
    float cosX = std::cos(euler.x * HalfPI);
    float sinX = std::sin(euler.x * HalfPI);
    float cosY = std::cos(euler.y * HalfPI);
    float sinY = std::sin(euler.y * HalfPI);
    float cosZ = std::cos(euler.z * HalfPI);
    float sinZ = std::sin(euler.z * HalfPI);

    rotation.m4x4[0][0] = cosY * cosZ;
    rotation.m4x4[0][1] = cosY * sinZ;
    rotation.m4x4[0][2] = -sinY;

    rotation.m4x4[1][0] = -cosX * sinZ + sinX * sinY * cosZ;
    rotation.m4x4[1][1] = cosX * cosZ + sinX * sinY * sinZ;
    rotation.m4x4[1][2] = sinX * cosY;

    rotation.m4x4[2][0] = sinX * sinZ + cosX * sinY * cosZ;
    rotation.m4x4[2][1] = -sinX * cosZ + cosX * sinY * sinZ;
    rotation.m4x4[2][2] = cosX * cosY;

    return rotation;
}

static inline ImVec3 ImVec3Transform(const ImVec3& vec, const ImMat44& matrix)
{
    ImVec4 in;
    ImVec3 out;

    in.x = vec.x * matrix.m4x4[0][0] + vec.y * matrix.m4x4[1][0] + vec.z * matrix.m4x4[2][0] + matrix.m4x4[3][0];
    in.y = vec.x * matrix.m4x4[0][1] + vec.y * matrix.m4x4[1][1] + vec.z * matrix.m4x4[2][1] + matrix.m4x4[3][1];
    in.z = vec.x * matrix.m4x4[0][2] + vec.y * matrix.m4x4[1][2] + vec.z * matrix.m4x4[2][2] + matrix.m4x4[3][2];
    in.w = vec.x * matrix.m4x4[0][3] + vec.y * matrix.m4x4[1][3] + vec.z * matrix.m4x4[2][3] + matrix.m4x4[3][3];

    in.x /= in.w;
    in.y /= in.w;
    in.z /= in.w;

    out.x = in.x;
    out.y = in.y;
    out.z = in.z;

    return out;
}

static inline ImVec3 ImVec3Cross(const ImVec3& vec1, const ImVec3& vec2)
{
    ImVec3 out;
    out.x = vec1.y * vec2.z - vec1.z * vec2.y;
    out.y = vec1.z * vec2.x - vec1.x * vec2.z;
    out.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return out;
}

static inline float ImVec3Magnitude(const ImVec3& vec)
{
    return sqrt(pow(vec.x, 2.f) + pow(vec.y, 2.f) + pow(vec.z, 2.f));
}

static inline ImVec3 ImVec3Normalize(const ImVec3& vec)
{
    ImVec3 out = vec;
    const float mag = ImVec3Magnitude(out);
    if (mag != 0.f)
    {
        out.x /= mag;
        out.y /= mag;
        out.z /= mag;
    }
    return out;
}

// Helpers: ImVec2 Math Functions
static inline float ImVec2Dot(const ImVec2& vec1, const ImVec2& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

static inline ImVec2 ImVec2Perpendicular(const ImVec2& vec)
{
    return ImVec2(vec.y, -vec.x);
}

static inline bool ImVec2SignedTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& p)
{
    ImVec2 ap = p - a;
    ImVec2 abPerp = ImVec2Perpendicular(b - a);
    return ImVec2Dot(ap, abPerp) >= 0.f;
}

static bool PointInTriangle(const ImVec2& edgeA, const ImVec2& edgeB, const ImVec2& edgeC, const ImVec2& point)
{
    bool areaAB = ImVec2SignedTriangleArea(edgeA, edgeB, point);
    bool areaBC = ImVec2SignedTriangleArea(edgeB, edgeC, point);
    bool areaCA = ImVec2SignedTriangleArea(edgeC, edgeA, point);

    return areaAB == areaBC && areaBC == areaCA;
}

namespace ImGizmo
{
    ImGizmoContext* CreateContext()
    {
        ImGizmoContext* context = IM_NEW(ImGizmoContext)();
        InitializeContext(context);
        context->currentSpace.initialized = false;
        if (GImGizmo == nullptr)
        {
            SetCurrentContext(context);
        }
        return context;
    }

    void DestroyContext(ImGizmoContext* context)
    {
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

    ImGizmoContext* GetCurrentContext()
    {
        return GImGizmo;
    }

    void SetCurrentContext(ImGizmoContext* context)
    {
        GImGizmo = context;
    }

    bool Begin(const char* id, float* viewMatrix, float* projectionMatrix, const ImVec3& cameraPosition)
    {
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

        if(ImGui::Begin(id, NULL, flags))
        {
            ImGuiWindow* window = g.CurrentWindow;

            current.initialized = true;
            current.viewMatrix = (ImMat44*)viewMatrix;
            current.projMatrix = (ImMat44*)projectionMatrix;
            current.drawList = window->DrawList;
            current.cameraPos = cameraPosition;
            current.frameRect = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImGui::GetMainViewport()->Size);

            ImGui::End();
        }

        return current.initialized;
    }

    void End()
    {
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

        for(const auto& object : gz.currentSpace.renderList)
        {
            IM_ASSERT_USER_ERROR(object.type < ImGizmoDrawType_COUNT, "ImGizmo is trying to render an unknown object.");

            switch (object.type)
            {
                case ImGizmoDrawType_Point:
                {
                    drawList._PathArcToFastEx(object.Point.pos.xy(), object.Point.radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.borderColor, ImDrawFlags_Closed, object.borderThickness);
                    break;
                }
                case ImGizmoDrawType_Line:
                {
                    drawList.AddLine(object.Line.pos1.xy(), object.Line.pos2.xy(), object.borderColor, object.borderThickness * 2.f);
                    drawList.AddLine(object.Line.pos1.xy(), object.Line.pos2.xy(), object.color, object.borderThickness);
                    break;
                }
                case ImGizmoDrawType_Triangle:
                {
                    drawList.PathLineTo(object.Triangle.pos1.xy());
                    drawList.PathLineTo(object.Triangle.pos2.xy());
                    drawList.PathLineTo(object.Triangle.pos3.xy());
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.borderColor, ImDrawFlags_Closed, object.borderThickness);
                    break;
                }
                case ImGizmoDrawType_Quad:
                {
                    drawList.PathLineTo(object.Quad.pos1.xy());
                    drawList.PathLineTo(object.Quad.pos2.xy());
                    drawList.PathLineTo(object.Quad.pos3.xy());
                    drawList.PathLineTo(object.Quad.pos4.xy());
                    drawList.AddConvexPolyFilled(drawList._Path.Data, drawList._Path.Size, object.color);
                    drawList.PathStroke(object.borderColor, ImDrawFlags_Closed, object.borderThickness);
                    break;
                }
                default: break;
            }

            if (hoverID != nullptr && hoverID != object.id)
            {
                continue;
            }

            switch (object.type)
            {
                case ImGizmoDrawType_Point:
                {
                    float mag = abs(pow(mousePos.x - object.Point.pos.x, 2.f) + pow(mousePos.y - object.Point.pos.y, 2.f));
                    if (mag < object.Point.radius)
                    {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Line:
                {
                    ImVec2 pos1ToMouse = mousePos - object.Line.pos1.xy();
                    ImVec2 pos1ToPos2 = object.Line.pos2.xy() - object.Line.pos1.xy();

                    float dot1 = ImVec2Dot(pos1ToMouse, pos1ToPos2);
                    float dot2 = ImVec2Dot(pos1ToPos2, pos1ToPos2);
                    float value = dot1 / dot2;
                    float clamp = value > 1.f ? 1.f : value < 0.f ? 0.f : value;
                    ImVec2 proj = pos1ToPos2 * clamp + object.Line.pos1.xy();
                    float mag = pow(mousePos.x - proj.x, 2.f) + pow(mousePos.y - proj.y, 2.f);

                    if (mag < 5.f)
                    {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Triangle:
                {
                    bool inTriangle = PointInTriangle(object.Triangle.pos1.xy(), object.Triangle.pos2.xy(), object.Triangle.pos3.xy(), mousePos);
                    if(inTriangle)
                    {
                        hoverID = object.id;
                    }
                    break;
                }
                case ImGizmoDrawType_Quad:
                {
                    bool inTriangle1 = PointInTriangle(object.Quad.pos1.xy(),object.Quad.pos2.xy(), object.Quad.pos3.xy(), mousePos);
                    bool inTriangle2 = PointInTriangle(object.Quad.pos1.xy(),object.Quad.pos3.xy(), object.Quad.pos4.xy(), mousePos);
                    if(inTriangle1 || inTriangle2)
                    {
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
        else if (mouseDown == false)
        {
            gz.currentSpace.hoverID = hoverID;
            gz.currentSpace.activeID = nullptr;
            gz.currentSpace.firstMouseDownFrame = true;
        }

        gz.currentSpace.lastMousePos = mousePos;
    }

    bool IsOver()
    {
        return (GImGizmo->currentSpace.hoverID == nullptr ? false : true);
    }

    bool IsUsing()
    {
        return (GImGizmo->currentSpace.activeID == nullptr ? false : true);
    }

    const char* GetHoveredID()
    {
        if(GImGizmo->currentSpace.hoverID == nullptr)
            return nullptr;

        return GImGizmo->currentSpace.hoverID;
    }

    const char* GetActiveID()
    {
        if(GImGizmo->currentSpace.activeID == nullptr)
            return nullptr;

        return GImGizmo->currentSpace.activeID;
    }

    ImVec3 GetHoveredPos()
    {
        return GImGizmo->currentSpace.hoverPos;
    }

    ImVec2 GetUsingStartPos()
    {
        return GImGizmo->currentSpace.initialMousePos;
    }

    ImVec2 GetLastMousePos()
    {
        return GImGizmo->currentSpace.lastMousePos;
    }

    ImVec2 ConvertTo2DCoords(const ImVec3& pos)
    {
        const ImMat44& viewMatrix = *GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = *GImGizmo->currentSpace.projMatrix;

        ImVec3 point;
        point = ImVec3Transform(pos, viewMatrix);
        point = ImVec3Transform(point, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec2 out = {};
        out.x = ((point.x + 1.f) / 2.f ) * max.x;
        out.y = (1 - ((point.y + 1.f) / 2.f )) * max.y;

        return out;
    }

    bool DrawPoint(
        const char* id,
        const ImVec3& point,
        float radius,
        ImU32 color,
        float borderThickness,
        ImU32 borderColor,
        ImU32 flags
    )
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        ImDrawList& drawList = *GImGizmo->currentSpace.drawList;
        const ImMat44& viewMatrix = *GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = *GImGizmo->currentSpace.projMatrix;

        ImVec3 transPoint;
        transPoint = ImVec3Transform(point, viewMatrix);
        transPoint = ImVec3Transform(transPoint, projMatrix);

        const auto& max = GImGizmo->currentSpace.frameRect.Max;
        ImVec3 screenPoint = {};
        screenPoint.x = ((transPoint.x + 1.f) / 2.f ) * max.x;
        screenPoint.y = (1 - ((transPoint.y + 1.f) / 2.f )) * max.y;
        screenPoint.z = transPoint.z;

        auto hoverID = GImGizmo->currentSpace.hoverID;
        if (hoverID && hoverID == id)
        {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Point;
        object.Point.pos = screenPoint;
        object.Point.radius = radius;
        object.color = color;
        object.borderThickness = borderThickness;
        object.borderColor = borderColor;
        object.depth = 20.f;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawLine(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        ImU32 color,
        float borderThickness,
        ImU32 borderColor,
        ImU32 flags
    )
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = *GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = *GImGizmo->currentSpace.projMatrix;

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
        if (hoverID && hoverID == id)
        {
            color = ImGui::GetColorU32({.5f, .5f, .5f, 1.f});
        }

        ImGizmoDraw object = {};
        object.id = id;
        object.type = ImGizmoDrawType_Line;
        object.Line.pos1 = screenPoint1;
        object.Line.pos2 = screenPoint2;
        object.color = color;
        object.borderColor = borderColor;
        object.borderThickness = borderThickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawTriangle(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        ImU32 color,
        float borderThickness,
        ImU32 borderColor,
        ImU32 flags
    )
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = *GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = *GImGizmo->currentSpace.projMatrix;

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
        object.borderColor = borderColor;
        object.borderThickness = borderThickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool DrawQuad(
        const char* id,
        const ImVec3& point1,
        const ImVec3& point2,
        const ImVec3& point3,
        const ImVec3& point4,
        ImU32 color,
        float borderThickness,
        ImU32 borderColor,
        ImU32 flags
    )
    {
        IM_ASSERT_USER_ERROR(GImGizmo != nullptr, "Current context is empty. Did you call ImGizmo::CreateContext()?");
        IM_ASSERT_USER_ERROR(GImGizmo->currentSpace.initialized == true, "Current ImGizmo space is empty. Did you call ImGizmo::Begin()?");

        const ImMat44& viewMatrix = *GImGizmo->currentSpace.viewMatrix;
        const ImMat44& projMatrix = *GImGizmo->currentSpace.projMatrix;

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
        if (hoverID && hoverID == id)
        {
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
        object.borderColor = borderColor;
        object.borderThickness = borderThickness;
        object.depth = transPoint1.z;
        GImGizmo->currentSpace.renderList.push_back(object);

        const char* activeID = GImGizmo->currentSpace.activeID;
        return (activeID && activeID == id);
    }

    bool Translate(
        const char* id,
        ImVec3* value,
        const ImVec3* position,
        const ImVec3* rotation
    )
    {
        bool active = false;

        auto drawAxis = [](const char* id, const ImVec3& pos, const ImVec3& at, const ImVec3& axis1, const ImVec3& axis2, bool inverted, ImU32 color) -> bool
        {
            ImVec3 p1 = pos;
            ImVec3 p2 = pos + (at * 1.f);
            ImVec3 p3 = p2 + (at * 0.2f);
            ImVec3 p4 = p2 + (axis1 * 0.1f);
            ImVec3 p5 = p2 + (axis2 * 0.1f);

            ImU32 innerColor = 0;
            ImU32 borderColor = 0;

            if(inverted) {
                innerColor = color;
                borderColor = ImGui::GetColorU32({0.f,0.f,0.f,0.f});
            } else {
                innerColor = ImGui::GetColorU32({0.f,0.f,0.f,0.f});
                borderColor = color;
            }

            bool active = false;
            active = active ^ DrawLine(id, p1, p2, innerColor, 1.f, borderColor);
            active = active ^ DrawTriangle(id, p3, p4, p2, innerColor, 1.f, borderColor);
            active = active ^ DrawTriangle(id, p3, p2, p5, innerColor, 1.f, borderColor);

            return active;
        };

        auto drawPlane = [](const char* id, const ImVec3& pos, const ImVec3& axis1, const ImVec3& axis2, bool inverted, ImU32 color) -> bool
        {
            ImVec3 p1 = pos + (axis1 * 0.25f) + (axis2 * 0.25f); 
            ImVec3 p2 = p1 + (axis1 * 0.25f);
            ImVec3 p3 = p1 + (axis1 * 0.25f) + (axis2 * 0.25f);
            ImVec3 p4 = p1 + (axis2 * 0.25f);

            ImU32 innerColor = 0;
            ImU32 borderColor = 0;

            if (inverted) {
                innerColor = ImGui::GetColorU32({0.f,0.f,0.f,0.f});
                borderColor = color;
            }
            else {
                innerColor = ImGui::GetColorU32({0.f,0.f,0.f,0.f});
                borderColor = color;
            }

            return DrawQuad(id, p1, p2, p3, p4, innerColor, 1.f, borderColor);
        };

        ImVec3 left = ImVec3(1.f, 0.f, 0.f);
        ImVec3 up = ImVec3(0.f, 1.f, 0.f);
        ImVec3 at = ImVec3(0.f, 0.f, 1.f);
        if(rotation != nullptr) {
            ImMat44 rotationMatrix = EulerToRotationMatrix(*rotation);
            left = ImMat44Left(rotationMatrix);
            up = ImMat44Up(rotationMatrix);
            at = ImMat44At(rotationMatrix);
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

        ImVec3 centerPos = ImVec3(0.f, 0.f, 0.f);
        if (position != nullptr) {
            centerPos = *position;
        }

        bool xAxis = drawAxis("x_axis", centerPos, left, up, at, xAxisInv, ImGui::GetColorU32({1.f,0.f,0.f,1.f}));
        bool yAxis = drawAxis("y_axis", centerPos, up, at, left, yAxisInv, ImGui::GetColorU32({0.f,1.f,0.f,1.f}));
        bool zAxis = drawAxis("z_axis", centerPos, at, left, up, zAxisInv, ImGui::GetColorU32({0.f,0.f,1.f,1.f}));

        bool xyAxis = drawPlane("xy_axis", centerPos, left, up, xAxisInv, ImGui::GetColorU32({0.f,0.f,1.f,1.f}));
        bool yzAxis = drawPlane("yz_axis", centerPos, up, at, yAxisInv, ImGui::GetColorU32({1.f,0.f,0.f,1.f}));
        bool zxAxis = drawPlane("zx_axis", centerPos, at, left, zAxisInv, ImGui::GetColorU32({0.f,1.f,0.f,1.f}));

        xAxis = xAxis ^ (xyAxis || zxAxis);
        yAxis = yAxis ^ (yzAxis || xyAxis);
        zAxis = zAxis ^ (zxAxis || yzAxis);

        DrawPoint("center_axis", centerPos, 5.f, 0.f);

        ImVec3 pos = *value;
        ImVec2 mousePos = ImGui::GetIO().MousePos;
        ImVec2 deltaPos = GetLastMousePos();
        ImVec2 pos1 = ConvertTo2DCoords(*value);
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

            *value += ImVec3Normalize(left) * diff;
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

            *value += ImVec3Normalize(up) * diff;
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

            *value += ImVec3Normalize(at) * diff;
        }

        active = active ^ xAxis;
        active = active ^ yAxis;
        active = active ^ zAxis;

        return active;
    }
};

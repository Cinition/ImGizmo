#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#ifndef IMGIZMO_API
#define IMGIZMO_API
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations, flags and enums
//-----------------------------------------------------------------------------

struct ImGizmoMatrix;
struct ImGizmoVec3;
struct ImGizmoQuaternion;
struct ImGizmoDrawList;
struct ImGizmo3DBox;
struct ImGizmoSpace;

typedef int ImGizmoAxis; //  -> enum ImGizmoAxis_ // Flags: for redefining axis to your engine

enum ImGizmoAxis_ {
    ImGizmoAxis_Forward = 0,  // Default
    ImGizmoAxis_Backward = 1, // Default
    ImGizmoAxis_Up = 2,       // Default
    ImGizmoAxis_Down = 3,     // Default
    ImGizmoAxis_Left = 4,     // Default
    ImGizmoAxis_Right = 5,    // Default
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoContext
//-----------------------------------------------------------------------------

struct ImGizmoContext
{
    ImGizmoSpace* currentSpace;
    ImVector<ImGizmoVec3> positionOffsets;
    ImVector<ImGizmoQuaternion> rotationOffsets;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoVec3
//-----------------------------------------------------------------------------

struct ImGizmoVec3 {
    float x, y, z;

    ImGizmoVec3() : x(0.f), y(0.f), z(0.f) {}
    ImGizmoVec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

    float& operator[](const size_t index) {
        return ((float*)(void*)(char*)this)[index];
    }

    const float& operator[](const size_t index) const {
        return ((const float*)(const void*)(const char*)this)[index];
    }

    IMGIZMO_API ImGizmoVec3 operator*(float rhs) const;
    IMGIZMO_API ImGizmoVec3 operator/(float rhs) const;
    IMGIZMO_API ImGizmoVec3 operator*(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator/(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator+(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 operator-(const ImGizmoVec3& rhs) const;

    IMGIZMO_API ImGizmoVec3 operator-() const;

    IMGIZMO_API ImGizmoVec3 operator*(float rhs);
    IMGIZMO_API ImGizmoVec3 operator/(float rhs);
    IMGIZMO_API ImGizmoVec3 operator*=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator/=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator+=(const ImGizmoVec3& rhs);
    IMGIZMO_API ImGizmoVec3 operator-=(const ImGizmoVec3& rhs);

    IMGIZMO_API bool operator==(const ImGizmoVec3& rhs) const;

    IMGIZMO_API float Dot(const ImGizmoVec3& rhs) const;
    IMGIZMO_API ImGizmoVec3 Cross(const ImGizmoVec3& rhs) const;
    IMGIZMO_API void Normalize();
    IMGIZMO_API ImGizmoVec3 Normalized() const;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoMatrix
//-----------------------------------------------------------------------------

struct ImGizmoMatrix {
    float m[16];

    ImGizmoMatrix() {
        m[0] = 1.f;
        m[1] = 0.f;
        m[2] = 0.f;
        m[3] = 0.f;
        m[4] = 0.f;
        m[5] = 1.f;
        m[6] = 0.f;
        m[7] = 0.f;
        m[8] = 0.f;
        m[9] = 0.f;
        m[10] = 1.f;
        m[11] = 0.f;
        m[12] = 0.f;
        m[13] = 0.f;
        m[14] = 0.f;
        m[15] = 1.f;
    }

    ImGizmoMatrix(const float* rhs) {
        m[0] = rhs[0];
        m[1] = rhs[1];
        m[2] = rhs[2];
        m[3] = rhs[3];
        m[4] = rhs[4];
        m[5] = rhs[5];
        m[6] = rhs[6];
        m[7] = rhs[7];
        m[8] = rhs[8];
        m[9] = rhs[9];
        m[10] = rhs[10];
        m[11] = rhs[11];
        m[12] = rhs[12];
        m[13] = rhs[13];
        m[14] = rhs[14];
        m[15] = rhs[15];
    }

    IMGIZMO_API ImGizmoMatrix operator*(const ImGizmoMatrix& rhs) const;
    IMGIZMO_API ImGizmoMatrix operator*=(const ImGizmoMatrix& rhs);

    IMGIZMO_API ImGizmoVec3 Transform(const ImGizmoVec3& rhs, float w) const;
    IMGIZMO_API ImGizmoMatrix Invert() const;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoQuaternion
//-----------------------------------------------------------------------------

struct ImGizmoQuaternion
{
    float x, y, z, w;

    ImGizmoQuaternion() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    ImGizmoQuaternion(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {}

    static ImGizmoQuaternion FromEuler(const float x, const float y, const float z);
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoDrawList
//-----------------------------------------------------------------------------

struct ImGizmoDrawList
{
    ImVector<ImDrawIdx> idxBuffer;
    ImVector<ImDrawVert> vertBuffer;
    ImVector<double> zBuffer;
    unsigned int _vertCurrentIdx;
    ImDrawVert* _vertWritePtr;
    ImDrawIdx* _idxWritePtr;
    double* _zWritePtr;
    ImDrawListFlags _drawListFlags;
    ImDrawListSharedData* _sharedData;

    ImGizmoDrawList()
    {
        _drawListFlags = ImDrawListFlags_None;
        _sharedData = nullptr;
        Reset();
    }

    void PushToDrawList(ImDrawList* output);

    void ReserveMesh(int idxCount, int vertCount);
    void RevokeMesh(int idxCount, int vertCount);

    void Reset()
    {
        idxBuffer.clear();
        vertBuffer.clear();
        zBuffer.clear();
        _vertCurrentIdx = 0;
        _vertWritePtr = vertBuffer.Data;
        _idxWritePtr = idxBuffer.Data;
        _zWritePtr = zBuffer.Data;
    }

    constexpr static unsigned int MaxIdx() { return sizeof(ImDrawIdx) == 2 ? 65535 : 4294967295; }
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoSpace
//-----------------------------------------------------------------------------

struct ImGizmoSpace
{
    ImGuiID ID;
    ImGizmoMatrix* viewMatrix;
    ImGizmoMatrix* projMatrix;
    ImDrawList* drawList;
    ImGizmoDrawList imGizmoDrawList;
    ImRect frameRect;
    const char* hoverID = nullptr;
    const char* activeID = nullptr;
    float nearClip = 0.1f;
    float farClip = 100.0f;
};

namespace ImGizmo {

//-----------------------------------------------------------------------------
// [SECTION] Context
//-----------------------------------------------------------------------------

    IMGIZMO_API ImGizmoContext* CreateContext();
    IMGIZMO_API void DestroyContext(ImGizmoContext* context = nullptr);
    IMGIZMO_API ImGizmoContext* GetCurrentContext();
    IMGIZMO_API void SetCurrentContext(ImGizmoContext* context);

//-----------------------------------------------------------------------------
// [SECTION] Begin/End Space
//-----------------------------------------------------------------------------

    bool Begin(const char* id, const float* viewMatrix, const float* projectionMatrix);
    void End();

//-----------------------------------------------------------------------------
// [SECTION] Gizmo variable functions
//-----------------------------------------------------------------------------

    void PushPositionOffset(const float x, const float y, const float z);
    void PopPositionOffset();

    void PushEulerRotationOffset(const float x, const float y, const float z);
    void PushQuatRotationOffset(const float x, const float y, const float z, const float w);
    void PopRotationOffset();

//-----------------------------------------------------------------------------
// [SECTION] Gizmo Functions
//-----------------------------------------------------------------------------

    void GizmoPoint(const char* id, const float* x, const float* y, const float* z);
    void GizmoTranslate(const char* id, float* x, float* y, float* z);
    void GizmoRotateEuler(const char* id, float* x, float* y, float* z);
    void GizmoRotateQuat(const char* id, float* x, float* y, float* z, float* w);
    void GizmoScale(const char* id, float* x, float* y, float* z);
}

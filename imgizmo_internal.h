#include "imgizmo.h"
#include "imgui_internal.h"

struct ImGizmoMatrix;
struct ImGizmoVec3;

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
    float nearClip = 0.1f;
    float farClip = 100.0f;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGizmoContext
//-----------------------------------------------------------------------------

struct ImGizmoContext
{
    ImGizmoSpace* currentSpace;
};

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

struct ImGizmoMatrix {
    // Row major
    // [ 11 12 13 14 ] 0 => 3
    // [ 21 22 23 24 ] 4 => 7
    // [ 31 32 33 34 ] 8 => 11
    // [ 41 42 43 44 ] 12 => 15

    // [ 00 01 02 03 ] 0 1 2 3
    // [ 10 11 12 13 ] 4 5 6 7
    // [ 20 21 22 23 ] 8 9 10 11
    // [ 30 31 32 33 ] 12 13 14 15

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
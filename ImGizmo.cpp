#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cmath>
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
constexpr float EPSILON = 0.00001f;

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

ImGizmoVec3 ImGizmoVec3::operator*(float rhs) const
{
    return ImGizmoVec3(this->x * rhs, this->y * rhs, this->z * rhs);
}

ImGizmoVec3 ImGizmoVec3::operator/(float rhs) const
{
    return ImGizmoVec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

ImGizmoVec3 ImGizmoVec3::operator*(const ImGizmoVec3& rhs) const
{
    return ImGizmoVec3(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
}

ImGizmoVec3 ImGizmoVec3::operator/(const ImGizmoVec3& rhs) const
{
    return ImGizmoVec3(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
}

ImGizmoVec3 ImGizmoVec3::operator+(const ImGizmoVec3& rhs) const
{
    return ImGizmoVec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

ImGizmoVec3 ImGizmoVec3::operator-(const ImGizmoVec3& rhs) const
{
    return ImGizmoVec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

ImGizmoVec3 ImGizmoVec3::operator-() const
{
    return ImGizmoVec3(-this->x, -this->y, -this->z);
}

ImGizmoVec3 ImGizmoVec3::operator*(float rhs)
{
    return ImGizmoVec3(this->x * rhs, this->y * rhs, this->z * rhs);
}

ImGizmoVec3 ImGizmoVec3::operator/(float rhs)
{
    return ImGizmoVec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

ImGizmoVec3 ImGizmoVec3::operator*=(const ImGizmoVec3& rhs)
{
    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    return *this;
}

ImGizmoVec3 ImGizmoVec3::operator/=(const ImGizmoVec3& rhs)
{
    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;
    return *this;
}

ImGizmoVec3 ImGizmoVec3::operator+=(const ImGizmoVec3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

ImGizmoVec3 ImGizmoVec3::operator-=(const ImGizmoVec3& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

bool ImGizmoVec3::operator==(const ImGizmoVec3& rhs) const
{
    return ((this->x - rhs.x) < EPSILON) &&
           ((this->y - rhs.y) < EPSILON) &&
           ((this->z - rhs.z) < EPSILON);
}

float ImGizmoVec3::Dot(const ImGizmoVec3& rhs) const
{
    return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}

ImGizmoVec3 ImGizmoVec3::Cross(const ImGizmoVec3& rhs) const
{
    return ImGizmoVec3(
        this->y * rhs.z - this->z * rhs.y,
        this->z * rhs.x - this->x * rhs.z,
        this->x * rhs.y - this->y * rhs.x
    );
}

void ImGizmoVec3::Normalize()
{
    const float magnitude = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    if (magnitude != 0)
    {
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
    }
    else
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
}

ImGizmoVec3 ImGizmoVec3::Normalized() const
{
    const float magnitude = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    if (magnitude != 0)
    {
        return *this / magnitude;
    }
    else
    {
        return ImGizmoVec3();
    }
}

ImGizmoMatrix ImGizmoMatrix::operator*(const ImGizmoMatrix& rhs) const
{
    ImGizmoMatrix out;

    out.m[0] = this->m[0] * rhs.m[0] + this->m[1] * rhs.m[4] + this->m[2] * rhs.m[8] + this->m[3] * rhs.m[12];
    out.m[1] = this->m[0] * rhs.m[1] + this->m[1] * rhs.m[5] + this->m[2] * rhs.m[9] + this->m[3] * rhs.m[13];
    out.m[2] = this->m[0] * rhs.m[2] + this->m[1] * rhs.m[6] + this->m[2] * rhs.m[10] + this->m[3] * rhs.m[14];
    out.m[3] = this->m[0] * rhs.m[3] + this->m[1] * rhs.m[7] + this->m[2] * rhs.m[11] + this->m[3] * rhs.m[15];

    out.m[4] = this->m[4] * rhs.m[0] + this->m[5] * rhs.m[4] + this->m[6] * rhs.m[8] + this->m[7] * rhs.m[12];
    out.m[5] = this->m[4] * rhs.m[1] + this->m[5] * rhs.m[5] + this->m[6] * rhs.m[9] + this->m[7] * rhs.m[13];
    out.m[6] = this->m[4] * rhs.m[2] + this->m[5] * rhs.m[6] + this->m[6] * rhs.m[10] + this->m[7] * rhs.m[14];
    out.m[7] = this->m[4] * rhs.m[3] + this->m[5] * rhs.m[7] + this->m[6] * rhs.m[11] + this->m[7] * rhs.m[15];

    out.m[8] = this->m[8] * rhs.m[0] + this->m[9] * rhs.m[4] + this->m[10] * rhs.m[8] + this->m[11] * rhs.m[12];
    out.m[9] = this->m[8] * rhs.m[1] + this->m[9] * rhs.m[5] + this->m[10] * rhs.m[9] + this->m[11] * rhs.m[13];
    out.m[10] = this->m[8] * rhs.m[2] + this->m[9] * rhs.m[6] + this->m[10] * rhs.m[10] + this->m[11] * rhs.m[14];
    out.m[11] = this->m[8] * rhs.m[3] + this->m[9] * rhs.m[7] + this->m[10] * rhs.m[11] + this->m[11] * rhs.m[15];

    out.m[12] = this->m[12] * rhs.m[0] + this->m[13] * rhs.m[4] + this->m[14] * rhs.m[8] + this->m[15] * rhs.m[12];
    out.m[13] = this->m[12] * rhs.m[1] + this->m[13] * rhs.m[5] + this->m[14] * rhs.m[9] + this->m[15] * rhs.m[13];
    out.m[14] = this->m[12] * rhs.m[2] + this->m[13] * rhs.m[6] + this->m[14] * rhs.m[10] + this->m[15] * rhs.m[14];
    out.m[15] = this->m[12] * rhs.m[3] + this->m[13] * rhs.m[7] + this->m[14] * rhs.m[11] + this->m[15] * rhs.m[15];

    return out;
}

ImGizmoMatrix ImGizmoMatrix::operator*=(const ImGizmoMatrix& rhs)
{
    return *this = *this * rhs;
}

ImGizmoVec3 ImGizmoMatrix::Transform(const ImGizmoVec3& rhs, float w) const
{
    ImVec4 in;
    ImGizmoVec3 out;

    in.x = rhs.x * this->m[0] + rhs.y * this->m[4] + rhs.z * this->m[8] + this->m[12];
    in.y = rhs.x * this->m[1] + rhs.y * this->m[5] + rhs.z * this->m[9] + this->m[13];
    in.z = rhs.x * this->m[2] + rhs.y * this->m[6] + rhs.z * this->m[10] + this->m[14];
    in.w = rhs.x * this->m[3] + rhs.y * this->m[7] + rhs.z * this->m[11] + this->m[15];

    in.x /= in.w;
    in.y /= in.w;
    in.z /= in.w;

    out.x = in.x;
    out.y = in.y;
    out.z = in.z;

    return out;
}

ImGizmoMatrix ImGizmoMatrix::Invert() const
{
    float a2323 = this->m[9] * this->m[15] - this->m[11] * this->m[13];
    float a1323 = this->m[9] * this->m[15] - this->m[11] * this->m[12];
    float a1223 = this->m[9] * this->m[14] - this->m[10] * this->m[12];
    float a0323 = this->m[8] * this->m[15] - this->m[11] * this->m[12];
    float a0223 = this->m[8] * this->m[14] - this->m[10] * this->m[12];
    float a0123 = this->m[8] * this->m[13] - this->m[9] * this->m[12];
    float a2313 = this->m[6] * this->m[15] - this->m[7] * this->m[14];
    float a1313 = this->m[5] * this->m[15] - this->m[7] * this->m[13];
    float a1213 = this->m[5] * this->m[14] - this->m[6] * this->m[13];
    float a2312 = this->m[6] * this->m[11] - this->m[7] * this->m[10];
    float a1312 = this->m[5] * this->m[11] - this->m[7] * this->m[9];
    float a1212 = this->m[5] * this->m[10] - this->m[6] * this->m[9];
    float a0313 = this->m[4] * this->m[15] - this->m[7] * this->m[12];
    float a0213 = this->m[4] * this->m[14] - this->m[6] * this->m[12];
    float a0312 = this->m[4] * this->m[11] - this->m[7] * this->m[8];
    float a0212 = this->m[4] * this->m[10] - this->m[6] * this->m[8];
    float a0113 = this->m[4] * this->m[13] - this->m[5] * this->m[12];
    float a0112 = this->m[4] * this->m[9] - this->m[5] * this->m[8];

    float det = this->m[0] * (this->m[5] * a2323 - this->m[6] * a1323 + this->m[7] * a1223)
        - this->m[1] * (this->m[4] * a2323 - this->m[6] * a0323 + this->m[7] * a0223)
        + this->m[2] * (this->m[4] * a1323 - this->m[5] * a0323 + this->m[7] * a0123)
        - this->m[3] * (this->m[4] * a1223 - this->m[5] * a0223 + this->m[6] * a0123);
    det = 1.0f / det;

    ImGizmoMatrix mat;
    mat.m[0] = det * (this->m[5] * a2323 - this->m[6] * a1323 + this->m[7] * a1223);
    mat.m[1] = det * -(this->m[1] * a2323 - this->m[2] * a1323 + this->m[3] * a1223);
    mat.m[2] = det * (this->m[1] * a2313 - this->m[2] * a1313 + this->m[3] * a1213);
    mat.m[3] = det * -(this->m[1] * a2312 - this->m[2] * a1312 + this->m[3] * a1212);

    mat.m[4] = det * -(this->m[4] * a2323 - this->m[5] * a0323 + this->m[6] * a0223);
    mat.m[5] = det * (this->m[0] * a2323 - this->m[2] * a0323 + this->m[3] * a0223);
    mat.m[6] = det * -(this->m[0] * a2313 - this->m[2] * a0313 + this->m[3] * a0213);
    mat.m[7] = det * (this->m[0] * a2312 - this->m[2] * a0312 + this->m[3] * a0212);

    mat.m[8] = det * (this->m[4] * a1323 - this->m[5] * a0323 + this->m[6] * a0123);
    mat.m[9] = det * -(this->m[0] * a1323 - this->m[1] * a0323 + this->m[3] * a0123);
    mat.m[10] = det * (this->m[0] * a1313 - this->m[1] * a0313 + this->m[3] * a0113);
    mat.m[11] = det * -(this->m[0] * a1312 - this->m[1] * a0312 + this->m[3] * a0112);

    mat.m[12] = det * -(this->m[4] * a1223 - this->m[5] * a0223 + this->m[6] * a0123);
    mat.m[13] = det * (this->m[0] * a1223 - this->m[1] * a0223 + this->m[2] * a0123);
    mat.m[14] = det * -(this->m[0] * a1213 - this->m[1] * a0213 + this->m[2] * a0113);
    mat.m[15] = det * (this->m[0] * a1212 - this->m[1] * a0212 + this->m[2] * a0112);

    return mat;
}

ImGizmoQuaternion ImGizmoQuaternion::FromEuler(const float x, const float y, const float z)
{
    //TODO
    return ImGizmoQuaternion(x, y, z, 0.0f);
}

void ImGizmoDrawList::PushToDrawList(ImDrawList* output)
{
    const int triCount = this->zBuffer.Size;
    if (triCount == 0)
    {
        this->Reset();
        return;
    }

    struct TriRef {
        double z;
        int triIdx;
    };
    TriRef* tris = (TriRef*)IM_ALLOC(sizeof(TriRef) * triCount);
    for (int i = 0; i < triCount; i++) {
        tris[i].z = this->zBuffer[i];
        tris[i].triIdx = i;
    }

    ImQsort(tris, (size_t)triCount, sizeof(TriRef), [](const void* a, const void* b) {
        double za = ((const TriRef*)a)->z;
        double zb = ((const TriRef*)b)->z;
        return (za < zb) ? -1 : (za > zb) ? 1 : 0;
    });

    output->PrimReserve(this->idxBuffer.Size, this->vertBuffer.Size);

    memcpy(output->_VtxWritePtr, this->vertBuffer.Data, this->vertBuffer.Size * sizeof(ImDrawVert));
    unsigned int idxOffset = output->_VtxCurrentIdx;
    output->_VtxWritePtr += this->vertBuffer.Size;
    output->_VtxWritePtr += this->vertBuffer.Size;

    unsigned int maxIndex = this->MaxIdx() - idxOffset;

    ImDrawIdx* idxOutBegin = output->_IdxWritePtr;
    ImDrawIdx* idxOut = idxOutBegin;
    ImDrawIdx* idxIn = this->idxBuffer.Data;
    for (unsigned int i = 0; i < triCount; i++)
    {
        int triIdx = tris[i].triIdx;
        int baseIdx = triIdx * 3;

        unsigned int i0 = idxIn[baseIdx];
        unsigned int i1 = idxIn[baseIdx + 1];
        unsigned int i2 = idxIn[baseIdx + 2];

        if (i0 > maxIndex || i1 > maxIndex || i2 > maxIndex)
        {
            break;
        }

        idxOut[0] = (ImDrawIdx)(i0 + idxOffset);
        idxOut[1] = (ImDrawIdx)(i1 + idxOffset);
        idxOut[2] = (ImDrawIdx)(i2 + idxOffset);

        idxOut += 3;
    }
    ImDrawIdx* idxOutEnd = idxOut;
    output->_IdxWritePtr = idxOutEnd;

    Reset();

    IM_FREE(tris);
}

void ImGizmoDrawList::ReserveMesh(int idxCount, int vertCount)
{
    int vertBufferSize = this->vertBuffer.Size;
    this->vertBuffer.resize(vertBufferSize + vertCount);
    this->_vertWritePtr = this->vertBuffer.Data + vertBufferSize;

    int idxBufferSize = this->idxBuffer.Size;
    this->idxBuffer.resize(idxBufferSize + idxCount);
    this->_idxWritePtr = this->idxBuffer.Data + idxBufferSize;

    int zBufferSize = this->zBuffer.Size;
    this->zBuffer.resize(zBufferSize + idxCount / 3);
    this->_zWritePtr = this->zBuffer.Data + zBufferSize;
}






#pragma once

void FPU_MatrixF_x_MatrixF(const float* a, const float* b, float* r)
{
  r[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
  r[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
  r[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
  r[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

  r[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
  r[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
  r[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
  r[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

  r[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
  r[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
  r[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
  r[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

  r[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
  r[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
  r[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
  r[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

struct ImGizmoMatrix;
struct ImGizmoVec {
    float x;
    float y;
    float z;
    float w;

    ImGizmoVec() {
        x = 0.f;
        y = 0.f;
        z = 0.f;
        w = 0.f;
    }

    ImGizmoVec(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    ImGizmoVec Transform(const ImGizmoMatrix& _matrix);
};

struct ImGizmoMatrix {
    union {
        float m4x4[4][4];
        float m16[16];
    };

    ImGizmoMatrix() {
        m4x4[0][0] = m4x4[0][1] = m4x4[0][2] = m4x4[0][3] =
        m4x4[1][0] = m4x4[1][1] = m4x4[1][2] = m4x4[1][3] =
        m4x4[2][0] = m4x4[2][1] = m4x4[2][2] = m4x4[2][3] =
        m4x4[3][0] = m4x4[3][1] = m4x4[3][2] = m4x4[3][3] = 0.f;
    };

    ImGizmoMatrix(float* _matrix) {
        m16[0] = _matrix[0];
        m16[1] = _matrix[1];
        m16[2] = _matrix[2];
        m16[3] = _matrix[3];
        m16[4] = _matrix[4];
        m16[5] = _matrix[5];
        m16[6] = _matrix[6];
        m16[7] = _matrix[7];
        m16[8] = _matrix[8];
        m16[9] = _matrix[9];
        m16[10] = _matrix[10];
        m16[11] = _matrix[11];
        m16[12] = _matrix[12];
        m16[13] = _matrix[13];
        m16[14] = _matrix[14];
        m16[15] = _matrix[15];
    }

    static ImGizmoMatrix Indentity() {
        ImGizmoMatrix m;

        m.m4x4[0][1] = m.m4x4[0][2] = m.m4x4[0][3] =
            m.m4x4[1][0] = m.m4x4[1][2] = m.m4x4[1][3] =
            m.m4x4[2][0] = m.m4x4[2][1] = m.m4x4[2][3] =
            m.m4x4[3][0] = m.m4x4[3][1] = m.m4x4[3][2] = 0.f;

        m.m4x4[0][0] = m.m4x4[1][1] = m.m4x4[2][2] = m.m4x4[3][3] = 1.f;

        return m;
    };

    ImGizmoMatrix operator * (const ImGizmoMatrix& _matrix) const {
        ImGizmoMatrix out;
        FPU_MatrixF_x_MatrixF(this->m16, _matrix.m16, out.m16);
        return out;
    }
};

ImGizmoVec ImGizmoVec::Transform(const ImGizmoMatrix& _matrix) {
    ImGizmoVec out;

    out.x = this->x * _matrix.m4x4[0][0] + this->y * _matrix.m4x4[1][0] + this->z * _matrix.m4x4[2][0] + _matrix.m4x4[3][0];
    out.y = this->x * _matrix.m4x4[0][1] + this->y * _matrix.m4x4[1][1] + this->z * _matrix.m4x4[2][1] + _matrix.m4x4[3][1];
    out.z = this->x * _matrix.m4x4[0][2] + this->y * _matrix.m4x4[1][2] + this->z * _matrix.m4x4[2][2] + _matrix.m4x4[3][2];
    out.w = this->x * _matrix.m4x4[0][3] + this->y * _matrix.m4x4[1][3] + this->z * _matrix.m4x4[2][3] + _matrix.m4x4[3][3];

    if(out.w != 1.f) {
        out.x /= out.w;
        out.y /= out.w;
        out.z /= out.w;
    }

    this->x = out.x;
    this->y = out.y;
    this->z = out.z;
    this->w = out.w;

    return *this;
}

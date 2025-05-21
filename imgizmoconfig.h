#pragma once

//---- Define constructor and implicit cast operators to convert back<->forth between your math types and ImVec3/ImMat44.
// This will be inlined as part of ImVec3 and ImMat44 class declarations.
/*
#define IM_VEC3_CLASS_EXTRA                                                     \
        constexpr ImVec3(const MyVec2& f) : x(f.x), y(f.y) {}                   \
        operator MyVec2() const { return MyVec2(x,y); }

#define IM_MAT44_CLASS_EXTRA                                                    \
        constexpr ImVec4(const MyVec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {}   \
        operator MyVec4() const { return MyVec4(x,y,z,w); }
*/
//---- ...Or use Dear ImGui's own very basic math operators.
//#define IMGUI_DEFINE_MATH_OPERATORS

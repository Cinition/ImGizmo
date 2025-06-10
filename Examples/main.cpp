#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <cstdio>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define IM_MAT44_CLASS_EXTRA        \
    ImMat44(const glm::mat4& m) {   \
        auto f = glm::value_ptr(m); \
        m16[0] = f[0];              \
        m16[1] = f[1];              \
        m16[2] = f[2];              \
        m16[3] = f[3];              \
        m16[4] = f[4];              \
        m16[5] = f[5];              \
        m16[6] = f[6];              \
        m16[7] = f[7];              \
        m16[8] = f[8];              \
        m16[9] = f[9];              \
        m16[10] = f[10];            \
        m16[11] = f[11];            \
        m16[12] = f[12];            \
        m16[13] = f[13];            \
        m16[14] = f[14];            \
        m16[15] = f[15];            \
    }                               \

#define IM_VEC3_CLASS_EXTRA \
    ImVec3(glm::vec3* v) {  \
        x = v->x;           \
        y = v->y;           \
        z = v->z;           \
    }                       \

#include <ImGizmo.h>

#define IMGUI_DEFINE_MATH_OPERATORS

struct Cube {
    float vertices[216] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 position {0.f, 0.f, 0.f};
    glm::vec3 rotation {0.f, 0.f, 0.f};
    glm::vec3 scale    {1.f, 1.f, 1.f};
};

struct Shader {
    unsigned int ID;

    Shader() {
        const char* vShaderCode =
            "#version 460 core\n"
            "layout (location = 0) in vec3 aPosition;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "uniform mat4 modelMatrix;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "out vec3 Color;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * modelMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0f);\n"
            "    Color = aNormal;\n"
            "}\0";

        const char * fShaderCode =
            "#version 460 core\n"
            "in vec3 Color;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(abs(Color), 1.0f);\n"
            "}\0";

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
};

void errorCallBack(int error, const char* desc) {
    std::printf("Error: %s\n", desc);
}

int main(int argc, char** argv) {
    if(!glfwInit())
    {
        std::printf("GLFW Initialization failed");
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(errorCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "ImGizmo Example Window", NULL, NULL);
    if (!window) {
        std::printf("GLFW Window creation failed");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::printf("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    Cube cube1;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube1.vertices), cube1.vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Init Shader
    Shader shader = Shader();

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGizmo::CreateContext();

    bool handleMouseInput = false;
    bool renderScene = false;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);

        glViewport(0, 0, displayW, displayH);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 proj = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));
        proj = glm::perspective(glm::radians(45.f), 1.778f, 0.1f, 100.f);

        // Update scene
        {
            ImGuiIO& io = ImGui::GetIO();
            handleMouseInput = !(io.WantCaptureMouse || ImGizmo::IsUsing());

            glm::mat4 rot = glm::mat4(1.f);

            // Mouse camera rotation
            static glm::vec2 rotation     = glm::vec2(0.f);
            static glm::vec2 lastMousePos = glm::vec2(0.f);

            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            glm::vec2 mousePos = glm::vec2(xPos, yPos);
            if(handleMouseInput) {
                if( glfwGetMouseButton(window, 0) == GLFW_PRESS) {
                    glm::vec2 relativePos = lastMousePos - mousePos;
                    rotation += (relativePos * 0.005f) * glm::vec2(-1.f, -1.f);
                }
            }
            lastMousePos.x = static_cast<float>(xPos);
            lastMousePos.y = static_cast<float>(yPos);

            rot = glm::rotate(rot, rotation.y, glm::vec3(1.f, 0.f, 0.f));
            rot = glm::rotate(rot, rotation.x, glm::vec3(0.f, 1.f, 0.f));
            view = view * rot;
        }

        // Render Scene
        {
            if(renderScene)
            {
                int viewMatrixLocation = glGetUniformLocation(shader.ID, "view");
                glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));

                int projectionMatrixLocation = glGetUniformLocation(shader.ID, "projection");
                glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(proj));

                glm::mat4 modelMatrix = glm::mat4(1.f);
                modelMatrix = glm::translate(modelMatrix, cube1.position);
                modelMatrix = glm::rotate(modelMatrix, glm::radians(cube1.rotation.x), glm::vec3(1.f, 0.f, 0.f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(cube1.rotation.y), glm::vec3(0.f, 1.f, 0.f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(cube1.rotation.z), glm::vec3(0.f, 0.f, 1.f));
                modelMatrix = glm::scale(modelMatrix, cube1.scale);
                int modelMatrixLocation = glGetUniformLocation(shader.ID, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

                glUseProgram(shader.ID);
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }

        // Render ImGui
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static bool showDemoWindow = false;
            if(showDemoWindow) {
                ImGui::ShowDemoWindow(&showDemoWindow);
            }

            if(ImGizmo::Begin("ImGizmo Space",view, proj)) {
                ImGizmo::Translate("Translate", (ImVec3*)&cube1.position);
                ImGizmo::End();
            }

            ImGui::SetWindowSize({300.f , 400.f});
            ImGui::Begin("ImGizmo Demo");

            if(ImGui::Button("Toggle ImGui Debugging")) {
                showDemoWindow = !showDemoWindow;
            }

            if(ImGui::Button("Toggle Cube")) {
                renderScene = !renderScene;
            }

            ImGui::SeparatorText("Cube");
            ImGui::Text("Position:");
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("X:##Pos", ImGuiDataType_Float, &cube1.position.x, 0.1f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Y:##Pos", ImGuiDataType_Float, &cube1.position.y, 0.1f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Z:##Pos", ImGuiDataType_Float, &cube1.position.z, 0.1f);
            ImGui::Dummy({1.f, 10.f});

            ImGui::Text("Rotation:");
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("X:##Rot", ImGuiDataType_Float, &cube1.rotation.x, 1.f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Y:##Rot", ImGuiDataType_Float, &cube1.rotation.y, 1.f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Z:##Rot", ImGuiDataType_Float, &cube1.rotation.z, 1.f);
            ImGui::Dummy({1.f, 10.f});

            ImGui::Text("Scale:");
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("X:##Scl", ImGuiDataType_Float, &cube1.scale.x, 0.1f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Y:##Scl", ImGuiDataType_Float, &cube1.scale.y, 0.1f);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            ImGui::DragScalar("Z:##Scl", ImGuiDataType_Float, &cube1.scale.z, 0.1f);
            ImGui::Dummy({1.f, 10.f});

            if(ImGui::Button("Reset Transform")) {
                cube1.position = glm::vec3(0.f);
                cube1.rotation = glm::vec3(0.f);
                cube1.scale = glm::vec3(1.f);
            }

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        glfwSwapBuffers(window);
    }

    // Destroy ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGizmo::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

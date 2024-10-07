#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGizmo.h>

struct Cube {
    glm::vec3 vertices[8] {
        {-0.5, -0.5, -0.5f}, // 0
        {0.5, -0.5, -0.5f},  // 1
        {0.5, -0.5, 0.5f},   // 2
        {-0.5, -0.5, 0.5f},  // 3
        {-0.5, 0.5, -0.5f},  // 4
        {0.5, 0.5, -0.5f},   // 5
        {0.5, 0.5, 0.5f},    // 6
        {-0.5, 0.5, 0.5f},   // 7
    };

    float indices[36] {
        0, 1, 4, 4, 1, 5, // front
        1, 2, 5, 5, 2, 6, // left
        2, 3, 6, 6, 3, 7, // back
        3, 4, 7, 7, 4, 8, // right
        4, 5, 7, 7, 5, 6, // top
        0, 1, 3, 3, 1, 2, // bottom
    };

    glm::vec3 position {0.f, 0.f, 0.f};
    glm::vec3 rotation {0.f, 0.f, 0.f};
    glm::vec3 scale    {1.f, 1.f, 1.f};
};

struct Shader {
    unsigned int ID;

    Shader() {
        const char* vShaderCode =
            "#version 330 core\n"
            "layout (location = 0) in vec3 pos;\n"
            "out vec3 normal;\n"
            "uniform mat4 modelMatrix;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = projection * view * modelMatrix * vec4(pos, 1.0);\n"
            "    normal = normalize(gl_NormalMatrix * gl_Normal.xyz);\n"
            "}\n";

        const char * fShaderCode =
            "#version 330 core\n"
            "in vec3 normal;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(normal, 1.0);\n"
            "}\n";

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
        // Initialization failed
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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::printf("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Cube cube1;

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube1.vertices), cube1.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube1.indices), cube1.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Init Shader
    Shader shader = Shader();
    std::printf(std::to_string(shader.ID).c_str());

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);

        glUseProgram(shader.ID);

        // Render Scene
        {
            glm::mat4 view = glm::mat4(1.f);
            glm::mat4 proj = glm::mat4(1.f);
            proj = glm::perspective(glm::radians(45.f), (float)displayW / (float)displayH, 0.1f, 100.f);
            view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));

            glBindVertexArray(VAO);
            glm::mat4 modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, cube1.position);
            int modelMatrixLocation = glGetUniformLocation(shader.ID, "modelMatrix");
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

        // Render ImGui
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            ImGui::Render();
        }

        glViewport(0, 0, displayW, displayH);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Destroy ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

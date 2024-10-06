#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGizmo.h>

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGizmo Example Window", NULL, NULL);
    if (!window) {
        // Window creation failed
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //Failed to initialize GLAD
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, 800, 600);

    while (!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

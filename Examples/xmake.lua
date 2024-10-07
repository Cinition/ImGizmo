set_languages("cxx17")

add_requires("imgui v1.91.1-docking", {configs = {opengl3 = true, glfw = true}})
add_requires("glad", "glfw", "glm")

target("ImGimzo")
    set_kind("binary")
    add_files("*.cpp")
    add_includedirs( "../" )
    add_packages("imgui", "glfw", "glad", "glm")

set_languages("cxx17")

set_symbols("hidden")
set_optimize("fastest")
set_strip("all")

add_rules("plugin.compile_commands.autoupdate");

add_requires("imgui v1.91.1-docking", {configs = {opengl3 = true, glfw = true}})
add_requires("glad", "glfw", "glm")

target("ImGizmo_Dev")
    set_kind("binary")
    add_files("*.cpp", "../*.cpp")
    add_includedirs( "../" )
    add_packages("imgui", "glfw", "glad", "glm")
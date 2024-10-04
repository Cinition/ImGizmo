set_languages("cxx17")

add_requires("imgui v1.91.1-docking", {configs = {opengl3 = true, sdl2 = true}})
add_requires("glad")
add_requires("libsdl")

target("ImGimzo")
    set_kind("console")
    add_files("*.cpp")
    add_includedirs( "../" )
    add_packages("imgui", "libsdl", "glad")
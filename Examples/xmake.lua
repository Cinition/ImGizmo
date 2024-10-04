set_languages("cxx17")
add_rules("mode.debug", "mode.release")

add_requires("imgui")

target("ImGimzo")
    set_kind("binary")
    add_files("main.cpp")
    add_includedirs( "../" )
    add_packages("imgui")
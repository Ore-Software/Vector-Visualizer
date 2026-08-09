include(FetchContent)

# Keep dependency configuration out of the normal build UI.
mark_as_advanced(FETCHCONTENT_BASE_DIR)

# ---------------------------------------------------------------------------
# GLFW
# ---------------------------------------------------------------------------

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    TRUE
)

# ---------------------------------------------------------------------------
# GLM
# ---------------------------------------------------------------------------

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
    GIT_SHALLOW    TRUE
)

# ---------------------------------------------------------------------------
# GLAD 2
#
# Generated at configure time for the exact profile the app requests
# (Window.cpp asks GLFW for a 4.6 core context).
# ---------------------------------------------------------------------------

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        658f48e72aee3c6582e80b05ac0f8787a64fe6bb
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  cmake
)

# ---------------------------------------------------------------------------
# Dear ImGui
#
# Dear ImGui does not provide the application-specific target we need, so
# obtain its sources here and define the target below.
# ---------------------------------------------------------------------------

FetchContent_Declare(
    imgui_source
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.9b
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(
    glfw
    glm
    glad
    imgui_source
)

# Generate one GLAD target for the OpenGL profile used by the application.
glad_add_library(
    vector_visualizer_glad
    STATIC
    REPRODUCIBLE
    API gl:core=4.6
)

# Dear ImGui core plus the GLFW/OpenGL 3 backends.
add_library(vector_visualizer_imgui STATIC)

target_sources(
    vector_visualizer_imgui
    PRIVATE
        "${imgui_source_SOURCE_DIR}/imgui.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_source_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
        "${imgui_source_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(
    vector_visualizer_imgui
    SYSTEM
    PUBLIC
        "${imgui_source_SOURCE_DIR}"
        "${imgui_source_SOURCE_DIR}/backends"
)

target_link_libraries(
    vector_visualizer_imgui
    PUBLIC
        glfw
        vector_visualizer_glad
)

# Avoid GLFW including the platform OpenGL headers before GLAD.
target_compile_definitions(
    vector_visualizer_imgui
    PUBLIC
        GLFW_INCLUDE_NONE
)

# Stable project-owned aliases. The application does not need to know how
# each dependency was obtained.
add_library(VectorVisualizer::GLAD  ALIAS vector_visualizer_glad)
add_library(VectorVisualizer::ImGui ALIAS vector_visualizer_imgui)

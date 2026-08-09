set(PROJECT_NAME Vector-Visualizer)

################################################################################
# Source files
################################################################################
set(Source_Files
    "src/main.cpp"
    "src/modes/ModeMenu.cpp"
    "src/modes/ModeBrownian.cpp"
    "src/modes/ModeVectorMultiple.cpp"
    "src/modes/ModeVectorTransformation.cpp"
    "src/renderer/Camera.cpp"
    "src/renderer/Shader.cpp"
    "src/renderer/VertexArray.cpp"
    "src/renderer/VertexBuffer.cpp"
    "src/VectorObject.cpp"
    "src/Window.cpp"
)

set(Header_Files
    "src/BufferUtils.h"
    "src/modes/Mode.h"
    "src/modes/ModeMenu.h"
    "src/modes/ModeBrownian.h"
    "src/modes/ModeVectorMultiple.h"
    "src/modes/ModeVectorTransformation.h"
    "src/renderer/Camera.h"
    "src/renderer/Shader.h"
    "src/renderer/VertexArray.h"
    "src/renderer/VertexBuffer.h"
    "src/renderer/VertexBufferLayout.h"
    "src/VectorObject.h"
    "src/Window.h"
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME}
    ${Source_Files}
    ${Header_Files}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:_DEBUG>"
    "$<$<CONFIG:Release>:NDEBUG>"
    "_CONSOLE"
    "UNICODE"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++17;
        /sdl;
        /W3;
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /DEBUG;
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Dependencies
################################################################################
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    VectorVisualizer::GLAD
    VectorVisualizer::ImGui
)

################################################################################
# Copy resources to output
################################################################################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini"

    VERBATIM
)

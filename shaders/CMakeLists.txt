file(GLOB SHADERS "*.vert" "*.frag" )

add_library(Shaders STATIC ${SHADERS})

set_target_properties(Shaders PROPERTIES LINKER_LANGUAGE CXX)

source_group("Shaders" FILES ${SHADERS})

set_target_properties(Shaders PROPERTIES FOLDER "")
MACRO(RE2C VAR)
    IF(NOT RE2C_EXECUTABLE)
        FIND_PACKAGE(RE2C REQUIRED)
    ENDIF()
    FOREACH(SRC ${ARGN})
        set(DST "${CMAKE_CURRENT_BINARY_DIR}")
        GET_FILENAME_COMPONENT(NAME "${SRC}" NAME_WE)
        ADD_CUSTOM_COMMAND(
            OUTPUT "${DST}/${NAME}.cpp"
            COMMAND ${RE2C_EXECUTABLE}
            ARGS -o "${DST}/${NAME}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}"
            DEPENDS "${SRC}"
            COMMENT "[re2c] making lexer"
        )
        SET(${VAR} ${${VAR}} "${DST}/${NAME}.cpp")
    ENDFOREACH()
ENDMACRO()



cmake_minimum_required(VERSION 3.16)

project(BasicInterpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GRAMMAR_PREFIX "TEH")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

find_package(antlr4-runtime REQUIRED)

if(TARGET antlr4_runtime)
    set(ANTLR4_RUNTIME_TARGET antlr4_runtime)
elseif(TARGET antlr4_shared)
    set(ANTLR4_RUNTIME_TARGET antlr4_shared)
else()
    message(FATAL_ERROR "Could not find a supported ANTLR4 runtime target")
endif()

set(PROJECT_SOURCES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/SemanticVisitor.cpp"
)

set(ANTLR_GENERATED_SOURCES
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Lexer.cpp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Parser.cpp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserBaseVisitor.cpp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserVisitor.cpp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserBaseListener.cpp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserListener.cpp"
)

set(ANTLR_GENERATED_HEADERS
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Lexer.h"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Parser.h"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserBaseVisitor.h"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserVisitor.h"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserBaseListener.h"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}ParserListener.h"
)

set(ANTLR_GENERATED_SUPPORT_FILES
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Lexer.tokens"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Lexer.interp"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Parser.tokens"
    "${GENERATED_DIR}/${GRAMMAR_PREFIX}Parser.interp"
)

foreach(generated_file
    IN LISTS
        ANTLR_GENERATED_SOURCES
        ANTLR_GENERATED_HEADERS
        ANTLR_GENERATED_SUPPORT_FILES
)
    if(NOT EXISTS "${generated_file}")
        message(FATAL_ERROR
            "Missing generated ANTLR file: ${generated_file}\n"
            "Run generate_cpp_classes.sh before configuring the project."
        )
    endif()
endforeach()

add_executable(tehpajler
    ${PROJECT_SOURCES}
    ${ANTLR_GENERATED_SOURCES}
    ${ANTLR_GENERATED_HEADERS}
)

target_include_directories(tehpajler PRIVATE
    "${SRC_DIR}"
    "${GENERATED_DIR}"
)

target_link_libraries(tehpajler PRIVATE ${ANTLR4_RUNTIME_TARGET})

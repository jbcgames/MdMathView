cmake_minimum_required(VERSION 3.15)
project(MdMathView LANGUAGES C)

include(GenerateG1A)
find_package(Gint 2.9 REQUIRED)

add_executable(MdMathView
  src/main.c
)
target_compile_options(MdMathView PRIVATE -Wall -Wextra -Os)
target_link_libraries(MdMathView Gint::Gint)

generate_g1a(TARGET MdMathView OUTPUT "MdMathView.g1a"
  NAME "MdMathView"
  ICON "assets/icon.png"
)

cmake_minimum_required(VERSION 3.0)

project(writepng)

set(CMAKE_C_FLAGS "-Wall -Werror")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -coverage")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(tests)

enable_testing()

add_test(NAME wp_adler32_0123 COMMAND wp_adler32 0123 1ee00c7)
add_test(NAME wp_adler32_abc  COMMAND wp_adler32  abc 24d0127)
add_test(NAME wp_adler32_CD   COMMAND wp_adler32   CD  cc0088)
add_test(NAME wp_adler32_7    COMMAND wp_adler32    7  380038)

add_test(NAME wp_crc32_0123 COMMAND wp_crc32 0123 a6669d7d)
add_test(NAME wp_crc32_IEND COMMAND wp_crc32 IEND ae426082)

add_test(NAME wp_set_header_ok COMMAND wp_set_header ok)
add_test(NAME wp_set_header_ng COMMAND wp_set_header ng)

add_test(NAME wp_set_ihdr_vga COMMAND wp_set_ihdr svga)

add_test(NAME wp_set_idat_line_rgba_raw COMMAND wp_silr2 fhd)

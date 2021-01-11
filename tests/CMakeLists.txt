include_directories (
  ${writepng_SOURCE_DIR}
)

add_executable(wp_adler32 wp_adler32.c)
target_link_libraries(wp_adler32 z)

add_executable(wp_crc32 wp_crc32.c)
target_link_libraries(wp_crc32 z)

add_executable(wp_set_header wp_set_header.c)
target_link_libraries(wp_set_header z)

add_executable(wp_set_ihdr wp_set_ihdr.c)
target_link_libraries(wp_set_ihdr z)

add_executable(wp_silr2 wp_silr2.c)
target_link_libraries(wp_silr2 z)

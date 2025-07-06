cmake_minimum_required(VERSION 3.7)
project(dos-app)

set (CMAKE_RANLIB ${CMAKE_FIND_ROOT_PATH}/djgpp/i586-pc-msdosdjgpp/bin/ranlib)
set (CMAKE_CXX_COMPILER_RANLIB ${CMAKE_FIND_ROOT_PATH}/djgpp/i586-pc-msdosdjgpp/bin/ranlib)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_compile_options(-Wall -std=c++23 -save-temps -U __STRICT_ANSI__ -Wno-array-bounds -fext-numeric-literals)

add_compile_options(
    "$<$<CONFIG:RELEASE>:-DRELEASE>"
    "$<$<CONFIG:DEBUG>:-DDEBUG>"
)

add_compile_options(
    "$<$<CONFIG:RELEASE>:-O3>"
)

#dxe's will need an export list
add_link_options(
    "$<$<CONFIG:RELEASE>:-s>"
)

#How do i make this blasted thing do this conditionally???
#add_compile_options(-D MOCK)

set (ENV{DJDIR} "${CMAKE_FIND_ROOT_PATH}/djgpp/")
set (ENV{DXE_LD_LIBRARY_PATH} "${CMAKE_FIND_ROOT_PATH}/djgpp/lib/gcc/i586-pc-msdosdjgpp/12.1.0/")
message("Environment variable DJDIR: $ENV{DJDIR}")
message("Environment variable DXE_LD_LIBRARY_PATH: $ENV{DXE_LD_LIBRARY_PATH}")

include_directories(include)

add_library(hag
            src/color.cpp
            src/math/fp/fp.cpp
            src/math/fp/fptables.cpp
            src/math/fp/m44.cpp
            src/math/flt/flt.cpp
            src/math/flt/m44.cpp
            src/system/bda.cpp
            src/system/pci.cpp
            src/system/pit.cpp
            src/testing/mock.cpp
            src/testing/testpat.cpp
            src/testing/log.cpp
           )

add_library(hagsup
            support/allocatr.cpp
            support/dump.cpp
           )

add_library(vga
            src/drivers/vga/modeset.cpp
            src/drivers/vga/data.cpp
            src/drivers/vga/extmsdef.cpp
           )

add_library(s3t64
            src/drivers/s3/shared/data.cpp
            src/drivers/s3/trio64/data.cpp
            src/drivers/s3/trio64/funcs.cpp
           )

add_library(mystique
            src/drivers/vga/modeset.cpp
            src/drivers/vga/data.cpp
            src/drivers/matrox/shared/modes.cpp
            src/drivers/matrox/shared/modeext.cpp
            src/drivers/matrox/shared/system.cpp
           )

add_library(banshee
            src/drivers/vga/modeset.cpp
            src/drivers/vga/data.cpp
            src/drivers/3dfx/shared/modes.cpp
            src/drivers/3dfx/shared/modeext.cpp
            src/drivers/3dfx/shared/system.cpp
            src/drivers/3dfx/shared/fifodefr.cpp
           )

#add_custom_target(mystiquedxe 
#                  COMMAND ${CMAKE_FIND_ROOT_PATH}/djgpp/i586-pc-msdosdjgpp/bin/dxe3gen
#                  -o mystique.dxe
#                  -D "Matrox Mystique"
#                  libmystique.a
#                 )
#add_dependencies(mystiquedxe mystique)

add_executable(myst
               tests/myst.cpp
              )

#libraries are linked in reverse order
target_link_libraries(myst
                      mystique
                      hagsup
                      hag
                     )
#add_dependencies(myst mystiquedxe)

#add_custom_target(assets
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/assets
#    ${CMAKE_BINARY_DIR}/assets
#)
#add_dependencies(main assets)

add_executable(mystst
               tests/mystst.cpp
               tests/mode.cpp
              )

#libraries are linked in reverse order
target_link_libraries(mystst
                      hagsup
                      mystique
                      hag
                     )

add_executable(bantst
               tests/bantst.cpp
              )

#libraries are linked in reverse order
target_link_libraries(bantst
                      banshee
                      hagsup
                      hag
                     )

add_executable(bansh
               tests/bansh.cpp
              )

#libraries are linked in reverse order
target_link_libraries(bansh
                      banshee
                      hagsup
                      hag
                     )

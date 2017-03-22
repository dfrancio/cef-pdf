# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# cef-pdf sources.
file(GLOB_RECURSE CEFPDF_SRCS "*.cpp" "*.h")
set(CEFPDF_EXE_SRCS "main.cpp")
set(CEFPDF_HELPER_SRCS "process_helper_mac.cc")
set(CEFPDF_PUBLIC_HEADERS "cefpdf.h")
foreach(src ${CEFPDF_EXE_SRCS})
    list(REMOVE_ITEM CEFPDF_SRCS ${src})
endforeach()
foreach(src ${CEFPDF_HELPER_SRCS})
    list(REMOVE_ITEM CEFPDF_SRCS ${src})
endforeach()
source_group(cef-pdf FILES ${CEFPDF_SRCS})

#
# Shared configuration.
#

# Target names.
set(CEF_TARGET "cefpdf")
set(CEF_EXE_TARGET "cef-pdf")
if(OS_MACOSX)
  set(CEF_HELPER_TARGET "${CEF_EXE_TARGET}_Helper")
  set(CEF_HELPER_TARGET_WS "${CEF_EXE_TARGET} Helper")
endif()

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
if(OS_MACOSX)
    set(CEF_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    set(CEF_TARGET_OUT_BUNDLE "${CEF_TARGET_OUT_DIR}/${CEF_EXE_TARGET}.app")
    set(CEF_TARGET_OUT_DIR "${CEF_TARGET_OUT_BUNDLE}/Contents")
    set(CEF_TARGET_FRAMEWORK_DIR "${CEF_TARGET_OUT_DIR}/Frameworks")
    set(CEF_TARGET_RESOURCE_DIR "${CEF_TARGET_OUT_DIR}/Resources")
    set(CEF_TARGET_BINARY_DIR "${CEF_TARGET_OUT_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
else()
    SET_CEF_TARGET_OUT_DIR()
    set(CEF_TARGET_RESOURCE_DIR ${CEF_TARGET_OUT_DIR})
    set(CEF_TARGET_BINARY_DIR ${CEF_TARGET_OUT_DIR})
endif()
set(CEF_TARGET_INC_DIR "${CEF_TARGET_OUT_DIR}/include/cefpdf")

# Library target.
add_library(${CEF_TARGET} ${CEF_PDF_LIBTYPE} ${CEFPDF_SRCS})
SET_LIBRARY_TARGET_PROPERTIES(${CEF_TARGET})
add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
target_link_libraries(${CEF_TARGET} ${Boost_LIBRARIES} libcef_dll_wrapper libcef_lib ${CEF_STANDARD_LIBS})
if(OS_WINDOWS)
    target_link_libraries(${CEF_TARGET} ws2_32 wsock32)
endif()
target_compile_options(${CEF_TARGET} PUBLIC -fexceptions)
if(OS_MACOSX)
    set_target_properties(${CEF_TARGET} PROPERTIES LINK_FLAGS -fno-pie)
endif()

# Export header.
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${CEF_TARGET} EXPORT_FILE_NAME "${CEF_TARGET_INC_DIR}/export.h")
target_include_directories(${CEF_TARGET} PUBLIC ${CEF_TARGET_INC_DIR})

# Logical target used to link the cef_sandbox library.
if(USE_SANDBOX)
  ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
  target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
endif()

# Helper executable target.
if(OS_MACOSX)
    add_executable(${CEF_HELPER_TARGET} ${CEFPDF_HELPER_SRCS})
    SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_HELPER_TARGET})
    target_link_libraries(${CEF_HELPER_TARGET} ${CEF_TARGET})
    set_target_properties(${CEF_HELPER_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET}.app/Contents/MacOS")
    add_custom_command(
        TARGET ${CEF_HELPER_TARGET}
        POST_BUILD
        # Use whitespace on Helper target.
        COMMAND ${CMAKE_COMMAND} -E remove_directory
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET_WS}.app"
        COMMAND ${CMAKE_COMMAND} -E rename
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET}.app/Contents/MacOS/${CEF_HELPER_TARGET}"
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET}.app/Contents/MacOS/${CEF_HELPER_TARGET_WS}"
        COMMAND ${CMAKE_COMMAND} -E rename
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET}.app"
            "${CEF_TARGET_FRAMEWORK_DIR}/${CEF_HELPER_TARGET_WS}.app"
        VERBATIM
    )
endif()

# Executable target.
add_executable(${CEF_EXE_TARGET} ${CEFPDF_EXE_SRCS})
SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_EXE_TARGET})
target_link_libraries(${CEF_EXE_TARGET} ${CEF_TARGET})

# Add the custom manifest files to the executable.
if(OS_WINDOWS AND NOT MINGW)
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")
endif()

# Copy binary and resource files to the target output directory.
COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_BINARY_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_RESOURCE_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEFPDF_PUBLIC_HEADERS}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET_INC_DIR}")
if(OS_MACOSX)
    add_custom_command(
        TARGET ${CEF_TARGET}
        POST_BUILD
        # Copy the CEF framework into the Frameworks directory.
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${CEF_TARGET_FRAMEWORK_DIR}/Chromium Embedded Framework.framework"
        VERBATIM
    )
endif()

# Set SUID permissions on the chrome-sandbox target.
if(OS_LINUX)
  SET_LINUX_SUID_PERMISSIONS("${CEF_EXE_TARGET}" "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
endif()

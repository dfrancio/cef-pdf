# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# cef-pdf sources.
file(GLOB_RECURSE CEFPDF_SRCS "*.cpp" "*.h")
set(CEFPDF_EXE_SRCS "main.cpp")
foreach(src ${CEFPDF_EXE_SRCS})
    list(REMOVE_ITEM CEFPDF_SRCS ${src})
endforeach()
source_group(cef-pdf FILES ${CEFPDF_SRCS})

#
# Shared configuration.
#

# Target names.
set(CEF_TARGET "cefpdf")
if(OS_MACOSX)
  set(CEF_HELPER_TARGET "cef-pdf_Helper")
endif()
set(CEF_EXE_TARGET "cef-pdf")

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()

# Library target.
add_library(${CEF_TARGET} ${CEF_PDF_LIBTYPE} ${CEFPDF_SRCS})
if(OS_MACOS)
    list(REMOVE_ITEM CEF_LINKER_FLAGS -Wl,-pie)
endif()
SET_LIBRARY_TARGET_PROPERTIES(${CEF_TARGET})
if(OS_MACOS)
    list(APPEND CEF_LINKER_FLAGS -Wl,-pie)
endif()
add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
target_link_libraries(${CEF_TARGET} ${Boost_LIBRARIES} libcef_dll_wrapper libcef_lib ${CEF_STANDARD_LIBS})
if(OS_WINDOWS)
    target_link_libraries(${CEF_TARGET} ws2_32 wsock32)
endif()
target_compile_options(${CEF_TARGET} PUBLIC -fexceptions)

# Logical target used to link the cef_sandbox library.
if(USE_SANDBOX)
  ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
  target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
endif()

# Executable target.
add_executable(${CEF_EXE_TARGET} ${CEFPDF_EXE_SRCS})
SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_EXE_TARGET})
target_link_libraries(${CEF_EXE_TARGET} ${CEF_TARGET})

# Set rpath so that libraries can be placed next to the executable.
#if(OS_LINUX)
#    set_target_properties(${CEF_EXE_TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
#    set_target_properties(${CEF_EXE_TARGET} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
#    set_target_properties(${CEF_EXE_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CEF_TARGET_OUT_DIR})
#endif()

# Add the custom manifest files to the executable.
if(OS_WINDOWS AND NOT MINGW)
  ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")
endif()

# Copy binary and resource files to the target output directory.
COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
COPY_FILES("${CEF_EXE_TARGET}" "" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")

# Set SUID permissions on the chrome-sandbox target.
if(OS_LINUX)
  SET_LINUX_SUID_PERMISSIONS("${CEF_EXE_TARGET}" "${CEF_TARGET_OUT_DIR}/chrome-sandbox")
endif()

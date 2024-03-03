cmake_minimum_required (VERSION 3.11)

project(pxtone)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

if (UNIX AND NOT WIN32 AND NOT APPLE)
	if (CMAKE_SIZEOF_VOID_P MATCHES "8")
		set (LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
		mark_as_advanced (LIB_POSTFIX)
	endif ()
endif ()
if (NOT DEFINED LIB_POSTFIX)
	set (LIB_POSTFIX "")
endif ()

set (TARGET_NAME pxtone)

set (HEADER_PATH "${CMAKE_CURRENT_LIST_DIR}/include")
set (SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/src")

set (LINK_LIBRARIES)

# Headers
set (TARGET_HEADERS
	${HEADER_PATH}/pxtn.h
	${HEADER_PATH}/pxtnDelay.h
	${HEADER_PATH}/pxtnDescriptor.h
	${HEADER_PATH}/pxtnError.h
	${HEADER_PATH}/pxtnEvelist.h
	${HEADER_PATH}/pxtnMaster.h
	${HEADER_PATH}/pxtnMax.h
	${HEADER_PATH}/pxtnMem.h
	${HEADER_PATH}/pxtnOverdrive.h
	${HEADER_PATH}/pxtnPulse_Frequency.h
	${HEADER_PATH}/pxtnPulse_Noise.h
	${HEADER_PATH}/pxtnPulse_NoiseBuilder.h
	${HEADER_PATH}/pxtnPulse_Oggv.h
	${HEADER_PATH}/pxtnPulse_Oscillator.h
	${HEADER_PATH}/pxtnPulse_PCM.h
	${HEADER_PATH}/pxtnService.h
	${HEADER_PATH}/pxtnText.h
	${HEADER_PATH}/pxtnUnit.h
	${HEADER_PATH}/pxtnWoice.h
	${HEADER_PATH}/pxtoneNoise.h
)


# Sources
set (TARGET_SOURCES
	${SOURCE_PATH}/pxtnDelay.cpp
	${SOURCE_PATH}/pxtnDescriptor.cpp
	${SOURCE_PATH}/pxtnError.cpp
	${SOURCE_PATH}/pxtnEvelist.cpp
	${SOURCE_PATH}/pxtnMaster.cpp
	${SOURCE_PATH}/pxtnMem.cpp
	${SOURCE_PATH}/pxtnOverdrive.cpp
	${SOURCE_PATH}/pxtnPulse_Frequency.cpp
	${SOURCE_PATH}/pxtnPulse_Noise.cpp
	${SOURCE_PATH}/pxtnPulse_NoiseBuilder.cpp
	${SOURCE_PATH}/pxtnPulse_Oggv.cpp
	${SOURCE_PATH}/pxtnPulse_Oscillator.cpp
	${SOURCE_PATH}/pxtnPulse_PCM.cpp
	${SOURCE_PATH}/pxtnService.cpp
	${SOURCE_PATH}/pxtnService_moo.cpp
	${SOURCE_PATH}/pxtnText.cpp
	${SOURCE_PATH}/pxtnUnit.cpp
	${SOURCE_PATH}/pxtnWoice.cpp
	${SOURCE_PATH}/pxtnWoice_io.cpp
	${SOURCE_PATH}/pxtnWoicePTV.cpp
	${SOURCE_PATH}/pxtoneNoise.cpp
)

source_group("Includes"	FILES ${TARGET_HEADERS})
source_group("PxTone"	FILES ${TARGET_SOURCES})

add_library(${TARGET_NAME} STATIC ${TARGET_SOURCES} ${TARGET_HEADERS})

target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})

# Set global cache var
set(PXTONE_HEADER_DIR "${HEADER_PATH}" CACHE INTERNAL "PXTONE_HEADER_DIR")
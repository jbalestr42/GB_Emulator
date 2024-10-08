cmake_minimum_required(VERSION 3.8)

project(gb_emu)

set(HEADER_FILES
	include/${PROJECT_NAME}/BitUtils.hpp
	include/${PROJECT_NAME}/BootRom.hpp
	include/${PROJECT_NAME}/Cartridge.hpp
	include/${PROJECT_NAME}/CPU.hpp
	include/${PROJECT_NAME}/Emulator.hpp
	include/${PROJECT_NAME}/HardwareRegisters.hpp
	include/${PROJECT_NAME}/IDisplay.hpp
	include/${PROJECT_NAME}/IMemoryRange.hpp
	include/${PROJECT_NAME}/Input.hpp
	include/${PROJECT_NAME}/Interrupts.hpp
	include/${PROJECT_NAME}/MBC1.hpp
	include/${PROJECT_NAME}/MBC2.hpp
	include/${PROJECT_NAME}/MBC3.hpp
	include/${PROJECT_NAME}/MBC5.hpp
	include/${PROJECT_NAME}/MBCRomOnly.hpp
	include/${PROJECT_NAME}/MemoryRange.hpp
	include/${PROJECT_NAME}/MMU.hpp
	include/${PROJECT_NAME}/OamSearchMode.hpp
	include/${PROJECT_NAME}/PPU.hpp
	include/${PROJECT_NAME}/Registers.hpp
	include/${PROJECT_NAME}/SFMLDisplay.hpp
	include/${PROJECT_NAME}/Timer.hpp
)

set(SOURCE_FILES
	source/main.cpp
	source/BitUtils.cpp
	source/Cartridge.cpp
	source/CPU.cpp
	source/Emulator.cpp
	source/Input.cpp
	source/Interrupts.cpp
	source/MBC1.cpp
	source/MBC2.cpp
	source/MBC3.cpp
	source/MBC5.cpp
	source/MBCRomOnly.cpp
	source/MemoryRange.cpp
	source/MMU.cpp
	source/OamSearchMode.cpp
	source/OpCodes.cpp
	source/PPU.cpp
	source/Registers.cpp
	source/SFMLDisplay.cpp
	source/Timer.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source Files" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	sfml-graphics
	sfml-window
	sfml-system
)

target_include_directories(${PROJECT_NAME} PRIVATE
	include/${PROJECT_NAME}
)

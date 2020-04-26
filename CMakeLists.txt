#/*
#   This file is part of TUM Open Infra Platform Early Binding EXPRESS 
#	Generator, a simple early binding generator for EXPRESS.
#	Copyright (c) 2016-2017 Technical University of Munich
#	Chair of Computational Modeling and Simulation.
#
#	TUM Open Infra Platform Early Binding EXPRESS Generator is free 
#	software; you can redistribute it and/or modify it under the terms 
#	of the GNU General Public License Version 3 as published by the Free
#	Software Foundation.
#
#   TUM Open Infra Platform Early Binding EXPRESS Generator is 
#	distributed in the hope that it will be useful, but WITHOUT ANY 
#	WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
#	for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.
#*/

cmake_minimum_required(VERSION 3.9.4)

project(oipExpress)

#------------------------------------------------------------------------------
# Create folders for different build configurations (if they do not exist)
#------------------------------------------------------------------------------

if(EXISTS ${CMAKE_BINARY_DIR}/Debug)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/Release)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/RelWithDebInfo)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/RelWithDebInfo)
endif()

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Some checks to ensure that the prerequisites are ok
#------------------------------------------------------------------------------

if(NOT MSVC14)
	message("Please switch to Visual Studio 2015.")
	return()
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8 )
	message("Please switch to x64 build.")
	return()
endif()

#------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake/)

if (WIN32)
	set(BISON_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/Tools/win_flex_bison-2.5.5/win_bison.exe)
	set(FLEX_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/Tools/win_flex_bison-2.5.5/win_flex.exe)
endif()

set(BOOST_ROOT "C:\\thirdparty\\vs2015\\x64\\boost_1_63_0")

find_package(Boost 1.65.1)
find_package(BISON)
find_package(FLEX)

bison_target(Parser ${CMAKE_CURRENT_SOURCE_DIR}/ExpressBinding/src/OpenInfraPlatform/ExpressBinding/Parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(Scanner ${CMAKE_CURRENT_SOURCE_DIR}/ExpressBinding/src/OpenInfraPlatform/ExpressBinding/Parser/tokens.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
add_flex_bison_dependency(Scanner Parser)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(ExpressBinding/src)

file(GLOB OpenInfraPlatform_ExpressBinding_Generator_Source	ExpressBinding/src/OpenInfraPlatform/ExpressBinding/Generator/*.*)	
file(GLOB OpenInfraPlatform_ExpressBinding_Meta_Source		ExpressBinding/src/OpenInfraPlatform/ExpressBinding/Meta/*.*)	
file(GLOB OpenInfraPlatform_ExpressBinding_Parser_Source	ExpressBinding/src/OpenInfraPlatform/ExpressBinding/Parser/*.*)	
file(GLOB OpenInfraPlatform_ExpressBinding_Source	        ExpressBinding/src/OpenInfraPlatform/ExpressBinding/*.*)	

source_group(OpenInfraPlatform\\ExpressBinding\\Generator	FILES ${OpenInfraPlatform_ExpressBinding_Generator_Source})
source_group(OpenInfraPlatform\\ExpressBinding\\Meta    	FILES ${OpenInfraPlatform_ExpressBinding_Meta_Source})	
source_group(OpenInfraPlatform\\ExpressBinding\\Parser		FILES ${OpenInfraPlatform_ExpressBinding_Parser_Source})	
source_group(OpenInfraPlatform\\ExpressBinding				FILES ${OpenInfraPlatform_ExpressBinding_Source})	

add_executable(oipExpress
	${OpenInfraPlatform_ExpressBinding_Generator_Source}
	${OpenInfraPlatform_ExpressBinding_Meta_Source}
	${OpenInfraPlatform_ExpressBinding_Parser_Source}
	${OpenInfraPlatform_ExpressBinding_Source}
	${FLEX_Scanner_OUTPUTS} 
	${BISON_Parser_OUTPUTS}
)

# housekeeping for resources
if(EXISTS ${CMAKE_BINARY_DIR}/Debug)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/Release)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
endif()

if(EXISTS ${CMAKE_BINARY_DIR}/RelWithDebInfo)
else()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/RelWithDebInfo)
endif()

add_custom_target(CopyOipExpressResources
	COMMAND	${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Template Template
	COMMAND	${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Template Debug/Template
	COMMAND	${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Template Release/Template
	COMMAND	${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Template RelWithDebInfo/Template
)

add_dependencies(oipExpress CopyOipExpressResources)
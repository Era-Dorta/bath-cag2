# g++ -I./include -D_REENTRANT -fPIC -pthread -ansi -O3 -s -shared -export-dynamic -static-libgcc -Wl,
#-Bsymbolic -o mayaShaderExample.so lightlink/maya_illum_lambert.cpp
#******************************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) 

PROJECT(t4_cloth_project)

SET(LIBRARY_OUTPUT_PATH /home/gdp24/maya/shaders/)

SET(CMAKE_CXX_COMPILER g++)

INCLUDE_DIRECTORIES( /usr/autodesk/mentalrayForMaya2015/devkit/include )
 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -fPIC -pthread -ansi -shared -export-dynamic -static-libgcc -Wl,-Bsymbolic")

ADD_LIBRARY(t4_cloth_mentalray
	SHARED
   src/maya_illum_lambert.cpp
)

SET_TARGET_PROPERTIES(t4_cloth_mentalray PROPERTIES PREFIX "")

cmake_minimum_required(VERSION 2.6)
project(t4_cloth)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
# can optionally specify a version: FIND_PACKAGE( Maya 2012 REQUIRED )
FIND_PACKAGE( Maya REQUIRED )

# the maya-specific plugin code (w/ initializePlugin, etc)
SET( ${PROJECT_NAME}_SRCS phongShader.cpp phongShaderOverride.cpp phongShaderOverride.h )

INCLUDE_DIRECTORIES( ${PUBLIC_INCLUDE_DIRS} ${${PROJECT_NAME}_proj_SOURCE_DIR}/src ${MAYA_INCLUDE_DIR} )

# create the plugin
ADD_LIBRARY( ${PROJECT_NAME} SHARED
  ${plugin_SRCS}
  ${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_MOC_SRCS}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
   ${MAYA_Foundation_LIBRARY} ${MAYA_OpenMaya_LIBRARY} ${MAYA_OpenMayaUI_LIBRARY}
   ${QT_LIBRARIES}
)

SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES 
  OUTPUT_NAME ${PROJECT_NAME}
  CLEAN_DIRECT_OUTPUT 1
)

# this is a macro defined in FindMaya that sets up our plugin with standard Maya build settings
MAYA_SET_PLUGIN_PROPERTIES( ${PROJECT_NAME} )

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /home/gdp24/maya/plug-ins/)
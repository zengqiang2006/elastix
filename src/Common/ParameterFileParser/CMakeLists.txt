# Sources of non-templated classes.
SET( param_SRCS
  itkParameterFileParser.h
  itkParameterFileParser.cxx
  itkParameterMapInterface.h
  itkParameterMapInterface.cxx
)

SOURCE_GROUP( "Parser" FILES ${param_SRCS} )

ADD_LIBRARY( param ${param_SRCS} )

SET_TARGET_PROPERTIES(param PROPERTIES ${ELASTIX_LIBRARY_PROPERTIES})

TARGET_LINK_LIBRARIES( param ITKCommon )

INSTALL( TARGETS param RUNTIME DESTINATION bin LIBRARY DESTINATION lib${LIB_SUFFIX} ARCHIVE DESTINATION lib${LIB_SUFFIX})

# Group in IDE's like Visual Studio
SET_PROPERTY( TARGET param PROPERTY FOLDER "libraries" )

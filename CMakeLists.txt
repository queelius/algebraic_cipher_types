# ./CMakeLists.txt
cmake_minimum_required( VERSION 3.15 )

include ( "${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in" )

set ( BOOST_VERSION 1.69.0 )

set ( Boost_USE_STATIC_LIBS         TRUE )
set ( Boost_USE_MULTITHREADED       TRUE )
set ( Boost_USE_STATIC_RUNTIME      FALSE )
set ( Boost_COMPILER                "-gcc8" )
set ( Boost_ADDITIONAL_VERSIONS     "${BOOST_VERSION}" )

project( BooleanAlgebra
         VERSION ${project_version}
         DESCRIPTION ${project_description}
         HOMEPAGE_URL ${project_url}
         LANGUAGES CXX )

find_package( Boost ${BOOST_VERSION} EXACT
    CONFIG
    REQUIRED COMPONENTS program_options )

if ( Boost_FOUND )
    set_target_properties( Boost::Boost
                           Boost::program_options
        PROPERTIES IMPORTED_GLOBAL TRUE )
endif {}

add_executable(AndProg)
add_executable(OrProg)
add_executable(NotProg)

# Add some sources to target.
target_sources( AndProg PRIVATE src/and.cpp )
target_sources( OrProg PRIVATE src/or.cpp )
target_sources( NotProg PRIVATE src/not.cpp )
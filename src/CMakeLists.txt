add_library(scalers SHARED)

target_sources(scalers PRIVATE scalers.cxx)

target_link_libraries(scalers PUBLIC aliphysics-lite-muondep)

add_executable(scalers2csv scalers2csv.cxx)
target_link_libraries(scalers2csv PUBLIC aliphysics-lite-muondep Boost::program_options fmt::fmt)

target_include_directories(scalers
    PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    INTERFACE
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

root_generate_dictionary(G__Scalers
  scalers.h
                        scalers-linkdef.h
  MODULE scalers)

set_property(TARGET scalers PROPERTY PUBLIC_HEADER 
  scalers.h
)
 
install(TARGETS scalers scalers2csv)

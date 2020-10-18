project(app)

include_directories(include)

add_executable(app 
                src/main.cpp 
                src/LedStripDataStore.cpp
                src/LedStripPrinter.cpp
                src/ImageProcessor.cpp)

target_link_libraries(app pthread)
 

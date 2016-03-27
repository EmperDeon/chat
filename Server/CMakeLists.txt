set(S_S sconnect.cpp smain.cpp sserver.cpp sutils.cpp swgt.cpp sclients.cpp)
set(S_H sconnect.h sdefines.h sserver.h sutils.h swgt.h sclients.h)

qt5_wrap_cpp(GENERATED_SOURCES ${S_H})

add_executable(Server ${S_S} ${S_H})
qt5_use_modules(Server Core Widgets Network)
target_compile_features(Server PRIVATE cxx_range_for)
target_link_libraries(Server ${Qt5Core_QTMAIN_LIBRARIES})
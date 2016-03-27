set(C_S cclient.cpp cmain.cpp cserver.cpp cutils.cpp cwgt.cpp)
set(C_H cclient.h cdefines.h cserver.h cutils.h cwgt.h)

qt5_wrap_cpp(GENERATED_SOURCES ${C_H})

add_executable(Client ${C_S} ${C_H})
qt5_use_modules(Client Core Widgets Network)
target_compile_features(Client PRIVATE cxx_range_for)
target_link_libraries(Client ${Qt5Core_QTMAIN_LIBRARIES})
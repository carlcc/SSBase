project(SSNet)

file(GLOB SSNet_SOURCE_FILES * impl/* thirdparty/*)

add_library(SSNet ${SSNet_SOURCE_FILES} TcpSocket.cpp TcpSocket.h impl/TcpSocketImpl.cpp impl/TcpSocketImpl.h SocketErrorCode.h)
target_include_directories(SSNet PRIVATE .. ../../thirdparty/include)
add_dependencies(SSNet SSBase SSIO)

file(GLOB THESE_SOURCES ${ARCH}/*.c)
file(GLOB THESE_HEADERS ${ARCH}/*.h)

set(ARCH_SOURCES ${THESE_SOURCES} PARENT_SCOPE)
set(ARCH_HEADERS ${THESE_HEADERS} PARENT_SCOPE)


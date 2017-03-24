cmake_minimum_required(VERSION 2.8)
 
project(watershed_2_wall)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

 INCLUDE_DIRECTORIES(
	"~/Documents/Code/irtk/src/image++/include/"
	"~/Documents/Code/irtk/src/common++/include/"
	"~/Documents/Code/irtk/src/contrib++/include/"
	"~/Documents/Code/irtk/src/geometry++/include/"
	"/usr/local/include/"
 )

 LINK_DIRECTORIES(
	"~/Documents/Code/irtk/bin/lib/"
	"/usr/local/lib/"
	"~/Documents/Code/third_party/zlib/src/"
 )

add_executable(watershed2wall Watershed2wall.cxx)
add_executable(Combinmask CombineMask.cxx)

target_link_libraries(watershed2wall ${VTK_LIBRARIES} contrib++ image++ geometry++ common++ znz gsl niftiio z)
target_link_libraries(Combinmask ${VTK_LIBRARIES} contrib++ image++ geometry++ common++ znz gsl niftiio z)


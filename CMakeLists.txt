cmake_minimum_required(VERSION 2.8)
 
project(watershed_2_wall)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

 INCLUDE_DIRECTORIES(
	"~/Documents/Code/third_party/irtk/source/image++/include/"
	"~/Documents/Code/third_party/irtk/source/common++/include/"
	"~/Documents/Code/third_party/irtk/source/contrib++/include/"
	"~/Documents/Code/third_party/irtk/source/geometry++/include/"
	"/usr/local/include/"
 )

 LINK_DIRECTORIES(
	"~/Documents/Code/third_party/irtk/binary/lib/"
	"/usr/local/lib/"
	"~/Documents/Code/third_party/zlib/src/"
 )

add_executable(watershed2wall Watershed2wall.cxx)
add_executable(Combinmask CombineMask.cxx)

target_link_libraries(watershed2wall ${VTK_LIBRARIES} contrib++ image++ geometry++ common++ znz gsl niftiio z)
target_link_libraries(Combinmask ${VTK_LIBRARIES} contrib++ image++ geometry++ common++ znz gsl niftiio z)


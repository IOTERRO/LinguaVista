file(GLOB  LIB_H
    ${CMAKE_CURRENT_LIST_DIR}/*.h
)

file(GLOB  LIB_CPP
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)


set(H_FILES ${LIB_H})

set(CPP_FILES ${LIB_CPP})

set(EXTERNAL_LIBS ${CMAKE_CURRENT_LIST_DIR}/../../External)
set(OPENCV_LIB ${EXTERNAL_LIBS}/OpenCV/opencv/OpenCvBuild/bin/Debug)
set(OPENCV_BUILD ${EXTERNAL_LIBS}/OpenCV/opencv/build)

set(OpenCV_DIR "D:/Learning/c++_leaning/git/LinguaVista/External/OpenCV/opencv/OpenCvBuild")
find_package(OpenCV REQUIRED)

message("OpenCV_INCLUDE_DIRS: + ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBS: + ${OpenCV_LIBS}")

add_module(mediaProcessing
      MODULE_TYPE
         dll 
      SOURCE_H_FILES
         ${H_FILES}
      SOURCE_CPP_FILES
         ${CPP_FILES}
      VS_FOLDER
         
      PRIVATE_INCLUDES
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/calib3d/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/core/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/dnn/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/features2d/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/flann/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/gapi/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/highgui/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/imgcodecs/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/imgproc/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/ml/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/objdetect/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/photo/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/stitching/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/ts/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/video/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/videoio/include
		# ${EXTERNAL_LIBS}/opencv-4.9.0/modules/world/include
      DEPENDENCIES
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_calib3d490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_core490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_dnn490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_features2d490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_flann490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_gapi490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_highgui490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_imgcodecs490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_imgproc490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_ml490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_objdetect490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_photo490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_stitching490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_video490$<$<CONFIG:debug>:d>.lib
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/lib/opencv_videoio490$<$<CONFIG:debug>:d>.lib
	  POSTBUILD_COPY
        ${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_calib3d490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_core490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_dnn490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_features2d490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_flann490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_gapi490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_highgui490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_imgcodecs490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_imgproc490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_ml490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_objdetect490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_photo490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_stitching490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_video490$<$<CONFIG:debug>:d>.dll
		${EXTERNAL_LIBS}/opencv-4.9.0/binairies/install/x86/vc16/bin/opencv_videoio490$<$<CONFIG:debug>:d>.dll
	  IMPORT_SUFFIX
		
	  MODULE_HELP
		FALSE
)
cmake_minimum_required(VERSION 2.8)

#---------------------------------------#
#         The zPure Data Project        #
#---------------------------------------#
project(zpd)
option(COVERALLS "Build with coveralls" OFF)
#set(CMAKE_BUILD_TYPE Release)

#---------------------------------------#
#         The zpd Sources        #
#---------------------------------------#

set(CPDSOURCES
${PROJECT_SOURCE_DIR}/cpd/cpd.c
${PROJECT_SOURCE_DIR}/cpd/cpd.h
)

#---------------------------------------#
#         The xpd Sources        #
#---------------------------------------#

set(XPDSOURCES
	${PROJECT_SOURCE_DIR}/xpd/xpd_def.hpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_mutex.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_mutex.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_tie.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_tie.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_symbol.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_symbol.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_atom.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_atom.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_midi.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_midi.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_console.hpp
  ${PROJECT_SOURCE_DIR}/xpd/xpd_console.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_environment.hpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_environment.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_instance.hpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_instance.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_patch.hpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_patch.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_object.hpp
	${PROJECT_SOURCE_DIR}/xpd/xpd_object.cpp
	${PROJECT_SOURCE_DIR}/xpd/xpd.hpp
)

#---------------------------------------#
#        The Pure Data Sources          #
#---------------------------------------#
set(PDSOURCES
  ${PROJECT_SOURCE_DIR}/pd/src/d_arithmetic.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_array.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_ctl.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_dac.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_delay.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_fft.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_fft_fftsg.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_filter.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_global.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_math.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_misc.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_osc.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_resample.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_soundfile.c
  ${PROJECT_SOURCE_DIR}/pd/src/d_ugen.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_all_guis.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_all_guis.h
  ${PROJECT_SOURCE_DIR}/pd/src/g_array.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_bang.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_canvas.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_canvas.h
  ${PROJECT_SOURCE_DIR}/pd/src/g_clone.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_editor.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_graph.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_guiconnect.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_hdial.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_hslider.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_io.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_mycanvas.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_numbox.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_readwrite.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_rtext.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_scalar.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_template.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_text.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_toggle.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_traversal.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_vdial.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_vslider.c
  ${PROJECT_SOURCE_DIR}/pd/src/g_vumeter.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_atom.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_binbuf.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_class.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_conf.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_glob.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_imp.h
  ${PROJECT_SOURCE_DIR}/pd/src/m_memory.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_obj.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_pd.c
  ${PROJECT_SOURCE_DIR}/pd/src/m_pd.h
  ${PROJECT_SOURCE_DIR}/pd/src/m_sched.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_audio.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_audio_dummy.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_file.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_inter.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_loader.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_main.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_path.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_print.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_stuff.h
  ${PROJECT_SOURCE_DIR}/pd/src/s_utf8.c
  ${PROJECT_SOURCE_DIR}/pd/src/s_utf8.h
  ${PROJECT_SOURCE_DIR}/pd/src/x_acoustics.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_arithmetic.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_array.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_connective.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_gui.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_interface.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_list.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_midi.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_misc.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_net.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_scalar.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_text.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_time.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_vexp.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_vexp.h
  ${PROJECT_SOURCE_DIR}/pd/src/x_vexp_fun.c
  ${PROJECT_SOURCE_DIR}/pd/src/x_vexp_if.c
)

#---------------------------------------#
#        The Pure Data Extra Sources    #
#---------------------------------------#
set(PDEXTRASOURCES
${PROJECT_SOURCE_DIR}/pd/extra/bob~/bob~.c
${PROJECT_SOURCE_DIR}/pd/extra/bonk~/bonk~.c
${PROJECT_SOURCE_DIR}/pd/extra/choice/choice.c
${PROJECT_SOURCE_DIR}/pd/extra/fiddle~/fiddle~.c
${PROJECT_SOURCE_DIR}/pd/extra/loop~/loop~.c
${PROJECT_SOURCE_DIR}/pd/extra/lrshift~/lrshift~.c
${PROJECT_SOURCE_DIR}/pd/extra/pique/pique.c
${PROJECT_SOURCE_DIR}/pd/extra/sigmund~/sigmund~.c
${PROJECT_SOURCE_DIR}/pd/extra/stdout/stdout.c
)

source_group(cpd FILES ${CPDSOURCES})
source_group(xpd FILES ${XPDSOURCES})
source_group(pd FILES ${PDSOURCES})
source_group(pdextra FILES ${PDEXTRASOURCES})
include_directories(${PROJECT_SOURCE_DIR}/zpd)
include_directories(${PROJECT_SOURCE_DIR}/cpd)
include_directories(${PROJECT_SOURCE_DIR}/pd/src/)

#---------------------------------------#
#         The zPure Data Library        #
#---------------------------------------#

if(${COVERALLS})
    message(STATUS "Build with coveralls")
    ENABLE_TESTING()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

    find_library(MATH_LIB m)
    find_package(Threads REQUIRED)
    add_definitions(-DPD=1 -DUSEAPI_DUMMY=1 -DPD_INTERNAL=1 -DHAVE_UNISTD_H=1 -DHAVE_ALLOCA_H=1 -DHAVE_LIBDL=1 -DZPD_TEST_VERBOSE=1)
    add_executable(zpdtest test/test.c ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
    target_link_libraries(zpdtest ${MATH_LIB})
    target_link_libraries(zpdtest ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(zpdtest ${CMAKE_DL_LIBS})
else()
    message(STATUS "Build without coveralls")
    if(${APPLE})
      add_definitions(-DPD=1 -DUSEAPI_DUMMY=1 -DPD_INTERNAL=1 -DHAVE_UNISTD_H=1 -DHAVE_ALLOCA_H=1 -DHAVE_LIBDL=1)
      set(CMAKE_MACOSX_RPATH On)
      set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
      #---------------------------------------#
      #             Shared Library            #
      #---------------------------------------#
      add_library(zpdshared SHARED ${PDSOURCES} ${PDEXTRASOURCES} ${CPDSOURCES})
      set_target_properties(zpdshared PROPERTIES OUTPUT_NAME zpd)
      set_target_properties(zpdshared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

      #---------------------------------------#
      #             Static Library            #
      #---------------------------------------#
      add_library(zpdstatic STATIC ${PDSOURCES} ${PDEXTRASOURCES} ${CPDSOURCES})
      set_target_properties(zpdstatic PROPERTIES OUTPUT_NAME zpd)
      set_target_properties(zpdstatic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

      #---------------------------------------#
      #                  Test                 #
      #---------------------------------------#
      add_executable(xpdtest ${PROJECT_SOURCE_DIR}/test/test.cpp ${XPDSOURCES} ${CPDSOURCES} ${PDSOURCES} ${PDEXTRASOURCES})
    elseif(${UNIX})
      add_definitions(-DPD=1 -DUSEAPI_DUMMY=1 -DPD_INTERNAL=1 -DHAVE_UNISTD_H=1 -DHAVE_ALLOCA_H=1 -DHAVE_LIBDL=1)
      find_library(MATH_LIB m)
      find_package(Threads REQUIRED)

      #---------------------------------------#
      #             Shared Library            #
      #---------------------------------------#
      add_library(zpdshared SHARED ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
      target_link_libraries(zpdshared ${MATH_LIB})
      target_link_libraries(zpdshared ${CMAKE_THREAD_LIBS_INIT})
      target_link_libraries(zpdshared ${CMAKE_DL_LIBS})
      set_target_properties(zpdshared PROPERTIES OUTPUT_NAME zpd)
      set_target_properties(zpdshared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

      #---------------------------------------#
      #             Static Library            #
      #---------------------------------------#
      add_library(zpdstatic STATIC ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
      target_link_libraries(zpdstatic ${MATH_LIB})
      target_link_libraries(zpdstatic ${CMAKE_THREAD_LIBS_INIT})
      target_link_libraries(zpdstatic ${CMAKE_DL_LIBS})
      set_target_properties(zpdstatic PROPERTIES OUTPUT_NAME zpd)
      set_target_properties(zpdstatic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

      #---------------------------------------#
      #                  Test                 #
      #---------------------------------------#
      add_executable(zpdtest test/test.c ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
      target_link_libraries(zpdtest ${MATH_LIB})
      target_link_libraries(zpdtest ${CMAKE_THREAD_LIBS_INIT})
      target_link_libraries(zpdtest ${CMAKE_DL_LIBS})
    elseif(${WIN32})
      add_definitions(-DPD=1 -DUSEAPI_DUMMY=1 -DPD_INTERNAL=1 -DHAVE_STRUCT_TIMESPEC=1 -DPTW32_STATIC_LIB=1)
      include_directories(${PROJECT_SOURCE_DIR}/pthread/include)
      #add_library(zpdshared SHARED ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
      add_library(zpdstatic STATIC ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
      if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
      link_directories(${PROJECT_SOURCE_DIR}/pthread/lib/x64)
      else(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
      link_directories(${PROJECT_SOURCE_DIR}/pthread/lib/x86)
      endif()
      add_definitions( "/D_CRT_SECURE_NO_WARNINGS /wd4091 /wd4996" )
      #target_link_libraries(zpdshared pthread_li‡b)
      target_link_libraries(zpdstatic pthread_lib)
      #set_target_properties(zpdshared PROPERTIES OUTPUT_NAME zpd)
      set_target_properties(zpdstatic PROPERTIES OUTPUT_NAME zpdlib)
    endif()
endif()

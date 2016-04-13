cmake_minimum_required(VERSION 2.6)

#---------------------------------------#
#         The zPure Data Project        #
#---------------------------------------#
project(zpdlib)
set(CMAKE_BUILD_TYPE Release)

if(${APPLE})
  message("Apple Version")
elseif(${UNIX})
  message("Unix Version")
elseif(${WIN32})
  if(${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
    message("Windows 64-bit Version")
  else(${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
    message("Windows 32-bit Version")
  endif()
endif()

#---------------------------------------#
#         The Preprocessor Defs         #
#---------------------------------------#
if(${APPLE})
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
  add_definitions(
    -DPD=1
    -DUSEAPI_DUMMY=1
    -DPD_INTERNAL=1
    -DHAVE_UNISTD_H=1
    -DHAVE_ALLOCA_H=1
    -DHAVE_LIBDL=1
  )
  set(CMAKE_MACOSX_RPATH On)
elseif(${UNIX})
  add_definitions(
    -DPD=1
    -DUSEAPI_DUMMY=1
    -DPD_INTERNAL=1
    -DHAVE_UNISTD_H=1
    -DHAVE_ALLOCA_H=1
    -DHAVE_LIBDL=1
  )
elseif(${WIN32})
  add_definitions(
    -DPD=1
    -DUSEAPI_DUMMY=1
    -DPD_INTERNAL=1
    -DHAVE_STRUCT_TIMESPEC=1
    -DPTW32_STATIC_LIB=1
  )
endif()

#---------------------------------------#
#         The zPure Data Sources        #
#---------------------------------------#

set(ZPDSOURCES
${PROJECT_SOURCE_DIR}/zpd/z_pd.c
${PROJECT_SOURCE_DIR}/zpd/z_pd.h
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

#---------------------------------------#
#         The zPure Data Library        #
#---------------------------------------#

set(LIBRARY_OUTPUT_PATH ../../lib/${CMAKE_BUILD_TYPE})
include_directories(${PROJECT_SOURCE_DIR}/zpd)
include_directories(${PROJECT_SOURCE_DIR}/pd/src/)
if(${WIN32})
  include_directories(${PROJECT_SOURCE_DIR}/pthread/lib/x86)
  add_library(zpdshared SHARED ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
  add_library(zpdstatic STATIC ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
  target_link_libraries(zpdshared pthread_lib)
  target_link_libraries(zpdstatic pthread_lib)
  if(${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
  link_directories(${PROJECT_SOURCE_DIR}/pthread/lib/x64)
  else(${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
  link_directories(${PROJECT_SOURCE_DIR}/pthread/lib/x86)
  endif()
  set_target_properties(zpdshared PROPERTIES OUTPUT_NAME zpd)
  set_target_properties(zpdstatic PROPERTIES OUTPUT_NAME zpdlib)
else()
  add_library(zpdshared SHARED ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
  add_library(zpdstatic STATIC ${PDSOURCES} ${PDEXTRASOURCES} ${ZPDSOURCES})
  set_target_properties(zpdshared PROPERTIES OUTPUT_NAME zpd)
  set_target_properties(zpdstatic PROPERTIES OUTPUT_NAME zpd)
endif()

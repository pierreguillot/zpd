# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "PD=1" -D "USEAPI_DUMMY=1" -D "PD_INTERNAL=1" -D "JUCER_LINUX_MAKE_2085FF7D=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 

  TARGET := libzpd.a
  BLDCMD = ar -rcs $(OUTDIR)/$(TARGET) $(OBJECTS)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "PD=1" -D "USEAPI_DUMMY=1" -D "PD_INTERNAL=1" -D "JUCER_LINUX_MAKE_2085FF7D=1" -D "JUCE_APP_VERSION=0.0.1" -D "JUCE_APP_VERSION_HEX=0x1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ 

  TARGET := libzpd.a
  BLDCMD = ar -rcs $(OUTDIR)/$(TARGET) $(OBJECTS)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/bob~_c63b734a.o \
  $(OBJDIR)/bonk~_736bc114.o \
  $(OBJDIR)/choice_86f76fca.o \
  $(OBJDIR)/fiddle~_4cccdb70.o \
  $(OBJDIR)/loop~_2fec9860.o \
  $(OBJDIR)/lrshift~_6f0fe66a.o \
  $(OBJDIR)/pique_f9d2867c.o \
  $(OBJDIR)/sigmund~_d8f0490a.o \
  $(OBJDIR)/stdout_58738c8a.o \
  $(OBJDIR)/d_arithmetic_ad7e4e00.o \
  $(OBJDIR)/d_array_98e54b8d.o \
  $(OBJDIR)/d_ctl_fc46c68f.o \
  $(OBJDIR)/d_dac_fc4c193a.o \
  $(OBJDIR)/d_delay_21147037.o \
  $(OBJDIR)/d_fft_fc6acde8.o \
  $(OBJDIR)/d_fft_fftsg_875cd71.o \
  $(OBJDIR)/d_filter_a5d87dce.o \
  $(OBJDIR)/d_global_b08b5739.o \
  $(OBJDIR)/d_math_9c9b399e.o \
  $(OBJDIR)/d_misc_9d0b6e82.o \
  $(OBJDIR)/d_osc_fcef4a73.o \
  $(OBJDIR)/d_resample_abc87a73.o \
  $(OBJDIR)/d_soundfile_6eb33c3f.o \
  $(OBJDIR)/d_ugen_aa8fd091.o \
  $(OBJDIR)/g_all_guis_4cef7929.o \
  $(OBJDIR)/g_array_55b29890.o \
  $(OBJDIR)/g_bang_c176b36b.o \
  $(OBJDIR)/g_canvas_a763830b.o \
  $(OBJDIR)/g_clone_b51d5294.o \
  $(OBJDIR)/g_editor_d3a7de0.o \
  $(OBJDIR)/g_graph_922efe45.o \
  $(OBJDIR)/g_guiconnect_81f87c42.o \
  $(OBJDIR)/g_hdial_af9b5a4f.o \
  $(OBJDIR)/g_hslider_f8619d00.o \
  $(OBJDIR)/g_io_4712ee59.o \
  $(OBJDIR)/g_mycanvas_16b8ad37.o \
  $(OBJDIR)/g_numbox_3d44d2f8.o \
  $(OBJDIR)/g_readwrite_77e8f540.o \
  $(OBJDIR)/g_rtext_dbb96956.o \
  $(OBJDIR)/g_scalar_7886d89f.o \
  $(OBJDIR)/g_template_5322222d.o \
  $(OBJDIR)/g_text_e06b0920.o \
  $(OBJDIR)/g_toggle_652e3a07.o \
  $(OBJDIR)/g_traversal_9f9678b5.o \
  $(OBJDIR)/g_vdial_943297dd.o \
  $(OBJDIR)/g_vslider_141faf0e.o \
  $(OBJDIR)/g_vumeter_12d659a1.o \
  $(OBJDIR)/m_atom_3015317e.o \
  $(OBJDIR)/m_binbuf_9f5be779.o \
  $(OBJDIR)/m_class_2df51df5.o \
  $(OBJDIR)/m_conf_3337de91.o \
  $(OBJDIR)/m_glob_39e15fe5.o \
  $(OBJDIR)/m_memory_41712b2e.o \
  $(OBJDIR)/m_obj_d8ffbfb4.o \
  $(OBJDIR)/m_pd_51530761.o \
  $(OBJDIR)/m_sched_6d594374.o \
  $(OBJDIR)/s_audio_4d448ad9.o \
  $(OBJDIR)/s_audio_dummy_22377142.o \
  $(OBJDIR)/s_file_a747fc23.o \
  $(OBJDIR)/s_inter_e9642a5f.o \
  $(OBJDIR)/s_loader_8f2bc87a.o \
  $(OBJDIR)/s_main_b2c7f120.o \
  $(OBJDIR)/s_path_b7eb648c.o \
  $(OBJDIR)/s_print_61ec47f0.o \
  $(OBJDIR)/s_utf8_c1784cd8.o \
  $(OBJDIR)/x_acoustics_b454ee18.o \
  $(OBJDIR)/x_arithmetic_52d20e6c.o \
  $(OBJDIR)/x_array_2e3df7a1.o \
  $(OBJDIR)/x_connective_bfbda470.o \
  $(OBJDIR)/x_gui_1e7c2ec3.o \
  $(OBJDIR)/x_interface_f4aab901.o \
  $(OBJDIR)/x_list_b8ee5720.o \
  $(OBJDIR)/x_midi_ba9c34e3.o \
  $(OBJDIR)/x_misc_baa2efee.o \
  $(OBJDIR)/x_net_1ed7b6a5.o \
  $(OBJDIR)/x_scalar_b1675bae.o \
  $(OBJDIR)/x_text_c65f03ef.o \
  $(OBJDIR)/x_time_c692298f.o \
  $(OBJDIR)/x_vexp_c9c8a629.o \
  $(OBJDIR)/x_vexp_fun_1f45f8e9.o \
  $(OBJDIR)/x_vexp_if_cf76fbfd.o \
  $(OBJDIR)/z_pd_2197ea6f.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking zpd
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning zpd
	@$(CLEANCMD)

strip:
	@echo Stripping zpd
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/bob~_c63b734a.o: ../../pd/extra/bob~/bob~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bob~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bonk~_736bc114.o: ../../pd/extra/bonk~/bonk~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bonk~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/choice_86f76fca.o: ../../pd/extra/choice/choice.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling choice.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fiddle~_4cccdb70.o: ../../pd/extra/fiddle~/fiddle~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fiddle~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/loop~_2fec9860.o: ../../pd/extra/loop~/loop~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling loop~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lrshift~_6f0fe66a.o: ../../pd/extra/lrshift~/lrshift~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lrshift~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/pique_f9d2867c.o: ../../pd/extra/pique/pique.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling pique.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sigmund~_d8f0490a.o: ../../pd/extra/sigmund~/sigmund~.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sigmund~.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/stdout_58738c8a.o: ../../pd/extra/stdout/stdout.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling stdout.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_arithmetic_ad7e4e00.o: ../../pd/src/d_arithmetic.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_arithmetic.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_array_98e54b8d.o: ../../pd/src/d_array.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_array.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_ctl_fc46c68f.o: ../../pd/src/d_ctl.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_ctl.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_dac_fc4c193a.o: ../../pd/src/d_dac.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_dac.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_delay_21147037.o: ../../pd/src/d_delay.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_delay.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_fft_fc6acde8.o: ../../pd/src/d_fft.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_fft.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_fft_fftsg_875cd71.o: ../../pd/src/d_fft_fftsg.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_fft_fftsg.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_filter_a5d87dce.o: ../../pd/src/d_filter.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_filter.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_global_b08b5739.o: ../../pd/src/d_global.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_global.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_math_9c9b399e.o: ../../pd/src/d_math.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_math.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_misc_9d0b6e82.o: ../../pd/src/d_misc.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_misc.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_osc_fcef4a73.o: ../../pd/src/d_osc.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_osc.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_resample_abc87a73.o: ../../pd/src/d_resample.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_resample.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_soundfile_6eb33c3f.o: ../../pd/src/d_soundfile.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_soundfile.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/d_ugen_aa8fd091.o: ../../pd/src/d_ugen.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling d_ugen.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_all_guis_4cef7929.o: ../../pd/src/g_all_guis.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_all_guis.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_array_55b29890.o: ../../pd/src/g_array.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_array.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_bang_c176b36b.o: ../../pd/src/g_bang.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_bang.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_canvas_a763830b.o: ../../pd/src/g_canvas.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_canvas.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_clone_b51d5294.o: ../../pd/src/g_clone.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_clone.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_editor_d3a7de0.o: ../../pd/src/g_editor.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_editor.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_graph_922efe45.o: ../../pd/src/g_graph.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_graph.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_guiconnect_81f87c42.o: ../../pd/src/g_guiconnect.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_guiconnect.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_hdial_af9b5a4f.o: ../../pd/src/g_hdial.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_hdial.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_hslider_f8619d00.o: ../../pd/src/g_hslider.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_hslider.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_io_4712ee59.o: ../../pd/src/g_io.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_io.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_mycanvas_16b8ad37.o: ../../pd/src/g_mycanvas.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_mycanvas.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_numbox_3d44d2f8.o: ../../pd/src/g_numbox.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_numbox.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_readwrite_77e8f540.o: ../../pd/src/g_readwrite.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_readwrite.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_rtext_dbb96956.o: ../../pd/src/g_rtext.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_rtext.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_scalar_7886d89f.o: ../../pd/src/g_scalar.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_scalar.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_template_5322222d.o: ../../pd/src/g_template.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_template.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_text_e06b0920.o: ../../pd/src/g_text.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_text.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_toggle_652e3a07.o: ../../pd/src/g_toggle.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_toggle.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_traversal_9f9678b5.o: ../../pd/src/g_traversal.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_traversal.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_vdial_943297dd.o: ../../pd/src/g_vdial.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_vdial.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_vslider_141faf0e.o: ../../pd/src/g_vslider.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_vslider.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/g_vumeter_12d659a1.o: ../../pd/src/g_vumeter.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling g_vumeter.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_atom_3015317e.o: ../../pd/src/m_atom.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_atom.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_binbuf_9f5be779.o: ../../pd/src/m_binbuf.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_binbuf.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_class_2df51df5.o: ../../pd/src/m_class.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_class.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_conf_3337de91.o: ../../pd/src/m_conf.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_conf.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_glob_39e15fe5.o: ../../pd/src/m_glob.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_glob.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_memory_41712b2e.o: ../../pd/src/m_memory.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_memory.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_obj_d8ffbfb4.o: ../../pd/src/m_obj.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_obj.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_pd_51530761.o: ../../pd/src/m_pd.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_pd.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/m_sched_6d594374.o: ../../pd/src/m_sched.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling m_sched.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_audio_4d448ad9.o: ../../pd/src/s_audio.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_audio.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_audio_dummy_22377142.o: ../../pd/src/s_audio_dummy.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_audio_dummy.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_file_a747fc23.o: ../../pd/src/s_file.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_file.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_inter_e9642a5f.o: ../../pd/src/s_inter.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_inter.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_loader_8f2bc87a.o: ../../pd/src/s_loader.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_loader.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_main_b2c7f120.o: ../../pd/src/s_main.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_main.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_path_b7eb648c.o: ../../pd/src/s_path.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_path.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_print_61ec47f0.o: ../../pd/src/s_print.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_print.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/s_utf8_c1784cd8.o: ../../pd/src/s_utf8.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling s_utf8.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_acoustics_b454ee18.o: ../../pd/src/x_acoustics.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_acoustics.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_arithmetic_52d20e6c.o: ../../pd/src/x_arithmetic.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_arithmetic.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_array_2e3df7a1.o: ../../pd/src/x_array.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_array.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_connective_bfbda470.o: ../../pd/src/x_connective.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_connective.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_gui_1e7c2ec3.o: ../../pd/src/x_gui.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_gui.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_interface_f4aab901.o: ../../pd/src/x_interface.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_interface.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_list_b8ee5720.o: ../../pd/src/x_list.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_list.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_midi_ba9c34e3.o: ../../pd/src/x_midi.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_midi.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_misc_baa2efee.o: ../../pd/src/x_misc.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_misc.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_net_1ed7b6a5.o: ../../pd/src/x_net.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_net.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_scalar_b1675bae.o: ../../pd/src/x_scalar.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_scalar.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_text_c65f03ef.o: ../../pd/src/x_text.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_text.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_time_c692298f.o: ../../pd/src/x_time.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_time.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_vexp_c9c8a629.o: ../../pd/src/x_vexp.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_vexp.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_vexp_fun_1f45f8e9.o: ../../pd/src/x_vexp_fun.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_vexp_fun.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/x_vexp_if_cf76fbfd.o: ../../pd/src/x_vexp_if.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling x_vexp_if.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/z_pd_2197ea6f.o: ../../zpd/z_pd.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling z_pd.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)

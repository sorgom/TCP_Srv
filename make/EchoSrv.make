# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=prod
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq ($(shell echo "test"), "test")
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES +=
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),prod)
TARGETDIR = bin
TARGET = $(TARGETDIR)/EchoSrv_prod
OBJDIR = obj/gcc/prod
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -pedantic-errors -Werror -Wall
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -pedantic-errors -Werror -Wall
ALL_LDFLAGS += $(LDFLAGS) -s -pthread

else ifeq ($(config),verbose)
TARGETDIR = bin
TARGET = $(TARGETDIR)/EchoSrv_verbose
OBJDIR = obj/gcc/verbose
DEFINES += -DDEBUG -DVERBOSE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O0 -g -std=c++17 -pedantic-errors -Werror -Wall
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O0 -g -std=c++17 -pedantic-errors -Werror -Wall
ALL_LDFLAGS += $(LDFLAGS) -pthread

else ifeq ($(config),vsmall)
TARGETDIR = bin
TARGET = $(TARGETDIR)/EchoSrv_vsmall
OBJDIR = obj/gcc/vsmall
DEFINES += -DDEBUG -DVERBOSE -DSELECT_MILLI_SECONDS=1000 -DREAD_BUFFER_SIZE=8
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O0 -g -std=c++17 -pedantic-errors -Werror -Wall
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O0 -g -std=c++17 -pedantic-errors -Werror -Wall
ALL_LDFLAGS += $(LDFLAGS) -pthread

else ifeq ($(config),gpt)
TARGETDIR = bin
TARGET = $(TARGETDIR)/EchoSrv_gpt
OBJDIR = obj/gcc/gpt
DEFINES += -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -pedantic-errors -Werror -Wall
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O3 -std=c++17 -pedantic-errors -Werror -Wall
ALL_LDFLAGS += $(LDFLAGS) -s -pthread

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

ifeq ($(config),prod)
GENERATED += $(OBJDIR)/TCP_Srv_Base.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo_main.o
GENERATED += $(OBJDIR)/Trace.o
OBJECTS += $(OBJDIR)/TCP_Srv_Base.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo_main.o
OBJECTS += $(OBJDIR)/Trace.o

else ifeq ($(config),verbose)
GENERATED += $(OBJDIR)/TCP_Srv_Base.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo_main.o
GENERATED += $(OBJDIR)/Trace.o
OBJECTS += $(OBJDIR)/TCP_Srv_Base.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo_main.o
OBJECTS += $(OBJDIR)/Trace.o

else ifeq ($(config),vsmall)
GENERATED += $(OBJDIR)/TCP_Srv_Base.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo.o
GENERATED += $(OBJDIR)/TCP_Srv_Echo_main.o
GENERATED += $(OBJDIR)/Trace.o
OBJECTS += $(OBJDIR)/TCP_Srv_Base.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo.o
OBJECTS += $(OBJDIR)/TCP_Srv_Echo_main.o
OBJECTS += $(OBJDIR)/Trace.o

else ifeq ($(config),gpt)
GENERATED += $(OBJDIR)/GPT_Srv.o
GENERATED += $(OBJDIR)/GPT_main.o
OBJECTS += $(OBJDIR)/GPT_Srv.o
OBJECTS += $(OBJDIR)/GPT_main.o

endif

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking EchoSrv
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning EchoSrv
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

ifeq ($(config),prod)
$(OBJDIR)/TCP_Srv_Base.o: ../code/TCP_Srv_Base.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo.o: ../code/TCP_Srv_Echo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo_main.o: ../code/TCP_Srv_Echo_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Trace.o: ../code/Trace.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

else ifeq ($(config),verbose)
$(OBJDIR)/TCP_Srv_Base.o: ../code/TCP_Srv_Base.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo.o: ../code/TCP_Srv_Echo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo_main.o: ../code/TCP_Srv_Echo_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Trace.o: ../code/Trace.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

else ifeq ($(config),vsmall)
$(OBJDIR)/TCP_Srv_Base.o: ../code/TCP_Srv_Base.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo.o: ../code/TCP_Srv_Echo.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/TCP_Srv_Echo_main.o: ../code/TCP_Srv_Echo_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Trace.o: ../code/Trace.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

else ifeq ($(config),gpt)
$(OBJDIR)/GPT_Srv.o: ../code/GPT_Srv.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GPT_main.o: ../code/GPT_main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

endif

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
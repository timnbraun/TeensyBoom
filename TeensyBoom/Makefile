#
# Makefile for TeensyBoom
#

LIBRARYPATH = ../../teensy-duino

TEENSY = TEENSY32
include $(LIBRARYPATH)/teensy.mak

TEENSY32 := 1

ifeq ($(TEENSYLC),1)
MCU_CLOCK := 48000000
USBTYPE := USB_MIDI_SERIAL
else 
ifeq ($(TEENSY32),1)
MCU_CLOCK := 72000000
USBTYPE := USB_MIDI_AUDIO_SERIAL
endif
endif

LIBDIR = lib
OBJDIR = obj

LIBS = -L$(LIBDIR) -lTeensyBoomVoice -lAudio -lSPI -lSD -lWire -lteensy -lm

CDEFINES = -DF_CPU=$(MCU_CLOCK) -D$(USBTYPE)

# options needed by many Arduino libraries to configure for Teensy 3.x
CDEFINES += -D__$(MCU)__ -DARDUINO=10805 -DTEENSYDUINO=$(TEENSYDUINO)

CPPFLAGS = -fsingle-precision-constant -Wall -g -Os \
	-mcpu=$(CPUARCH) -mthumb -MMD $(CDEFINES) -I$(LIBRARYPATH)/include
CXXFLAGS = -fpermissive -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti
CFLAGS =
LDFLAGS = -Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 \
	--specs=nano.specs -mcpu=$(CPUARCH) -mthumb -T$(MCU_LD)
ARFLAGS = crvs

TEENSY_LIB = $(LIBDIR)/libteensy.a
AUDIO_LIB  = $(LIBDIR)/libAudio.a
SD_LIB     = $(LIBDIR)/libSD.a
SPI_LIB    = $(LIBDIR)/libSPI.a
WIRE_LIB   = $(LIBDIR)/libWire.a
VOICE_LIB  = $(LIBDIR)/libTeensyBoomVoice.a
LIB_LIST   = $(TEENSY_LIB) $(VOICE_LIB) $(AUDIO_LIB) $(SD_LIB) $(SPI_LIB) $(WIRE_LIB)
TARGET     = TeensyBoom

.PHONY: all load clean
all: $(TARGET).hex
	@echo built $^

clean:
	-rm -f *.hex *.elf
	-rm -rf $(OBJDIR) $(LIBDIR)

$(OBJDIR): ; -$(MKDIR) $@
$(LIBDIR): ; -$(MKDIR) $@


C_FILES = 
INO_FILES = TeensyBoom.ino
CPP_FILES = editor-modes.cpp editor.cpp panel-scanner.cpp pattern.cpp \
	player.cpp voice.cpp
OBJS := $(addprefix $(OBJDIR)/,$(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o))

CPPFLAGS += -I../TeensyBoomVoiceLibrary -I$(LIBRARYPATH)/Audio \
	-I$(LIBRARYPATH)/SD -I$(LIBRARYPATH)/SPI \
	-I$(LIBRARYPATH)/SerialFlash -I$(LIBRARYPATH)/Wire

$(OBJDIR)/%.o : %.ino
	$(COMPILE.cpp) -Werror -x c++ $(OUTPUT_OPTION) $<

$(OBJDIR)/%.o : %.cpp
	$(COMPILE.cpp) -Werror $(OUTPUT_OPTION) $<

$(TARGET).elf: $(OBJDIR) $(OBJS) $(LIBDIR) $(LIB_LIST) $(MCU_LD)
	$(LINK.o) $(OBJS) $(LIBS) -o $@
	@echo built $@

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@$(SIZE) $<
	@echo
	@echo Converting $@ from $<
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
	@echo

load: $(TARGET).hex
	teensy_loader_cli.exe --mcu=$(MCU) -w -v $<

##############################
#
# Library building
#
##############################

VOICE_LIB_CPP_FILES = Effect-ClapEnv.cpp \
	Synth-Clatter.cpp Synth-Decay.cpp Synth-DrumHeart.cpp \
	TeensyBoomVoiceLibrary.cpp synth_simple_drum.cpp
VOICE_LIB_OBJS := $(addprefix $(OBJDIR)/,$(VOICE_LIB_CPP_FILES:.cpp=.o) $(VOICE_LIB_C_FILES:.c=.o))

$(OBJDIR)/%.o : ../TeensyBoomVoiceLibrary/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(VOICE_LIB): $(OBJDIR) $(LIBDIR) $(VOICE_LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $(VOICE_LIB_OBJS)

include $(LIBRARYPATH)/libraries.mak

-include $(wildcard $(OBJDIR)/*.d)

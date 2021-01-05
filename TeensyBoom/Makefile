#
# Makefile for teensykick
#

LIBRARYPATH = ../../teensyduino

TEENSY32 := 1

ifeq ($(TEENSYLC),1)
MCU := MKL26Z64
CPUARCH := cortex-m0plus
MCU_LD := $(LIBRARYPATH)/mkl26z64.ld
MCU_CLOCK := 48000000
TEENSYDUINO=144
USBTYPE := USB_SERIAL
else 
ifeq ($(TEENSY32),1)
MCU := MK20DX256
CPUARCH := cortex-m4
MCU_LD := $(LIBRARYPATH)/mk20dx256.ld
MCU_CLOCK := 72000000
TEENSYDUINO := 153
USBTYPE := USB_AUDIO
endif
endif

LIB_DIR = lib
OBJ_DIR = obj

LIBS = -L$(LIB_DIR) -lTeensyBoomVoice -lAudio -lSPI -lSD -lWire -lteensy -lm

CDEFINES = -DF_CPU=$(MCU_CLOCK) -D$(USBTYPE)

# options needed by many Arduino libraries to configure for Teensy 3.x
CDEFINES += -D__$(MCU)__ -DARDUINO=10805 -DTEENSYDUINO=$(TEENSYDUINO)

CPPFLAGS = -fsingle-precision-constant -Wall -g -Os \
	-mcpu=$(CPUARCH) -mthumb -MMD $(CDEFINES) -I$(LIBRARYPATH)/include
CXXFLAGS = -fpermissive -std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti
CFLAGS =
LDFLAGS = -Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 \
	--specs=nosys.specs -mcpu=$(CPUARCH) -mthumb -T$(MCU_LD)

# names for the compiler programs
CROSS_COMPILE=arm-none-eabi-
CC      = $(CROSS_COMPILE)gcc
CXX     = $(CROSS_COMPILE)g++
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE    = $(CROSS_COMPILE)size
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib

MKDIR   = mkdir -p

TEENSY_LIB = $(LIB_DIR)/libteensy.a
AUDIO_LIB  = $(LIB_DIR)/libAudio.a
SD_LIB     = $(LIB_DIR)/libSD.a
SPI_LIB    = $(LIB_DIR)/libSPI.a
WIRE_LIB   = $(LIB_DIR)/libWire.a
VOICE_LIB  = $(LIB_DIR)/libTeensyBoomVoice.a

.PHONY: all load clean
all: $(TEENSY_LIB) $(VOICE_LIB) $(AUDIO_LIB) $(SD_LIB) $(SPI_LIB) $(WIRE_LIB)
	@echo built $^

clean:
	-rm -f *.hex *.elf
	-rm -rf $(OBJ_DIR) $(LIB_DIR)

$(OBJ_DIR): ; -$(MKDIR) $@
$(LIB_DIR): ; -$(MKDIR) $@

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/src/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/src/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/Audio/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/Audio/%.S
	$(COMPILE.S) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/Audio/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/SD/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/SD/utility/%.cpp
	$(COMPILE.cpp) -I$(LIBRARYPATH)/SD/utility $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/SPI/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : $(LIBRARYPATH)/Wire/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : ../TeensyBoomVoiceLibrary/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

LIB_C_FILES = analog.c mk20dx128.c nonstd.c pins_teensy.c
LIB_C_FILES += usb_desc.c usb_dev.c usb_inst.c usb_mem.c usb_midi.c usb_seremu.c \
	usb_serial.c
LIB_CPP_FILES = AudioStream.cpp DMAChannel.cpp EventResponder.cpp \
	HardwareSerial.cpp Print.cpp WMath.cpp WString.cpp avr_emulation.cpp main.cpp \
	new.cpp serialEvent.cpp usb_audio.cpp yield.cpp

VOICE_LIB_CPP_FILES = Effect-ClapEnv.cpp \
	Synth-Clatter.cpp Synth-Decay.cpp Synth-DrumHeart.cpp \
	TeensyBoomVoiceLibrary.cpp synth_simple_drum.cpp
VOICE_LIB_OBJS := $(addprefix $(OBJ_DIR)/,$(VOICE_LIB_CPP_FILES:.cpp=.o) $(VOICE_LIB_C_FILES:.c=.o))


LIB_OBJS := $(LIB_C_FILES:.c=.o) $(LIB_CPP_FILES:.cpp=.o)
LIB_OBJS := $(addprefix $(OBJ_DIR)/,$(LIB_OBJS))

AUDIO_LIB_CPP_FILES = control_sgtl5000.cpp effect_multiply.cpp filter_biquad.cpp \
	mixer.cpp output_i2s.cpp synth_dc.cpp synth_whitenoise.cpp
AUDIO_LIB_C_FILES = data_waveforms.c
AUDIO_LIB_S_FILES = memcpy_audio.S
AUDIO_OBJS := $(addprefix $(OBJ_DIR)/,$(AUDIO_LIB_C_FILES:.c=.o) \
	$(AUDIO_LIB_CPP_FILES:.cpp=.o) $(AUDIO_LIB_S_FILES:.S=.o))

SD_LIB_CPP_FILES = File.cpp SD.cpp Sd2Card.cpp SdFile.cpp SdVolume.cpp
SD_OBJS := $(addprefix $(OBJ_DIR)/,$(SD_LIB_CPP_FILES:.cpp=.o))

SPI_LIB_CPP_FILES = SPI.cpp
SPI_OBJS := $(addprefix $(OBJ_DIR)/,$(SPI_LIB_CPP_FILES:.cpp=.o))

WIRE_LIB_CPP_FILES = Wire.cpp WireKinetis.cpp
WIRE_OBJS := $(addprefix $(OBJ_DIR)/,$(WIRE_LIB_CPP_FILES:.cpp=.o))

-include $(LIB_OBJS:.o=.d)

$(TEENSY_LIB): $(OBJ_DIR) $(LIB_DIR) $(LIB_OBJS)
	$(AR) crvs $@ $(LIB_OBJS)

$(VOICE_LIB): $(OBJ_DIR) $(LIB_DIR) $(VOICE_LIB_OBJS)
	$(AR) crvs $@ $(VOICE_LIB_OBJS)

$(AUDIO_LIB): $(OBJ_DIR) $(LIB_DIR) $(AUDIO_OBJS)
	$(AR) crvs $@ $(AUDIO_OBJS)

$(SD_LIB): $(OBJ_DIR) $(SD_OBJS)
	$(AR) crvs $@ $(SD_OBJS)

$(SPI_LIB): $(OBJ_DIR) $(SPI_OBJS)
	$(AR) crvs $@ $(SPI_OBJS)

$(WIRE_LIB): $(OBJ_DIR) $(WIRE_OBJS)
	$(AR) crvs $@ $(WIRE_OBJS)

TARGET = TeensyBoom
C_FILES = 
INO_FILES = TeensyBoom.ino
CPP_FILES = editor-modes.cpp editor.cpp panel-scanner.cpp pattern.cpp \
	player.cpp voice.cpp
OBJS := $(addprefix $(OBJ_DIR)/,$(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) $(INO_FILES:.ino=.o))

CPPFLAGS += -I../TeensyBoomVoiceLibrary -I$(LIBRARYPATH)/Audio \
	-I$(LIBRARYPATH)/SD -I$(LIBRARYPATH)/SPI \
	-I$(LIBRARYPATH)/SerialFlash -I$(LIBRARYPATH)/Wire

$(OBJ_DIR)/%.o : %.ino
	$(COMPILE.cpp) -Werror -x c++ $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : %.cpp
	$(COMPILE.cpp) -Werror $(OUTPUT_OPTION) $<

all: $(TARGET).hex

load: $(TARGET).hex
	teensy_loader_cli.exe --mcu=$(MCU) -w -v $<

$(TARGET).elf: $(OBJ_DIR) $(OBJS) $(MCU_LD)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
	@echo built $@

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@$(SIZE) $<
	@echo
	@echo Converting $@ from $<
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
	@echo

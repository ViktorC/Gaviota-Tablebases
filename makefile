MAKE := make -f makefile
ifeq ($(OS),Windows_NT)
	JINC_DIR := \include
	JINC_SUBDIR := \win32
	TARG_LIB := gtb.dll
	DEFAULT_DEFS := -DZ_PREFIX -DNDEBUG -DMINGW
	DEFAULT_LIBS := 
else
	JINC_DIR := /include
	JINC_SUBDIR := /linux
	TARG_LIB := libgtb.so
	DEFAULT_DEFS := -DZ_PREFIX -DNDEBUG
	DEFAULT_LIBS := -lm -lpthread
endif
INCLUDES = -I. \
	-Isysport/ \
	-Icompression/ \
	-Icompression/liblzf/ \
	-Icompression/zlib/ \
	-Icompression/lzma/ \
	-Icompression/huffman/ \
	-Ijni/ \
	-I"$(JAVA_HOME)$(JINC_DIR)" \
	-I"$(JAVA_HOME)$(JINC_DIR)$(JINC_SUBDIR)"
DEFAULT_CC := gcc
DEFAULT_ARCH := -m64
DEFAULT_CFLAGS = -fPIC -std=c99 $(DEFAULT_ARCH) $(DEFAULT_DEFS) $(INCLUDES)
OPT_CFLAGS := -msse -O3
DEBUG_CFLAGS := -O0 -g -DDEBUG
DEFAULT_LDFLAGS := -shared
SOURCES := gtb-probe.c gtb-dec.c gtb-att.c sysport/sysport.c \
	compression/wrap.c compression/huffman/hzip.c \
	compression/lzma/LzmaEnc.c compression/lzma/LzmaDec.c \
	compression/lzma/Alloc.c compression/lzma/LzFind.c \
	compression/lzma/Lzma86Enc.c compression/lzma/Lzma86Dec.c \
	compression/lzma/Bra86.c compression/zlib/zcompress.c \
	compression/zlib/uncompr.c compression/zlib/inflate.c \
	compression/zlib/deflate.c compression/zlib/adler32.c \
	compression/zlib/crc32.c compression/zlib/infback.c \
	compression/zlib/inffast.c compression/zlib/inftrees.c \
	compression/zlib/trees.c compression/zlib/zutil.c \
	compression/liblzf/lzf_c.c compression/liblzf/lzf_d.c \
	jni/net_viktorc_detroid_framework_engine_GaviotaTableBaseJNI.c
BUILD_DIR := build
OBJECTS = $(SOURCES:%.c=%.o)
$(TARG_LIB): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $?
.PHONY: all clean
.DEFAULT_GOAL := debug
all:
	$(MAKE) $(TARG_LIB) \
		CC='$(DEFAULT_CC)' \
		CFLAGS='$(DEFAULT_CFLAGS)' \
		LDFLAGS='$(DEFAULT_LDFLAGS)' \
		LIBS='$(DEFAULT_LIBS)'
opt:
	$(MAKE) $(TARG_LIB) \
		CC='$(DEFAULT_CC)' \
		CFLAGS='$(DEFAULT_CFLAGS) $(OPT_CFLAGS)' \
		LDFLAGS='$(DEFAULT_LDFLAGS)' \
		LIBS='$(DEFAULT_LIBS)'
debug:
	$(MAKE) $(TARG_LIB) \
		CC='$(DEFAULT_CC)' \
		CFLAGS='$(DEFAULT_CFLAGS) $(DEBUG_CFLAGS)' \
		LDFLAGS='$(DEFAULT_LDFLAGS)' \
		LIBS='$(DEFAULT_LIBS)'
clean:
	$(RM) $(OBJECTS) $(TARG_LIB)
.depend:
	$(CC) -MM $(DEFAULT_CFLAGS) $(SOURCES) > $@
include .depend

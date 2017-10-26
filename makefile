INCLUDE = -I"." \
	-Isysport/ \
	-Icompression/ \
	-Icompression/liblzf/ \
	-Icompression/zlib/ \
	-Icompression/lzma/ \
	-Icompression/huffman/ \
	-Ijni/ \
	-I"$(JAVA_HOME)\include" \
	-I"$(JAVA_HOME)\include\win32"
MAKE = make -f makefile
DEFAULT_CC = gcc
DEFAULT_DEFINE = -DZ_PREFIX -DNDEBUG -DMINGW
DEFAULT_ARCHFLAGS = -m32
DEFAULT_CFLAGS = -fPIC -Wall -Wextra -O3 -std=c99 -c $(INCLUDE)
OPTFLAGS = -msse
DEBUGFLAGS = -O0 -g -DDEBUG
LDFLAGS = -shared
LIBS = -lm -lpthread
SRCFILES := gtb-probe.c gtb-dec.c gtb-att.c sysport/sysport.c \
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
OBJECTS := $(patsubst %.c, %.o, $(SRCFILES))
TARGETLIB := libgtb.so
$(TARGETLIB): $(OBJECTS)
	$(CC) $(ARCHFLAGS) $(CFLAGS) $(DEFINE) $(LIBS) -o $(TARGETLIB) \
	$(OBJECTS) $(LDFLAGS)
.PHONY: all clean
.DEFAULT_GOAL := all
all:
	$(MAKE) $(TARGETLIB) \
		CC='$(DEFAULT_CC)' \
		ARCHFLAGS='$(DEFAULT_ARCHFLAGS)' \
		DEFINE='$(DEFAULT_DEFINE)' \
		CFLAGS='$(DEFAULT_CFLAGS)' \
		LDFLAGS='$(LDFLAGS)'
opt:
	$(MAKE) $(TARGETLIB) \
		CC='$(DEFAULT_CC)' \
		ARCHFLAGS='$(DEFAULT_ARCHFLAGS)' \
		DEFINE='$(DEFAULT_DEFINE)' \
		CFLAGS='$(OPTFLAGS) $(DEFAULT_CFLAGS)' \
		LDFLAGS='$(LDFLAGS)'
debug:
	$(MAKE) $(TARGETLIB) \
		CC='$(DEFAULT_CC)' \
		ARCHFLAGS='$(DEFAULT_ARCHFLAGS)' \
		DEFINE='$(DEFAULT_DEFINE)' \
		CFLAGS='$(DEBUGFLAGS) $(DEFAULT_CFLAGS)' \
		LDFLAGS='$(LDFLAGS)'
clean:
	$(RM) -f $(OBJFILES) $(TARGETLIB)
.depend:
	$(CC) -MM $(DEFAULT_CFLAGS) $(SRCFILES) > $@
include .depend

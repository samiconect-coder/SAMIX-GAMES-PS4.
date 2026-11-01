
TITLE       := SAMIX GAMES
VERSION     := 1.00
TITLE_ID    := SAMX00001

TOOLCHAIN   := $(OO_PS4_TOOLCHAIN)
COMMONDIR   := $(TOOLCHAIN)/samples/_common
PROJDIR     := src
INTDIR      := build

LIBS        := -lc -lkernel -lc++ -lSceVideoOut -lScePad -lSceUserService

CPPFILES    := $(wildcard $(PROJDIR)/*.cpp)
COMMONFILES := $(wildcard $(COMMONDIR)/*.cpp)
OBJS        := $(patsubst $(PROJDIR)/%.cpp,$(INTDIR)/%.o,$(CPPFILES)) \
               $(patsubst $(COMMONDIR)/%.cpp,$(INTDIR)/%.o,$(COMMONFILES))

CXXFLAGS    := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -c \
               -isysroot $(TOOLCHAIN) -isystem $(TOOLCHAIN)/include \
               -isystem $(TOOLCHAIN)/include/c++/v1 -I$(COMMONDIR)

LDFLAGS     := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x \
               --eh-frame-hdr -L$(TOOLCHAIN)/lib $(LIBS) \
               $(TOOLCHAIN)/lib/crt1.o

CCX         := clang++
LD          := ld.lld

all: eboot.bin

$(INTDIR):
	mkdir -p $(INTDIR)

$(INTDIR)/%.o: $(PROJDIR)/%.cpp | $(INTDIR)
	$(CCX) $(CXXFLAGS) -o $@ $<

$(INTDIR)/%.o: $(COMMONDIR)/%.cpp | $(INTDIR)
	$(CCX) $(CXXFLAGS) -o $@ $<

eboot.bin: $(OBJS)
	$(LD) $(INTDIR)/*.o -o $(INTDIR)/SAMIX.elf $(LDFLAGS)
	$(TOOLCHAIN)/bin/linux/create-fself -in=$(INTDIR)/SAMIX.elf -out=$(INTDIR)/SAMIX.oelf --eboot "eboot.bin" --paid 0x3800000000000011

clean:
	rm -rf $(INTDIR) eboot.bin

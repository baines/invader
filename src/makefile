exec = invader
headers = $(wildcard *.h)
sources = $(wildcard *.cpp)
objects = $(patsubst %.cpp,$(builddir)/%.o,$(sources))

ifdef WINDOWS
    CXX := i586-mingw32msvc-g++
    CXXFLAGS := -DWIN32 -Dmain=SDL_main
    LDFLAGS := -lmingw32 -lSDLmain -mwindows
    builddir := ../build/win32
    exec := $(exec).exe
	objects += $(builddir)/$(exec).res
else
    CXX := g++
    CXXFLAGS := `sdl-config --cflags`
    LDFLAGS := -Wl,-rpath,'$$ORIGIN/data/lib'
    builddir := ../build/linux
endif

LDFLAGS += -lSDL -lSDL_image -lSDL_mixer
CXXFLAGS += -ffast-math -fno-rtti -fno-exceptions

ifdef RELEASE
    CXXFLAGS += -O3 -fno-stack-protector -fomit-frame-pointer -U_FORTIFY_SOURCE
    LDFLAGS += -Wl,-s -Wl,-hash-style=both -Wl,--as-needed
else
    CXXFLAGS += -O2 -g
endif

all: $(exec)

$(exec): $(objects)
	$(CXX) -o ../$(exec) $(objects) $(CXXFLAGS) $(LDFLAGS)

$(builddir)/%.o: %.cpp $(headers)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
$(builddir)/$(exec).res:
	i586-mingw32msvc-windres $(builddir)/$(exec).rc -O coff -o $@

clean:
	rm -f $(objects) ../$(exec) *~
	
.PHONY: clean

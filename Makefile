CC=clang
CXX=clang++
TARGETDIR=./

FLAGS= -march=native -W -Wall

CFLAGS := -std=c99 -fgnu89-inline -Wno-unused-function -Wno-unused-const-variable

CXXFLAGS := -std=c++11
LD_FLAGS := $(CXXFLAGS) -lssl -lcrypto

ifdef DEBUG
	FLAGS += -g -ftrapv -fsanitize=undefined -fsanitize=address -fsanitize=alignment
else
	FLAGS += -O3
	LD_FLAGS += -s
endif

#Determine if SSE2 or greater is available
SSE_TAGS = $(shell /bin/grep -m 1 flags /proc/cpuinfo | /bin/grep -o \
	'sse2\|sse3\|ssse3\|sse4a\|sse4.1\|sse4.2' | sed  's/\_/./g')

ifneq (${SSE_TAGS},) 
    #Choose optimized hash function
	HDIR=./src/hashes/blake2-sse
	HASH=$(HDIR)/blake2b.c


else
	#use reference implementation
    HDIR=./src/hashes/blake2-ref
	HASH=$(HDIR)/blake2b-ref.c
endif
#add hash directory to flags
CFLAGS +=  -I$(HDIR)
CXXFLAGS += -I$(HDIR)
LDFLAGS += -L$(HDIR)

BASEFILES := src/catena-helpers.cpp src/catena.cpp src/catenafactory.cpp

MODULES   := algorithms random_layers graphs hashes phi
SRC_DIR   := $(addprefix src/,$(MODULES))
OBJS_DIR  := $(addprefix objs/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,objs/%.o,$(SRC))

BLAKE2b 	  := objs/blake2.o

vpath %.cpp $(SRC_DIR)

#builds the cpp files in the respective directories
define make-goal
$1/%.o: %.cpp
	$(CXX) $(FLAGS) $(CXXFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdir clean 

all: checkdir catena-scramble catena-measure

catena-scramble: src/catena-scramble.cpp $(BASEFILES) $(OBJ) $(BLAKE2b)
	$(CXX) $(FLAGS) $(LD_FLAGS) -o $(TARGETDIR)$@ $^

catena-measure: src/catena-measure.cpp $(BASEFILES) $(OBJ) $(BLAKE2b)
	$(CXX) $(FLAGS) $(LD_FLAGS) -o $(TARGETDIR)$@ $^

$(BLAKE2b):$(HASH)
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

checkdir: $(OBJS_DIR)

$(OBJS_DIR):
	@mkdir -p $@

clean:
	@rm -rf ./objs/*
	@rm -f catena-scramble
	@rm -f catena-measure

#creates a target for every subfolder of src/objs
$(foreach bdir,$(OBJS_DIR),$(eval $(call make-goal,$(bdir))))

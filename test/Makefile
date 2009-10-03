TARGET=test
CPP_FILES=test.cpp test1.cpp

INC_PATH=../

LIB_FILES=
STATIC_LIB_FILES=

CFLAGS=
DEFINES=

LINKFLAGS=


OBJ_DIR=debug.gcc


# =============
# common part
# =============

O_EXT=.o
            

GCC=$(PREFIX)g++
LD=$(PREFIX)ld
STRIP=$(PREFIX)strip -s

BIN_DIR=
LIB_DIR=
INC_DIR=
THPL_INC_DIR=

MAKE_INC_PATH:=$(subst \,/,$(INC_PATH) $(INC_DIR) $(THPL_INC_DIR))

GCC_DEPS_FILENAME=$(OBJ_DIR)/$(TARGET).dep
SPECIFIC_DEPS_OPTIONS=
GCC_DEPS_OPTIONS=-MM -MG $(addprefix -I, $(MAKE_INC_PATH)) $(SPECIFIC_DEPS_OPTIONS)


SPECIFIC_CFLAGS= -Wall -pthread -D_THREAD_SAFE -D_REENTRANT -D_LIBC_REENTRANT -DLINUX\
             -Woverloaded-virtual -Wsign-promo -ggdb $(addprefix -I , $(MAKE_INC_PATH)) \
                 $(addprefix -D, $(DEFINES))

SPECIFIC_LINKFLAGS= -ggdb $(addprefix -L , $(MAKE_LIB_PATH) $(BIN_DIR))

MAKE_CFLAGS=$(CFLAGS) $(SPECIFIC_CFLAGS)
MAKE_LINKFLAGS=$(LINKFLAGS) $(SPECIFIC_LINKFLAGS)

LIBS=$(addprefix -l, $(LIB_FILES) $(STATIC_LIB_FILES))
STATIC_LIBS=$(addprefix lib, $(addsuffix $(LIB_EXT), $(STATIC_LIB_FILES)))


obj_files=$(patsubst %.CPP,%$(O_EXT),$(patsubst %.cpp,%$(O_EXT),$(1)))

OBJECT_FILES=$(addprefix $(OBJ_DIR)/,$(notdir $(call obj_files,$(CPP_FILES))))



# =============
# targets
# =============

all: build


#///////////////////////////////////////////////
ifeq ($(DEPENDENCY_CALCULATED),yes)

build: $(OBJ_DIR)/$(TARGET)


$(OBJ_DIR)/$(TARGET): $(OBJECT_FILES) $(STATIC_LIBS)
	$(GCC) $(MAKE_LINKFLAGS) -o $(OBJ_DIR)/$(TARGET) $(OBJECT_FILES) $(LIBS) $(LIBS)


$(OBJ_DIR)/%$(O_EXT): %.cpp
	$(GCC) $(MAKE_CFLAGS) $(DEFINES) -c $< -o $@


include $(GCC_DEPS_FILENAME)


#////////////////($(DEPENDENCY_CALCULATED),...)
else

build: make_deps_manual call_make

# dependency
make_deps_manual: make_deps_banner $(CPP_FILES)

make_deps_banner:
	@echo Perform dependency analysis...
	@rm -f $(GCC_DEPS_FILENAME)


.PHONY: $(CPP_FILES)

$(CPP_FILES):
	@$(GCC) $(GCC_DEPS_OPTIONS) -MT $(OBJ_DIR)/$(notdir $(call obj_files,$@)) $@>>$(GCC_DEPS_FILENAME)

call_make:
	@make -f Makefile DEPENDENCY_CALCULATED=yes


endif
#////////////////($(DEPENDENCY_CALCULATED),...)


clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/$(TARGET) $(GCC_DEPS_FILENAME)


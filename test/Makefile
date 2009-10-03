TARGET=test
CPP_FILES=test.o test1.o
HEADERS=pch.h PrintData.h struct.h test.h treater.h

MAKE_CFLAGS= -I ../
DEFINES=

STATIC_LIBS=
LIBS=
MAKE_LINKFLAGS=


OBJ_DIR=debug.gcc


# =============
# common part
# =============

O_EXT=.o
            

GCC=$(PREFIX)g++
LD=$(PREFIX)ld
STRIP=$(PREFIX)strip -s


obj_files=$(patsubst %.CPP,%$(O_EXT),$(patsubst %.cpp,%$(O_EXT),$(1)))

OBJECT_FILES=$(addprefix $(OBJ_DIR)/,$(notdir $(call obj_files,$(CPP_FILES))))

# =============
# targets
# =============

all:	$(OBJ_DIR)/$(TARGET)

$(OBJ_DIR)/$(TARGET): $(OBJECT_FILES) $(STATIC_LIBS)
	$(GCC) $(MAKE_LINKFLAGS) -o $(OBJ_DIR)/$(TARGET) $(OBJECT_FILES) $(LIBS) $(LIBS)


$(OBJ_DIR)/%$(O_EXT): %.cpp
	$(GCC) $(MAKE_CFLAGS) $(DEFINES) -c $< -o $@


#$(CPP_FILES):
#	@$(GCC) $(GCC_DEPS_OPTIONS) -MT $(BIN_PATH)/$(notdir $(call obj_files,$@)) $@>>$(GCC_DEPS_FILENAME)


#$(TARGET):$(OBJECT_FILES)
#	$(CC) -o $@ $^
#	$(STRIP) $@

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/$(TARGET)

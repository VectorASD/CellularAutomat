APP_NAME = main
LIB_NAME = libyeah
PROJECT = cellular

FreeTypeARCHIVE = freetype-2.12.1
FreeTypeLIB_PATH = $(FreeTypeARCHIVE)/objs/libfreetype.a
GLEW_ARCHIVE = glew-1.9.0
GLEW_PATH = $(GLEW_ARCHIVE)
GLFW_ARCHIVE = glfw-3.3.7
GLFW_PATH = $(GLFW_ARCHIVE)/lol

CFLAGS = -Wall -Werror -I include -MMD -I $(FreeTypeARCHIVE)/include
LFLAGS = -lglfw -lGL -lGLEW -lm
TEST_CFLAGS = $(CFLAGS) -I thirdparty

APP_SRC = $(wildcard src/$(PROJECT)/*.c)
LIB_SRC = $(wildcard src/lib$(PROJECT)/*.c)
TEST_SRC = $(wildcard test/*.c)
APP_OBJ = $(APP_SRC:src/%.c=obj/src/%.o)
LIB_OBJ = $(LIB_SRC:src/%.c=obj/src/%.o)
TEST_OBJ = $(TEST_SRC:test/%.c=obj/test/%.o)
OBJ = $(APP_OBJ) $(LIB_OBJ)
DEPS = $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

APP_PATH = bin/$(PROJECT)/$(APP_NAME)
LIB_PATH = obj/src/lib$(PROJECT)/$(LIB_NAME).a
TEST_PATH = bin/$(PROJECT)-test/$(APP_NAME)

.PHONY: all
all: $(FreeTypeLIB_PATH) $(GLFW_PATH) $(OBJ) $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJ) $(LIB_PATH) $(FreeTypeLIB_PATH) 
	gcc $^ -o $@ $(LFLAGS)

$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $^

$(OBJ):
	gcc $(CFLAGS) -o $@ -c $(@:obj/src/%.o=src/%.c)

ifneq ($(wildcard $(FreeTypeARCHIVE)),)
.PHONY: clean
clean:
	rm -f $(APP_PATH) $(LIB_PATH) $(OBJ) $(DEPS) $(TEST_OBJ) $(TEST_PATH)
	cd $(FreeTypeARCHIVE) && make clean
endif

run: all
	./$(APP_PATH)

.PHONY: test
test: $(TEST_PATH)
	./$(TEST_PATH)

$(TEST_PATH): $(TEST_OBJ) $(LIB_PATH)
	gcc $^ -o $@

$(TEST_OBJ):
	gcc $(TEST_CFLAGS) -o $@ -c $(@:obj/test/%.o=test/%.c)

$(FreeTypeLIB_PATH):
ifeq ($(wildcard $(FreeTypeARCHIVE)),)
	tar -jxf $(FreeTypeARCHIVE).tar.bz
endif
	cd $(FreeTypeARCHIVE) && make

$(GLEW_PATH):
ifeq ($(wildcard $(GLEW_ARCHIVE)),)
	tar -jxf $(GLEW_ARCHIVE).tar.bz
endif
	@echo XD

$(GLFW_PATH):
	sudo apt-get install libglfw3-dev
#ifeq ($(wildcard $(GLFW_ARCHIVE)),)
#	tar -jxf $(GLFW_ARCHIVE).tar.bz
#endif
#ifeq ($(wildcard $(GLFW_ARCHIVE)/Makefile),)
#	cd $(GLFW_ARCHIVE) && cmake CMakeLists.txt
#endif
	@echo XD

.PHONY: archivate
archivate:
	tar -jcf $(FreeTypeARCHIVE).tar.bz $(FreeTypeARCHIVE)

.PHONY: archivate2
archivate2:
	tar -jcf $(GLEW_ARCHIVE).tar.bz $(GLEW_ARCHIVE)

.PHONY: archivate3
archivate3:
	tar -jcf $(GLFW_ARCHIVE).tar.bz $(GLFW_ARCHIVE)

format:
	git ls-files *.c | xargs clang-format -i --verbose && git diff --exit-code

APP_NAME = main
LIB_NAME = libyeah
PROJECT = cellular

CFLAGS = -Wall -Werror -I include -MMD
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
all: $(OBJ) $(APP_PATH)

-include $(DEPS)

$(APP_PATH): $(APP_OBJ) $(LIB_PATH)
	gcc $^ -o $@ $(LFLAGS)

$(LIB_PATH): $(LIB_OBJ)
	ar rcs $@ $^

$(OBJ):
	gcc $(CFLAGS) -o $@ -c $(@:obj/src/%.o=src/%.c)

.PHONY: clean
clean:
	rm -f $(APP_PATH) $(LIB_PATH) $(OBJ) $(DEPS) $(TEST_OBJ) $(TEST_PATH)

run: all
	./$(APP_PATH)

.PHONY: test
test: $(TEST_PATH)
	./$(TEST_PATH)

$(TEST_PATH): $(TEST_OBJ) $(LIB_PATH)
	gcc $^ -o $@

$(TEST_OBJ):
	gcc $(TEST_CFLAGS) -o $@ -c $(@:obj/test/%.o=test/%.c)


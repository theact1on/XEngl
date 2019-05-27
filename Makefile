.PHONY: all clean run run_test

PROJECT    := XEngl
TEST    := XEngl_test

DATA       := data
COMPILER   := gcc
FLAGS      := -fdiagnostics-color=always -std=gnu11 -Werror -o
GTK_LIBS   := `pkg-config --libs gtk+-3.0`
GTK_CFLAGS :=  `pkg-config --cflags gtk+-3.0`
#GTK_LIBS   := $(wildcard include_gtk/lib64/*.so*) $(wildcard include_gtk/lib64/lib64_data/*.so*) $(wildcard include_gtk/lib64/*.a*)
#GTK_CFLAGS :=  -I include_gtk/include/gtk-3.0 -I include_gtk/include/pango-1.0 -I include_gtk/include/glib-2.0 -I include_gtk/lib64/glib-2.0/include -I include_gtk/include/fribidi -I include_gtk/include/cairo -I include_gtk/include/pixman-1 -I include_gtk/include/freetype2 -I include_gtk/include/libpng16 -I include_gtk/include/uuid -I include_gtk/include/harfbuzz -I include_gtk/include/gdk-pixbuf-2.0 -I include_gtk/include/libmount -I include_gtk/include/blkid -I include_gtk/include/gio-unix-2.0/ -I include_gtk/include/libdrm -I include_gtk/include/atk-1.0 -I include_gtk/include/at-spi2-atk/2.0 -I include_gtk/include/at-spi-2.0 -I include_gtk/include/dbus-1.0 -I include_gtk/lib64/dbus-1.0/include -pthread
SRCC       := src/code
SRCT       := src/tests
BUILDC     := build/code
BUILDT     := build/tests
BIN        := bin
OBJECTSC   := $(patsubst $(SRCC)/%.c,$(BUILDC)/%.o,$(wildcard $(SRCC)/*.c))
OBJECTST   := $(patsubst $(SRCT)/%.c,$(BUILDT)/%.o,$(wildcard $(SRCT)/*.c))

all: dirs $(BIN)/$(PROJECT) $(BIN)/$(TEST)

-include $(BUILDC)/*.d

$(BIN)/$(PROJECT): $(OBJECTSC)
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(GTK_CFLAGS) $(FLAGS)  $@ $^ $(GTK_LIBS) 2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
		printf "\033[0;32m%s \033[0m%s\n"  "   [OK]  " " The program was compiled successfuly. "; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		printf "\033[0;31m%s \033[0m%s\n"  "[FAILING]" " The program was compiled with errors. "; \
		cat error.log; \
		exit 1; \
	fi
	@rm error.log

$(BUILDC)/%.o: $(SRCC)/%.c
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(GTK_CFLAGS) -MMD -c $(FLAGS) $@ $< $(GTK_LIBS) 2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		cat error.log; \
		exit 1; \
	fi

run: $(BIN)/$(PROJECT)
	$(BIN)/$(PROJECT)

# Compiling tests

-include $(BUILDT)/*.d

$(BIN)/$(TEST): $(OBJECTST)
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(FLAGS)  $@ $^ 2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
		printf "\033[0;32m%s \033[0m%s\n"  "   [OK]  " " The tests were compiled successfuly. "; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		printf "\033[0;31m%s \033[0m%s\n"  "[FAILING]" " The tests were compiled with errors. "; \
		cat error.log; \
		exit 1; \
	fi
	@rm error.log

$(BUILDT)/%.o: $(SRCT)/%.c
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) -MMD -c -I thirdparty $(FLAGS) $@ $< 2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		cat error.log; \
		exit 1; \
	fi

run_test: $(BIN)/$(TEST)
	$(BIN)/$(TEST)

dirs:
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Created: " $(BUILDC)
	@mkdir -p $(BUILDC)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Created: " $(BUILDC)

	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Created: " $(BUILDT)
	@mkdir -p $(BUILDT)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Created: " $(BUILDT)

	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Created: " $(BIN)
	@mkdir -p $(BIN)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Created: " $(BIN)

	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Created: " $(DATA)
	@mkdir -p $(DATA)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Created: " $(DATA)

clean:
	@printf "\033[0;33m%s \033[0m%s\r" "[WAITING]" " Removing build artifacts."
	@rm -rf build $(BIN)
	@printf "\033[0;32m%s \033[0m%s\n" "   [OK]  " " All assembly artifacts have been removed successfully."
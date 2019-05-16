PROJECT    := XEngl

DATA       := data
COMPILER   := gcc
FLAGS      := -fdiagnostics-color=always -std=gnu11 -Werror -o
GTK_LIBS   := `pkg-config --libs gtk+-3.0`
GTK_CFLAGS :=  `pkg-config --cflags gtk+-3.0`
#GTK_LIBS   := $(wildcard include_gtk/lib64/*.so*) $(wildcard include_gtk/lib64/lib64_data/*.so*) $(wildcard include_gtk/lib64/*.a*)
#GTK_CFLAGS :=  -I include_gtk/include/gtk-3.0 -I include_gtk/include/pango-1.0 -I include_gtk/include/glib-2.0 -I include_gtk/lib64/glib-2.0/include -I include_gtk/include/fribidi -I include_gtk/include/cairo -I include_gtk/include/pixman-1 -I include_gtk/include/freetype2 -I include_gtk/include/libpng16 -I include_gtk/include/uuid -I include_gtk/include/harfbuzz -I include_gtk/include/gdk-pixbuf-2.0 -I include_gtk/include/libmount -I include_gtk/include/blkid -I include_gtk/include/gio-unix-2.0/ -I include_gtk/include/libdrm -I include_gtk/include/atk-1.0 -I include_gtk/include/at-spi2-atk/2.0 -I include_gtk/include/at-spi-2.0 -I include_gtk/include/dbus-1.0 -I include_gtk/lib64/dbus-1.0/include -pthread
SRC        := src/code
BUILD      := build/code
BIN        := bin
OBJECTS    := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(wildcard $(SRC)/*.c))
LD_DEBUG := all make

all: dirs bin/XEngl

check:
	echo $(GTK_LIBS)

-include $(BUILD)/*.d

$(BIN)/$(PROJECT): $(OBJECTS)
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

$(BUILD)/%.o: $(SRC)/%.c
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(GTK_CFLAGS) -MMD -c $(FLAGS) $@ $< $(GTK_LIBS) 2>error.log; then \
	printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
	else  \
	printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
	cat error.log; \
	exit 1; \
	fi

dirs:
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Created: " $(BUILD)
	@mkdir -p $(BUILD)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Created: " $(BUILD)
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
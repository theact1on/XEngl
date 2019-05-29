.PHONY: all clean test vocabulary

PROJECT    := XEngl
TEST       := XEngl_test

LIBS_ARC   := https://github.com/luparev22/gtk_lib/raw/master/include_gtk.tar.gz
DEF_DAT    := https://github.com/luparev22/gtk_lib/raw/master/voc.dat

DATA       := data
COMPILER   := gcc
FLAGS      := -fdiagnostics-color=always -std=gnu11 -Werror -o
GTK_LIBS   := -L include_gtk/lib64 -lcairo -lharfbuzz -lthai -lz -lgio-2.0 -latk-1.0 -lgdk-3 -lcairo-gobject -lfribidi -lgtk-3 -lpango-1.0 -lepoxy -lfreetype -lgobject-2.0 -lpcre -lpangocairo-1.0 -lpng16 -lgdk_pixbuf-2.0 -lglib-2.0
GTK_CFLAGS :=  -I include_gtk/lib64/dbus-1.0/include/dbus -I include_gtk/lib64/glib-2.0/include -I include_gtk/include/gtk-3.0 -I include_gtk/include/pango-1.0 -I include_gtk/include/glib-2.0 -I include_gtk/lib64/glib-2.0/include -I include_gtk/include/fribidi -I include_gtk/include/cairo -I include_gtk/include/pixman-1 -I include_gtk/include/freetype2 -I include_gtk/include/libpng16 -I include_gtk/include/uuid -I include_gtk/include/harfbuzz -I include_gtk/include/gdk-pixbuf-2.0 -I include_gtk/include/libmount -I include_gtk/include/blkid -I include_gtk/include/gio-unix-2.0/ -I include_gtk/include/libdrm -I include_gtk/include/atk-1.0 -I include_gtk/include/at-spi2-atk/2.0 -I include_gtk/include/at-spi-2.0 -I include_gtk/include/dbus-1.0 -I include_gtk/lib64/dbus-1.0/include -pthread
#GTK_LIBS   := $(wildcard include_gtk/lib64/*.so*) $(wildcard include_gtk/lib64/lib64_data/*.so*) $(wildcard include_gtk/lib64/*.a*)
#GTK_CFLAGS :=  -I include_gtk/include/gtk-3.0 -I include_gtk/include/pango-1.0 -I include_gtk/include/glib-2.0 -I include_gtk/lib64/glib-2.0/include -I include_gtk/include/fribidi -I include_gtk/include/cairo -I include_gtk/include/pixman-1 -I include_gtk/include/freetype2 -I include_gtk/include/libpng16 -I include_gtk/include/uuid -I include_gtk/include/harfbuzz -I include_gtk/include/gdk-pixbuf-2.0 -I include_gtk/include/libmount -I include_gtk/include/blkid -I include_gtk/include/gio-unix-2.0/ -I include_gtk/include/libdrm -I include_gtk/include/atk-1.0 -I include_gtk/include/at-spi2-atk/2.0 -I include_gtk/include/at-spi-2.0 -I include_gtk/include/dbus-1.0 -I include_gtk/lib64/dbus-1.0/include -pthread
SRCC       := src/code
SRCT       := src/tests
BUILDC     := build/code
BUILDT     := build/tests
BIN        := bin
LIB        := include_gtk
OBJECTSC   := $(patsubst $(SRCC)/%.c,$(BUILDC)/%.o,$(wildcard $(SRCC)/*.c))
OBJECTST   := $(patsubst $(SRCT)/%.c,$(BUILDT)/%.o,$(wildcard $(SRCT)/*.c)) $(BUILDT)/checks.o

all: $(LIB) $(BUILDC) $(BIN) $(DATA) $(BIN)/$(PROJECT)

$(LIB):
	@rm -rf $(LIB).tar.gz
	@printf "\033[0;33m%s \033[0m%s %s" "[WAITING]" " Downloading libs:     "
	@wget --progress=dot $(LIBS_ARC) 2>&1 | grep --line-buffered "%" | \
        sed -u -e "s,\.,,g" | awk '{printf ("\b\b\b\b%4s", $$2)}'
	@printf "\r\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Downloading libs:  100%"
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Installing libs: " $(LIB)
	@tar -xf $(LIB).tar.gz
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Installing libs: " $(LIB)
	@rm -rf $(LIB).tar.gz

vocabulary:
	@rm -rf $(DATA)/voc.dat
	@printf "\033[0;33m%s \033[0m%s %s" "[WAITING]" " Downloading vocabulary:     "
	@wget -P data/ --progress=dot $(DEF_DAT) 2>&1 | grep --line-buffered "%" | \
        sed -u -e "s,\.,,g" | awk '{printf ("\b\b\b\b%4s", $$2)}'
	@printf "\r\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Downloading vocabulary:  100%"

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

test: $(LIB) $(BUILDC) $(BUILDT) $(BIN) $(DATA) $(BIN)/$(TEST)

# Compiling tests

-include $(BUILDT)/*.d

$(BIN)/$(TEST): $(OBJECTST)
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(GTK_CFLAGS) $(FLAGS)  $@ $^ $(GTK_LIBS) 2>error.log; then \
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
	@if $(COMPILER) $(GTK_CFLAGS) -I thirdparty -I src/code -MMD -c $(FLAGS) $@ $< $(GTK_LIBS)  2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		cat error.log; \
		exit 1; \
	fi

$(BUILDT)/checks.o: $(SRCC)/checks.c
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Compiling: " $@
	@if $(COMPILER) $(GTK_CFLAGS) -MMD -c $(FLAGS) $@ $< $(GTK_LIBS)  2>error.log; then \
		printf "\033[0;32m%s \033[0m%s %s\n"  "   [OK]  " " Compiling: " $@; \
	else  \
		printf "\033[0;31m%s \033[0m%s %s\n"  "[FAILING]" " Compiling: " $@; \
		cat error.log; \
		exit 1; \
	fi


$(BUILDC):
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Creating: " $(BUILDC)
	@mkdir -p $(BUILDC)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Creating: " $(BUILDC)

$(BUILDT):
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Creating: " $(BUILDT)
	@mkdir -p $(BUILDT)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Creating: " $(BUILDT)

$(BIN):
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Creating: " $(BIN)
	@mkdir -p $(BIN)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Creating: " $(BIN)

$(DATA):
	@printf "\033[0;33m%s \033[0m%s %s\r" "[WAITING]" " Creating: " $(DATA)
	@mkdir -p $(DATA)
	@printf "\033[0;32m%s \033[0m%s %s\n" "   [OK]  " " Creating: " $(DATA)

clean:
	@printf "\033[0;33m%s \033[0m%s\r" "[WAITING]" " Removing build artifacts."
	@rm -rf build $(BIN)
	@printf "\033[0;32m%s \033[0m%s\n" "   [OK]  " " All assembly artifacts have been removed successfully."
COMPILER   := gcc
FLAGS      := -std=gnu11 -Werror -o
GTK_LIBS   := pkg-config --libs gtk+-3.0
GTK_CFLAGS :=  pkg-config --cflags gtk+-3.0
SRC        := src/code
BUILD      := build/code


all: dirs bin/XEngl

-include $(BUILD)/*.d

bin/XEngl: $(BUILD)/main.o $(BUILD)/stats_win.o $(BUILD)/vocabulary.o $(BUILD)/training.o
	$(COMPILER) `$(GTK_CFLAGS)` $(FLAGS) $@ $^ `$(GTK_LIBS)`

$(BUILD)/main.o: $(SRC)/main.c
	$(COMPILER) `$(GTK_CFLAGS)` -MMD -c $(FLAGS) $@ $< `$(GTK_LIBS)`

$(BUILD)/stats_win.o: $(SRC)/stats_win.c
	$(COMPILER) `$(GTK_CFLAGS)` -MMD -c $(FLAGS) $@ $< `$(GTK_LIBS)`

$(BUILD)/vocabulary.o: $(SRC)/vocabulary.c
	$(COMPILER) `$(GTK_CFLAGS)` -MMD -c $(FLAGS) $@ $< `$(GTK_LIBS)`

$(BUILD)/training.o: $(SRC)/training.c
	$(COMPILER) `$(GTK_CFLAGS)` -MMD -c $(FLAGS) $@ $< `$(GTK_LIBS)`

dirs:
	mkdir -p $(BUILD)
	mkdir -p bin
	mkdir -p data

clean:
	rm -rf build bin
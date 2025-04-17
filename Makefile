# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = 

TARGETS = main primary keepalive dummy

.PHONY: all clean run debug

all: $(TARGETS)

main: main.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

primary: primary.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

keepalive: keepalive.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

dummy: dummy.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: all
	@echo "Starting keep-alive system..."
	@./main

debug: CFLAGS += -g
debug: clean all

clean:
	@echo "Cleaning build files..."
	@rm -f $(TARGETS)
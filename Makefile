OUTPUT_DIR := .

SRCS := ./client/client.c ./server/server.c

TARGETS := $(patsubst %.c,$(OUTPUT_DIR)/%,$(SRCS))

all: $(TARGETS)

$(OUTPUT_DIR)/%: %.c ./utenti/utenti.c ./contatto/contatto.c ./rubrica/rubrica.c
	gcc -o $@ $^

clean:
	rm -rf server
	rm -rf client
	
.PHONY: all clean

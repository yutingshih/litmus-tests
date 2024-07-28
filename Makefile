CFLAGS = -std=c11 -Wall
LDLIBS = -pthread
BIN = \
	message-passing \
	store-buffer \
	indept-read-indept-write \
	coherence \

all: $(BIN)

clean:
	$(RM) $(BIN)

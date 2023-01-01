BIN_FILES  = server client 

CC = g++ -fopenmp -Wall




LDFLAGS = -L$(INSTALL_PATH)/lib/



all: $(BIN_FILES)
.PHONY : all


server: server.o lines.o
	$(CC) $(LDFLAGS) $^  -o $@

client: client.o lines.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^  -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(CFLAGS) -c $< 

clean:
	rm -f $(BIN_FILES) *.o

.SUFFIXES:
.PHONY : clean

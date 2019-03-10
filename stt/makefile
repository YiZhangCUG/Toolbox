CC = g++-8
PROM = stt
CFLAGS = -I.
DEPS = $(shell find ./src -name "*.h")
SRC = $(shell find ./src -name "*.cc")
OBJ = $(SRC:%.cc=%.o)

$(PROM): $(OBJ)
	$(CC) -o $(PROM) $(OBJ) $(CFLAGS) -O2

%.o:%.cc $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) -O2

clean:
	rm -rf $(OBJ)
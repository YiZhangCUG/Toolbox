CC = g++-8 -fopenmp
PROM = /usr/local/sbin/gm3d
PROM2 = ./bin/gm3d
CFLAGS = -I.
DEPS = $(shell find ./src -name "*.h")
SRC = $(shell find ./src -name "*.cpp")
OBJ = $(SRC:%.cpp=%.o)

install: $(PROM)

test: $(PROM2)

$(PROM): $(OBJ)
	$(CC) -o $(PROM) $(OBJ) $(CFLAGS) -O2

$(PROM2): $(OBJ)
	$(CC) -o $(PROM2) $(OBJ) $(CFLAGS) -O2

%.o:%.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) -O2

clean:
	rm -rf $(OBJ)

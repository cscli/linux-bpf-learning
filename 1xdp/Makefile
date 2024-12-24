TARGET = xdp_drop.o

all:
	clang -O2 -target bpf -g -c xdp_drop.c -o $(TARGET)

clean:
	rm -f $(TARGET)
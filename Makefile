CC = mpicc
CFLAGS = -Wall
LDFLAGS = -lm

all: 2dg 2dc 1d

2dg: 2d-general.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
2dc: 2d-corner.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
1d: 1d.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f 2dg 2dc 1d

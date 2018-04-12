head = pctl.h
src = pctl.c
objs = pctl.o
opts = -g -c

all: pctl

pctl: $(objs)
	gcc $(objs) -o pctl

pctl.o: $(src) $(head)
	gcc $(opt) $(src)

clean:
	rm pctl *.o

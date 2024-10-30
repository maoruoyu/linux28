src = $(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src))
#target = $(patsubst %.c,%,$(src))

myArgs = -g -Wall 
lib= -levent

ALL: server

%.o:%.c
	gcc -c $< -o $@ $(myArgs) $(lib)

server:$(obj)
	gcc $^ -o $@ $(myArgs) $(lib)

clean:
	-rm -rf $(obj) server

.PHONY:clean ALL



src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))

myArgs = -g -Wall 

ALL:$(target)

%:%.c
	gcc -c $< -o $@ $(myArgs) 

clean:
	-rm -rf $(obj) server

.PHONY:clean ALL



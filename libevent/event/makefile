src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))

myArgs = -g -Wall -levent

ALL: $(target)

%:%.c
	gcc $< -o $@ $(myArgs)

clean:
	-rm -rf $(target)

.PHONY:clean ALL



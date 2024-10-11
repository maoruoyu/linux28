src=$(wildcard *.c)
target=$(patsubst %.c,%,$(src))
myArgs= -Wall

ALL:$(target) 

%:%.o
	gcc $< -o $@ $(myArgs)

clean:
	-rm -rf $(target)

.PHONY: clean ALL

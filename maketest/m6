src=$(wildcard ./src/*.c) #./src/add.c ,,，./src/sub.c  .. div1.c hello.c
obj=$(patsubst ./src/%.c,./obj/%.o,$(src)) #./obj/add.o .sub.o div1.o hello.o
myargs=-Wall -g
myinc=./inc

ALL:a.out

a.out:$(obj)
	gcc $^ -o $@ $(myargs) 
$(obj):./obj/%.o:./src/%.c
	gcc -c $< -o $@ $(myargs) -I $(myinc)
clean:
	-rm -rf $(obj) a.out
.PHONY:clean ALL

src = $(wildcard ./*.c)
obj = $(patsubst %.c,%.o,$(src))

#obj = main.o add.o sub.o 
app:$(obj)
	gcc $(obj) -o app

%.o:%.c
	gcc -c $< -o $@

clean:
	-rm $(obj) app -f


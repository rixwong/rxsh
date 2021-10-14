all: *.c *.h
	gcc -o rxsh rxsh.c buildin_cmd.c func.c

.PHONY: clean

clean:
	rm -rf *.o rxsh
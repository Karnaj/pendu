CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wfatal-errors -pedantic -pedantic-errors -Wmain -Wmissing-declarations -Wswitch-default -Wunreachable-code -Winit-self -Wshadow -Wwrite-strings -I include

Programme : main.o
	$(CC) main.o -o Programme

main.o : src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

clean :
	del *.o

mrproper : clean
	del Programme.ex

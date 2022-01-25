# Variaveis: Compiler, compiler flags, libraries to link, name of of object files
CC=cc
FLAGS=-c -Wall
LIBS=-lm
OBS= main.o gestorFun.o queue.o linkedList.o

all : gestor

main.o : gestor.h main.c
	$(CC) $(FLAGS) main.c

gestorFun.o : gestor.h gestorFun.c
	$(CC) $(FLAGS) gestorFun.c

queue.o : gestor.h queue.c
	$(CC) $(FLAGS) queue.c

linkedList.o: gestor.h linkedList.c
	$(CC) $(FLAGS) linkedList.c

gestor: $(OBS)
	$(CC) -o gestor $(OBS) $(LIBS)

clean limpar :S
	rm -f gestor *.o
	rm -f *~
	echo "Limpeza dos ficheiros exectuaveis, objectos e gedit tralha"
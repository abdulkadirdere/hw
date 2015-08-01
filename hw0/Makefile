all: hw0

hw0: main.o map.o wc.o 
	g++ main.o -o main
	g++ map.o -o map
	g++ wc.o -o wc

main.o: main.c
	g++ -c main.c -o main.o

map.o: map.c
	g++ -c map.c -o map.o

wc.o: wc.c
	g++ -c wc.c -o wc.o

clean:
	rm -f *o main map wc


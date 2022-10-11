main: main.c lib.a
	gcc -g -o main main.c lib.a

lib.a: mapping.o write.o initLcd.o clear.o
	ar -cvq lib.a mapping.o write.o initLcd.o clear.o

mapping.o: mapping.s
	as -g -o mapping.o mapping.s

write.o: write.s
	as -g -o write.o write.s

initLcd.o: initLcd.s
	as -g -o initLcd.o initLcd.s

clear.o: clear.s
	as -g -o clear.o clear.s

clean:
	rm *.o *.a main
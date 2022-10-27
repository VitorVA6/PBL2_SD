main: main.c lib.a
	gcc -g -o main main.c lib.a

lib.a: mapping.o write_lcd.o initLcd.o clear.o
	ar -cvq lib.a mapping.o write_lcd.o initLcd.o clear.o

mapping.o: mapping.s
	as -g -o mapping.o mapping.s

write_lcd.o: write_lcd.s
	as -g -o write_lcd.o write_lcd.s

initLcd.o: initLcd.s
	as -g -o initLcd.o initLcd.s

clear.o: clear.s
	as -g -o clear.o clear.s

clean:
	rm *.o *.a main
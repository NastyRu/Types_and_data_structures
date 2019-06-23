CC := gcc

app.exe: division.o nules.o number.o main.o
	$(CC) $^ -o $@

%.o: %.c %.h
	$(CC) -c $<

clean:
	rm *.o *.exe

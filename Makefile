OBJECTS = main.o

%.o: %.c
	$(CC) -c $< -o $@

cachetest: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f cachetest

info:
	sudo dmidecode -t cache

run: cachetest
	/usr/bin/time ./cachetest

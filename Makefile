DEPS = proto.h

shell: main.o proto.o
	$(CC) $? -o shell

main.o: main.c
	$(CC) $? -c main.c

proto.o: proto.c
	$(CC) $? -c proto.c

clean:
	rm -f shell *.o

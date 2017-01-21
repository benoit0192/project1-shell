MKDIR_P = mkdir -p
DEPS = proto.h

shell: build_dir main.o proto.o
	$(CC) build/*.o -o build/shell

main.o: main.c
	$(CC) -o build/main.o -c $?

proto.o: proto.c
	$(CC) -o build/proto.o -c $?

build_dir:
	${MKDIR_P} build

run:
	./build/shell

clean:
	rm -r build

MKDIR_P = mkdir -p

shell: build_dir keyboard.o proto.o
	$(CC) build/*.o -o build/shell

keyboard.o: keyboard.c
	$(CC) -o build/keyboard.o -c $?

proto.o: proto.c
	$(CC) -o build/proto.o -c $?

build_dir:
	${MKDIR_P} build

run:
	sudo -S ./build/shell

clean:
	rm -r build

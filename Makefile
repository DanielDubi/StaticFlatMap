CC=gcc-8
CXX=g++-8
GENERATOR=Ninja


build: debug release

.PHONY: run
run: release
	./build/release/bench/bench

.PHONY: test
test: debug
	./build/debug/test/unittest

.PHONY: release
release: build/release
	ninja -C build/release

.PHONY: debug
debug: build/debug
	ninja -C build/debug

build/release:
	mkdir -p build/release
	CC=$(CC) CXX=$(CXX) cmake -G$(GENERATOR) -DCMAKE_BUILD_TYPE=Release -Bbuild/release -H.

build/debug:
	mkdir -p build/debug
	CC=$(CC) CXX=$(CXX) cmake -G$(GENERATOR) -DCMAKE_BUILD_TYPE=Debug -Bbuild/debug -H.

.PHONY: fullclean
fullclean:
	rm -rf build/

.PHONY: clean
clean:
	ninja -C build/debug clean
	ninja -C build/release clean

#	Virtual bass plugin for DeaDBeeF
#   Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
#   The MIT License (MIT)

CFLAGS += -std=c99 -fPIC -Wall -Werror -g

virtual_bass.so: virtual_bass.o
	gcc -shared -fPIC -O2 $^ -o $@

virtual_bass_test: virtual_bass_test.o virtual_bass.so
	gcc -O2 $^ -o $@

all: virtual_bass.so virtual_bass_test

#	Virtual bass plugin for DeaDBeeF
#   Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
#   The MIT License (MIT)

all:
	gcc -I/usr/local/include  -std=c99 -shared -O2 -o virtual_bass.so virtual_bass.c -fPIC

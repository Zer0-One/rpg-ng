#
# Makefile for rpg-ng
#

cc = clang

.PHONY : analyze clean

rpg-ng : bin/rpg-ng
rpg-ng-debug : bin/rpg-ng_debug

bin/rpg-ng : src/*.c include/*.h
	mkdir -p bin
	$(cc) -O3 -fstack-protector-strong -std=c11 -pedantic-errors -Wall -Wextra -D_FORTIFY_SOURCE=2 -D_XOPEN_SOURCE=700 -I include/ -lSDL2 -lSDL2_image -ljansson src/*.c -o $@

bin/rpg-ng_debug : src/*.c include/*.h
	mkdir -p bin
	$(cc) -g3 -fstack-protector-strong -std=c11 -pedantic-errors -Wall -Wextra -D_XOPEN_SOURCE=700 -I include/ -lSDL2 -lSDL2_image -ljansson src/*.c -o $@
	chmod +x $@

analyze :
	mkdir -p bin
	scan-build -v -o analysis $(cc) -g3 -fstack-protector-strong -std=c11 -pedantic-errors -Wall -Wextra -D_XOPEN_SOURCE=700 -I include/ -lSDL2 -lSDL2_image -ljansson src/*.c -o bin/rpg-ng
	chmod +x bin/rpg-ng

clean :
	-rm -r bin

main_01: main_01.c functions.c functions.h
	clang main_01.c functions.c -o bin/main_01 -Wall -lm

main_02: main_02.c functions.c functions.h
	clang main_02.c functions.c -o bin/main_02 -Wall -lm

main_03: main_03.c functions.c functions.h
	clang main_03.c functions.c -o bin/main_03 -Wall -lm

main_04: main_04.c functions.c functions.h
	clang main_04.c functions.c -o bin/main_04 -Wall -lm

main_05: main_05.c functions.c functions.h
	clang main_05.c functions.c -o bin/main_05 -Wall -lm

main_06: main_06.c functions.c functions.h
	clang main_06.c functions.c -o bin/main_06 -Wall -lm

main_07: main_07.c functions.c functions.h
	clang main_07.c functions.c -o bin/main_07 -Wall -lm -lcrypto

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	// Check args
	if (argc < 2) {
		printf("No input filename provided.\n");
		return 1;
	}

	if (argc > 2) {
		printf("Too many arguments.\n");
		return 1;
	}

	FILE* in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("Couldn't open input file: %s\n", argv[1]);
		return 1;
	}

	char code[65536];
	int len = fread(code, 1, sizeof(code), in);

	fclose(in);

	unsigned char tape[65536] = {0};
	int pc = 0;
	int ptr = 0;

	while (pc < len) {
		switch (code[pc]) {
			case '>':
				ptr = (ptr + 1) % sizeof(tape);
				break;

			case '<':
				ptr = (ptr + sizeof(tape) - 1) % sizeof(tape);
				break;

			case '+':
				tape[ptr]++;
				break;

			case '-':
				tape[ptr]--;
				break;

			case '.':
				putchar(tape[ptr]);
				break;

			case ',':
				tape[ptr] = getchar();
				break;

			case '[':
				if (!tape[ptr]) {
					int depth = 1;
					while (depth && ++pc < len) {
						if (code[pc] == '[') depth++;
						if (code[pc] == ']') depth--;
					}
				}
				break;

			case ']':
				if (tape[ptr]) {
					int depth = 1;
					while (depth && --pc >= 0) {
						if (code[pc] == ']') depth++;
						if (code[pc] == '[') depth--;
					}
					pc--;
				}
				break;
		}
		pc++;
	}
	return 0;
}
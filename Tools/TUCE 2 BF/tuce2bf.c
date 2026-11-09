#include <stdio.h>

#define I1 0b00000001
#define I2 0b00000010
#define I3 0b00000100
#define I4 0b00001000
#define I5 0b00010000
#define I6 0b00100000
#define I7 0b01000000
#define I8 0b10000000

int main(int argc, char** argv) {
	// Check args
	if (argc < 2) {
		printf("No input filename provided.\n");
		return 1;
	}

	if (argc < 3) {
		printf("No output filename provided.\n");
		return 1;
	}

	if (argc > 3) {
		printf("Too many arguments.\n");
		return 1;
	}

	// Open and create the files
	FILE* in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("Couldn't open input file: %s\n", argv[1]);
		return 1;
	}

	FILE* out = fopen(argv[2], "wb");
	if (out == NULL) {
		printf("Couldn't open/create output file: %s\n", argv[2]);
		return 1;
	}

	// Translate
	int ch = 0;
	while (ch != EOF) {
		ch = fgetc(in);

		switch (ch) {
		case I1: fputc('>', out); break;
		case I2: fputc('<', out); break;
		case I3: fputc('+', out); break;
		case I4: fputc('-', out); break;
		case I5: fputc('.', out); break;
		case I6: fputc(',', out); break;
		case I7: fputc('[', out); break;
		case I8: fputc(']', out); break;
		default: break; // Ignore anything that isn't brainfuck.
		}
	}
	
	// Cleanup
	fclose(out);
	fclose(in);

	return 0;
}
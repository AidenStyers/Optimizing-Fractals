#include <stdio.h>

extern "C" {
	int square(int i) {
		return i * i;
	}
}
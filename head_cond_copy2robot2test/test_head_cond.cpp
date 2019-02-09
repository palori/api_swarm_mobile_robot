#include "header_conditionals.h"

int main()
{
	char * msg = new char[1];
	msg[0] = 'P';
	msg = func4(msg);
	printf("\n\n\nprinting %s!!!\n\n\n", msg);

	delete[] msg;
	func3();
	return 0;
}
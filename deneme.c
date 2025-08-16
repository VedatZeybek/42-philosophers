typedef union selam
{
	int a;
	struct
	{
		unsigned char b;
		unsigned char c;
		unsigned char d;
		unsigned char e;
	};
}	u_selam;

#include "stdio.h"

int main()
{
	u_selam a1;

	a1.a = 235;
	printf("a: %d\n", a1.a);
	printf("b: %d\n", a1.b);
	printf("c: %d\n", a1.c);
	printf("d: %d\n", a1.d);

}

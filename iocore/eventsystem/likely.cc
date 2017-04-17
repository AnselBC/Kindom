#include <stdio.h>
#include <stdlib.h>

int
main()
{
	int a = 3;
	if (unlikely(a == 3)) {
		printf("a  == %d\n", a);
	} else {
		 printf("a  != %d\n", a);
	}
}

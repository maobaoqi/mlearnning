#include <stdio.h>
#include "ipp.h"



int main(void)
{
	const Ipp8u string[] = "Hello World! fafha Wo adfadWo jaodiwo World";
	const char pattern[] = "a";
	IppRegExpFind find[5] = { 0 };

	int numFind = sizeof(find);
	int numFind0 = sizeof(find[0]);

	int find_num = numFind / numFind0;
	IppStatus status = ippStsNoErr;
	int i = 0, errOffset = 0;
	IppRegExpState *pRegExpState = NULL;
	int regExpStateSize = 0;

	status = ippsRegExpGetSize(pattern, &regExpStateSize);

	pRegExpState = (IppRegExpState*)ippMalloc(regExpStateSize);

	status = ippsRegExpInit(pattern, NULL, pRegExpState, &errOffset);

	status = ippsRegExpFind_8u(string, sizeof(string) - 1, pRegExpState, find, &find_num);

	//status = ippsReg

	printf("Found strings number = %d\n", find_num);

	IppRegExpFind test;

	for (i = 0; i < find_num; i++) 
	{
		Ipp8u tmp = ((Ipp8u*)find[i].pFind)[find[i].lenFind];
		((Ipp8u*)find[i].pFind)[find[i].lenFind] = 0;
		printf("%d: %s\n", i, (char*)find[i].pFind);
		((Ipp8u*)find[i].pFind)[find[i].lenFind] = tmp;
	}

	
	printf("Exit status %d (%s)\n", (int)status, ippGetStatusString(status));
	return (int)status;
}

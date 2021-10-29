#include<stdio.h>
#include<iostream>
#include<time.h>
#include<stdlib.h>

void ResetRandomNumberGenerator(void)
{
	srand((unsigned int)time(nullptr));
}

int GenerateRandomNumber(void)
{
	int r = rand() % 1000;
	return r;
}

int TakeInput(int r)
{
	int u;
	printf("\nType %d: ", r);
	scanf("%d", &u);
	return u;
}

bool CompareNumber(int r, int u)
{
	bool result;
	if (r == u)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}
time_t Countdown()
{
	time_t a = (unsigned int)time(NULL);
	int b = int(difftime((unsigned int)time(NULL), a));
	int count1 = 0; int count2 = 0; int count3 = 0; int count4 = 0;

	while(b <= 3)
	{
		b = int(difftime((unsigned int)time(NULL), a));
		if (b == 0 && count1 < 1) { printf("\n3"); count1 += 1; }
		else if (b == 1 && count2 < 1) { printf("\n2"); count2 += 1;}
		else if (b == 2 && count3 < 1) { printf("\n1"); count3 += 1;}
		else if (b == 3 && count4 < 1) { printf("\nGo! \n"); count4 += 1; }
		else if (b == 0 || b == 1 || b == 2 || b == 3 || count1 > 1 || count2 > 1 || count3 > 1 || count4 > 1) { continue; }
		else { break; }
	}
	return NULL;
}


int main(void) 
{
	ResetRandomNumberGenerator();
	printf("Fast Number Typing Game: \n Enter 10 correct numbers as fast as you can! \n");
	Countdown();
	int t = int((unsigned int)time(NULL));
	int count = 1;
	while(count<11)
	{
		int r = GenerateRandomNumber();
		time_t t;
		t = (unsigned int)time(NULL);
		int u = TakeInput(r);
		if (CompareNumber(r, u) == true)
		{
			count += 1;
			continue;
		}
		else
		{
			printf("\n Incorrect! Try again!");
		}
	}
	t = int(difftime((unsigned int)time(NULL), t));
	printf("Elapsed %d seconds", t);
	getchar();
	getchar();
	
	return 0;
}
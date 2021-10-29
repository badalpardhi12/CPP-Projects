#include<stdio.h>
#include<iostream>
#include<time.h>
#include<stdlib.h>

void ResetRandomNumberGenerator(void)
{
	srand((unsigned int)time(nullptr));
}

int GenerateRandomNumber(int n)
{
	int r = rand() % n;
	return r;
}

int NumberOfCards(void)
{
	int n;
	while (true)
	{
		printf("\n\nEnter the number of cards you want to work with : ");
		scanf("%d", &n);
		if (n < 145 && n>0) 
		{
			break;
		}
		else if (n <= 0 || n>145) 
		{
			printf("The number of cards must be between 1 and 144.");
		}

	}
	return n;
}

// Questionaire Function
int FlashCard(int n)
{
	int Answers[144];
	int QuesNo = 0;
	int CorrectAnswer = 0;
	while (QuesNo < n)
	{
		int size = sizeof(Answers) / sizeof(Answers[0]);
		int a = rand() % 12 + 1;
		int b = rand() % 12 + 1;
		printf("\n %d x %d = ", a, b);
		// Taking answer input from user
		int ans;
		scanf("%d", &ans);
		auto num = std::find(Answers, Answers + size, a * b);
		if (ans == a * b && num == std::end(Answers))
		{
			Answers[QuesNo] = a * b;
			++CorrectAnswer;
			++QuesNo;
		}
		else if (ans != a * b)
		{
			printf("\n The correct answer is: %d ", a * b);
			++QuesNo;
		}
	}
	return CorrectAnswer;
}

int main(void)
{
	ResetRandomNumberGenerator();
	//Correct answer
	int CorrectAnswer;

	// Taking inout from learner
	int n = NumberOfCards();

	// Asking Questions and counting time
	int t = int((unsigned int)time(NULL));
	CorrectAnswer = FlashCard(n);
	t = int(difftime((unsigned int)time(NULL), t));

	printf("\n\nYou answered %d questions in %d seconds.",n,t);
	printf("\n\nYou answered %d problems correctly (%d%%).\n\n\n\n\n",CorrectAnswer, (CorrectAnswer*100/n));

	getchar();
	getchar();

	return 0;
}
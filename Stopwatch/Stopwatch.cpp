#include <stdio.h>
#include <chrono>

//Chrono library functions were refered from "www.cplusplus.com/reference/chrono"

int main(void) {
	using namespace std::chrono;

	printf("24-780 Engineering Computation Problem Set 1-3 - Stopwatch");
	printf("\nPress the ENTER key to start timer >");
	
	duration<double> time_passed;
	
	if (getchar() == '\n') {
		
		steady_clock::time_point t1 = steady_clock::now();
		
		printf("Press the ENTER key to end timer >");
		
		if (getchar() == '\n') {
			
			steady_clock::time_point t2 = steady_clock::now();
			
			time_passed = duration_cast<duration<double>>(t2 - t1);
			
			printf("Time elapsed: %lf \n\n\n\n", time_passed.count());
		}
		else {
			printf("You pressed an incorrect key, run the program again and press ENTER to stop.\n\n\n\n");
		}
	}
	else {
		printf("You pressed an incorrect key, run the program again and press ENTER to start.\n\n\n\n");
	}

	getchar();
	return 0;
}

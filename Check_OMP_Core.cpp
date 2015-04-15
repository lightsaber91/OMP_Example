#include <iostream>
#include <omp.h>

using namespace std;

int main(void) {

	int num_thread;

	num_thread = omp_get_max_threads();
	cout << "Max Thread on this CPU: " << num_thread << endl;

	#pragma omp parallel 
	{
		#pragma omp critical
		{
			cout << "Hello World\n";
		}
	}
	return 0;
}


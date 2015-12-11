#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// prototype methods
void pmergeSort(int *a, int min, int max);
void mergeSort(int *a, int min, int max);
void merge(int *a, int min, int max, int mid);

// global variables
int compare;
int swap;

//main
int main() {
	string line;
	string command;
	string name;
	bool exit = false;
	bool sorted = false;
	bool start = false;	
	int size = 0;
	clock_t t1, t2;

	while (!exit)
	{	
		ifstream file;

		int *array;
		
		getline(cin, line);
	
		command = line.substr(0, line.find(' ')); // command
		name = line.substr(line.find(' ') + 1); // name
		
		if (command.compare("Start") == 0 && name.length() <= 20){	
			int i = 0;
			cout << "Processing fitness data of: " << name << endl;
			file.open((name + ".txt").c_str()); // For some reason my compiler requires c_str()

			while (getline (file, line)) {
				size++;
			}
			file.close();
			array = new int[size];
		
			//Debugging: cout << "The size of the array is " << size << endl;
			//Dubugging: cout << "Now copying the contents of the file" << endl;
		
			file.open((name + ".txt").c_str());
			while (getline ( file, line)) {
				array[i] = atoi(line.c_str()); // copy line into position index i
				i++;
			}		
			start = true;
			file.close();
		}
		else if (command.compare("End") == 0) {
			cout << "End of processing fitness data for: " << name << endl;
			delete [] array;

			line.clear();
			command.clear();
			name.clear();
			exit = false;
			sorted = false;
			start = false;	
			size = 0;
		}
		else if (command.compare("Exit") == 0){ 
			cout << "Program Terminating" << endl; 
			exit = true;
			
		}
		else if (command.compare("InsertionSort") == 0 && start){
			t1 = clock();	// initial time
			int compare = 0;
			int swap = 0;

			/****start insertion sort****/
			for (int j = 1; j < size; j++){
				
				int key = array[j];
				int i = j - 1;
				
				while (i >= 0 && array[i] > key) {
					array[i+1] = array[i];
					i = i -1;
					compare++;
					swap++;
				}
				array[i+1] =  key;
				compare++;
			}
			/****end insertion sort******/
			
			t2 = clock(); // final time
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			cout << "Number of comparisons made by Insertion Sort: " << compare << endl;
			cout << "Number of swaps made by Insertion Sort: " << swap << endl;
			cout << "Time to run Insertion Sort (ms): " << ms << endl;
			sorted = true;
		}
		else if (command.compare("Average") == 0 && start){
			t1 = clock();
			double avg = 0;
			double sum = 0;
			sum = array[0]; // do the first
			
			for (int i = 1; i < size; i++){
				sum += array[i];
			}
			avg = sum/size;
			cout << "Average number of steps: " << avg << endl;

			t2 = clock();
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			cout << "Time to run the average (ms): " << ms << endl;
		}
		else if (command.compare("Select") == 0 && sorted){
			bool select = true;
			t1 = clock();
			// Start Select
			if (name.compare("min") == 0)
				cout << "Selecting item: " << array[0] << endl;
			else if (name.compare("max") == 0) 
				cout << "Selecting item: " << array[size - 1] << endl;
			else if (name.compare("median") == 0){
				int mid = size/2;
				cout << "Selecting item: " << array[mid] << endl;
			}
			else if (atoi(name.c_str()) >= 0 && atoi(name.c_str()) < size){
				cout << "Selecting item: " << array[atoi(name.c_str())] << endl;
			}
			else{
				cout << "Invalid selection" << endl;
				select = false;
			}
			// End Select
			
			t2 = clock();
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			
			if (select)
				cout << "Time to run the selection (ms): " << ms << endl;
			
		}
		else if (command.compare("Select") == 0 && !sorted)
			cout << "Unable to select from an unsorted array." << endl;
		else if (command.compare("MergeSort") == 0) {
			::compare = 0;
			::swap = 0;
			t1 = clock();
			mergeSort(array, 0, size - 1);
			t2 = clock();
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			cout << "Time to run merge sort (ms): " << ms << endl;
			cout << "Number of comparisons made by merge sort: " << ::compare << endl;
			cout << "Number of swaps made by merge sort: " << ::swap << endl;
		}
		else if (command.compare("ParallelAverage") == 0 && start){
			t1 = clock();
			double avg = 0;
			double sum = 0;
			sum = array[0]; // do the first
			int i;
			#pragma omp parallel for default(none) shared(size,array) reduction(+:sum) num_threads(2)
			for (i = 1; i < size; i++){
				sum += array[i];
			}
			avg = sum/size;
			cout << "Average number of steps (parallel sum): " << avg << endl;

			t2 = clock();
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			cout << "Time to run the average (parallel sum) (ms): " << ms << endl;
		}	
		else if (command.compare("ParallelMergeSort") == 0) {
			::compare = 0;
			::swap = 0;
			t1 = clock();
			pmergeSort(array, 0, size - 1);
			t2 = clock();
			float diff((float)t2 - (float)t1);
			float seconds = diff/CLOCKS_PER_SEC;
			float ms = seconds * 1000;
			cout << "Time to run parallel merge sort (ms): " << ms << endl;
			cout << "Number of comparisons made by parallel merge sort: " << ::compare << endl;
			cout << "Number of swaps made by parallel merge sort: " << ::swap << endl;
		}	
		else
			cout << "Run Start first, filename too long, or Command not recognized" << endl;
	}
	
	return 0;
}

void mergeSort(int *a, int min, int max){
    int mid;
    if (min < max)
    {
        mid=(min+max)/2;
        mergeSort(a,min,mid);
        mergeSort(a,mid+1,max);
        merge(a,min,max,mid);
    }
    return;
}
void pmergeSort(int *a, int min, int max){
    int mid;
    if (min < max)
    {
        mid=(min+max)/2;
	#pragma omp parallel num_threads(4)
	{
        pmergeSort(a,min,max);
        pmergeSort(a,mid+1,max);
	}
        merge(a,min,max,mid);
    }
    return;
}
void merge(int *a, int min, int max, int mid){
    int i, j, k, c[max];
    i = min;
    k = min;
    j = mid + 1;
    
    ::compare++;
    while (i <= mid && j <= max){
        if (a[i] < a[j]){
            c[k] = a[i];
            k++;
            i++;
	    ::compare++;
        }
        else{
            c[k] = a[j];
            k++;
            j++;
	    ::swap++;
	    ::compare++;
        }
    } // end while

    while (i <= mid){
        c[k] = a[i];
        k++;
        i++;
    }
    while (j <= max){
        c[k] = a[j];
        k++;
        j++;
    }
    for (i = min; i < k; i++){
        a[i] = c[i];
    }
}




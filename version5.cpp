/* kthSmallest(arr[0..n-1], k)
    1) Divide arr[] into ⌈n/g⌉ groups where size of each group is g
    except possibly the last group which may have less  than g elements.

    2) Sort the above created ⌈n/g⌉ groups and find median of all groups.
    Create an auxiliary array 'median[]' and store medians of all ⌈n/g⌉ groups in this median array.

    // Recursively call this method to find median of median[0..⌈n/g⌉-1]
    3) medOfMed = kthSmallest(median[0..⌈n/g⌉-1], ⌈n/2g⌉)

    4) Partition arr[] around medOfMed and obtain its position.
        pos = partition(arr, n, medOfMed)

    5) If pos == k return medOfMed
    6) If pos < k return kthSmallest(arr[l..pos-1], k)
    7) If poa > k return kthSmallest(arr[pos+1..r], k-pos+l-1)
 * */
#include <iostream>
#include <algorithm>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <chrono>

//using namespace std;

template <typename T>
void swap(T &a, T &b){
    T c(std::move(a)); a=std::move(b); b=std::move(c);
}

template <typename T>
void printArray(const T *A, T start, T end){
    std::cout << std::endl;
    for(int i=start; i<=end; i++) {
        std::cout << *(A+i) << "  "; //A[i]
        if(i%5 == 4 && i > 1) std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
void randomInRange(T *A, T start, T end){
    for(T i = start; i <= end; i++)
        *(A+i) = i; //or can use A[i] = i

    //Create random pos in the array
    T n = end - start+1;
    for(T i = start; i <= end/2; i++) {
        T pos1 = rand() % n;
        T pos2 = rand() % n;
        if (pos1 != pos2) swap(A[pos1], A[pos2] );
    }
}

// It searches for x in arr[l..r], and partitions the array around x.
template <typename T>
T partition(T *arr, T left, T right, T x){
    // Search for x in arr[l..r] and move it to end
    T i;
    for (i=left; i<right; i++)
        if ( *(arr+i) == x)
            break;
    swap(arr[i], arr[right]);

    /* Standard partition algorithm
     * Move from start to end of the sequence. If you reach an element smaller than x*/
    T index = left; //index is the lowest position of the element greater than x
    for (T j = left; j <= right - 1; j++) {
        if ( *(arr+j) <= x){ //Find j is the position where the element smaller or equal x
            if (index!=j)
                swap(arr[index], arr[j]);//or use: swap(*(arr+index), *(arr+j));
            index++; // Move to next element which is greater than x
        }
    }
    swap(*(arr+index), *(arr+right));
    return index;
}

// A simple function to find median of arr[].  This is called only for an array of size g in this program.
template <typename T1, typename T2>
T1 findMedian(T1 *arr, T2 n){
    std::sort(arr, arr+n);  // Sort the array
    return *(arr+ n/2);   // Return middle element
}

// Returns k'th smallest element in arr[l..r] in worst case
// linear time. ASSUMPTION: ALL ELEMENTS IN ARR[] ARE DISTINCT
template <typename T>
T kthSmallest(T *arr, T left, T right, T k, T g){
    // If k is smaller than number of elements in array
    if (k > 0 && k <= right - left + 1){
        T n = right-left+1; // Number of elements in arr[l..r]

        // Divide arr[] in groups of size g, calculate median
        // of every group and store it in median[] array.
        T i, *median = new T[(n+g-1)/g]; // There will be floor((n+g-1)/g) groups;
        for (i=0; i<n/g; i++)
            *(median+i) = findMedian(arr+left+i*g, g);
        if (i*g < n){ //For last group with less than g elements
            *(median+i) = findMedian(arr+left+i*g, n%g);
            i++;
        }

        /* Find median of all medians using recursive call.
           If median[] has only one element, then no need of recursive call*/
        T medOfMed = (i == 1)? *(median+i-1): kthSmallest(median, (T) 0, i-1, i/2, g);
        delete median;
        /* Partition the array around a random element and
           get position of pivot element in sorted array*/
        T pos = partition(arr, left, right, medOfMed);

        // If position is same as k
        if (pos-left == k-1)    return *(arr+pos);
        if (pos-left > k-1)  // If position is more, recur for left
            return kthSmallest(arr, left, pos-1, k, g);

        // Else recur for right sub-array
        return kthSmallest(arr, pos+1, right, k-pos+left-1, g);
    }

    // If k is more than number of elements in array
    return INT_MAX;
}


//======================================================================//
int main(){
    std::fstream fs;
    fs.open("result.txt", std::ios::app);
    long long M[8] = {10'000, 100'000, 1'000'000, 5'000'000, 10'000'000, 50'000'000, 100'000'000, 200'000'000};
//    long long M[15] = {10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000};
    long long m = 10'000, ss=0;
//    long long *B = new long long[m];
    //long long g = 30;
    for(long long g=3; g <= 50; g++){
        for(int j=0; j<8; j++){
            long long m = M[j], ss=0;
            long long *B = new long long[m];
            randomInRange(B, ss, m);
            long long k = m / 3;

            // Starting count time
            auto start = std::chrono::high_resolution_clock::now();
            long long pos = kthSmallest(B, ss, m, k, g);
            std::cout <<"g = " << g << ", m = " << M[j] << ", k = "<< k << "'th is: " << pos << "\n";

            // Record end time
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s\n\n";
            fs << elapsed.count() << '\t';
            delete [] B;
        }
        fs << '\n';
    }
    fs.close();
	return 0;
}

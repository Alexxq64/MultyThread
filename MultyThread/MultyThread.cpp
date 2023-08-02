#include "MultyThread.h"

// function to execute in each thread
void calculateSum(const std::vector<int>& array, int startIndex, int endIndex, long long& partialSum) {
	partialSum = std::accumulate(array.begin() + startIndex, array.begin() + endIndex, 0LL);
}

double threadCalculation(int M, int N, std::vector<int>& array, long long& totalSum)
{
	// time to start
	auto start = std::chrono::high_resolution_clock::now();

	// vector to save results of perticial summation
	std::vector<long long> partialSums(M, 0);

	// creating and running threads
	std::vector<std::thread> threads;
	int chunkSize = N / M;
	for (int i = 0; i < M; ++i) {
		int startIndex = i * chunkSize;
		int endIndex = (i == M - 1) ? N : (i + 1) * chunkSize;

		threads.emplace_back(calculateSum, std::ref(array), startIndex, endIndex, std::ref(partialSums[i]));
	}

	// waiting for threads to complete
	for (auto& thread : threads) {
		thread.join();
	}

	// calculating the total
	totalSum = std::accumulate(partialSums.begin(), partialSums.end(), 0LL);

	// time to finish
	auto end = std::chrono::high_resolution_clock::now();
	// calculating the execution time
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << std::setw(4) << M << std::setw(18) << std::fixed << std::setprecision(0) << duration.count();
	return (double)duration.count();
}

void tests(int N, int numCores)
{
	std::cout << "\nSummation of " << N << " random numbers between " << LOW_BOUND << " and " << UPPER_BOUND << std::endl;
	std::cout << "threads      microseconds       ratio to one thread" << std::endl;
	// creating a vector of random numbers
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(LOW_BOUND, UPPER_BOUND);
	std::vector<int> array(N);
	for (int i = 0; i < N; ++i) {
		array[i] = dis(gen);
	}
	int basis = 1;
	int time = basis;
	int dotsNumber = 0;
	long long total;

	// greating a vector of treads number for pivot table based on number of cores
	std::vector<int> threads;
	for (int i = 1; i < (int)(numCores * 0.7); i = i + 3) threads.push_back(i);
	for (int i = (int)(numCores * 0.7); i < numCores * 1.3; i++) threads.push_back(i);
	for (int i = (int)(numCores * 1.3); i < numCores * 2; i = i + 3) threads.push_back(i);
	for (auto i:threads) {
		time = (int)threadCalculation(i, N, array, total);
		// as the basis for the ratio is chosen the execution time by one thread
		if (i == 1) basis = time;
		// number of dots calculation
		dotsNumber = BASIS_LENGTH * time / basis;
		// limiter
		if (dotsNumber > 90) dotsNumber = 70;
		std::cout << "          " << std::string(dotsNumber, '.') << std::endl;
	}
	std::cout << "the sum is: " << total << std::endl;
}

int main() {

	int numCores = std::thread::hardware_concurrency();

	if (numCores == 0) {
		std::cout << "Unable to determine the number of cores." << std::endl;
	}
	else {
		std::cout << "Number of processor cores: " << numCores << std::endl;
	}

	std::cout.imbue(std::locale("en_US"));

	tests(100000, numCores);
	tests(1000000, numCores);
	tests(10000000, numCores);
	tests(50000000, numCores);

	return 0;
}

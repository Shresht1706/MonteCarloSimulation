#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

void countPointsInsideCircle(int numPoints, int& localCount) {
    localCount = 0;
    for (int i = 0; i < numPoints; ++i) {
        double x = static_cast<double>(rand()) / RAND_MAX; // generate random x coordinate between 0 and 1
        double y = static_cast<double>(rand()) / RAND_MAX; // generate random y coordinate between 0 and 1

        // check if the point is inside the unit circle
        if (x * x + y * y <= 1) {
            ++localCount;
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // seed the random number generator

    int NUM_POINTS = 100000000;  // number of random points to generate
    cout << "Number of points to test = " << NUM_POINTS << endl;

    auto start_time = chrono::high_resolution_clock::now(); // start timing

    // Determine the number of available hardware threads
    int numThreads = thread::hardware_concurrency();

    cout << "Number of threads = " << numThreads << endl;

    vector<thread> threads;
    vector<int> localCounts(numThreads, 0); // vector to store local counts for each thread
    int pointsPerThread = NUM_POINTS / numThreads;
    int remainingPoints = NUM_POINTS % numThreads;

    // Create threads
    for (int i = 0; i < numThreads; ++i) {
        int numPoints = pointsPerThread + (i == 0 ? remainingPoints : 0); // add remaining points to the first thread
        threads.push_back(thread(countPointsInsideCircle, numPoints, ref(localCounts[i])));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Sum up the results from all threads
    int pointsInsideCircle = 0;
    for (int count : localCounts) {
        pointsInsideCircle += count;
    }

    auto end_time = chrono::high_resolution_clock::now(); // stop timing

    // Estimate pi using the ratio of points inside the circle to total points
    double piEstimate = 4.0 * pointsInsideCircle / NUM_POINTS;

    cout << "Estimated value of pi: " << piEstimate << endl;

    // Print processing time
    chrono::duration<double> processing_time = (end_time - start_time);
    cout << "Processing time: " << processing_time.count() * 1000 << " ms " << endl;

    // Calculate and print percentage error
    double percentageError = ((piEstimate / 3.14159265) * 100) - 100;
    cout << "Percentage Error: " << percentageError << "%" << endl;

    return 0;
}

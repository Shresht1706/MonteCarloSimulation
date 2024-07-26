#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>
#include <fstream>

using namespace std;

void estimatePi(int numPoints, int numThreads, ofstream& csvFile) {
    int pointsInsideCircle = 0;

    auto start_time = chrono::high_resolution_clock::now(); // start timing

#pragma omp parallel num_threads(numThreads)
    {
        // Get the thread number for unique seeding
        int thread_num = omp_get_thread_num();
        std::mt19937 gen(thread_num); // Initialize with unique seed based on thread number
        std::uniform_real_distribution<> dis(0.0, 1.0);

        int localCount = 0;

#pragma omp for
        for (int i = 0; i < numPoints; ++i) {
            double x = dis(gen); // generate random x coordinate between 0 and 1
            double y = dis(gen); // generate random y coordinate between 0 and 1

            // check if the point is inside the unit circle
            if (x * x + y * y <= 1) {
                ++localCount;
            }
        }

#pragma omp atomic
        pointsInsideCircle += localCount;
    }

    auto end_time = chrono::high_resolution_clock::now(); // stop timing

    // Estimate pi using the ratio of points inside the circle to total points
    double piEstimate = 4.0 * pointsInsideCircle / numPoints;

    // Print results
    cout << "Number of threads: " << numThreads << endl;
    cout << "Estimated value of pi: " << piEstimate << endl;

    // Print processing time
    chrono::duration<double> processing_time = (end_time - start_time);
    cout << "Processing time: " << processing_time.count() * 1000 << " ms " << endl;

    // Calculate and print percentage error
    double percentageError = ((piEstimate / 3.14159265) * 100) - 100;
    cout << "Percentage Error: " << percentageError << "%" << endl << endl;

    // Write the results to the CSV file
    csvFile << numThreads << "," << piEstimate << "," << processing_time.count() * 1000 << "," << percentageError << "\n";
}

int main() {
    int NUM_POINTS = 100000000;  // number of random points to generate
    cout << "Number of points to test = " << NUM_POINTS << endl;

    int threadCounts[] = {1, 2, 4, 8};

    // Open the CSV file in append mode
    ofstream csvFile("runtime.csv", ios::app);
    if (csvFile.is_open()) {
        for (int numThreads : threadCounts) {
            estimatePi(NUM_POINTS, numThreads, csvFile);
        }

        csvFile.close();
        cout << "Results appended to runtime.csv" << endl;
    } else {
        cerr << "Unable to open file for writing" << endl;
    }

    return 0;
}

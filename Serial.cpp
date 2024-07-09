#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

int main() {
    srand(time(0)); // seed the random number generator

    int NUM_POINTS = 100000000;  // number of random points to generate

    int pointsInsideCircle = 0;

    cout << "Number of points to test = " << NUM_POINTS << endl;


    auto start_time = chrono::high_resolution_clock::now(); // start timing

    for (int i = 0; i < NUM_POINTS; i++) {
        double x = (double)rand() / RAND_MAX; // generate random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX; // generate random y coordinate between 0 and 1

        // check if the point is inside the unit circle
        if (x * x + y * y <= 1) {
            pointsInsideCircle++;
        }
    }

    auto end_time = chrono::high_resolution_clock::now(); // stop timing

    // estimate pi using the ratio of points inside the circle to total points
    double piEstimate = 4.0 * pointsInsideCircle / NUM_POINTS;

    cout << "Estimated value of pi: " << piEstimate << endl;

    // print processing time
    chrono::duration<double> processing_time = (end_time - start_time);
    cout << "Processing time: " << processing_time.count() * 1000 << " ms " << endl;
    double percentageError = ((piEstimate / 3.14159265) * 100) - 100;
    cout << "Percentage Error: " << percentageError << "%" << endl;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_FEATURES 16   // not including animal name and class label
#define NUM_SAMPLES 100      // total number of samples / rows in the given zoo dataset
#define NUM_CLASSES 7        // total number of class labels in zoo dataset
#define NUM_TEST_DATA 20     // total number of samples / rows in the test dataset - you need this for task 9
#define MAX_LENGTH_ANIMAL_NAME 50

struct Animal {
   
   char animalName [MAX_LENGTH_ANIMAL_NAME];
   int features [NUM_FEATURES];
   int classLabel;
   
};

/* Task 1 */
int readFromFile (char fName [30], struct Animal dataZoo [NUM_SAMPLES]) ;

/* Task 2 */
void distanceFunctions (int vector1 [NUM_FEATURES],
                        int vector2 [NUM_FEATURES],
                        float * euclideanDistance,
                        int * hammingDistance,
                        float * jaccardSimilarity
);

/* Task 3 */
void findKNearestNeighbors (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int k, int whichDistanceFunction, int kNearestNeighbors [NUM_SAMPLES]);

/* Task 4 */
int predictClass (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int k);

/* Task 5 */
float findAccuracy (struct Animal dataZoo [NUM_SAMPLES], int whichDistanceFunction, struct Animal testData [NUM_TEST_DATA], int k);

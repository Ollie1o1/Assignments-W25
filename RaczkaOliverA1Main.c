#include "givenA1.h"
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

int dataLoaded = 0;  // Flag to track if data is loaded
int numRecords = 0;  // Track number of records loaded
struct Animal dataZoo[NUM_SAMPLES]; // Array to store the dataset
struct Animal testData[NUM_TEST_DATA]; // Array to store test data
int k; // Number of nearest neighbors

int main() {
    int input;
    int whichDistanceFunction; // Variable for distance selection
    int predictedClass;
    float accuracy;

    while (1) { // Keep the menu running until user exits
        printf("\nHello, welcome to the menu. Your options are:\n");
        printf("1. Read from file\n");
        printf("2. Calculate distance\n");
        printf("3. Find nearest neighbours\n");
        printf("4. Predict class\n");
        printf("5. Find accuracy\n");
        printf("6. Exit\n");
        printf("Please select a number from 1 to 5: ");

        if (scanf("%d", &input) != 1) { // Handle invalid input
            printf("Invalid input. Please enter a number from 1 to 5.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (input) {
            case 1: {
                printf("Reading from file...\n");
                numRecords = readFromFile("a1Data.txt", dataZoo); // Load dataset
                if (numRecords > 0) {
                    printf("Successfully loaded %d records from a1Data.txt.\n", numRecords);
                    dataLoaded = 1;
                } else {
                    printf("Error: No records loaded. Please check the file.\n");
                }
                break;
            }

            case 2: {
                if (!dataLoaded) {
                    printf("Error: No data loaded. Please read from file first.\n");
                    break;
                }

                printf("Calculating distance...\n");
                float euclideanDistance;
                int hammingDistance;
                float jaccardSimilarity;

                distanceFunctions(dataZoo[0].features, dataZoo[1].features, 
                                  &euclideanDistance, &hammingDistance, &jaccardSimilarity);

                printf("Euclidean Distance: %.6f\n", euclideanDistance);
                printf("Hamming Distance: %d\n", hammingDistance);
                printf("Jaccard Similarity: %.6f\n", jaccardSimilarity);
                break;
            }

            case 3: { // FIXED: Encapsulate in braces to limit VLA scope
                if (!dataLoaded) {
                    printf("Error: No data loaded. Please read from file first.\n");
                    break;
                }

                printf("Enter k (number of nearest neighbours): ");
                if (scanf("%d", &k) != 1 || k <= 0) {
                    printf("Invalid input. Please enter a positive integer.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    break;
                }

                printf("Select distance function (1=Euclidean, 2=Hamming, 3=Jaccard): ");
                if (scanf("%d", &whichDistanceFunction) != 1 || whichDistanceFunction < 1 || whichDistanceFunction > 3) {
                    printf("Invalid choice. Please select a valid distance function.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    break;
                }

                int kNearestNeighbors[k]; // Now safely scoped within this block

                findKNearestNeighbors(dataZoo, testData[0].features, k, whichDistanceFunction, kNearestNeighbors);
                
                printf("Nearest neighbors for test sample: ");
                for (int i = 0; i < k; i++) {
                    printf("%d ", kNearestNeighbors[i]);
                }
                printf("\n");
                break;
            }

            case 4: {
                if (!dataLoaded) {
                    printf("Error: No data loaded. Please read from file first.\n");
                    break;
                }

                if (k <= 0) { // Validate k
                    printf("Error: Set k first (use option 3).\n");
                    break;
                }

                printf("Select distance function (1=Euclidean, 2=Hamming, 3=Jaccard): ");
                if (scanf("%d", &whichDistanceFunction) != 1 || whichDistanceFunction < 1 || whichDistanceFunction > 3) {
                    printf("Invalid choice. Please select a valid distance function.\n");
                    while (getchar() != '\n');
                    break;
                }

                predictedClass = predictClass(dataZoo, testData[0].features, whichDistanceFunction, k);
                printf("The predicted class is: %d\n", predictedClass);
                break;
            }

            case 5: {
                if (!dataLoaded) {
                    printf("Error: No data loaded. Please read from file first.\n");
                    break;
                }

                if (k <= 0) { // Validate k
                    printf("Error: Set k first (use option 3).\n");
                    break;
                }

                printf("Select distance function (1=Euclidean, 2=Hamming, 3=Jaccard): ");
                if (scanf("%d", &whichDistanceFunction) != 1 || whichDistanceFunction < 1 || whichDistanceFunction > 3) {
                    printf("Invalid choice. Please select a valid distance function.\n");
                    while (getchar() != '\n');
                    break;
                }

                accuracy = findAccuracy(dataZoo, whichDistanceFunction, testData, k);
                printf("Accuracy for test data: %.6f\n", accuracy);
                break;
            }
            case 6: {
                printf("Exiting program...\n");
                return 0;
            }
            default:
                printf("You entered an invalid value. Please try again!\n");
        }
    }
    return 0;
}
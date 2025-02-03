#include "givenA1.h"
#include "helper.h"

int dataLoaded = 0;  // Flag to track if data is loaded
int numRecords = 0;  // Track number of records loaded
struct Animal dataZoo[NUM_SAMPLES]; // Array to store the dataset
struct Animal testData[NUM_TEST_DATA]; // Array to store test data
int k = 5; // Default number of nearest neighbors

int main() {
    int input;

    while (1) { // Keep the menu running until user exits
        printf("\nHere is the menu - enter a number between 1 and 5\n");
        printf("1. Read from file\n");
        printf("2. Calculate distance\n");
        printf("3. Find nearest neighbors\n");
        printf("4. Predict class\n");
        printf("5. Find accuracy\n");
        printf("6 Exit\n"); // Exit option
        printf("Enter your choice: ");

        if (scanf("%d", &input) != 1) { // Handle invalid input
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        switch (input) {
            case 1: {
                printf("a1Data.txt\n");
                numRecords = readFromFile("a1Data.txt", dataZoo);
                if (numRecords > 0) {
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

                int vector1[NUM_FEATURES] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1};
                int vector2[NUM_FEATURES] = {1,0,0,1,0,0,1,1,1,1,0,0,4,0,0,1};

                float euclideanDistance;
                int hammingDistance;
                float jaccardSimilarity;

                distanceFunctions(vector1, vector2, &euclideanDistance, &hammingDistance, &jaccardSimilarity);

                printf("Euclidean Distance: %.6f\n", euclideanDistance);
                printf("Hamming Distance: %d\n", hammingDistance);
                printf("Jaccard Similarity: %.6f\n", jaccardSimilarity);
                break;
            }

            case 3: {
                if (!dataLoaded) {
                printf("Error: No data loaded. Please read from file first.\n");
                break;
                }
    
                int newSample[NUM_FEATURES] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1};
                int kNearestNeighbors[k];

                // Find and print k-nearest neighbors using Euclidean Distance
                findKNearestNeighbors(dataZoo, newSample, k, 1, kNearestNeighbors);
                printf("Nearest neighbors for new sample with Euclidean Distance: ");
                 for (int i = 0; i < k; i++) {
                printf("%d ", kNearestNeighbors[i]);
                }
                printf("\n");

                // Find and print k-nearest neighbors using Hamming Distance
                findKNearestNeighbors(dataZoo, newSample, k, 2, kNearestNeighbors);
                printf("Nearest neighbors for new sample with Hamming Distance: ");
                for (int i = 0; i < k; i++) {
                printf("%d ", kNearestNeighbors[i]);
                }
                printf("\n");

                // Find and print k-nearest neighbors using Jaccard Similarity
                findKNearestNeighbors(dataZoo, newSample, k, 3, kNearestNeighbors);
                printf("Nearest neighbors for new sample with Jaccard Similarity: ");
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
                int newSample[NUM_FEATURES] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1};
    
                // Predict class using Euclidean Distance
                int predictedClassEuclidean = predictClass(dataZoo, newSample, 1, k);
                printf("Predicted class for new sample with Euclidean Distance: %d\n", predictedClassEuclidean);

                // Predict class using Hamming Distance
                int predictedClassHamming = predictClass(dataZoo, newSample, 2, k);
                printf("Predicted class for new sample with Hamming Distance: %d\n", predictedClassHamming);

                // Predict class using Jaccard Similarity
                int predictedClassJaccard = predictClass(dataZoo, newSample, 3, k);
                printf("Predicted class for new sample with Jaccard Similarity: %d\n", predictedClassJaccard);

                break;
            }

            case 5: {
                if (!dataLoaded) {
                printf("Error: No data loaded. Please read from file first.\n");
                break;
                }
                // Read test data from testData.csv
                int testRecords = readTestData("testData.csv", testData);
                if (testRecords == 0) {
                printf("Error: No test data loaded. Accuracy cannot be computed.\n");
                break;
                }

                // Compute and display accuracy for each distance metric
                float accuracyEuclidean = findAccuracy(dataZoo, 1, testData, k);
                printf("The accuracy for test data using Euclidean Distance is: %.2f%%\n", accuracyEuclidean);

                float accuracyHamming = findAccuracy(dataZoo, 2, testData, k);
                printf("The accuracy for test data using Hamming Distance is: %.2f%%\n", accuracyHamming);
    
                float accuracyJaccard = findAccuracy(dataZoo, 3, testData, k);
                printf("The accuracy for test data using Jaccard Similarity is: %.2f%%\n", accuracyJaccard);

                break;
            }
            case 6: {
                printf("Exiting program.\n");
                return 0;
            }
            default:
                printf("Invalid choice. Please select a valid option.\n");
        }
    }
    return 0;
}

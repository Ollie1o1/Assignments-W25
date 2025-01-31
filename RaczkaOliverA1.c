#include "givenA1.h"

int readFromFile (char fName [30],struct Animal dataZoo [NUM_SAMPLES]){

    FILE *file = fopen(fName, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", fName);
        return -1; 
    }

    for (int i = 0; i < NUM_SAMPLES; i++) {
        char line[256];
        if (!fgets(line, sizeof(line), file)) {
            printf("Error: Insufficient data in file\n");
            fclose(file);
            return -1;
        }

    char *token = strtok(line, " "); //strtok splits strings into tokens
        strncpy(dataZoo[i].animalName, token, MAX_LENGTH_ANIMAL_NAME);
        dataZoo[i].animalName[MAX_LENGTH_ANIMAL_NAME - 1] = '\0';

    for (int j = 0; j < NUM_FEATURES; j++) {
            token = strtok(NULL, " ");   
            if (token == NULL) {
                printf("Error: Missing features in line %d\n", i + 1);
                fclose(file);
                return -1;
            }
            dataZoo[i].features[j] = atoi(token);
        }

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Error: Missing class label in line %d\n", i + 1);
            fclose(file);
            return -1;
        }
        dataZoo[i].classLabel = atoi(token);
    }

    fclose(file);
    return 1; 
}

void distanceFunctions (int vector1 [NUM_FEATURES],int vector2 [NUM_FEATURES], float * euclideanDistance, int * hammingDistance, float * jaccardSimilarity){

    float sumOfSquares = 0.0; // For Euclidean Distance
    int hammingCount = 0;    // For Hamming Distance
    int onesInBoth = 0;      // For Jaccard Similarity numerator
    int totalFeatures = 0;   // For Jaccard Similarity denominator

    for (int i = 0; i < NUM_FEATURES; i++) {
        // Compute Euclidean Distance 
        float difference = vector1[i] - vector2[i];
        sumOfSquares += difference * difference;

        // Compute Hamming Distance
        if (vector1[i] != vector2[i]) {
            hammingCount++;
        }

        // Compute Jaccard Similarity
        if (vector1[i] == 1 && vector2[i] == 1) {
            onesInBoth++;
        }
        if (vector1[i] != 0 || vector2[i] != 0) {
            totalFeatures++;
        }
    }

    *euclideanDistance = sqrt(sumOfSquares);  //final compute of the 2 distances and jaccard similarity 
    *hammingDistance = hammingCount;
    if (totalFeatures == 0) {
        *jaccardSimilarity = 0.0;
    } else {
        *jaccardSimilarity = (float)onesInBoth / totalFeatures;
    }

}
void findKNearestNeighbors (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int k, int whichDistanceFunction, int kNearestNeighbors [NUM_SAMPLES]){

    float distances[NUM_SAMPLES];
    int indices[NUM_SAMPLES];

     for (int i = 0; i < NUM_SAMPLES; i++) {
        float euclideanDistance = 0.0;
        int hammingDistance = 0;
        float jaccardSimilarity = 0.0;

        // Use the distanceFunctions to calculate distances
        distanceFunctions(newSample, dataZoo[i].features, &euclideanDistance, &hammingDistance, &jaccardSimilarity);

        // Store the selected distance measure based on whichDistanceFunction
        if (whichDistanceFunction == 1) {
            distances[i] = euclideanDistance;
        } else if (whichDistanceFunction == 2) {
            distances[i] = hammingDistance;
        } else if (whichDistanceFunction == 3) {
            distances[i] = jaccardSimilarity;
        }
        indices[i] = i; // Keep track of indices
    }

    for (int i = 0; i < NUM_SAMPLES - 1; i++) {
        for (int j = 0; j < NUM_SAMPLES - i - 1; j++) {
            int swap = 0;

            if (whichDistanceFunction == 3) { // For Jaccard, sort in increasing order
                if (distances[j] > distances[j + 1]) {
                    swap = 1;
                }
            } else { // For Euclidean and Hamming, sort in decreasing order
                if (distances[j] < distances[j + 1]) {
                    swap = 1;
                }
            }

            if (swap) {
                // Swap distances
                float tempDistance = distances[j];
                distances[j] = distances[j + 1];
                distances[j + 1] = tempDistance;

                // Swap indices
                int tempIndex = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = tempIndex;
            }
        }
    }
    for (int i = 0; i < k; i++) {
        kNearestNeighbors[i] = indices[i];
    }
}

int predictClass (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int k){

    int *kNearestNeighbors = (int *)malloc(k * sizeof(int));  // have to do all of this because k told me error
    if (kNearestNeighbors == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1; 
    }

    int classCounts[NUM_CLASSES] = {0};

    findKNearestNeighbors(dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);

    for (int i = 0; i < k; i++) {
        int neighborIndex = kNearestNeighbors[i];
        int neighborClass = dataZoo[neighborIndex].classLabel;
        classCounts[neighborClass - 1]++;
    }

    free(kNearestNeighbors); // free what i did above

    // Find the class with the highest count
    int predictedClass = 1;
    int maxCount = classCounts[0];
    for (int i = 1; i < NUM_CLASSES; i++) {
        if (classCounts[i] > maxCount) {
            maxCount = classCounts[i];
            predictedClass = i + 1;
        } else if (classCounts[i] == maxCount && (i + 1) < predictedClass) {
            predictedClass = i + 1; // Tie-breaker: choose the smaller class label
        }
    }

    return predictedClass;
}

float findAccuracy(struct Animal dataZoo[NUM_SAMPLES], 
                   struct Animal testData[NUM_TEST_DATA], 
                   int whichDistanceFunction, 
                   int k) {
    int correctPredictions = 0;

    // Loop through each test sample
    for (int i = 0; i < NUM_TEST_DATA; i++) {
        int predictedClass = predictClass(dataZoo, testData[i].features, whichDistanceFunction, k);
        if (predictedClass == testData[i].classLabel) {
            correctPredictions++;
        }
    }
    return (float)correctPredictions / NUM_TEST_DATA; //simple return for the accuracy value
}

#include "givenA1.h"

int readFromFile(char *filename, Animal dataZoo[NUM_SAMPLES]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return 0; // Return 0 to indicate no records were loaded
    }

    int count = 0; // Track number of records loaded

    printf("\nReading data from %s...\n", filename);
    
    while (count < NUM_SAMPLES && fscanf(file, "%s", dataZoo[count].animalName) == 1) {
        for (int i = 0; i < NUM_FEATURES; i++) {
            if (fscanf(file, "%d", &dataZoo[count].features[i]) != 1) {
                printf("Error: Unexpected file format at record %d\n", count + 1);
                fclose(file);
                return count; // Return the number of successfully loaded records
            }
        }
        if (fscanf(file, "%d", &dataZoo[count].classLabel) != 1) {
            printf("Error: Missing class label at record %d\n", count + 1);
            fclose(file);
            return count;
        }

        // Print each record as it is read
        printf("%s ", dataZoo[count].animalName);
        for (int i = 0; i < NUM_FEATURES; i++) {
            printf("%d ", dataZoo[count].features[i]);
        }
        printf("%d\n", dataZoo[count].classLabel);

        count++; // Increment the record count
    }

    fclose(file);
    printf("\nSuccessfully loaded %d records from %s.\n", count, filename);
    return count; // Return the number of records loaded
}

void distanceFunctions (int vector1 [NUM_FEATURES],int vector2 [NUM_FEATURES], float * euclideanDistance, int * hammingDistance, float * jaccardSimilarity){

    int i;
    float sumSquares = 0;
    int hammingCount = 0;
    int intersection = 0;
    int unionCount = 0;

    // Iterate over the feature vectors
    for (i = 0; i < NUM_FEATURES; i++) {
        // Euclidean Distance Calculation
        float diff = vector1[i] - vector2[i];
        sumSquares += diff * diff;

        // Hamming Distance Calculation
        if (vector1[i] != vector2[i]) {
            hammingCount++;
        }

        // Jaccard Similarity Calculation
        if (vector1[i] == 1 || vector2[i] == 1) {
            unionCount++;
        }
        if (vector1[i] == 1 && vector2[i] == 1) {
            intersection++;
        }
    }

    // Compute final values
    *euclideanDistance = sqrt(sumSquares);
    *hammingDistance = hammingCount;
    *jaccardSimilarity = (unionCount == 0) ? 0 : (float) intersection / unionCount;

}
void findKNearestNeighbors(struct Animal dataZoo[NUM_SAMPLES], int newSample[NUM_FEATURES], int k, int whichDistanceFunction, int kNearestNeighbors[NUM_SAMPLES]) {
    float distances[NUM_SAMPLES];
    int indices[NUM_SAMPLES];

    // Compute distances for each sample in the dataset
    for (int i = 0; i < NUM_SAMPLES; i++) {
        float euclideanDistance = 0.0;
        int hammingDistance = 0;
        float jaccardSimilarity = 0.0;

        distanceFunctions(newSample, dataZoo[i].features, &euclideanDistance, &hammingDistance, &jaccardSimilarity);

        if (whichDistanceFunction == 1) {
            distances[i] = euclideanDistance;
        } else if (whichDistanceFunction == 2) {
            distances[i] = hammingDistance;
        } else {
            distances[i] = jaccardSimilarity;
        }
        indices[i] = i;
    }

    // Sort distances in ascending order for Euclidean & Hamming, descending for Jaccard
    for (int i = 0; i < NUM_SAMPLES - 1; i++) {
        for (int j = 0; j < NUM_SAMPLES - i - 1; j++) {
            int swap = 0;
            if ((whichDistanceFunction == 3 && distances[j] < distances[j + 1]) ||
                (whichDistanceFunction != 3 && distances[j] > distances[j + 1])) {
                swap = 1;
            }

            if (swap) {
                float tempDist = distances[j];
                distances[j] = distances[j + 1];
                distances[j + 1] = tempDist;

                int tempIdx = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = tempIdx;
            }
        }
    }

    // Store the top k nearest neighbors
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

float findAccuracy(struct Animal dataZoo[NUM_SAMPLES],int whichDistanceFunction, struct Animal testData[NUM_TEST_DATA], int k) {
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

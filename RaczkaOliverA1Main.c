#include "givenA1.h"
#include <stdio.h>
#include "helper.h"

int main(){
    
    int input;
    printf("Hello welcome to the menu, your options are:\n");
    printf("Read from file (1)\n");
    printf("Calculate the distance (2)\n");
    printf("Find nearest neighbours (3)\n");
    printf("Predict Class (4)\n");
    printf("Find Accuracy (5)\n");
    printf("Please select a number from one to five: ");
    scanf("%d", input);

    if(input == 1){

    }else if(input == 2){


    }else if(input == 3){


    }else if(input == 4){


    }else if(input == 5){


    }else{
        printf("You entered a invalid value please try again!!");
        return 0;
    }

    return 0;
}
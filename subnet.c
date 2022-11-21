#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int *splitDottedDecimal(char *dd) {
    char test[15];
    static int returnArray[4];
    // char temp;

    // char *returnArray = (char*)(malloc(4*3*sizeof(char)));
    int i = 0;
    int j;
    int k = 0;
    int l = 0;
    int count = 0;

    while (dd[l] != '.' && dd[l] != '/') {   // loop until . or /
        // printf("dd[%d]= %c\n", l, dd[l]);
        
            count++;
            l++;
            // test[k] = dd[i];
        }
        // l++;
    char *temp = malloc(count*sizeof(char));
    for (i = 0; i<(count); i++) {
        temp[i] = dd[i];     // PROBLEM!
            // k++;
    }
    returnArray[0] = atoi(temp);
    free(temp);
    l++;
        // printf("l: %d\n", l);
int m;
    for (j = 1; j < 4; j++) {
        // if (dd[l] != '.' && dd[l] != '/') {
        //     count++;
        //     l++;
        // }

        // printf("dd[%d]= %c\n", l, dd[l]);
        m=l;
        while (dd[l] != '.' && dd[l] != '/') {   // loop until . or /
        // printf("dd[%d]= %c\n", l, dd[l]);
        // m = l;
            count++;
            l++;
            // test[k] = dd[i];
        }
        // l++;
        char *temp = malloc(count*sizeof(char));
        for (i = 0; i<(count); i++) {
            temp[i] = dd[m];     // PROBLEM!
            // k++;
            m++;
        }
        returnArray[j] = atoi(temp);
        // printf("OctetX: %s,\t i: %d\n", temp, i);
        // printf("Index of dd: %d\n", l);
        count = 0;
        free(temp);
        i = 0;
        /*
        while (dd[i] != '.' && dd[i] != '/') {   // loop until . or /
            temp[k] = dd[i];
            i++;
            k++;
            }
        */
        // returnArray[j] = atoi(temp);
        // free(temp);
        l++;
        // i = 0;
        // k = 0;

    }
/*
    int j = 0;

    char *token = strtok(dd, ".");
    while (token != NULL) {
        // printf("Index: %d, Value: %s\n", i, token);
        for(j=0; j < strlen(token); j++) {
        returnArray[j][*i] = token[j];  // This is failing
        }
        i++;
        token = strtok(NULL, ".");
    }
*/
    return returnArray;
}

bool checkdots(char *dd) {
    int i;
    int dot = 0;
    int slash = 0;
    for (i = 0; i<strlen(dd); i++) {
        if (dd[i] == '.') {
            dot++;
        }
        if (dd[i] == '/') {
            slash++;
        }
    }
    // printf("Dots: %d\n", dot);
    // printf("Slashes: %d\n", slash);
    if (dot < 4 && slash <= 1) {
        return true;
    }
    return false;
}

bool checkIPcharacters(char *dd) {
    const char *test = "123456789./";
    if (strlen(dd) < 19) {
        if (dd[strspn(dd, test)] == '\0') {
            if (checkdots(dd)) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[])  {

    int i;
    // unsigned char ip[4] = {192, 168, 23, 12};
    // unsigned char mask[4] = {255, 255, 255, 0};
    char *dd;

    if( argc == 2 ) {
        dd = argv[1];
    }

    if (checkIPcharacters(dd)) {
        int *newArray;
        newArray = splitDottedDecimal(dd);
        for (i = 0; i < 4; i++) {
            printf("Octets\t%d\n", newArray[i]);
        }
    }
    else {
        printf("Invalid IP Address.\n");
    }

    // int *newArray;
    // size_t arraySize;

    // printf("New Array: %s\n", newArray);

}

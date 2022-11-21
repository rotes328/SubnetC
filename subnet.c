#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void invalid(int error) {
    if (error == 0) {
    printf("Invalid IP Address.\n");
    }
    else if (error == 1) {
        printf("Multicast IP Address.\n");
    }
    else if (error == 2) {
        printf("Mask is invalid.\n");
    }
    // return;
    // exit(0);
}


int *split(char *dd) {
    int *returnArray = malloc(4*sizeof(int));
    // int returnArray[4];

    int j = 0;
    int i = 0;

    char *token = strtok(dd, ".");
    returnArray[0] = atoi(token);
    // for(j=1; j<4; j++) {
    while (token != NULL) {
        // printf("Index: %d, Value: %s\n", i, token);
        // for(j=1; j < 4; j++) {
        returnArray[j] = atoi(token);  // This is failing
        j++;
        // i++;
        // }
        // i++;
        token = strtok(NULL, ".");
    }
    // }
    return returnArray;

}


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

void checkIP(int *ipArray) {
    int i;

    if (ipArray[0] > 255) {
        invalid(0);
    }
    else if (ipArray[0] > 223) {
        invalid(1);
    }
    else if (ipArray[0] == 0) {
        invalid(0);
    }

    for (i = 0; i < 4; i++) {
        if (ipArray[i] > 255) {
            invalid(0);
        }
    }
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
    if (dot == 3 && slash <= 1) {
        return true;
    }
    return false;
}
/*
void reverseMask(bool *bytemask[]) {
    // static bool reversed[32];
    int start = 0;
    bool temp;
    int end = sizeof(bytemask) / sizeof(bytemask[0]);
    while (start < end) {
        temp = bytemask[start];
        bytemask[start] = bytemask[end];
        bytemask[end] = temp;
        start++;
        end--;
    }
}
*/

void checkMaskBytes(int *mask) {
    int firstOne = 0;
    // int Zeros = 0;
    bool bytemask[32];
    // bool byteArray[8];
    // bool oneEncountered = false;
    // bool newByte = false;
    // bool issue = false;
    for(int i = 0; i < 4; i++) {
        int byte = mask[i];
        bool hasZero = false;
        for(int j = 0; j < 8; j++) {
            if (byte & (1 << j)) {
                bytemask[(i*8)+j] = true;
                // printf("%d.%d.%d.1 ", i, j, (i*8+j));
                firstOne++;
            }
            else {
                // printf("%d.%d.%d.0 ", i, j, (i*8+j));
                hasZero = true;
                bytemask[(i*8)+j] = false;
                if (firstOne > 0) {
                    invalid(2);
                    exit(0);
                }
                }
            }
        firstOne = 0;
        if (hasZero) {
            // If not all 1s, reverse byte
            bool temp;
            int start = (i*8);
            int end = (i*8) + 7;
            while (start < end) {
            temp = bytemask[start];
            bytemask[start] = bytemask[end];
            bytemask[end] = temp;
            start++;
            end--;
            }
        }
        // hasZero = false;

    //     reverseMask(*byteArray);
    // for (int i = 0; i < 8; i++) {
    //     printf("%d", byteArray[i]);
    // }
    }
    // printf("Zero counter %d\n", firstZero);
    // firstOne = 0;
    // firstZero = 0;
    bool zeroFound = false;
    // Iterate mask and if there is a 
    for (int i = 0; i < 32; i++) {
        if (zeroFound && bytemask[i]) {
            invalid(2);
            exit(0);
        }
        if (bytemask[i] == false) {
            zeroFound = true;
        }
    }
    printf("\n");
    printf("Byte Array: ");
    for (int k = 0; k < 32; k++) {
        printf("%d", bytemask[k]);
    }
    printf("\n");
    // newByte = true;
    }
//     printf("\n");
//     // firstZero = 0;
// }

bool checkIPcharacters(char *dd, int argc) {
    const char *testIncludingMask = "1234567890./";
    const char *testNoMask = "1234567890.";
    if (argc == 2) {
        if (strlen(dd) < 19) {
        if (dd[strspn(dd, testIncludingMask)] == '\0') {
            if (checkdots(dd)) {
                return true;
            }
        }
    } }
    else if (argc == 3) {
        if (strlen(dd) < 19) {
        if (dd[strspn(dd, testNoMask)] == '\0') {
            if (checkdots(dd)) {
                return true;
            }
        }
    } }
    /*
    if (strlen(dd) < 19) {
        if (dd[strspn(dd, test)] == '\0') {
            if (checkdots(dd)) {
                return true;
            }
        }
    }
    */
   printf("Here's the problem\n");
    return false;
}

void help() {
    printf("Please supply an argument.\n");
    printf("%% ip 192.168.20.4/29\n");
    printf("%% ip 172.16.20.40 255.255.255.128\n");
    exit(0);
}

int main(int argc, char *argv[])  {

    int i;
    // unsigned char ip[4] = {192, 168, 23, 12};
    // unsigned char mask[4] = {255, 255, 255, 0};
    char *dd;
    char *mask;
    int *ipArray;
    int *maskArray;

    if( argc == 2 ) {
        dd = argv[1];
        if ((strcmp(dd, "-h") == 0) || strcmp(dd, "--help") == 0) {
            help();
        }
    }
    else if (argc == 3) {
        dd = argv[1];
        mask = argv[2];
        // printf("%s\n", mask);
    }
    else {
        help();
    }

    if (checkIPcharacters(dd, argc)) {
        // int *ipArray;
        // newArray = splitDottedDecimal(dd);
        ipArray = split(dd);
    }
    else {
        invalid(0);
        exit(0);
        // printf("Invalid IP Address.\n");
    }

    if (checkIPcharacters(mask, argc)) {
        printf("Hello!\n");
        maskArray = split(mask);
        checkMaskBytes(maskArray);
        printf("Mask: ");
        for (i = 0; i < 3; i++) {
            printf("%d.", maskArray[i]);
        }
        printf("%d\n", maskArray[3]);
    }
    else {
        invalid(2);
    }

    checkIP(ipArray);
        for (i = 0; i < 4; i++) {
            printf("Octets\t%d\n", ipArray[i]);
        }
        
    free(maskArray);
    free(ipArray);
    // int *newArray;
    // size_t arraySize;

    // printf("New Array: %s\n", newArray);

}

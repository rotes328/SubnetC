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
    // exit(0);
}


int *split(char *dd) {
    int *returnArray = malloc(4*sizeof(int));
    int j = 0;
    int i = 0;

    char *token = strtok(dd, ".");
    returnArray[0] = atoi(token);
    while (token != NULL) {
        returnArray[j] = atoi(token);
        j++;
        token = strtok(NULL, ".");
    }
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
    if (dot == 3 && slash <= 1) {
        return true;
    }
    return false;
}

int getCIDR(char *dd) {
    int cidr;
    int *returnArray = malloc(4*sizeof(int));

    char *token = strtok(dd, "/");
    while (token != NULL) {
        cidr = atoi(token);
        token = strtok(NULL, ".");
    }
    return cidr;
}

int checkCIDR(int cidr) {
    if (cidr == 0) {
        invalid(2);
        return 0;
    }
    else if (cidr > 32) {
        invalid(2);
        return 0;
    }
    return cidr;
}

int checkMaskBytes(int *mask) {
    int firstOne = 0;
    int cidr = 0;
    bool bytemask[32];
    for(int i = 0; i < 4; i++) {
        int byte = mask[i];
        bool hasZero = false;
        for(int j = 0; j < 8; j++) {
            if (byte & (1 << j)) {
                bytemask[(i*8)+j] = true;
                cidr++;
                firstOne++;
            }
            else {
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
    } } }

    bool zeroFound = false;
    // Iterate mask and if there is a 
    for (int i = 0; i < 32; i++) {
        if (zeroFound && bytemask[i]) {
            invalid(2);
            exit(0);
        }
        if (bytemask[i] == false) {
            zeroFound = true;
    } }
    // printf("\n");
    // printf("Byte Array: ");
    // for (int k = 0; k < 32; k++) {
    //     printf("%d", bytemask[k]);
    // }
    // printf("\n");
    return cidr;
}

bool checkInputCharacters(char *dd, int argc) {
    const char *testIncludingMask = "1234567890./";
    const char *testNoMask = "1234567890.";
    if (argc == 2) {
        if (strlen(dd) < 19) {
        if (dd[strspn(dd, testIncludingMask)] == '\0') {
            if (checkdots(dd)) {
                return true;
     } } } }
    else if (argc == 3) {
        if (strlen(dd) < 19) {
        if (dd[strspn(dd, testNoMask)] == '\0') {
            if (checkdots(dd)) {
                return true;
    } } } }
//    printf("Here's the problem\n");
    return false;
}

void help() {
    printf("Please supply an argument.\n");
    printf("%% ip 192.168.20.4/29\n");
    printf("%% ip 172.16.20.40 255.255.255.128\n");
    exit(0);
}

int *makeMask(int cidr) {
    int *returnArray = malloc(4*sizeof(int));

    for (int i = 0; i < 4; i++) {
        unsigned char currentByte;
        for (int j = 0; j < 8; j++) {
            if (cidr > 0) {
                currentByte = currentByte << 1;
                currentByte++;
                cidr--;
            }
            else {
                currentByte = currentByte << 1;
            }
    }
        int intByte = (int)(currentByte);
        returnArray[i] = intByte;

    }
    // printf("Your constructed mask is: ");
    // for (int i = 0; i < 3; i++) {
    //     printf("%d.", returnArray[i]);
    // }
    // printf("%d\n", returnArray[3]);
return returnArray;
}

void subnet(int *ip, int *mask) {
    int result[4];
    int i;

    for (i = 0; i < 4; i++) {
        result[i] = ip[i] & mask[i];
    }

    printf("Network Address is ");
    for (i = 0; i < 3; i++) {
        printf("%d.", result[i]);
    }
    printf("%d\n", result[3]);
}

int main(int argc, char *argv[])  {

    char *dd;
    char *mask;
    int *ipArray;
    int *maskArray;
    int *constructedMask;
    int errno = 0;
    int cidr;


    if( argc == 2 ) {
        dd = argv[1];
        if ((strcmp(dd, "-h") == 0) || strcmp(dd, "--help") == 0) {
            help();
        }
        cidr = getCIDR(dd);
        int cidr_code = checkCIDR(cidr);
        if (cidr_code == 0) {
            exit(0); }
        maskArray = makeMask(cidr);     // malloc called
    }
    else if (argc == 3) {
        dd = argv[1];
        mask = argv[2];
         if (checkInputCharacters(mask, argc)) {
        maskArray = split(mask);     // malloc called
        cidr = checkMaskBytes(maskArray);
    }
    else {
        invalid(2);
    }

    }
    else {
        help();
    }

    if (checkInputCharacters(dd, argc)) {

        ipArray = split(dd);       // malloc called

        // printf("Your IP is: ");
        // for (int i = 0; i < 3; i++) {
        //     printf("%d.", ipArray[i]);
        // }
        // printf("%d/%d\n", ipArray[3], cidr);
    }
    else {
        invalid(0);
        exit(0);
    }

    checkIP(ipArray);
    
    // Print IP
    printf("IP Address: ");
    for (int i = 0; i < 3; i++) {
        printf("%d.", ipArray[i]);
    }
    printf("%d\n", ipArray[3]);

    // Print Mask
    printf("Mask: ");
    for (int i = 0; i < 3; i++) {
        printf("%d.", maskArray[i]);
    }
    printf("%d\n", maskArray[3]);

    subnet(ipArray, maskArray);

    free(maskArray);
    free(ipArray);
}
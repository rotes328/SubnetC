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


unsigned int *split(char *dd) {
    unsigned int *returnArray = malloc(4*sizeof(unsigned int));
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



void checkIP(unsigned int *ipArray) {
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
    int dot = 0;
    int slash = 0;
    for (int i = 0; i < strlen(dd); i++) {
        if (dd[i] == '.') {
            dot++;
        }
        if (dd[i] == '/') {
            slash++;
        }
    }
    if (dot == 3 && slash <= 1) {
        int thirddot = 3;
        for (int i = 0; i < strlen(dd); i++) {
            if (dd[i] == '.') {
                thirddot--;
            }
            if (thirddot == 0) {
                if (i != strlen(dd) -1 ) {
                    return true;
                }
            }
        }
        // return true;
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
        exit(0);
    }
    else if (cidr > 32) {
        invalid(2);
        exit(0);
    }
    return cidr;
}

int checkMaskBytes(unsigned int *mask) {
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
    printf("Please supply one or two arguments.\n");
    printf("%% ip 192.168.20.4/29\n");
    printf("%% ip 172.16.20.40 255.255.255.128\n");
    exit(0);
}

unsigned int *makeMask(int cidr) {
    unsigned int *returnArray = malloc(4*sizeof(unsigned int));

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

unsigned int *subnet(unsigned int *ip, unsigned int *mask) {
    unsigned int *networkAddress = malloc(4*sizeof(unsigned int));
    // int result[4];
    int i;

    for (i = 0; i < 4; i++) {
        networkAddress[i] = ip[i] & mask[i];
    }

    return networkAddress;
}

unsigned int *firstAddress(unsigned int *networkAddress) {
    unsigned int *firstIP = malloc(4*sizeof(unsigned int));
    for (int i = 0; i < 3; i++) {
        firstIP[i] = networkAddress[i];
    }
    firstIP[3] = (networkAddress[3] + 1);
    return firstIP;
}

unsigned int *broadcast(unsigned int *networkAddress, unsigned int *mask) {
    unsigned int *broadcastIP = malloc(4*sizeof(unsigned int));
    for (int i = 0; i < 4; i++) {
        broadcastIP[i] = (~mask[i] & 0xff) + networkAddress[i];
    }
    return broadcastIP;
}

unsigned int *lastAddress(unsigned int *broadcastIP) {
    unsigned int *lastIP = malloc(4*sizeof(unsigned int));
    for (int i = 0; i < 3; i++) {
        lastIP[i] = broadcastIP[i];
    }
    lastIP[3] = (broadcastIP[3] - 1);
    return lastIP;
}

void printDD(unsigned int *dd) {
    for (int i = 0; i < 3; i++) {
    printf("%d.",  dd[i]);
    }
    printf("%d\n", dd[3]);
}

void renderOutput(unsigned int *networkAddress, unsigned int *firstIP, unsigned int *lastIP, unsigned int *broadcastIP) {
    printf("Network Address is: ");
    printDD(networkAddress);
    printf("First IP is: ");
    printDD(firstIP);
    printf("Last IP is ");
    printDD(lastIP);
    printf("Broadcast is ");
    printDD(broadcastIP);
}

int main(int argc, char *argv[])  {

    char *dd;
    char *mask;
    unsigned int *ipArray;
    unsigned int *maskArray;
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
        exit(0);
    }

    }
    else {
        help();
    }

    if (checkInputCharacters(dd, argc)) {
        ipArray = split(dd);       // malloc called
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

    unsigned int *networkAddress = subnet(ipArray, maskArray);
    unsigned int *firstIP = firstAddress(networkAddress);
    unsigned int *broadcastIP = broadcast(networkAddress, maskArray);
    unsigned int *lastIP = lastAddress(broadcastIP);

    renderOutput(networkAddress, firstIP, lastIP, broadcastIP);

    free(firstIP);
    free(broadcastIP);
    free(lastIP);
    free(networkAddress);
    free(maskArray);
    free(ipArray);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void invalid(int error) {
    if (error == 0) {
        printf("Invalid IP Address.\n");
        return;
    }
    printf("Invalid mask.\n");
    return;
}


unsigned int *split(char *dd) {
    unsigned int *returnArray = malloc(4*sizeof(unsigned int));
    if (strstr(dd, "..")) {
        for (int i = 0; i < 4; i++) {
            returnArray[i] = 0;
        }
        return returnArray;
    }
    int j = 0;
    char *token = strtok(dd, ".");
    returnArray[0] = atoi(token);
    while (token != NULL) {
        returnArray[j] = atoi(token);
        j++;
        token = strtok(NULL, ".");
    }
    return returnArray;
}



bool checkIP(unsigned int *ipArray) {
    if ((ipArray[0] > 239) || (ipArray[0] == 0)) {
        invalid(0);
        return false;
    }
    for (int i = 1; i < 4; i++) {
        if (ipArray[i] > 255) {
            invalid(0);
            return false;
    } }
    return true;
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
    } }
    if (dot == 3 && slash <= 1) {
        int thirddot = 3;
        for (int i = 0; i < strlen(dd); i++) {
            if (dd[i] == '.') {
                thirddot--;
            }
            if (thirddot == 0) {
                if (i != strlen(dd) -1 ) {
                    return true;
    } } } }
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
        invalid(1);
        exit(0);
    }
    else if (cidr > 32) {
        invalid(1);
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
                    invalid(1);
                    exit(0);
        } } }
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
        if (zeroFound && bytemask[i] == 1) {
            invalid(1);
            exit(0);
        }
        if (bytemask[i] == 0) {
            zeroFound = true;
        }
    }
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
        } }
        int intByte = (int)(currentByte);
        returnArray[i] = intByte;
    }
return returnArray;
}

unsigned int *subnet(unsigned int *ip, unsigned int *mask) {
    unsigned int *networkAddress = malloc(4*sizeof(unsigned int));
    for (int i = 0; i < 4; i++) {
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

void printDD(unsigned int *dd, bool newline) {
    for (int i = 0; i < 3; i++) {
    printf("%d.",  dd[i]);
    }
    printf("%d", dd[3]);
    if (newline) {
        printf("\n");
} }

void renderOutput(
    unsigned int *networkAddress,
    unsigned int *firstIP,
    unsigned int *lastIP,
    unsigned int *broadcastIP,
    bool supernetFlag,
    int ipClass,
    int maskClass,
    int cidr) {
        if (maskClass == 6) {
            printf("Host address: ");
            printDD(networkAddress, false);
            printf("/32\n");
            return;
        }
        if (supernetFlag) {
            printf("Supernet IP range is: ");
            printDD(networkAddress, false);
            printf(" - ");
            printDD(broadcastIP, true);
            return;
        }
        if (ipClass == 4) {
            printf("Multicast range: ");
            printDD(networkAddress, false);
            printf("/%d\n", cidr);
            printf("Range is: ");
            printDD(networkAddress, false);
            printf(" - ");
            printDD(broadcastIP, true);
            return;    
        }
        if (maskClass == 5) {                 // Handle 31
            printf("Network address: ");
            printDD(networkAddress, false);
            printf("/%d\n", cidr);
            printf("IP range is: ");
            printDD(lastIP, false);
            printf(" - ");
            printDD(firstIP, true);
        }
        else {
            printf("Network address is: ");
            printDD(networkAddress, false);
            printf("/%d\n", cidr);
            printf("Broadcast address is: ");
            printDD(broadcastIP, true);
            printf("IP range is: ");
            printDD(firstIP, false);
            printf(" - ");
            printDD(lastIP, true);
        }
    return;
}

int getMaskClass(int mask) {
    switch (mask) {
        case 1 ... 7:
            return 0;  // Supernet
        case 8 ... 15:
            return 2;  // Class A
        case 16 ... 23:
            return 3;  // Class B
        case 24 ... 30:
            return 4;  // Class C
        case 31:
            return 5;  // RFC 3021
        default:
            return 6;  // Host Address
} }

int getIPClass(int firstOctet) {
    switch (firstOctet) {
        case 1 ... 127:
            return 1;  // Class A
        case 128 ... 191:
            return 2;  // Class B
        case 192 ... 223:
            return 3;  // Class C
        default:
            return 4;  // Multicast
} }

bool supernet(int ipClass, int maskClass) {
    if (ipClass == 4 && maskClass == 0) {    // Multicast is supernet < /8
        return true;
    }
    else if (ipClass == 4) {                 // Otherwise it's not a supernet
        return false;
    }
    else if (ipClass < maskClass) {          // Classful ranges are not supernets in this condition
        return false;
    }
    else {
        return true;                        // Otherwise it's a supernet
} }

bool checkMaskFormat(char *mask) {
    char first_letter = mask[0];
    if (first_letter == '.') {
        return false;
    }
    for (int i = 1; i < strlen(mask); i++) {
        if (mask[i] == '.' && mask[i-1] == '.') {
            return false;
        }
    }
    return true;
}

bool checkForSlash(char *dd) {
    char* hasSlash = strchr(dd, '/');
    if (hasSlash == NULL) {
        return false;
    }
    return true;
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
        if (!checkForSlash(dd)) {
            invalid(1);
            exit(0);
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
        bool maskFormat = checkMaskFormat(mask);
        if (maskFormat == false) {
            invalid(1);
            exit(0);
        }
        if (checkInputCharacters(mask, argc)) {
        maskArray = split(mask);     // malloc called
        cidr = checkMaskBytes(maskArray);
        if (cidr == 0) {
            invalid(1);
            free(maskArray);
            exit(0);
        } }
    else {
        invalid(1);
        exit(0);
    } }
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

    if (checkIP(ipArray) == false) {
        free(ipArray);
        free(maskArray);
        exit(0);
    }

    printf("IP Address: ");
    printDD(ipArray, true);
    printf("Mask: ");
    printDD(maskArray, true);

    unsigned int *networkAddress = subnet(ipArray, maskArray);
    unsigned int *firstIP = firstAddress(networkAddress);
    unsigned int *broadcastIP = broadcast(networkAddress, maskArray);
    unsigned int *lastIP = lastAddress(broadcastIP);
    int firstOctet = networkAddress[0];
    int maskClass = getMaskClass(cidr);
    int ipClass = getIPClass(firstOctet);
    bool supernetFlag = supernet(ipClass, maskClass);

    renderOutput(networkAddress, firstIP, lastIP, broadcastIP, supernetFlag, ipClass, maskClass, cidr);

    free(firstIP);
    free(broadcastIP);
    free(lastIP);
    free(networkAddress);
    free(maskArray);
    free(ipArray);
    return 0;
}
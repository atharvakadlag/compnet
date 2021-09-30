#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void div_xor(int* a, int* b) {
    for (int i = 0; i < 4; i++)
        *(a + i) = *(a + i) ^ *(b + i);
}

int main() {
    int len;
    int* code;

    int divisor[4] = {1, 0, 1, 1};

    printf("Enter the length of the code: ");
    scanf("%d", &len);

    code = (int*)malloc((len + 3) * sizeof(int));
    memset(code, 0, (len + 3) * sizeof(int));

    // create backup of size len+3
    int* backup = (int*)malloc((len + 3) * sizeof(int));
    memset(backup, 0, (len + 3) * sizeof(int));

    printf("Enter the code: ");
    for (int i = 0; i < len; i++) {
        scanf("%d", &code[i]);
        backup[i] = code[i];
    }

    int i = 0;
    while (i < len) {
        div_xor(&code[i], divisor);
        while (code[i] == 0) i++;
    }

    printf("CRC: %d %d %d\n", code[len], code[len + 1], code[len + 2]);

    // copy first len bits from backup to code
    for (i = 0; i < len; i++) {
        code[i] = backup[i];
    }

    printf("The code is: ");
    for (int i = 0; i < len + 3; i++) {
        printf("%d ", code[i]);
    }
    printf("\n");

    // currupt message randomly
    // if(rand()%2){
    //     code[rand()%len] = rand()%2; // currupt a random bit
    //     printf("Bit corrupted...\n");
	// } else {
        printf("No bit corrupted...\n");
	// }

    // check if the code is correct
    i = 0;
    while (i < len) {
        div_xor(&code[i], divisor);
        while (code[i] == 0 && i < (len + 3)) i++;
    }

    int flag = 0;

    for (int i = 0; i < 3; i++) {
        if (code[len + i] == 1) {
            printf("The code is not correct!\n");
            return 0;
        }
    }
    printf("The code is correct!\n");

    return 0;
}
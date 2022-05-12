#include <stdio.h>
#include <msp430.h>

const int SIZE = 8;

int main() {

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    int a[SIZE][SIZE] = {{0,2,7,4,1,0,5,0},
                         {1,2,3,4,5,6,7,8},
                         {3,4,5,6,9,7,4,9},
                         {1,6,9,7,5,3,4,8},
                         {9,6,3,4,7,5,2,1},
                         {8,4,3,6,9,5,2,1},
                         {0,1,3,6,9,5,2,0},
                         {9,6,7,4,1,8,5,3}};

    int b[SIZE][SIZE] = {{0,2,7,4,1,0,5,0},
                         {1,2,3,4,5,6,7,8},
                         {3,4,5,6,9,7,4,9},
                         {1,6,9,7,5,3,4,8},
                         {9,6,3,4,7,5,2,1},
                         {8,4,3,6,9,5,2,1},
                         {0,1,3,6,9,5,2,0},
                         {9,6,7,4,1,8,5,3}};

    int c[SIZE][SIZE] = {0};

    int i, j, k;

    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            for(k = 0; k < SIZE; k++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    //print a
    printf("a = \n\n");
       for(i = 0; i < SIZE; i++ ){
           for(j = 0; j < SIZE; j++){
               printf("%6d ", b[i][j]);

               if(j== SIZE - 1) printf("\n");
           }
       }
    //print b
    printf("\n\n b = \n\n");
       for(i = 0; i < SIZE; i++ ){
           for(j = 0; j < SIZE; j++){
               printf("%6d ", b[i][j]);

               if(j== SIZE - 1) printf("\n");
           }
       }
    //print result matrix
    printf("\n\n a X b = \n\n");
    for(i = 0; i < SIZE; i++ ){
        for(j = 0; j < SIZE; j++){
            printf("%6d ", c[i][j]);

            if(j== SIZE - 1) printf("\n");
        }
    }
    return 0;
}

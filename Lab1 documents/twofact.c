/*------------------------------------------------------------------------------
* Instructor:  Aleksandar Milenkovic
* Program:     Subroutines ifact and lifact for lab1 demo  
* Date:        Aug 16, 2017
* Input:       Integer variable p
* Output:      Factorial of input (integer and long integer)
* Description: This C code calculates the factorial of a given number
               ifact gives an integer output;
               lifact gives a long integer output.
*-----------------------------------------------------------------------------*/ 
int ifact(int p) {
    int j = 0;
    int t = 1;

    if(p==0 || p==1) return 1;    // factorial of 1 and 0 is = 1
    else {
        for(j=2; j<=p; j++) t=t*j;  // Do the factorial multiplication
        return t;
    }
}


long int lifact(int p) {
    int j = 0;
    long int t = 1;

    if(p==0 || p==1) return 1;     // factorial of 1 and 0 is = 1
    else {
        for(j=2; j<=p; j++) t=t*j;   // Do the factorial multiplication
        return t;
    }
}
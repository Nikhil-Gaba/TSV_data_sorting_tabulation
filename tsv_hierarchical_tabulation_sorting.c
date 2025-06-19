/* Program to generate hierarchical reports from TSV structured data

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2024

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Nikhil Gaba
   Dated:     03/09/2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'	     /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */
#define NULL_BYTE '\0'       /* null byte character, used to end strings */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50       /* maximum field length */
#define MAXDATACOLS 30       /* maximum number of columns (fields) in data */
#define MAXDATAROWS 1000     /* maximum number of rows (entries) in data */

#define FIRSTCOLUMN 0        /* index of the first data column */
#define FIRSTROW 0           /* index of the first row */

#define HEADER 1             /* the number of rows occupied by the header */
#define HEADERINDEX 0        /* the index of the header row */

#define NEGATIVE -1          /* return if str1 is less than str2 (ascii) */
#define POSITIVE 1           /* return if str1 is greater than str2 */
#define NOCHANGE 0           /* return if str1 is identical to str2 */

#define ZEROINDEXING 1       /* handle discrepancy between 0 and 1 indexing */
#define SORTING_ARGV_IDX  1  /* argv index for primary sorting */

#define FIRST_DATA_ROW 1     /* index for 1st data row in 2D array */
#define SECOND_DATA_ROW 2    /* index for 2nd data row in 2D array */

#define LAST_COL_IDX 1       /* index for the last column printed */
#define SCND_LAST_COL_IDX 2  /* index for the 2nd last column printed */

#define INDENTATION "    "   /* Indentation for Stage 3 Output */
#define INDENTSPACES 4       /* Number of spaces required for indentation */
#define EXTRA_SPACE 6        /* Space to the right of the longest entry */
#define HYPHEN "-"           /* Required for hyphenated lines in stage 3 */

#define HALVES 2             /* We need to split the array into two halves */
#define IGNORE 35            /* a arbitrary value for unused variable */

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];

/* the whole 2 dimensional data of strings */
typedef field_t data_t[MAXDATAROWS][MAXDATACOLS];


/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);
void stage1(data_t, int *, int *);
void fill_array(data_t, char *, int, int);
void stage1_formatting (int row, int column);
void print_data(data_t, int, int);

void stage2(data_t, int, int, int, char **);
void stage2_formatting(data_t, int, int);
void sort_array(data_t, int, int, char **);
int compare_func(field_t *, field_t *, char **, int*);
void swap_row(data_t, int, int, int);
void swap_field(data_t, int, int, int);

void stage3(data_t, int, int, char **);
void print_output(field_t *, int elements, int *, int argc);
int find_max_length (field_t *, int);
void print_hypenated_lines(int);
int strings_concatenation (data_t, field_t strings[], int, int, char **, int);


/**************************************************************/

/* main program provides traffic control
*/
int
main(int argc, char *argv[]) {

    /* allocate memory for the 2D array of strings and declare integer
    variables for the dimensions of this array
    */
    data_t array; 
    int num_rows, num_cols;

    stage1(array, &num_rows, &num_cols); 

    /* if sorting parameters are supplied, progress to
    sorting and printing a report by specified parameters */

    if (argc != SORTING_ARGV_IDX) {

        stage2(array, num_rows, num_cols, argc, argv);
        stage3(array, num_rows, argc, argv);
        
    }

    /* all done, time to go home */
    printf("ta daa!\n");
    //printf("algorithms are fun!\n");

    // successful termination of the program
    return EXIT_SUCCESS;
}

/**************************************************************/

/* read characters and build a string, stopping when a tab or newline
   as encountered, with the return value indicating what that
   terminating character was
*/
int
getfield(field_t dest) {

    int ch, nchs=0;
    int status=STATUS_NML;

    dest[0] = '\0';
    while ((ch=mygetchar())
        && (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

        if (nchs < MAXFIELDLEN) {
            /* ok to save this character */
            dest[nchs++] = ch;
            dest[nchs] = '\0';
        } else {
            /* silently discard extra characters if present */
        }
    }

    /* return status is defined by what character stopped the loop */
    if (ch == EOF) {
        status = STATUS_EOF;
    } else if (ch == CHAR_NL) {
        status = STATUS_EOL;
    } else if (ch == CHAR_TB) {
        status = STATUS_NML;
    }
    return status;
}

/**************************************************************/

/* read a single character, bypassing any CR characters encountered,
   so as to work correctly with either PC-type or Unix-type input
 */
int
mygetchar(void) {
    int c;
    while ((c=getchar()) == CHAR_CR) {
        /* empty loop body */
    }
    return c;
}

/**************************************************************/

/* obtain data from standard input using getfield function and allocate data
fields to their correct position in the array, while adjusting the dimensions
of the array, each time
*/
void stage1(data_t data_array, int * rows, int * columns) {
   
    /* a data structure used to store the field obtained from standard 
    input and variables storing the dimensions of the array being filled */
    
    field_t data_val;
    int row_number = FIRSTROW, column_number = FIRSTCOLUMN, max_columns; 

    // obtain input character by character until end of input
    int data_field;
    while ((data_field = getfield(data_val)) != STATUS_EOF) {

        fill_array (data_array, data_val, row_number, column_number);
        column_number++;

        if (data_field==STATUS_EOL) {
            /* since the input's last character signifies end of a line,
            column_number will be reset to FIRSTCOLUMN, so we must store
            the number of columns in any row */
            max_columns = column_number;
            row_number++;
            column_number = FIRSTCOLUMN;
        }
    }

    stage1_formatting (row_number, max_columns);
    print_data(data_array, row_number - HEADER, max_columns);

    /* update the rows and columns pointer variables so other functions can
    access the dimensions of the array */
    *rows = row_number;
    *columns = max_columns; 

}

/* Fill the input data into the correct positions in the 2D array */
void fill_array (data_t array, char * string, int row, int column) {

    strcpy(array[row][column], string);

}

/* Format introductory stage 1 output, informing dimensions of the 2D array */
void stage1_formatting (int row, int column) {
    printf("Stage 1\n");
    printf("input tsv data has %d rows and %d columns\n" , 
        row - HEADER, column);
    printf("row %d is:\n", row - HEADER);
}

/* Print output in the required form for tasks 1 and 2 */
void print_data(data_t array, int row, int max_columns) {

    for (int i=0; i<max_columns; i++) {

        /* iterate over each column, printing the column header and its 
        corresponding value in the required row */
        printf("%4d: %-10s %s\n", i + ZEROINDEXING, 
        array[HEADERINDEX][i], array[row][i]);
    }

}

/* sorting the 2d array of strings by provided sorting parameters (columns).
Primary sorting based on the first command line argument, and tiebreaking
in the case that previous columns are the same by subsequent arguments.
*/
void stage2(data_t twodarray, int rows, int cols, int argc, char *argv[]) {
    
    printf("\nStage 2\n");
    printf("sorting by \"%s\"", 
        twodarray[HEADERINDEX][atoi(argv[SORTING_ARGV_IDX])-ZEROINDEXING]);
    for (int i=SECOND_DATA_ROW; argv[i]; i++) {
        printf(",\n   then by \"%s\"", 
        twodarray[HEADERINDEX][atoi(argv[i])-ZEROINDEXING]);
    }
    printf("\n");
    sort_array(twodarray, rows, cols, argv);

    stage2_formatting(twodarray, FIRST_DATA_ROW, cols);
    stage2_formatting(twodarray, rows/HALVES, cols);
    stage2_formatting(twodarray, rows-FIRST_DATA_ROW, cols);

    printf("\n");
}

// printing required rows for stage 2
void stage2_formatting(data_t array, int row, int cols) {
    printf("row %d is:\n", row);
    print_data(array, row, cols); 
}


/* Sorting the array using insertion sort, following the structure as specified
in Figure 7.3 on page 104 of Programming, Problem Solving and Abstraction */
void sort_array(data_t twodarray, int rows, int cols, char * argv[]) {

    /* A variable that is not important for now, instead for stage 3 */
    int unused_variable = IGNORE;

    for (int i = SECOND_DATA_ROW; i<rows; i++) {
        for (int j = i-1; (j >= FIRST_DATA_ROW) && (compare_func(twodarray[j+1],
            twodarray[j], argv, &unused_variable) < 0); j--) { 
            swap_row(twodarray, j + 1, j, cols); 
        }
    }
}

/* Comparing rows by comparing columns based on command line arguments
lexicographically, progressing to the next column if the previous columns
were the same */
int 
compare_func(field_t * str1, field_t * str2, char * argv[], int* differences) {

    int sorting_index, status;

    for (int i = SORTING_ARGV_IDX; argv[i]; i++) { 
        (*differences)--;
        /* the loop will only progress to the next iteration if status = 0 
        (i.e. the two strings are identical) */
        sorting_index = atoi(argv[i]) - ZEROINDEXING;

        if ((status = strcmp(str1[sorting_index], str2[sorting_index])) 
            < NOCHANGE) {
            return NEGATIVE;
        }
        else if (status > NOCHANGE) {
            return POSITIVE;
        }
    }

    (*differences)--;
    return NOCHANGE; // required to ensure that the array is stable
}

/* Swapping rows by interchanging columns one by one, following structure as
specified in Figure 6.7 of Programming, Problem Solving and Abstraction, 
adjusted to swap strings rather than integers */
void swap_row (data_t twodata, int row1, int row2, int cols) {

    for (int i = 0; i<cols; i++) {
        swap_field (twodata, row1, row2, i);
    }

}

/* Swapping function as specified in Figure 6.7 of Programming, Problem
Solving and Abstraction, adjusted to swap strings rather than integers */
void swap_field (data_t twodata, int row1, int row2, int col) {

    field_t temp;
    strcpy(temp, twodata[row1][col]);
    strcpy(twodata[row1][col], twodata[row2][col]);
    strcpy(twodata[row2][col], temp);
}

void stage3(data_t array, int num_rows, int argc, char * argv[]) {

    /* An array of strings which are required to be printed, with another 
    array with the corresponding counts at the same index */
    field_t strings[num_rows * argc];
    int counts[num_rows * argc];

    // Initialising the strings and count array to avoid garbage being printed
    for (int i = 0; i < num_rows * argc; i++) {
        counts[i] = 0;
        strings[i][0] = NULL_BYTE;
    }

    // A variable keeping track of the number of elements in the strings array
    int numberoutput = 0;

    /* Add the header columns by which we are sorting to the start of strings
    (with the appropriate indentation) */
    for (int i = 1; argv[i]; i++) {

        // add indentation before subsequent columns (if required)
        for (int j=1; j<i; j++) {
            strcat(strings[numberoutput], INDENTATION);
        }

        // concatenate the column to the indentation, adding to strings
        numberoutput = strings_concatenation(array, strings, numberoutput,
            HEADERINDEX, argv, i);
    }

    /* Add the columns by which row 1 has been sorted to strings in the 
    correct order with appropriate indentation */
    for (int i = 1; i < argc; i++) {
        for (int j = i; j > 1; j--) {
            strcat(strings[numberoutput], INDENTATION);
        }

        numberoutput = strings_concatenation(array, strings, 
            numberoutput, FIRST_DATA_ROW, argv, i);

    }

    /* initialising variables for count (the number of rows with the same
    sorting columns) and differences (the number of sorting columns differing
    from the previous row) */
    int count = 1;
    int differences = argc;

    for (int i = SECOND_DATA_ROW; i < num_rows; i++) {

        /* Compare each row with the previous row and if there are differences
        in the sorting columns, add the columns of the sorting indices into 
        strings with required indentation (in correct order)
        */
        if (compare_func(array[i], array[i-1], argv, &differences)) {

            counts[numberoutput-1] = count;

            for (int j = differences; j > 0; j--) { 
                for (int k = 1; k < argc - j; k++) {
                    strcat(strings[numberoutput], INDENTATION);
                } 
                /* argc - j because we are printing the last j columns,
                j being the number of columns different from the last row */
                numberoutput = strings_concatenation(array, strings, 
                    numberoutput, i, argv, argc-j);
            }

            count = 1;
            
        }
        /* if the current row is the same as the previous row by the 
        sorting indices, increment the count by 1 */
        else {
            count++;
        }

        differences = argc;
    }

    // add the counts of the last row to counts array
    counts[numberoutput - 1] = count;

    print_output(strings, numberoutput, counts, argc); 

}

/* print a report of the number of rows with certain columns, as specified by 
the command line sorting indices */
void print_output(field_t output[], int elements, int counts[], int argc) {

    printf("Stage 3\n");

    int max_string_length = find_max_length (output, elements); 

    print_hypenated_lines(max_string_length);
    printf("\n");

    // print each string in strings
    for (int i = 0; i < elements; i++) {

        // hyphenated line after the header sorting columns
        if (i == argc - LAST_COL_IDX) {
            print_hypenated_lines(max_string_length);
            printf("\n");
        }

        /* formatting left justified taking maximum_string_length space
        as suggested by Alistair Moffat */
        if (counts[i]) {
            printf("%-*s ", max_string_length, output[i]);
            printf("%5d", counts[i]);
        }

        else {

            // print "Count" in the correct place
            if (i == argc - SCND_LAST_COL_IDX) {
                printf("%-*s Count", max_string_length, output[i]);
            }
            else {
                printf("%s", output[i]);
            }

        }

        printf("\n");
    }

    print_hypenated_lines(max_string_length);

    printf("\n");
}

/* Calculate the maximum string length (the minimum length of any string
is 0) and iterating through each element, and updating maximum length */
int find_max_length (field_t output[], int elements) {

    int max_string_length = 0;

    for (int i = 0; i < elements; i++) {
        if (strlen(output[i]) > max_string_length) {
            max_string_length = strlen(output[i]); 
        }
    }
    return max_string_length;
}   

/* A function that just prints required number of hyphens for formatting */
void print_hypenated_lines (int num) {

    // last column printed one space to the right of the longest entry
    for (int i = 0; i< num + EXTRA_SPACE; i++) {
        printf(HYPHEN);
    }
}

// add the column specified by argv[argument] to strings by strcat
int strings_concatenation ( data_t array, field_t strings[], int index, 
                            int row, char * argv[], int argument) {

    strcat(strings[index], array[row][atoi(argv[argument])-ZEROINDEXING]);
    index++;
    return index;
}
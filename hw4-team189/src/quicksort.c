#include <stdio.h>
#include <string.h>
#include "quicksort.h"

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*cmp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*cmp) (const void*, const void*));

/**
 * Swaps the values in two pointers.
 *
 * Casts the void pointers to type (char *) and works with them as char pointers
 * for the remainder of the function. Swaps one byte at a time, until all 'size'
 * bytes have been swapped. For example, if ints are passed in, size will be 4
 * and this function will swap 4 bytes starting at a and b pointers.
 */
static void swap(void *a, void *b, size_t size) {
    char *A = (char *)a, *B = (char *)b;
    char temp;
    //copies array A to an empty one
    do {
        temp = *A; 
	    *A++ = *B;
	    *B++ = temp;
    } while (--size > 0);
}

/**
 * Partitions array around a pivot, utilizing the swap function. Each time the
 * function runs, the pivot is placed into the correct index of the array in
 * sorted order. All elements less than the pivot should be to its left, and all
 * elements greater than or equal to the pivot should be to its right.
 */
//so we have to use pointer arithmetic to 
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*cmp) (const void*, const void*)) {
    char *A = (char *)array;
    char* p = A+(left*elem_sz);
    int s = left;
    for (int i = left + 1; i <= right; i++) {
        int cmp_res = cmp(((char *) array + i * elem_sz), p);
        if (cmp_res < 0) {
            s++;
            swap(A+(s*elem_sz), A+(i*elem_sz), elem_sz); //we have to swap based on the bytes themselves rather than the values of the array
        }
    }
    swap(A+(left*elem_sz), A+(s*elem_sz), elem_sz);
    return s;
}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*cmp) (const void*, const void*)){
    if (len > 1) {
        quicksort_helper(array, 0, len-1, elem_sz,cmp);
    }
}


/**
 * Sorts with lomuto partitioning, with recursive calls on each side of the
 * pivot.
 * This is the function that does the work, since it takes in both left and
 * right index values.
 */
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*cmp) (const void*, const void*)) {
    int s; 
    if(left < right){
        s = lomuto(array, left, right, elem_sz, cmp);
        quicksort_helper(array, left, s-1, elem_sz, cmp);
        quicksort_helper(array, s+1, right, elem_sz, cmp);
    }
}

int int_cmp(const void *a, const void *b) {
    int aval = *(int *)a, bval = *(int*)b;
    if (aval > bval) {
        return 1;
    }
    if (aval < bval) {
        return -1;
    }
    return 0;
}

int dbl_cmp(const void *a, const void *b) {
    double aval = *(double *)a, bval = *(double *)b;
    //printf("aval = %lf, bval = %lf\n", aval, bval);
    if (aval > bval) {
        return 1;
    }
    if (aval < bval) {
        return -1;
    }
    return 0;
}

int str_cmp(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}


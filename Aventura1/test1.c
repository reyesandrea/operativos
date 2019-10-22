/*
 * This code is a test of the functions implemented in my_lib.c
 * It's required to declare the functions and types in my_lib.h
 */

#include <stdio.h>
#include <string.h>

#include "my_lib.h"

int test_strlen(char **strs, int n) { // char **strs
    int i;
    printf("\n***************************************\n");
    printf("Testeando my_strlen() frente a strlen()\n");
    printf("\n***************************************\n");
    for (i = 0; i < n; i++) {

        printf("strlen(\"%s\") = %lu\n", strs[i], strlen(strs[i]));
        printf("my_strlen(\"%s\") = %lu\n\n", strs[i], my_strlen(strs[i]));

        if (strlen(strs[i]) != my_strlen(strs[i])) {
            printf("\n%s failed :-(\n", __func__);
            return -1;
        }
    }

    printf("\n%s passed :-)\n", __func__);
    return 0;
}

int sign(int x) { return (x > 0) - (x < 0); }

int test_strcmp(char *strs[], int n) {
    int i, j;
    printf("\n***************************************\n");
    printf("Testeando my_strcmp() frente a strcmp()\n");
    printf("\n***************************************\n");
    for (i = 0; i < n; i++) {
        for (j = n - 1; j >= 0; j--) {
            printf("strcmp(\"%s\", \"%s\") = %d\n", strs[i], strs[j], strcmp(strs[i], strs[j]));
            printf("my_strcmp(\"%s\", \"%s\") = %d\n\n", strs[i], strs[j], my_strcmp(strs[i], strs[j]));
            if (sign(strcmp(strs[i], strs[j])) != sign(my_strcmp(strs[i], strs[j]))) {
                printf("\n%s failed :-(\n", __func__);

                return -1;
            }
        }
    }

    printf("\n%s passed :-)\n", __func__);
    return 0;
}

int test_strcpy(char **strs, int n) {
    int i;
    char cat1[1024];
    char cat2[1024];
    char res1[1024];
    char res2[1024];

    cat1[0] = 0; // Mark as empty strings
    cat2[0] = 0;

    printf("\n***************************************\n");
    printf("Testeando my_strcpy() frente a strcpy()\n");
    printf("\n***************************************\n");
    for (i = 0; i < n; i++) {

        res1[0] = 0;
        res2[0] = 0;

        printf("strcpy(\"%s\", \"%s\") = ", cat1, strs[i]);
        printf("\"%s\"\n", strcpy(res1, strcpy(cat1, strs[i])));

        printf("my_strcpy(\"%s\", \"%s\") = ", cat2, strs[i]);
        printf("\"%s\"\n\n", strcpy(res2, my_strcpy(cat2, strs[i])));

        if (strcmp(cat2, cat1) != 0 || strcmp(res2, res1) != 0) {
            printf("\n%s failed :-(\n", __func__);
            return -1;
        }
    }

    printf("\n%s passed :-)\n", __func__);
    return 0;
}

int test_strncpy(char *strs[], int n) {

    int i, len;
    char cat1[1024];
    char cat2[1024];
    char res1[1024];
    char res2[1024];

    memset(cat1, 0, sizeof(cat1));
    memset(cat2, 0, sizeof(cat2));

    printf("\n*****************************************\n");
    printf("Testeando my_strncpy() frente a strncpy()\n");
    printf("\n*****************************************\n");

    for (i = 0; i < n; i++) {
        memset(cat1, 0, sizeof(cat1)); //anteriormente comentado
        memset(cat2, 0, sizeof(cat2)); //anteriormente comentado
        for (len = 1; len < strlen(strs[i]) + 3; len++) {
            memset(res1, 0, sizeof(res1));
            memset(res2, 0, sizeof(res2));

            printf("strncpy(\"%s\", \"%s\", %d) = ", cat1, strs[i], len);
            printf(" \"%s\"\n", strcpy(res1, strncpy(cat1, strs[i], len)));

            printf("my_strncpy(\"%s\", \"%s\", %d) = ", cat2, strs[i], len);
            printf(" \"%s\"\n\n", strcpy(res2, my_strncpy(cat2, strs[i], len)));

            if (strcmp(cat2, cat1) != 0 || strcmp(res2, res1) != 0) {
                printf("%s failed for len %d :-(\n", __func__, len);
                printf("dest con strcpy: %s\t", cat1);
                printf("dest con my_strcpy: %s\n", cat2);
                printf("return con strcpy: %s\t", res1);
                printf("return con my_strcpy: %s\n", res2);
                return -1;
            }
        }
    }

    
    //strcpy(cat1, strs[0]);
    //strcpy(cat2, strs[0]);

    //printf("strncpy(\"%s\", \"%s\", %d) = ", cat1, strs[1], len);
    //printf(" \"%s\"\n", strcpy(res1, strncpy(cat1, strs[1], len)));

    //printf("my_strncpy(\"%s\", \"%s\", %d) = ", cat2, strs[1], len);
    //printf(" \"%s\"\n\n", strcpy(res2, my_strncpy(cat2, strs[1], len)));
    

    printf("%s passed :-)\n", __func__);
    return 0;
}

int test_strcat(char **strs, int n) {
    int i;
    char cat1[1024];
    char cat2[1024];
    char res1[1024];
    char res2[1024];

    //cat1[0] = 0; // Mark as empty strings
    //cat2[0] = 0;
    memset(cat1, 0, sizeof(cat1));
    memset(cat2, 0, sizeof(cat2));

    printf("\n***************************************\n");
    printf("Testeando my_strcat() frente a strcat()\n");
    printf("\n***************************************\n");
    for (i = 0; i < n; i++) {
        memset(res1, 0, sizeof(res1)); //res1[0] = 0;
        memset(res2, 0, sizeof(res2)); //res2[0] = 0;
        printf("strcat(\"%s\", \"%s\") = ", cat1, strs[i]);
        printf("\"%s\"\n", strcpy(res1, strcat(cat1, strs[i])));

        printf("my_strcat(\"%s\", \"%s\") = ", cat2, strs[i]);
        printf("\"%s\"\n\n", strcpy(res2, my_strcat(cat2, strs[i])));
    }

    if (strcmp(cat2, cat1) != 0 || strcmp(res2, res1) != 0) {
        printf("\n%s failed :-(\n", __func__);
        return -1;
    }

    printf("\n%s passed :-)\n", __func__);

    return 0;
}

int main() {
    const int n = 3;
    char *strs[n];

    // Init the array of char * with several strings and NULL at the end
    strs[0] = "programa";   // longitud 8
    strs[1] = "compilador"; // longitud 10 
    strs[2] = "depurador";  // longitud 9 

    test_strlen(strs, n);
    test_strcmp(strs, n);
    test_strcpy(strs, n);
    test_strncpy(strs, n);
    test_strcat(strs, n);

    return 0; // To avoid warning in -Wall
}

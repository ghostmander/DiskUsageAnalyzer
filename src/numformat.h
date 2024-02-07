#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Format a number to a human readable string
 * @param num uint_64 - The number to format
 * @return result char* - The result string
 *
 * @example 1024 --> 1,024
 * @example 123456789 --> 12,34,56,789
 * @example 1234567890 --> 1,23,45,67,890
 */
char *formatNumber(uint_64 num) {
    char *result = (char *) malloc(20 * sizeof(char));
    char *temp = (char *) malloc(20 * sizeof(char));
    sprintf(temp, "%llu", num);
    int len = (int) strlen(temp);
    int j = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (j % 3 == 0 && j != 0) result[j++] = ',';
        result[j++] = temp[i];
    }
    result[j] = '\0';
    free(temp);
    return strrev(result);
}

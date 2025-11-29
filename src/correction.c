#include <string.h>
#include "correction.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int levenstein(char* a, char* b) {
    int n = strlen(a);
    int m = strlen(b);
    int edits[n + 1][m + 1];

    for (int i = 0; i < n + 1; i++) {
        edits[i][0] = i;
    }

    for (int i = 0; i < m + 1; i++) {
        edits[0][i] = i;
    }

    for (int x = 1; x < n + 1; x++) {
        for (int y = 1; y < m + 1; y++) {
            int sub_cost = (a[x - 1] != b[y - 1]) ? 1 : 0;
            edits[x][y] = MIN(MIN(edits[x - 1][y], edits[x][y - 1]) + 1,
                                    edits[x - 1][y - 1] + sub_cost);
        }
    }

    return edits[n][m];
}
#include <string.h>
#include <stdio.h>
#include "const.h"
#include "correction.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int levenstein(const char* a, const char* b) {
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

// Returns 0 if the closest match is the word
int closest_match(const char* word, long* offsets, char* match) {
    int len = strlen(word);

    if (len > MAX_LEN || offsets[len] == -1)
        return 0;

    long offset = offsets[len];

    char best[len + 2];
    int min_dist = MAX_LEN + 1;

    FILE *f = fopen(FILE_PATH, "r");
    if (!f) return -1;

    if (fseek(f, offset, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }

    char buffer[MAX_LEN + 2];

    while (fgets(buffer, sizeof(buffer), f)) {
        int length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n')
            buffer[--length] = '\0';

        if (length != len)
            break;

        int dist = levenstein(buffer, word);
        if (dist < min_dist) {
            min_dist = dist;
            strcpy(best, buffer);
            if (dist == 0) break; // early exit
        }
    }

    fclose(f);

    if (min_dist <= MAX_LEN)
        strcpy(match, best);

    return min_dist;
}
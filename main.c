#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "read.h"
#include "correction.h"
#include "const.h"

/*
TO DO:
Make old buffers into one message to save computing power.                      |
Make backspaces work over multiple lines.                                       | DONE
Add cursor functionality.                                                       | DONE
Make the text into a savable file (basic).                                      |
Make the text into a savable file (call windows/linux select folder API).       |
Add autocorrect functionality.                                                  |
*/

int main(int argc, char** argv) {
    // start_console();
    long* offs = offsets(FILE_PATH);

    char word[32];
    printf("Word to match: ");
    scanf(" %31s", word);

    char match[32];
    if (closest_match(word, offs, match))
        printf("Closest match: %s", match);
    else 
        printf("Closest match: %s", word);
        
    return 0;
}
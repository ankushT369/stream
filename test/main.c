#include "../stream.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int fd = open("test.txt", O_RDONLY);

    /* Create a stream */
    stream* s = stream_create(fd, 256);

    char* data;
    /* Read the data in loop */
    while(((data = stream_read(s)) != NULL)) {
        printf("BUFFER SIZE %zu\n", stream_size(s));
        printf("%s\n", data);
    }

    /* Free the stream */
    stream_free(s);

    return 0;
}


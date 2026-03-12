/* 
 * Allocate a single buffer of configured size
 * then repeatedly use that single buffer
 */
#include "stream.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>


typedef struct dynamic_mem dmem;

typedef struct stream {
    int fd;
    size_t pos;
    size_t buf_size;

    char* buffer;
    size_t len;     // len is the sent current data len
} stream;

/* Static declarations */
static char* stream_databuf_alloc(stream* s);
static size_t align_4k(size_t size);

static char* stream_databuf_alloc(stream* s) {
    char* data;
    data = calloc(1, align_4k(s->buf_size));
    if (!data) return NULL;

    return data;
}

static size_t align_4k(size_t size) {
    const size_t PAGE = 4096;

    if (size <= PAGE)
        return PAGE;

    return (size + (PAGE - 1)) & ~(PAGE - 1);
}


/* API implementations */
stream* stream_create(int fd, size_t size) {
    char* buffer;
    if (fd < 0) return NULL;

    stream* reader = calloc(1, sizeof(stream));
    if (!reader) {
        printf("Error failed to allocate memory!!\n");
        return NULL;
    }

    reader->fd = fd;
    reader->pos = 0;
    reader->buf_size = (size <= 0) ? STREAM_BUFSIZE : size;

    buffer = stream_databuf_alloc(reader);

    if (!buffer) {
        free(reader);
        return NULL;
    }

    reader->buffer = buffer;
    return reader;
}

char* stream_read(stream* s) {
    ssize_t n;
    if (!s) return NULL;

    do {
        n = pread(s->fd, s->buffer, s->buf_size, s->pos);
    } while(n < 0 && errno == EINTR);


    if (n == 0) {
        s->len = 0;
        return NULL;
    }

    if (n < 0) {
        // error
        s->len = 0;
        return NULL;
    }

    s->pos += (size_t)n;
    s->len = (size_t)n;
    s->buffer[s->len] = '\0';
    return s->buffer;
}

size_t stream_size(stream* s) {
    return s->len;
}

void stream_free(stream* s) {
    if (!s) return;
    free(s->buffer);
    free(s);
}

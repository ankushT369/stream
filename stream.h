#ifndef STREAM_H
#define STREAM_H

#include <stddef.h>

typedef struct stream stream;
typedef struct stream_data stream_data;


#define STREAM_BUFSIZE 4096

/* API */
stream* stream_create(int fd, size_t size);

char* stream_read(stream* s);
size_t stream_size(stream* s);

void stream_free(stream* s);


#endif // STREAM_H

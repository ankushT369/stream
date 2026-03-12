# stream — Minimal Binary Streaming Library (C)

**stream** is a tiny, fast, allocation‑aware streaming helper for C.  
It gives you a reusable, page‑aligned buffer and a clean API for reading binary data from file descriptors using `pread()`.

Designed for:

*   binary files
*   structured binary formats
*   fast sequential/positional reads
*   minimal overhead and zero hidden copies


## Features

*   **Single reusable buffer** (page‑aligned to 4KB)
*   **Binary‑safe** — no null‑termination, no string assumptions
*   **Positional reads** using `pread()`
*   **User‑controlled read size**
*   **No unnecessary zeroing or clearing**
*   **Small, clean API**



##  API Overview

```c
stream* stream_create(int fd, size_t size);
char*   stream_read(stream* s);     // returns pointer to internal buffer
size_t  stream_size(stream* s);     // number of valid bytes in buffer
void    stream_free(stream* s);
```

`stream_read()` returns raw bytes.  
Use `stream_size()` to know how many bytes are valid.

The buffer is reused every call.



##  Usage Example

```c
int fd = open("file.bin", O_RDONLY);
stream* s = stream_create(fd, 1024);   // request size; internally aligned to 4KB

while (1) {
    char* buf = stream_read(s);
    if (!buf) break;                   // EOF or error
    size_t n = stream_size(s);

    // consume n bytes from buf (binary-safe)
}

stream_free(s);
close(fd);
```

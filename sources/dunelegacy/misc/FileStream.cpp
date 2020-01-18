/*
  This is part of libdunemusic.
  It's a replacement of SDL rwops.
*/

#include "FileStream.h"
#include <cstring>

FileStream::FileStream(FILE *fh, bool autoclose)
    : fh(fh), autoclose(autoclose)
{
}

FileStream::~FileStream()
{
    Close();
}

FileStream *FileStream::Open(const char *filename, const char *mode)
{
    FILE *fh = fopen(filename, mode);
    if (fh == nullptr)
        return nullptr;
    return new FileStream(fh, true);
}

size_t FileStream::Read(void *ptr, size_t size, size_t n)
{
    if (!fh)
        return 0;
    return fread(ptr, size, n, fh);
}

size_t FileStream::Write(const void *ptr, size_t size, size_t n)
{
    if (!fh)
        return 0;
    return fwrite(ptr, size, n, fh);
}

int64_t FileStream::Size()
{
    int64_t pos = ftell(fh);
    if (pos == -1)
        return -1;
    if (fseek(fh, 0, SEEK_END) == -1)
        return -1;
    int64_t size = ftell(fh);
    if (fseek(fh, pos, SEEK_SET) == -1)
        return -1;
    return size;
}

int64_t FileStream::Seek(int64_t offset, int whence)
{
    if (!fh || fseek(fh, offset, whence) == -1)
        return -1;
    return ftell(fh);
}

int FileStream::Close()
{
    int ret = 0;
    if (autoclose) {
        if (fh)
            ret = fclose(fh);
        else
            ret = -1;
    }
    fh = nullptr;
    return ret;
}

//==============================================================================
MemStream::MemStream(void *mem, size_t size)
    : base((uint8_t *)mem), here((uint8_t *)mem), stop((uint8_t *)mem + size), ro(false)
{
}

MemStream::MemStream(const void *mem, size_t size)
    : base((uint8_t *)mem), here((uint8_t *)mem), stop((uint8_t *)mem + size), ro(true)
{
}

size_t MemStream::Read(void *ptr, size_t size, size_t n)
{
    size_t total_bytes;
    size_t mem_available;

    total_bytes = (n * size);
    if ((n <= 0) || (size <= 0)
        || ((total_bytes / n) != size)) {
        return 0;
    }

    mem_available = (stop - here);
    if (total_bytes > mem_available) {
        total_bytes = mem_available;
    }

    memcpy(ptr, here, total_bytes);
    here += total_bytes;

    return (total_bytes / size);
}

size_t MemStream::Write(const void *ptr, size_t size, size_t n)
{
    if (ro)
        return 0;

    if ((here + (n * size)) > stop) {
        n = (stop - here) / size;
    }
    memcpy(here, ptr, n * size);
    here += n * size;
    return n;
}

int64_t MemStream::Size()
{
    return (int64_t)(stop - base);
}

int64_t MemStream::Seek(int64_t offset, int whence)
{
    uint8_t *newpos;

    switch (whence) {
    case SEEK_SET:
        newpos = base + offset;
        break;
    case SEEK_CUR:
        newpos = here + offset;
        break;
    case SEEK_END:
        newpos = stop + offset;
        break;
    default:
        return -1;
    }
    if (newpos < base) {
        newpos = base;
    }
    if (newpos > stop) {
        newpos = stop;
    }
    here = newpos;
    return (int64_t)(here - base);
}

int MemStream::Close()
{
    return 0;
}

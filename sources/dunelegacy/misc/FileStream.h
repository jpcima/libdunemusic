/*
  This is part of libdunemusic.
  It's a replacement of SDL rwops.
*/

#pragma once
#include <cstdio>
#include <cstdint>
#include <cstddef>

class AbstractStream {
public:
    virtual ~AbstractStream() {}

    virtual size_t Read(void *ptr, size_t size, size_t n) = 0;
    virtual size_t Write(const void *ptr, size_t size, size_t n) = 0;
    virtual int64_t Size() = 0;
    virtual int64_t Seek(int64_t offset, int whence) = 0;
    virtual int Close() = 0;
};

class FileStream : public AbstractStream {
public:
    FileStream(FILE *fh, bool autoclose);
    ~FileStream();

    static FileStream *Open(const char *filename, const char *mode);

    FileStream(const FileStream &) = delete;
    FileStream &operator=(const FileStream &) = delete;

    size_t Read(void *ptr, size_t size, size_t n) override;
    size_t Write(const void *ptr, size_t size, size_t n) override;
    int64_t Size() override;
    int64_t Seek(int64_t offset, int whence) override;
    int Close() override;

private:
    FILE *fh = nullptr;
    bool autoclose = false;
};

class MemStream : public AbstractStream {
public:
    MemStream(void *mem, size_t size);
    MemStream(const void *mem, size_t size);

    size_t Read(void *ptr, size_t size, size_t n) override;
    size_t Write(const void *ptr, size_t size, size_t n) override;
    int64_t Size() override;
    int64_t Seek(int64_t offset, int whence) override;
    int Close() override;

private:
    uint8_t *base;
    uint8_t *here;
    uint8_t *stop;
    bool ro;
};

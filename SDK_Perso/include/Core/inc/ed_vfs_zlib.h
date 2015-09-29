#ifndef _ED_VFS_ZLIB_H_
#define _ED_VFS_ZLIB_H_

// ed_vfs -> zlib ioapi bridge

#ifdef __cplusplus
extern "C" {
#endif

static
voidpf ZCALLBACK vfs_zlib_open(voidpf opaque, const char *filename, int mode)
{
    enum {
        Z_WRITE = ZLIB_FILEFUNC_MODE_CREATE | ZLIB_FILEFUNC_MODE_WRITE,
        Z_READ = ZLIB_FILEFUNC_MODE_EXISTING | ZLIB_FILEFUNC_MODE_READ,
    };
    if ((mode & Z_READ) == Z_READ)
        return VFS_open_read(filename);
    else if ((mode & Z_WRITE) == Z_WRITE)
        return VFS_open_write(filename);
    else
        return NULL;
}

static
uLong vfs_zlib_read(voidpf opaque, voidpf stream, void *buf, uLong size)
{
    uint64_t res = VFS_read((VFS_File *) stream, buf, size);
    return (uLong) res;
}

static
uLong vfs_zlib_write(voidpf opaque, voidpf stream, const void *buf, uLong size)
{
    uint64_t res = VFS_write((VFS_File *) stream, buf, size);
    return (uLong) res;
}

static
long vfs_zlib_tell(voidpf opaque, voidpf stream)
{
    return (long) VFS_tell((VFS_File *) stream);
}

static
long vfs_zlib_seek(voidpf opaque, voidpf stream, uLong offset, int origin)
{
    uint64_t off = (uint64_t) offset;
    if (origin == ZLIB_FILEFUNC_SEEK_CUR)
        off += VFS_tell((VFS_File *) stream);
    else if (origin == ZLIB_FILEFUNC_SEEK_END)
        off += VFS_getsize((VFS_File *) stream);
    return VFS_seek((VFS_File *) stream, off) ? 0 : -1;
}

static
int vfs_zlib_close(voidpf opaque, voidpf stream)
{
    VFS_close((VFS_File *) stream);
    return 0;
}

static
int vfs_zlib_testerror(voidpf opaque, voidpf stream)
{
    return 0;
}

static zlib_filefunc_def VFS_zlib_io = {
    vfs_zlib_open,
    vfs_zlib_read,
    vfs_zlib_write,
    vfs_zlib_tell,
    vfs_zlib_seek,
    vfs_zlib_close,
    vfs_zlib_testerror,
    NULL
};


#ifdef __cplusplus
} // extern "C"
#endif


#endif /* _ED_VFS_ZLIB_H_ */

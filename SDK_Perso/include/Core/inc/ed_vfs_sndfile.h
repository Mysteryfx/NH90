/**
 * ed_vfs -> SF_VIRTUAL_IO bridge
 *
 */
#ifndef _ED_VFS_SNDFILE_H_
#define _ED_VFS_SNDFILE_H_

//#include "sndfile.h"
//#include "ed_vfs.h"

#ifdef __cplusplus
extern "C" {
#endif


static
sf_count_t vfs_sf_get_filelen(void *userdata)
{
    return VFS_getsize((VFS_File*) userdata);
}

static
sf_count_t vfs_sf_seek(sf_count_t offset, int whence, void *userdata)
{
    VFS_File *f = (VFS_File*) userdata;
    if (whence == SEEK_CUR)
        offset += VFS_tell(f);
    if (whence == SEEK_END)
        offset += VFS_getsize(f);
    if (VFS_seek(f, offset))
        return VFS_tell(f);
    else
        return -1;
}

static
sf_count_t vfs_sf_read(void *buf, sf_count_t sz, void *userdata)
{
    return VFS_read((VFS_File*) userdata, buf, sz);
}

static
sf_count_t vfs_sf_write(const void *buf, sf_count_t sz, void *userdata)
{
    return VFS_write((VFS_File*) userdata, buf, sz);
}

static
sf_count_t vfs_sf_tell(void *userdata)
{
    return VFS_tell((VFS_File*) userdata);
}


static struct SF_VIRTUAL_IO VFS_sndfile_io = {
    vfs_sf_get_filelen,
    vfs_sf_seek,
    vfs_sf_read,
    vfs_sf_write,
    vfs_sf_tell,
};


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _ED_VFS_SNDFILE_H_ */

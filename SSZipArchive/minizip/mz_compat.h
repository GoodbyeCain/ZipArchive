/* mz_compat.h -- Backwards compatible interface for older versions
   Version 2.8.6, April 8, 2019
   part of the MiniZip project

   Copyright (C) 2010-2019 Nathan Moinvaziri
     https://github.com/nmoinvaz/minizip
   Copyright (C) 1998-2010 Gilles Vollant
     https://www.winimage.com/zLibDll/minizip.html

   This program is distributed under the terms of the same license as zlib.
   See the accompanying LICENSE file for the full text of the license.
*/

#ifndef MZ_COMPAT_H
#define MZ_COMPAT_H

#include "mz.h"
#include "../SSZipCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************/

#if defined(HAVE_ZLIB) && defined(MAX_MEM_LEVEL)
#ifndef DEF_MEM_LEVEL
#  if MAX_MEM_LEVEL >= 8
#    define DEF_MEM_LEVEL 8
#  else
#    define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#  endif
#endif
#endif
#ifndef MAX_WBITS
#define MAX_WBITS     15
#endif
#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef ZEXPORT
#  define ZEXPORT MZ_EXPORT
#endif

/***************************************************************************/

#if defined(STRICTZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */
typedef struct TagzipFile__ { int unused; } zip_file__;
typedef zip_file__ *zipFile;
#else
typedef void *zipFile;
#endif

/***************************************************************************/

typedef void *zlib_filefunc_def;
typedef void *zlib_filefunc64_def;
typedef const char *zipcharpc;

typedef struct tm tm_unz;
typedef struct tm tm_zip;

typedef uint64_t ZPOS64_T;

/***************************************************************************/

// ZipArchive 2.x uses dos_date
#define MZ_COMPAT_VERSION 120

#if MZ_COMPAT_VERSION <= 110
#define mz_dos_date dosDate
#else
#define mz_dos_date dos_date
#endif

typedef struct
{
    uint32_t    mz_dos_date;
    struct tm   tmz_date;
    uint16_t    internal_fa;        /* internal file attributes        2 bytes */
    uint32_t    external_fa;        /* external file attributes        4 bytes */
} mz_zip_fileinfo;

/***************************************************************************/

#define ZIP_OK                          (0)
#define ZIP_EOF                         (0)
#define ZIP_ERRNO                       (-1)
#define ZIP_PARAMERROR                  (-102)
#define ZIP_BADZIPFILE                  (-103)
#define ZIP_INTERNALERROR               (-104)

#define Z_BZIP2ED                       (12)

#define APPEND_STATUS_CREATE            (0)
#define APPEND_STATUS_CREATEAFTER       (1)
#define APPEND_STATUS_ADDINZIP          (2)

/***************************************************************************/
/* Writing a zip file  */

ZEXPORT zipFile mzZipOpen(const char *path, int append);
ZEXPORT zipFile mzZipOpen64(const void *path, int append);
ZEXPORT zipFile mzZipOpen2(const char *path, int append, const char **globalcomment,
    zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT zipFile mzZipOpen2_64(const void *path, int append, const char **globalcomment,
    zlib_filefunc64_def *pzlib_filefunc_def);
        zipFile mzZipOpen_MZ(void *stream, int append, const char **globalcomment);

ZEXPORT int     mzZipOpenNewFileInZip5(zipFile file, const char *filename, const mz_zip_fileinfo *zipfi,
    const void *extrafield_local, uint16_t size_extrafield_local, const void *extrafield_global,
    uint16_t size_extrafield_global, const char *comment, uint16_t compression_method, int level,
    int raw, int windowBits, int memLevel, int strategy, const char *password,
    signed char aes, uint16_t version_madeby, uint16_t flag_base, int zip64);

ZEXPORT int     mzZipWriteInFileInZip(zipFile file, const void *buf, uint32_t len);

ZEXPORT int     mzZipCloseFileInZipRaw(zipFile file, uint32_t uncompressed_size, uint32_t crc32);
ZEXPORT int     mzZipCloseFileInZipRaw64(zipFile file, int64_t uncompressed_size, uint32_t crc32);
ZEXPORT int     mzZipCloseFileInZip(zipFile file);
ZEXPORT int     mzZipCloseFileInZip64(zipFile file);

ZEXPORT int     mzZipClose(zipFile file, const char *global_comment);
ZEXPORT int     mzZipClose_64(zipFile file, const char *global_comment);
ZEXPORT int     mzZipClose2_64(zipFile file, const char *global_comment, uint16_t version_madeby);
        int     mzZipClose_MZ(zipFile file, const char *global_comment);
        int     mzZipClose2_MZ(zipFile file, const char *global_comment, uint16_t version_madeby);
ZEXPORT void*   mzZipGetStream(zipFile file);

/***************************************************************************/

#if defined(STRICTUNZIP) || defined(STRICTZIPUNZIP)
/* like the STRICT of WIN32, we define a pointer that cannot be converted
    from (void*) without cast */
typedef struct TagunzFile__ { int unused; } unz_file__;
typedef unz_file__ *unzFile;
#else
typedef void *unzFile;
#endif

/***************************************************************************/

#define UNZ_OK                          (0)
#define UNZ_END_OF_LIST_OF_FILE         (-100)
#define UNZ_ERRNO                       (-1)
#define UNZ_EOF                         (0)
#define UNZ_PARAMERROR                  (-102)
#define UNZ_BADZIPFILE                  (-103)
#define UNZ_INTERNALERROR               (-104)
#define UNZ_CRCERROR                    (-105)
#define UNZ_BADPASSWORD                 (-106)

/***************************************************************************/

typedef int (*unzFileNameComparer)(unzFile file, const char *filename1, const char *filename2);
typedef int (*unzIteratorFunction)(unzFile file);
typedef int (*unzIteratorFunction2)(unzFile file, unz_file_info64 *pfile_info, char *filename,
    uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
    uint16_t comment_size);

/***************************************************************************/
/* Reading a zip file */

ZEXPORT unzFile mzunzOpen(const char *path);
ZEXPORT unzFile mzunzOpen64(const void *path);
ZEXPORT unzFile mzunzOpen2(const char *path, zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT unzFile mzunzOpen2_64(const void *path, zlib_filefunc64_def *pzlib_filefunc_def);
        unzFile mzunzOpen_MZ(void *stream);

ZEXPORT int     mzunzClose(unzFile file);
        int     mzunzClose_MZ(unzFile file);

ZEXPORT int     mzunzGetGlobalInfo(unzFile file, unz_global_info* pglobal_info32);
ZEXPORT int     mzunzGetGlobalInfo64(unzFile file, unz_global_info64 *pglobal_info);
ZEXPORT int     unzGetGlobalComment(unzFile file, char *comment, uint16_t comment_size);

ZEXPORT int     mzunzOpenCurrentFile(unzFile file);
ZEXPORT int     mzunzOpenCurrentFilePassword(unzFile file, const char *password);
ZEXPORT int     mzunzOpenCurrentFile2(unzFile file, int *method, int *level, int raw);
ZEXPORT int     mzunzOpenCurrentFile3(unzFile file, int *method, int *level, int raw, const char *password);
ZEXPORT int     mzunzReadCurrentFile(unzFile file, void *buf, uint32_t len);
ZEXPORT int     mzunzCloseCurrentFile(unzFile file);


ZEXPORT int     mzunzGetCurrentFileInfo(unzFile file, unz_file_info *pfile_info, char *filename,
    uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
    uint16_t comment_size);
ZEXPORT int     mzunzGetCurrentFileInfo64(unzFile file, unz_file_info64 * pfile_info, char *filename,
    uint16_t filename_size, void *extrafield, uint16_t extrafield_size, char *comment,
    uint16_t comment_size);

ZEXPORT int     mzunzGoToFirstFile(unzFile file);
ZEXPORT int     mzunzGoToNextFile(unzFile file);
ZEXPORT int     mzunzLocateFile(unzFile file, const char *filename, unzFileNameComparer filename_compare_func);

ZEXPORT int     mzunzGetLocalExtrafield(unzFile file, void *buf, unsigned int len);

/***************************************************************************/
/* Raw access to zip file */

typedef struct unz_file_pos_s
{
    uint32_t pos_in_zip_directory;  /* offset in zip file directory */
    uint32_t num_of_file;           /* # of file */
} unz_file_pos;

ZEXPORT int     mzunzGetFilePos(unzFile file, unz_file_pos *file_pos);
ZEXPORT int     mzunzGoToFilePos(unzFile file, unz_file_pos *file_pos);

typedef struct unz64_file_pos_s
{
    int64_t  pos_in_zip_directory;   /* offset in zip file directory  */
    uint64_t num_of_file;            /* # of file */
} unz64_file_pos;

ZEXPORT int     mzunzGetFilePos64(unzFile file, unz64_file_pos *file_pos);
ZEXPORT int     mzunzGoToFilePos64(unzFile file, const unz64_file_pos *file_pos);

ZEXPORT int64_t mzunzGetOffset64(unzFile file);
ZEXPORT int32_t mzunzGetOffset(unzFile file);
ZEXPORT int     mzunzSetOffset64(unzFile file, int64_t pos);
ZEXPORT int     mzunzSetOffset(unzFile file, uint32_t pos);
ZEXPORT int64_t mzunztell(unzFile file);
ZEXPORT int32_t mzunzTell(unzFile file);
ZEXPORT int64_t mzunzTell64(unzFile file);
ZEXPORT int     mzunzSeek(unzFile file, int32_t offset, int origin);
ZEXPORT int     mzunzSeek64(unzFile file, int64_t offset, int origin);
ZEXPORT int     mzunzEndOfFile(unzFile file);
ZEXPORT void*   mzunzGetStream(unzFile file);

/***************************************************************************/

ZEXPORT void mzfill_fopen_filefunc(zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT void mzfill_fopen64_filefunc(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void mzfill_win32_filefunc(zlib_filefunc_def *pzlib_filefunc_def);
ZEXPORT void mzfill_win32_filefunc64(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void mzfill_win32_filefunc64A(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void mzfill_win32_filefunc64W(zlib_filefunc64_def *pzlib_filefunc_def);
ZEXPORT void mzfill_memory_filefunc(zlib_filefunc_def *pzlib_filefunc_def);

/***************************************************************************/

#ifdef __cplusplus
}
#endif

#endif

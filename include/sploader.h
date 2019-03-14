/*
** File: include/sploader.h
**
** Author: bringey
**
** Header for the sploader utility library used by both the loader and tooling.
** Note that this library is compiled twice, one for the tooling and one for
** the loader. The SPLOADERK definition, when defined, will define functions
** pertaining to the loader version. If this definition is not defined, then
** the tooling version is used instead.
**
** Functions in both versions:
**  spl_check
**  spl_checkBin
**  spl_crc32
**  spl_crc32_acc
**  spl_crc32_end
**  spl_reverse32
**  spl_mbr_read
**  spl_gpt_read
**
** Functions in the tooling library (SPLOADERK is not defined):
**  spl_setChecksum
*/


#ifndef _SPLOADER_H
#define _SPLOADER_H


//
// Architecture values (same as ELF)
//
#define SPL_ARCH_NONE           0x0
#define SPL_ARCH_SPARC          0x2
#define SPL_ARCH_X86            0x3
#define SPL_ARCH_MIPS           0x8
#define SPL_ARCH_POWERPC        0x14
#define SPL_ARCH_ARM            0x28
#define SPL_ARCH_SUPERH         0x2A
#define SPL_ARCH_IA64           0x32
#define SPL_ARCH_X86_64         0x3E
#define SPL_ARCH_AARCH64        0xB7

//
// Endianess
//
#define SPL_ENDIAN_LITTLE       0x01
#define SPL_ENDIAN_BIG          0x02


//
// Signature "SPLOADER"
//
#define SPL_HEADER_SIGNATURE_HI 0x52454441   // "ADER" little-endian
#define SPL_HEADER_SIGNATURE_LO 0x4F4C5053   // "SPLO" little-endian
#define SPL_HEADER_SIGNATURE 0x524544414F4C5053ULL
#define SPL_HEADER_SIGNATURE_STR "SPLOADER"

//
// Header byte offsets (for assembly)
//
#define SPL_HEADER_SIGNATURE    0x0
#define SPL_HEADER_VERSION      0x8
#define SPL_HEADER_ENDIAN       0x29
#define SPL_HEADER_ARCH         0x2A
#define SPL_HEADER_HEADERCRC    0x2C
#define SPL_HEADER_LOADERCRC    0x30
#define SPL_HEADER_LOADER_SIZE  0x34
#define SPL_HEADER_FLAGS        0x38
#define SPL_HEADER_LABEL        0x3C
#define SPL_HEADER_PARTITION    0x40

//
// Flags
//

// if enabled, the partition marked as 'active' in the disk label will be used
// as the boot partition. If disabled, the partition set in SplHeader::partition
// will be used instead.
#define SPL_HEADER_FLAG_ACTIVE  0x1

//
// Error codes
//
#define SPL_HEADER_SUCCESS 0
#define SPL_HEADER_INVALID 1
#define SPL_HEADER_INTEGRITY 2

#define SPL_CRC32_INIT       0xFFFFFFFF
#define SPL_CRC32_POLYNOMIAL 0x04C11DB7

//
// device is read only, (only for sploaderk)
//
#define SPL_DEVICE_FLAG_RO 0x1

#ifndef __ASM__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


//
// Abstraction for a block-addressable device. This provides an interface that
// is used by both the loader and the tooling. Devices can be physical hard
// disks, raw disk images, etc, as long as it is block-addressable.
//
typedef struct SplDev SplDev;

//
// Structure containing the partition table of a disk label
//
typedef struct SplLabel SplLabel;

//
// Structure representing a disk partition using LBA
//
typedef struct SplPart SplPart;

//
// Utility structure for a fixed size buffer.
//
typedef struct SplBuf {
    size_t size;
    // buffer location pointer
    union {
        uint8_t *loc8;
        uint16_t *loc16;
        uint32_t *loc32;
        uint64_t *loc64;
        char *str;
        void *loc;
    };
} SplBuf;

// enum for disk labels recognized by sploader
// each value for an enum must be a power of 2

typedef enum SplLabelKind {

    SPL_DISK_LABEL_UNKNOWN = 0x0,
    SPL_DISK_LABEL_NONE    = 0x1,
    SPL_DISK_LABEL_MBR     = 0x2,
    SPL_DISK_LABEL_GPT     = 0x4

} SplLabelKind;

typedef struct SplDevInfo {
    uint64_t totalBlocks;
    uint32_t blocksize;
    uint32_t maxBlocksPerRead;
    uint32_t maxBlocksPerWrite;
    uint32_t flags;
} SplDevInfo;

typedef struct SplLabel {
    SplDev *dev;
    SplLabelKind kind;
    size_t tablesize;
    SplPart *table;
} SplLabel;

struct SplPart {
    uint32_t num;
    bool active;
    uint64_t startLba;
    uint64_t endLba;
};

struct SplHeader_s {

    uint8_t signature[8];       // must be "SPLOADER"
    uint8_t version[33];        // version string: <major>.<minor>.<patch>
    uint8_t endian;             // endianess of the header
    uint16_t arch;              // target architecture
    uint32_t headerCrc;         // CRC32 checksum of the header
    uint32_t loaderCrc;         // CRC32 checksum of the loader binary
    uint32_t loaderSize;        // size of the loader binary, in bytes

    // =======================================================================
    // The following fields are to be set by the install tool
    // must be zero'd for binaries created by mkbin
    // =======================================================================

    uint32_t flags;             // flags bitmap
    uint32_t label;             // disk label type
    uint32_t partition;         // boot partition index

    // remainder of the block is zero

} __attribute__((packed));

typedef struct SplHeader_s SplHeader;

// ============================================================================
// SplHeader functions
// ============================================================================

//
// Verifiy that the header->headerCrc checksum is correct
//
bool spl_hdr_check(SplHeader *header);

//
// Verify that the header->loaderCrc checksum is correct for the given binary.
//
bool spl_hdr_checkBin(SplHeader *header, void *binary);

#ifndef SPLOADERK
//
// Calculates and updates the headerCrc for the given header.
//
void spl_hdr_setChecksum(SplHeader *header);
#endif

// ============================================================================
// CRC32 functions
// ============================================================================

//
// Calculate a CRC32 checksum for the given data buffer
//
uint32_t spl_crc32(void *data, size_t size);

//
// Accumulate a CRC32 checksum with the given data byte. This function is meant
// to be used to calculate a checksum for a buffer of unknown length. This
// function should be called for each byte of the data to checksum, with crc
// being the return value of the previous call (use SPL_CRC32_INIT for the
// first call). When finished processing the data, call spl_crc32_end to get
// the final checksum value.
//
uint32_t spl_crc32_acc(uint8_t data, uint32_t crc);

//
// Concludes a crc32 calculation, this function is to be called after
// successive spl_crc32_acc calls.
//
#define spl_crc32_end(crc) \
    spl_reverse32(~crc)

//
// Reverses the bits in a 32-bit integer. The reversed value of num is
// returned.
//
uint32_t spl_reverse32(uint32_t num);

// todo: remove guard once functions are implemented
#if 0

// ============================================================================
// Device functions
// ============================================================================

int spl_dev_close(SplDev *dev);

#ifndef SPLOADERK
int spl_dev_fromFile(SplDev *dev, const char *path);
#endif

int spl_dev_init(SplDev *dev, SplDevDriver *drv, void *param);

int spl_dev_info(SplDev *dev, SplDevInfo *info);

int spl_dev_read(SplDev *dev, void *buf, size_t bufsize, uint64_t lba, uint32_t bs, uint32_t blocks);

int spl_dev_pread(SplDev *dev, void *buf, size_t bufsize, uint64_t lba, uint32_t blocks);

int spl_dev_pwrite(SplDev *dev, void *buf, size_t bufsize, uint64_t lba, unit32_t blocks);

// ============================================================================
// Label functions
// ============================================================================

int spl_label_init(SplDev *dev, SplLabel *label, SplLabelKind kind)

int spl_label_getActive(SplLabel *label, SplPart *part);

int spl_label_getPart(SplLabel *label, uint32_t partnum, SplPart *part);

// libc implementations (for loader)

#ifdef SPLOADERK

#else

#define spl_malloc malloc
#define spl_memcpy memcpy
#define spl_strlen strlen
#define 

#endif


// dependencies
// these functions must be implemented in the loader

#ifdef SPLOADERK
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN int spl_dev_drv_read(void *aux, SplBuf *inBuf, uint64_t lba, uint32_t blocks);

EXTERN int spl_dev_drv_write(void *aux, SplBuf *outBuf, uint64_t lba, uint32_t blocks);

EXTERN int spl_dev_drv_open(void *aux);

EXTERN int spl_dev_drv_close(void *aux);

EXTERN int spl_dev_drv_init(void *aux);

#endif

#endif // __ASM__

#endif // _SPLOADER_H
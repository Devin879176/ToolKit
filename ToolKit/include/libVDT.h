#pragma once

#ifndef _VDT_API_H_
#define _VDT_API_H_

#ifdef OS_LINUX
#ifdef LIBVDT_LIBRARY
#define VDT_API extern "C" __attribute__((visibility("default")))
#else
#define VDT_API extern "C" __attribute__((visibility("hidden")))
#endif
#else
#ifdef   LIBVDT_LIBRARY
#define  VDT_API extern "C" __declspec(dllexport)
#else
#define  VDT_API extern "C" __declspec(dllimport)
#endif
#endif

#define VDT_MAX_BLOB_COUNT     255

#define VDT_BLOB_TYPE_ABANDON    1
#define VDT_BLOB_TYPE_REMOVED    2

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int type;  /*abandon or removed*/
} VDT_RECT;

typedef struct {
	unsigned int    blobCount;
	VDT_RECT        blobInfo[VDT_MAX_BLOB_COUNT];
} VDT_RESULT;

VDT_API long long vdtCreate();

VDT_API void  vdtInit(long long handle, int width, int height);

VDT_API void  vdtReset(long long handle);

VDT_API long  vdtDetect(long long handle, int width, int height, unsigned char* rgbBuffer, unsigned char* pFGMask, VDT_RESULT* pBlobInfo);

VDT_API void  vdtRelease(long long handle);

#endif

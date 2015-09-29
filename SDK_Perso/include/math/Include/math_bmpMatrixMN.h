#pragma once

#include <windows.h>
#include "misc/misc_error.h"
#include "misc/misc_mmf.h"
#include "math_matrixMN.h"
#include "misc/misc_mmf_wr.h"

namespace math
{
	template <class CHANNEL>
	struct t_rgb
	{
		CHANNEL r;
		CHANNEL g;
		CHANNEL b;
	};
	template <class CHANNEL>
	struct t_rgba
	{
		CHANNEL r;
		CHANNEL g;
		CHANNEL b;
		CHANNEL a;
	};
	struct PALETRE
	{
		RGBQUAD rgbq[256];
	};
	inline bool operator ==(const RGBQUAD& arg1, const RGBQUAD& arg2)
	{
		return 
			arg1.rgbBlue  == arg2.rgbBlue ||
			arg1.rgbGreen == arg2.rgbGreen ||
			arg1.rgbRed   == arg2.rgbRed;
	}
	typedef t_rgb<unsigned char>  rgb;
	typedef t_rgba<unsigned char> rgba;

	typedef t_rgb<float>  frgb;
	typedef t_rgba<float> frgba;

	inline void set_rgb( rgb& dst, unsigned int src)
	{
		dst.r = GetRValue(src);
		dst.g = GetGValue(src);
		dst.b = GetBValue(src);
	}

	// Простая матрица
	typedef matrixMN<rgb>  bmpMatrix;
	typedef matrixMN<rgba> tgaMatrix;
	typedef matrixMN<frgb>  fbmpMatrix;
	typedef matrixMN<frgba> ftgaMatrix;
	typedef matrixMN<unsigned char> palBmpMatrix;

	inline bool loadBmpPalette(const char* filename, palBmpMatrix& bmp, PALETTEENTRY* pal, int size);
	inline bool saveBmpPalette(const char* filename, palBmpMatrix& bmp, PALETTEENTRY* pal, int size);

	inline bool loadBmp(const char* filename, bmpMatrix& bmp, PALETTEENTRY* palette=NULL);
	inline bool saveBmp(const char* filename, bmpMatrix& bmp);

	inline bool loadTga(const char* filename, tgaMatrix& tga);
	inline bool saveTga(const char* filename, tgaMatrix& tga);
	inline bool loadTga(const char* filename, ftgaMatrix& tga);
	inline bool saveTga(const char* filename, ftgaMatrix& tga);

	inline bool loadPalette(const char* filename, PALETTEENTRY* pal, int size);
	inline bool savePalette(const char* filename, PALETTEENTRY* pal, int size);
}

namespace math
{
	inline bool loadBmpPalette(const char* filename, palBmpMatrix& bmp, PALETTEENTRY* pal, int size)
	{
		misc::mmf theMMF;
		if( !theMMF.open(filename)) return false;

		unsigned int offset = 0;
		BITMAPFILEHEADER* bHeader;
		BITMAPINFOHEADER* biHeader;

		theMMF.getup(offset, bHeader);
		if( bHeader->bfType != 19778) return false;
		theMMF.getup(offset, biHeader);

		if( biHeader->biBitCount!=8) return false;

		PALETRE* rgbq;
		theMMF.getup(offset, rgbq);
		char* data;
		theMMF.get(offset,   data);

		if( size>256) size = 256;
		for(int i=0; i<size; i++)
		{
			PALETTEENTRY& pe = pal[i];
			RGBQUAD& src = rgbq->rgbq[i];
			pe.peRed = src.rgbRed;
			pe.peGreen = src.rgbGreen;
			pe.peBlue = src.rgbBlue;
			pe.peFlags = 0;
		}

		int NumPointsX = biHeader->biWidth; 
		int NumPointsY = biHeader->biHeight;
		bmp.init(NumPointsX, NumPointsY);

		int NumPointsXAligned = NumPointsX;
		if (NumPointsX%4 != 0) NumPointsXAligned = (NumPointsX/4 + 1)*4;

		for (int i = 0; i < NumPointsY; i++)
		{
			for (int j = 0; j < NumPointsX; j++)
			{
				unsigned char index = (unsigned char)data[j];
				bmp[i][j] = index;
			}
			data+=NumPointsXAligned;
		}

		return true;
	}

	inline bool saveBmpPalette(const char* filename, palBmpMatrix& bmp, PALETTEENTRY* pal, int size)
	{
		FILE *bmpptr = 0;
		unsigned char *BitmapIndexes= 0;
		try
		{
			int NumPointsX = bmp.xSize(); 
			int NumPointsY = bmp.ySize();

			int NumPointsXAligned = NumPointsX;
			if (NumPointsX%4 != 0) NumPointsXAligned = (NumPointsX/4 + 1)*4;

			BITMAPFILEHEADER bHeader;
			bHeader.bfType      = 19778;
			bHeader.bfSize      = sizeof(BITMAPFILEHEADER) + 
								  sizeof(BITMAPINFOHEADER) +
								  sizeof(RGBQUAD)*256 +
								  NumPointsXAligned*NumPointsY;
			bHeader.bfReserved1 = 0;
			bHeader.bfReserved2 = 0;
			bHeader.bfOffBits   = 1078;
			BITMAPINFOHEADER biHeader;
			biHeader.biSize          = 40;
			biHeader.biWidth         = NumPointsX;
			biHeader.biHeight        = NumPointsY;
			biHeader.biPlanes        = 1;
			biHeader.biBitCount      = 8;
			biHeader.biCompression   = 0;
			biHeader.biSizeImage     = 0;
			biHeader.biXPelsPerMeter = 99962;
			biHeader.biYPelsPerMeter = 99962;
			biHeader.biClrUsed       = 0;
			biHeader.biClrImportant  = 0;
			RGBQUAD rgbq[256];
			int i, j;
			for (i = 0; i < 256; i++)
			{
				rgbq[i].rgbBlue     = pal[i].peBlue;
				rgbq[i].rgbGreen    = pal[i].peGreen;
				rgbq[i].rgbRed      = pal[i].peRed;
				rgbq[i].rgbReserved = 0;
			}
			bmpptr = fopen(filename, "wb");
			if (!bmpptr) throw misc::error("Can't open file %s", filename);

			fwrite(&bHeader, sizeof(BITMAPFILEHEADER), 1, bmpptr);
			fwrite(&biHeader, sizeof(BITMAPINFOHEADER), 1, bmpptr);
			fwrite(rgbq, sizeof(RGBQUAD), 256, bmpptr);

			BitmapIndexes = new unsigned char [NumPointsXAligned];
			for (i = 0; i < NumPointsY; i++)
			{
				for (j = 0; j < NumPointsX; j++)
				{
					BitmapIndexes[j] = bmp[i][j];
				}
				for (j = NumPointsX; j < NumPointsXAligned; j++)
				{
					BitmapIndexes[j] = 0;
				}
				fwrite(BitmapIndexes, sizeof(unsigned char), NumPointsXAligned, bmpptr);
			}

			if( BitmapIndexes) delete [] BitmapIndexes;
			if(bmpptr) fclose(bmpptr);
		}
		catch(const misc::error&)
		{
			if( BitmapIndexes) delete [] BitmapIndexes;
			if(bmpptr) fclose(bmpptr);
			throw;
		}
		return true;
	}



	inline bool loadBmp(const char* filename, bmpMatrix& bmp, PALETTEENTRY* palette)
	{
		misc::mmf theMMF;
		if( !theMMF.open(filename)) return false;

		unsigned int offset = 0;
		BITMAPFILEHEADER* bHeader;
		BITMAPINFOHEADER* biHeader;

		theMMF.getup(offset, bHeader);
		if( bHeader->bfType != 19778) return false;
		theMMF.getup(offset, biHeader);

		int NumPointsX = biHeader->biWidth; 
		int NumPointsY = biHeader->biHeight;
		bmp.init(NumPointsX, NumPointsY);

		if( biHeader->biBitCount==8)
		{
			PALETRE* rgbq;
			theMMF.getup(offset, rgbq);
			char* data;
			theMMF.get(offset,   data);

			int NumPointsXAligned = NumPointsX;
			if (NumPointsX%4 != 0) NumPointsXAligned = (NumPointsX/4 + 1)*4;

			for (int i = 0; i < NumPointsY; i++)
			{
				for (int j = 0; j < NumPointsX; j++)
				{
					unsigned char index = (unsigned char)data[j];
					rgb& dst = bmp[i][j];
					if(!palette)
					{
						RGBQUAD& src = rgbq->rgbq[index];
						dst.r = src.rgbRed;
						dst.g = src.rgbGreen;
						dst.b = src.rgbBlue;
					}
					else
					{
						PALETTEENTRY& src = palette[index];
						dst.r = src.peRed;
						dst.g = src.peGreen;
						dst.b = src.peBlue;
					}
				}
				data+=NumPointsXAligned;
			}
		}
		if( biHeader->biBitCount==24)
		{
			char* data;
			theMMF.get(offset, data);

			for (int y = 0; y < NumPointsY; y++)
			{
				for (int x = 0; x < NumPointsX; x++)
				{
					rgb& dst = bmp[y][x];
					dst.r = data[2];
					dst.g = data[1];
					dst.b = data[0];
					data+=3;
				}
			}
		}
		return true;
	}

	inline bool saveBmp(const char* filename, bmpMatrix& bmp)
	{
		int NumPointsX = bmp.rowsize; 
		int NumPointsY = bmp.size;

		misc::mmf_write theMMF;
		if( !theMMF.Create(filename, 3*NumPointsX*NumPointsY+10000)) 
			return false;

		BITMAPFILEHEADER* bHeader;
		BITMAPINFOHEADER* biHeader;
		char* data;
		theMMF.alloc(bHeader);
		theMMF.alloc(biHeader);
		theMMF.allocarray(data, NumPointsX*NumPointsY*3);

		bHeader->bfType      = 19778;
		bHeader->bfSize      = sizeof(BITMAPFILEHEADER) + 
							  sizeof(BITMAPINFOHEADER) +
							  NumPointsX*NumPointsY*3;
		bHeader->bfReserved1 = 0;
		bHeader->bfReserved2 = 0;
		bHeader->bfOffBits   = 54;

		biHeader->biSize          = 40;
		biHeader->biWidth         = NumPointsX;
		biHeader->biHeight        = NumPointsY;
		biHeader->biPlanes        = 1;
		biHeader->biBitCount      = 24;
		biHeader->biCompression   = 0;
		biHeader->biSizeImage     = 0;
		biHeader->biXPelsPerMeter = 99962;
		biHeader->biYPelsPerMeter = 99962;
		biHeader->biClrUsed       = 0;
		biHeader->biClrImportant  = 0;

		for (int i = 0; i < NumPointsY; i++)
		{
			for (int j = 0; j < NumPointsX; j++)
			{
				rgb& src = bmp[i][j];
				data[2] = src.r;
				data[1] = src.g;
				data[0] = src.b;
				data+=3;
			}
		}
		return true;
	}

	struct TARGAHEADER
	{
		BYTE	IDlength;
		BYTE	ColorMapType;
		BYTE	ImgType;
		BYTE	FirstColorMapEntry[2];
		BYTE	ColorMapLength[2];
		BYTE	ColorMapEntrySize;
		BYTE	Xorg[2];
		BYTE	Yorg[2];
		WORD	Width;
		WORD	Height;
		BYTE	Bpp;
		BYTE	Bits;
	};

	inline bool loadTga(const char* filename, tgaMatrix& tga)
	{
		misc::mmf theMMF;
		if( !theMMF.open(filename)) return false;

		unsigned int offset = 0;
		TARGAHEADER* bHeader;

		theMMF.getup(offset, bHeader);
		char* data;
		theMMF.get(offset, data);

		int NumPointsX = bHeader->Width;
		int NumPointsY = bHeader->Height;
		tga.init(NumPointsX, NumPointsY);
		for (int y = 0; y < NumPointsY; y++)
		{
			for (int x = 0; x < NumPointsX; x++)
			{
				rgba& dst = tga[y][x];
				dst.r = data[2];
				dst.g = data[1];
				dst.b = data[0];
				dst.a = data[3];
				data+=4;
			}
		}
		return true;
	}
	inline bool saveTga(const char* filename, tgaMatrix& tga)
	{
		int NumPointsX = tga.rowsize; 
		int NumPointsY = tga.size;

		misc::mmf_write theMMF;
		if( !theMMF.Create(filename, 4*NumPointsX*NumPointsY+10000)) 
			return false;

		TARGAHEADER* bHeader;
		char* data;
		theMMF.alloc(bHeader);
		theMMF.allocarray(data, NumPointsX*NumPointsY*4);

		bHeader->IDlength = 0;
		bHeader->ColorMapType = 0;
		bHeader->ImgType = 2;
		bHeader->FirstColorMapEntry[0] = bHeader->FirstColorMapEntry[1] = 0;
		bHeader->ColorMapLength[0] = bHeader->ColorMapLength[1] = 0;
		bHeader->ColorMapEntrySize = 0;
		bHeader->Xorg[0] = bHeader->Xorg[1] = 0;
		bHeader->Yorg[0] = bHeader->Yorg[1] = 0;
		bHeader->Width = NumPointsX;
		bHeader->Height= NumPointsY;
		bHeader->Bpp = 32;
		bHeader->Bits= 8;
		for (int y = 0; y < NumPointsY; y++)
		{
			for (int x = 0; x < NumPointsX; x++)
			{
				rgba& dst = tga[y][x];
				data[2] = dst.r;
				data[1] = dst.g;
				data[0] = dst.b;
				data[3] = dst.a;
				data+=4;
			}
		}
		return true;
	}

	inline bool loadTga(const char* filename, ftgaMatrix& tga)
	{
		tgaMatrix tgauc;
		if( !loadTga(filename, tgauc)) return false;
		tga.init(tgauc.xSize(), tgauc.ySize());
		for(int iy = 0; iy<tga.ySize(); iy++)
		{
			for(int ix = 0; ix<tga.xSize(); ix++)
			{
				math::rgba   cs = tgauc[iy][ix];
				math::frgba& cd = tga[iy][ix];
				cd.r = cs.r/255.f;
				cd.g = cs.g/255.f;
				cd.b = cs.b/255.f;
				cd.a = cs.a/255.f;
			}
		}
		return true;
	}
	inline bool saveTga(const char* filename, ftgaMatrix& tga)
	{
		tgaMatrix tgauc;
		tgauc.init(tga.xSize(), tga.ySize());

		for(int iy = 0; iy<tga.ySize(); iy++)
		{
			for(int ix = 0; ix<tga.xSize(); ix++)
			{
				math::frgba cs = tga[iy][ix];
				math::rgba& cd = tgauc[iy][ix];
				cd.r = __max( 0, __min( 1, (unsigned char)cs.r))*255;
				cd.g = __max( 0, __min( 1, (unsigned char)cs.g))*255;
				cd.b = __max( 0, __min( 1, (unsigned char)cs.b))*255;
				cd.a = __max( 0, __min( 1, (unsigned char)cs.a))*255;
			}
		}
		return saveTga(filename, tgauc);
	}

	inline bool loadPalette(const char* filename, PALETTEENTRY* pal, int size)
	{
		// Палитру в отдельный файл
		if( !filename || !filename[0])
			return false;
		FILE* palfile = fopen(filename, "rb");
		if( !palfile) 
			return false;
		for(int c=0; c<size; c++)
		{
			if( feof(palfile)) 
				return false;
			BYTE ch[3];
			fread(&ch, 1, 3, palfile);
			pal[c].peRed = ch[0];
			pal[c].peGreen = ch[1];
			pal[c].peBlue = ch[2];
		}
		fclose(palfile);
		return true;
	}
	inline bool savePalette(const char* filename, PALETTEENTRY* pal, int size)
	{
		// Палитру в отдельный файл
		FILE* palfile = fopen(filename, "wb");
		if( !palfile) 
			return false;
		for(int c=0; c<size; c++)
		{
			fwrite(&pal[c], 1, 3, palfile);
		}
		fclose(palfile);
		return true;
	}
}


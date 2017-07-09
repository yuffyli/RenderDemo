//
//  Texture.cpp
//  RenderDemo
//
//  Created by 李燕霏 on 2017/5/6.
//  Copyright © 2017年 yuffy. All rights reserved.
//

#include "Texture.hpp"
#include "MathUtil.hpp"
#include "png.h"
#include "pngconf.h"
#include <iosfwd>
#include <stdio.h>

using namespace std;

#define T3D_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
	(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))

static void PngErrorCallback(png_structp png_ptr, png_const_charp error)
{
	png_error(png_ptr, error);
}

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource *isource = (tImageSource *)png_get_io_ptr(png_ptr);

	if ((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data+isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}


//////////////////////////////////////////////////////////////////////////
void Texture::initByPng(char *fileName)
{
	setupTextureByPng(fileName);
}

void Texture::setupTexture()
{
	// 将纹理分成32x32的小块，并为每个小块填充同一种颜色
	uint32_t color[4] = {COLOR0, COLOR1, COLOR2, COLOR3};
	for (int32_t j = 0; j < nHeight; ++j)
	{
		for (int32_t i = 0; i < nWidth; ++i)
		{
			int32_t x = i/32;
			int32_t y = j/32;
			pTexture[j][i] = color[(x+y)%4];
		}
	}
}

void Texture::setupTextureByPng(char *fileName)
{
	FILE  *pFile = fopen (fileName, "rb" );  
	if (pFile!=NULL)  
	{  
		fseek (pFile , 0 , SEEK_END);  
		size_t rawSize = ftell (pFile);  
		rewind (pFile);  

		byte *rawData = new byte[rawSize];
		size_t result = fread (rawData,1,rawSize,pFile);  

		fclose (pFile);

		const size_t PNGSIGSIZE = 8;
		bool ret = false;

		png_byte        header[PNGSIGSIZE]   = {0}; 
		png_structp     png_ptr = 0;
		png_infop       info_ptr = 0;

		int32_t width = 0;
		int32_t height = 0;
		int32_t pitch = 0;
		int32_t bpp = 0;
		uint8_t *data = nullptr;
		size_t size = 0;
		bool hasAlpha = false;
		bool isPreMulti = false;
		//PixelFormat eFormat = E_PF_A8R8G8B8;

		do 
		{
			// png header len is 8 bytes
			if (rawSize < PNGSIGSIZE)
				break;

			// check the data is png or not
			memcpy(header, rawData, PNGSIGSIZE);
			if (png_sig_cmp(header, 0, PNGSIGSIZE))
				break;

			// init png_struct
			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, PngErrorCallback, 0);
			if (! png_ptr)
				break;

			// init png_info
			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr)
				break;

			// set the read call back function
			tImageSource imageSource;
			imageSource.data    = (uint8_t*)rawData;
			imageSource.size    = rawSize;
			imageSource.offset  = 0;
			png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

			// read png header info

			// read png file info
			png_read_info(png_ptr, info_ptr);

			width = png_get_image_width(png_ptr, info_ptr);
			height = png_get_image_height(png_ptr, info_ptr);

			pTexture = new uint32_t *[height];
			for (int32_t j = 0; j < height; ++j)
			{
				pTexture[j] = new uint32_t[width];
			}

			nWidth = width;
			nHeight = height;
			fMaxU = (float)(nWidth-1);
			fMaxV = (float)(nHeight-1);

			bpp = png_get_bit_depth(png_ptr, info_ptr);
			png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

			//CCLOG("color type %u", color_type);

			// force palette images to be expanded to 24-bit RGB
			// it may include alpha channel
			if (color_type == PNG_COLOR_TYPE_PALETTE)
			{
				png_set_palette_to_rgb(png_ptr);
			}
			// low-bit-depth grayscale images are to be expanded to 8 bits
			if (color_type == PNG_COLOR_TYPE_GRAY && bpp < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}

			// expand any tRNS chunk data into a full alpha channel
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			{
				png_set_tRNS_to_alpha(png_ptr);
			}  
			// reduce images with 16-bit samples to 8 bits
			if (bpp == 16)
			{
				png_set_strip_16(png_ptr);            
			} 
			// expand grayscale images to RGB
			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			{
				png_set_gray_to_rgb(png_ptr);
			}

			// read png data
			// m_nBitsPerComponent will always be 8
			//             bpp = 8;
			png_uint_32 rowbytes;
			png_bytep* row_pointers = new png_bytep[height];

			png_read_update_info(png_ptr, info_ptr);

			png_set_bgr(png_ptr);

			rowbytes = png_get_rowbytes(png_ptr, info_ptr);

			pitch = rowbytes;
			size = rowbytes * height;
			data = new uint8_t[size];
			if (!data)
				break;

			for (uint16_t i = 0; i < height; ++i)
			{
				row_pointers[i] = data + i*rowbytes;
			}

			png_read_image(png_ptr, row_pointers);

			png_read_end(png_ptr, NULL);

			png_uint_32 channel = rowbytes / width;
			if (channel == 4)
			{
				hasAlpha = true;
				uint32_t *tmp = (uint32_t *)data;
				for (uint16_t i = 0; i < height; i++)
				{
					for(uint32_t j = 0; j < rowbytes; j += 4)
					{
						pTexture[i][j/4]=T3D_RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1], 
							row_pointers[i][j + 2], row_pointers[i][j + 3] );
						//*tmp++= T3D_RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1], 
						//	row_pointers[i][j + 2], row_pointers[i][j + 3] );
					}
				}

				isPreMulti = true;
				//eFormat = E_PF_A8R8G8B8;
				bpp = 4;
			}
			else
			{
				//eFormat = E_PF_R8G8B8;
				bpp = 3;
			}

			//T3D_SAFE_DELETE_ARRAY(row_pointers);

			ret = true;
		} while (0);
		
		if (png_ptr)
		{
			png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
		}	

		delete []rawData;
	}  

	

	
//	delete []buffer;  
//	buffer = NULL;
}

void Texture::destroy()
{
	for (int32_t j = 0; j < nHeight; ++j)
	{
		delete []pTexture[j];
	}

	delete []pTexture;
	pTexture = NULL;
}

uint32_t Texture::getColorByUV(float u, float v)
{
	int32_t x = middle(0, nWidth-1, rounding(u*fMaxU));
	int32_t y = middle(0, nHeight-1, rounding(v*fMaxV));
	return pTexture[nHeight-1-y][x];
}

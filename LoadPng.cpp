// Cargame -- PNG file IO
// Copyright 2010 (C) Ralph Thomas

#include "LoadPng.h"
#include <png.h>

PngFile::~PngFile() {
	if (m_data) delete[] m_data;
	m_data = 0;
}
bool PngFile::read(const char* name) {
	// XXX: leaky
	png_byte header[8];
	FILE* f = fopen(name, "rb");
	if (!f) return false;
	int len = fread(header, 1, 8, f);
	if (len < 8) return false;
	if (png_sig_cmp(header, 0, 8)) { fclose(f); return false; }

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_ptr) { fclose(f); return false; }
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) { fclose(f); return false; }
	if (setjmp(png_jmpbuf(png_ptr))) { fclose(f); return false; }

	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	m_width = info_ptr->width;
	m_height = info_ptr->height;
	m_rowBytes = info_ptr->rowbytes;
	m_data = new unsigned char[m_rowBytes * m_height];
/*
	int color_type = 0;
	png_get_IHDR(png_ptr, info_ptr, 0, 0, 0, &color_type, 0, 0, 0);
	png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);

	if (color_type & PNG_COLOR_MASK_ALPHA) {
		unsigned char* dest = m_data;
		for (int i = m_height - 1; i >= 0; i--, dest += m_rowBytes)
			memcpy(dest, row_pointers[i], m_rowBytes);
	} else {
		unsigned char* dest = m_data;
		for (int i = m_height - 1; i >= 0; i--) {
			png_byte* src = row_pointers[i];
			for (int j = 0; j < width; j++) {

			}
		}
	}
*/
	png_bytep* row_ptrs = new png_bytep[m_height];
	for (int i = 0; i < m_height; i++)
		row_ptrs[i] = m_data + i * m_rowBytes;
	png_read_image(png_ptr, row_ptrs);
	delete[] row_ptrs;
	// delete png_ptr and info_ptr
	return true;
}

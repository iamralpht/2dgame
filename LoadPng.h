// Cargame -- PNG file IO
// Copyright 2010 (C) Ralph Thomas

#ifndef LOAD_PNG_H
#define LOAD_PNG_H

struct PngFile {
	PngFile() : m_width(0), m_height(0), m_rowBytes(0), m_data(0) {}
	virtual ~PngFile();
	
	bool read(const char* name);
	unsigned int width() const { return m_width; }
	unsigned int height() const { return m_height; }
	unsigned int rowBytes() const { return m_rowBytes; }
	unsigned char* data() const { return m_data; }
  private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_rowBytes;
	unsigned char* m_data;
};

#endif


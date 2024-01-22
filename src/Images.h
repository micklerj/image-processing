#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


struct Pixel {
	unsigned char blue = 0;
	unsigned char green = 0;
	unsigned char red = 0;

	void setb(unsigned char i) {
		blue = i;
	}
	void setg(unsigned char i) {
		green = i;
	}
	void setr(unsigned char i) {
		red = i;
	}

	//copy assignment
	Pixel& operator=(const Pixel& other) {
		blue = other.blue;
		green = other.green;
		red = other.red;
		return *this;
	}
};

struct Image {
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width = 0;
	short height = 0;
	char bitsPerPixel;
	char imageDescriptor;
	//int length = int(width) * int(height);
	vector <Pixel> pixels;

	vector <Pixel> get_pixels() {   //getter
		return pixels;
	}
	void add_pixel(Pixel p) {       //adder
		pixels.push_back(p);
	}

	void set_pixel(int index, unsigned char b, unsigned char g, unsigned char r) {
		pixels[index].blue = b;
		pixels[index].green = g;
		pixels[index].red = r;

	}
	void set_blue(int index, unsigned char b) {
		pixels[index].blue = b;
	}
	void set_green(int index, unsigned char g) {
		pixels[index].green = g;
	}
	void set_red(int index, unsigned char r) {
		pixels[index].red = r;
	}
	
	//copy assignment operator 
	Image& operator=(const Image& other) {
		idLength = other.idLength;
		colorMapType = other.colorMapType;
		dataTypeCode = other.dataTypeCode;
		colorMapOrigin = other.colorMapOrigin;
		colorMapLength = other.colorMapLength;
		colorMapDepth = other.colorMapDepth;
		xOrigin = other.xOrigin;
		yOrigin = other.yOrigin;
		width = other.width;
		height = other. height;
		bitsPerPixel = other.bitsPerPixel;
		imageDescriptor = other.imageDescriptor;
		for (int i = 0; i < int(other.width) * int(other.height); ++i) {
			Pixel pix;
			pixels.push_back(pix);
			pixels[i] = (other.pixels)[i];
		}
		return *this;
	}
	
};

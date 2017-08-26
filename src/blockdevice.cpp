#include "filsys.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

BlockDevice::BlockDevice() : p(0), data(nullptr) {
}

BlockDevice::BlockDevice(const std::string path) : BlockDevice() {
	struct stat st;
	stat((this->path = path).c_str(), &st );
	std::ifstream fin;
	delete[] data;
	data = new uint8_t [st.st_size ] ;
	fin.open( path.c_str() , std::ios_base::in | std::ios_base::binary );
	fin.read ( (char*)data, st.st_size );
	fin.read ( (char*)data, st.st_size );
	fin.close ();
}

BlockDevice::~BlockDevice() {
}

void BlockDevice::setIndex( uint32_t p) {
	this->p = p;
}

uint16_t BlockDevice::readShort() {
	uint8_t tmp = data [p++];
	return (uint16_t)(tmp | data [p++] << 8);
}

uint8_t BlockDevice::readChar() {
	return data [p++];
}

std::string BlockDevice::readString() {
	std::string s = (char*)&data [p];
	p += (s.length() + 1);
	return s;
}

void BlockDevice::show() {
	for (int i = 0; i < 32; ++i) {
		if (i % 16 == 0) printf("\n");
		printf("%02x ", data [i]);
	}
	printf("\n");
}

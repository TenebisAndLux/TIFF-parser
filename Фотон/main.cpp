#include <iostream>
#include <fstream>
#include <vector>
#include <set>

struct TIFFTag {
	uint16_t tag;
	uint16_t type;
	uint32_t count;
	uint32_t valueOffset;
};

enum COMPRESSION{
COMPRESSION_NONE = 1,
COMPRESSION_CCITTRLE = 2,
COMPRESSION_CCITTFAX3 = 3,
COMPRESSION_CCITTFAX4 = 4,
COMPRESSION_LZW = 5,
COMPRESSION_OJPEG = 6,
COMPRESSION_JPEG = 7,
COMPRESSION_NEXT = 32766,
COMPRESSION_CCITTRLEW = 32771,
COMPRESSION_PACKBITS = 32773,
COMPRESSION_THUNDERSCAN = 32809,
COMPRESSION_IT8CTPAD = 32895,
COMPRESSION_IT8LW = 32896,
COMPRESSION_IT8MP = 32897,
COMPRESSION_IT8BL = 32898,
COMPRESSION_PIXARFILM = 32908,
COMPRESSION_PIXARLOG = 32909,
COMPRESSION_DEFLATE = 32946,
COMPRESSION_ADOBE_DEFLATE = 8,
COMPRESSION_DCS = 32947,
COMPRESSION_JBIG = 34661,
COMPRESSION_SGILOG = 34676,
COMPRESSION_SGILOG24 = 34677,
COMPRESSION_JP2000 = 34712,
};

enum ORIENTATION {
ORIENTATION_TOPLEFT = 1,
ORIENTATION_TOPRIGHT = 2,
ORIENTATION_BOTRIGHT = 3,
ORIENTATION_BOTLEFT = 4,
ORIENTATION_LEFTTOP = 5,
ORIENTATION_RIGHTTOP = 6,
ORIENTATION_RIGHTBOT = 7,
ORIENTATION_LEFTBOT = 8,
};

enum PLANARCONFIG {
PLANARCONFIG_CONTIG = 1,
PLANARCONFIG_SEPARATE = 2,
};

enum RESUNIT {
RESUNIT_NONE = 1,
RESUNIT_INCH = 2,
RESUNIT_CENTIMETER = 3,
};

std::ostream& operator << (std::ostream& out, const COMPRESSION& t){
	switch (t) {
	case COMPRESSION_NONE: return (out << "COMPRESSION_NONE");
	case COMPRESSION_CCITTRLE: return (out << "COMPRESSION_CCITTRLE");
	case COMPRESSION_CCITTFAX3: return (out << "COMPRESSION_CCITTFAX3");
	case COMPRESSION_CCITTFAX4: return (out << "COMPRESSION_CCITTFAX4");
	case COMPRESSION_LZW: return (out << "COMPRESSION_LZW");
	case COMPRESSION_OJPEG: return (out << "COMPRESSION_OJPEG");
	case COMPRESSION_JPEG: return (out << "COMPRESSION_JPEG");
	case COMPRESSION_NEXT: return (out << "COMPRESSION_NEXT");
	case COMPRESSION_CCITTRLEW: return (out << "COMPRESSION_CCITTRLEW");
	case COMPRESSION_PACKBITS: return (out << "COMPRESSION_PACKBITS");
	case COMPRESSION_THUNDERSCAN: return (out << "COMPRESSION_THUNDERSCAN");
	case COMPRESSION_IT8CTPAD: return (out << "COMPRESSION_IT8CTPAD");
	case COMPRESSION_IT8LW: return (out << "COMPRESSION_IT8LW");
	case COMPRESSION_IT8MP: return (out << "COMPRESSION_IT8MP");
	case COMPRESSION_IT8BL: return (out << "COMPRESSION_IT8BL");
	case COMPRESSION_PIXARFILM: return (out << "COMPRESSION_PIXARFILM");
	case COMPRESSION_PIXARLOG: return (out << "COMPRESSION_PIXARLOG");
	case COMPRESSION_DEFLATE: return (out << "COMPRESSION_DEFLATE");
	case COMPRESSION_ADOBE_DEFLATE: return (out << "COMPRESSION_ADOBE_DEFLATE");
	case COMPRESSION_DCS: return (out << "COMPRESSION_DCS");
	case COMPRESSION_JBIG: return (out << "COMPRESSION_JBIG");
	case COMPRESSION_SGILOG: return (out << "COMPRESSION_SGILOG");
	case COMPRESSION_SGILOG24: return (out << "COMPRESSION_SGILOG24");
	case COMPRESSION_JP2000: return (out << "COMPRESSION_JP2000");
	}
	return (out);
}

std::ostream& operator << (std::ostream& out, const ORIENTATION& t) {
	switch (t) {
	case ORIENTATION_TOPLEFT: return (out << "ORIENTATION_TOPLEFT");
	case ORIENTATION_TOPRIGHT: return (out << "ORIENTATION_TOPRIGHT");
	case ORIENTATION_BOTRIGHT: return (out << "ORIENTATION_BOTRIGHT");
	case ORIENTATION_BOTLEFT: return (out << "ORIENTATION_BOTLEFT");
	case ORIENTATION_LEFTTOP: return (out << "ORIENTATION_LEFTTOP");
	case ORIENTATION_RIGHTTOP: return (out << "ORIENTATION_RIGHTTOP");
	case ORIENTATION_RIGHTBOT: return (out << "ORIENTATION_RIGHTBOT");
	case ORIENTATION_LEFTBOT: return (out << "ORIENTATION_LEFTBOT");
	}
	return (out);
}

std::ostream& operator << (std::ostream& out, const PLANARCONFIG& t) {
	switch (t) {
	case PLANARCONFIG_CONTIG: return (out << "PLANARCONFIG_CONTIG");
	case PLANARCONFIG_SEPARATE: return (out << "PLANARCONFIG_SEPARATE");
	}
	return (out);
}

std::ostream& operator << (std::ostream& out, const RESUNIT& t) {
	switch (t) {
	case RESUNIT_NONE: return (out << "RESUNIT_NONE");
	case RESUNIT_INCH: return (out << "RESUNIT_INCH");
	case RESUNIT_CENTIMETER: return (out << "RESUNIT_CENTIMETER");
	}
	return (out);
}

template <typename T>
T reedTagValue(TIFFTag& tag, FILE* tif) {
	T tagValue;
	if (tag.count > 1) {
		int32_t currentOffset = ftell(tif);
		fseek(tif, tag.valueOffset, 0);
		fread_s(&tagValue, sizeof(T), sizeof(T), 1, tif);
		fseek(tif, currentOffset, 0);
	}
	else
		tagValue = tag.valueOffset;
	return tagValue;
}

template <typename T>
T* reedTagValueArr(TIFFTag& tag, FILE* tif) {
	T* tagValue = new T[tag.count];
	if (tag.count > 1) {
		int32_t currentOffset = ftell(tif);
		fseek(tif, tag.valueOffset, 0);
		fread_s(tagValue, sizeof(T) * tag.count, sizeof(T), tag.count, tif);
		fseek(tif, currentOffset, 0);
	}
	return tagValue;
}

template <typename T>
T* reedTagOffsetArr(TIFFTag& tag, FILE* tif) {
	T* tagValue = new T[tag.count];
	T* tagOffsetValue = new T[tag.count];
	int32_t currentOffset = ftell(tif);
	fseek(tif, tag.valueOffset, 0);
	fread_s(tagValue, sizeof(T) * tag.count, sizeof(T), tag.count, tif);
	for (int i = 0; i < tag.count; i++) {
		fseek(tif, tagValue[i], 0);
		fread_s(tagOffsetValue, sizeof(T) * tag.count, sizeof(T), tag.count, tif);
	}
	fseek(tif, currentOffset, 0);
	return tagOffsetValue;
}

int main() {
	FILE* tif;
	fopen_s(&tif, R"(C:\Users\79109\Desktop\Фотон\TestImage.tif)", "rb");
	
	//Header
	uint16_t byteOrder;
	uint16_t tifVer;
	uint32_t ifdOffset;
	//ReadHeader
	fread_s(&byteOrder, 2, 2, 1, tif);
	fread_s(&tifVer, 2, 2, 1, tif);
	fread_s(&ifdOffset, 4, 4, 1, tif);
	//Offset
	fseek(tif, ifdOffset, 0);
	//TagCount
	uint16_t tagCount;
	fread_s(&tagCount, 2, 2, 1, tif);
	//TagRead
	TIFFTag* Tags = new TIFFTag[tagCount];
	for (int tagNumber = 0; tagNumber < tagCount; tagNumber++)
		fread_s(&Tags[tagNumber], sizeof(Tags[tagNumber]), sizeof(Tags[tagNumber]), 1, tif);
	//write header
	std::cout << "Version: " << byteOrder << std::endl;
	std::cout << "TifVer: " << tifVer << std::endl;
	std::cout << "IfdOffset: " << ifdOffset << std::endl;
	std::cout << std::endl;
	std::cout << "TagCount: " << tagCount << std::endl;
	std::cout << std::endl;
	//write TIFF tags
	for (int tagNumber = 0; tagNumber < tagCount; tagNumber++) {
		std::cout << "Value count: " << Tags[tagNumber].count << std::endl;
		std::cout << "Tag DEC: " << Tags[tagNumber].tag << std::endl;
		std::cout << "Type of tag: " << Tags[tagNumber].type << std::endl;
		std::cout << "Tag value: " << Tags[tagNumber].valueOffset << std::endl;
		std::cout << std::endl;
	}
	for (int tagNumber = 0; tagNumber < tagCount; tagNumber++) {
		switch (Tags[tagNumber].tag) {
		case 256: {
			uint16_t height = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Height : " << height << std::endl;
		}
				break;
		case 257: {
			uint16_t length = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Length : " << length << std::endl;
			std::cout << std::endl;
		}
				break;
		case 258: {
			uint16_t* bits = reedTagValueArr<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Number of bits per component: ";
			for (int i = 0; i < Tags[tagNumber].count; i++) {
				switch (i) {
				case 0:
					std::cout << "\nred: ";
					break;
				case 1:
					std::cout << "\ngreen: ";
					break;
				case 2:
					std::cout << "\nblue: ";
					break;
				case 3:
					std::cout << "\nalpha: ";
					break;
				}
				std::cout << bits[i];
			}
			std::cout << '\n' << std::endl;
			delete[] bits;
		}
				break;
		case 259: {
				uint16_t compression = reedTagValue<uint16_t>(Tags[tagNumber], tif);
				std::cout << "Compression: ";
				std::cout << COMPRESSION(compression) << std::endl;
				std::cout << std::endl;
		}
			break;
		case 262: {
			uint16_t CSofID = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "The color space of the image data: ";
			switch (CSofID) {
			case 1:
				std::cout << "Black is zero";
				break;
			case 2:
				std::cout << "RGB/sRGB";
				break;
			}
			std::cout << std::endl;
		}
			break;
		case 273: {
			uint16_t* bandOffsets = reedTagValueArr<uint16_t>(Tags[tagNumber], tif);
			std::cout << "\nFor each strip, the byte offset of that strip: ";
			for (int i = 0; i < Tags[tagNumber].count; i++)
				std::cout << bandOffsets[i] << " ";
			std::cout << '\n' << std::endl;
			delete[] bandOffsets;
		}
			break;
		case 274: {
			uint16_t orientation = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Orientation: ";
			std::cout << ORIENTATION(orientation) << std::endl;
			std::cout << std::endl;
		}
			break;
		case 277: {
			uint16_t SPP = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "The number of components per pixel: ";
			switch (SPP) {
			case 1:
				std::cout << "Binary images, Grayscale images, and Paletted colors";
				break;
			case 3:
				std::cout << "RGB/sRGB";
				break;
			}
			std::cout << std::endl;
		}
			break;
		case 278: {
			uint16_t RPS = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "\nThe number of rows per image: " << floor((reedTagValue<uint16_t>(Tags[1], tif) + RPS - 1) / RPS);
			std::cout << std::endl;
		}
			break;
		case 279: {
			uint16_t* BitSAC = reedTagValueArr<uint16_t>(Tags[tagNumber], tif);
			std::cout << "\nFor each strip, the number of bytes in the strip after compression: ";
			for (int i = 0; i < Tags[tagNumber].count; i++)
				std::cout << BitSAC[i] << " ";
			std::cout << '\n' << std::endl;
			delete[] BitSAC;
		}
			break;
		case 282: {
			uint16_t XResolution = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "The number of pixels per ResolutionUnit in the ImageWidth direction: " << XResolution << std::endl;
			std::cout << std::endl;
		}
			break;
		case 283: {
			uint16_t YResolution = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "The number of pixels per ResolutionUnit in the ImageLength direction: " << YResolution << std::endl;
			std::cout << std::endl;
		}
			break;
		case 284: {
			uint16_t PlanarConfiguration = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Planar Configuration: ";
			std::cout << PLANARCONFIG(PlanarConfiguration) << std::endl;
			std::cout << std::endl;
		}
			break;
		case 296: {
			uint16_t ResolutionUnit = reedTagValue<uint16_t>(Tags[tagNumber], tif);
			std::cout << "The unit of measurement for XResolution and YResolution: ";
			std::cout << RESUNIT(ResolutionUnit) << std::endl;
			std::cout << std::endl;
		}
			break;
		case 305: {
			uint16_t* bits = reedTagOffsetArr<uint16_t>(Tags[tagNumber], tif);
			std::cout << "Name and version number of the software package(s) used to create the image: ";
			for (int i = 0; i < Tags[tagNumber].count; i++) {
				std::cout << bits[i] << " ";
			}
			std::cout << '\n' << std::endl;
			delete[] bits;
		}
			break;
		};
	};
	return 0;
}
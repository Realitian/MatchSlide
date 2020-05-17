/**
 * User: Bruce Washington
 * Date: 23/3/20.
 *
 */


#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <png.h>

#include "basicimage.h"

namespace Helper {

void BasicImage::ReadFromStream(png_structp PngPointer, std::uint8_t* Data, std::size_t Length)
{
	std::ifstream* Stream = reinterpret_cast<std::ifstream*>(png_get_io_ptr(PngPointer));
	Stream->read(reinterpret_cast<char*>(Data), Length);
}

void BasicImage::loadimage(const char* FilePath, std::vector<RGBA> &Pixels, std::uint32_t &width, std::uint32_t &height, std::uint16_t &BitsPerPixel)
{
	Pixels.clear();
	std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
	//std::vector<RGBA> pix_src;

	if (!hFile.is_open()) {
		throw std::invalid_argument("File Not Found.");
	}

	std::uint8_t Header[18] = {0};
	hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));
	hFile.seekg(8, std::ios::beg);
	
	if (!png_sig_cmp(Header, 0, 8)) {
		loadpng(hFile, Pixels/*pix_src*/, width, height, BitsPerPixel);
	} else {
		hFile.close();
		throw std::invalid_argument("Error: Invalid File Format. Required: Png.");
	}

#if 0
	// GL has a funny coordinate system so we need to reflect and rotate.
//	std::reverse(pix.begin(), pix.end());
	//unsigned count=0, i = 0;
	//for (std::vector<RGBA>::iterator iter; iter != pix_src.end(); iter++, i++) {
	unsigned count = 0;
	for (unsigned i = 0; i < height; i++) {
		for (unsigned j = 0; j < width; j++) {
			Pixels.insert(Pixels.begin() + count, pix_src[(height-1-i)*width + j]);
			count++;
		}
	}
#endif
}

void BasicImage::loadpng(std::fstream &hFile, std::vector<RGBA> &Pixels, std::uint32_t &width, std::uint32_t &height, std::uint16_t &BitsPerPixel)
{
	png_structp PngPointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!PngPointer) {
		hFile.close();
		throw std::runtime_error("Error: Cannot Create Read Structure.");
	}

	png_infop InfoPointer = png_create_info_struct(PngPointer);
	if (!InfoPointer) {
		hFile.close();
		png_destroy_read_struct(&PngPointer, nullptr, nullptr);
		throw std::runtime_error("Error: Cannot Create InfoPointer Structure.");
	}

	png_infop EndInfo = png_create_info_struct(PngPointer);
	if (!EndInfo) {
		hFile.close();
		png_destroy_read_struct(&PngPointer, &InfoPointer, nullptr);
		throw std::runtime_error("Error: Cannot Create EndInfo Structure.");
	}

	if (setjmp(png_jmpbuf(PngPointer)))
	{
		hFile.close();
		png_destroy_read_struct(&PngPointer, &InfoPointer, nullptr);
		throw std::runtime_error("Error: Cannot Set Jump Pointer.");
	}

	png_set_sig_bytes(PngPointer, 8);
	png_set_read_fn(PngPointer, &hFile, BasicImage::ReadFromStream);
	png_read_info(PngPointer, InfoPointer);

//	png_byte channels = png_get_channels(PngPointer, InfoPointer);
	int bitdepth, colortype, interlacetype;
	png_get_IHDR(PngPointer, InfoPointer, &width, &height, &bitdepth, &colortype, &interlacetype, nullptr, nullptr);
	png_set_interlace_handling(PngPointer);
	png_set_strip_16(PngPointer);
	png_set_packing(PngPointer);

	switch (colortype) {
	case PNG_COLOR_TYPE_GRAY:
		png_set_expand_gray_1_2_4_to_8(PngPointer);
		png_set_gray_to_rgb(PngPointer);
		png_set_bgr(PngPointer);
		break;
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(PngPointer);
		if (png_get_valid(PngPointer, InfoPointer, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(PngPointer);
		else
			png_set_filler(PngPointer, 0xFF, PNG_FILLER_AFTER);
		png_set_bgr(PngPointer);
		BitsPerPixel = 24;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		png_set_gray_to_rgb(PngPointer);
		break;
	case PNG_COLOR_TYPE_RGB:
		png_set_bgr(PngPointer);
		png_set_filler(PngPointer, 0xFF, PNG_FILLER_AFTER);
		BitsPerPixel = 24;
		break;
	case PNG_COLOR_TYPE_RGBA:
		png_set_bgr(PngPointer);
		BitsPerPixel = 32;
		break;
	default:
		png_destroy_read_struct(&PngPointer, &InfoPointer, nullptr);
		throw std::runtime_error("Error: Png Type not supported.");
		break;
	}

	png_read_update_info(PngPointer, InfoPointer);
	//channels = png_get_channels(PngPointer, InfoPointer);
	png_get_IHDR(PngPointer, InfoPointer, &width, &height, &bitdepth, &colortype, &interlacetype, nullptr, nullptr);

	Pixels.resize(width * height);
	std::vector<std::uint8_t*> RowPointers(height);
	std::uint8_t* BuffPos = reinterpret_cast<std::uint8_t*>(Pixels.data());

	for (size_t I = 0; I < height; ++I) {
		RowPointers[I] = BuffPos + (I * width * sizeof(RGBA));
	}

	png_read_image(PngPointer, RowPointers.data());
	png_read_end(PngPointer, InfoPointer);
	png_destroy_read_struct(&PngPointer, &InfoPointer, nullptr);
	hFile.close();
}

} /* namespace */

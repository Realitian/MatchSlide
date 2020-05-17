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

typedef union RGBA {
	std::uint32_t Colour;
	struct {
		std::uint8_t R, G, B, A;
	};
} *PRGB;

namespace Helper {

class BasicImage {
public:
	BasicImage() {};

	void loadimage(const char* FilePath, std::vector<RGBA> &Pixels, std::uint32_t &width, std::uint32_t &height, std::uint16_t &BitsPerPixel);

private:
	void loadpng(std::fstream &hFile, std::vector<RGBA> &Pixels, std::uint32_t &width, std::uint32_t &height, std::uint16_t &BitsPerPixel);
	static void ReadFromStream(png_structp PngPointer, std::uint8_t* Data, std::size_t Length);
};

} /* namespace */

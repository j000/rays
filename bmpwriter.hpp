#ifndef BMPWRITER_HPP
#define BMPWRITER_HPP

#include <cstdint>
#include <vector>

static std::vector<uint8_t> create_bitmap(const uint8_t* rgb, const size_t& width, const size_t& height)
{
	static_assert(sizeof(unsigned char) == 1, "unsigned char should be 8-bit");
	static_assert(std::is_same<std::uint8_t,unsigned char>::value, "unsigned char is not uint8_t");
	((void)rgb);

	uint8_t pad = (4 - (width & 3)) & 3; // padding to 4 bytes
	size_t size = 54 + height * (3 * width + pad);
	std::vector<uint8_t> out(size);
	out[0] = 'B';
	out[1] = 'M';
	// 2 - 5: file size in bytes
	for (auto i = 0u; i < 4; ++i) {
		out[2 + i] = (size >> (8 * i)) % 256;
	}
	// 6 - 9: reserved, 0
	// 10 - 13: offset, our header size
	out[10] = 54;
	// 14 - 17: size of part of header
	out[14] = 40;
	// 18 - 21: width of image
	for (auto i = 0u; i < 4; ++i) {
		out[18 + i] = (width >> (8 * i)) % 256;
	}
	// 22 - 25: height of image
	for (auto i = 0u; i < 4; ++i) {
		out[22 + i] = (height >> (8 * i)) % 256;
	}
	out[26] = 1; // color planes
	out[28] = 24; // bites per pixel
	// 30 - 33: disable compression: 0
	// 34 - 37: size of raw pixel data: can be 0 if no compression
	// 38 - 41: vertical resolution: 0
	// 42 - 45: horizontal resolution: 0
	// 46 - 49: number of colors: 0 = all
	// 50 - 53: number of important colors: 0 = all are
	////////////////////

	return out;
}

#endif /* BMPWRITER_HPP */

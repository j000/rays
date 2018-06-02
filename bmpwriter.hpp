#ifndef BMPWRITER_HPP
#define BMPWRITER_HPP

#ifndef PRECOMPILED
#include <cstdint>
#include <vector>
#include <endian.h>
#endif

static std::vector<uint8_t> create_bitmap(const uint8_t* rgb, const unsigned& width, const unsigned& height) {
	static_assert(sizeof(uint8_t) == 1, "uint8_t should be 8-bit");
	((void)rgb); // unused for now

	uint8_t pad = (4 - (width & 3)) & 3; // padding to 4 bytes
	unsigned size = 54 + height * (3 * width + pad);
	std::vector<uint8_t> out(size);
	out[0] = 'B';
	out[1] = 'M';
	// 2 - 5: file size in bytes
	// treat &out[2] as a pointer to uint32_t, and write there
	*reinterpret_cast<uint32_t*>(&(out[2])) = htole32(static_cast<uint32_t>(size));
	// 6 - 9: reserved, 0
	// 10 - 13: offset, our header size
	out[10] = 54;
	// 14 - 17: size of next(?) part of header
	out[14] = 40;
	// 18 - 21: width of image
	*reinterpret_cast<uint32_t*>(&(out[18])) = htole32(static_cast<uint32_t>(width));
	// 22 - 25: height of image
	*reinterpret_cast<uint32_t*>(&(out[22])) = htole32(static_cast<uint32_t>(height));
	out[26] = 1; // color planes
	out[28] = 3 * 8; // bites per pixel
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

#include <cassert>
#include <vector>

#include <bc7enc16.h>

#include <algorithm>

#ifndef _WIN32
#include <cstring>
#endif

struct color_quad_u8
{
	uint8_t m_c[4];

	inline color_quad_u8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		set(r, g, b, a);
	}

	inline color_quad_u8(uint8_t y = 0, uint8_t a = 255)
	{
		set(y, a);
	}

	inline color_quad_u8& set(uint8_t y, uint8_t a = 255)
	{
		m_c[0] = y;
		m_c[1] = y;
		m_c[2] = y;
		m_c[3] = a;
		return *this;
	}

	inline color_quad_u8& set(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		m_c[0] = r;
		m_c[1] = g;
		m_c[2] = b;
		m_c[3] = a;
		return *this;
	}

	inline uint8_t& operator[] (uint32_t i) { assert(i < 4);  return m_c[i]; }
	inline uint8_t operator[] (uint32_t i) const { assert(i < 4); return m_c[i]; }

	inline int get_luma() const { return (13938U * m_c[0] + 46869U * m_c[1] + 4729U * m_c[2] + 32768U) >> 16U; } // REC709 weightings
};
typedef std::vector<color_quad_u8> color_quad_u8_vec;

class image_u8
{
public:
	image_u8() :
		m_width(0), m_height(0)
	{
	}

	image_u8(uint32_t width, uint32_t height) :
		m_width(width), m_height(height)
	{
		m_pixels.resize(width * height);
	}

	inline const color_quad_u8_vec& get_pixels() const { return m_pixels; }
	inline color_quad_u8_vec& get_pixels() { return m_pixels; }

	inline uint32_t width() const { return m_width; }
	inline uint32_t height() const { return m_height; }
	inline uint32_t total_pixels() const { return m_width * m_height; }

	inline color_quad_u8& operator()(uint32_t x, uint32_t y) { assert(x < m_width&& y < m_height);  return m_pixels[x + m_width * y]; }
	inline const color_quad_u8& operator()(uint32_t x, uint32_t y) const { assert(x < m_width&& y < m_height);  return m_pixels[x + m_width * y]; }

	image_u8& clear()
	{
		m_width = m_height = 0;
		m_pixels.clear();
		return *this;
	}

	image_u8& init(uint32_t width, uint32_t height)
	{
		clear();

		m_width = width;
		m_height = height;
		m_pixels.resize(width * height);
		return *this;
	}

	image_u8& set_all(const color_quad_u8& p)
	{
		for (uint32_t i = 0; i < m_pixels.size(); i++)
			m_pixels[i] = p;
		return *this;
	}

	image_u8& crop(uint32_t new_width, uint32_t new_height)
	{
		if ((m_width == new_width) && (m_height == new_height))
			return *this;

		image_u8 new_image(new_width, new_height);

		const uint32_t w = std::min(m_width, new_width);
		const uint32_t h = std::min(m_height, new_height);

		for (uint32_t y = 0; y < h; y++)
			for (uint32_t x = 0; x < w; x++)
				new_image(x, y) = (*this)(x, y);

		return swap(new_image);
	}

	image_u8& swap(image_u8& other)
	{
		std::swap(m_width, other.m_width);
		std::swap(m_height, other.m_height);
		std::swap(m_pixels, other.m_pixels);
		return *this;
	}

	inline void get_block(uint32_t bx, uint32_t by, uint32_t width, uint32_t height, color_quad_u8* pPixels)
	{
		assert((bx * width + width) <= m_width);
		assert((by * height + height) <= m_height);

		for (uint32_t y = 0; y < height; y++)
			memcpy(pPixels + y * width, &(*this)(bx * width, by * height + y), width * sizeof(color_quad_u8));
	}

	inline void set_block(uint32_t bx, uint32_t by, uint32_t width, uint32_t height, const color_quad_u8* pPixels)
	{
		assert((bx * width + width) <= m_width);
		assert((by * height + height) <= m_height);

		for (uint32_t y = 0; y < height; y++)
			memcpy(&(*this)(bx * width, by * height + y), pPixels + y * width, width * sizeof(color_quad_u8));
	}

	image_u8& swizzle(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		assert((r | g | b | a) <= 3);
		for (uint32_t y = 0; y < m_height; y++)
		{
			for (uint32_t x = 0; x < m_width; x++)
			{
				color_quad_u8 tmp((*this)(x, y));
				(*this)(x, y).set(tmp[r], tmp[g], tmp[b], tmp[a]);
			}
		}

		return *this;
	}

private:
	color_quad_u8_vec m_pixels;
	uint32_t m_width, m_height;
};

struct bc7_block
{
	uint64_t m_vals[2];
};

typedef std::vector<bc7_block> bc7_block_vec;

void bc7compress(color_quad_u8_vec data, int _width, int _height, unsigned char*& output, int& outputSize, int quality);

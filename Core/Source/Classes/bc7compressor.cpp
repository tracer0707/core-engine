#include "bc7compressor.h"

void bc7compress(color_quad_u8_vec data, int _width, int _height, unsigned char*& output, int& outputSize, int quality)
{
	int uber_level = quality;
	int max_partitions_to_scan = BC7ENC16_MAX_PARTITIONS1;
	bool perceptual = false;
	bool y_flip = false;

	image_u8 source_image;
	source_image.init(_width, _height);
	color_quad_u8_vec& src = source_image.get_pixels();
	src = data;

	const uint32_t orig_width = source_image.width();
	const uint32_t orig_height = source_image.height();

	if (y_flip)
	{
		image_u8 temp;
		temp.init(orig_width, orig_height);

		for (uint32_t y = 0; y < orig_height; y++)
			for (uint32_t x = 0; x < orig_width; x++)
				temp(x, (orig_height - 1) - y) = source_image(x, y);

		temp.swap(source_image);
	}

	source_image.crop((source_image.width() + 3) & ~3, (source_image.height() + 3) & ~3);

	const uint32_t blocks_x = source_image.width() / 4;
	const uint32_t blocks_y = source_image.height() / 4;

	bc7_block_vec packed_image(blocks_x * blocks_y);

	bc7enc16_compress_block_params pack_params;
	bc7enc16_compress_block_params_init(&pack_params);
	if (!perceptual)
		bc7enc16_compress_block_params_init_linear_weights(&pack_params);
	pack_params.m_max_partitions_mode1 = max_partitions_to_scan;
	pack_params.m_uber_level = uber_level;

	//printf("Max mode 1 partitions: %u, uber level: %u, perceptual: %u\n", pack_params.m_max_partitions_mode1, pack_params.m_uber_level, perceptual);

	bc7enc16_compress_block_init();

	bool has_alpha = false;

	//clock_t start_t = clock();
	for (uint32_t by = 0; by < blocks_y; by++)
	{
		for (uint32_t bx = 0; bx < blocks_x; bx++)
		{
			color_quad_u8 pixels[16];

			source_image.get_block(bx, by, 4, 4, pixels);

			bc7_block* pBlock = &packed_image[bx + by * blocks_x];

			if (bc7enc16_compress_block(pBlock, pixels, &pack_params))
				has_alpha = true;
		}

		/*if ((by & 63) == 0)
		{
			printf(".");
			fflush(stdout);
		}*/
	}

	//clock_t end_t = clock();

	//printf("\nTotal time: %f secs\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);

	//if (has_alpha)
	//	printf("Source image had an alpha channel.\n");

	uint32_t imageSize = (((orig_width + 3) & ~3) * ((orig_height + 3) & ~3) * 8) >> 3;
	//output = new unsigned char[imageSize];
	memcpy(output, &packed_image[0], imageSize);
	outputSize = imageSize;
	
	source_image.clear();
	data.clear();
}

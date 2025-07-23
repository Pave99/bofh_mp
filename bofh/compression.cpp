#include "compression.h"
#include <zlib.h>

size_t compressGameState(const GameState& gameState, uint8_t* compressedBuffer)
{
	size_t gameStatesize = sizeof(GameState);
	
	uint8_t* uncompressedBuffer = const_cast<uint8_t*>((uint8_t*)&gameState);


	//zlib

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = gameStatesize;
	strm.next_in = uncompressedBuffer;
	strm.avail_out = COMPRESSED_GAMESTATE_MAX_SIZE;
	strm.next_out = compressedBuffer;

	int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if (ret != Z_OK){
		printf("deflateinit failed with error code %d\n",ret);
		return 0;
	}

	ret = deflate(&strm, Z_FINISH);
	if (ret == Z_STREAM_ERROR) {
		printf("deflate failed with error code %d\n",ret);

		deflateEnd(&strm);
		return 0;
	}

	size_t compressedSize = strm.total_out;

	deflateEnd(&strm);

	return compressedSize;
}

size_t decompressGameState(uint8_t* compressedBuffer, size_t compressedSize, uint8_t* decompressedBuffer)
{
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = compressedSize;
	strm.next_in = compressedBuffer;
	strm.avail_out = GAMESTATE_SIZE;
	strm.next_out = decompressedBuffer;

	int ret = inflateInit(&strm);
	if (ret != Z_OK) {
		printf("inflateinit failed with error code %d\n",ret);
		return 0;

	}

	ret = inflate(&strm, Z_FINISH);
	if (ret == Z_STREAM_ERROR) {
			printf("inflate failed with error code %d\n",ret);

			inflateEnd(&strm);
			return 0;
	}

	size_t decompressedSize = strm.total_out;

	inflateEnd(&strm);

	return decompressedSize;
}



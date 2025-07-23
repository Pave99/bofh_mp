#ifndef __COMPRESSION_H__
#define __COMPRESSION_H__

#include <game.h>
#include <inttypes.h>

#define COMPRESSED_GAMESTATE_MAX_SIZE 10000
#define GAMESTATE_SIZE sizeof(GameState)

size_t compressGameState(const GameState& gamestate, uint8_t* compressedBuffer);
size_t decompressGameState(uint8_t* compressedBuffer, size_t compressedSize, uint8_t* decompressedBuffer);

#endif




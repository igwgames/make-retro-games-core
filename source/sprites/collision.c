#include "source/globals.h"
#include "source/sprites/collision.h"
#include "source/library/bank_helpers.h"
#include "source/sprites/map_sprites.h"
#include "temp/tile_meta.h"

CODE_BANK(PRG_BANK_MAP_SPRITES);
ZEROPAGE_DEF(unsigned int, collisionTemp);

unsigned char test_collision(unsigned char tileId, unsigned char isPlayer) {
    // FIXME: Should unpack 2bit data here, rather than use 64. 
    // 11 11 11 11
    // 0  40 80 a0 - 64 based, but then grab bits based on top 2 bits.
    // The top 2 bits of the tile id are the palette color. We don't care about that here, so skip them.
    collisionTemp = (currentMapTilesetId<<6) + (tileId & 0x3f);
    return collisionDataLookup[collisionTemp];
}
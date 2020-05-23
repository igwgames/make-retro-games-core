#include "source/globals.h"
#include "source/sprites/collision.h"
#include "source/library/bank_helpers.h"
#include "source/sprites/map_sprites.h"
#include "temp/tile_meta.h"

CODE_BANK(PRG_BANK_MAP_SPRITES);
ZEROPAGE_DEF(unsigned int, collisionTemp);

unsigned char test_collision(unsigned char tileId, unsigned char isPlayer) {
    collisionTemp = currentMapTilesetId;
    collisionTemp <<= 7;
    collisionTemp += (tileId >> 1);
    if (tileId & 0x01 == 0) {
        return (collisionDataLookup[collisionTemp] & 0x0f);
    } else {
        return (collisionDataLookup[collisionTemp] >> 4);
    }
}
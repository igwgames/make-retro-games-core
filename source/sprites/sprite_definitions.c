#include "source/sprites/sprite_definitions.h"

// NOTE: All of this data is in the primary bank.

// First set
// Put all newly-designed sprites here. 8 Bytes per sprite, defined mostly from constants
// in sprite_definitions.h. The 8 bytes are: 
// 1st byte: Sprite type 
// 2nd byte: Tile id for first tile, using 8x8 grid. See the guide for more detail on this.
// 3rd byte: Split; contains sprite size and palette. Combine the constants
//           using the logical OR operator (represented by the bar character: |) 
// 4th byte: Animation type
// 5th byte: How the sprite moves.
// 6th-8th byte: Miscellaneous; you can use this for whatever extra data you need. For enemies, byte 5
//           is used as the damage they deal. Health powerups use byte 5 to store how much to restore. 
//           Byte 6 is used for movement speed on enemies and npcs.
//           Byte 7 is used for health on enemy sprites in our `add_sword` branch.
// NOTE: This array cannot contain more than 64 sprites, or other logic will break.

// Second set
// This is for groupings of 5 sprites. They aren't strictly linked, but define things like palette, chr bank, etc
// 1: chr bank
// 2: reserved?
// 3: reserved
// 4: reserved
// 5-8: reserved
// 8-16: 2 palettes for use in this group. Will be SPRITE_PALETTE_2 and SPRITE_PALETTE_3


// ----- RETRO INTERACTIVE CODE REPLACEMENT -----

const unsigned char spriteDefinitions[] = {
    SPRITE_TYPE_MONEY, 0xc0, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_0, SPRITE_ANIMATION_NONE, SPRITE_MOVEMENT_NONE, 1, 0, 0, 
    SPRITE_TYPE_KEY, 0xc2, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_1, SPRITE_ANIMATION_NONE, SPRITE_MOVEMENT_NONE, 1, 0, 0, 
    SPRITE_TYPE_DOOR, 0xc6, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_0, SPRITE_ANIMATION_NONE, SPRITE_MOVEMENT_NONE, 0, 0, 0, 
    SPRITE_TYPE_TELEPORT, 0xcc, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_1, SPRITE_ANIMATION_SWAP_FAST, SPRITE_MOVEMENT_NONE, 1, 0, 0, 
    SPRITE_TYPE_HEALTH, 0xe0, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_0, SPRITE_ANIMATION_NONE, SPRITE_MOVEMENT_NONE, 1, 0, 0, 
    SPRITE_TYPE_LOCKED_DOOR, 0xe6, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_0, SPRITE_ANIMATION_NONE, SPRITE_MOVEMENT_NONE, 1, 0, 0, 
    SPRITE_TYPE_ENDGAME, 0xc8, SPRITE_SIZE_16PX_16PX | SPRITE_PALETTE_0, SPRITE_ANIMATION_SWAP_FAST, SPRITE_MOVEMENT_NONE, 1, 0, 0
};

// ----- END RETRO INTERACTIVE CODE REPLACEMENT ----- 
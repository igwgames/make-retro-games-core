// We combine size and palette into one byte for sprites, since neither one needs a full byte. (Palette needs 2 bits!)
// The first 2 bits are used for the palette, and the rest used for the size.
// As a result, you have to use a mask to get either value - for example to get size, you have to do `theValue & SPRITE_SIZE_MASK`,
// or to get the palette, you can use `theValue & SPRITE_PALETTE_MASK`
#define SPRITE_SIZE_MASK 0x3f
#define SPRITE_SIZE_8PX_8PX 0x00
#define SPRITE_SIZE_16PX_16PX 0x01

#define SPRITE_PALETTE_MASK 0xc0
#define SPRITE_PALETTE_0 0x00
#define SPRITE_PALETTE_1 0x40
#define SPRITE_PALETTE_2 0x80
#define SPRITE_PALETTE_3 0xc0

#define SPRITE_ANIMATION_NONE 0
#define SPRITE_ANIMATION_SWAP 10
#define SPRITE_ANIMATION_SWAP_FAST 11
#define SPRITE_ANIMATION_FULL 50

#define SPRITE_MOVEMENT_NONE 0
#define SPRITE_MOVEMENT_LEFT_RIGHT 1
#define SPRITE_MOVEMENT_UP_DOWN 2

// Use _NOTHING for decoration sprites that don't do anything when collided with.
#define SPRITE_TYPE_NOTHING 0x00
#define SPRITE_TYPE_KEY 0x01
#define SPRITE_TYPE_HEALTH 0x02
#define SPRITE_TYPE_OFFSCREEN 0x7f
// Used _STATIC_ENEMY for sprites that deal damage, but otherwise don't move.
#define SPRITE_TYPE_STATIC_ENEMY 0x03 
#define SPRITE_TYPE_DOOR 0x04

// Used to figure out where to put the sprites in sprite memory. 
// Have to skip over sprite 0 (0x00) and player (0x10)
#define FIRST_ENEMY_SPRITE_OAM_INDEX 0x20

// How much to shift to get the position on spriteDefinitions. We store 8 bytes, so we shift by 3. 
// If you want to expand spriteDefinitions to 16 bytes per definition, change this to 4.
#define SPRITE_DEF_SHIFT 3

// How much to shift sprite positions to put them on the map. 
#define SPRITE_POSITION_SHIFT 4

// List of data contained in the sprite definitions array.
#define SPRITE_DEF_POSITION_TYPE            0
#define SPRITE_DEF_POSITION_TILE_ID         1
#define SPRITE_DEF_POSITION_SIZE_PALETTE    2
#define SPRITE_DEF_POSITION_ANIMATION_TYPE  3
#define SPRITE_DEF_POSITION_MOVEMENT_TYPE   4
#define SPRITE_DEF_POSITION_HP_RESTORE      5
#define SPRITE_DEF_POSITION_HEALTH          5
#define SPRITE_DEF_POSITION_SLIDE_SPEED     5

extern const unsigned char spriteDefinitions[];

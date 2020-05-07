/* PRG_BANK_SPRITE_COLLISION */
const fs = require('fs');

// NOTE: This bank needs to line up with the rest of the game!
// The sprite collision stuff can't afford to be constantly swapping.
const PRG_BANK_MAP_SPRITES = '02',
	TILE_START = 0,
	MAX_TILES = 10,
	SPRITE_START = 0x10, 
	MAX_SPRITES = 10,
	MAX_CHR = 32,
	SPLASH_CHR = 31,
	MENU_CHR = 0x0a;

const graphicsData = require('../../../graphics/graphics.json'),
	spriteData = require('../../../graphics/sprites.json');

graphicsData.forEach(function(spr) { 
	// Go through each sprite file and pack all of the info in collisionMap, 2 values per byte.
	// index becomes i >> 2, value becomes i%2==0?i&0b00001111:0b11110000>>4
	let packedSprites = [];
	let currentVal = 0;
	spr.collisionMap.forEach(function(val, idx) {
		if (idx % 2 === 0) {
			currentVal += val;
		} else {
			currentVal += (val << 4);
			packedSprites.push(currentVal);
			currentVal = 0;
		}
	});
	spr.packedCollisionMap = packedSprites;
});

let modifiedGraphicsData = [];
let i = 0;
for (; i != MAX_TILES; i++) {
	if (graphicsData[i]) {
		modifiedGraphicsData.push(graphicsData[i]);
	} else {
		modifiedGraphicsData.push(graphicsData[0]);
	}
}

// Fill in extra slots with "something" - just use first tiles for now.
for (; i != SPRITE_START; i++) {
	modifiedGraphicsData.push(graphicsData[0]);
}

// Now fill in up to sprites
for (; i != (SPRITE_START + MAX_SPRITES); i++) {
	if (spriteData[i]) {
		modifiedGraphicsData.push(spriteData[i]);
	} else {
		modifiedGraphicsData.push(spriteData[0]);
	}

}

// Fill in the rest of the sprites with "something"
for (; i != MAX_CHR; i++) {
	modifiedGraphicsData.push(spriteData[0]);
}

modifiedGraphicsData[SPLASH_CHR] = {
	chrFile: 'graphics/splash.pngE/chrblock.chr',
	paletteFile: 'graphics/splash.pngE/palette_edit.pal',
	collisionMap: undefined,
	type: 'splash'
};

modifiedGraphicsData[MENU_CHR] = {
	chrFile: 'graphics/ascii.chr',
	paletteFile: 'graphics/palettes/title.pal',
	collisionMap: undefined,
	type: 'menu'
};

let chrData = '',
	collisionData = '_collisionDataLookup:\n\n'
	tilePaletteData = '_tilePaletteDataLookup:\n\n',
	spritePaletteData = '_spritePaletteDataLookup:\n\n';

for (i = 0; i != modifiedGraphicsData.length; i++) {
	chrData += `
		.segment "CHR_${(''+(i).toString(16)).padStart(2, '0').toUpperCase()}"
			${modifiedGraphicsData[i].type === 'sprite' ? `.incbin "graphics/sprites_player.chr"` : ''}
			.incbin "${modifiedGraphicsData[i].chrFile}"
			${modifiedGraphicsData[i].type === 'sprite' ? `.incbin "graphics/sprites_other.chr"` : ''}
	`;

	if (i >= TILE_START && i < MAX_TILES) {
		tilePaletteData += `
			; Palette for tile ${i}
			.incbin "${modifiedGraphicsData[i].paletteFile}"
		`;

		collisionData += `
			; Collision Data for ${i}
`;

		// This isn't pretty, but it's a generated; meh...
		for (var j = 0; j != modifiedGraphicsData[i].packedCollisionMap.length; j++) {
			collisionData += '    .byte ' + modifiedGraphicsData[i].packedCollisionMap[j] + '\n';
		}

		collisionData += '\n\n\n';
	}

	if (i >= SPRITE_START && i < MAX_SPRITES) {
		spritePaletteData += `
			; Palette for sprite ${i}
			.incbin "${modifiedGraphicsData[i].paletteFile}"
		`;
	}



}

const output = `

.export _tilePaletteDataLookup, _spritePaletteDataLookup, _collisionDataLookup

${chrData}

.segment "ROM_${PRG_BANK_MAP_SPRITES}"

${collisionData}

${tilePaletteData}
${spritePaletteData}
`

fs.writeFileSync('temp/chr_data.asm', output);

const headerFile = `
	#define PRG_BANK_TILE_META ${PRG_BANK_MAP_SPRITES}

	extern const unsigned char tilePaletteDataLookup[];
	extern const unsigned char spritePaletteDataLookup[];
	extern const unsigned char collisionDataLookup[];
`;

fs.writeFileSync('temp/tile_meta.h', headerFile);
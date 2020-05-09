/* PRG_BANK_SPRITE_COLLISION */
const fs = require('fs'),
    path = require('path'),
    exec = require('child_process').exec;

// NOTE: This bank needs to line up with the rest of the game!
// The sprite collision stuff can't afford to be constantly swapping.
const MAX_MAPS = 6,
    FIRST_MAP = 0x08,
    FIRST_SPRITE_CHR_BANK = 0x10;

const levelData = require('../../../levels/levels.json'),
    graphicsData = require('../../../graphics/graphics.json'),
    spritesData = require('../../../graphics/sprites.json'),
    spriteDataIds = spritesData.map(sprite => { return sprite.id }),
    updatedLevelData = [],
	promises = [];

// Get exactly the amount of data we need; pad up as needed.
for (var i = 0; i != MAX_MAPS; i++) {
    if (levelData[i]) {
        updatedLevelData.push(levelData[i]);
    } else {
        updatedLevelData.push(levelData[0]);
    }
}

updatedLevelData.forEach(function(level, val) { 
    promises.push(new Promise((resolve, reject) => {
        const commandLine = 'node ' + path.join('tools', 'tmx2c', 'src', 'index.js') + ' ' + (FIRST_MAP + val).toString(16).toUpperCase() + ' map_' + val + ' ' + level.tmxFile + ' temp/map__' + val;
        exec(commandLine, {}, function(err, stdout, stderr) {
            console.info(stdout, stderr);
            if (err) {
                reject(err);
                return
            }

            const tileIndex = graphicsData.map(graphic => { return graphic.id; }).indexOf(level.tilesetId);
            if (tileIndex === -1) {
                reject('Tileset for map "' + level.name + '" not found!');
                return;
            }
            let spriteIndexes = []
            level.spritesetIds.forEach(spriteset => { 
                spriteIndexes.push(spriteDataIds.indexOf(spriteset));
            });
            if (spriteIndexes.filter(a => { return a === -1 }).length > 0) {
                reject('Some sprite groups referenced in "' + level.name + '" were not found in the list of availabe spritesets.');
                return;
            }
            
            const metaContent = `

#include "source/library/bank_helpers.h"
CODE_BANK(${(FIRST_MAP + val).toString(16).toUpperCase()});

const unsigned char map_${val}_spritegroups[] = {
    ${spriteIndexes.join(',\n    ')}
};

const unsigned char map_${val}_chr_bank_id = ${tileIndex};
CODE_BANK_POP();
            `;

            fs.writeFileSync('temp/map_'+val+'_meta.c', metaContent);

            resolve();
        });


    }));
});

(async function() {
    try { 
        await Promise.all(promises);
    } catch (e) {
        console.error('FAILED MAP CONVERSION', e);
        process.exit(1);
    }
    console.info('Done converting ' + updatedLevelData.length + ' files!');
})();

let spriteGroupData = [];
spritesData.forEach((sprite, idx) => {
    let thisOne = []
    thisOne.push(FIRST_SPRITE_CHR_BANK + idx);
    for (var i = 0; i != 7; i++) {
        thisOne.push(0);
    }
    let dataBuff = fs.readFileSync(sprite.paletteFile);
    for (var i = 0; i != 8; i++) {
        thisOne.push(dataBuff[i]);
    }
    spriteGroupData.push(thisOne.join(', '));
});

const outputData = `
const unsigned char spriteDefinitionGroups[] = {
    ${spriteGroupData.join(',\n')}
};
`

const outputH = `
#define FIRST_TILESET_BANK_ID ${FIRST_SPRITE_CHR_BANK}

extern const unsigned char spriteDefinitionGroups[]; 
extern const unsigned char map_0_chr_bank_id;
extern const unsigned char map_0_spritegroups[];
`

fs.writeFileSync('temp/sprite_groups.c', outputData);
fs.writeFileSync('temp/sprite_groups.h', outputH);

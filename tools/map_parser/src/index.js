/* PRG_BANK_SPRITE_COLLISION */
const fs = require('fs'),
    path = require('path'),
    exec = require('child_process').exec;

// NOTE: This bank needs to line up with the rest of the game!
// The sprite collision stuff can't afford to be constantly swapping.
const MAX_MAPS = 6,
    FIRST_MAP = 0x08;

const levelData = require('../../../levels/levels.json'),
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

            resolve();
        });
    }));
});

(async function() {
    await Promise.all(promises);
    console.info('Done converting ' + updatedLevelData.length + ' files!');
})();
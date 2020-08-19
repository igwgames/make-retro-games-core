const baseText = require('../../../source/menus/credits.json'),
    fs = require('fs');


let theString = '';

for (var i = 0; i < baseText.length; i++) {
    if (baseText[i].who && baseText[i].what) {
        theString += '" ' + baseText[i].what + '\\n"  ';
        theString += '"   ' + baseText[i].who + '\\n"  ';
        theString += '"\\n"  ';
    } else if (baseText[i].isSpacer) {
        theString += '"\\n\\n"';
    } else if (baseText[i].custom) {

        // Custom text should always be centered.
        var spacer = '';
        for (var j = 0; j < ((32 - baseText[i].custom.length) / 2); j++) {
            spacer += ' ';
        }

        theString += '"' + spacer + baseText[i].custom +'\\n\\n"  ';
    } else {
        logger.info('Unknown type of credits data encountered', baseText[i]);
    }
}

theString += '';

const fileText = 
`
// ----- MAKERETROGAMES CODE REPLACEMENT -----
// ----- Changes in this area are discouraged and may be lost! -----
#define RI_CREDITS_TEXT ${theString}
// ----- END MAKERETROGAMES CODE REPLACEMENT -----
`;

fs.writeFileSync('temp/credits_text.h', fileText);
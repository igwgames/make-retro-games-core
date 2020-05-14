#include "source/menus/credits.h"
#include "source/graphics/palettes.h"
#include "source/configuration/system_constants.h"
#include "source/globals.h"
#include "source/neslib_asm/neslib.h"
#include "source/configuration/game_states.h"
#include "source/menus/text_helpers.h"
#include "source/configuration/game_info.h"

CODE_BANK(PRG_BANK_CREDITS_MENU);

// ----- RETRO INTERACTIVE CODE REPLACEMENT -----
// ----- Changes in this area are discouraged and may be lost! -----
    #define RI_CREDITS_TEXT ""  "       ----- Credits -----\n\n"  "\n\n"" Game Builder\n"  "   Retro-Interactive\n"  "\n"  " Game Creation\n"  "   You!\n"  "\n"  " Music\n"  "   Wolfgang (OpenGameArt)\n"  "\n"  " Artwork\n"  "   Refresh Games (OpenGameArt)\n"  "\n"  "\n\n""   Your custom text goes here\n\n"  "\n\n""       Thanks for playing!\n\n"    ""
// ----- END RETRO INTERACTIVE CODE REPLACEMENT ----- 

const unsigned char creditsText[] = RI_CREDITS_TEXT;

void draw_win_screen(void) {

    set_vram_update(NULL);
    ppu_off();
    clear_screen();
    // We reuse the title palette here, though we have the option of making our own if needed.
    pal_bg(titlePalette);
	pal_spr(titlePalette);
    scroll(0, 0);

	set_chr_bank_0(CHR_BANK_MENU);
    set_chr_bank_1(CHR_BANK_MENU);

    // Add whatever you want here; NTADR_A just picks a position on the screen for you. Your options are 0, 0 to 32, 30
    put_str(NTADR_A(7, 2), "- Congratulations -");

    put_str(NTADR_A(4, 8), "You did the thing, and");
    put_str(NTADR_A(4, 9), "thus won the game!");

    // Hide all existing sprites
    oam_clear();
    ppu_on_all();

}

void draw_credits_screen(void) {
    set_vram_update(NULL);
    ppu_off();
    set_mirroring(MIRROR_MODE_HORIZONTAL);
    oam_clear();

    // NOTE: We're doing some trickery here with palettes... title palette 0 and 2 are identical, but
    // we use 0 on the top and 2 on the bottom, then toggle them to all black temporarily during scroll
    // Borrow screenBuffer for it to use it
    memcpy(screenBuffer, titlePalette, 16);
    screenBuffer[9] = 0x0f;
    screenBuffer[10] = 0x0f;
    screenBuffer[11] = 0x0f;

    pal_bg(screenBuffer);
	pal_spr(screenBuffer);

    clear_screen();
    clear_screen_c();
    clear_attrs_a(0);
    clear_attrs_c(0xaa);


    tempInt2 = 240; // Scroll row id
    scroll(0, tempInt2);

	set_chr_bank_0(CHR_BANK_MENU);
    set_chr_bank_1(CHR_BANK_MENU);
    
    // Kill palette 3 so we can start there and scroll into normal credits
    vram_adr(0x3f09);
    vram_put(0x0f);
    vram_put(0x0f);
    vram_put(0x0f);


    // Loop over all text in the credits...
    tempChar1 = 0; // row id
    vram_adr(NTADR_A(0, tempChar1));
    for (tempInt1 = 0; ; ++tempInt1) {

        // Break out on the null character at the end    
        if (creditsText[tempInt1] == 0) {
            break;
        }

        if (creditsText[tempInt1] == '\n') {
            if (tempChar1 > 28) {
                vram_adr(NTADR_C(0, (++tempChar1 - 30)));
            } else {
                vram_adr(NTADR_A(0, (++tempChar1)));
            }
            continue;
        }

        vram_put(creditsText[tempInt1] - 0x20);
    }

    ppu_on_all();

    while (tempInt2 != 239) {

        // NOTE: Scroll appears to break after 480, which is perhaps not shocking
        // We probably have to redraw everything in NT_A and seamlessly scroll
        scroll(0, ++tempInt2);
        ppu_wait_nmi();
        ppu_wait_nmi();

        if (tempInt2 == 479) {
            tempInt2 = 0;
            // Now show both sets
            memcpy(screenBuffer, titlePalette, 16);
            pal_bg(screenBuffer);
        }
    }
    // Okay, one more loop to play it all out
    screenBuffer[1] = 0x0f;
    screenBuffer[2] = 0x0f;
    screenBuffer[3] = 0x0f;
    pal_bg(screenBuffer);

    while (tempInt2 != 479) {
        scroll(0, ++tempInt2);
        ppu_wait_nmi();
        ppu_wait_nmi();
    }

    delay(20);

}
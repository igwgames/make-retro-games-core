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
    #define RI_CREDITS_TEXT ""  "Game Builder\n"  "  Retro-Interactive\n"  "\n"  "Game Creation\n"  "  You!\n"  "\n"  "Music\n"  "  Wolfgang (OpenGameArt)\n"  "\n"  "Artwork\n"  "  Refresh Games (OpenGameArt)\n"  "\n"  "Your custom text goes here\n\n"  "More custom text goes here\n\n"  "\n\n""Thanks for playing!\n\n"    ""
// ----- END RETRO INTERACTIVE CODE REPLACEMENT ----- 

const unsigned char creditsText[] = RI_CREDITS_TEXT;

void draw_win_screen(void) {
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
    ppu_off();
    set_mirroring(MIRROR_MODE_HORIZONTAL);
    oam_clear();
    clear_screen();
    clear_screen_c();
    // We reuse the title palette here, though we have the option of making our own if needed.
    pal_bg(titlePalette);
	pal_spr(titlePalette);
    tempInt2 = 0; // Scroll row id
    scroll(0, tempInt2);

	set_chr_bank_0(CHR_BANK_MENU);
    set_chr_bank_1(CHR_BANK_MENU);
    
    // Loop over all text in the credits...
    tempChar1 = 31; // row id
    vram_adr(NTADR_C(0, tempChar1 - 30));
    for (tempInt1 = 0; ; ++tempInt1) {

        // Break out on the null character at the end    
        if (creditsText[tempInt1] == 0) {
            break;
        }

        if (creditsText[tempInt1] == '\n') {
            if (tempChar1 > 59) {
                ppu_on_all();
                while (tempInt2 != 239) {
                    ++tempInt2;
                    scroll(0, tempInt2);
                    ppu_wait_nmi();
                    ppu_wait_nmi();
                }
                tempChar1 = 0-1;
                scroll(0, tempInt2);
                ppu_off();
                ppu_wait_nmi();
            }
            if (tempChar1 > 29) {
                vram_adr(NTADR_C(0, (++tempChar1 - 30)));
            } else {
                vram_adr(NTADR_A(0, (++tempChar1)));
            }
            continue;
        }

        vram_put(creditsText[tempInt1] - 0x20);
    }


    // Hide all existing sprites
    ppu_on_all();

    while (tempInt2 != 479) {

        // NOTE: Scroll appears to break after 480, which is perhaps not shocking
        // We probably have to redraw everything in NT_A and seamlessly scroll
        scroll(0, ++tempInt2);
        ppu_wait_nmi();
        ppu_wait_nmi();
    }

    delay(40);
}

void do_credits_input(void) {
    // Do nothing, just scroll credits then move on.
    return;/*
    while (1) {
        lastControllerState = controllerState;
        controllerState = pad_poll(0);

        // If Start is pressed now, and was not pressed before...
        if (controllerState & PAD_START && !(lastControllerState & PAD_START)) {
            break;
        }

        ppu_wait_nmi();

    }*/
}
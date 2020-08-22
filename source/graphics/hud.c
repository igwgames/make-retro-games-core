#include "source/neslib_asm/neslib.h"
#include "source/graphics/hud.h"
#include "source/globals.h"
#include "source/configuration/hud.h"

CODE_BANK(PRG_BANK_HUD);

void draw_hud(void) {
    vram_adr(NAMETABLE_A + HUD_POSITION_START);
    for (i = 0; i != 160; ++i) {
        vram_put(HUD_TILE_BLANK);
    }
    vram_put(HUD_TILE_BORDER_BL);
    for (i = 0; i != 30; ++i) {
        vram_put(HUD_TILE_BORDER_HORIZONTAL);
    }
    vram_put(HUD_TILE_BORDER_BR);

    vram_adr(NAMETABLE_A + HUD_ATTRS_START);
    for (i = 0; i != 16; ++i) {
        vram_put(0xff);
    }
}

void update_hud(void) {
    // Make sure we don't update vram while this is in progress, or we could break something.
    set_vram_update(NULL);
    // This sets up screenBuffer to print x hearts, then x more empty hearts. 
    // You give it the address, tell it the direction to write, then follow up with
    // Ids, ending with NT_UPD_EOF
    
    // We use i for the index on screen buffer, so we don't have to shift things around
    // as we add values. 
    i = 0;
    screenBuffer[i++] = MSB(NAMETABLE_A + HUD_HEART_START) | NT_UPD_HORZ;
    screenBuffer[i++] = LSB(NAMETABLE_A + HUD_HEART_START);
    screenBuffer[i++] = playerMaxHealth;
    // Add one heart for every health the player has
    for (j = 0; j != playerHealth; ++j) {
        #if HUD_SHOW_HEALTH_BOOL
            screenBuffer[i++] = HUD_TILE_HEART;
        #else
            screenBuffer[i++] = HUD_TILE_BLANK;
        #endif
    }
    // Using the same variable, add empty hearts up to max health
    for (; j != playerMaxHealth; ++j) {
        #if HUD_SHOW_HEALTH_BOOL
            screenBuffer[i++] = HUD_TILE_HEART_EMPTY;
        #else
            screenBuffer[i++] = HUD_TILE_BLANK;
        #endif
    }

    // Next, draw the key count, using the key tile, and our key count variable
    #if HUD_SHOW_KEYS_BOOL
        screenBuffer[i++] = MSB(NAMETABLE_A + HUD_KEY_START) | NT_UPD_HORZ;
        screenBuffer[i++] = LSB(NAMETABLE_A + HUD_KEY_START);
        screenBuffer[i++] = 2;
        screenBuffer[i++] = HUD_TILE_KEY;
        screenBuffer[i++] = HUD_TILE_NUMBER + playerKeyCount;
    #endif

    #if HUD_SHOW_MONEY_BOOL
        screenBuffer[i++] = MSB(NAMETABLE_A + HUD_MONEY_START) | NT_UPD_HORZ;
        screenBuffer[i++] = LSB(NAMETABLE_A + HUD_MONEY_START);
        screenBuffer[i++] = 4;
        screenBuffer[i++] = HUD_TILE_MONEY;
        screenBuffer[i++] = HUD_TILE_NUMBER + ((playerMoneyCount >> 8) & 0x0f);
        screenBuffer[i++] = HUD_TILE_NUMBER + ((playerMoneyCount >> 4) & 0x0f);
        screenBuffer[i++] = HUD_TILE_NUMBER + (playerMoneyCount & 0x0f);

    #endif

    // FIXME: Make sure the buffer is big enough to fit this


    screenBuffer[i++] = NT_UPD_EOF;
    set_vram_update(screenBuffer);

}
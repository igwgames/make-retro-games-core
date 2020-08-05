; Startup code for cc65 and Shiru's NES library
; based on code by Groepaz/Hitmen <groepaz@gmx.net>, Ullrich von Bassewitz <uz@cc65.org>
.feature c_comments

FT_SFX_STREAMS			= 4			;number of sound effects played at once, 1..4

.define FT_DPCM_ENABLE  1			;undefine to exclude all DMC code
.define FT_SFX_ENABLE   1			;undefine to exclude all sound effects code

.include "source/neslib_asm/mmc1_macros.asm"

    .export _exit,__STARTUP__:absolute=1
	.import initlib,push0,popa,popax,_main,zerobss,copydata,pusha,pushax,_fade_in,_fade_out_slow, _gameAuthor, _currentYear
	.importzp _tempChar8, _tempChar9, _tempChara

; Linker generated symbols
	.import __RAM_START__   ,__RAM_SIZE__
	.import __ROM0_START__  ,__ROM0_SIZE__
	.import __STARTUP_LOAD__,__STARTUP_RUN__,__STARTUP_SIZE__
	.import	__CODE_LOAD__   ,__CODE_RUN__   ,__CODE_SIZE__
	.import	__RODATA_LOAD__ ,__RODATA_RUN__ ,__RODATA_SIZE__
	.import NES_PRG_BANKS,NES_CHR_BANKS,NES_MAPPER
    .include "tools/cc65/asminc/zeropage.inc"

	.export _frameCount



FT_BASE_ADR=$0100			;page in RAM, should be $xx00

.define FT_THREAD       1	;undefine if you call sound effects in the same thread as sound update
.define FT_PAL_SUPPORT	0   ;undefine to exclude PAL support
.define FT_NTSC_SUPPORT	1   ;undefine to exclude NTSC support


PPU_CTRL	=$2000
PPU_MASK	=$2001
PPU_STATUS	=$2002
PPU_OAM_ADDR=$2003
PPU_OAM_DATA=$2004
PPU_SCROLL	=$2005
PPU_ADDR	=$2006
PPU_DATA	=$2007
PPU_OAM_DMA	=$4014
DMC_FREQ	=$4010
CTRL_PORT1	=$4016
CTRL_PORT2	=$4017

OAM_BUF		=$0200
PAL_BUF		=$01c0



.segment "ZEROPAGE"

NTSC_MODE: 			.res 1
_frameCount: ; By doing this, I make it available to C code with an extern
FRAME_CNT1: 		.res 1
FRAME_CNT2: 		.res 1
VRAM_UPDATE: 		.res 1
NAME_UPD_ADR: 		.res 2
NAME_UPD_ENABLE: 	.res 1
PAL_UPDATE: 		.res 1
PAL_BG_PTR: 		.res 2
PAL_SPR_PTR: 		.res 2
SCROLL_X: 			.res 1
SCROLL_Y: 			.res 1
SCROLL_X1: 			.res 1
SCROLL_Y1: 			.res 1
WRITE1:             .res 1   
WRITE2:             .res 2      ; For extra X/Y split data.
PAD_STATE: 			.res 2		;one byte per controller
PAD_STATEP: 		.res 2
PAD_STATET: 		.res 2
PPU_CTRL_VAR: 		.res 1
PPU_CTRL_VAR1: 		.res 1
PPU_MASK_VAR: 		.res 1
RAND_SEED: 			.res 2
BP_BANK:            .res 1
BP_BANK_TEMP:       .res 1
NMI_BANK_TEMP:      .res 1
FT_TEMP: 			.res 3

MUSIC_PLAY:			.res 1
ft_music_addr:		.res 2

BUF_4000:			.res 1
BUF_4001:			.res 1
BUF_4002:			.res 1
BUF_4003:			.res 1
BUF_4004:			.res 1
BUF_4005:			.res 1
BUF_4006:			.res 1
BUF_4007:			.res 1
BUF_4008:			.res 1
BUF_4009:			.res 1
BUF_400A:			.res 1
BUF_400B:			.res 1
BUF_400C:			.res 1
BUF_400D:			.res 1
BUF_400E:			.res 1
BUF_400F:			.res 1

PREV_4003:			.res 1
PREV_4007:			.res 1

TEMP: 				.res 11

PAD_BUF		=TEMP+1

PTR			=TEMP	;word
LEN			=TEMP+2	;word
NEXTSPR		=TEMP+4
SCRX		=TEMP+5
SCRY		=TEMP+6
SRC			=TEMP+7	;word
DST			=TEMP+9	;word

RLE_LOW		=TEMP
RLE_HIGH	=TEMP+1
RLE_TAG		=TEMP+2
RLE_BYTE	=TEMP+3



.segment "HEADER"

    .byte $4e,$45,$53,$1a
	.byte <NES_PRG_BANKS
	.byte <NES_CHR_BANKS
	.byte <NES_MAPPER
	.byte 0
	.res 8,0



.segment "STARTUP"

start:
_exit:

    sei
    ldx #$ff
    txs
    inx
    stx PPU_MASK
    stx DMC_FREQ
    stx PPU_CTRL		;no NMI

initPPU:

    bit PPU_STATUS
@1:
    bit PPU_STATUS
    bpl @1
@2:
    bit PPU_STATUS
    bpl @2

clearPalette:

	lda #$3f
	sta PPU_ADDR
	stx PPU_ADDR
	lda #$0f
	ldx #$20
@1:
	sta PPU_DATA
	dex
	bne @1

clearVRAM:

	txa
	ldy #$20
	sty PPU_ADDR
	sta PPU_ADDR
	ldy #$10
@1:
	sta PPU_DATA
	inx
	bne @1
	dey
	bne @1

clearRAM:

    txa
@1:
    sta $000,x
    sta $100,x
    sta $200,x
    sta $300,x
    sta $400,x
    sta $500,x
    sta $600,x
    sta $700,x
	; Clear WRAM area. SRAM is 7k
	sta $6000,x
	sta $6100,x
	sta $6200,x
	sta $6300,x
	sta $6400,x
	sta $6500,x
	sta $6600,x
	sta $6700,x
	sta $6800,x
	sta $6900,x
	sta $6a00,x
	sta $6b00,x
	sta $6c00,x
	sta $6d00,x
	sta $6e00,x
	sta $6f00,x
    inx
    bne @1

	lda #4
	jsr _pal_bright
	jsr _pal_clear
	jsr _oam_clear

    jsr	zerobss
	jsr	copydata

    lda #<(__RAM_START__+__RAM_SIZE__)
    sta	sp
    lda	#>(__RAM_START__+__RAM_SIZE__)
    sta	sp+1            ; Set argument stack ptr

	jsr	initlib

	lda #%10000000
	sta <PPU_CTRL_VAR
	sta PPU_CTRL		;enable NMI
	lda #%00000110
	sta <PPU_MASK_VAR

	lda #<(_main)
	clc
	adc #120
	sta _tempChar8
	lda #>(_main)
	sta _tempChar9


waitSync3:
	lda <FRAME_CNT1
@1:
	cmp <FRAME_CNT1
	beq @1

detectNTSC:
	ldx #52				;blargg's code
	ldy #24
@1:
	dex
	bne @1
	dey
	bne @1

	lda PPU_STATUS
	and #$80
	sta <NTSC_MODE

	jsr _ppu_off

	lda #$ff				;previous pulse period MSB, to not write it when not changed
	sta PREV_4003
	sta PREV_4007
	
	jsr _music_stop

.if(FT_SFX_ENABLE)
	ldx #<sounds_data
	ldy #>sounds_data
	jsr FamiToneSfxInit
.endif

	lda #$fd
	sta <RAND_SEED
	sta <RAND_SEED+1

	lda #0
	sta PPU_SCROLL
	sta PPU_SCROLL			
	sta PPU_OAM_ADDR

	lda #%11111
	mmc1_register_write MMC1_CTRL
	lda #0
	mmc1_register_write MMC1_PRG
	mmc1_register_write MMC1_CHR0

	lda #0
	ldx #0
	jsr _set_vram_update

	; This is a little weird - this variable expects to be a non-zero value on startup.
	; We call this method to set it properly so the average user doesn't see this weirdness.
	; If you're not the average engine user, hi! Welcome to the code behind the matrix ;)
	jsr _unset_nmi_chr_tile_bank

	lda #06
	sta BP_BANK
	mmc1_register_write MMC1_PRG
	jsr _draw_splash
	lda _tempChar8
	sec
	sbc _tempChara
	sta _tempChar8
	.byte $6c, <(_tempChar8), >(_tempChar8)
	@continue:

	.include "source/neslib_asm/ft_drv/driver.s"
    .include "source/library/bank_helpers.asm"
	.include "source/neslib_asm/neslib.asm"
	.include "source/graphics/palettes.asm"

.include "temp/chr_data.asm"

; MMC1 needs a reset stub in every bank that will put us into a known state. This defines it for all banks.
.repeat (SYS_PRG_BANKS-1), I
	resetstub_in .concat("STUB_", .sprintf("%02X", I))
.endrepeat 
resetstub_in "STUB_PRG"

; Throw a single jmp to reset in every bank other than the main PRG bank. This accomplishes 2 things:
; 1) Puts something in the bank, so we avoid warnings
; 2) If we somehow end up here by accident, we'll reset correctly.
.repeat (SYS_PRG_BANKS-1), I
	first_byte_reset_in .concat("ROM_", .sprintf("%02X", I))
.endrepeat


.segment "ROM_00"

music_data:

	.incbin "sound/music/music.bin"
	
music_dummy_data:

	.byte $0D,$00,$0D,$00,$0D,$00,$0D,$00,$00,$10,$0E,$B8,$0B,$0F,$00,$16
	.byte $00,$01,$40,$06,$96,$00,$18,$00,$22,$00,$22,$00,$22,$00,$22,$00
	.byte $22,$00,$00,$3F

.if(FT_SFX_ENABLE)
sounds_data:
	.include "sound/sfx/generated/sfx.s"
.endif

.segment "DMC"

.if(FT_DPCM_ENABLE)
	.incbin "sound/samples/samples.bin"
.endif

.segment "ROM_06"
	_draw_splash:
		

		; gpl

		jsr _oam_clear
		jsr _ppu_off
		jsr _ppu_wait_nmi
		lda #$0a
		jsr _set_chr_bank_0
		lda #0a
		jsr _set_chr_bank_1
		lda #1
		jsr _bank_spr
		lda #<(_titlePalette)
		ldx #>(_titlePalette)
		jsr _pal_bg
		lda #<(splash_pal)
		ldx #>(splash_pal)
		jsr _pal_spr

		lda #<($2000)
		ldx #>($2000)
		jsr _vram_adr

		lda #<(gpl_nam)
		ldx #>(gpl_nam)
		jsr _vram_unrle

		lda #<($2062)
		ldx #>($2062)
		jsr _vram_adr
		ldx #0
		@loop_name:
			lda _gameAuthor, x
			cmp #0
			beq @loop_name_done
			sec
			sbc #$20
			sta PPU_DATA
			inx
			jmp @loop_name
		@loop_name_done:
		lda #<($2050)
		ldx #>($2050)
		jsr _vram_adr
		ldx #0
		.repeat 4
			lda _currentYear, x
			sec
			sbc #$20 
			sta PPU_DATA
			inx
		.endrepeat

		lda #160
		sta _tempChara

		lda #0
		jsr _pal_bright
		jsr _ppu_wait_nmi
		jsr _ppu_on_all
		jsr _ppu_wait_nmi
		jsr _fade_in
		
		lda #200
		sta _tempChara

		@_loop_time2:
			jsr _ppu_wait_nmi
			inc _tempChara
			lda _tempChara
			cmp #120
			bne @_loop_time2

		@loop_until_start:
			jsr _ppu_wait_nmi
			lda #0
			jsr _pad_trigger
			and #$08
			cmp #0
			beq @loop_until_start
		
		jsr _fade_out_slow

		; splash
		jsr _oam_clear
		jsr _ppu_off
		jsr _ppu_wait_nmi
		lda #$1f
		jsr _set_chr_bank_0
		lda #10
		jsr _set_chr_bank_1
		lda #1
		jsr _bank_spr
		lda #<(splash_pal)
		ldx #>(splash_pal)
		jsr _pal_bg
		;lda #<(splash_pal)
		;ldx #>(splash_pal)
		;jsr _pal_spr

		lda #<($2000)
		ldx #>($2000)
		jsr _vram_adr

		lda #<(splash_nam)
		ldx #>(splash_nam)
		jsr pushax
		lda #<(1024)
		ldx #>(1024)
		jsr _vram_write

		lda #160
		sta _tempChara
		ldx #0
		ldy #0
		@loop_ver:
			lda ENGINE_VERSION, x
			cmp #0
			beq @end_loop

			lda #12
			sta $200, y
			iny
			lda ENGINE_VERSION, x
			cmp #46
			bne @skip_dot
				dey
				lda #20
				sta $200, y
				iny
				lda #$07 ; 10 less than the id, for math below
			@skip_dot:

			sec
			sbc #$20
			sta $200, y
			iny
			lda #0
			sta $200, y
			iny
			dex
			lda ENGINE_VERSION, x
			inx
			cmp #46
			beq @period
				; No Period; re-space
				lda _tempChara
				clc
				adc #8
				sta _tempChara

				jmp @after_period
			@period:
				lda _tempChara
				clc
				adc #4
				sta _tempChara

			@after_period:
			sta $200, y 
			iny

			inx
			jmp @loop_ver
		@end_loop:

		lda #0
		jsr _pal_bright
		jsr _ppu_wait_nmi
		jsr _ppu_on_all
		jsr _ppu_wait_nmi
		jsr _fade_in
		
		lda #0 
		sta _tempChara

		@_loop_time:
			jsr _ppu_wait_nmi
			inc _tempChara
			lda _tempChara
			cmp #120
			bne @_loop_time
		
		jsr _fade_out_slow

		rts
	
	splash_nam:
		.incbin "graphics/splash.pngE/nametable0.nam"
	splash_pal:
		.incbin "graphics/splash.pngE/palette_edit.pal"
	gpl_nam:
		.incbin "graphics/gpl.rle"
	.include "source/configuration/engine_version.asm"
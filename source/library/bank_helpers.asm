.feature c_comments
.linecont +

.define NO_CHR_BANK 255

.segment "ZEROPAGE"
    nmiChrTileBank: .res 1
    .exportzp nmiChrTileBank

.segment "CODE"

.export _set_prg_bank, _get_prg_bank, _set_chr_bank_0, _set_chr_bank_1
.export _set_nmi_chr_tile_bank, _unset_nmi_chr_tile_bank
.export _set_mirroring

_set_prg_bank:

    ; Store new bank into BP_BANK
    sta BP_BANK

    ; Set a flag for the nmi method to check, on the offchance it catches us-mid-write
    ldx #1
    stx BANK_WRITE_IP

    ; Write it to the reg (destroys a)
    jsr _set_prg_bank_raw

    ; See if nmi ran
    ldx BANK_WRITE_IP
    cpx #2
    bne @no_nmi
        ; Yep. Need to run nmi once we're done.
        ldx #0
        stx BANK_WRITE_IP
        jsr real_nmi
    @no_nmi:
    ; Carry on as we were.
    ldx #0
    stx BANK_WRITE_IP

    rts
_set_prg_bank_raw:
    mmc1_register_write MMC1_PRG
    rts

_get_prg_bank:
    lda BP_BANK
    rts

_set_chr_bank_0:
    mmc1_register_write MMC1_CHR0
    rts

_set_chr_bank_1:
    mmc1_register_write MMC1_CHR1
    rts

; Both of these just set/unset a varible used in `neslib.asm` to trigger this during nmi.
_set_nmi_chr_tile_bank: 
    sta nmiChrTileBank
    rts

_unset_nmi_chr_tile_bank:
    lda #NO_CHR_BANK
    sta nmiChrTileBank
    rts

_set_mirroring:
    ; Limit this to mirroring bits, so we can add our bytes safely.
    and #%00000011
    ; Now, set this to have 4k chr banking, and not mess up which prg bank is which.
    ora #%00011100
    ; Bombs away!
    mmc1_register_write MMC1_CTRL
    rts
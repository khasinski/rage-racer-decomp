.set noat
.set noreorder

/* libgte InitGeom: brings the GTE up.
 *
 * It calls the exception-vector patch, sets the COP2-enable bit in the COP0
 * status register, then loads the GTE control registers with the screen offset,
 * projection distance and fog range the library defaults to. mfc0/mtc0 and
 * ctc2 are the entire routine; none of it is expressible in C, so it was
 * assembly in the original. $ra is parked at a fixed global across the call,
 * reached through %hi/%lo so the block still relocates.
 */
glabel InitGeom
    /* 80068928 */  lui        $at, %hi(D_80094B0C)
    /* 8006892C */  sw         $ra, %lo(D_80094B0C)($at)
    /* 80068930 */  jal        func_80069FA8
    /* 80068934 */   nop
    /* 80068938 */  lui        $ra, %hi(D_80094B0C)
    /* 8006893C */  lw         $ra, %lo(D_80094B0C)($ra)
    /* 80068940 */  nop
    /* 80068944 */  mfc0       $v0, $12
    /* 80068948 */  lui        $v1, (0x40000000 >> 16)
    /* 8006894C */  or         $v0, $v0, $v1
    /* 80068950 */  mtc0       $v0, $12
    /* 80068954 */  nop
    /* 80068958 */  addiu      $t0, $zero, 0x155
    /* 8006895C */  ctc2       $t0, $29
    /* 80068960 */  nop
    /* 80068964 */  addiu      $t0, $zero, 0x100
    /* 80068968 */  ctc2       $t0, $30
    /* 8006896C */  nop
    /* 80068970 */  addiu      $t0, $zero, 0x3E8
    /* 80068974 */  ctc2       $t0, $26
    /* 80068978 */  nop
    /* 8006897C */  addiu      $t0, $zero, -0x1062
    /* 80068980 */  ctc2       $t0, $27
    /* 80068984 */  nop
    /* 80068988 */  lui        $t0, (0x1400000 >> 16)
    /* 8006898C */  ctc2       $t0, $28
    /* 80068990 */  nop
    /* 80068994 */  ctc2       $zero, $24
    /* 80068998 */  ctc2       $zero, $25
    /* 8006899C */  nop
    /* 800689A0 */  jr         $ra
    /* 800689A4 */   nop
endlabel InitGeom

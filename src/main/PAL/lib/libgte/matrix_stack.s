.set noat
.set noreorder

/* libgte matrix stack.
 *
 * cfc2/ctc2 move the GTE rotation matrix and translation vector between the
 * coprocessor and a 20-deep stack in bss, which is the whole job and has no C
 * spelling. The stack depth and the saved $ra live at fixed globals that the
 * routines reach through %hi/%lo, so the block relocates with the image
 * instead of pinning the retail addresses.
 */

glabel PushMatrix
    /* 80069234 */  lui        $t6, %hi(D_80094CA8)
    /* 80069238 */  lw         $t6, %lo(D_80094CA8)($t6)
    /* 8006923C */  nop
    /* 80069240 */  slti       $at, $t6, 0x280
    /* 80069244 */  bnez       $at, .L80069270
    /* 80069248 */   lui       $at, %hi(D_80094C9C)
    /* 8006924C */  sw         $ra, %lo(D_80094C9C)($at)
    /* 80069250 */  lui        $a0, %hi(D_80094F2C)
    /* 80069254 */  jal        printf
    /* 80069258 */   addiu     $a0, $a0, %lo(D_80094F2C)
    /* 8006925C */  lui        $ra, %hi(D_80094C9C)
    /* 80069260 */  lw         $ra, %lo(D_80094C9C)($ra)
    /* 80069264 */  nop
    /* 80069268 */  jr         $ra
    /* 8006926C */   nop
  .L80069270:
    /* 80069270 */  lui        $t7, %hi(D_80094CAC)
    /* 80069274 */  addu       $t7, $t7, $t6
    /* 80069278 */  addiu      $t7, $t7, %lo(D_80094CAC)
    /* 8006927C */  cfc2       $t0, $0
    /* 80069280 */  cfc2       $t1, $1
    /* 80069284 */  sw         $t0, 0x0($t7)
    /* 80069288 */  sw         $t1, 0x4($t7)
    /* 8006928C */  cfc2       $t0, $2
    /* 80069290 */  cfc2       $t1, $3
    /* 80069294 */  sw         $t0, 0x8($t7)
    /* 80069298 */  sw         $t1, 0xC($t7)
    /* 8006929C */  cfc2       $t0, $4
    /* 800692A0 */  nop
    /* 800692A4 */  sw         $t0, 0x10($t7)
    /* 800692A8 */  cfc2       $t0, $5
    /* 800692AC */  cfc2       $t1, $6
    /* 800692B0 */  cfc2       $t2, $7
    /* 800692B4 */  sw         $t0, 0x14($t7)
    /* 800692B8 */  sw         $t1, 0x18($t7)
    /* 800692BC */  sw         $t2, 0x1C($t7)
    /* 800692C0 */  addi       $t6, $t6, 0x20
    /* 800692C4 */  lui        $at, %hi(D_80094CA8)
    /* 800692C8 */  sw         $t6, %lo(D_80094CA8)($at)
    /* 800692CC */  jr         $ra
    /* 800692D0 */   nop
endlabel PushMatrix

glabel PopMatrix
    /* 800692D4 */  lui        $t6, %hi(D_80094CA8)
    /* 800692D8 */  lw         $t6, %lo(D_80094CA8)($t6)
    /* 800692DC */  nop
    /* 800692E0 */  bgtz       $t6, .L8006930C
    /* 800692E4 */   lui       $at, %hi(D_80094C9C)
    /* 800692E8 */  sw         $ra, %lo(D_80094C9C)($at)
    /* 800692EC */  lui        $a0, %hi(D_80094F5D)
    /* 800692F0 */  jal        printf
    /* 800692F4 */   addiu     $a0, $a0, %lo(D_80094F5D)
    /* 800692F8 */  lui        $ra, %hi(D_80094C9C)
    /* 800692FC */  lw         $ra, %lo(D_80094C9C)($ra)
    /* 80069300 */  nop
    /* 80069304 */  jr         $ra
    /* 80069308 */   nop
  .L8006930C:
    /* 8006930C */  addi       $t6, $t6, -0x20
    /* 80069310 */  lui        $at, %hi(D_80094CA8)
    /* 80069314 */  sw         $t6, %lo(D_80094CA8)($at)
    /* 80069318 */  lui        $t7, %hi(D_80094CAC)
    /* 8006931C */  addu       $t7, $t7, $t6
    /* 80069320 */  addiu      $t7, $t7, %lo(D_80094CAC)
    /* 80069324 */  lw         $t0, 0x0($t7)
    /* 80069328 */  lw         $t1, 0x4($t7)
    /* 8006932C */  ctc2       $t0, $0
    /* 80069330 */  ctc2       $t1, $1
    /* 80069334 */  lw         $t0, 0x8($t7)
    /* 80069338 */  lw         $t1, 0xC($t7)
    /* 8006933C */  ctc2       $t0, $2
    /* 80069340 */  ctc2       $t1, $3
    /* 80069344 */  lw         $t0, 0x10($t7)
    /* 80069348 */  nop
    /* 8006934C */  ctc2       $t0, $4
    /* 80069350 */  nop
    /* 80069354 */  lw         $t0, 0x14($t7)
    /* 80069358 */  lw         $t1, 0x18($t7)
    /* 8006935C */  lw         $t2, 0x1C($t7)
    /* 80069360 */  ctc2       $t0, $5
    /* 80069364 */  ctc2       $t1, $6
    /* 80069368 */  ctc2       $t2, $7
    /* 8006936C */  jr         $ra
    /* 80069370 */   nop
endlabel PopMatrix

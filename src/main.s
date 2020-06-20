	.text
	.syntax unified
	.eabi_attribute	67, "2.09"	@ Tag_conformance
	.cpu	arm7tdmi
	.eabi_attribute	6, 2	@ Tag_CPU_arch
	.eabi_attribute	8, 1	@ Tag_ARM_ISA_use
	.eabi_attribute	9, 1	@ Tag_THUMB_ISA_use
	.eabi_attribute	34, 0	@ Tag_CPU_unaligned_access
	.eabi_attribute	15, 1	@ Tag_ABI_PCS_RW_data
	.eabi_attribute	16, 1	@ Tag_ABI_PCS_RO_data
	.eabi_attribute	17, 2	@ Tag_ABI_PCS_GOT_use
	.eabi_attribute	20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute	21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute	23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute	24, 1	@ Tag_ABI_align_needed
	.eabi_attribute	25, 1	@ Tag_ABI_align_preserved
	.eabi_attribute	38, 1	@ Tag_ABI_FP_16bit_format
	.eabi_attribute	18, 4	@ Tag_ABI_PCS_wchar_t
	.eabi_attribute	26, 2	@ Tag_ABI_enum_size
	.eabi_attribute	14, 0	@ Tag_ABI_PCS_R9_use
	.file	"main.c"
	.globl	dats_parse              @ -- Begin function dats_parse
	.p2align	2
	.type	dats_parse,%function
	.code	32                      @ @dats_parse
dats_parse:
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	.pad	#40
	sub	sp, sp, #40
	ldr	r0, .LCPI0_0
.LPC0_0:
	ldr	r0, [pc, r0]
	mov	r1, #0
	mov	r2, #2
	str	r1, [r11, #-12]         @ 4-byte Spill
	bl	fseek
	ldr	r1, .LCPI0_1
.LPC0_1:
	ldr	r1, [pc, r1]
	str	r0, [r11, #-16]         @ 4-byte Spill
	mov	r0, r1
	bl	ftell
	str	r0, [r11, #-4]
	ldr	r0, [r11, #-4]
	bl	malloc
	ldr	r1, .LCPI0_2
.LPC0_2:
	add	r1, pc, r1
	str	r0, [r1]
	ldr	r0, .LCPI0_3
.LPC0_3:
	ldr	r0, [pc, r0]
	ldr	r1, [r11, #-12]         @ 4-byte Reload
	ldr	r2, [r11, #-12]         @ 4-byte Reload
	bl	fseek
	ldr	r1, .LCPI0_4
.LPC0_4:
	ldr	r1, [pc, r1]
	ldr	r2, [r11, #-4]
	ldr	r3, .LCPI0_5
.LPC0_5:
	ldr	r3, [pc, r3]
	mov	r12, #1
	str	r0, [sp, #20]           @ 4-byte Spill
	mov	r0, r1
	mov	r1, r12
	bl	fread
	ldr	r1, .LCPI0_6
.LPC0_6:
	ldr	r1, [pc, r1]
	ldr	r2, .LCPI0_7
.LPC0_7:
	add	r2, pc, r2
	str	r0, [sp, #16]           @ 4-byte Spill
	mov	r0, r2
	bl	printf
	ldr	r1, [r11, #-12]         @ 4-byte Reload
	str	r1, [r11, #-8]
	b	.LBB0_1
.LBB0_1:                                @ =>This Loop Header: Depth=1
                                        @     Child Loop BB0_3 Depth 2
	ldr	r0, [r11, #-8]
	ldr	r1, [r11, #-4]
	cmp	r0, r1
	beq	.LBB0_6
	b	.LBB0_2
.LBB0_2:                                @   in Loop: Header=BB0_1 Depth=1
	b	.LBB0_3
.LBB0_3:                                @   Parent Loop BB0_1 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	ldr	r0, .LCPI0_8
.LPC0_8:
	ldr	r0, [pc, r0]
	ldrb	r0, [r0]
	cmp	r0, #10
	beq	.LBB0_5
	b	.LBB0_4
.LBB0_4:                                @   in Loop: Header=BB0_3 Depth=2
	ldr	r0, .LCPI0_9
.LPC0_9:
	add	r0, pc, r0
	ldr	r1, .LCPI0_10
.LPC0_10:
	ldr	r1, [pc, r1]
	ldrb	r1, [r1]
	str	r0, [sp, #12]           @ 4-byte Spill
	mov	r0, r1
	bl	putchar
	ldr	r1, .LCPI0_11
.LPC0_11:
	ldr	r1, [pc, r1]
	add	r1, r1, #1
	ldr	r2, [sp, #12]           @ 4-byte Reload
	str	r1, [r2]
	ldr	r1, [r11, #-8]
	add	r1, r1, #1
	str	r1, [r11, #-8]
	b	.LBB0_3
.LBB0_5:                                @   in Loop: Header=BB0_1 Depth=1
	ldr	r0, .LCPI0_12
.LPC0_12:
	add	r0, pc, r0
	ldr	r1, .LCPI0_13
.LPC0_13:
	ldr	r1, [pc, r1]
	add	r1, r1, #1
	str	r1, [r0]
	ldr	r0, [r11, #-8]
	add	r0, r0, #1
	str	r0, [r11, #-8]
	b	.LBB0_1
.LBB0_6:
	ldr	r0, .LCPI0_14
.LPC0_14:
	ldr	r0, [pc, r0]
	ldr	r1, [r11, #-8]
	sub	r1, r0, r1
	ldr	r0, .LCPI0_15
.LPC0_15:
	add	r0, pc, r0
	bl	printf
	ldr	r1, .LCPI0_16
.LPC0_16:
	ldr	r1, [pc, r1]
	ldr	r2, [r11, #-8]
	sub	r1, r1, r2
	str	r0, [sp, #8]            @ 4-byte Spill
	mov	r0, r1
	bl	free
	ldr	r0, .LCPI0_17
.LPC0_17:
	ldr	r0, [pc, r0]
	bl	fclose
	mov	r1, #0
	str	r0, [sp, #4]            @ 4-byte Spill
	mov	r0, r1
	mov	sp, r11
	pop	{r11, lr}
	bx	lr
	.p2align	2
@ %bb.7:
.LCPI0_0:
	.long	parse_in-(.LPC0_0+8)
.LCPI0_1:
	.long	parse_in-(.LPC0_1+8)
.LCPI0_2:
	.long	file_buffer-(.LPC0_2+8)
.LCPI0_3:
	.long	parse_in-(.LPC0_3+8)
.LCPI0_4:
	.long	file_buffer-(.LPC0_4+8)
.LCPI0_5:
	.long	parse_in-(.LPC0_5+8)
.LCPI0_6:
	.long	file_buffer-(.LPC0_6+8)
.LCPI0_7:
	.long	.L.str-(.LPC0_7+8)
.LCPI0_8:
	.long	file_buffer-(.LPC0_8+8)
.LCPI0_9:
	.long	file_buffer-(.LPC0_9+8)
.LCPI0_10:
	.long	file_buffer-(.LPC0_10+8)
.LCPI0_11:
	.long	file_buffer-(.LPC0_11+8)
.LCPI0_12:
	.long	file_buffer-(.LPC0_12+8)
.LCPI0_13:
	.long	file_buffer-(.LPC0_13+8)
.LCPI0_14:
	.long	file_buffer-(.LPC0_14+8)
.LCPI0_15:
	.long	.L.str.1-(.LPC0_15+8)
.LCPI0_16:
	.long	file_buffer-(.LPC0_16+8)
.LCPI0_17:
	.long	parse_in-(.LPC0_17+8)
.Lfunc_end0:
	.size	dats_parse, .Lfunc_end0-dats_parse
	.cantunwind
	.fnend
                                        @ -- End function
	.globl	main                    @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	32                      @ @main
main:
	.fnstart
@ %bb.0:
	.save	{r11, lr}
	push	{r11, lr}
	.setfp	r11, sp
	mov	r11, sp
	.pad	#16
	sub	sp, sp, #16
	mov	r2, #0
	str	r2, [r11, #-4]
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	ldr	r0, [sp, #8]
	cmp	r0, #1
	bgt	.LBB1_2
	b	.LBB1_1
.LBB1_1:
	ldr	r0, [sp, #4]
	ldr	r2, [r0]
	ldr	r0, .LCPI1_0
.LPC1_0:
	ldr	r0, [pc, r0]
	add	r0, r0, #168
	ldr	r1, .LCPI1_1
.LPC1_1:
	add	r1, pc, r1
	bl	fprintf
	mov	r1, #1
	str	r1, [r11, #-4]
	b	.LBB1_5
.LBB1_2:
	ldr	r0, [sp, #4]
	ldr	r0, [r0, #4]
	ldr	r1, .LCPI1_2
.LPC1_2:
	add	r1, pc, r1
	bl	fopen
	ldr	r1, .LCPI1_3
.LPC1_3:
	add	r1, pc, r1
	str	r0, [r1]
	ldr	r0, .LCPI1_4
.LPC1_4:
	ldr	r0, [pc, r0]
	cmp	r0, #0
	bne	.LBB1_4
	b	.LBB1_3
.LBB1_3:
	ldr	r0, [sp, #4]
	ldr	r0, [r0, #4]
	bl	perror
	mov	r0, #1
	str	r0, [r11, #-4]
	b	.LBB1_5
.LBB1_4:
	bl	dats_parse
	mov	r1, #0
	str	r1, [r11, #-4]
	b	.LBB1_5
.LBB1_5:
	ldr	r0, [r11, #-4]
	mov	sp, r11
	pop	{r11, lr}
	bx	lr
	.p2align	2
@ %bb.6:
.LCPI1_0:
.Ltmp0:
	.long	__sF(GOT_PREL)-((.LPC1_0+8)-.Ltmp0)
.LCPI1_1:
	.long	.L.str.2-(.LPC1_1+8)
.LCPI1_2:
	.long	.L.str.3-(.LPC1_2+8)
.LCPI1_3:
	.long	parse_in-(.LPC1_3+8)
.LCPI1_4:
	.long	parse_in-(.LPC1_4+8)
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cantunwind
	.fnend
                                        @ -- End function
	.type	WAV_BPM,%object         @ @WAV_BPM
	.data
	.globl	WAV_BPM
	.p2align	2
WAV_BPM:
	.long	120                     @ 0x78
	.size	WAV_BPM, 4

	.type	WAV_ALLOC,%object       @ @WAV_ALLOC
	.bss
	.globl	WAV_ALLOC
	.p2align	3
WAV_ALLOC:
	.long	0                       @ 0x0
	.long	0
	.size	WAV_ALLOC, 8

	.type	WAV_SAMPLE_RATE,%object @ @WAV_SAMPLE_RATE
	.data
	.globl	WAV_SAMPLE_RATE
	.p2align	2
WAV_SAMPLE_RATE:
	.long	44100                   @ 0xac44
	.size	WAV_SAMPLE_RATE, 4

	.type	NOTE_KEY,%object        @ @NOTE_KEY
	.globl	NOTE_KEY
	.p2align	3
NOTE_KEY:
	.ascii	"C1"
	.zero	6
	.long	2576980378              @ double 32.700000000000003
	.long	1077959065
	.ascii	"D1"
	.zero	6
	.long	1202590843              @ double 36.710000000000001
	.long	1078090465
	.ascii	"F1"
	.zero	6
	.long	2233382994              @ double 43.640000000000001
	.long	1078317547
	.size	NOTE_KEY, 48

	.type	parse_in,%object        @ @parse_in
	.bss
	.globl	parse_in
	.p2align	2
parse_in:
	.long	0
	.size	parse_in, 4

	.type	file_buffer,%object     @ @file_buffer
	.globl	file_buffer
	.p2align	2
file_buffer:
	.long	0
	.size	file_buffer, 4

	.type	.L.str,%object          @ @.str
	.section	.rodata.str1.1,"aMS",%progbits,1
.L.str:
	.asciz	"address file_buffer 1 %p\n"
	.size	.L.str, 26

	.type	.L.str.1,%object        @ @.str.1
.L.str.1:
	.asciz	"\nafter while %p\n"
	.size	.L.str.1, 17

	.type	.L.str.2,%object        @ @.str.2
.L.str.2:
	.asciz	"try '%s [filename]'\n"
	.size	.L.str.2, 21

	.type	.L.str.3,%object        @ @.str.3
.L.str.3:
	.asciz	"r"
	.size	.L.str.3, 2


	.ident	"clang version 9.0.0 (tags/RELEASE_900/final)"
	.section	".note.GNU-stack","",%progbits

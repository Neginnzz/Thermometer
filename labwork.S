

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.text

	 .global hexasc
 .global time2string
 .global delay
 .global enable_interrupt
delay:
add $t0, $0, $a0
nop
addi $t1, $0, 4711
addi $t2, $0, 0
nop
biggo:
addi $t3, $0, 0

ble $t0, $t2, end
nop
addi $t2, $t2, 1

smol:
addi $t3, $t3, 1

ble $t1, $t3, biggo
nop
j smol
nop
end:
jr $ra

nop

  # you can write your code for subroutine "hexasc" below this line
  #
  

   hexasc:
add $v0, $t4, $0
andi $v0, $v0, 0xF
addiu $t0, $0, 0x09
ble $v0, $t0, jumpback 
nop
addiu $v0, $v0, 0x040
addi $t1, $0, 0x9
sub $v0, $v0, $t1
jr $ra  
 nop
 jumpback:

addiu $v0, $v0, 0x30 
jr $ra
nop


time2string: #hiyaaaaaa
add $t4, $0, $a1
PUSH $ra
PUSH $t0
PUSH $a0
add $a0, $0, $t4 
jal hexasc
nop
POP $a0
sb $v0, 4($a0)
PUSH $a0
srl $t4, $t4, 4

add $a0, $0, $t4 
jal hexasc
nop
POP $a0
sb $v0, 3($a0)


addi $v0, $0, 0x3A
sb $v0, 2($a0)
nop
srl $t4, $t4, 4
PUSH $a0
add $a0, $0, $t4 
jal hexasc
nop
POP $a0
sb $v0, 1($a0)

srl $t4, $t4, 4
PUSH $a0
add $a0, $0, $t4 
jal hexasc
nop
POP $a0
sb $v0, 0($a0)

sb $0, 5($a0)

POP $t0
POP $ra
nop

jr $ra
nop


enable_interrupt:
ei
j $ra

fine:
nop

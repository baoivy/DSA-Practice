.data
	
.text
	li $v0 5
	syscall
	move $s0 $v0 #a
	li $v0 5
	syscall 
	move $s1 $v0 #b
	li $v0 5
	syscall
	move $s2 $v0 #c
	sub $s0 $s0 $s1
	sub $a0 $s0 $s2
	li $v0 1
	syscall

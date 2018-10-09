.model  small
include lib\cga_init.asm
include lib\paint.asm

.data

.const
	X_COORD			equ		110
	Y_COORD			equ		10
	Y2_COORD		equ		120
	
.code

main:
	mov ax,@data
	mov ds,ax

	call put_cga5          		

	mov ax, CGA_COLOR_CYAN 			;���������� ����
	mov PX_COLOR, ax
	
	mov bx, X_COORD					;x
	mov ax, Y_COORD 				;y1
	mov dx, Y2_COORD 				;y2
	
	call vertical_line
	
	xor ax,ax			;�������� ������� �������
	int 16h

	mov ax,4c00h		;����� �� ������� � ���������
	int 21h				;� ���������� �����

end main
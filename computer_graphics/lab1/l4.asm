.model  small
include lib\cga_init.asm
include lib\paint.asm

.data
	X_COORD			dw		111
	X2_COORD		dw		259
	Y_COORD			equ		110
	
.code
main:
	mov ax,@data
	mov ds,ax

	call put_cga5          		;
	
	mov ax, CGA_COLOR_RED 			;установить цвет
	mov PX_COLOR, ax
	
	mov bx, X_COORD 			;x1
	mov ax, X_COORD				;y
	mov dx, X2_COORD 			;x2
	
	call gorizontal_line	

	xor ax,ax			;ожидание нажатия клавиши
	int 16h

	mov ax,4c00h		;выход из графики с возвратом
	int 21h				;в предыдущий режим

end main
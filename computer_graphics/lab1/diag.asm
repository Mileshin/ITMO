.model  small
.286

include lib\cga_init.asm
include lib\paint.asm

.data
	
.code

main:
	mov ax,@data
	mov ds,ax

	call put_cga5        

;влево-вправо-вниз
	mov ax, 70	;y0 
	mov bx, 200	;x0

	mov dx, 30	;y1
	mov cx, 0	;x1

	call inclined_line	 

;лево-право-вверх
	mov ax, 70	;y0
	mov bx, 30	;x0

	mov dx, 30	;y1
	mov cx, 150	;x1

	call inclined_line	
	

	xor ax,ax			
	int 16h

	mov ax,4c00h	
	int 21h			

end main
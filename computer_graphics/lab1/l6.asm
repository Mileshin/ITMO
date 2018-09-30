.model small
.stack 500h

include lib\cga_init.asm
include lib\paint.asm

.code
main:

	mov ax, @data
	mov ds, ax
	
	call put_cga5 
    
	mov ax, CGA_COLOR_GRAY 			;установить цвет
	mov PX_COLOR, ax	
	
	mov x_center, 100
	mov y_center, 70
	mov r, 40
	call circle
	
	mov bx, 140
	mov ax, 30
	mov cx, 50
	mov dx, 130
	
	call inclined_line
	
	mov ax, 70	;y0 
	mov bx, 200	;x0

	mov dx, 30	;y1
	mov cx, 60	;x1
	
	call inclined_line
	
	mov ax, CGA_COLOR_RED 			;установить цвет
	mov PX_COLOR, ax
	
	mov ax, 120
	mov bx, 60
	call filling
	
	mov ax, CGA_COLOR_GRAY			;установить цвет
	mov PX_COLOR, ax
	
	mov ax, 100
	mov bx, 35
	call filling
	
	mov ax, CGA_COLOR_CYAN			;установить цвет
	mov PX_COLOR, ax
	mov ax, 80
	mov bx, 70
	call filling


	xor ax, ax              ; ждем нажатия клавиши
	int 16h
	mov ax,4c00h            ; выходим из программы
	int 21h

end main
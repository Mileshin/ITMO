;3) Разработать программу для этого же видеоадаптера для вывода на экран
;точки с координатами x = 93, y = 110, красного цвета. Значения цвета
;и координат можно задавать непосредственно в тексте программы.
.model  small

include lib\cga_init.asm
include lib\paint.asm
	
	
.const
	X_COORD		equ		93
	Y_COORD		equ		110
   
.code

main:
	mov ax,@data
	mov ds,ax

	call put_cga5					;установка режима cga
	
	mov ax, CGA_COLOR_RED 			;установить цвет
	mov PX_COLOR, ax
	
	mov bx, X_COORD					;x
	mov ax, Y_COORD					;y
	
	call put_pixel 
      
	xor ax,ax						;ожидание нажатия клавиши
	int 16h

	mov ax,4c00h					;выход из графики с возвратом
	int 21h							;в предыдущий режим

end main
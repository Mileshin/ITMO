.const
	CGA_COLOR_CYAN		equ		1
	CGA_COLOR_MAGENTA	equ		2
	CGA_COLOR_GRAY		equ		3
	CGA_COLOR_RED		equ		2
	
.data
	bitmask10 	db 128, 32, 8, 2		; красный 10
	bitmask01 	db 64, 16, 4, 1			;	синий 01
	bitmask11 	db 192, 48, 12, 3		;	серый 1
	bitmaskred	db 170, 42, 10, 2, 128, 160, 168, 170
	bitmaskcyan	db 85, 21, 5, 1, 64, 80, 84, 85
	bitmaskgrey	db 255, 63, 15, 3, 192, 240, 252, 255
	
	PX_COLOR	dw		CGA_COLOR_RED
	
	x_center dw ? 
	y_center dw ?
	x dw ?
	y dw ?
	r dw ?
	
	x_temp dw ?
	y_temp dw ?

	ignore dw ?                   ; переменная 0 - игнорировать пиксель  1 - пиксель поместить в стек


.code

; ------ процедура рассчитывает отступ для пикселя ------
; bx - X COORD
; ax - Y COORD
put_offset proc 
	mov dx, bx
	and dx, 03h						;отсекаем 2 последних бита
	mov cl, 2
	shr bx, cl 						;делим x на 4, тк в байте 4 пикселя, по 2 бита на пиксель

	mov cx, ax
	and cx, 01h						;проверка на четность
	cmp cx, 0						;строки
	jz EV
	add bx, 2000h

EV: 
	mov cl, 1						;ax = (y-1)/2 or y/2
	shr ax, cl 						;т.к. банк только четных/нечетных строк
	;z*80 = z*64 + z*16 - т.к. 4 пикселя в байте, то в строке - 80 байт
	mov cl, 4
	sal ax, cl 						;ax=(y/2)*2^4, bx=x/4
	add bx, ax
	mov cl, 2
	sal ax, cl 						;ax=(y/2)*2^6, bx=x/4 + (y/2)*2^4
	add bx, ax						;bx=x/4+(y/2)*80 - полных байт

	xor ax,ax
	mov si,dx
	ret 
put_offset endp

; ------ процедура выводит пиксель с заданными координатами ------
; PX_COLOR - color pixel
; bx - X COORD
; ax - Y COORD
; es - offset
put_pixel proc uses ax bx cx si di dx

	call put_offset
	
	mov cx, PX_COLOR
	cmp cx, 3
	jz	C3
	cmp cx, 2
	jz	C2
	cmp cx, 1
	jz	C1
	ret
	
C3:
	mov al,bitmask11+[si]  			; в al - маска 
	jmp PUT
C2:
	mov al,bitmask10+[si]  			; в al - маска 
	jmp PUT
C1:
	mov al,bitmask01+[si]  			; в al - маска 
	
PUT:
	or es:[bx],ax     				;выводим точку  

	ret
put_pixel endp

; ------ процедура рисует горизонтальную линию с заданными координатами ------
; PX_COLOR - color pixel
; bx - X COORD
; ax - Y COORD
; dx - X2 COORD
; es - offset
gorizontal_line proc uses ax bx cx dx si 
	cmp bx, dx
	jle POS
	xchg bx, dx
	
POS:
	push bx
	push dx
	
	call put_offset
	
	mov cx, PX_COLOR
	cmp cx, 3
	jz	C3
	cmp cx, 2
	jz	C2
	cmp cx, 1
	jz	C1
	ret
	
C3:
	mov al,bitmaskgrey+[si]  			; в al - маска 
	jmp PUT
C2:
	mov al,bitmaskred+[si]  			; в al - маска 
	jmp PUT
C1:
	mov al,bitmaskcyan+[si]  			; в al - маска 
	
PUT:	
	or es:[bx],ax     				
	
	mov ax, dx
	pop dx 							;x2
	pop cx 							;x1
	sub cx, ax						;убираем уже отрисованне точки 
	sub dx, cx 						;x2-x1
	mov cx, dx 						
	and cx, 03h						;количество точек в последнем байте
	push cx

	
	mov cl, 2
	shr dx, cl						;bx = x2/4
	dec dx							;последний байт обработаем последним
							
	mov cx, dx
	
	mov dx, PX_COLOR
	cmp dx, 3
	jz	C32
	cmp dx, 2
	jz	C22
	cmp dx, 1
	jz	C12
	ret
C32:
	mov al,bitmaskgrey 			; в al - маска 
	jmp PUT2
C22:
	mov al,bitmaskred  			; в al - маска 
	jmp PUT2
C12:
	mov al,bitmaskcyan 			; в al - маска 
PUT2:	
	inc bx
 	
line_middle:	
	or es:[bx],ax
      inc bx
	loop line_middle

	pop dx
	cmp dx, 0						;проверяем, нужно ли отдельно отрисовать последний бит
	jnz end_line
	mov dx, 4
end_line:
	xor ax,ax						;отрисовываем последний байт отдельно
	mov si, dx
	add si, 3
	
	
	mov cx, PX_COLOR
	cmp cx, 3
	jz	C33
	cmp cx, 2
	jz	C23
	cmp cx, 1
	jz	C13
	ret
	
C33:
	mov al,bitmaskgrey+[si]  			; в al - маска 
	jmp PUT3
C23:
	mov al,bitmaskred+[si]  			; в al - маска 
	jmp PUT3
C13:
	mov al,bitmaskcyan+[si]  			; в al - маска 
PUT3:	
	or es:[bx],ax 
	ret
gorizontal_line endp

; ------ процедура рисует вертикальную линию с заданными координатами ------
; PX_COLOR - color pixel
; bx - X COORD
; ax - Y COORD
; dx - Y2 COORD
; es - offset
vertical_line proc uses ax bx cx si 
	push ax
	push dx
	
	call put_offset
	
	mov cx, PX_COLOR
	cmp cx, 3
	jz	C3
	cmp cx, 2
	jz	C2
	cmp cx, 1
	jz	C1
	ret
	
C3:
	mov al,bitmask11+[si]  			; в al - маска 
	jmp PUT
C2:
	mov al,bitmask10+[si]  			; в al - маска 
	jmp PUT
C1:
	mov al,bitmask01+[si]  			; в al - маска 
	
PUT:

	or es:[bx],ax     				;выводим точку     

	pop cx 							;y2
	pop dx 							;y1
	sub cx, dx
	shr cx, 1
	push cx
	
L1:
	add bx, 50h			; перевод на через строку
	or es:[bx],ax
	loop L1
	
	pop cx
	cmp bx, 2000h
	jge CORR
	add bx, 2000h
	inc cx
	jmp L2

CORR:	
	sub bx, 2000h

L2:
	or es:[bx],ax
	sub bx, 50h			; перевод на через строку
	loop L2

	ret
vertical_line endp

; ------ процедура рисует наклонную линию с заданными координатами ------
; PX_COLOR - color pixel
; bx - X COORD
; ax - Y COORD
; dx - Y2 COORD
; cx - X2 COORD
; es - offset
inclined_line proc uses ax dx si cx dx di
	
	cmp ax, dx
	jl OK1		;y1 > y0, ok
	mov si, 01b 	;keybit: y0 > y1!  

OK1:
	cmp bx, cx		
	jl OK2		;x1 > x0, ok
	mov di, 01b		;keybit: x0 > x1!

ok2:
	cmp si, di		; si = di = 0 or si = di = 1
	je OK3
	cmp si, 01b		;one of them is 1
	je YCORR		;y0 > y1, x0 < x1, коррекци¤ необходима

	jmp LRU		;else: y0 < y1, x0 > x1, коррекци¤ не нужна

YCORR:			;y0 > y1, x0 < x1
	xchg ax, dx
	xchg bx, cx
	jmp LRU		;y0 < y1, x0 > x1
 
Ok3:				;рисуем влево-вправо-вниз
	cmp si, 01b		;if si = 1 (or di = 1), do xchg
	jne LRD
	xchg ax, dx
	xchg bx, cx

LRD:			;влево-вправо-вниз
	push ax
	push bx 

	sub cx, bx 	;cx = dx
	sub dx, ax 	;dx = dy


	mov si, cx	;si = dx
	mov di, dx	;di = dy

	pop bx
	pop ax
	xor dx, dx

L1:
	push cx
	call put_pixel
	inc bx 		;inc x

	add dx, di	;dx(error) = dx + dy
	mov cx, dx
	shl cx, 1	;2*error

	cmp cx, si
	jle A1
	inc ax		;inc y
	sub dx, si

A1:
	pop cx

	loop L1
	ret

LRU:			;y0<y1, x0>x1: лево-право-вверх
	push ax
	push bx 

	sub bx, cx 	;bx = dx, bx > cx
	sub dx, ax 	;dx = dy


	mov si, bx	;si = dx
	mov di, dx	;di = dy

	mov cx, bx

	pop bx
	pop ax
	xor dx, dx

L2:
	push cx
	call put_pixel
	dec bx 		;dec x

	add dx, di	;dx(error) = dx + dy
	mov cx, dx
	shl cx, 1

	cmp cx, si
	jle A2
	inc ax		;inc y
	sub dx, si

A2:
	pop cx
	loop L2
	ret

inclined_line endp 

; ------ процедура рассчитывает точки окружности ------
;x_center 	- center point abscissa
;y_center 	- center point ordinate
;r			- radius
circle proc	uses ax bx cx dx di si
	mov x, 0                ; начальное значение х
	mov ax, r
	mov y, ax               ; начальное значение у
	shl ax, 1
	mov dx, 3
	sub dx, ax              ; вычислено начальное значение управляющей переменной d0 = 3-2*r


q:  cmp dx, 0			; сравниваем d с нулем
	jl s		        ; если меньше нуля, то это точка s
t:  mov ax, x	        ; точка t. вычисляем новое d
	sub ax, y	        ; как x-y
	shl ax, 1			; умноженное 
	shl ax, 1			; на 4
	add ax, 10	        ; плюс 10
	add dx, ax	        ; плюс старое значение d
	dec y				; уменьшаем y
	jmp dspl
s:  mov ax, x	        ; точка s. вычисляем новое d как х
	shl ax, 1			; умноженное 
	shl ax, 1			; на 4
	add ax, 6			; плюс 6
	add dx, ax	        ; плюс старое значение d

; ------ x и y определены ------
dspl: 
	;1 часть
	mov bx, x
	mov ax, y
	add bx, x_center
	sub ax, y_center
	neg ax
	call put_pixel
	;2 часть
	mov ax, x
	mov bx, y
	add bx, x_center
	sub ax, y_center
	neg ax
	call put_pixel
	;3 часть
	mov ax, x
	mov bx, y
	add bx, x_center
	add ax, y_center
	call put_pixel
	;4 часть
	mov bx, x
	mov ax, y
	add bx, x_center
	add ax, y_center
	call put_pixel
	;5 часть
	mov bx, x
	mov ax, y
	sub bx, x_center
	neg bx
	add ax, y_center
	call put_pixel
	;6 часть  
	mov ax, x
	mov bx, y
	sub bx, x_center
	neg bx
	add ax, y_center
	call put_pixel
	;7 часть
	mov ax, x
	mov bx, y
	sub bx, x_center
	neg bx
	sub ax, y_center
	neg ax
	call put_pixel
	;8 часть
	mov bx, x
	mov ax, y
	sub bx, x_center
	neg bx
	sub ax, y_center
	neg ax
	call put_pixel 
	
	inc x
	mov bx, x
	mov cx, y
	cmp bx, cx      		;  пока х < у цикл повторяется
	jb q

	ret
circle endp

; ------ процедура проверяет, нужно игнорировать пиксель или закрашивать ------
;x_temp
;y_temp
color_check proc uses ax bx cx dx di si
	mov ignore, 0
	mov di,0                
	mov ax, y_temp          
	shr ax, 1               
	jnc ev             
	add di, 2000h           
ev:
	mov cl, 4
	shl ax, cl             
	add di, ax
	mov cl, 2
	shl ax, cl              
	add di, ax              ; учтена игрековая часть адреса             

	mov ax, x_temp
	shr ax, cl              
	add di, ax              ; итоговый адрес нужного байта в видеопамяти

	mov bl, byte ptr es:[di]; состояние нужного байта в видеопамяти
	
	mov dx, x_temp          ; текущее значение х
	and dx, 03h               ; получен остаток x/8, на него надо сдвигать
	mov si, dx
	mov cx, PX_COLOR
	cmp cx, 3
	jz	C3
	cmp cx, 2
	jz	C2
	cmp cx, 1
	jz	C1
	ret
	
C3:
	mov al,bitmask11+[si]  			; в al - маска 
	jmp PUT
C2:
	mov al,bitmask10+[si]  			; в al - маска 
	jmp PUT
C1:
	mov al,bitmask01+[si]  			; в al - маска 
	
PUT:
	and bl, al              ; очистка всех бит кроме нужных двух

	cmp bl, 0
	jne go2end
	mov ignore, 1      
go2end:
	ret
color_check endp

; ------ процедура выполняет заливку ------
;ax	-	x seed pixel
;bx	-	y seed pixel
filling proc uses ax bx cx dx si di 
	mov si, sp              ; адрес когда стек пустой

	; 1. затравочный пиксель помещается в стек
	push ax                 
	push bx                 

fill:
	; 2. пиксель извлекается из стека
	pop bx                  ; y
	pop ax                  ; x
	mov x_temp, ax
	mov y_temp, bx
	; 3. пиксель закрашивается.
	mov ax, y_temp
	mov bx, x_temp
	call put_pixel

	; 4. для каждого из соседних пикселей: если пиксель является закрашенным
	;   или граничным, пиксель игнорируется, иначе пиксель помещается в стек
	; порядок обхода справа, слева, снизу, сверху
	mov ax, x_temp
	inc ax
	mov x_temp, ax          ; координата х + 1
	call color_check
	cmp ignore, 1
	jne next1
	mov ax, x_temp          ; x
	mov bx, y_temp          ; y
	push ax                 ; снизу х
	push bx                 ; сверху у

next1:
	mov ax, x_temp
	sub ax, 2
	mov x_temp, ax          ; координата х - 1
	call color_check
	cmp ignore, 1
	jne next2
	mov ax, x_temp          ; x
	mov bx, y_temp          ; y
	push ax                 ; снизу х
	push bx                 ; сверху у

next2:
	mov ax, x_temp
	inc ax
	mov x_temp, ax          ; обратно координата х

	mov ax, y_temp
	inc ax
	mov y_temp, ax          ; координата y + 1
	call color_check
	cmp ignore, 1
	jne next3
	mov ax, x_temp          ; x
	mov bx, y_temp          ; y
	push ax                 ; снизу х
	push bx                 ; сверху у

next3:
	mov ax, y_temp
	sub ax, 2
	mov y_temp, ax          ; координата y - 1
	call color_check
	cmp ignore, 1
	jne next4
	mov ax, x_temp          ; x
	mov bx, y_temp          ; y
	push ax                 ; снизу х
	push bx                 ; сверху у

next4:
	cmp si, sp
	jne fill
	ret
filling endp 
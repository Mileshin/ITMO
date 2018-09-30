.const
	CGA_COLOR_CYAN		equ		1
	CGA_COLOR_MAGENTA	equ		2
	CGA_COLOR_GRAY		equ		3
	CGA_COLOR_RED		equ		2
	
.data
	bitmask10 	db 128, 32, 8, 2		; красный 10
	bitmask01 	db 64, 16, 4, 1			;	синий 01
	bitmask11 	db 192, 48, 12, 3		;	серый 1
	bitmaskbig	db 
	PX_COLOR	dw		CGA_COLOR_RED

.code
put_pixel proc uses ax bx cx si 
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
	jmp PUT
	
PUT:
	mov es:[bx],ax     				;выводим точку  

	ret
put_pixel endp
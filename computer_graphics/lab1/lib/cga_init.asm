.const
	CGA_VMEM_SEG	equ		0B800h

.code

; the offset will be recorded in the es register
;mode 4
put_cga4 proc 
	mov  ax,04h       				;инициализация графического
	int  10h						;режима CGA: 320x200

	mov ax, CGA_VMEM_SEG      		;указываем на видеобуфер
	mov es,ax          				;
	ret
put_cga4 endp

;mode 5
put_cga5 proc 
	mov  ax,05h       				;инициализация графического
	int  10h						;режима CGA: 320x200

	mov ax, CGA_VMEM_SEG      		;указываем на видеобуфер
	mov es,ax          				;
	ret
put_cga5 endp

;mode 6
put_cga6 proc 
	mov  ax,06h       				;инициализация графического
	int  10h						;режима CGA: 320x200

	mov ax, CGA_VMEM_SEG      		;указываем на видеобуфер
	mov es,ax          				;
	ret
put_cga6 endp



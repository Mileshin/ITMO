;3) ����������� ��������� ��� ����� �� ������������� ��� ������ �� �����
;����� � ������������ x = 93, y = 110, �������� �����. �������� �����
;� ��������� ����� �������� ��������������� � ������ ���������.
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

	call put_cga5					;��������� ������ cga
	
	mov ax, CGA_COLOR_RED 			;���������� ����
	mov PX_COLOR, ax
	
	mov bx, X_COORD					;x
	mov ax, Y_COORD					;y
	
	call put_pixel 
      
	xor ax,ax						;�������� ������� �������
	int 16h

	mov ax,4c00h					;����� �� ������� � ���������
	int 21h							;� ���������� �����

end main
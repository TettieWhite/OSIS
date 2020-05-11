.model tiny
.code
.startup
.386
jmp real_start 
    installed dw 7788
    old_int9h_offset dw ?
    old_int9h_segment dw ?
	flagD db 0

new_int9h proc far 
    pusha
    push es
    push ds
    push cs
    pop ds
    pushf

	cmp ax, 65100
	jne notTry
	
	mov ax, 65000
	jmp quit

notTry:
	
    mov bx,0
    mov dx,0
    
    call dword ptr cs:[old_int9h_offset]
    mov ax, 40h
    mov es, ax
    mov bx, es:[1ch] ;указатель на хвост => после последнего
    cmp bl, 1Eh
    jne continue
	mov dl, 1
    mov bl,	3Eh
continue:
    sub bl, 2

	mov ax,es:[bx]
	mov ah,0
	cmp al, 'b'
	je skip
	cmp al, 'c'
	je skip
	cmp al, 'd'
	je skip
	cmp al, 'f'
	je skip
	cmp al, 'g'
	je skip
	cmp al, 'h'
	je skip
	cmp al, 'j'
	je skip
	cmp al, 'k'
	je skip
	cmp al, 'l'
	je skip
	cmp al, 'm'
	je skip
	cmp al, 'n'
	je skip
	cmp al, 'p'
	je skip
	cmp al, 'q'
	je skip
	cmp al, 'r'
	je skip
	cmp al, 's'
	je skip
	cmp al, 't'
	je skip
	cmp al, 'v'
	je skip
	cmp al, 'w'
	je skip
	cmp al, 'x'
	je skip
	cmp al, 'z'
	je skip
	cmp al, 'B'
	je skip
	cmp al, 'C'
	je skip
	cmp al, 'D'
	je skip
	cmp al, 'F'
	je skip
	cmp al, 'G'
	je skip
	cmp al, 'H'
	je skip
	cmp al, 'J'
	je skip
	cmp al, 'K'
	je skip
	cmp al, 'L'
	je skip
	cmp al, 'M'
	je skip
	cmp al, 'N'
	je skip
	cmp al, 'P'
	je skip
	cmp al, 'Q'
	je skip
	cmp al, 'R'
	je skip
	cmp al, 'S'
	je skip
	cmp al, 'T'
	je skip
	cmp al, 'V'
	je skip
	cmp al, 'W'
	je skip
	cmp al, 'X'
	je skip
	cmp al, 'Z'
	je skip
	
    jmp quit
skip:
	mov al, 0
    mov es:[bx],ax
    jmp quit
notBig:
    
quit:
	pop ds
    pop es
    popa
    iret
new_int9h endp  

real_start:      
	mov si,80h
	mov ch, 0
    lodsb
	mov ah, al
    lodsb
	inc ch
	jmp firstLabel

maybeParamError2:

	maybeParamError2Label:
		cmp al, " "
		jne maybeParamError
		lodsb	
		jmp maybeParamError2Label

maybeParamError:
	cmp ch, ah
	jg thirdLabel
paramError:
	mov dx, offset paramErrorStr
    mov ah, 9
    int 21h
	jmp exit

firstLabel:
	cmp al, " "
	jne secondLabel
	lodsb
	inc ch
	jmp firstLabel

secondLabel:
	cmp al, '-'
	jne maybeParamError
	lodsb
	inc ch
	cmp al, 'a'
	jne paramError
	cmp ch, ah
	jne maybeParamError2
	mov al, 1
	mov flagD, al

thirdLabel:      
    mov ax,3509h                    
    int 21h                   
    cmp word ptr es:installed, 7788  ; проверка того, загружена ли уже программа
    je alreadyInstalled                   

	cmp flagD, 1
	jne install 
	
notInstalled:
	mov dx, offset errorWrongParam     
    mov ah, 9
    int 21h
	jmp exit

alreadyInstalled:
	cmp flagD, 1
	je remove
	mov dx, offset alreadyInstalledText     
    mov ah, 9
    int 21h
	jmp exit

install:
    mov cs:old_int9h_offset, bx         
    mov cs:old_int9h_segment, es        
    mov ax, 2509h                   
    mov dx, offset new_int9h            
    int 21h
    mov dx, offset ok_installed         
    mov ah, 9
    int 21h
    mov dx, offset real_start       
    int 27h  
    
remove:                             
    push es
    push ds
    mov dx, es:old_int9h_offset         
    mov ds, es:old_int9h_segment        
    mov ax, 2509h
    int 21h
    pop ds
    pop es
    mov ah, 49h                     
    int 21h
    jc not_remove                   
    mov dx, offset removed_msg     
    mov ah, 9
    int 21h
    jmp exit                        

	not_remove:                        
		mov dx, offset noremove_msg                     
		mov ah, 9
		int 21h
		jmp exit
exit:                              
    int 20h

errorWrongParam db "COM isn't installed yet$"
alreadyInstalledText db "COM is already installed$"
paramErrorStr db "Error with param$"
ok_installed db 'Installed$'
removed_msg db 'Uninstalled$'
noremove_msg db 'Cannot unload program$'
end
assume cs:code,ds:data

data segment
	db 80 dup ('-') ;定义棋盘              ;展现棋盘状态，.为无落子，x为玩家1，o为玩家2。  
	dw 800 dup ('.');40个字*20列 
	db 80 dup ('-')
	dw 0
	db 'Use the "wsad" to move the cursor, Confirm with a "space" at the point.       ';引导语(上下左右移动光标，空格落子，esc退出)。
	db '  cursor position: 10 21. Press "r" to regret chess. Press "esc" to exit.       '          
	db '  Now is  player 1:o.';落子方（判断落子）,子:(1为o,2为x).
	dw 0
	dw 0				;落子数(一个字)
	db 0,2,10,40,0,10,40;胜负方(put:1胜2胜3和棋)/胜负状态（1跳出循环，2继续）/dh,dl(落子行列号)/
	;（悔棋许可0禁止1允许）/悔棋行，悔棋列
	dw 0
	db 'Use the "wsad" to move the cursor, Confirm with a "space" at the point.       ';重新开始游戏时更改引导语

	dw 0
	db 'Winner is player 1, press "esc" to exit the game. Press "r" to start over.  ';结束引导语，用于替换
	dw 0
	
	dw 0
	db 'Winner is player 2, press "esc" to exit the game. Press "r" to start over.  '
	dw 0
	
	dw 0
	db 'The result is a draw. Press "esc" to exit the game. Press "r" to start over '
	dw 0
	db 0,1,2,3	;左判定，右判定，上判定，下判定
data ends

;一些地址:[743-744H]行提示号/[746-747H]列提示号
;[791H]玩家/[797H]落子数/[799H]胜负方/[79AH]胜负状态/[79BH]光标行/[79CH]光标列
;[79DH]悔棋许可/[79EH]悔棋行/[79FH]悔棋列/[7A0H]重开引导语替换/[7F0H]玩家1胜/[840H]玩家2/[890H]和
;[8E0H]左判定，[8E1H]右判定，[8E2H]上判定，[8E3H]下判定

code segment
start:    
	call show				;清屏并初始化输出棋盘
starts:                     ;只改动引导语。
	call shows				;输出引导语
	call scanf				;输入操作

	cmp ax,0FFFFH           ;防止call后不ret导致栈溢出
	je pass

	call judge				;判断是否胜利或和棋
	call mainswap			;交换落子方
	call show				;清屏并初始化输出棋盘
pass:
	mov ch,0
	mov cl,ds:[79AH]       	;获得胜负状态，胜利后跳出loop
	loop starts

	call put    	   	    ;更改引导语
	call mainswap			;再交换落子方确保颜色输出正确
	call show				;清屏输出最终结果
	call shows				;输出引导语
	call restart			;重开或者结束
	jmp start
nojudge:
	mov ax,0FFFFH
	ret

;——————————————————————————————————————清屏（主函数）
clear:
	mov ax,3H	      ;调用10H的3号中断
	int 10H
	ret
;——————————————————————————————————————输出棋盘（主函数）
show:
	call clear				;清屏
	mov dh,0		;设置输出属性
	mov dl,0
	mov cl,7
	mov ax,data
	mov ds,ax
	mov si,0
	
	mov ax,0b800H	;以此输出数据段应该输出的数据	
	mov es,ax		
	mov al,00a0H		
 	mul dh			
 	push ax				
 	mov al,2			
 	mul dl				
 	pop di				
 	add di,ax

	input:
		mov bh,0		;不同方落子输出不同颜色提示语
		mov bl,ds:[si]
		cmp bl,111
		je green
		cmp bl,120
		je red
		mov cl,7
	backinput:
		push cx				
		mov cx,ds:[si]		
 		jcxz end_input		
 		mov es:[di],cx			
 		inc si				
 		inc di					
 		pop cx
 		mov es:[di],cl			
 		inc di
 		jmp input		

	end_input:
		pop cx					
		ret
green:
	mov cl,2
	jmp backinput
red:
	mov cl,4
	jmp backinput
;①获得数据段数据来输出棋盘状态 ret
;——————————————————————————————————————输出引导语（主函数）
shows:
	mov dh,22		;设置输出属性
	mov dl,2
	mov ax,data
	mov ds,ax
	mov si,6E2H
	
	mov ax,0b800H	;以此输出数据段应该输出的数据	
	mov es,ax		
	mov al,00a0H		
 	mul dh			
 	push ax				
 	mov al,2			
 	mul dl				
 	pop di				
 	add di,ax

	mov bl,ds:[793H]
	cmp bl,111
	je greens
	cmp bl,120
	je reds
	inputs:
		push cx				
		mov cx,ds:[si]		
 		jcxz end_inputs		
 		mov es:[di],cx			
 		inc si				
 		inc di					
 		pop cx
 		mov es:[di],cl			
 		inc di
 		jmp inputs		

	end_inputs:
		pop cx					
		ret

greens:
	mov cl,2
	jmp inputs
reds:
	mov cl,4
	jmp inputs

;①获得数据段数据来输出棋盘状态 ret
;——————————————————————————————————————落子（主函数）;①进入中断，获得输入的字符
scanf:
	mov ah,0cH			;清除键盘缓存
	mov al,0
	int 21H

	mov ah,2        ;初始化光标位置
	mov al,0
	mov bh,0
	mov dh,ds:[79BH]     ;对应地址存行号
	mov dl,ds:[79CH]     ;对应地址存列号
	int 10H
scan:           	;移动光标
	mov cx,5
	mov ah,0H				;等待数据输入
	int 16H

	cmp al,119			;接受输入数据进行比较 al存数据
	je up
	cmp al,115
	je down
	cmp al,97
	je left
	cmp al,100
	je right
	cmp al,27
	je exit
	cmp al,114
	je regret0	
	cmp al,32
	je enter1

	cursor:                  ;判断越界并输出光标位置
		cmp dh,0
		je backup1
		cmp dh,21
		je backdown1
		cmp dl,-2
		je backleft1
		cmp dl,78
		ja backright1

		push dx		;入栈保护数据
		mov dl,dh	;行号转成ascii码放入行提示号中
		mov dh,0
		mov ax,dx
		mov dl,10
		div dl
		add al,48
		mov ds:[743H],al
		add ah,48
		mov ds:[744H],ah
		pop dx
	
		push dx		;入栈保护数据
		mov dh,0	;列号转成ascii码放入列提示号中
		mov ax,dx
		mov dh,2	;列号是双倍的 因为中间有空格
		div dh
		mov ah,0
		inc ax		;列从零开始
		mov dl,10
		div dl
		add al,48
		mov ds:[746H],al
		add ah,48
		mov ds:[747H],ah
		pop dx

	mov ax,0FFFFH 
	mov byte ptr ds:[79BH],dh	;存回行号
	mov byte ptr ds:[79CH],dl   ;存回列号
	ret
;————————————————————————
regret0:
	jmp regret		;中继
;———————————————————————— ;对上下左右 退出 落子判断的计算
	up:                   
		sub dh,1
		jmp cursor   
 	down:
	 	add dh,1
		jmp cursor 
	left:
		sub dl,2
		jmp cursor
	right:
		add dl,2
		jmp cursor  
	exit:                 			 ;———esc判断一次退出
		call clear				;清屏
		mov ax,4c00H
		int 21H	
	
	enter1:							;jmp过长 接力
		jmp enter
	backup1:
		jmp backup
	backdown1:
		jmp backdown
	backleft1:
		jmp backleft
	backright1:
		jmp backright
	nojudge1:						;nojudge接力 并存回行列确定上一步位置（虽然是重复位置但用于定位）
		mov byte ptr ds:[79BH],dh	;存回行号
		mov byte ptr ds:[79CH],dl   ;存回列号
		jmp nojudge

	regret:							;悔棋，更改悔棋许可，取行列号
		mov ax,0
		mov al,ds:[79DH]			;悔棋许可
		cmp al,0					;判断是否已经悔过棋(数字0和1而非ascii码的0和1)
		je nojudge1

		mov dh,ds:[79EH]			;获得悔棋行列号
		mov dl,ds:[79FH]
		mov al,50H					;同理于下面的enter处
		mul dh
		push dx
		mov dh,0
		add ax,dx
		pop dx
		mov bx,ax
		mov byte ptr ds:[bx],46				;存回点，表示悔棋

		mov al,ds:[791H]					;更改落子方结束悔棋，落子数减一，改悔棋许可
		cmp al,49							;交换落子方
		je reswap
		mov byte ptr ds:[791H],49
		mov byte ptr ds:[793H],111
	backregret:
		dec word ptr ds:[797H]      		;减落子数,改悔棋许可为禁止
		mov byte ptr ds:[79DH],0			;数字0而非ascii码
		mov byte ptr ds:[79BH],dh	    	;将光标位置移动到悔棋位置
		mov byte ptr ds:[79CH],dl
		call show							;悔棋不做判定，单独输出一次棋盘   									
		jmp nojudge
	reswap:
		mov byte ptr ds:[791H],50
		mov byte ptr ds:[793H],120
		jmp backregret

	enter:				  			 ;——落子改变(已落子不能再落子,未落子存入落子数据x玩家1，o玩家2并改变玩家状态)	
		mov ax,0					 ;找到对应数据段位置
		mov bx,0
		mov al,50H           		 ;行对应数据段
		mul dh
		push dx						 ;溢出，dx可能后面有用 bug1
		mov dh,0
		add ax,dx			  		 ;实际位置加上列
		pop dx
		mov bx,ax					 
 		mov al,ds:[bx]           ;找到数据段对应位置
	
		cmp al,46				;判断是否落子，未落子则落，已落子则不落子。
		jne nojudge1					 ;已落子，结束本次操作
		
		mov al,ds:[791H]			 ;判断玩家落子并交换玩家
		cmp al,49					 ;向下为玩家2落子o,跳转为玩家1落子x
		je swaps

		mov byte ptr ds:[bx],120		;玩家2落子x

		backenter:
		inc word ptr ds:[797H]		;加落子数,改悔棋许可为允许
		mov byte ptr ds:[79DH],1		;一定是落子成功才存入
		mov byte ptr ds:[79EH],dh		;存回悔棋行号
		mov byte ptr ds:[79FH],dl   	;存回悔棋列号
		next:
			ret
	swaps:                            	;跳转为玩家1落子o
			mov byte ptr ds:[bx],111
			jmp backenter
;——————————————————————————————————————;越界回到界内。
	backup:
		mov dh,1
		jmp scan
	backdown:
		mov dh,20
		jmp scan
	backleft:
		mov dl,0
		jmp scan
	backright:
		mov dl,78
		jmp scan
;————————————————————————————————————————胜利判断（主函数）;①从落子处开始判定 
;做四次判定，依次都为从落子处向左再向右，count总和大于等于5判定胜利（判断边界）
judge:
	mov ax,data
	mov ds,ax
	mov dh,ds:[79BH]			 ;拿出行列号
	mov dl,ds:[79CH]

heng_left:
	mov ax,0
	mov bx,0
	mov al,dl					;al做行号
	mov bl,2					;除以2判断区间
	div bl
	cmp al,4 
	jb record_left
	mov al,4
record_left:
	mov byte ptr ds:[8E0H],al

heng_right:
	mov ax,0
	mov bx,0
	mov al,dl
	mov bl,2
	div bl
	mov ah,39
	sub ah,al
	cmp ah,4
	jb record_right
	mov ah,4
record_right:
	mov byte ptr ds:[8E1H],ah

shu_up:					    ;竖循环次数判定 入栈左右
	mov ax,0
	mov al,dh					;al做列号
	dec al
	cmp al,4 
	jb record_up
	mov al,4
record_up:
	mov byte ptr ds:[8E2H],al

shu_down:
	mov ax,0
	mov al,dh
	mov ah,20
	sub ah,al
	cmp ah,4
	jb record_down
	mov ah,4
record_down:
	mov byte ptr ds:[8E3H],ah

juheng_start:
	mov ah,1						  ;本身的子算一个 ah为count
	mov al,ds:[793H]				  ;获得当前落子方的子 al为落子方的子
	mov ch,0
	mov cl,ds:[8E0H]
	mov dh,ds:[79BH]				  ;拿出行列号
	mov dl,ds:[79CH]
	jcxz juheng_left_end				  ;等于0直接跳转
juheng_left:                           ;横左判定
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	mul dh
	sub dl,2
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0
	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne juheng_left_end
	inc ah							;同子加一
	mov ch,0
	loop juheng_left

juheng_left_end:						 ;左判定结束直接右判定
	mov ch,0
	mov cl,ds:[8E1H]
	mov dh,ds:[79BH]		    	 ;拿出行列号
	mov dl,ds:[79CH]
	mov bx,cx						 ;bug3:右判定溢出，虽然不知道为什么
	mov cx,bx
	jcxz jushu_start			         ;等于0直接跳转
juheng_right:						 ;横右判定
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	mul dh	
	add dl,2
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0

	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne heng_count
	inc ah							;同子加一
	mov ch,0
	loop juheng_right

heng_count:						;判断是否胜利
	cmp ah,4
	ja gamewin1
	jmp jushu_start
;—————————————————————————————————————————————
;—————————————————————————————————————————————
jushu_start:							 ;竖判定
	mov ah,1						  ;本身的子算一个 ah为count
	mov al,ds:[793H]				  ;获得当前落子方的子 al为落子方的子
	mov ch,0
	mov cl,ds:[8E2H]
	mov dh,ds:[79BH]				  ;拿出行列号
	mov dl,ds:[79CH]
	jcxz jushu_up_end				  ;等于0直接跳转
jushu_up:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	dec dh
	mul dh
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0
	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne jushu_up_end
	inc ah							;同子加一
	mov ch,0
	loop jushu_up

jushu_up_end:
	mov ch,0
	mov cl,ds:[8E3H]
	mov dh,ds:[79BH]		    	 ;拿出行列号
	mov dl,ds:[79CH]
	mov bx,cx						 ;bug3:右判定溢出，虽然不知道为什么
	mov cx,bx
	jcxz jupie_start			         ;等于0直接跳转
jushu_down:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	inc dh
	mul dh	
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0

	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne shu_count
	inc ah							;同子加一
	mov ch,0
	loop jushu_down

shu_count:						;判断是否胜利
	cmp ah,4
	ja gamewin1
	jmp jupie_start
;—————————————————————————————————————————————
gamewin1:							;中继
	jmp gamewin2
;—————————————————————————————————————————————
put_pie_left:
	mov cl,ch
	mov ch,0
	jmp back_pie_left
;—————————————————————————————————————————————
jupie_start:							 ;撇判定
	mov ah,1						  ;本身的子算一个 ah为count
	mov al,ds:[793H]				  ;获得当前落子方的子 al为落子方的子
	mov ch,ds:[8E0H]
	mov cl,ds:[8E2H]
	cmp ch,cl
	jb put_pie_left
	mov ch,0
back_pie_left:
	mov dh,ds:[79BH]				  ;拿出行列号
	mov dl,ds:[79CH]
	jcxz jupie_left_end				  ;等于0直接跳转
jupie_left:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	dec dh
	mul dh
	sub dl,2
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0
	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne jupie_left_end
	inc ah							;同子加一
	mov ch,0
	loop jupie_left
jmp jupie_left_end

put_pie_right:
	mov cl,ch
	mov ch,0
	jmp back_pie_right
jupie_left_end:
	mov ch,0
	mov ch,ds:[8E1H]
	mov cl,ds:[8E3H]
	cmp ch,cl
	jb put_pie_right
back_pie_right:
	mov dh,ds:[79BH]		    	 ;拿出行列号
	mov dl,ds:[79CH]
	mov bx,cx						 ;bug3:右判定溢出，虽然不知道为什么
	mov cx,bx
	jcxz juna_start			         ;等于0直接跳转
jupie_right:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	inc dh
	mul dh
	add dl,2	
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0

	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne pie_count
	inc ah							;同子加一
	mov ch,0
	loop jupie_right

pie_count:						;判断是否胜利
	cmp ah,4
	ja gamewin2
	jmp juna_start
;—————————————————————————————————————————————
gamewin2:							;中继
	jmp gamewin
;—————————————————————————————————————————————
put_na_left:
	mov cl,ch
	mov ch,0
	jmp back_na_left
;—————————————————————————————————————————————
juna_start:							 ;捺判定
	mov ah,1						  ;本身的子算一个 ah为count
	mov al,ds:[793H]				  ;获得当前落子方的子 al为落子方的子
	mov ch,ds:[8E0H]
	mov cl,ds:[8E3H]
	cmp ch,cl
	jb put_na_left
	mov ch,0
back_na_left:
	mov dh,ds:[79BH]				  ;拿出行列号
	mov dl,ds:[79CH]
	jcxz juna_left_end				  ;等于0直接跳转
juna_left:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	inc dh
	mul dh
	sub dl,2
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0
	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne juna_left_end
	inc ah							;同子加一
	mov ch,0
	loop juna_left
jmp juna_left_end

put_na_right:
	mov cl,ch
	mov ch,0
	jmp back_na_right
juna_left_end:
	mov ch,0
	mov ch,ds:[8E1H]
	mov cl,ds:[8E2H]
	cmp ch,cl
	jb put_na_right
back_na_right:
	mov dh,ds:[79BH]		    	 ;拿出行列号
	mov dl,ds:[79CH]
	mov bx,cx						 ;bug3:右判定溢出，虽然不知道为什么
	mov cx,bx
	jcxz draw		
juna_right:
	push ax							;存入落子方

	mov ax,0					 ;寻址;找到对应数据段位置
	mov bx,0
	mov al,50H           		 ;行对应数据段
	dec dh
	mul dh
	add dl,2	
	push dx						 ;溢出，dx可能后面有用 bug1
	mov dh,0

	add ax,dx			  		 ;实际位置加上列
	pop dx
	mov bx,ax					 
	mov ch,ds:[bx]           	 ;找到数据段对应位置

	pop ax							;取回落子方
	cmp ch,al
	jne na_count
	inc ah							;同子加一
	mov ch,0
	loop juna_right
na_count:						;判断是否胜利
	cmp ah,4
	ja gamewin
	jmp draw

gamewin:							 ;游戏胜利，获得胜利方，修改胜利方，胜负状态，退出判定
	mov al,ds:[791H]				 ;获得胜利方
	sub al,48						 ;bug2:ascii码转化成数字
	mov byte ptr ds:[799H],al        ;修改胜利方
	mov byte ptr ds:[79AH],1         ;修改胜负状态
	jmp gameover

draw:						     ;和棋判定
	mov ax,ds:[797H]
	cmp ax,800
	je gamedraw
	jmp gameover

gamedraw:							;和棋，更改胜利方，胜负状态
	mov byte ptr ds:[799H],3        ;修改胜利方
	mov byte ptr ds:[79AH],1        ;修改胜负状态
	jmp gameover


gameover:
	ret

;无结果不改变任何数据
;胜利后依照落子方改变胜负方，改变胜负状态。
;————————————————————————————————————————交换落子方（主函数）
mainswap:
	mov ax,0
	mov al,ds:[791H]			 ;判断玩家落子并交换玩家
	cmp al,49					 ;向下为切换成玩家1，跳转为切换成玩家2
	je swap

	mov byte ptr ds:[791H],49		;切换下一次玩家1落子
	mov byte ptr ds:[793H],111
	ret

swap:
	mov byte ptr ds:[791H],50	;切换下一次玩家2落子
	mov byte ptr ds:[793H],120

	ret

;————————————————————————————————————————输出结束语句（主函数）;①1红胜 2蓝胜 3和棋ret
put:
	mov ax,data
	mov ds,ax						;ds为原始位置
	mov es,ax						;es为目标位置
	mov ah,ds:[799H]				;设置乘数并依照获胜方获得数据段位置
	mov al,50H
	mul ah
	mov si,ax
	add si,7A0H
	mov di,6E0H
	mov cx,80
	cld
	rep movsb
ret
;————————————————————————————————————————重开或者结束（主函数）
;结束：清屏    重开：玩家 改胜负状态 落子行列 悔棋许可 悔棋行列 落子方 重置棋盘以及引导语
restart:
	mov ah,0cH       ;清除键盘缓存
	mov al,0
	int 21H

	mov ah,2        ;初始化光标位置
	mov al,0
	mov bh,0
	mov dh,ds:[79BH]     ;对应地址存行号
	mov dl,ds:[79CH]     ;对应地址存列号
	int 10H
choose:
	mov cx,5
	mov ah,0H				;等待数据输入
	int 16H

	cmp al,27				;esc退出
	je endgame
	cmp al,114				;r重开
	je begin	
	loop choose
endgame:                 			;———esc判断一次退出
	call clear						;清屏
	mov ax,4c00H		
	int 21H	
begin:
	mov ax,data
	mov ds,ax						;ds为原始位置
	mov es,ax						;es为目标位置
	mov word ptr ds:[743H],3031H	;重置行提示号
	mov word ptr ds:[746H],3132H	;重置列提示号
	mov byte ptr ds:[791H],49		;重置玩家1
	mov byte ptr ds:[793H],111		;重置玩家1的子:o
	mov word ptr ds:[797H],0		;重置落子数
	mov byte ptr ds:[799H],0		;重置胜负方
	mov byte ptr ds:[79AH],2		;重置胜负状态
	mov byte ptr ds:[79BH],10		;重置落子行
	mov byte ptr ds:[79CH],40		;重置落子列
	mov byte ptr ds:[79DH],0		;重置悔棋许可
	mov byte ptr ds:[79EH],10		;重置悔棋行
	mov byte ptr ds:[79FH],40		;重置悔棋列

	mov cx,800						;重置棋盘
	mov bx,50H						;定位棋盘起始位置
point:								;重置棋盘
	mov byte ptr ds:[bx],46
	inc bx
	mov byte ptr ds:[bx],00
	inc bx
	loop point

	mov si,7A0H						;重置引导语
	mov di,6E0H
	mov cx,80
	cld
	rep movsb;重置引导语
ret

code ends
end start

	import currentTCB;
	import highestTCB

	AREA SWITCH, CODE, READONLY
context_switch PROC
		export context_switch
		mrs  r0, psp
		;ldr r1, [r0]
		stmdb r0!, {r4-r11} 			; store the rest from register bank in process stack. 
										; stbdb->lowest address register goes to lowest memory address
		ldr r1, =currentTCB;			; store process stack pointer on TCB->TopOfStkPtr
		ldr r2, [r1]
		str r0, [r2]
		
		ldr r2, =highestTCB 			; get new psp from newTCB->TopOfStkPtr
		ldr r1, [r2]					
		ldr r0, [r1]			
		ldmia r0!, {r4-r11}
		msr psp, r0
		; update currentTCB with highestTCB
		ldr r0, =currentTCB
		ldr r1, =highestTCB
		ldr r2, [r1]
		str r2, [r0]
		bx lr
		ENDP
		
start_first_task PROC
		export start_first_task
		ldr r2, =highestTCB 			; get new psp from newTCB->TopOfStkPtr
		ldr r1, [r2]					
		ldr r0, [r1]			
		ldmia r0!, {r4-r11}
		msr psp, r0
		orr lr, lr, #0x4					;only for first time, get state from psp
		bx lr
		ENDP
		; load r11-r4 from there 
		; execute return from exception (bx LR)
		
				

;		ldr r3, =highestTCB ; decision of highest prio task to run next should be from within  OR not? 
;		ldr r4, [r3]
;		str r4, [r2]

MainSVCHandler PROC
	EXPORT MainSVCHandler
	; go to r0 + 4 * 6
	; this is cm3 convention
	ldr r1, [r0, #(4 * 6)]
	ldr r0, [r1, #- 0x02]
	and r0, r0, #0xFF
	; call the right sys_call handler
	cmp r0, #0x23
	beq start_first_task
	
MainSVCHandlerExit
	bx lr
	ENDP


	AREA EXCEPTION_RAISE, CODE, READONLY
init_service PROC
	EXPORT init_service
	svc 0
	bx LR
	ENDP

	AREA  SWAP, CODE, READONLY
swap    PROC
		export swap
		sub R2, R3, R2     
		sub R3, R2
		bx  LR
	

	
;;
; bad way (using main stack pointer)
;context_switch PROC
;	     ;export context_switch
;		 push {r3-r4}
;		 ldr r3, =currentTCBalt
;		 ldr r4, [r3]
;		 str r0, [r4, #4]!
;		 str r1, [r4, #4]!
;		 str r2, [r4, #4]!
;		 pop {r3}
;		 str r3, [r4, #4]!
;		 pop {r3}
;		 str r3, [r4, #4]!
;		 stmdb r4!, {r5-r12}
;		 mov r2, SP ; store stack pointer: option 1 to the stack option 2 to the currentTCB->StkPtr which i think is better.
;		 str r2, [r4]
;       ;do the oposite here		 
; 
; Old attempt		
;		mov r0, SP
;		;push {SP}
;		
;		push {R0 - R12}
;		
;		MRS R2, PSR
;		push {R2}
;		;push {xPSR}
;		; link register is stored as well (important)
;		
;		push {LR}
;		;test
;		pop {LR}
;		pop {r0}
;		MSR PSR, R0
;		pop {R0 - R12}
;		mov SP, R0
;		bx LR
;		;/test
;		ldr r3, =currentTCB
;		ldr r4, [r3]
;		mov r2, SP ; store stack pointer: option 1 to the stack option 2 to the currentTCB->StkPtr which i think is better.
;		str r2, [r4]
		
				
		; we did it we stored them!
		; no go and cahnge currentTCB with highest TCB
		; we need to optimize this
;		ldr r2, =currentTCB ; this should be changed
;		ldr r3, =highestTCB ; decision of highest prio task to run next should be from within context_switch OR not? 
;		ldr r4, [r3]
;		str r4, [r2]
		;restore execution context
		;first get the new SP
;		ldr SP, [r4]
		;pop LR
		;pop {LR}
		;pop {PSR}
		;pop {}
		;str currentTCB, r3
		;mov currentTCB, [highestTCB]
		;push { }
;		bx PC
;		ENDP

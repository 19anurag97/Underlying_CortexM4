- For CONTROL register, this is how you toggle privilege and stack pointer selection.  
MRS R0, CONTROL     ; Read CONTROL into R0  
ORR R0, R0, #1      ; Set bit0 → Unprivileged  
MSR CONTROL, R0     ; Write back  
ISB                 ; Flush pipeline  
  
MRS R0, CONTROL     ; Read CONTROL  
BIC R0, R0, #1      ; Clear bit0 → Privileged  
MSR CONTROL, R0  
ISB  
  
- Use ORR to set bits.  
ORR R0, R0, #0x01   ; Set bit0 of R0  
  
- Use BIC to clear bits.  
BIC R0, R0, #0x01   ; Clear bit0 of R0  
  



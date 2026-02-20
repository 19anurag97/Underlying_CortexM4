**PSR Register**  
*Change in CPU register indicates which exception occured.*  

*Thread mode*  
Name : xpsr  
	Hex:0x61000000  
	Decimal:1627389952  
	Octal:014100000000  
	Binary:1100001000000000000000000000000  
	Default:1627389952  
  
*Handler mode*(Sys exception or IRQ)  
Name : xpsr  
	Hex:0x1000013  
	Decimal:16777235  
	Octal:0100000023  
	Binary:1000000000000000000010011  
	Default:16777235  
  
**Function calls**  
  
08000192:   bl      0x80001a8 <generate_interrupt>  
  
080001d2:   bx      lr  
  
- **LR**: Holds the return address when a function call (BL or BLX) is made.  
  
- When branching (BL), **PC** is updated with the target address, while LR stores the return address.  
  
On the ARM Cortex‑M4, when the processor enters Handler mode (e.g., during an interrupt or exception),  
the Link Register (LR) doesn’t hold a normal return address.   
Instead, it is loaded with a special “EXC_RETURN” value — which looks like a “negative” value because its high bits are set.  
*(e.g., 0xFFFFFFF9, 0xFFFFFFFD, 0xFFFFFFF1)*.  
  
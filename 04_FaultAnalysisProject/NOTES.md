**Failed Merge-sort recursion**  
  
After adding wrong base condition in Mergesort code.  
When I run the code in STM32 Cortex M cpu,  
it first goes into Memmanage fault handler without executing instructions  
it moves to HardFault handler.   

*Why ?*  

A classic fault escalation sequence on the Cortex‑M4.  

**Stack overflow/underflow** during exception entry → handler cannot execute properly.
  

**Explanation of the Flow**
1. Invalid Memory Access  
Caused by wrong base condition in MergeSort (out‑of‑bounds array access).
2. MemManage Fault Raised  
The SCB detects invalid memory access and sets the MemManage fault flag.
3. MemManage Handler Invoked  
 If enabled in SHCSR, the CPU attempts to run your MemManage_Handler.
4. Handler Not Enabled or Fails  
 If the handler is missing, disabled, or itself triggers another fault (e.g., stack overflow, invalid logging), the CPU cannot resolve it.
5. Escalation → HardFault  
  The system escalates to the HardFault handler, which is always enabled and acts as the final catch‑all  



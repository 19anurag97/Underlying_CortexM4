Q- Why my printf is not executing after changing access level from Priv to Unpriv level.  
Code going to hardfault handler in stm32 cortex m4. I have enabled SWV debugging using ITM.  
  
- *ITM/SWV access requires privileged mode*
- Privilege separation ensures user code cannot tamper with debug or system registers.
- Solution: Instead, request logging via an **SVC call** (Supervisor Call).
  
- Unprivileged Thread mode cannot *enable/disable interrupts directly* (e.g., writing to NVIC registers).  
- That requires privileged access.  
  

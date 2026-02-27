#include "fault_handlers.h"

// Global fault name strings for assembly handlers
const char UsageFault[] = "UsageFault";
const char MemManage[] = "MemManage";
const char BusFault[] = "BusFault";

void MPU_Config(void) {
  MPU->CTRL = 0;
  MPU->RNR = 0;
  MPU->RBAR = 0x20001000;
  MPU->RASR =
    (0x0 << MPU_RASR_AP_Pos) |
    (0x9 << MPU_RASR_SIZE_Pos) |
    (1 << MPU_RASR_ENABLE_Pos);
  MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
  __DSB();
  __ISB();
}

void EnableFaultExceptions(void) {
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk
              | SCB_SHCSR_BUSFAULTENA_Msk
              | SCB_SHCSR_USGFAULTENA_Msk;
  SCB->CCR   |= (SCB_CCR_DIV_0_TRP_Msk
              | SCB_CCR_UNALIGN_TRP_Msk);
  MPU_Config();
}

void GetFault(uint8_t faultType) {
  switch(faultType) {
    case 0://Usage Fault due to divide by zero.
      fun_divide(10,0);
      break;
    case 1: {//Usage Fault due to unaligned access
      uint32_t *ptr = (uint32_t*)0x20000001;
      *ptr = 0xDEADBEEF;
      break; }
    case 2:
        //Usage Fault due to infinite recursion (stack overflow)
      uint8_t a[] = {5, 2, 8, 1, 9, 3, 7, 4, 6, 19, 11};
      int size = sizeof(a)/sizeof(a[0]);
      MergeSort(0, (size-1), a);
      break;
    case 3: {//Memory Management Fault due to MPU violation
      volatile uint32_t *forbidden = (uint32_t*)0x20001000;
      *forbidden = 0xDEADBEEF;
      break; }
    case 4: {//Bus Fault due to invalid memory access
      volatile uint32_t *invalid = (uint32_t*)0x400FFFFC;
      *invalid = 0xDEADBEEF;
      break; }
    default:
      break;
  }
}

int fun_divide(int x , int y) {
  return x/y;
}

void HardFault_Handler(void) {
  printf("Exception : Hardfault\n");
  while(1);
}

__attribute__ ((naked)) void UsageFault_Handler(void) {
  __asm volatile (
    "MRS r0, MSP\n"
    "LDR r1, =UsageFault\n"
    "B GlobalFault_Handler_c\n"
  );
}

__attribute__ ((naked)) void MemManage_Handler(void) {
  __asm volatile (
    "MRS r0, MSP\n"
    "LDR r1, =MemManage\n"
    "B GlobalFault_Handler_c\n"
  );
}

__attribute__ ((naked)) void BusFault_Handler(void) {
  __asm volatile (
    "MRS r0, MSP\n"
    "LDR r1, =BusFault\n"
    "B GlobalFault_Handler_c\n"
  );
}

void GlobalFault_Handler_c(uint32_t *pBaseStackFrame, const char *faultName) {
  printf("Exception : %s\n", faultName);
  if (faultName && strcmp(faultName, "UsageFault") == 0) {
    uint16_t *pUFSR = (uint16_t*)0xE000ED2A;
    printf("UFSR = %x\n", *pUFSR);
  }
  if (faultName && strcmp(faultName, "MemManage") == 0) {
    uint8_t *pMMFSR = (uint8_t*)0xE000ED28;
    printf("MMFSR = %x\n", *pMMFSR);
  }
  if (faultName && strcmp(faultName, "BusFault") == 0) {
    uint8_t *pBFSR = (uint8_t*)0xE000ED29;
    printf("BFSR = %x\n", *pBFSR);
  }
  printf("pBaseStackFrame = %p\n", pBaseStackFrame);
  printf("Value of R0 = %lx\n", pBaseStackFrame[0]);
  printf("Value of R1 = %lx\n", pBaseStackFrame[1]);
  printf("Value of R2 = %lx\n", pBaseStackFrame[2]);
  printf("Value of R3 = %lx\n", pBaseStackFrame[3]);
  printf("Value of R12 = %lx\n", pBaseStackFrame[4]);
  printf("Value of LR = %lx\n", pBaseStackFrame[5]);
  printf("Value of PC = %lx\n", pBaseStackFrame[6]);
  printf("Value of XPSR = %lx\n", pBaseStackFrame[7]);
  while(1);
}

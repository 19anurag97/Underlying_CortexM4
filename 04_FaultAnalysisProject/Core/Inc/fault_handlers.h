#ifndef FAULT_HANDLERS_H
#define FAULT_HANDLERS_H

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Global fault name strings
extern const char UsageFault[];
extern const char MemManage[];
extern const char BusFault[];

void MPU_Config(void);
void EnableFaultExceptions(void);
void GetFault(uint8_t faultType);
void GlobalFault_Handler_c(uint32_t *pMSP, const char *faultName);
int fun_divide(int x , int y);

void HardFault_Handler(void);
void UsageFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);

#endif // FAULT_HANDLERS_H

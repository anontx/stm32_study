#define RCC_APB2ENR (*((volatile unsigned long *)(0x40021000 + 0x18)))
#define GPIOB_CRH   (*((volatile unsigned long *)(0x40010C00 + 0x04)))
#define GPIOB_BSRR  (*((volatile unsigned long *)(0x40010C00 + 0x10)))

void ResetISR(void);
void NMIException(void);
void HardFaultException(void);

typedef void (*pfnISR)(void);

__attribute__ ((section(".isr_vector")))
pfnISR VectorTable[] =
{
  (pfnISR)(0x20001000),
  ResetISR,
  NMIException,
  HardFaultException
};

int main(void)
{
  unsigned int c = 0;

  RCC_APB2ENR = (1 << 3);
  GPIOB_CRH = 0x44444443;

  while(1)
  {
    GPIOB_BSRR = (1 << 24);
    for(c = 0; c < 100000; c++);
    GPIOB_BSRR = (1 << 8);
    for(c = 0; c < 100000; c++);
  }
}

extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

void ResetISR(void)
{
  unsigned long *src, *dst;

  src = &_etext;
  dst = &_data;
  while(dst < &_edata) *dst++ = *src++;

  for(dst = &_bss; dst < &_ebss; dst++)
    *dst = 0;

  main();
}

void NMIException(void)
{
  return;
}

void HardFaultException(void)
{
  return;
}

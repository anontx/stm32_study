//
// bootloader
//
void ResetISR(void);
void NMIException(void);
void HardFaultException(void);

int main(void);

typedef void (*pfnISR)(void);

__attribute__ ((section(".isr_vector")))
pfnISR VectorTable[] =
{
  (pfnISR)(0x20001000),
  ResetISR,
  NMIException,
  HardFaultException
};

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

//
// main
//
#define RCC_CR      (*((volatile unsigned long *)(0x40021000)))
#define RCC_CFGR    (*((volatile unsigned long *)(0x40021000 + 0x04)))

#define RCC_APB2ENR (*((volatile unsigned long *)(0x40021000 + 0x18)))

#define GPIOB_CRH   (*((volatile unsigned long *)(0x40010C00 + 0x04)))
#define GPIOB_BSRR  (*((volatile unsigned long *)(0x40010C00 + 0x10)))

void delay(void)
{
  unsigned int c = 0;
  for(c = 0; c < 100000; c++);
}

void SetSystemClock(void)
{
  //1.enable HSE
  RCC_CR |= 0x10000;

  //2.wait HSERDY
  while(RCC_CR & 0x20000 == 0);

  //3.set PLL source is HSE
  RCC_CFGR |= 0x10000;

  //4.set PLLMUL
  RCC_CFGR |= 0x1c0000;

  //5.set PCLK1 & PCLK2 prescaler
  RCC_CFGR |= 0x400;

  //6.set HCLK prescaler
  RCC_CFGR |= 0x0;

  //7.enable PLL
  RCC_CR |= 0x1000000;

  //8.wait PLLRDY
  while(RCC_CR & 0x2000000 == 0);

  //9.set system clock is PLL
  RCC_CFGR &= (unsigned long)~0x3;
  RCC_CFGR |= (unsigned long)0x2;

  //10.check system clock switch status
  while(RCC_CFGR & 0x8 == 0)
  {
  }
}

int main(void)
{
  SetSystemClock();

  RCC_APB2ENR = (1 << 3);
  GPIOB_CRH = 0x44444443;

  while(1)
  {
    GPIOB_BSRR = (1 << 24);
    delay();
    GPIOB_BSRR = (1 << 8);
    delay();
  }
}

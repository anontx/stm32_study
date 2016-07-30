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
#define RCC_APB2ENR (*((volatile unsigned long *)(0x40021000 + 0x18)))

#define GPIOA_CRH   (*((volatile unsigned long *)(0x40010800 + 0x04)))
#define GPIOA_BSRR  (*((volatile unsigned long *)(0x40010800 + 0x10)))
#define GPIOB_CRH   (*((volatile unsigned long *)(0x40010C00 + 0x04)))
#define GPIOB_BSRR  (*((volatile unsigned long *)(0x40010C00 + 0x10)))

#define USART1_SR   (*((volatile unsigned long *)(0x40013800)))
#define USART1_DR   (*((volatile unsigned long *)(0x40013800 + 0x04)))
#define USART1_BRR  (*((volatile unsigned long *)(0x40013800 + 0x08)))
#define USART1_CR1  (*((volatile unsigned long *)(0x40013800 + 0x0c)))
#define USART1_CR2  (*((volatile unsigned long *)(0x40013800 + 0x10)))
#define USART1_CR3  (*((volatile unsigned long *)(0x40013800 + 0x14)))


void delay(void)
{
  unsigned int c = 0;
  for(c = 0; c < 100000; c++);
}

int main(void)
{
  char *body = "hello, world!";

  //usart1 & GPIOA RCC enable
  RCC_APB2ENR |= 0x4004;
  GPIOA_CRH |= 0x4B0;

  USART1_CR1 |= 0x2000; //enable USART & set word length
  USART1_CR2 |= 0x0;    //default stop bits eq 8
  //USART1_CR3 |= 0x80;
  USART1_BRR |= 0x341;  //baud rate eq 9600

  USART1_CR1 |= 0x8;    //te

  while(1)
  {
    for(;*body;body++)
    {
      USART1_DR = *body;
      while(USART1_SR & 0x40 == 0); //check tc
    }
  }
}

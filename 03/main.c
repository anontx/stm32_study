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
//main
//
#define RCC_BASE          (0x40021000)
#define RCC_APB2ENR       (*((volatile unsigned long *)(RCC_BASE + 0x18)))

#define USART1_BASE       (0x40013800)
#define USART1_SR         (*((volatile unsigned long *)(USART1_BASE)))
#define USART1_DR         (*((volatile unsigned long *)(USART1_BASE + 0x04)))
#define USART1_BRR        (*((volatile unsigned long *)(USART1_BASE + 0x08)))
#define USART1_CR1        (*((volatile unsigned long *)(USART1_BASE + 0x0c)))
#define USART1_CR2        (*((volatile unsigned long *)(USART1_BASE + 0x10)))
#define USART1_CR3        (*((volatile unsigned long *)(USART1_BASE + 0x14)))

#define GPIOA_BASE        (0x40010800)
#define GPIOA_CRH         (*((volatile unsigned long *)(GPIOA_BASE + 0x04)))
#define GPIOA_BSRR        (*((volatile unsigned long *)(GPIOA_BASE + 0x10)))

void delay(void)
{
  unsigned int c = 0;
  for(c = 0; c < 100000; c++);
}

int main(void)
{
  char *body = "hello, world";

  //USART1 & GPIOA clock enable
  RCC_APB2ENR |= 0x4004;

  //set GPIO mode
  GPIOA_CRH |= 0xb0;  //PA9
  GPIOA_CRH |= 0x400; //PA10

  //1.enable USART_CR1 UE
  USART1_CR1 |= 0x2000;

  //2.set USART_CR1 word length
  //USART1_CR1 |= 0x0000;

  //3.set USART_CR2 stop bits
  //USART1_CR2 |= 0x0000;

  //4.set USART_BRR baud rate
  USART1_BRR = 0x341;

  //5.set USART_CR1 TE
  USART1_CR1 |= 0x8;

  //6.write date in USART_DR
  for(;*body;body++)
  {
    USART1_SR &= ~(1 << 6);
    USART1_DR = (unsigned short)(0x1ff & *body);
    while(USART1_SR & 0x40 == 0); //bug
  }
}

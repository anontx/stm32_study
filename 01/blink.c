#define RCC_APB2ENR (*((volatile unsigned long *)(0x40021000 + 0x18)))
#define GPIOB_CRH   (*((volatile unsigned long *)(0x40010C00 + 0x04)))
#define GPIOB_BSRR  (*((volatile unsigned long *)(0x40010C00 + 0x10)))

asm(".word 0x20001000");
asm(".word main");

int main()
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

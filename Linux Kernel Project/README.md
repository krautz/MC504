### NAME:
randcall

### SYNOPSIS:
```C
#include <unistd.h>
```
unsigned long int sycall(384, unsigned long int begin, unsigned long int end);

### DESCRIPTION:
The randcall syscall outputs pseudo-random integers in the interval [begin, end]

### ERRORS:
0 - the min value is greater and equal than the end value, the begin value is less than zero or the end value is greater than 1,000,000

### Warning:
Do not use this function in infinite loop expecting random values. The function returns pseudo-random values if called isolated.

#### Authors:
Caio Krauthamer             RA: 165447
Heitor Boschirolli Comel RA: 169477
João Phillipe Cardenuto  RA: 170674
Leandro Nascimento Gonçalves de Araujo RA: 171855
/* 
 * Teste da nova chamada de sistema
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    unsigned long int aleatorio, F0 = 0, F1 = 1, aux, quad, cub, i, flag = 1;
    int ale;

    printf("Entre com um numero positivo (exceto 0), o intervalo de escolha sera entre 0 e seu numero:\n");
    while(flag) {
        scanf("%d", &ale);
        if (ale>0) {
            flag = 0;
        } else {
            printf("O numero inserido eh negativo ou 0! Por favor insira um positivo\n");
        }
    }

    aleatorio = syscall(384, 0, (unsigned long int) ale);
    printf("Retorno da chamada de sistema: %lu.\n", aleatorio);

    printf("\n---------TESTANDO A PARIDADE DO NUMERO---------\n");
    if (aleatorio % 2 == 0) {
        printf("O retorno da chamada de sistema eh par!\n");
        printf("\n---------TESTANDO SE O NUMERO EH PRIMO---------\n");
        if (aleatorio != 2) {
            printf("O retorno da chamada de sistema nao eh primo!\n");
        } else {
            printf("O retorno da chamada de sistema eh primo!\n");
        }
    } else {
        printf("O retorno da chamada de sistema eh impar!\n");
        printf("\n---------TESTANDO SE O NUMERO EH PRIMO---------\n");
        for (i = 3, flag = 1; i < (aleatorio / 2) && flag; i = i + 2) {
            if (aleatorio % i == 0) {
                flag = 0;
                printf("O retorno da chamada de sistema nao eh primo!\n");
            }
        }
        if (flag)
            printf("O retorno da chamada de sistema eh primo!\n");
    }

    printf("\n---------TESTANDO SE O NUMERO EH DE FIBONACCI---------\n");
    if (aleatorio == 0) {
        printf("O retorno da chamada de sistema eh um numero de fibonacci!\n");
        flag = 0;
    } else {
        flag = 1;
        while (flag) {
            if (aleatorio == F1) {
                printf("O retorno da chamada de sistema eh um numero de fibonacci!\n");
                flag = 0;
            }
            if (F1 > aleatorio) {
                printf("O retorno da chamada de sistema nao eh um numero de fibonacci!\n");
                flag = 0;
            }
            aux = F1;
            F1 += F0;
            F0 = aux;
        }
    }

    printf("\n---------TESTANDO SE O NUMERO EH QUADRADO PERFEITO---------\n");
    flag = 1;
    for (i = 0; flag; i++) {
        quad = i*i;
        if (quad == aleatorio) {
            printf("O retorno da chamada de sistema eh um quadrado perfeito!\n");
            flag = 0;
        }
        if (quad > aleatorio) {
            printf("O retorno da chamada de sistema nao eh um quadrado perfeito!\n");
            flag = 0;
        }
    }

    printf("\n---------TESTANDO SE O NUMERO EH CUBO PERFEITO---------\n");
    flag = 1;
    for (i = 0; flag; i++) {
        cub = i*i*i;
        if (cub == aleatorio) {
            printf("O retorno da chamada de sistema eh um cubo perfeito!\n");
            flag = 0;
        }
        if (cub > aleatorio) {
            printf("O retorno da chamada de sistema nao eh um cubo perfeito!\n");
            flag = 0;
        }
    }

    return aleatorio;
}
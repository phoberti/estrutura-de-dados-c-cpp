/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <stdio.h>

int mult_russa(int a, int b) {
    if (a == 0) {
        return 0;
    }
    if (a % 2 != 0) {
        return b + mult_russa(a / 2, b * 2);
    } else {
        return mult_russa(a / 2, b * 2);
    }
}

int main() {
    int n1, n2;

    printf("Insira n1: ");
    scanf("%d", &n1);

    printf("Insira n2: ");
    scanf("%d", &n2);

    int resultado = mult_russa(n1, n2);

    printf("O resultado da multiplicação russa é: %d\n", resultado);

    return 0;
}

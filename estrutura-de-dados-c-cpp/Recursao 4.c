/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <stdio.h>

int padovan(int n) {
    if (n == 0 || n == 1 || n == 2) {
        return 1;
    } else {
        return padovan(n - 2) + padovan(n - 3);
    }
}

int main() {
    int n;

    printf("Digite o valor de N: ");
    scanf("%d", &n);

    int resultado = padovan(n);

    printf("O  termo  %d  da sequência de Padovan é: %d\n", n, resultado);

    return 0;
}
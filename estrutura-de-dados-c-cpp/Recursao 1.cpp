/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>

int Multiplicacao(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }

    return a + Multiplicacao(a, b - 1);
}

int main() {
    int n1, n2;

    std::cout << "Insira n1: ";
    std::cin >> n1;

    std::cout << "Insira n2: ";
    std::cin >> n2;

    int resultado = Multiplicacao(n1, n2);

    std::cout << "O resultado da multiplicação é: " << resultado << std::endl;

    return 0;
}

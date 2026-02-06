/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>

int Tribonacci(int n) {
    if (n == 0 || n == 1) {
        return 0;
    } else if (n == 2) {
        return 1;
    } else {
        return Tribonacci(n - 1) + Tribonacci(n - 2) + Tribonacci(n - 3);
    }
}

int Padovan(int n) {
    if (n == 0 || n == 1 || n == 2) {
        return 1;
    } else {
        return Padovan(n - 2) + Padovan(n - 3);
    }
}

int main() {
    int n;

    std::cout << "Digite o valor de N: ";
    std::cin >> n;

    int trib = Tribonacci(n);
    int pad = Padovan(n);

    std::cout << "O  termo " << n << " da sequência de Tribonacci é: " << trib << std::endl;
    std::cout << "O  termo " << n << " da sequência de Padovan é: " << pad << std::endl;

    return 0;
}

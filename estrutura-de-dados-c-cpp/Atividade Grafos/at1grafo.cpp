#include <iostream>
#include <map>
#include <vector>

class Estado {
public:
    Estado(const std::string& nome) : nomeEstado(nome) {}

    std::string nomeEstado;
    std::vector<Estado*> fronteiras;
};

class MapaNordeste {
public:
    void adicionarEstado(const std::string& nomeEstado) {
        Estado* novoEstado = new Estado(nomeEstado);
        estados[nomeEstado] = novoEstado;
    }

    void adicionarFronteira(const std::string& estadoDe, const std::string& estadoPara) {
        if (estados.find(estadoDe) != estados.end() && estados.find(estadoPara) != estados.end()) {
            estados[estadoDe]->fronteiras.push_back(estados[estadoPara]);
            estados[estadoPara]->fronteiras.push_back(estados[estadoDe]);
        }
    }

    void encontrarEstadosComMaisFronteiras() {
        int maxFronteiras = 0;
        std::vector<std::string> estadosComMaisFronteiras;

        for (const auto& parEstado : estados) {
            const Estado* estado = parEstado.second;
            int numFronteiras = estado->fronteiras.size();

            if (numFronteiras > maxFronteiras) {
                maxFronteiras = numFronteiras;
                estadosComMaisFronteiras.clear();
                estadosComMaisFronteiras.push_back(estado->nomeEstado);
            } else if (numFronteiras == maxFronteiras) {
                estadosComMaisFronteiras.push_back(estado->nomeEstado);
            }
        }

        if (maxFronteiras > 0) {
            std::cout << "Estados com mais fronteiras (" << maxFronteiras/2 << " fronteiras): ";
            for (const std::string& nomeEstado : estadosComMaisFronteiras) {
                std::cout << nomeEstado << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Nenhuma fronteira adicionada ou estados encontrados." << std::endl;
        }
    }

private:
    std::map<std::string, Estado*> estados;
};

int main() {
    MapaNordeste mapaNordeste;

    // Adicionar estados
    mapaNordeste.adicionarEstado("Bahia");
    mapaNordeste.adicionarEstado("Sergipe");
    mapaNordeste.adicionarEstado("Alagoas");
    mapaNordeste.adicionarEstado("Pernambuco");
    mapaNordeste.adicionarEstado("Paraíba");
    mapaNordeste.adicionarEstado("Rio Grande do Norte");
    mapaNordeste.adicionarEstado("Ceará");
    mapaNordeste.adicionarEstado("Maranhão");
    mapaNordeste.adicionarEstado("Piauí");

    // Adicionar fronteiras
    mapaNordeste.adicionarFronteira("Bahia", "Sergipe");
    mapaNordeste.adicionarFronteira("Bahia", "Alagoas");
    mapaNordeste.adicionarFronteira("Bahia", "Pernambuco");
    mapaNordeste.adicionarFronteira("Bahia", "Piauí");
    mapaNordeste.adicionarFronteira("Piauí", "Ceará");
    mapaNordeste.adicionarFronteira("Piauí", "Pernambuco");
    mapaNordeste.adicionarFronteira("Piauí", "Bahia");
    mapaNordeste.adicionarFronteira("Piauí", "Maranhão");
    mapaNordeste.adicionarFronteira("Maranhão", "Piauí");
    mapaNordeste.adicionarFronteira("Ceará", "Piauí");
    mapaNordeste.adicionarFronteira("Ceará", "Rio Grande do Norte");
    mapaNordeste.adicionarFronteira("Ceará", "Paraíba");
    mapaNordeste.adicionarFronteira("Ceará", "Pernambuco");
    mapaNordeste.adicionarFronteira("Pernambuco", "Bahia");
    mapaNordeste.adicionarFronteira("Pernambuco", "Piauí");
    mapaNordeste.adicionarFronteira("Pernambuco", "Ceará");
    mapaNordeste.adicionarFronteira("Pernambuco", "Paraíba");
    mapaNordeste.adicionarFronteira("Pernambuco", "Alagoas");
    mapaNordeste.adicionarFronteira("Paraíba", "Ceará");
    mapaNordeste.adicionarFronteira("Paraíba", "Pernambuco");
    mapaNordeste.adicionarFronteira("Paraíba", "Rio Grande do Norte");
    mapaNordeste.adicionarFronteira("Rio Grande do Norte", "Ceará");
    mapaNordeste.adicionarFronteira("Rio Grande do Norte", "Paraíba");
    mapaNordeste.adicionarFronteira("Alagoas", "Bahia");
    mapaNordeste.adicionarFronteira("Alagoas", "Pernambuco");
    mapaNordeste.adicionarFronteira("Alagoas", "Sergipe");
    mapaNordeste.adicionarFronteira("Sergipe", "Bahia");
    mapaNordeste.adicionarFronteira("Sergipe", "Alagoas");

    // Encontrar estados com mais fronteiras
    mapaNordeste.encontrarEstadosComMaisFronteiras();

    return 0;
}

#include <iostream>
#include <map>
#include <set>
#include <vector>

class Usuario {
public:
    Usuario(const std::string& nome) : nomeUsuario(nome) {}

    std::string nomeUsuario;
    std::set<Usuario*> amigos;
};

class SocialNet {
public:
    void adicionarUsuario(const std::string& nomeUsuario) {
        Usuario* novoUsuario = new Usuario(nomeUsuario);
        usuarios[nomeUsuario] = novoUsuario;
    }

    void adicionarAmizade(const std::string& usuario1, const std::string& usuario2) {
        if (usuarios.find(usuario1) != usuarios.end() && usuarios.find(usuario2) != usuarios.end()) {
            usuarios[usuario1]->amigos.insert(usuarios[usuario2]);
            usuarios[usuario2]->amigos.insert(usuarios[usuario1]);
        }
    }

    void encontrarUsuarioMaisPopular() {
        int maxAmigos = 0;
        std::string usuarioMaisPopular;

        for (const auto& parUsuario : usuarios) {
            const Usuario* usuario = parUsuario.second;
            int numAmigos = usuario->amigos.size();

            if (numAmigos > maxAmigos) {
                maxAmigos = numAmigos;
                usuarioMaisPopular = usuario->nomeUsuario;
            }
        }

        if (maxAmigos > 0) {
            std::cout << "Usuário mais popular: " << usuarioMaisPopular << " com " << maxAmigos << " amigos." << std::endl;
        } else {
            std::cout << "Nenhuma amizade adicionada ou usuários encontrados." << std::endl;
        }
    }

void sugerirAmizade(const std::string& usuario) {
    if (usuarios.find(usuario) != usuarios.end()) {
        Usuario* usuarioAtual = usuarios[usuario];

        std::set<Usuario*> amigosDoUsuario = usuarioAtual->amigos;
        std::set<Usuario*> sugerir;

        for (const auto& parUsuario : usuarios) {
            Usuario* outroUsuario = parUsuario.second;
            if (outroUsuario != usuarioAtual && amigosDoUsuario.find(outroUsuario) == amigosDoUsuario.end()) {
                sugerir.insert(outroUsuario);
            }
        }

        if (!sugerir.empty()) {
            std::cout << "Sugestão de amizade para " << usuario << ": ";
            for (const Usuario* sugerido : sugerir) {
                std::cout << sugerido->nomeUsuario << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Nenhuma sugestão de amizade disponível para " << usuario << "." << std::endl;
        }
    }
}


private:
    std::map<std::string, Usuario*> usuarios;
};

int main() {
    SocialNet socialNet;

    // Adicionar usuários
    socialNet.adicionarUsuario("Alice");
    socialNet.adicionarUsuario("Bob");
    socialNet.adicionarUsuario("Carol");
    socialNet.adicionarUsuario("David");
    socialNet.adicionarUsuario("Eve");
    socialNet.adicionarUsuario("Frank");
    socialNet.adicionarUsuario("Grace");

    // Adicionar amizades
    socialNet.adicionarAmizade("Alice", "Bob");
    socialNet.adicionarAmizade("Alice", "Carol");
    socialNet.adicionarAmizade("Bob", "David");
    socialNet.adicionarAmizade("Carol", "Eve");
    socialNet.adicionarAmizade("David", "Eve");
    socialNet.adicionarAmizade("Eve", "Frank");
    socialNet.adicionarAmizade("Frank", "Grace");

    // Encontrar usuário mais popular
    socialNet.encontrarUsuarioMaisPopular();

    // Sugerir amizade para um usuário
    socialNet.sugerirAmizade("Alice");

    return 0;
}

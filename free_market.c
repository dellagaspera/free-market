#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DADOS_USUARIO "usuariosTeste.bin"
#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    int nota;
} usuarios_t;

void util_removeQuebraLinhaFinal(char dados[]) {
    int tamanho = strlen(dados);
    if (tamanho > 0 && dados[tamanho - 1] == '\n') {
        dados[tamanho - 1] = '\0';
    }
}

void guardaUsuario(usuarios_t *usuario) {
    FILE *arquivo = fopen(DADOS_USUARIO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(usuario, sizeof(usuarios_t), 1, arquivo);//Guarda 1 usuario por vez
    fclose(arquivo);
}

void cadastraUsuario(usuarios_t *usuarios, int indice) {
    printf("Digite o nome do usuário: ");
    fgets(usuarios[indice].nome, MAX_NOME, stdin);
    util_removeQuebraLinhaFinal(usuarios[indice].nome);

    printf("Digite a nota do usuário: ");
    scanf("%d%*c", &usuarios[indice].nota);
}

void imprimeUsuarios() {
    FILE *arquivo = fopen(DADOS_USUARIO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    usuarios_t usuario;//Variavel local pra passar os dados pro arquivo, um por um
    while (fread(&usuario, sizeof(usuarios_t), 1, arquivo) == 1) {//Enquanto retornar 1(qntd retorna 1 é pq leu certo)
        printf("Nome: %s, Nota: %d\n", usuario.nome, usuario.nota);
    }

    fclose(arquivo);
}

int main() {
    int qntdUsuarios = 0;
    int opcao = -1;
    usuarios_t *usuarios = NULL;

    do {
        printf("Digite o que você quer fazer:\n");
        printf("1 - Cadastro\n2 - Imprimir usuários\n0 - Sair\n");
        scanf("%d%*c", &opcao);

        switch (opcao) {
            case 1: {
                
                usuarios = (usuarios_t *)realloc(usuarios, sizeof(usuarios_t) * (qntdUsuarios + 1));
                cadastraUsuario(usuarios, qntdUsuarios);
                

                guardaUsuario(&usuarios[qntdUsuarios]);//Guada o ultimo usuario cadastrado
                qntdUsuarios++;
                break;
            }

                case 2:
                imprimeUsuarios(usuarios, qntdUsuarios);
                break;


            case 0:
                free(usuarios);
                return 0;
        }
    } while (opcao != 0);

    return 0;
}

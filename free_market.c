#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DADOS_USUARIO "roblox.bin"

typedef struct usuarios_t {
    char nome[50];
    int nota;

} usuarios_t;

void util_removeQuebraLinhaFinal(char dados[]) {
int tamanho;
tamanho = strlen(dados);
if ((tamanho > 0) && (dados[tamanho - 1] == '\n')) {
dados[tamanho - 1] = '\0';
}
}

void guardaUsuario(usuarios_t *usuarios, int qntdUsuarios) {
    FILE *arquivo = fopen(DADOS_USUARIO, "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(usuarios, sizeof(usuarios_t), qntdUsuarios, arquivo);
    fclose(arquivo);
}

void cadastraUsuario(usuarios_t *usuarios, int indice) {
    
    printf("Digite o nome do usuário: ");
    fgets(usuarios[indice].nome, 50, stdin);
    util_removeQuebraLinhaFinal(usuarios[indice].nome);

    printf("Digite a nota do usuário: ");
    scanf("%d%*c", &usuarios[indice].nota);
}

int main() {
    int qntdUsuarios = 0;
    int opcao = -1;
    usuarios_t *usuarios =(usuarios_t*)malloc(sizeof(usuarios_t)*1);

    do {
        printf("Digite o que você quer fazer:\n");
        printf("1 - Cadastro\n0 - Sair\n");
        scanf("%d%*c", &opcao);

        switch (opcao) {

            case 1:
                qntdUsuarios++;
                usuarios = (usuarios_t *)realloc(usuarios, sizeof(usuarios_t) * qntdUsuarios+1);
                cadastraUsuario(usuarios, qntdUsuarios);
                guardaUsuario(usuarios, qntdUsuarios);
                break;

            case 0:
            free(usuarios);
                return 0;
        }
        
    } while (opcao != 0);

}


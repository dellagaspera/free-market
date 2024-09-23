    //  INCLUSOES  //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    //  CONSTANTES ARQUIVOS  //

#define ARQUIVO_USUARIOS "fm_usuarios.bin" // Arquivo onde estão armazendados os usuários
#define ARQUIVO_PRODUTOS "fm_produtos.bin" // Arquivo onde estão armazendados os produtos
#define ARQUIVO_AVALIACOES "fm_avaliacoes.bin" // Arquivo onde estão armazendados as avaliações

    //  CONSTANTES USUARIOS  //

#define CARACTERES_EMAIL (30 + 1) // Máximo de caracteres de um e-mail
#define CARACTERES_NOME_EXIBICAO (30 + 1) // Máximo de caracteres de um nome de exibição
#define CARACTERES_ID (20 + 1)  // Máximo de caracteres de um identificador
#define CARACTERES_SENHA (16 + 1)  // Máximo de caracteres de um asenha

    //  CONSTANTES PRODUTOS  //

#define CARACTERES_NOME_PRODUTO (30 + 1)  // Máximo de caracteres de um nome de produto
#define CARACTERES_DESCRICAO_PRODUTO (255 + 1)  // Máximo de caracteres de uma descrição de produto

    //  CONSTANTES AVALIACOES  //

#define CARACTERES_MENSAGEM (300 + 1)  // Máximo de caracteres de uma avaliação

    //  CONSTANTES RETORNO  //

#define SUCESSO 0
#define INEXISTENTE -1

    //  CONSTANTES GERAIS  //

#define true  (1!=0)
#define false (1==0)

    //  ESTRUTURAS  //
    
/* 
    #### Usuário
    - email : E-mail do usuário
    - senha : Senha do usuário
    - id: Identificador do usuário
    - nomeExibicao: Nome de exibição do usuário
*/
typedef struct usuario_e {
    char email[CARACTERES_EMAIL];
    char senha[CARACTERES_SENHA];
    char id[CARACTERES_ID];
    char nomeExibicao[CARACTERES_NOME_EXIBICAO];
} usuario_t;

/* 
    #### Produto
    - idVendedor : Identificador do vendedor
    - id : Identificador do produto
    - nome: Nome do produto
    - nota: Nota média do produto
*/
typedef struct produto_e {
    char idVendedor[CARACTERES_ID];
    char id[CARACTERES_ID];
    char nome[CARACTERES_NOME_PRODUTO];
    char descricao[CARACTERES_DESCRICAO_PRODUTO];
    int  nota;
} produto_t;

/*
    #### Avaliacao
    - idAvaliacao: Identificador da avaliação
    - idProduto: Identificador do produto avaliado
    - idComprador: Identificador do comprador que avaliou
    - mensagem: Mensagem da avaliação
    - nota: Nota dada para o produto
*/
typedef struct avaliacao_e {
    char idAvaliacao[CARACTERES_ID];
    char idProduto[CARACTERES_ID];
    char idComprador[CARACTERES_ID];
    char mensagem[CARACTERES_MENSAGEM];
    int  nota;
} avaliacao_t;

typedef unsigned int bool;

    //  FUNCOES  //

// Verifica se existe o e-mail digitado
// > true/false
bool emailExiste(char email[CARACTERES_EMAIL]) {
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    int nUsuarios;
    usuario_t * usuarios;
    fscanf(arquivoUsuarios,"%d", &nUsuarios);
    fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);

    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(email, usuarios[i].email)) {
            return true;
        }
    }

    return false;
}
// Remove a quebra de linha ('\n') no final de uma string
void removeQuebra(char string[]) {
    int tamanho = strlen(string);
    if(tamanho > 0) string[tamanho - 1] = '\0';
}

// Cadastra um novo usuário e o adiciona ao arquivo
void cadastrarUsuario() {
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "ab");
    fseek (arquivoUsuarios, 0, SEEK_SET);

    char email[CARACTERES_EMAIL];
    char senha[CARACTERES_SENHA];
    char id[CARACTERES_ID];
    char nomeExibicao[CARACTERES_NOME_EXIBICAO];

    do {
        printf("E-MAIL\n : ");
        fgets(email, CARACTERES_EMAIL, stdin);
        removeQuebra(email);
        if(emailExiste(email)) printf("E-MAIL JA CADASTRADO\n");
    } while(emailExiste(email));

    printf("SENHA\n : ");
    fgets(senha, CARACTERES_SENHA, stdin);
    usuario_t novoUsuario;

    fclose(arquivoUsuarios);
    return;
}


    //  MAIN  //

int main(int argc, char ** argv) {
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        FILE * arquivoUsuario = fopen(ARQUIVO_USUARIOS, "wb");
        int i = 0;
        fwrite(&i, sizeof(int), 1, arquivoUsuario);
        fclose(arquivoUsuario);
    }
    if(fopen(ARQUIVO_PRODUTOS, "rb") == NULL) fopen(ARQUIVO_PRODUTOS, "wb");
    if(fopen(ARQUIVO_AVALIACOES, "rb") == NULL) fopen(ARQUIVO_AVALIACOES, "wb");

    return SUCESSO;
}

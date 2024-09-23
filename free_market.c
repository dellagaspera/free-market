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

    //  CONSTANTES GERAIS  //

#define true  (1!=0)
#define false (1==0)

    //  ESTRUTURAS  //
    
/* 
    #### Usuário
    - email : E-mail do usuário
    - senha : Senha do usuário
    - nomeExibicao: Nome de exibição do usuário
    - id: Identificador do usuário
*/
typedef struct usuarios_e {
    char email[CARACTERES_EMAIL];
    char senha[CARACTERES_SENHA];
    char nomeExibicao[CARACTERES_NOME_EXIBICAO];
    char id[CARACTERES_ID];
} usuarios_t;

/* 
    #### Produto
    - idVendedor : Identificador do vendedor
    - id : Identificador do produto
    - nome: Nome do produto
    - nota: Nota média do produto
*/
typedef struct produtos_e {
    char idVendedor[CARACTERES_ID];
    char id[CARACTERES_ID];
    char nome[CARACTERES_NOME_PRODUTO];
    char descricao[CARACTERES_DESCRICAO_PRODUTO];
    int  nota;
} produtos_t;

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

    //  FUNCOES  //



    //   MAIN  //

int main(int argc, char ** argv) {

    return SUCESSO;
}

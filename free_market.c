//  INCLUSOES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  CONSTANTES ARQUIVOS
#define ARQUIVO_USUARIOS "fm_usuarios.bin"
#define ARQUIVO_PRODUTOS "fm_produtos.bin"
#define ARQUIVO_AVALIACOES "fm_avaliacoes.bin"
#define ARQUIVO_FAVORITOS "fm_favoritos.bin"

//  CONSTANTES USUARIOS
#define CARACTERES_EMAIL (30 + 1)
#define CARACTERES_NOME_EXIBICAO (30 + 1)
#define CARACTERES_ID (20 + 1)
#define CARACTERES_SENHA (16 + 1)

//  CONSTANTES PRODUTOS
#define CARACTERES_NOME_PRODUTO (30 + 1)
#define CARACTERES_DESCRICAO_PRODUTO

//  CONSTANTES AVALIACOES
#define CARACTERES_MENSAGEM (300 + 1)

//  CONSTANTES RETORNO
#define SUCESSO 0

//  CONSTANTES GERAIS
#define true  (1!=0)
#define false (1==0)

//  ESTRUTURAS
typedef struct usuarios_e {
    char email[CARACTERES_EMAIL];
    char senha[CARACTERES_SENHA];
    char nomeExibicao[CARACTERES_NOME_EXIBICAO];
    char id[CARACTERES_ID];
} usuarios_t;

typedef struct produtos_e {
    char idVendedor[CARACTERES_ID];
    char id[CARACTERES_ID];
    char nome[CARACTERES_NOME_PRODUTO];
    char descricao[CARACTERES_DESCRICAO_PRODUTO];
    int  nota;
}

typedef struct avaliacoes_e {
    char idAvaliacao[CARACTERES_ID];
    char idProduto[CARACTERES_ID];
    char idComprador[CARACTERES_ID];
    int  nota;
    char mensagem[CARACTERES_MENSAGEM];
} avaliacoes_t;

//  FUNCOES


//  MAIN
int main(int argc, char ** argv) {

    return SUCESSO;
}

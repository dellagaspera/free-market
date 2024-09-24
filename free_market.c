    //  INCLUSÕES  //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    //  CONSTANTES ARQUIVOS  //

#define ARQUIVO_USUARIOS "fm_usuarios.bin" // Arquivo onde estão armazendados os usuários
#define ARQUIVO_PRODUTOS "fm_produtos.bin" // Arquivo onde estão armazendados os produtos
#define ARQUIVO_AVALIACOES "fm_avaliacoes.bin" // Arquivo onde estão armazendados as avaliações

    //  CONSTANTES USUÁRIOS  //

#define CARACTERES_EMAIL (30 + 1) // Máximo de caracteres de um e-mail
#define CARACTERES_NOME_EXIBICAO (30 + 1) // Máximo de caracteres de um nome de exibição
#define CARACTERES_ID (20 + 1)  // Máximo de caracteres de um identificador
#define CARACTERES_SENHA (16 + 1)  // Máximo de caracteres de uma senha
#define MINIMO_CARACTERES_SENHA (16 + 1)  // Mínimo de caracteres de uma senha

    //  CONSTANTES PRODUTOS  //

#define CARACTERES_NOME_PRODUTO (30 + 1)  // Máximo de caracteres de um nome de produto
#define CARACTERES_DESCRICAO_PRODUTO (255 + 1)  // Máximo de caracteres de uma descrição de produto

    //  CONSTANTES AVALIAÇÕES  //

#define CARACTERES_MENSAGEM (300 + 1)  // Máximo de caracteres de uma avaliação

    //  CONSTANTES RETORNO  //

#define SUCESSO 0
#define INEXISTENTE -1

#define ERRO_SENHA_CARACTERES_INSUFICIENTES  1
#define ERRO_SENHA_SEM_NUMEROS               2
#define ERRO_SENHA_SEM_MAIUSCULAS            3

    //  CONSTANTES GERAIS  //

typedef unsigned int bool;
#define true 1;
#define false 0;

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

    //  FUNCOES  //

/* Verifica se existe o e-mail digitado
 - true/false
*/
bool emailExiste(char email[CARACTERES_EMAIL]) {
    // Abre o arquivo com os dados dos usuários
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    int nUsuarios;
    usuario_t * usuarios;

    // Pega o numero de usuários contidos no arquivo
    fscanf(arquivoUsuarios,"%d", &nUsuarios);
    // Pega os dados de todos os usuários contidos no arquivo
    fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);

    // Passa por todos os usuários, comparando o e-mail digitado para verificar se o e-mail já está cadastrado
    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(email, usuarios[i].email)) {
            return true;
        }
    }

    // Se chegou aqui, o e-mail não foi cadastrado anteriormente
    return false;
}

/* Verifica se existe o ID de usuário digitado
 - true/false
*/
bool idExiste(char id[CARACTERES_EMAIL]) {
    // Abre o arquivo com os dados dos usuários
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    int nUsuarios;
    usuario_t * usuarios;

    // Pega o numero de usuários contidos no arquivo
    fscanf(arquivoUsuarios,"%d", &nUsuarios);
    // Pega o dado de todos os usuários contidos no arquivo
    fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);

    // Passa por todos os usuários, comparando o ID digitado para verificar se o ID já está cadastrado
    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(id, usuarios[i].id)) {
            return true;
        }
    }

    return false;
}

/* Verifica se a senha é válida
 - ERRO_SENHA_CARACTERES_INSUFICIENTES 
 - ERRO_SENHA_SEM_NUMEROS
 - ERRO_SENHA_SEM_MAIUSCULAS
*/
int validaSenha(char senha[CARACTERES_SENHA]) {
    int tamanho = strlen(senha);
    int nNumeros = 0, nMaiusculas = 0;
    for(int i = 0; i < tamanho; i++) {
        if(senha[i] > '0' && senha[i] < '9') nNumeros++;
        if(senha[i] > 'A' && senha[i] < 'Z') nMaiusculas++;
    }

    if(tamanho < MINIMO_CARACTERES_SENHA) return ERRO_SENHA_CARACTERES_INSUFICIENTES;
    if(nNumeros < 1) return ERRO_SENHA_SEM_NUMEROS;
    if(nMaiusculas < 1) return ERRO_SENHA_SEM_MAIUSCULAS;

    return SUCESSO;
}

/* Verifica se o e-mail é válido (formatado como "#@#.#")
 - true/false
*/
bool validaEmail(char email[CARACTERES_EMAIL]) {
    int tamanho = strlen(email);

    for(int i = 1; i < tamanho; i++) {
        if(email[i] == '@') {
            for(int j = i + 2; j < tamanho; j++) {  
                if(email[j] == '.' && j + 1 < tamanho) {
                    return true;
                }
            }
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
        if(validaEmail(email)) printf("E-MAIL INVALIDO\n");
    } while(emailExiste(email) && !validaEmail(email));

    do {
        printf("SENHA\n : ");
        fgets(senha, CARACTERES_SENHA, stdin);
        removeQuebra(senha);
        if(validaSenha(senha) != SUCESSO) {
            switch (validaSenha(senha)) {
            case ERRO_SENHA_CARACTERES_INSUFICIENTES:
                printf(" - A SENHA DEVE CONTER AO MENOS %d CARACTERES\n", MINIMO_CARACTERES_SENHA);
            break;

            case ERRO_SENHA_SEM_MAIUSCULAS:
                printf(" - A SENHA DEVE CONTER AO MENOS 1 LETRA MAIUSCULA\n");
            break;

            case ERRO_SENHA_SEM_NUMEROS:
                printf(" - A SENHA DEVE CONTER AO MENOS 1 NYMERO\n");
            break;
            }
        }
    } while(validaSenha(senha) != SUCESSO);

    do {
        printf("NOME DE USUARIO\n : ");
        fgets(id, CARACTERES_ID, stdin);
        removeQuebra(id);
    } while(1);

    usuario_t novoUsuario = {
        .email = email,
        .senha = senha,
        .id = id,
        .nomeExibicao = nomeExibicao
    };

    fwrite(&novoUsuario, sizeof(usuario_t), 1, arquivoUsuarios);

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

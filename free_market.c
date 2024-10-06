    //  INCLUSÕES  //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    //  CONSTANTES ARQUIVOS  //

#define ARQUIVO_USUARIOS "fm_usuarios.bin" // Arquivo onde estão armazendados os usuários
#define ARQUIVO_PRODUTOS "fm_produtos.bin" // Arquivo onde estão armazendados os produtos
#define ARQUIVO_AVALIACOES "fm_avaliacoes.bin" // Arquivo onde estão armazendados as avaliações

    //  CONSTANTES USUÁRIOS  //

#define CARACTERES_EMAIL (40 + 1) // Máximo de caracteres de um e-mail
#define CARACTERES_NOME_EXIBICAO (40 + 1) // Máximo de caracteres de um nome de exibição
#define CARACTERES_ID (30 + 1)  // Máximo de caracteres de um identificador
#define CARACTERES_SENHA (30 + 1)  // Máximo de caracteres de uma senha
#define MINIMO_CARACTERES_SENHA (8)  // Mínimo de caracteres de uma senha
#define MINIMO_MAIUSCULAS_SENHA (1)  // Mínimo de caracteres de uma senha
#define MINIMO_NUMEROS_SENHA (1)  // Mínimo de caracteres de uma senha

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
#define true 1
#define false 0

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
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        printf("Erro na abertura do arquivo\n");
        return true;
    }
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    usuario_t * usuarios;
    int nUsuarios;

    fread(&nUsuarios, sizeof(int), 1, arquivoUsuarios);
    usuarios = malloc(sizeof(usuario_t) * nUsuarios);
    if(nUsuarios > 0) {
        fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);
        if(usuarios == NULL) printf("Erro ao ler dados do arquivo\n");
        fclose(arquivoUsuarios);
    } else return false;

    // Passa por todos os usuários, comparando o e-mail digitado para verificar se o e-mail já está cadastrado
    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(email, usuarios[i].email) == 0) {
            return true;
        }
    }

    // Se chegou aqui, o e-mail não foi cadastrado anteriormente
    return false;
}

/* Verifica se existe o ID de usuário digitado
 - true/false
*/
bool idExiste(char id[CARACTERES_ID]) {
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        printf("Erro na abertura do arquivo\n");
        return false;
    }
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    usuario_t * usuarios;
    int nUsuarios;

    fread(&nUsuarios, sizeof(int), 1, arquivoUsuarios);
    usuarios = malloc(sizeof(usuario_t) * nUsuarios);
    if(nUsuarios > 0) {
        fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);
        if(usuarios == NULL) printf("Erro ao ler dados do arquivo\n");
        fclose(arquivoUsuarios);
    } else return false;

    // Passa por todos os usuários, comparando o ID digitado para verificar se o ID já está cadastrado
    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(id, usuarios[i].id) == 0) {
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
    if(nNumeros < MINIMO_NUMEROS_SENHA) return ERRO_SENHA_SEM_NUMEROS;
    if(nMaiusculas < MINIMO_MAIUSCULAS_SENHA) return ERRO_SENHA_SEM_MAIUSCULAS;

    return SUCESSO;
}

/* Verifica se o e-mail é válido (formatado como "#@#.#")
 - true/false
*/
bool validaEmail(char email[CARACTERES_EMAIL]) {
    int tamanho = strlen(email);
    int nArrobas = 0;

    // Verifica cada caractere do e-mail
    for(int i = 0; i < tamanho; i++) {
        // Conta a quantidade de arrobas no e-mail
        if(email[i] == '@') nArrobas++;
        // Verifica se após um ponto não há outro ponto ou um espaço vazio
        if(email[i] == '.' && (email[i + 1] == '.' || email[i + 1] == ' ')) return false;
    }
    // Se houver mais de um @ retona falso
    if(nArrobas != 1) return false;

    for(int i = 1; i < tamanho; i++) {
        if(email[i] == '@') {
            for(int j = i + 2; j < tamanho; j++) {  
                // Verifica se após um ponto não há outro ponto ou um espaço vazio
                if(email[i] == '.' && (email[i + 1] == '.' || email[i + 1] == ' ')) return false;
            }
        }
    }

    return true;
} 

// Remove a quebra de linha ('\n') no final de uma string
void removeQuebra(char string[]) {
    int tamanho = strlen(string);
    if(tamanho > 0) string[tamanho - 1] = '\0';
    return;
}

// Cadastra um novo usuário e o adiciona ao arquivo
void cadastrarUsuario() {
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    usuario_t * usuarios;
    int nUsuarios;

    fread(&nUsuarios, sizeof(int), 1, arquivoUsuarios);
    usuarios = malloc(sizeof(usuario_t) * nUsuarios);
    if(nUsuarios > 0) {
        fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);
        if(usuarios == NULL) printf("Erro ao ler dados do arquivo\n");
    }
    fclose(arquivoUsuarios);

    nUsuarios++;
    usuarios = realloc(usuarios, sizeof(usuario_t) * nUsuarios);
    if(usuarios == NULL) printf("Erro ao realocar a memoria\n");
    char email[CARACTERES_EMAIL];
    char senha[CARACTERES_SENHA];
    char id[CARACTERES_ID];
    char nomeExibicao[CARACTERES_NOME_EXIBICAO];

    // Lê o e-mail digitado até ser um e-mail válido
    do {
        printf("E-MAIL\n");
        printf(" : ");
        fgets(email, CARACTERES_EMAIL, stdin);
        removeQuebra(email);
        if(emailExiste(email) == true) printf("E-MAIL JA CADASTRADO\n");
        if(validaEmail(email) == false) printf("E-MAIL INVALIDO\n");
    } while(emailExiste(email) == true || validaEmail(email) == false);

    // Lê a senha digitada até ser uma senha válida
    do {
        printf("SENHA\n");
        printf("deve conter:\n");
        printf(" - %d caracteres;\n", MINIMO_CARACTERES_SENHA);
        printf(" - %d letras maiusculas;\n", MINIMO_MAIUSCULAS_SENHA);
        printf(" - %d numeros;\n", MINIMO_NUMEROS_SENHA);
        printf(" : ");
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

    // Lê o identificador do usuário
    do {
        printf("NOME DE USUARIO\n");
        printf(" : ");
        fgets(id, CARACTERES_ID, stdin);
        removeQuebra(id);
        if(idExiste(id) == true) printf("NOME DE USUARIO JA CADASTRADO\n");
    } while(idExiste(id) == true);

    strcpy(usuarios[nUsuarios - 1].email, email);
    strcpy(usuarios[nUsuarios - 1].id, id);
    strcpy(usuarios[nUsuarios - 1].nomeExibicao, nomeExibicao);
    strcpy(usuarios[nUsuarios - 1].senha, senha);

    arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "wb");
    if(arquivoUsuarios == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }
    fwrite(&nUsuarios, sizeof(int), 1, arquivoUsuarios);
    fwrite(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);
    fclose(arquivoUsuarios);

    return;
}

// Lista todos os usuários cadastrados
void listaUsuarios() {
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        printf("Erro na abertura do arquivo\n");
        return;
    }
    FILE * arquivoUsuarios = fopen(ARQUIVO_USUARIOS, "rb");
    usuario_t * usuarios;
    int nUsuarios;

    fread(&nUsuarios, sizeof(int), 1, arquivoUsuarios);
    usuarios = malloc(sizeof(usuario_t) * nUsuarios);

    fread(usuarios, sizeof(usuario_t), nUsuarios, arquivoUsuarios);
    fclose(arquivoUsuarios);

    printf("Numero de usuarios cadastrado: %d\n", nUsuarios);

    printf("%-*s\t", CARACTERES_EMAIL, "E-mail");
    printf("%-*s\t", CARACTERES_SENHA, "Senha");
    printf("%-*s\n", CARACTERES_ID, "Nome de Usuario");
    for(int i = 0; i < nUsuarios; i++) {
        printf("%-*s\t", CARACTERES_EMAIL, usuarios[i].email);
        printf("%-*s\t", CARACTERES_SENHA, usuarios[i].senha);
        printf("%-*s\n", CARACTERES_ID, usuarios[i].id);
    }
}

    //  MAIN  //

int main(int argc, char ** argv) {
    // Tenta abrir o arquivo com os usuários, se não existir, cria
    if(fopen(ARQUIVO_USUARIOS, "rb") == NULL) {
        FILE * arquivoUsuario = fopen(ARQUIVO_USUARIOS, "wb");
        int nUsuarios = 0;
        fwrite(&nUsuarios, sizeof(int), 1, arquivoUsuario);
        fclose(arquivoUsuario);
    }
    // Tenta abrir o arquivo com os produtos, se não existir, cria
    if(fopen(ARQUIVO_PRODUTOS, "rb") == NULL) fopen(ARQUIVO_PRODUTOS, "wb");
    // Tenta abrir o arquivo com as avaliações, se não existir, cria
    if(fopen(ARQUIVO_AVALIACOES, "rb") == NULL) fopen(ARQUIVO_AVALIACOES, "wb");

    int escolha;
    do {
        printf("1 - Cadastrar usuario\n");
        printf("2 - Listar usuarios\n");
        printf("0 - Sair\n");
        // printf(" - \n");
        printf(" : ");
        scanf("%d%*c", &escolha);

        switch(escolha) {
            case 1:
            cadastrarUsuario();
            break;

            case 2:
            listaUsuarios();
            break;
        }

    } while(escolha != 0);

    return SUCESSO;
}

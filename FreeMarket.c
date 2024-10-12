// Inclusões
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Constantes
#define MAX_CHAR_NOME   (30 + 1) // Tamanho máximo do nome do usuário
#define MAX_CHAR_EMAIL  (30 + 1)
#define MAX_CHAR_SENHA  (20 + 1)
#define MAX_CHAR_USUARIO (20 + 1)
#define MAX_CHAR_ID     (20 + 1)

#define MAX_CHAR_NOME_PRODUTO (40 + 1)
#define MAX_CHAR_DESCRICAO    (128 + 1)

#define USUARIO_SEM_LOGIN -1

#define SIM 1
#define NAO 0

#define MENU_PRI_OPCAO_MIN 0    // Opção mínima que o usuário pode escolher no menu principal
#define MENU_PRI_OPCAO_MAX 14   // Opção máxima que o usuário pode escolher no menu principal

// Arquivos
#define ARQUIVO_USUARIOS "usuarios_freemarket"

// Saídas 
#define SUCESSO 0

#define ERRO_BUSCAR_SEM_CORRESPONDENTE -1


// bool EMANUEL_RECUPERACAO = true;


typedef struct avaliacao_s {

    // Informações de quem mandou
    char autorMensagem[MAX_CHAR_USUARIO];
    char autorMensagemID[MAX_CHAR_ID];

    // Avaliação
    int nota; // 0 a 5 (estrelas)
    char  mensagem[MAX_CHAR_DESCRICAO];

} avaliacao_t;


// Estrutura do produto
typedef struct produto_s { //obs: a chave é do lado do nome! nao é! é sim!

    // Informações básicas do produto
    char nomeProduto[MAX_CHAR_NOME_PRODUTO];
    char descricaoProduto[MAX_CHAR_DESCRICAO];

    int Id;
    char idDono[MAX_CHAR_ID];
    int nImagens;
    char * imagens;
    
    int estoque;
    int nAvaliacoes;
    avaliacao_t * avaliacoes; 
    
} produto_t;

// Estrutura do usuário
typedef struct usuario_s
{
    char ID[MAX_CHAR_ID]; 
    char nome[MAX_CHAR_NOME];
    char email[MAX_CHAR_EMAIL];
    char senha[MAX_CHAR_SENHA];
    produto_t * produtos;
    int nProdutos;

    int * produtosFavoritos;
    int nFavoritos;

    
} usuario_t;

// Tipo para quando retornar códigos de erro
typedef int erro_t;

// Para o usuário conseguir ler e depois dar "enter"
// para continuar o programa e conseguir ler tudo.
void esperar_apertarEnter() {
    printf("\n\nAperte enter para continuar.");
    scanf("%*c");
}

// Remove a quebra de linha de uma string
void removeQuebra(char string[])
{
    int tamanho = strlen(string);
    if(tamanho > 0 && string[tamanho - 1] == '\n') string[tamanho - 1] = '\0';
}

// Realoca o vetor imagens para ter o tamanho <novoTamanho>
char * alocarVetorImagens(char * imagens, int novoTamanho) {
    return (char*)realloc(imagens, novoTamanho * sizeof(char));
}

// Função para listar os produtos de um usuário, e escolher um produto, retornando seu indice
int escolherProdutoComId(usuario_t usuario)
{
    if(usuario.nProdutos == 0) return ERRO_BUSCAR_SEM_CORRESPONDENTE;

    int idDigitado;

    //Faz um loop e imprime todos os usuarios pra voce selecionar
    //Faz um loop por todos dnv e vc digita um nome(dps muda pra ID) e retorna o indice dele

    printf("ID\t| Nome\n");

    // Loop em todos os produtos do usuário e printa o id e o nome do produto
    for(int i = 0; i < usuario.nProdutos; i++) {
        printf("%02d\t  %s\n", usuario.produtos[i].Id, usuario.produtos[i].nomeProduto);
    }

    // Pede o id
    printf("Digite o id do produto: ");
    scanf("%i%*c", &idDigitado);

    // Loop até achar o produto com esse id
    for(int i = 0; i < usuario.nProdutos; i++) {

        if(usuario.produtos[i].Id == idDigitado) {
            // O id do produto é igual ao id digitado pelo usuário.
            return i;
        }
    }
    
    // Se chegou aqui, o usuário digitou um ID que não existe
    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}


// Função para listar os produtos de um usuário, e escolher um produto, retornando seu indice
int escolherProduto(usuario_t usuario)
{
    if(usuario.nProdutos == 0) return ERRO_BUSCAR_SEM_CORRESPONDENTE;

    //Faz um loop e imprime todos os usuarios pra voce selecionar
    //Faz um loop por todos dnv e vc digita um nome(dps muda pra ID) e retorna o indice dele

    printf("ID\t| Nome\n");
    for(int i = 0; i < usuario.nProdutos; i++) {

        printf("%02d\t  %s\n", i, usuario.produtos[i].nomeProduto);//Muda esse %d dps pra %s, ai vai ser o id seguido do nome, assim-> id - nome
    }

    char tempVar[MAX_CHAR_ID];
    printf("Digite o id do produto:");
    fgets(tempVar, MAX_CHAR_ID, stdin);
    removeQuebra(tempVar);

    for(int i = 0; i < usuario.nProdutos; i++) {

        if(strcmp(tempVar, usuario.produtos[i].nomeProduto) == 0) {
            return i;
        }
    }
}

int buscaUsuario(usuario_t *usuario, int nUsuarios) {
    
    //Faz um loop e imprime todos os usuarios pra voce selecionar;
    //Depois, faz um loop por todos dnv e vc digita um id e retorna o indice dele

    char tempVar[MAX_CHAR_ID];

    //
    printf("%-20s| %-30s\n", "ID", "Nome");
    for(int i = 0; i < nUsuarios; i++) {
        printf("%-20s  %-30s\n", usuario[i].ID, usuario[i].nome);
    }

    // Pede o id que o usuario quer
    printf("Digite o ID do usuario: ");
    fgets(tempVar, MAX_CHAR_ID, stdin);
    removeQuebra(tempVar);

    // Loop em todos os usuários até achar um ID que seja igual ao digitado
    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(tempVar, usuario[i].ID) == 0) {
            return i;
        }
    }

    // Se não achar um usuário com tal id
    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}

void listarProdutos(usuario_t *usuario, int nUsuarios) {
    
    int indiceUsuario = buscaUsuario(usuario, nUsuarios);
    
    if (indiceUsuario == -1) {
        return; // Retorna se o usuário não foi encontrado
    }

    // Verifica se o usuário tem produtos
    if (usuario[indiceUsuario].nProdutos == 0) {
        printf("Nenhum produto cadastrado para o usuario %s.\n", usuario[indiceUsuario].nome);
        return;
    }

    // Lista os produtos do usuário
    printf("%-2s | %-30s\n", "ID", "Nome");
    for (int i = 0; i < usuario[indiceUsuario].nProdutos; i++) {
        printf("%02d   %s\n", i, usuario[indiceUsuario].produtos[i].nomeProduto);
    }

    esperar_apertarEnter();
}

void compraProduto(usuario_t *usuario, int nUsuarios) {

    int indiceUsuario = buscaUsuario(usuario, nUsuarios);
    int indiceProduto = escolherProdutoComId(*usuario);

    int nUnidades;

    if (indiceUsuario == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }

    if (usuario[indiceUsuario].produtos[indiceProduto].estoque <= 0) {
        printf("PRODUTO SEM ESTOQUE\n");
        return;
    } else {
        printf("ESTOQUE PRODUTO: %d\n", usuario[indiceUsuario].produtos[indiceProduto].estoque);
    }

    do {
        printf("Numero de unidades: ");
        scanf("%d%*c", &nUnidades);

        if (nUnidades <= usuario[indiceUsuario].produtos[indiceProduto].estoque) {
            usuario[indiceUsuario].produtos[indiceProduto].estoque -= nUnidades;
            printf("PRODUTO COMPRADO COM SUCESSO\n");
            return;
        } else {
            printf("NAO HA UNIDADES SUFICIENTES\n");
            printf("1 - Tentar novamente\n");
            printf("0 - Voltar\n: ");
            int escolha;
            scanf("%d%*c", &escolha);
            if (escolha == 0) return;
        }
    } while (usuario[indiceUsuario].produtos[indiceProduto].estoque);
    
    esperar_apertarEnter();
}


void apagarProduto(usuario_t * usuario) {
    int indiceProduto = escolherProduto(* usuario);
    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE)
    {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }

    for (int i = indiceProduto; i < (* usuario).nProdutos - 1; i++) {
        usuario->produtos[i] = usuario->produtos[i + 1];        
    }

    (usuario->nProdutos)--;
    usuario->produtos = (produto_t *)realloc(usuario->produtos, sizeof(produto_t) * (usuario->nProdutos));

    printf("O PRODUTO FOI REMOVIDO COM SUCESSO\n");
}
// Cadastra um produto novo em um usuário
void cadastraProduto(usuario_t * usuario, produto_t ** PRODUTOS, int * nProdutos) { 

    //Se não estiver aloca, aloca
    if (usuario->produtos == NULL) {
        usuario->produtos = malloc(sizeof(produto_t));
        if (usuario->produtos == NULL) {
            perror("Nao foi possivel alocar o vetor PRODUTOS");
            return;
        }
//Verifica novamente se alocou, caso positivo, realoca. Caso negativo, aloca e entra na condição positiva
    } else if ((usuario->produtos = realloc(usuario->produtos, sizeof(produto_t) * (usuario->nProdutos + 1))) == NULL) {
        return;
    }

    // Pede o nome do produto
    printf("Nome produto: ");
    fgets(usuario->produtos[usuario->nProdutos].nomeProduto, MAX_CHAR_NOME_PRODUTO, stdin);
    removeQuebra(usuario->produtos[usuario->nProdutos].nomeProduto);

    // Pede a descrição do produto
    printf("Descricao produto: ");
    fgets(usuario->produtos[usuario->nProdutos].descricaoProduto, MAX_CHAR_DESCRICAO, stdin);
    removeQuebra(usuario->produtos[usuario->nProdutos].descricaoProduto);
    
    //Imagens fica pra dps

    // Pede o estoque do produto
    printf("Estoque produto: ");
    scanf("%d%*c", &usuario->produtos[usuario->nProdutos].estoque);

    // Coloca o ID do produto como a quantidade de produtos já criados
    usuario->produtos[usuario->nProdutos].Id = *nProdutos;
    
    // Coloca o ID do dono do produto como o usuário logado agora
    strcpy(usuario->produtos[usuario->nProdutos].idDono, usuario->ID);

    // Aloca para o novo produto & Seta para o produto que acabou de criar
    *PRODUTOS = realloc(*PRODUTOS, sizeof(produto_t) * (*nProdutos + 1));
    
    if ((*PRODUTOS) == NULL) {
        perror("Nao foi possivel realocar o vetor PRODUTOS");
        printf("Falha na criacao do produto.\n");
        return;
    }
    
    usuario->produtos[usuario->nProdutos].nAvaliacoes = 0;
    usuario->produtos[usuario->nProdutos].avaliacoes = NULL;
    
    (*PRODUTOS)[*nProdutos] = usuario->produtos[usuario->nProdutos];

    // Implementa a quantidade de produtos do usuário e de produtos totais
    (usuario->nProdutos)++;
    (*nProdutos)++;
}

int validaEmail(char email[])
{
    int tamanho = strlen(email);
    for(int i = 1; i < tamanho; i++)
    {
        if(email[i] == '@') 
        {
            for(int o = i + 1; o < tamanho; o++)
            {
                if(email[o] == '.' && o != tamanho)
                {
                    return SIM;
                }
            }
        }
    }

    return NAO;
}

// Busca um usuário cujo ID (usuario.ID) seja igual ao "id" enviado e retorna o seu índice.
int buscarUsuarioPorId(usuario_t *usuarios, int nUsuarios, char Id[MAX_CHAR_ID]) 
{
    // Loop por todos os usuários
    for(int i = 0; i < nUsuarios; i++) 
    {
        if(strcmp(usuarios[i].ID, Id) == 0) return i; // Se o ID do usuário for igual ao ID procurado, retorna.
    }

    // Se não achou nenhum com tal id
    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}


// Retorna o índice de um usuário com o email "_email"
int buscarUsuarioPorEmail(usuario_t *usuarios, int nUsuarios, char _email[MAX_CHAR_EMAIL]) 
{
    for(int i = 0; i < nUsuarios; i++) 
    {
        if(strcmp(usuarios[i].email, _email) == 0) return i;
    }

    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}


int verificaID(char ID_[], int nUsuarios, usuario_t *usuario) {

    for(int i = 0; i < nUsuarios; i++) {
        if(strcmp(ID_, usuario[i].ID) == 0) {
            printf("ID já existente\n");
            return NAO;
        }
    }
    return SIM;
}

// Cadastra o usuario com um usuario unico e uma senha
void cadastraUsuario(usuario_t ** usuarios, int * nUsuarios)
{
    char email_[MAX_CHAR_EMAIL];
    char senha_[MAX_CHAR_SENHA];
    char ID_[MAX_CHAR_ID];
    * usuarios = realloc(*usuarios, sizeof(usuario_t) * (* nUsuarios + 1));

    // Recebe um e-mail e verifica se ja esta cadastrado
    do
    {
        printf("E-Mail\n : ");
        fgets(email_, MAX_CHAR_EMAIL, stdin);
        removeQuebra(email_);
        if(validaEmail(email_) == NAO) printf("E-MAIL INVALIDO\n");
        if(buscarUsuarioPorEmail(* usuarios, * nUsuarios, email_) != ERRO_BUSCAR_SEM_CORRESPONDENTE) printf("E-MAIL JA UTILIZADO\n");
    } while(buscarUsuarioPorEmail(* usuarios, * nUsuarios, email_) != ERRO_BUSCAR_SEM_CORRESPONDENTE || validaEmail(email_) == NAO);//Se digitar certo sai do loop
    strcpy((* usuarios)[* nUsuarios].email, email_);

    // Cadastra a senha
    printf("Senha: ");
    fgets((* usuarios)[* nUsuarios].senha, MAX_CHAR_SENHA, stdin);
    removeQuebra((* usuarios)[* nUsuarios].senha);
    
    //Cadastra o ID
    do {
        printf("ID: ");
        fgets(ID_, MAX_CHAR_ID, stdin);
        removeQuebra(ID_);
    } while (verificaID(ID_, *nUsuarios, *usuarios) == NAO);

    strcpy((*usuarios)[*nUsuarios].ID, ID_);
    
    //Cadastra o nome
    printf("Nome de Exibicao\n : ");
    fgets((* usuarios)[* nUsuarios].nome, MAX_CHAR_NOME, stdin);
    removeQuebra((* usuarios)[* nUsuarios].nome);
    
    

    (* usuarios)[* nUsuarios].nProdutos = 0; // Configura o número de produtos vinculados ao usuário
    (* usuarios)[* nUsuarios].produtos = NULL; // Configura o vetor de produtos vinculados ao usuário

    (* usuarios)[* nUsuarios].nFavoritos = 0; // Configura o número de produtos vinculados ao usuário
    (* usuarios)[* nUsuarios].produtosFavoritos = NULL; // Configura o vetor de produtos vinculados ao usuário

    // Incrementa o contador de usuarios
    (* nUsuarios)++;

}

// Faz o login do usuario usando o login e a senha
void loginUsuario(int * loginAtual, usuario_t * usuarios, int nUsuarios)
{
    char email_[MAX_CHAR_EMAIL];
    char senha_[MAX_CHAR_SENHA];

    int indiceUsuario;

    if(nUsuarios == 0)
    {
        printf("NENHUM USUARIO FOI CADASTRADO AINDA. CRIE UM CADASTRO E DEPOIS RETORNE\n");
        return;
    }

    // Procura um usuario para fazer o login
    do
    {
        // Pede o nome do usuário
        printf("E-mail\n : ");
        fgets(email_, MAX_CHAR_EMAIL, stdin);
        removeQuebra(email_);

        if(buscarUsuarioPorEmail(usuarios, nUsuarios, email_) == ERRO_BUSCAR_SEM_CORRESPONDENTE)
        {
            printf("E-MAIL INVALIDO\n1 - Voltar ao menu inicial\n2 - Tentar novamente\n : "); //Verifica se o usuario existe
            int escolha;
            scanf("%d%*c", &escolha);
            if(escolha == 1) return;
        }
        else indiceUsuario = buscarUsuarioPorEmail(usuarios, nUsuarios, email_); //Pega o indice retornado pela funcao

    }while(buscarUsuarioPorEmail(usuarios, nUsuarios, email_) == ERRO_BUSCAR_SEM_CORRESPONDENTE);

    if (indiceUsuario != -1) { 
        // Pessoa não quis sair da tela de login. Portanto, digitou um nome válido
        do
        {
            printf("Senha: ");
            fgets(senha_, MAX_CHAR_SENHA, stdin);
            removeQuebra(senha_);
            // Compara a senha digitada com a do usuario
            if(strcmp(usuarios[indiceUsuario].senha, senha_) != 0) printf("SENHA INVALIDA\n");
        } while(strcmp(usuarios[indiceUsuario].senha, senha_) != 0);

        // Define o login atual
        *loginAtual = indiceUsuario;
        //O login do usario agora e seu indice
    }
}

// Lista os usuarios para verificar se o cadastro foi feito corretamente
void listaUsuarios(usuario_t * usuarios, int nUsuarios)
{
    for(int i = 0; i < nUsuarios; i++)
    {
        printf("%d/%d - %s\t%s\n", i + 1, nUsuarios, usuarios[i].ID, usuarios[i].senha);
    }
}

void editarImagemProduto(produto_t * produto) {

}

void editarProduto(usuario_t *usuario) {
    
    // indice do produto selecionado pelo usuário
    int indiceProduto = escolherProduto(* usuario);

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE)
    {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }

    int escolha; // o que a pessoa escolheu editar
    int temp_val = -1;
    char temp_sinal;
    
    produto_t * _produto = &usuario->produtos[indiceProduto];
    // endereço do produto

    do {
        // Pergunta o que quer editar
        printf("\nO que deseja editar?\n");
        printf("\t 1 - Nome\n");
        printf("\t 2 - Descricao\n");
        printf("\t 3 - Imagens\n");
        printf("\t 4 - Estoque\n");
        printf("\t 0 - Sair\n");
        printf("Escolha: ");
        scanf("%i%*c", &escolha);

        // Casos de escolha
        switch (escolha) {
        
            case 1: // nome
                printf("Nome atual %s\n", _produto->nomeProduto);
                printf("Novo nome\n : ");
                fgets(_produto->nomeProduto, MAX_CHAR_NOME_PRODUTO, stdin);
                removeQuebra(_produto->nomeProduto);

                printf("NOME EDITADO COM SUCESSO\n");

                break;

            case 2: // Descrição
                printf("Descricao atual:\n %s\n", _produto->descricaoProduto);
                printf("Digite a nova descricao do produto\n : ");
                fgets(_produto->descricaoProduto, MAX_CHAR_DESCRICAO, stdin);
                removeQuebra(_produto->descricaoProduto);

                printf("DESCRICAO EDITADA COM SUCESSO\n");
            
                break;

            case 3: // Imagem
                editarImagemProduto(_produto);
            
                break;

            case 4: // Estoque
                // Pede a ação pro usuário
                printf("Estoque atual: %d\n", usuario->produtos[indiceProduto].estoque);
                // printf("Digite apenas um numero para ser o novo estoque do produto\n");
                // printf("Digite (A) para adicionar ou (R) para remover N produtos\n(Ex.: R 1 para retirar 1\t 4 para o novo estoque ser 4)\n");
                printf("Novo estoque\n : ");
                
                if (scanf(" %d", &temp_val) >= 0) {
                    // Se tiver digitado apenas número, já coloca o novo valor
                    usuario->produtos[indiceProduto].estoque = temp_val; 
                }/* else {
                    // Se digitou um caractere, vê qual foi e em função dele, executa algo
                    
                    scanf("%c", &temp_sinal); // Lê o caractere q digitou

                    if (temp_sinal == 'A' || temp_sinal == 'a') {
                        // Adiciona o valor que usuário digitou pro estoque
                        scanf("%i", &temp_val); // Lê o numero

                        _produto->estoque += temp_val;

                        printf("Adicionado %i ao estoque. Novo estoque: %i\n", temp_val, _produto->estoque);
                    } else if (temp_sinal == 'R' || temp_sinal == 'r') {
                        // Remove o valor que usuário digitou pro estoque
                        scanf("%i", &temp_val); // Lê o numero

                        _produto->estoque -= temp_val;

                        // Caso foi retirado mais do que deveria
                        if (_produto-> estoque < 0) {
                            printf("Foram retirados mais do que havia.\nO novo estoque é 0");
                        } else printf("Removendo %i ao estoque. Novo estoque: %i\n", temp_val, _produto->estoque);

                    } else printf("Operacao invalida!");
                }
                */
                break;

                case 5:
                    printf("Em breve :)\n");
                break;

                default: // parar de editar (digitou 0)
                break;
        }
    } while (escolha != 0);
}

//Função para fazer avaliação de um produto
void fazerAvaliacao(usuario_t *usuarios, int nUsuarios, int loginAtual) {

    
    int indiceUsuario = buscaUsuario(usuarios, nUsuarios);
       
    int indiceProduto = escolherProdutoComId(usuarios[indiceUsuario]);

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("NAO HA PRODUTOS CADASTRADOS\n");            
        return;
    }

    produto_t * produtoAtual = &(usuarios[indiceUsuario].produtos[indiceProduto]);
    
    produtoAtual->avaliacoes = (avaliacao_t *)realloc(produtoAtual->avaliacoes, sizeof(avaliacao_t) * (produtoAtual->nAvaliacoes + 1));
    
        
    int nota=0;
    char avaliaTemp[MAX_CHAR_DESCRICAO];
   
    printf("Avalie o produto com as suas palavras: ");
    fgets(avaliaTemp, MAX_CHAR_DESCRICAO, stdin);
    removeQuebra(avaliaTemp);
    
    printf("Digite uma nota inteira entre 0 e 5: ");
    scanf("%d%*c", &nota);

    if (nota < 0 || nota > 5) {
        printf("Nota invalida. Deve ser entre 0 e 5.\n");
        return;
    }

    // Seta os valores da avaliação
    strcpy(produtoAtual->avaliacoes[produtoAtual->nAvaliacoes].autorMensagem, usuarios[loginAtual].nome);
    strcpy(produtoAtual->avaliacoes[produtoAtual->nAvaliacoes].autorMensagemID, usuarios[loginAtual].ID);
    produtoAtual->avaliacoes[produtoAtual->nAvaliacoes].nota = nota;
    strcpy(produtoAtual->avaliacoes[produtoAtual->nAvaliacoes].mensagem, avaliaTemp);

    produtoAtual->nAvaliacoes++;
}

// Função para listar as avaliações de um produto
void listarAvaliacoes(usuario_t *usuarios, int nUsuarios) {

    int indiceUsuario = buscaUsuario(usuarios, nUsuarios);
    
    if (indiceUsuario == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    int indiceProduto = escolherProdutoComId(usuarios[indiceUsuario]);
    if (indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("Produto nao encontrado.\n");
        return;
    }

    // Declare uma variável produto_t para armazenar o produto
    produto_t produto = usuarios[indiceUsuario].produtos[indiceProduto];

    if (produto.nAvaliacoes == 0) {
        printf("Nao ha avaliacoes para este produto.\n");
        return;
    }

    
    printf("\nAvaliacoes para o produto %s:\n\n", produto.nomeProduto);
    for (int i = 0; i < produto.nAvaliacoes; i++) {
        printf("De: %s\nNota: %i/5\nAvaliacao: %s\n\n", produto.avaliacoes[i].autorMensagem, produto.avaliacoes[i].nota, produto.avaliacoes[i].mensagem);
    }
    
    esperar_apertarEnter();
} 

// Favorita um produto do usuário "usuarios[usuarioAtual]"
erro_t favoritarProduto(usuario_t *usuarios, int nUsuarios, int idUsuarioAtual) {
    
    int indiceUsuario, indiceProduto;
    usuario_t * usuarioAtual = &usuarios[idUsuarioAtual];

    // Pede para escolher um usuário
    indiceUsuario = buscaUsuario(usuarios, nUsuarios);

    printf("\n"); // Estética

    // Pede para escolher um produto do usuário já escolhido
    indiceProduto = escolherProdutoComId(usuarios[indiceUsuario]);

    printf("\n"); // Estética

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("NAO HA PRODUTOS CADASTRADOS\n");            
        return ERRO_BUSCAR_SEM_CORRESPONDENTE;
    }

    // emanuel, eu corriji o erro em 8 segunos dps q olhmacacohei ele.
    // me copia nisso 😍😋😁😉⬛🥛🥛🤢🍆🥺💕➗🐱‍👤😘😘📄✏👩‍🦰👩‍🦰
    // imagina o leandro vendo isso
    // bora deixar só pro leandro ver a gnt conversando e nos dar 0?
    // vira homem
    // vou compilar e ver se funcionou
    //elite vou fechar o discord, crashou msm
    //meu pc crashou
    // spammei emoji
    //voltei
    
    // Aloca & Coloca o produto escolhido no vetor "produtosFavoritados" do usuário atual
    int *novoFavoritos = realloc(usuarioAtual->produtosFavoritos, (usuarioAtual->nFavoritos + 1) * sizeof(int));
    //Cria uma variável temporária para realocar o tamanho de produtosFavoritos, evitando erros de realocação em uma posição inexistente

    if (novoFavoritos == NULL) {
        printf("Erro ao alocar memória para produtos favoritos.\n");
        return 1; // Defina um erro apropriado
    }

    usuarioAtual->produtosFavoritos = novoFavoritos;//produtosFavoritos recebe a realocação de novosFavoritos

    usuarioAtual->produtosFavoritos[usuarioAtual->nFavoritos] = usuarios[indiceUsuario].produtos[indiceProduto].Id;
    usuarioAtual->nFavoritos++;
    printf("Produto %s favoritado com sucesso!\n", usuarios[indiceUsuario].produtos[indiceProduto].nomeProduto);

    esperar_apertarEnter();
}

void listarFavoritosDoUsuario(usuario_t usuario, produto_t * PRODUTOS, int nProdutos, usuario_t * USUARIOS, int nUsuarios) {

    int i, idUsuarioDono;
    produto_t produtoAtual;
 
    // Obtem o vetor que salva os id's dos produtos & o tamanho do vetor
    int * indicesProdutosFavoritos = usuario.produtosFavoritos;

    int nFavoritos = usuario.nFavoritos;

    // mensagem legal
    printf("%-13s | %-30s | %-30s\n", "Id do Produto", "Nome do Produto", "Vendedor do Produto");

    // Loop entre todos os valores de id de produto (no vetor
    // de indices dos produtos favoritos, "indicesProdutosFavoritos")
    for (i = 0; i < nFavoritos; i++) {

        // Obtem o produto com o id "indicesProdutosFavoritos[i]"
        produtoAtual = PRODUTOS[indicesProdutosFavoritos[i]];

        // Procura o índice do usuário com id igual ao id do dono do produto
        idUsuarioDono = buscarUsuarioPorId(USUARIOS, nUsuarios, produtoAtual.idDono);

        // Verifica se a função retornou o código de erro ou um índice
        if (idUsuarioDono == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
            continue; // Pula esse produto
        }

        // Printa as informações do produto
        printf("%-13i   %-30s   %-30s\n", produtoAtual.Id, produtoAtual.nomeProduto, USUARIOS[idUsuarioDono].nome);
    }

    // Espera pro usuário poder ver as coisas
    esperar_apertarEnter();

}

// Mostra a tela inicial ao usuário e retorna a ação dele.
// Note que se o usuário estiver ou não com login afetará
// as opções dele (informação passada por "usuarioLogado")
int telaInicial(usuario_t * Usuarios, int loginAtual) {

    int escolha = 0;

    printf("\n\n"); // Para dar um espaço
    
    // Verifica se está ou não com login    
    if(loginAtual == USUARIO_SEM_LOGIN) printf("Voce esta com acesso limitado!\nFaca login para ter acesso a todas as funcoes.\n");

    // Se o usuário estiver logado
    if (loginAtual != USUARIO_SEM_LOGIN) {

        // Printa as ações & pergunta o que o usuário quer (menos dar o oríficio)
        printf("\n\tCadastro\n\n1 - Criar uma nova conta\n2 - Fazer login em outra conta\n3 - Sair da conta");
        printf("\n\n\tProduto\n\n4 - Cadastrar produto\n5 - Editar produto\n6 - Excluir produto\n7 - Comprar produto\n8 - Avaliar produto");
        printf("\n9 - Listar avaliacoes de um Produto\n10 - Favoritar um produto\n11 - Desfavoritar um produto\n12 - Listar os favoritos do usuario");
        printf("\n\n\tGerais\n\n13 - Buscar usuario\n14 - Listar produtos\n0 - Sair do programa\n\n");

        // Loop até o usuário digitar algo válido
        do {
            printf("Bem vindo, %s! O que deseja fazer? ", Usuarios[loginAtual].nome);
            scanf("%d%*c", &escolha);
            
            if (escolha < MENU_PRI_OPCAO_MIN || escolha > MENU_PRI_OPCAO_MAX) {
                printf("Opcao invalida, tente novamente.\n\n");
            }

        } while (escolha < MENU_PRI_OPCAO_MIN || escolha > MENU_PRI_OPCAO_MAX); // Enquanto o usuário não tiver digitado algo válido
       
    } else { // Se o usuário não estiver logado

        printf("\n\tCadastro\n\n1 - Criar uma nova conta\n2 - Fazer login em uma conta\n0 - Sair do programa\n\n");

        // Loop até o usuário digitar algo válido
        do {
            printf("O que deseja fazer?: ");
            scanf("%d%*c", &escolha);
            
            if (escolha < 0 || escolha > 3) {
                printf("Opcao invalida, tente novamente.\n\n");
            }

        } while (escolha < 0 || escolha > 3); // Enquanto o usuário não tiver digitado algo válido

    }

    printf("\n"); // Estética

    return escolha;

}


erro_t desfavoritarProduto(usuario_t *usuarios, int nUsuarios, int idUsuarioAtual) {
    int indiceUsuario, indiceProduto;

    usuario_t *usuarioAtual = &usuarios[idUsuarioAtual];

    indiceUsuario = buscaUsuario(usuarios, nUsuarios);

    indiceProduto = escolherProduto(usuarios[indiceUsuario]);

    if (indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return ERRO_BUSCAR_SEM_CORRESPONDENTE;
    }

    int produtoId = usuarios[indiceUsuario].produtos[indiceProduto].Id;
    int encontrado = 0;

    for (int i = 0; i < usuarioAtual->nFavoritos; i++) {
        //Só entra na condição se o produto selecionado for igual a algum produto do vetor
        if (usuarioAtual->produtosFavoritos[i] == produtoId) {
            encontrado = 1;

            // Move ate o tamanho e realoca pra -1, pra deletar
            for (int j = 0; j < usuarioAtual->nFavoritos - 1; j++) {
                usuarioAtual->produtosFavoritos[j] = usuarioAtual->produtosFavoritos[j + 1];
            }

            // Reduz o número de favoritos e realoca a memória
            usuarioAtual->nFavoritos--;
            usuarioAtual->produtosFavoritos = (int *)realloc(usuarioAtual->produtosFavoritos, usuarioAtual->nFavoritos * sizeof(int));
            break;
        }
    }
}

int main(int argc, char ** argv)
{
    usuario_t * usuarios = NULL;
    int nUsuarios = 0;

    produto_t * PRODUTOS = NULL;
    int nProdutos = 0;
    
    int loginAtual = USUARIO_SEM_LOGIN; // Sempre incia sem login
    int escolha = -1;
    
    do{

        // Mostra as opções para o usuário & retorna o que o usuário escolheu
        escolha = telaInicial(usuarios, loginAtual);

        // Casos de escolha
        switch (escolha)
        {

        case 1: // Cadastro
            cadastraUsuario(&usuarios, &nUsuarios);
            break;

        case 2: // Login
            loginUsuario(&loginAtual, usuarios, nUsuarios);
            break;

        case 3: // Desfaz o login
            loginAtual = USUARIO_SEM_LOGIN;
            break;

        case 4: // Cadastrar um produto
            cadastraProduto(&usuarios[loginAtual], &PRODUTOS, &nProdutos);
            break;

        case 5: // Editar um produto
            editarProduto(&usuarios[loginAtual]);
            break;

        case 6: // Apagar um produto
            apagarProduto(&usuarios[loginAtual]);
            break;

        case 7: // Comprar um produto
            compraProduto(usuarios, nUsuarios);
            break;

        case 8: // Fazer uma avaliação
            fazerAvaliacao(usuarios, nUsuarios, loginAtual);
            break;
        
        case 9: // Lista as avaliações de um produto
            listarAvaliacoes(&usuarios[loginAtual], nUsuarios);
            break;

        case 10: // Favoritar um produto
            favoritarProduto(usuarios, nUsuarios, loginAtual);
            break;

        case 11: // Desfavoritar
            desfavoritarProduto(usuarios, nUsuarios, loginAtual);
            break;

        case 12: // Lista os favoritos do usuário
            listarFavoritosDoUsuario(usuarios[loginAtual], PRODUTOS, nProdutos, usuarios, nUsuarios);
            break;
        
        case 13: // Buscar um usuário
            buscaUsuario(usuarios, nUsuarios);
            break;
        
        case 14://Lista todos os produtos de um usuário
            listarProdutos(usuarios, nUsuarios);
            break;
            
        case 0: // Encerra o programa
            return SUCESSO;
            break;
        }
        
    }while(escolha != 0); 
    // O loop nunca chegará nessa parte sem ser parado.
    // Entretanto, para facilitar a leitura, a condição foi colocada.

    return SUCESSO;
}

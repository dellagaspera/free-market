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
#define MENU_PRI_OPCAO_MAX 12   // Opção máxima que o usuário pode escolher no menu principal

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
int escolherProduto(usuario_t usuario)
{
    if(usuario.nProdutos == 0) return ERRO_BUSCAR_SEM_CORRESPONDENTE;

    //Faz um loop e imprime todos os usuarios pra voce selecionar
    //Faz um loop por todos dnv e vc digita um nome(dps muda pra ID) e retorna o indice dele

    for(int i = 0; i < usuario.nProdutos; i++) {

        printf("%d - %s\n", (i+1), usuario.produtos[i].nomeProduto);//Muda esse %d dps pra %s, ai vai ser o id seguido do nome, assim-> id - nome
    }

    char tempVar[MAX_CHAR_ID];
    printf("Digite o nome do produto(vai ser id dps tropa):");
    fgets(tempVar, MAX_CHAR_ID, stdin);
    removeQuebra(tempVar);

    for(int i = 0; i < usuario.nProdutos; i++) {

        if(strcmp(tempVar, usuario.produtos[i].nomeProduto) == 0) {
            return i;
        }
    }
}

int buscaUsuario(usuario_t *usuario, int nUsuarios) {
    //Faz um loop e imprime todos os usuarios pra voce selecionar
    //Faz um loop por todos dnv e vc digita um nome(dps muda pra ID) e retorna o indice dele

    printf("Nomes\n");
    for(int i = 0; i < nUsuarios; i++) {
        printf("%s\n", usuario[i].nome);
    }

    char tempVar[MAX_CHAR_ID];
    printf("Digite o nome do usuario:"); // vai ser id dps
    fgets(tempVar, MAX_CHAR_ID, stdin);
    removeQuebra(tempVar);

    for(int i = 0; i < nUsuarios; i++) {

        if(strcmp(tempVar, usuario[i].nome) == 0) {
            return i;
        }
    }

}

void compraProduto(usuario_t *usuario, int nUsuarios) {
    
    int indiceProduto = buscaUsuario(usuario, nUsuarios); // TROCAR DEPOIS PARA PASSAR O PRODUTO DESEJADO
    int nUnidades;

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE)
    {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }
    if(usuario->produtos[indiceProduto].estoque <= 0) {
        //Verifica no indice informado se o produto ja foi comprado
        printf("PRODUTO SEM ESTOQUE\n");
        return;
    } else printf("ESTOQUE PRODUTO: %d", usuario->produtos[indiceProduto].estoque);
    
    do
    {
        printf("Numero de unidades: ");
        scanf("%d%*c", &nUnidades);

        if(nUnidades <= usuario->produtos[indiceProduto].estoque) 
        {
            usuario->produtos[indiceProduto].estoque -= nUnidades;
            printf("PRODUTO COMPRADO COM SUCESSO\n");
            return;
        } 
        else
        {
            printf("NAO HA UNIDADES SUFICIENTES\n");
            printf("1 - Tentar novamente\n");
            printf("0 - Voltar\n : ");
            int escolha;
            scanf("%d%*c", &escolha);
            if(escolha == 0) return;
        }
    } while(usuario->produtos[indiceProduto].estoque);
    
    
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

    produto_t produtoCadastrado;
   
    // Aloca para produtos do usuario
    if (usuario->produtos == NULL) {
        usuario->produtos = malloc(sizeof(produto_t));
        if (usuario->produtos == NULL) {
            return;
        }
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

    // Aloca para o novo produto & Seta para o produto que acabou de criar
    
    if ((*PRODUTOS = realloc(*PRODUTOS, sizeof(produto_t) * (*nProdutos + 1))) == NULL) {
        return;
    }
    (*PRODUTOS)[*nProdutos] = usuario->produtos[usuario->nProdutos];

    usuario->produtos[usuario->nProdutos].nAvaliacoes = 0; //
    usuario->produtos[usuario->nProdutos].avaliacoes = NULL;

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

// Retorna o índice de um usuário com o email "_email"
int buscarUsuarioPorEmail(usuario_t *usuarios, int nUsuarios, char _email[MAX_CHAR_EMAIL]) 
{
    for(int i = 0; i < nUsuarios; i++) 
    {
        if(strcmp(usuarios[i].email, _email) == 0) return i;
    }

    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}



// Cadastra o usuario com um usuario unico e uma senha
void cadastraUsuario(usuario_t ** usuarios, int * nUsuarios)
{
    char email_[MAX_CHAR_EMAIL];
    char senha_[MAX_CHAR_SENHA];
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

    printf("Nome de Exibicao\n : ");
    fgets((* usuarios)[* nUsuarios].nome, MAX_CHAR_NOME, stdin);
    removeQuebra((* usuarios)[* nUsuarios].nome);

    (* usuarios)[* nUsuarios].nProdutos = 0; // Configura o número de produtos vinculados ao usuário
    (* usuarios)[* nUsuarios].produtos = NULL; // Configura o vetor de produtos vinculados ao usuário

 //   (* usuarios)[* nUsuarios].ID = (* nUsuarios); // Determina o ID do usuário

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
        printf("%d/%d - %d\t%s\n", i + 1, nUsuarios, usuarios[i].ID, usuarios[i].senha);
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

void fazerAvaliacao(usuario_t *usuarios, int nUsuarios) {

    
    int indiceUsuario = buscaUsuario(usuarios, nUsuarios);
       
    int indiceProduto = escolherProduto(usuarios[indiceUsuario]);

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
    
    printf("Digite a nota: ");
    scanf("%d%*c", &nota);


    if (nota < 0 || nota > 5) {
        printf("Nota invalida. Deve ser entre 0 e 5.\n");
        return;
    }

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

    int indiceProduto = escolherProduto(usuarios[indiceUsuario]);
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

    
    printf("Avaliacoes para o produto %s:\n", produto.nomeProduto);
    for (int i = 0; i < produto.nAvaliacoes; i++) {
        printf("Nota %d\n ",produto.avaliacoes[i].nota);
    }
    

    
            
    esperar_apertarEnter();
} 

// Favorita um produto do usuário "usuarios[usuarioAtual]"
erro_t favoritarProduto(usuario_t *usuarios, int nUsuarios, int idUsuarioAtual) {
    
    int indiceUsuario, indiceProduto;
    usuario_t * usuarioAtual = &usuarios[idUsuarioAtual];

    // Pede para escolher um usuário
    indiceUsuario = buscaUsuario(usuarios, nUsuarios);

    // Pede para escolher um produto do usuário já escolhido
    indiceProduto = escolherProduto(usuarios[indiceUsuario]);

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
    usuarioAtual->produtosFavoritos = (int *)realloc(usuarioAtual->produtosFavoritos, (usuarioAtual->nFavoritos + 1) * sizeof(int));
    usuarioAtual->produtosFavoritos[usuarioAtual->nFavoritos] = usuarios[indiceUsuario].produtos[indiceProduto].Id;
    usuarioAtual->nFavoritos++;
    printf("Produto %s favoritado com sucesso!", usuarios[indiceUsuario].produtos[indiceProduto].nomeProduto);

    esperar_apertarEnter();

}

void listarFavoritosDoUsuario(usuario_t usuario, produto_t * PRODUTOS, int nProdutos) {

    int i;
    produto_t produtoAtual;

    // Obtem o vetor que salva os id's dos produtos & o tamanho do vetor
    int * indicesProdutosFavoritos = usuario.produtosFavoritos;
    int nFavoritos = usuario.nFavoritos;

    // mensagem legal
    printf("%-13s | %30s | %30s\n", "Id do Produto", "Nome do Produto", "Vendedor do Produto");

    // Loop entre todos os valores de id de produto (no vetor
    // de indices dos produtos favoritos, "indicesProdutosFavoritos")
    for (i = 0; i < nFavoritos; i++) {

        // Obtem o produto com o id "indicesProdutosFavoritos[i]"
        produtoAtual = PRODUTOS[indicesProdutosFavoritos[i]];

        // Printa as informações do produto
        printf("%13i   %30s   %30s\n", produtoAtual.Id, produtoAtual.nomeProduto, "N/A");

    }

}

// Mostra a tela inicial ao usuário e retorna a ação dele.
// Note que se o usuário estiver ou não com login afetará
// as opções dele (informação passada por "usuarioLogado")
int telaInicial(usuario_t * Usuarios, int loginAtual) {

    int escolha = 0;

    printf("\n\n"); // Para dar um espaço
    
    // Verifica se está ou não com login
    if(loginAtual == USUARIO_SEM_LOGIN) printf("Voce esta com acesso limitado!\nFaca login para ter acesso a todas as funcoes.\n");
    else printf("Bem vindo, %s!\n", Usuarios[loginAtual].nome);

    // Se o usuário estiver logado
    if (loginAtual != USUARIO_SEM_LOGIN) {

        // Printa as ações & pergunta o que o usuário quer (menos dar o oríficio)
        printf("\n\tCadastro\n\n1 - Criar uma nova conta\n2 - Fazer login em outra conta\n3 - Sair da conta");
        printf("\n\n\tProduto\n\n4 - Cadastrar produto\n5 - Editar produto\n6 - Excluir produto\n7 - Comprar produto\n8 - Avaliar produto");
        printf("\n9 - Listar avaliacoes de um Produto\n10 - Favoritar um produto\n11 - Listar os favoritos do usuario");
        printf("\n\n\tGerais\n\n12 - Buscar usuario\n0 - Sair do programa\n\n");

        // Loop até o usuário digitar algo válido
        do {
            printf("O que deseja fazer?: ");
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

    return escolha;

}


int main(int argc, char ** argv)
{
    usuario_t * usuarios = NULL;

    produto_t * PRODUTOS = NULL;
    int nProdutos = 0;
    
    //Sempre incia sem login
    int loginAtual = USUARIO_SEM_LOGIN;
    int nUsuarios = 0;
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
            compraProduto(&usuarios[loginAtual], nUsuarios);
            break;

        case 8: // Fazer uma avaliação
            fazerAvaliacao(usuarios, nUsuarios);
            break;
        
        case 9: // Lista as avaliações de um produto
            listarAvaliacoes(&usuarios[loginAtual], nUsuarios);
            break;

        case 10: // Favoritar um produto
            favoritarProduto(&usuarios[loginAtual], nUsuarios, loginAtual);
            break;

        case 11: // Lista os favoritos do usuário
            listarFavoritosDoUsuario(usuarios[loginAtual], PRODUTOS, nProdutos);
            break;
        
        case 12: // Buscar um usuário
            buscaUsuario(usuarios, nUsuarios);
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

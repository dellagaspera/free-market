// Inclusões
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Constantes
#define MAX_CHAR_NOME   (30 + 1) // Tamanho máximo do nome do usuário
#define MAX_CHAR_EMAIL  (30 + 1)
#define MAX_CHAR_SENHA  (20 + 1)

#define MAX_CHAR_NOME_PRODUTO (40 + 1)
#define MAX_CHAR_DESCRICAO    (128 + 1)

#define USUARIO_SEM_LOGIN -1

#define SIM 1
#define NAO 0

// Arquivos
#define ARQUIVO_USUARIOS "usuarios_freemarket"

// Saídas 
#define SUCESSO 0

#define ERRO_BUSCAR_SEM_CORRESPONDENTE -1


/* DEPOIS A GENTE PREOCUPA COM AVALIACAO
    VAMO AOS POUCOS
typedef struct avaliacao_s {

    // Informações de quem mandou
    char autorMensagem[MAX_CHAR_USUARIO];
    char autorMensagemID[MAX_CHAR_ID];

    // Avaliação
    int nota; // 0 a 5 (estrelas)
    char * mensagem;

} avaliacao_t;

*/
// Estrutura do produto
typedef struct produto_s { //obs: a chave é do lado do nome! nao é! é sim!

    // Informações básicas do produto
    char nomeProduto[MAX_CHAR_NOME_PRODUTO];
    char descricaoProduto[MAX_CHAR_DESCRICAO];

    int nImagens;
    char * imagens;
    
    int estoque;

    // Avaliacoes do produto
    // int nAvaliacoes;
    // avaliacao_t * avaliacoes; 
    
} produto_t;

// Estrutura do usuário
typedef struct usuario_s
{
    int ID;
    char nome[MAX_CHAR_NOME];
    char email[MAX_CHAR_EMAIL];
    char senha[MAX_CHAR_SENHA];
    produto_t * produtos;
    int nProdutos;
    
    
} usuario_t;

// Tipo para quando retornar códigos de erro
typedef int erro_t;

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

    int indiceProduto;

    // Printa todos os produtos do usuário
    for(int i = 0; i < usuario.nProdutos; i++) {
        printf("%d - %s\n", (i + 1), usuario.produtos[i].nomeProduto);
    }

    // Pede o usuário que produto quer editar
    do {
        printf("Escolha o produto (somente o numero)\n : ");
        scanf("%d%*c", &indiceProduto);
        indiceProduto--;
        if(indiceProduto > usuario.nProdutos || indiceProduto < 0) printf("PRODUTO INVALIDO\n");
    } while (indiceProduto > usuario.nProdutos || indiceProduto < 0);

    return indiceProduto;
}

int buscaUsuario(usuario_t *usuario, int nUsuarios) {

    int indiceUsuario;

    for(int i = 0; i<nUsuarios; i++) {
    
    printf("%d - %s\n", (i + 1), usuario[i].nome);
    }
    
    do{
        printf("Digite o número do usuario desejado: ");
        scanf("%d%*c", &indiceUsuario);
        
        indiceUsuario-=1;//Passa onde q o usuario ta armazenado
        
        for(int i = 0; i < usuario->nProdutos; i++) {
            
        printf("%d - %s\n", (i + 1), usuario[indiceUsuario].produtos[i].nomeProduto);
        
        }
        
        if(indiceUsuario > nUsuarios || indiceUsuario < 0) printf("USUARIO NÃO ENCONTRADO\n");
        
    }while(indiceUsuario > nUsuarios || indiceUsuario < 0);
    
    return indiceUsuario;
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
void cadastraProduto(usuario_t * usuario) { 
   
    usuario->produtos = (produto_t *)realloc(usuario->produtos, sizeof(produto_t) * (usuario->nProdutos + 1));
    printf("Nome produto: ");
    fgets(usuario->produtos[usuario->nProdutos].nomeProduto, MAX_CHAR_NOME_PRODUTO, stdin);
    removeQuebra(usuario->produtos[usuario->nProdutos].nomeProduto);
    printf("Descricao produto: ");
    fgets(usuario->produtos[usuario->nProdutos].descricaoProduto, MAX_CHAR_DESCRICAO, stdin);
    removeQuebra(usuario->produtos[usuario->nProdutos].descricaoProduto);
    
    //Imagens fica pra dps
    printf("Estoque produto: ");
    scanf("%d%*c", &usuario->produtos[usuario->nProdutos].estoque);
    
    (usuario->nProdutos)++;
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

// Procura um usuario com um nome <usuario_>
int buscarUsuario(usuario_t *usuarios, int nUsuarios, char email_[MAX_CHAR_EMAIL]) 
{
    for(int i = 0; i < nUsuarios; i++) 
    {
        if(strcmp(usuarios[i].email, email_) == 0) return i;
    }

    return ERRO_BUSCAR_SEM_CORRESPONDENTE;
}

// Cadastra o usuario com um usuario unico e uma senha
void cadastraUsuario(usuario_t ** usuarios, int * nUsuarios)
{
    char email_[MAX_CHAR_EMAIL];
    char senha_[MAX_CHAR_SENHA];
    * usuarios = realloc(* usuarios, sizeof(usuario_t) * (* nUsuarios + 1));

    // Recebe um e-mail e verifica se ja esta cadastrado
    do
    {
        printf("E-Mail\n : ");
        fgets(email_, MAX_CHAR_EMAIL, stdin);
        removeQuebra(email_);
        if(validaEmail(email_) == NAO) printf("E-MAIL INVALIDO\n");
        if(buscarUsuario(* usuarios, * nUsuarios, email_) != ERRO_BUSCAR_SEM_CORRESPONDENTE) printf("E-MAIL JA UTILIZADO\n");
    } while(buscarUsuario(* usuarios, * nUsuarios, email_) != ERRO_BUSCAR_SEM_CORRESPONDENTE || validaEmail(email_) == NAO);//Se digitar certo sai do loop
    strcpy((* usuarios)[* nUsuarios].email, email_);

    // Cadastra a senha
    printf("Senha: ");
    fgets((* usuarios)[* nUsuarios].senha, MAX_CHAR_SENHA, stdin);
    removeQuebra((* usuarios)[* nUsuarios].senha);

    printf("Nome de Exibicao\n : ");
    fgets((* usuarios)[* nUsuarios].nome, MAX_CHAR_NOME, stdin);

    (* usuarios)[* nUsuarios].nProdutos = 0; // Configura o número de produtos vinculados ao usuário
    (* usuarios)[* nUsuarios].produtos = NULL; // Configura o vetor de produtos vinculados ao usuário

    (* usuarios)[* nUsuarios].ID = (* nUsuarios); // Determina o ID do usuário

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

        if(buscarUsuario(usuarios, nUsuarios, email_) == ERRO_BUSCAR_SEM_CORRESPONDENTE)
        {
            printf("E-MAIL INVALIDO\n1 - Voltar ao menu inicial\n2 - Tentar novamente\n : "); //Verifica se o usuario existe
            int escolha;
            scanf("%d%*c", &escolha);
            if(escolha == 1) return;
        }
        else indiceUsuario = buscarUsuario(usuarios, nUsuarios, email_); //Pega o indice retornado pela funcao

    }while(buscarUsuario(usuarios, nUsuarios, email_) == ERRO_BUSCAR_SEM_CORRESPONDENTE);

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
        * loginAtual = indiceUsuario;
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

int main(int argc, char ** argv)
{
    usuario_t * usuarios = NULL;
    
    //Sempre incia sem login
    int loginAtual = USUARIO_SEM_LOGIN;
    int nUsuarios = 0;
    int escolha = -1;
    
    do{

        // Verifica se o usuário está com login ou não.
        // Se estiver, mostra em que conta você está
        if(loginAtual == USUARIO_SEM_LOGIN) printf("Sem login\n");
        else printf("Logado como: %s\n", usuarios[loginAtual].nome);

        // Pede para o usuário escolher
        printf("1 - Cadastrar usuario\n2 - Logar\n3 - Deslogar\n4 - Cadastrar produto\n5 - Editar produto\n6 - Excluir produto\n7 - Comprar produto\n8 - Buscar usuario\n0 - Sair");

        printf("\nescolha: ");
        scanf("%d%*c", &escolha);

        // Casos de escolha
        switch(escolha)
        {
            
        case 1: //Cadastro
        cadastraUsuario(&usuarios, &nUsuarios);
        break;
        
        case 2: //Login
        loginUsuario(&loginAtual, usuarios, nUsuarios);
        break;

        case 3: // Desfaz o login
        loginAtual = USUARIO_SEM_LOGIN;
        break;

        case 4:
        if(loginAtual != USUARIO_SEM_LOGIN)
        {
            cadastraProduto(&usuarios[loginAtual]);
        } else printf("VOCE PRECISA ESTAR LOGADO EM UMA CONTA\n");
        break;
        
        case 5:
        if(loginAtual != USUARIO_SEM_LOGIN)
        {
            editarProduto(&usuarios[loginAtual]);
        } else printf("VOCE PRECISA ESTAR LOGADO EM UMA CONTA\n");
        break;
        
        case 6:
        if(loginAtual != USUARIO_SEM_LOGIN)
        {
            apagarProduto(&usuarios[loginAtual]);
        } else printf("VOCE PRECISA ESTAR LOGADO EM UMA CONTA\n");
        break;
        
        case 7:
        if(loginAtual != USUARIO_SEM_LOGIN)
        {
            compraProduto(&usuarios[loginAtual], nUsuarios);
        } else printf("VOCE PRECISA ESTAR LOGADO EM UMA CONTA\n");
        break;
        
        case 8:
        if(loginAtual != USUARIO_SEM_LOGIN)
        {
            buscaUsuario(&usuarios[loginAtual], nUsuarios);
        } else printf("VOCE PRECISA ESTAR LOGADO EM UMA CONTA\n");
        break;
        
        case 0: // Encerra o programa
        return SUCESSO;

        break;
        }
    } while(escolha != 0); 
    // O loop nunca chegará nessa parte sem ser parado.
    // Entretanto, para facilitar a leitura, a condição foi colocada.

    return SUCESSO;
}

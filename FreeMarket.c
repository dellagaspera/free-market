// Inclusões
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Constantes
    // Tamanhos máximos
#define MAX_CHAR_NOME           (30 + 1)
#define MAX_CHAR_EMAIL          (30 + 1)
#define MAX_CHAR_SENHA          (20 + 1)
#define MAX_CHAR_USUARIO        (20 + 1)
#define MAX_CHAR_ID             (20 + 1)
#define MAX_LINK                (200 + 1)
#define MAX_CHAR_NOME_PRODUTO   (40 + 1)
#define MAX_CHAR_DESCRICAO      (128 + 1)
#define MAX_IMAGENS              70

#define USUARIO_SEM_LOGIN -1

#define SIM 1
#define NAO 0

#define MENU_PRI_OPCAO_MIN 0    // Opção mínima que o usuário pode escolher no menu principal
#define MENU_PRI_OPCAO_MAX 14   // Opção máxima que o usuário pode escolher no menu principal

// Constantes de imagem

#define BUFFER_TAMANHO                  50000           // Tamanho do buffer para cada linha da imagem
#define LINHA_COMANDO                   10000           // Tamanho máximo da linha de comando
#define LIMIAR_INFERIOR_TAMANHO_IMAGEM  500             // Limiar inferior que considera o download com sucesso
#define IMAGENS_COLORIDAS               true            // Se a imagem é colorida ou não
#define IMAGEM_PRODUTO_NUMERO_COLUNAS   70              // Número de colunas da imagem

// Nome do executável da ferramenta de download e conversão da imagem
#ifdef __unix__ 
#define FERRAMENTA_IMAGEM   "ascii-image-converter.bin"
#else
#define FERRAMENTA_IMAGEM   "ascii-image-converter.exe"
#endif

#define ARQUIVO_IMAGEM_TMP  "ascii_art.txt" // Nome do arquivo de sa�da tempor�rio da imagem

// Arquivos
#define NOME_ARQUIVO_USUARIOS "./usuarios_freemarket.bin"
#define NOME_ARQUIVO_PRODUTOS "./produtos_freemarket.bin"

// Saídas 
#define SUCESSO 0

#define ERRO_BUSCAR_SEM_CORRESPONDENTE -1
#define SEM_AVALIACOES -1

// bool EMANUEL_RECUPERACAO = true;


/*  =====================================================  */
/*  ================= Código de Imagens =================  */
/*  =====================================================  */

        /// Estrutura que representa uma imagem em Ascii
        struct asciiImg_s {
        uint8_t * bytes;
        int nBytes;
        };

        /// Tipo "Imagem ASCII"
        typedef struct asciiImg_s asciiImg_t;

        /**
         *  \brief Função que carrega uma imagem informada na URL.
         *  
         *  \param [in] colorido Define se a imagem ser� colorida.
         *  \param [in] largura Define a largura da imagem gerada.
         *  \return Endereço da estrutura com a imagem. 
         *          Caso a imagem não tenha sido carregada corretamente, a função
         *          retornará NULL.
         */
        asciiImg_t * insta_carregaImagem(char url[], bool colorido, int largura) {
        
        FILE * arquivo;
        char buffer[BUFFER_TAMANHO];
        int nBytes, nBytesTotal = 0;
        char linhaComando[LINHA_COMANDO];

        asciiImg_t * img;
        
        // Aloca espaço para uma imagem
        img = malloc(sizeof(asciiImg_t));
        if (img == NULL) return NULL;
        
        // Inicializa a estrutura
        img->bytes = NULL;
        img->nBytes = 0;
        
        // Monta a linha de comando
        (void)sprintf(linhaComando, "%s %s %s -W %d -c > %s", FERRAMENTA_IMAGEM, url, (colorido ? "-C" : ""), largura, ARQUIVO_IMAGEM_TMP);
        
        // Chama o programa para fazer o download da imagem
        (void)system(linhaComando);

        // Tenta abrir o arquivo recem criado
        arquivo = fopen(ARQUIVO_IMAGEM_TMP, "r");
        if (arquivo != NULL) {
            
            while(!feof(arquivo)) {
            
            // Limpa a linha
            (void)memset(buffer, 0, sizeof(buffer));
            
            // Tenta ler uma linha
            if (fgets(buffer, BUFFER_TAMANHO, arquivo) == NULL) continue;
            
            // Descobre o n�mero de bytes da linha
            for(nBytes = 0; buffer[nBytes] != 0; nBytes++);
            
            // Aloca o espa�o
            img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + nBytes));
            
            // Copia para o espa�o alocado
            (void)memcpy(&(img->bytes[nBytesTotal]), buffer, nBytes);
            nBytesTotal+=nBytes;
            }

            // Finaliza a imagem colocando o \0 final e o tamanho
            img->bytes = realloc(img->bytes, sizeof(unsigned char) * (nBytesTotal + 1));
            img->bytes[nBytesTotal++] = '\0';
            img->nBytes = nBytesTotal;
            
            // Fecha o arquivo
            fclose(arquivo);
        }
        
        // Verifica se a imagem é válida
        if (img->nBytes < LIMIAR_INFERIOR_TAMANHO_IMAGEM) {
            // Libera todo o espaço alocado
            free(img->bytes);
            free(img);
            
            return NULL;
        }
        
        // Retorna a imagem carregada
        return img;
        }

        /**
         *  \brief Fun��o que imprime uma Imagem ASCII.
         *  
         *  \param [in] img Endere�o da estrutura com os dados da imagem.
         */
        void insta_imprimeImagem(asciiImg_t * img) {
        printf("%s", img->bytes);
        }

        /**
         *  \brief Fun��o que libera a mem�ria alocada por uma imagem.
         *  
         *  \param [in] img Endere�o da estrutura com os dados da imagem a ser liberada.
         */
        void insta_liberaImagem(asciiImg_t * img) {
  free(img->bytes);
  free(img);
}




/*  =========================================================  */
/*  ================= Código do Free Market =================  */
/*  =========================================================  */


typedef struct avaliacao_s {

    // Informações de quem mandou
    char autorMensagem[MAX_CHAR_USUARIO];
    char autorMensagemID[MAX_CHAR_ID];

    // Avaliação
    int nota; // 0 a 5 (estrelas)
    char  mensagem[MAX_CHAR_DESCRICAO];

} avaliacao_t;

// Estrutura do produto
typedef struct produto_s {

    // Informações básicas do produto
    char nomeProduto[MAX_CHAR_NOME_PRODUTO];
    char descricaoProduto[MAX_CHAR_DESCRICAO];

    int Id;
    char idDono[MAX_CHAR_ID];
    int nImagens;
    asciiImg_t** imagens;
    
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

    int idDigitado, i, j;
    produto_t produtoAtual;

    //Faz um loop e imprime todos os usuarios pra voce selecionar
    //Faz um loop por todos dnv e vc digita um nome(dps muda pra ID) e retorna o indice dele

    printf("Produtos para escolher: \n\n");
    // Loop em todos os produtos do usuário e printa o id e o nome do produto
    for(i = 0; i < usuario.nProdutos; i++) {

        // Seta o produto atual como o produto sendo printado agora
        produtoAtual = usuario.produtos[i];

        // Se houver imagens do produto
        if (produtoAtual.nImagens > 0) {
            printf("Imagens do produto:\n");
            
            // Loop no vetor das imagens(vetor de "links") & printa cada imagem
            for (j = 0; j < produtoAtual.nImagens; j++){

                // Verifica se a imagem existe
                if (produtoAtual.imagens[j] != NULL) {

                    // Printa o "título" da imagem
                    printf("\nImagem %i/%i:\n", j+1, produtoAtual.nImagens);  

                    // Printa a imagem
                    insta_imprimeImagem(produtoAtual.imagens[j]);  
                }

                
            }  
        }

        printf("Nome do produto: %s\nDescricao do produto: %s\nID do produto: %i\n\n", produtoAtual.nomeProduto, produtoAtual.descricaoProduto, produtoAtual.Id);
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

// Retorna a média das notas do produto
float mediaNotasProduto(produto_t produto) {
    
    int i;
    float soma;

    // Se não houver nenhuma avaliação, retorna -1
    if (produto.nAvaliacoes <= 0) {
        return SEM_AVALIACOES;
    }

    // Se chegou até aqui, é porque existe pelo menos uma avaliação
    soma = 0;

    // Loop em cada avaliação, somando a sua nota
    for (i = 0; i < produto.nAvaliacoes; i++) {
        soma += produto.avaliacoes[i].nota;
    }

    // Retorna a média
    return soma / produto.nAvaliacoes;
}

// Busca um usuário e mostra as informações dele
int buscaUsuario(usuario_t *usuario, int nUsuarios, bool esperar_enter) {

    char tempVar[MAX_CHAR_ID];
    usuario_t usuarioPesquisado;
    int i, j;
    float temp;

    // Exibe todos os usuários
    printf("%-31s | %-30s\n", "ID", "Nome");
    for (i = 0; i < nUsuarios; i++) {
        printf("%-31s   %s\n", usuario[i].ID, usuario[i].nome);
    }

    printf("Digite o ID do usuario: ");
    fgets(tempVar, MAX_CHAR_ID, stdin);
    removeQuebra(tempVar);

    // Busca o usuário pelo ID
    for (i = 0; i < nUsuarios; i++) {

        if (strcmp(tempVar, usuario[i].ID) == 0) { // Vê se o usuário de índice i é o que está procurando

            usuarioPesquisado = usuario[i]; // Usuário que a pessoa procura é o usuario[i]

            // Printa as informações sobre o usuário
            printf("Nome do usuario: %s\n", usuarioPesquisado.nome);
            printf("Id do usuario: %s\n", usuarioPesquisado.ID);
            printf("Quantidade de produtos cadastrados: %d\nProdutos do Usuario:\n", usuarioPesquisado.nProdutos);

            // Exibe informações sobre cada produto
            for (j = 0; j < usuarioPesquisado.nProdutos; j++) {

                // Printa a primeira imagem(se tiver) e as informações sobre o produto
                if (usuarioPesquisado.produtos[j].nImagens > 0) {
                    insta_imprimeImagem(usuarioPesquisado.produtos[j].imagens[0]);     
                }
                printf("\nNome do Produto: %s\nDescricao: %s\n", usuarioPesquisado.produtos[j].nomeProduto, usuarioPesquisado.produtos[j].descricaoProduto);
                
                temp = mediaNotasProduto(usuarioPesquisado.produtos[j]);
                if (temp >= 0) {
                    printf("Nota Media do Produto: %1.1f\n", temp);
                } else {
                    printf("Nao ha avaliacoes do produto.\n");
                }
            }

            if (esperar_enter) {
                esperar_apertarEnter();
            }

            return i; // Retorna o índice do usuário encontrado
        }
    }

    printf("Usuário não encontrado.\n");
    return ERRO_BUSCAR_SEM_CORRESPONDENTE; // Retorna -1 se não encontrar o usuário
}

// Lista os produtos de um usuário
void listarProdutos(usuario_t *usuario, int nUsuarios) {
    
    int indiceUsuario = buscaUsuario(usuario, nUsuarios, false);
    
    if (indiceUsuario == -1) {
        return; // Retorna se o usuário não foi encontrado
    }

    // Verifica se o usuário tem produtos
    if (usuario[indiceUsuario].nProdutos == 0) {
        printf("Nenhum produto cadastrado para o usuario %s.\n", usuario[indiceUsuario].nome);
        return;
    }

    // Lista os produtos do usuário
    printf("\n\nResumo:\n%-2s | %-30s\n", "ID", "Nome");
    for (int i = 0; i < usuario[indiceUsuario].nProdutos; i++) {
        printf("%02d   %s\n", i, usuario[indiceUsuario].produtos[i].nomeProduto);
    }

    esperar_apertarEnter();
}

// Compra um produto de um usuário
void compraProduto(usuario_t *usuario, int nUsuarios, produto_t * PRODUTOS, int nProdutos) {

    int indiceUsuario = buscaUsuario(usuario, nUsuarios, false);
    int indiceProduto = escolherProdutoComId(*usuario);

    int nUnidades;
    int i;

    if (indiceUsuario == ERRO_BUSCAR_SEM_CORRESPONDENTE && usuario[indiceUsuario].nProdutos <= 0) {
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

            // Seta a mudança no vetor "PRODUTOS" totais também
            for (i = 0; i < nProdutos; i++) {
                if (PRODUTOS[i].Id == usuario[indiceUsuario].produtos[indiceProduto].Id) {   // Se os ID's forem iguais
                    PRODUTOS[i] = usuario[indiceUsuario].produtos[indiceProduto];            // O "produto" do vetor total agora é o mesmo do usuário
                    break;
                }
            }

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

// Apaga um produto do usuário
void apagarProduto(usuario_t * usuario, produto_t ** PRODUTOS, int * nProdutos) {
    int indiceProduto = escolherProdutoComId(* usuario);
    int i;
    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE)
    {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }

    for (i = indiceProduto; i < (* usuario).nProdutos - 1; i++) {
        usuario->produtos[i] = usuario->produtos[i + 1];        
    }

    // Seta a mudança no vetor "PRODUTOS" totais também
    for (i = 0; i < (*nProdutos); i++) {
        if ((*PRODUTOS)[i].Id == (*PRODUTOS)[indiceProduto].Id) {   // Se os ID's forem iguais
            indiceProduto = i;
            break;
        }
    }

    for (i = indiceProduto; i < (*nProdutos) - 1; i++) {
        (*PRODUTOS)[i] = usuario->produtos[i + 1];        
    }

    (usuario->nProdutos)--;
    (*nProdutos)--;
    usuario->produtos = (produto_t *)realloc(usuario->produtos, sizeof(produto_t) * (usuario->nProdutos));
    *PRODUTOS = (produto_t *)realloc(*PRODUTOS, sizeof(produto_t) * (*nProdutos));

    printf("O PRODUTO FOI REMOVIDO COM SUCESSO\n");
}

// Cadastra um produto novo em um usuário
void cadastraProduto(usuario_t * usuario, produto_t ** PRODUTOS, int * nProdutos) { 

    //Se não estiver alocado(o usuário não tem nenhum produto), aloca
    if (usuario->produtos == NULL) {
        
        usuario->produtos = malloc(sizeof(produto_t));
        if (usuario->produtos == NULL) {
            perror("Nao foi possivel alocar o vetor PRODUTOS");
            return;
        }

    } else if ((usuario->produtos = realloc(usuario->produtos, sizeof(produto_t) * (usuario->nProdutos + 1))) == NULL) {
        //Verifica novamente se alocou, caso positivo, realoca. Caso negativo, aloca e entra na condição positiva
        return;
    }

    produto_t produtoCriado; // Produto sendo cadastrado
    char tempLink[MAX_LINK];
    int i;

    produtoCriado.nAvaliacoes = 0;
    produtoCriado.avaliacoes = NULL;

    // Pede o nome do produto
    printf("Nome do produto: ");
    fgets(produtoCriado.nomeProduto, MAX_CHAR_NOME_PRODUTO, stdin);
    removeQuebra(produtoCriado.nomeProduto);

    // Pede a descrição do produto
    printf("Descricao do produto: ");
    fgets(produtoCriado.descricaoProduto, MAX_CHAR_DESCRICAO, stdin);
    removeQuebra(produtoCriado.descricaoProduto);
    
    // Pede as imagens do produto
    printf("Quantas imagens deseja colocar? ");
    scanf("%i%*c", &produtoCriado.nImagens);

    // Se o usuário for colocar imagens (nImagens > 0)
    if (produtoCriado.nImagens > 0) {

        // Caso o usuário queira postar a galeria de google fotos dele
        if (produtoCriado.nImagens > MAX_IMAGENS) {
            printf("O maximo de imagens eh %i. O numero de\nimagens agora eh %i.", MAX_IMAGENS, MAX_IMAGENS);
            produtoCriado.nImagens = MAX_IMAGENS;
        }

        // Aloca para o vetor de imagens
        produtoCriado.imagens = (asciiImg_t **) malloc(produtoCriado.nImagens * sizeof(asciiImg_t*));

        // Loop em todos os elementos do vetor "imagens"(de 0 até o número de imagens)
        for (i = 0; i < produtoCriado.nImagens; i++) {

            do { // Até conseguir uma imagem certa

                // Pede o link da imagem
                printf("Digite o link da %ia imagem: ", i+1);
                fgets(tempLink, MAX_LINK, stdin);
                removeQuebra(tempLink);

                // Carrega a imagem e coloca no vetor
                produtoCriado.imagens[i] = insta_carregaImagem(tempLink, IMAGENS_COLORIDAS, IMAGEM_PRODUTO_NUMERO_COLUNAS);
                if (produtoCriado.imagens[i] == NULL) {
                    // Não foi possível carregar a imagem
                    printf("Nao foi possivel carregar a imagem %d. Tente novamente:\n", i + 1);
                } else {
                    break; // Foi possível carregar a imagem
                }

            } while (produtoCriado.imagens[i] == NULL);

        }

    }

    // Pede o estoque do produto
    printf("Estoque do produto: ");
    scanf("%d%*c", &produtoCriado.estoque);

    // Coloca o ID do produto como a quantidade de produtos já criados
    produtoCriado.Id = *nProdutos;
    
    // Coloca o ID do dono do produto como o usuário logado agora
    strcpy(produtoCriado.idDono, usuario->ID);

    // Aloca para o novo produto & Seta para o produto que acabou de criar
    *PRODUTOS = realloc(*PRODUTOS, sizeof(produto_t) * (*nProdutos + 1));
    
    if ((*PRODUTOS) == NULL) {
        perror("Nao foi possivel realocar o vetor PRODUTOS");
        printf("Falha na criacao do produto.\n");
        return;
    }

    usuario->produtos[usuario->nProdutos] = produtoCriado;
    (*PRODUTOS)[*nProdutos] = produtoCriado;

    // Implementa a quantidade de produtos do usuário e de produtos totais
    (usuario->nProdutos)++;
    (*nProdutos)++;
}

// Valida um email (se tem @ e . depois do @)
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

// Verifica se um ID já existe
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

// Libera a memória ocupada pelas imagens de um produto
void liberaImagensProduto(produto_t * produto) {
    for (int i = 0; i < produto->nImagens; i++) {
        if (produto->imagens[i] != NULL) {
            insta_liberaImagem(produto->imagens[i]);
        }
    }
    free(produto->imagens);
}

// Edita um produto do usuário escolhido
void editarProduto(usuario_t *usuario, produto_t * PRODUTOS, int nProdutos) {
    
    // indice do produto selecionado pelo usuário
    int indiceProduto = escolherProdutoComId(* usuario);

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE)
    {
        printf("NAO HA PRODUTOS CADASTRADOS\n");
        return;
    }

    
    int escolha; // o que a pessoa escolheu editar
    int temp_val = -1;
    int i;
    char temp_sinal;
    char tempLink[MAX_LINK];

    produto_t * _produto = &usuario->produtos[indiceProduto]; // produto no usuário
    
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

                // Libera as imagens do produto
                liberaImagensProduto(_produto);

                // Pede a nova quantidade de imagens
                printf("Quantas imagens deseja colocar? ");
                scanf("%i%*c", &_produto->nImagens);
                
                // Caso o usuário queira postar a galeria de google fotos dele :D
                if (_produto->nImagens > MAX_IMAGENS) {
                    printf("O maximo de imagens eh %i. O numero de\nimagens agora eh %i.", MAX_IMAGENS, MAX_IMAGENS);
                    _produto->nImagens = MAX_IMAGENS;
                }

                // Se o usuário queria editar/colocar pelo menos uma imagem
                if (_produto->nImagens > 0) {

                    // Aloca para o vetor de imagens
                    _produto->imagens = (asciiImg_t **) malloc(_produto->nImagens * sizeof(asciiImg_t*));

                    // Loop em todos os elementos do vetor "imagens"(de 0 até o número de imagens)
                    for (i = 0; i < _produto->nImagens; i++) {

                        // Pede o link da imagem
                        printf("Digite o link da %ia imagem: ", i+1);
                        fgets(tempLink, MAX_LINK, stdin);
                        removeQuebra(tempLink);

                        // Carrega a imagem e coloca no vetor
                        _produto->imagens[i] = insta_carregaImagem(tempLink, IMAGENS_COLORIDAS, IMAGEM_PRODUTO_NUMERO_COLUNAS);
                    }

                    } else { // Libera todas as imagens já que o usuário quer 0 fotos(ou negativo)
                        liberaImagensProduto(_produto);
                    }

                

                printf("IMAGENS EDITADAS COM SUCESSO\n");
            
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

    // Seta a mudança no vetor "PRODUTOS" totais também
    for (i = 0; i < nProdutos; i++) {
        if (PRODUTOS[i].Id == _produto->Id) {   // Se os ID's forem iguais
            PRODUTOS[i] = *_produto;            // O "produto" do vetor total agora é o mesmo do usuário
            break;
        }
    }
}

//Função para fazer avaliação de um produto
void fazerAvaliacao(usuario_t *usuarios, int nUsuarios, produto_t * PRODUTOS, int nProdutos, int loginAtual) {

    
    int indiceUsuario = buscaUsuario(usuarios, nUsuarios, false);
    int indiceProduto = escolherProdutoComId(usuarios[indiceUsuario]);

    int nota, i;
    char avaliaTemp[MAX_CHAR_DESCRICAO];

    if(indiceProduto == ERRO_BUSCAR_SEM_CORRESPONDENTE) {
        printf("NAO HA PRODUTOS CADASTRADOS\n");            
        return;
    }

    produto_t * produtoAtual = &(usuarios[indiceUsuario].produtos[indiceProduto]);
    
    produtoAtual->avaliacoes = (avaliacao_t *)realloc(produtoAtual->avaliacoes, sizeof(avaliacao_t) * (produtoAtual->nAvaliacoes + 1));
   
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

    for (i = 0; i < nProdutos; i++) {
        if (PRODUTOS[i].Id == produtoAtual->Id) {   // Se os ID's forem iguais
            PRODUTOS[i] = *produtoAtual;            // O "produto" do vetor total agora é o mesmo do usuário
            break;
        }
    }
}

// Função para listar as avaliações de um produto
void listarAvaliacoes(usuario_t *usuarios, int nUsuarios) {

    int indiceUsuario = buscaUsuario(usuarios, nUsuarios, false);
    
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
    indiceUsuario = buscaUsuario(usuarios, nUsuarios, false);

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

// Lista os produtos favoritados do usuário atual
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
        printf("\n\n\tProduto\n\n4  - Cadastrar produto\n5  - Editar produto\n6  - Excluir produto\n7  - Comprar produto\n8  - Avaliar produto");
        printf("\n9  - Listar avaliacoes de um Produto\n10 - Favoritar um produto\n11 - Desfavoritar um produto\n12 - Listar os favoritos do usuario");
        printf("\n\n\tGerais\n\n13 - Buscar usuario\n14 - Listar produtos\n0  - Sair do programa\n\n");

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

// Desfavorita um produto do usuário atual
erro_t desfavoritarProduto(usuario_t *usuarios, int nUsuarios, int idUsuarioAtual) {
    int indiceUsuario, indiceProduto;

    usuario_t *usuarioAtual = &usuarios[idUsuarioAtual];

    indiceUsuario = buscaUsuario(usuarios, nUsuarios, false);

    indiceProduto = escolherProdutoComId(usuarios[indiceUsuario]);

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

/*      Arquivos      */

#define ERRO_ABRIR_ARQUIVO 1 // Caso não dê para abrir o arquivo
#define ERRO_LER_INFORMACAO 2 // Caso não dê para abrir o arquivo
// Salva o vetor "Usuarios" e o vetor "Produtos" em seus respectivos arquivos
erro_t salvarInformacoesArquivo(usuario_t * USUARIOS, int nUsuarios, produto_t * PRODUTOS, int nProdutos) {

    FILE * arquivo;
    int i, j;
    asciiImg_t * imagem;

    /* Salvar os usuários */

    // Abre o arquivo & verifica se abriu com sucesso
    arquivo = fopen(NOME_ARQUIVO_USUARIOS, "wb");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo de usuarios");
        return ERRO_ABRIR_ARQUIVO;
    }

    // Salva a quantidade de usuários
    fwrite(&nUsuarios, sizeof(int), 1, arquivo);

    // Salva o vetor de USUÁRIOS
    fwrite(USUARIOS, sizeof(usuario_t), nUsuarios, arquivo);

    // Faz um loop por todos os usuários, salvando seus favoritos
    for (i = 0; i < nUsuarios; i++) {
        fwrite(&(USUARIOS[i].nFavoritos), sizeof(int), 1, arquivo); // Salva a quantidade de favoritos
        if (USUARIOS[i].nFavoritos > 0) { // Se há favoritos, salva eles
            fwrite(USUARIOS[i].produtosFavoritos, sizeof(int), USUARIOS[i].nFavoritos, arquivo); // Salva o vetor "produtosFavoritos"    
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    /* Salvar os Produtos */

    arquivo = fopen(NOME_ARQUIVO_PRODUTOS, "wb");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo de produtos");
        return ERRO_ABRIR_ARQUIVO;
    }

    // Salva a quantidade de produtos
    fwrite(&nProdutos, sizeof(int), 1, arquivo);

    // Salva o vetor de PRODUTOS
    fwrite(PRODUTOS, sizeof(produto_t), nProdutos, arquivo);

    // Faz um loop por todos os produtos, salvando suas avaliações & imagens
    for (i = 0; i < nProdutos; i++) {
        fwrite(&(PRODUTOS[i].nAvaliacoes), sizeof(int), 1, arquivo); // Salva a quantidade de avaliacoes
        if (PRODUTOS[i].nAvaliacoes > 0) { // Se há avaliações, salva elas
            fwrite(PRODUTOS[i].avaliacoes, sizeof(avaliacao_t), PRODUTOS[i].nAvaliacoes, arquivo); // Salva o vetor "avaliacoes"
        }

        fwrite(&(PRODUTOS[i].nImagens), sizeof(int), 1, arquivo); // Salva a quantidade de imagens
        if (PRODUTOS[i].nImagens > 0) { // Se há imagens, salva elas
            // Para cada imagem, salvar os dados de asciiImg_t*
            for (j = 0; j < PRODUTOS[i].nImagens; j++) {

                imagem = PRODUTOS[i].imagens[j];

                // Salva o número de bytes da imagem
                fwrite(&(imagem->nBytes), sizeof(int), 1, arquivo);

                // Salva os bytes da imagem
                fwrite(imagem->bytes, sizeof(uint8_t), imagem->nBytes, arquivo);

            }
            
        }
    }

    // Fecha o arquivo
    fclose(arquivo);
}

erro_t obterInformacoesArquivo(usuario_t ** USUARIOS, int * nUsuarios, produto_t ** PRODUTOS, int * nProdutos) {

    FILE * arquivo;
    int i, j, idDono;
    produto_t * produtoAt; // produtoAtual
    usuario_t * usuarioAt; // usuarioAtual

    /* Obter os usuários */

    // Abre o arquivo & verifica se abriu com sucesso
    arquivo = fopen(NOME_ARQUIVO_USUARIOS, "rb");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo de usuarios");
        return ERRO_ABRIR_ARQUIVO;
    }

    // Lê a quantidade de usuários
    if (fread(nUsuarios, sizeof(int), 1, arquivo) <= 0) {
        // Não conseguiu ler a quantidade de usuários. (Provavelmente, o arquivo está vazio)
        perror("Nao foi possivel ler informacoes no arquivo");

        *nUsuarios = 0;

        return ERRO_LER_INFORMACAO;
    }

    // Aloca para guardar os usuários salvos
    *USUARIOS = (usuario_t *)malloc(sizeof(usuario_t) * (*nUsuarios));

    // Lê o vetor de USUÁRIOS
    if (fread(*USUARIOS, sizeof(usuario_t), *nUsuarios, arquivo) <= 0) {
        // Não conseguiu ler a quantidade de usuários
        perror("Nao foi possivel ler informacoes no arquivo");
        
        free(*USUARIOS); // Libera a memória armazenada :D

        *nUsuarios = 0;
        *USUARIOS = NULL;

        return ERRO_LER_INFORMACAO;
    }

    // Faz um loop por todos os usuários, salvando seus favoritos. Além disso, aloca para seus produtos
    for (i = 0; i < *nUsuarios; i++) {
        usuarioAt = &(*USUARIOS)[i]; // Seta o usuário atual como USUARIOS[i]

        fread(&(usuarioAt->nFavoritos), sizeof(int), 1, arquivo); // Lê a quantidade de favoritos

        usuarioAt->produtosFavoritos = (int *)malloc(sizeof(int) * usuarioAt->nFavoritos); // Aloca para os id's dos produtos favoritos

        if (usuarioAt->produtosFavoritos > 0) {
            fread(usuarioAt->produtosFavoritos, sizeof(int), usuarioAt->nFavoritos, arquivo); // Salva o vetor "produtosFavoritos"    
        } else {
            usuarioAt->produtosFavoritos = NULL;
        }

        // Aloca para os produtos
        usuarioAt->produtos = (produto_t *)malloc(sizeof(produto_t) * usuarioAt->nProdutos);
        usuarioAt->nProdutos = 0; 
        // Seta 0 o nProdutos para, quando for colocar os produtos no vetor de produtos,
        // já ter uma variável cuidando do tamanho. Note que essa variável voltará ao
        // valor certo(o valor antes de ser 0) durante a "colocação" dos produtos.
    }

    // Fecha o arquivo
    fclose(arquivo);

    /* Lê os Produtos */

    arquivo = fopen(NOME_ARQUIVO_PRODUTOS, "rb");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo de produtos");
        return ERRO_ABRIR_ARQUIVO;
    }

    // Lê a quantidade de produtos
    if (fread(nProdutos, sizeof(int), 1, arquivo) <= 0 ) {
        // Não foi possível ler a quantidade de produtos. (Provavelmente, o arquivo está vazio)
        perror("Nao foi possivel ler informacoes no arquivo");

        *nProdutos = 0;

        return ERRO_LER_INFORMACAO;
    }

    // Aloca para produtos
    *PRODUTOS = (produto_t *) malloc(sizeof(produto_t) * (*nProdutos));

    // Lê o vetor de PRODUTOS
    if (fread(*PRODUTOS, sizeof(produto_t), *nProdutos, arquivo) <= 0) {
        // Não conseguiu ler os produtos
        perror("Nao foi possivel ler informacoes no arquivo");
        
        free(*PRODUTOS); // Libera a memória armazenada :D

        *nProdutos = 0;
        *PRODUTOS = NULL;

        return ERRO_LER_INFORMACAO;
    }

    // Faz um loop por todos os produtos, lendo suas avaliações & imagens. Além disso, coloca-se no vetor do seu respectivo vendedor
    for (i = 0; i < *nProdutos; i++) {
        produtoAt = &(*PRODUTOS)[i];

        // Lê a quantidade de avaliações
        fread(&( produtoAt->nAvaliacoes ), sizeof(int), 1, arquivo); // lê a quantidade de avaliacoes

        // Verifica se há avaliações. Se há, procura por elas
        if (produtoAt->nAvaliacoes > 0) {
            // Aloca usando a quantidade de avaliações obtida
            produtoAt->avaliacoes = (avaliacao_t *)malloc(sizeof(avaliacao_t) * produtoAt->nAvaliacoes); // Aloca para as avaliações

            // Lê todas as avaliações
            fread(produtoAt->avaliacoes, sizeof(avaliacao_t), produtoAt->nAvaliacoes, arquivo); // lê o vetor "avaliacoes"
        }
        
        // Lê a quantidade de imagens
        fread(&(produtoAt->nImagens), sizeof(int), 1, arquivo); // lê a quantidade de imagens
        
        // Verifica se há imagens. Se há imagens, obtem elas
        if (produtoAt->nImagens > 0) {
            
            // Aloca usando a quantidade de imagens no arquivo
            produtoAt->imagens = (asciiImg_t **)malloc(sizeof(asciiImg_t *) * produtoAt->nImagens); // lê para as avaliações

            // Loop em todas as imagens no arquivo
            for (j = 0; j < produtoAt->nImagens; j++) {
                // Aloca para a imagem atual
                produtoAt->imagens[j] = (asciiImg_t *)malloc(sizeof(asciiImg_t));

                // Lê o tamanho
                fread(&(produtoAt->imagens[j]->nBytes), sizeof(int), 1, arquivo);
            
                // Aloca pro tamanho
                produtoAt->imagens[j]->bytes = (uint8_t *)malloc(sizeof(uint8_t) * produtoAt->imagens[j]->nBytes);

                // Lê os bytes e coloca no vetor de bytes
                fread(produtoAt->imagens[j]->bytes, sizeof(uint8_t), produtoAt->imagens[j]->nBytes, arquivo); // lê o vetor "imagens[j]"
            }

        }
        
        // Coloca o produto no vetor do seu vendedor
        idDono = buscarUsuarioPorId(*USUARIOS, *nUsuarios, produtoAt->idDono);
        (*USUARIOS)[idDono].produtos[(*USUARIOS)[idDono].nProdutos] = (*PRODUTOS)[i];

        (*USUARIOS)[idDono].nProdutos++;
    }

    // Fecha o arquivo
    fclose(arquivo);
}

int main(int argc, char ** argv) {

    usuario_t * usuarios = NULL;
    int nUsuarios = 0;

    produto_t * PRODUTOS = NULL;
    int nProdutos = 0;
    
    // Lê o que já está escrito nos arquivos e traz pro código
    obterInformacoesArquivo(&usuarios, &nUsuarios, &PRODUTOS, &nProdutos); 
    
    int loginAtual = USUARIO_SEM_LOGIN; // Sempre incia sem login
    int escolha = -1;
    int temp;
    
    do{

        // Mostra as opções para o usuário & retorna o que o usuário escolheu
        escolha = telaInicial(usuarios, loginAtual);

        // Casos de escolha
        switch (escolha)
        {

        case 1: // Cadastro
            cadastraUsuario(&usuarios, &nUsuarios);

            // Pergunta se vai fazer login (já que acabou de criar uma conta)
            printf("\n1 - Fazer Login\t\t2 - Nao fazer login\nDeseja fazer login? ");
            scanf("%i%*c", &temp);
            if (temp == 1) { // Se for fazer login
                loginUsuario(&loginAtual, usuarios, nUsuarios);
            }

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
            editarProduto(&usuarios[loginAtual], PRODUTOS, nProdutos);
            break;

        case 6: // Apagar um produto
            apagarProduto(&usuarios[loginAtual], &PRODUTOS, &nProdutos);
            break;

        case 7: // Comprar um produto
            compraProduto(usuarios, nUsuarios, PRODUTOS, nProdutos);
            break;

        case 8: // Fazer uma avaliação
            fazerAvaliacao(usuarios, nUsuarios, PRODUTOS, nProdutos, loginAtual);
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
            buscaUsuario(usuarios, nUsuarios, true);
            break;
        
        case 14://Lista todos os produtos de um usuário
            listarProdutos(usuarios, nUsuarios);
            break;
            
        case 0: // Encerra o programa
            break;
        }
        
    }while(escolha != 0); 
    
    // Salva as informações no arquivo
    salvarInformacoesArquivo(usuarios, nUsuarios, PRODUTOS, nProdutos);

    return SUCESSO;
}

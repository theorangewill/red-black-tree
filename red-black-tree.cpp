#include <stdio.h>
#include <string.h>
#define TAM 20

//Esta classe e a celula de uma lista encadeada
class Lista_Celula{
public:
    void imprimir(const char *sep);
    Lista_Celula(int num);

    Lista_Celula *prox;
    int linha;
};

//Esta classe e a cabeca de uma lista encadeada
class Lista{
public:
    Lista();
    void adiciona_na_lista(int num);

    Lista_Celula *inicio;
};

//Esta classe e o no de uma arvore
class Arvore_Celula{
friend class Arvore;
friend class Fila;
public:
    void adiciona_na_lista(int num);
    Arvore_Celula(char p[TAM+1]);
    Arvore_Celula();

    Lista ls;
private:
    Arvore_Celula *pai;
    Arvore_Celula *esq;
    Arvore_Celula *dir;
    char palavra[TAM+1];
    //Para facilidar as operacoes, foi determinado que, para cor = 1 equivale a cor PRETA
    //e para cor = 2 equivale a cor VERMELHA
    int cor;
};

//Esta classe eh a cabeca de uma arvore
class Arvore{
public:
    void printa(Arvore_Celula *x);
    void rotacao_esquerda(Arvore_Celula *x);
    void rotacao_direita(Arvore_Celula *x);
    Arvore_Celula* insercao(char p[TAM+1]);
    void corrige_insercao(Arvore_Celula *x);
    void imprime_linhas(Arvore_Celula *x);
    Arvore_Celula* busca(char p[TAM+1], Arvore_Celula *x);
    Arvore();
    Arvore_Celula *raiz;
    Arvore_Celula *nulo;
};

//Esta classe eh a celula de uma fila
class Celula_Fila{
friend class Fila;
public:
    Celula_Fila(Arvore_Celula *x);
private:
    Arvore_Celula *cel;
    Celula_Fila *prox;
};

//Esta classe eh a cabeca de uma fila
class Fila{
public:
    void printa(Arvore T);
    Fila(Arvore_Celula *x);
private:
    Celula_Fila *inicio;
};


void ler_arquivo(FILE *pt, Arvore *T);

int main(int argc,char *argv[])
{
    Arvore T;
    FILE *pt;

    //Abre o arquivo
    pt = fopen(argv[1],"r");

    //Leitura do arquivo, e impressao da arvore
    ler_arquivo(pt, &T);

    Fila F(T.raiz);
    //Imprime a arvore
    F.printa(T);

    //Imprime as palavras e as linhas em que se encontram
    T.imprime_linhas(T.raiz);
    delete T.nulo;

    return 0;
}

//Esta funcao le o arquivo de texto e insere em uma arvore cada palavra do texto
void ler_arquivo(FILE *pt, Arvore *T)
{
    Arvore_Celula *node;
    int c,i, linhas;
    char palavra[TAM+1];
    Lista_Celula *p;

    //A variavel 'linhas' indica em que linha esta a leitura do arquivo
    linhas = 1;

    //Verifica se o arquivo pode ser lido
    if(pt != NULL){
        //Atribui a 'c' o primeiro caracter  do arquivo
        c = fgetc(pt);

        //A variavel 'i' indica em que indice da string esta
        i=0;

        //Caminha o arquivo ate o final
        while(c != EOF){
            //Se 'c' eh '\n', entao pulou linha
            if(c == '\n')
                linhas++;
            else{
                //Caso seja letra maiuscula, convertemos para mnuscula
                if(c >= 65 && c <= 90)
                    c+=32;
                //Caso seja uma letra, adicionamos na palavra
                if(c >= 97 && c <= 122){
                    palavra[i] = c;
                    i++;
                }
                //Caso seja outro caracter
                else{
                    //Se a palavra tiver mais de 2 caracteres, eh valida
                    if(i >= 3){
                        palavra[i] = '\0';
                        //Busca a palavra na arvore
                        node = T->busca(palavra,T->raiz);
                        //Se nao existe a palavra na arvore
                        if(node == T->nulo)
                            //Insere a palavra na arvore
                            node = T->insercao(palavra);
             
                        //Cria-se uma lista encadeada com a linha em que ela foi encontrada, sem repetir
                        Lista_Celula *cel = new Lista_Celula(linhas);
			if(node->ls.inicio != NULL){
		            p = node->ls.inicio;
		       	    while(p->prox != NULL)
		                p = p->prox;
		            if(p->linha != linhas)
		                p->prox = cel;
			    else
			        delete cel;
			}
			else
			    node->ls.inicio = cel;
                    }
                    //Zera o indice da palavra
                    i=0;
                }
            }
            //Le a proxima letra
            c = fgetc(pt);
            }
        //Fecha o arquivo
        fclose(pt);
    }
}

//Construtor da classe Lista_Celula, em que coloca 'NULL'
//no campo 'prox' e o valor de 'num' no campo 'linha'
Lista_Celula::Lista_Celula(int num)
{
    prox = NULL;
    linha = num;
}

//Esta funcao imprime o valor do campo 'linha' de cada celula da lista
void Lista_Celula::imprimir(const char *sep)
{
  printf("%d%s", linha, sep);
}

//Construtor da clase Lista, em que coloca 'NULL' no campo 'inicio'
Lista::Lista()
{
    inicio = NULL;
}

//Construtor da classe Celula_Fila, em que coloca
//um no da arvore em uma fila
Celula_Fila::Celula_Fila(Arvore_Celula *x)
{
    cel = x;
    prox = NULL;
}

//Construtor da classe Fila
Fila::Fila(Arvore_Celula *x)
{
    inicio = new Celula_Fila(x);
}

//Esta funcao imprime na tela a arvore rubro-negra
void Fila::printa(Arvore T)
{
    //Executa apenas se a arvore nao for vazia
    if(T.raiz != T.nulo){
        Celula_Fila *p, *f, *c;
        //'p' sempre aponta para o inicio da fila,
        p = inicio;
        //'f' caminha a lista, adicionando novas celulas
        f = inicio;
        f->prox = new Celula_Fila(NULL);
        f = f->prox;

        //Executa enquanto 'p' e 'f' forem diferentes
        while(p!=f){
            do{
                //Verifica se tem filho esquerdo, se sim, adiciona na fila
                if(p->cel->esq != T.nulo){
                    printf("/");
                    f->prox = new Celula_Fila(p->cel->esq);
                    f = f->prox;
                }
                else
                    printf("*");

                //Imprime a palavra e sua respectiva cor, se 1, entao eh preto, se 2, entao eh vermelho
                if(p->cel->cor == 1)
                    printf("(%s [B])", p->cel->palavra);
                else
                    printf("(%s [R])", p->cel->palavra);

                //Verifica se tem filho direito, se sim, adiciona na fila
                if(p->cel->dir != T.nulo){
                    printf("\\ ");
                    f->prox = new Celula_Fila(p->cel->dir);
                    f = f->prox;
                }
                else
                    printf("* ");

                c = p;
                p = p->prox;

                //desaloca a Fila_Celula
                delete c;
                //Efetua ate acabar o nivel da arvore
            }while(p->cel != NULL);
            printf("\n");

            //Quando acaba um nivel da arvore, acrescenta um 'NULL'
            c = p;
            if(p != f){
		f->prox = new Celula_Fila(NULL);
          	f = f->prox;
                p = p->prox;
            }
            //desaloca a Fila_Celula
            delete c;
        }
    }
}

//Construtor da classe Arvore_Celula, iniciando todos os ponteiros em 'NULL'
//e sua 'cor' em '0' e seu campo 'palavra' a string 'n'
Arvore_Celula::Arvore_Celula(char p[TAM+1])
{
    strcpy(palavra,p);
    pai = NULL;
    esq = NULL;
    dir = NULL;
    cor = 0;
}

//Construtor da classe Arvore_Celula para o T.nulo da arvore
Arvore_Celula::Arvore_Celula()
{
    strcpy(palavra,"NULO");
    pai = NULL;
    esq = NULL;
    dir = NULL;
    cor = 1;
}

//Construtor da classe Arvore, iniciando seu no T.nulo e sua raiz em T.nulo
Arvore::Arvore()
{
    nulo = new Arvore_Celula();
    raiz = nulo;
}

//Esta funcao imprime cada palavra e as linhas em que
//a palavra esta presente no arquivo de texto, em ordem alfabetica
void Arvore::imprime_linhas(Arvore_Celula *x)
{
    Lista_Celula *p, *c;
    //Executa apenas se x nao for T.nulo
    if(x != nulo){
        //Imprime o de menor valor
        imprime_linhas(x->esq);

        printf("%s ", x->palavra);

        //
        p = x->ls.inicio;
        while(p->prox != NULL){
            p->imprimir(",");
            c = p;
            p = p->prox;
            //desaloca a Lista_Celula
            delete c;
        }
        p->imprimir("\n");
        //desaloca a Lista_Celula
        delete p;

        //Imprime o de maior valor
        imprime_linhas(x->dir);

        //desaloca a Arvore_Celula
        delete x;
    }
}

//Esta funcao busca uma palavra na arvore, e retorna o no em que esta presente
Arvore_Celula* Arvore::busca(char p[TAM+1], Arvore_Celula *x)
{
    int comp;
    //Se x for T.nulo, entao a palavra nao esta presente na arvore
    if(x == nulo)
        return nulo;
    //Se x nao for T.nulo, continua a buscar na arvore
    comp = strcmp(x->palavra,p);
    if(comp == 0)
        return x;
    else if(comp < 0)
        return busca(p,x->dir);
    else
        return busca(p,x->esq);
}

//Esta funcao recebe uma celula de uma arvore e rotaciona para a esquerda
void Arvore::rotacao_direita(Arvore_Celula *x)
{
    Arvore_Celula *y;

    y = x->esq;
    x->esq = y->dir;

    if(y->dir != nulo)
        y->dir->pai = x;

    y->pai = x->pai;

    if(x->pai == nulo)
        raiz = y;
    else if(x == x->pai->dir)
        x->pai->dir = y;
    else
        x->pai->esq = y;

    y->dir = x;
    x->pai = y;
}

//Esta funcao recebe uma celula de uma arvore e rotaciona para a direita
void Arvore::rotacao_esquerda(Arvore_Celula *x)
{
    Arvore_Celula *y;

    y = x->dir;
    x->dir = y->esq;

    if(y->esq != nulo)
        y->esq->pai = x;

    y->pai = x->pai;

    if(x->pai == nulo)
        raiz = y;
    else if(x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

//Esta funcao recebe uma celula e insere na arvore
Arvore_Celula* Arvore::insercao(char p[TAM+1])
{
    Arvore_Celula *x = new Arvore_Celula(p);
    Arvore_Celula *y;
    Arvore_Celula *z;

    y = nulo;
    z = raiz;

    //Verifica se a arvore e vazia
    while(z != nulo){
        y = z;
        //Caso a palavra a inserir for menor que a palavra em 'z', vai para a esquerda
        if(strcmp(x->palavra,z->palavra) < 0)
            z = z->esq;
        //Se nao, vai para a direita
        else
            z = z->dir;
    }

    x->pai = y;

    //Se a arvore esta vazia, 'x' eh raiz
    if(y == nulo)
        raiz = x;
    //Se nao, verifica se a palavra inserida eh menor que a palavra em 'y'
    else if(strcmp(x->palavra,y->palavra) < 0)
        y->esq = x;
    else
        y->dir = x;

    //O noh inserido eh folha, e recebe 'cor' '2' (vermelho)
    x->esq = nulo;
    x->dir = nulo;
    x->cor = 2;

    //Corrige a insercao, de acordo com as propriedade da arvore rubro-negra
    corrige_insercao(x);
    return x;
}

//Esta funcao balanceia a arvore apos a insercao,
//de acordo com as propriedades da arvore rubro-negra
void Arvore::corrige_insercao(Arvore_Celula *x)
{
    Arvore_Celula *y;

    //Se a cor do pai eh 2 (vermelho)
    while(x->pai->cor == 2){
        //Se o pai de x eh filho esquerdo
        if(x->pai == x->pai->pai->esq){
            y = x->pai->pai->dir;
            //Se o tio eh de cor 2 (vermelho)
            if(y->cor == 2){
                x->pai->cor = 1;
                y->cor = 1;
                x->pai->pai->cor = 2;
                x = x->pai->pai;
            }
            //Se nao
            else{
                //Se x eh filho direito, rotaciona para a esquerda
                if(x == x->pai->dir){
                    x = x->pai;
                    rotacao_esquerda(x);
                }
                //E entao, rotaciona para a direita
                x->pai->cor = 1;
                x->pai->pai->cor = 2;
                rotacao_direita(x->pai->pai);
            }
        }
        //Se nao pai de x eh filho direito
        else{
            y = x->pai->pai->esq;
            //Se o tio eh de cor 2 (vermelho)
            if(y->cor == 2){
                x->pai->cor = 1;
                y->cor = 1;
                x->pai->pai->cor = 2;
                x = x->pai->pai;
            }
            //Se nao
            else{
                //Se x eh filho direito, rotaciona para a esquerda
                if(x == x->pai->esq){
                    x = x->pai;
                    rotacao_direita(x);
                }
                //E entao, rotaciona para a direita
                x->pai->cor = 1;
                x->pai->pai->cor = 2;
                rotacao_esquerda(x->pai->pai);
            }
        }
    }
    //Impoe a cor da raiz em 1 (preto)
    raiz->cor = 1;
}

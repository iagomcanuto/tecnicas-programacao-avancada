#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector>
#include <queue>
#define NULL 0 
using namespace std;
struct Vizinho;
struct Vertice {
    int id;
    Vizinho *vizinhos;
};
struct Vizinho {
    Vertice *vizinho;
    Vizinho *proximoVizinho;
};
vector<Vertice> grafo;
void inicializa(int tamanho);
void adicionaVizinho(Vertice *a, Vertice *b);
void lerArquivo();
void criarGrafo();
void criarArquivoDOT(int vertices,bool direcionado);
bool conexidade(int vertices, bool direcionado);
void menu();
void plotarLista(int vertices);
void preencherListaAleatoria(int qtdArestas,int vertices,bool direcionado);

int main(){
    menu();
    return 0;
}

void inicializa(int tamanho){
    grafo.clear();
    grafo.resize(tamanho);

    for(int i = 0; i<tamanho ; i++){
        grafo[i].id = i;
        grafo[i].vizinhos = NULL;
    }

}

//falar que o B é vizinho de A
void adicionaVizinho(Vertice *a, Vertice *b){
    if (a == NULL || b == NULL){
        return;
    }
        Vizinho *temp = new Vizinho;
        temp->vizinho = b;
        temp->proximoVizinho = a->vizinhos;
        a->vizinhos = temp;
    //agora eu preciso fazer o mesmo processo para
    //fazer o A ser vizinho de B
    //Não é melhor só repetir a função com indices trocados?
}
void menu(){
    int op = 0;
    cout << "Escolha a opcao desejada \n";
    cout << "Opcao 1: Ler arquivo .DOT \n";
    cout << "Opcao 2: Criar arquivo .DOT\n";
    cout << "Opcao 3: Sair \n";
    cout << "Qual a opcao escolhida: ";

    cin >> op;
    switch(op){
        case 1:
        cout << "Escolheu a opcao 1 \n";
        lerArquivo();
        break;
        case 2:
        cout << "Escolheu a opcao 2\n";
        criarGrafo();
        break;
        case 3: 
        cout << "Escolheu a opcao 3\n";
        break;
        }
};

void lerArquivo(){
    ifstream arquivo;
    string linha;
    arquivo.open("grafo.dot");
    int v = 0;
    bool digrafo = false;
    if(arquivo.is_open()){
        while(getline(arquivo, linha)){
            if(linha.empty()){
                continue;
            }
            if(linha.find("graph") != string::npos || linha.find("{") != string::npos || linha.empty() || linha.find("digraph") != string::npos || linha.find("Grafo") != string::npos || linha.find("}") != string::npos ){
                if(linha.find("digraph") != string::npos){
                    digrafo = true;
                    continue;
                };
                continue;
            };
            if(linha.find("--")!= string::npos || linha.find("->") != string::npos){
                break;
            };
            v++;
        }
        cout << "Quantidade de vertices: ";
        cout<<v << endl;
        if (v <= 0) { 
            cout << "Grafo vazio (ou invalido)." << endl;
            return;
        }
        inicializa(v);
        arquivo.clear();
        arquivo.seekg(0,ios::beg);
        while (getline(arquivo,linha)){
            if(linha.find("--") == string::npos && linha.find("->") == string::npos){
                continue;
            }
            cout << linha;
            stringstream ss(linha);
            int i,j;
            char lixo;
            ss >> i >> lixo >> lixo >> j;
            if(digrafo){
                adicionaVizinho( &grafo[i], &grafo[j]);
            }else{
            adicionaVizinho( &grafo[i], &grafo[j]);
            adicionaVizinho( &grafo[j], &grafo[i]);
            }
        }
        plotarLista(v);
        if(conexidade(v,digrafo)){
            cout << "\n Grafo conexo";
        }else{
            cout << "\n Grafo Desconexo";
        }
    }else{
        cout << "Falha na leitura";
    }

    arquivo.close();
};

void plotarLista(int vertices){
    cout << "\n-- Grafo em Lista de Adjascencia --\n";
    for (int i = 0; i < vertices; i++) {
            cout << grafo[i].id << " --> ";

            Vizinho *atual = grafo[i].vizinhos;

            while (atual != NULL) {
                
                cout << atual->vizinho->id;


                if (atual->proximoVizinho != NULL) {
                    cout << " - ";
                } else {
                    cout << "|";
                }

                atual = atual->proximoVizinho;
            }
            cout << endl;
        }
};
bool conexidade(int vertices, bool direcionado){
    for(int i = 0; i < vertices; i++){
        vector<bool> caminho(vertices,false);
        queue<int> fila;
        fila.push(i);
        caminho[i] = true;
            int visitados = 0;
            while(!fila.empty()){
                int primeiro = fila.front();
                fila.pop();
                visitados++;
            
            Vizinho *aux = grafo[primeiro].vizinhos;
                while(aux != NULL){
                    if(!caminho[aux->vizinho->id]){
                        caminho[aux->vizinho->id] = true;
                        fila.push(aux->vizinho->id);
                    }
                    aux = aux->proximoVizinho;
                }
            }
        if(visitados != vertices){
            return false;
        }
        if(!direcionado){
            return true;
        }
    }
    return true;
};
void criarGrafo(){
    int vertices; 
    int tipo;
    bool direcionado = false;
    int maxArestas;
    int preenchimento;
    int qtdArestas;
    cout << "Escolha a quantidade de vertices ";
    cin >> vertices;
    if (vertices <= 0) { 
        cout << "Grafo vazio (ou invalido)." << endl;
         return;
    }
    inicializa(vertices);

    do{
    cout << "Escolha se o grafo e direcionado ou nao\n";
    cout << "1 - nao direcionado\n";
    cout << "2 - direcionado\n";
    cin >> tipo;
    }while(tipo != 1 && tipo != 2);

    direcionado = (tipo == 2);

    if(direcionado){
        maxArestas = vertices*(vertices - 1);
    }else{
        maxArestas =(vertices*(vertices - 1))/2;
    }
    cout << "A quantidade maxima de arestas e " << maxArestas << endl;
    cout << "Escolha a porcentagem de arestas de 0 a 100% ";
    cin >> preenchimento;
    if (preenchimento > 100){
        preenchimento = 100;
    };
    if (preenchimento < 0){
        preenchimento = 0;
    };

    qtdArestas = (int)((preenchimento/100.0)*maxArestas);
    
    cout << "Quantidade de arestas e " << qtdArestas << endl;
    srand(time(NULL));
    preencherListaAleatoria(qtdArestas,vertices,direcionado);
    plotarLista(vertices);
    criarArquivoDOT(vertices,direcionado);
};
void preencherListaAleatoria(int qtdArestas,int vertices,bool direcionado){
    int linha, coluna;
    int contagem = 0;

    while(contagem < qtdArestas){
        linha = rand() % vertices;
        coluna = rand() % vertices;
        bool arestaConectada = false;
        if(linha == coluna){
            continue;
        };
        Vizinho *aux = grafo[linha].vizinhos;
        while(aux != NULL){
            if(aux->vizinho->id == coluna){
            arestaConectada = true;
            break;
            }
            aux = aux->proximoVizinho;
        }
        if(arestaConectada){
            continue;
        }
        adicionaVizinho( &grafo[linha], &grafo[coluna]);
        if(direcionado == false){
            adicionaVizinho( &grafo[coluna], &grafo[linha]);
        }
        contagem ++;
    } 
};
void criarArquivoDOT(int vertices, bool direcionado){
    ofstream arquivo("grafo.dot");

    if(!arquivo.is_open()){
        cout<< "falha ao criar arquivo";
        return;
    }

    if(direcionado){
        arquivo << "digraph Grafo{\n";
    }else{
        arquivo << "graph Grafo{\n";
    }

    for(int i = 0 ; i < vertices; i++ ){
        arquivo << i <<";\n";
    }
    for(int i = 0 ; i < vertices; i++){
      Vizinho  *aux = grafo[i].vizinhos;
        while(aux != NULL){
            int j = aux->vizinho->id;
            if(direcionado){
            arquivo << i << "->" << j << ";\n";
            }else{
                if(i < j){
                arquivo << i << "--" << j << ";\n";
                } 
            }
            aux = aux->proximoVizinho;
        }
    }

    arquivo << "}\n";
    arquivo.close();
    cout<< " Arquivo .dot criado ";
    if(conexidade(vertices,direcionado)){
        cout << "\n Grafo conexo";
    }else{
        cout << "\n Grafo Desconexo";
    }
    system("dot -Tpng grafo.dot -o grafo.png");
};
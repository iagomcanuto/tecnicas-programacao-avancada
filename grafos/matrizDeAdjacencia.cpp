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

using namespace std;
void lerArquivo();
void menu();
void criarGrafo();
void preencherMatrizAleatoria(vector<vector<bool>> & matriz,int qtdArestas,int vertices,bool direcionado);
void plotarMatriz(const vector<vector<bool>> & matriz, int vertices);
void criarArquivoDOT(const vector<vector<bool>> & matriz, int vertices, bool direcionado);
void conexidade(const vector<vector<bool>> & matriz, int vertices);
int main(){
    menu();

    return 0;
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
    vector<vector<bool>> matriz(vertices, vector<bool>(vertices,false));
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
    preencherMatrizAleatoria(matriz,qtdArestas,vertices,direcionado);
    plotarMatriz(matriz,vertices);
    criarArquivoDOT(matriz,vertices,direcionado);
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
        if (v <= 0) { 
            cout << "Grafo vazio (ou invalido)." << endl;
            return;
        }
        cout<<v << endl;
        vector<vector<bool>> matriz(v, vector<bool>(v,false));
        while (getline(arquivo,linha)){
            if(linha.find("--") == string::npos && linha.find("->") == string::npos){
                continue;
            }
            stringstream ss(linha);
            int i,j;
            char lixo;
            ss >> i >> lixo >> lixo >> j;
            if(digrafo){
                matriz[i][j] = true;
            }else{
            matriz[i][j] = true;
            matriz[j][i] = true;
            }
        }
        plotarMatriz(matriz,v);
        conexidade(matriz,v);

    }else{
        cout << "Falha na leitura";
    }

    arquivo.close();


};

void preencherMatrizAleatoria(vector<vector<bool>> & matriz,int qtdArestas,int vertices,bool direcionado){
    int linha, coluna;
    int contagem = 0;

    while(contagem < qtdArestas){
        linha = rand() % vertices;
        coluna = rand() % vertices;

        if(linha == coluna){
            continue;
        };
        if(matriz[linha][coluna] == true){
            continue;
        };

        matriz[linha][coluna] = true;
        if(direcionado == false){
            matriz[coluna][linha] = true;
    }
    contagem ++;
    } 
};
void plotarMatriz(const vector<vector<bool>> & matriz, int vertices){
    cout << "\n-- Grafo em matriz --\n";
    cout << "    ";
    for(int i = 0 ; i < vertices ; i++){
        cout << i << " ";
    }
    cout << endl;
    cout << "  ";
    for(int i = 0 ; i < vertices ; i++){
        cout <<  "--";
    }
    cout << endl;
    for(int i = 0 ; i < vertices ; i++){
        cout << i << " | ";  
        for(int j = 0; j < vertices; j++){
            cout << matriz[i][j] << " ";
        }
        cout<<endl;
    }  
};
void criarArquivoDOT(const vector<vector<bool>> & matriz, int vertices, bool direcionado){
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
        arquivo << "\t" << i <<";\n";
    }

    for(int i = 0 ; i < vertices; i++){
        for(int j = 0 ; j < vertices; j++){
            if(matriz[i][j]){
                if(direcionado){
                    arquivo << "\t" << i << " -> " << j << ";\n"; 
                }else{
                    if(j>=i){
                        arquivo <<"\t" << i << " -- " << j << ";\n";

                    }
                }
            }
        }
    }
    arquivo << "}\n";
    arquivo.close();
    cout<< " Arquivo .dot criado ";
    conexidade(matriz,vertices);
    system("dot -Tpng grafo.dot -o grafo.png");
};

void conexidade(const vector<vector<bool>> & matriz, int vertices){
    vector<bool> caminho(vertices,false);
    queue<int> fila;

    fila.push(0);
    caminho[0] = true;

    int visitados = 0;
    while(!fila.empty()){
        int primeiro = fila.front();
        fila.pop();
        visitados++;
        for(int i = 0; i<vertices; i++){
            if(matriz[primeiro][i] && !caminho[i]){
                caminho[i] = true;
                fila.push(i);
            }
        }
        
    }
    if(visitados == vertices){
        cout << " Grafo Conexo";
    }else{
        cout << "Grafo desconexo";
    }
};
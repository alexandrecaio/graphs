#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#define tam 1000 //definindo um tamanho máximo de nós
using namespace std;
typedef struct ord{ //estrutura para ordenar a saída de acordo com a entrada
  int v1,v2;
  int ordem;
}ord;
//todos as matrizes usados para abstrair as arestas, pesos e flows
int G[tam][tam];
int Gr[tam][tam];
int Grflow[tam][tam];
bool mark[tam];
int excess[tam];
int height[tam];
int pre[tam];
ord flag[tam];
int U=0;

int pathLenght(int tag,int source, int sink){//função que busca subcaminhos aumentantes, a partir de uma BFS
    for(unsigned int i=1;i<tam;i++)
        mark[i] = false;

    int cont=0,conta=0,achou=0;
    queue<int>Q;

    Q.push(tag);
    mark[tag] = true;
    while (!Q.empty()){
        int u = Q.front();
        //cout<< "u = "<<u<<endl;
        Q.pop();
        for (int i=1; i<=tam; i++) {
            //cout<<"Loading...   "<<i<<endl;
            if (!mark[i] && Gr[u][i] > 0) {
                //cout<< "Verificando aresta ("<<u<<","<<i<<")"<<endl;
                pre[i]=u;
                Q.push(i);
                mark[i] = true;
                if (i==sink) {
                        queue<int> Q;
                        i=tam;
                }
            }
        }
    }

        int it=sink;
        while(pre[it]!=tag){
            conta++;
            it=pre[it];
        }
        if (pre[it]==tag) conta++;



    return conta; //se o sumidouro foi marcado, significa que existe um caminho da fonte ao sumidouro
}

int heighmin(int u) {
        int h = 9999999992;
        for (int i = 0 ; i<= tam; i++) {
            if (Gr[u][i]>0){
                if (height[i]<h) h=height[i];
            }

        }

        return h;

}

void pushrelabelmy(int source, int sink){
    int max_flow = 0,p,i,m;
    for (i = 0; i < tam; i++)
        for (int j = 0; j < tam; j++)//criando o grafo residual, que serve como estrutura fundamental
             Gr[i][j] = G[i][j];     // para manutenção dos fluxos

    int list[tam];     //MUDAR DEPOIS O INDEXAMENTO DE list
    vector<int>Path[tam]; //vector para guardar os vértices visitados

    //=================== PreFlow Operation ==================
    for (i = 0, p = 0; i < tam; i++){
        if((i != source) && (i != sink)) {
          list[p] = i;
          p++;
        }
    }
    for (unsigned int i = source+1; i < sink; i++) {
            height[i]= pathLenght(i,source,sink);
            cout<<"h["<<i<<"]:"<<height[i]<<endl;

    }
    height[source] = sink;
    height[sink]   = 0;
    excess[source] = 99999;
    queue<int>Q;
    for (i = 1 ; i < tam; i++) {
      if (G[source][i]>0){
            m = G[source][i];
            Gr[source][i] -= m;
            Grflow[source][i] += m;
            Gr[i][source] += m;
          //  Grflow[i][source] -= m;
            excess[i]+=m;
            cout<<"f("<<source<<","<<i<<")="<<m<<endl;
            Q.push(i);

            //max_flow+=m;
        }
      }

    while(Q.size()>0){
        int u = Q.front();
        Q.pop();
        //cout<<"sai da fila "<<u<<endl;
//=================== PUSHHH ==================
//              (u,i)=(v,w)
    for (i = 1 ; i <= tam; i++) {
        int k=-3;
        if (Gr[u][i]>0 && u!=sink && u!=source ){
            int m = min(excess[u],Gr[u][i]);
           // cout<< "m: min(x("<<u<<")= "<<excess[u]<<",c'("<<u<<","<<i<<")="<<(G[u][i]-Grflow[u][i])<<")"<<endl;
            if (height[u]==height[i]+1){
                excess[u]-= m;
                excess[i]+= m;
                if (G[u][i]>0&&Gr[u][i]>0) k=1; else k=-1;
                Gr[u][i] -= m;
                Grflow[u][i] += m*k;
                Gr[i][u] += m;
               // Grflow[i][u] -= m*k;
               // cout<<"Atualizando f("<<u<<","<<i<<")="<<m<<endl;
                max_flow += m*k;
                if (i!=sink && i!=source){
                  //  cout<<"Entrou na fila: "<<i<<endl;
                    Q.push(i);
                }
            }



        }
    }

    //=================== RELABEL ==================
    if (excess[u]>0 && u!=sink && u!=source){
       // cout<<"Relabel h[u]:"<<height[u]<<" to ";
        height[u]=1+heighmin(u);
        //cout<<height[u]<<endl;
        Q.push(u);

    }


  }

 //retorna o maior fluxo possível
}



int min(int x,int y){
    if (x<y){
        return x;
    }
    else return y;
}

void imprime_flow(){//imprime os fluxos resultantes, no formato [C1,C2...,Cn]
  bool g=true;
 cout << "[";
    for(unsigned int k=0;k<U;k++){
       if(!g) cout << ",";
        else g = false;
         if(Grflow[flag[k].v1][flag[k].v2]>0)
           cout << Grflow[flag[k].v1][flag[k].v2];
            else  cout << 0;
    }
  cout << "]" << endl;
}
void read_input_file(){//ler o arquivo de entrada e constrói o grafo
    string line;
    ifstream input ("ffflow.txt");
        for(unsigned int k=0;k<tam;k++){
        for(unsigned int t=0;t<tam;t++){
            G[k][t] = 0;
            Grflow[k][t] = 0;
        }
    }
    int v1,v2,cap;
    while(!input.eof()){
    getline(input, line);
     stringstream ss(line);
    ss >> v1 >> v2 >> cap;
    cout << v1 << "--" << v2 << " cap: " << cap << endl;
    G[v1][v2] = cap;
    //a flag abaixo, será usada para imprimir na ordem do arquivo
    flag[U].v1 = v1;
    flag[U].v2 = v2;
    flag[U].ordem = U++;
  }
    input.close();
}
int main(){
    int source=1,sink=6;
    read_input_file();
    //imprime o maior fluxo:
    //os vértices de fonte e destino, devem ser modificados das variávies source e sink, respectivamente.
   // cout << "Fluxo maximo: " << pushrelabelmy(1, 6)  << endl;
   pushrelabelmy(1, 6);
    imprime_flow();
    return 0;
}

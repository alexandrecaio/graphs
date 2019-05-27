#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstdlib>

#define NAO_VISITADO 0
#define ROTULADO 1
#define EXAMINADO 2


using namespace std;
typedef struct vertex{ //Estrutura de abstração de um vértice
  char name;
  bool mark;
  bool enfileirado;
  int estado;
  int distance;
  vertex *pred; 
  vector<vertex*>adj;
} vertex;
 
typedef struct edges{ //Estrutura de abstração de uma aresta
     vector<vertex*>ext;
     vertex *v,*w;
     char v_name,w_name;
	 int weight;
	 bool mark;
	 char edge_name;
}edges;

typedef struct Graph{ //Estrutura de abstração de um grafo
   vector<edges>aG;
   vector<vertex*>VG;
}Graph;

bool is_in(Graph *G,vector<vertex*>adj,char v,int mode){//Função para checar se o vértice já foi adicionado
    if(mode) for(size_t i=0;i<G->VG.size();i++)
            if(G->VG[i]->name == v) return true; //retorna true se já tiver sido adicionado

    if(!mode) for(size_t i=0;i<adj.size();i++)
                if(adj[i]->name == v) return true;

    return false; //retorna false se não tiver sido adicionado
}
void make_graph(ifstream &input,Graph *G,int n, int m){ //Montando o grafo a partir da matriz de incidência
   int k;
   vertex *v = (vertex*)new vertex[n]; //Alocando espaço para os n vértices
   edges *ed = (edges*)new edges[m]; //Alocando espaço para as m arestas

   for(int i=0;i<n;i++){
        v[i].name = (char)65+i; //nome para os vértices
        v[i].mark = false;
     for(int j = 0;j<m && input.is_open();j++){ //lendo a matriz de incidência e criando as arestas
        input >> k;
        if(k)
            ed[j].ext.push_back(&v[i]); //v[i] será um dos extremos da aresta ed[j]
       }
  }
  int op;
  cout << "definir pesos aleatorios? 1 = Sim, 0 = Nao" << endl;
  cin >> op;
  for(int i =0;i<m;i++){ //Finalizando a montagem do grafo
       ed[i].v = ed[i].ext[0];
       ed[i].v_name = ed[i].ext[0]->name;
       ed[i].w = ed[i].ext[1];
       ed[i].w_name = ed[i].ext[1]->name;

        ed[i].edge_name = (char)(97+i); //Dando nome as arestas
       if(!op){
        cout << "Digite o peso da aresta: " << ed[i].v_name << "-" << ed[i].edge_name << "-" << ed[i].w_name << endl;
        cin >>  ed[i].weight; //definindo pesos
       }
       else ed[i].weight = rand()%9;

       if(!is_in(G,ed[i].v->adj,ed[i].w->name,0)) ed[i].v->adj.push_back(ed[i].w);
       if(!is_in(G,ed[i].w->adj,ed[i].v->name,0)) ed[i].w->adj.push_back(ed[i].v);

       if(!is_in(G,ed[i].v->adj,ed[i].v->name,1)) G->VG.push_back(ed[i].v);
       if(!is_in(G,ed[i].w->adj,ed[i].w->name,1)) G->VG.push_back(ed[i].w);

       G->aG.push_back(ed[i]);
}
  cout << endl << "Adjacencias: " << endl;
  for(size_t i=0;i<G->VG.size();i++){
        cout << "Vertice: " << G->VG[i]->name << " adj -> ";
     for(size_t j = 0;j<G->VG[i]->adj.size();j++){
      cout << G->VG[i]->adj[j]->name << ", ";
     }
    cout << endl;
  }
  cout << endl << "Lista das arestas do grafo: " << endl;
  for(int i=0;i<m;i++)
  cout << G->aG[i].ext[0]->name << "-" << G->aG[i].edge_name << "-" << G->aG[i].ext[1]->name<< " = " << G->aG[i].weight <<endl;
}
int peso(vector<edges>aG,char v, char w,int m){//Função que procura o peso da aresta correspondentes aos vértices v e w
	for(int i=0;i<m;i++){
		if((aG[i].v_name == v  && aG[i].w_name == w)
          || (aG[i].v_name == w  && aG[i].w_name == v)){
		  return aG[i].weight;
		}
	}
	return 0;
}


void bellman(Graph *G,vertex *v){
	
	
	queue<vertex*> Q; 
	vector<vertex*> VG=G->VG;
	
	for (size_t i =0;i<VG.size();i++){
		VG[i]->mark=false;
		VG[i]->distance=99999999;
		VG[i]->pred=NULL;
		VG[i]->enfileirado=false;
	}
	//getchar();
	v->estado=ROTULADO;
	v->distance=0;
	Q.push(v);
	v->enfileirado=true;
	//getchar();
	while(!Q.empty()){
			vertex *u = Q.front();//RECEBE O PRIMEIRO VERTICE DA FILA
			
			for(size_t i=0;i< u->adj.size() ;i++) { // PERCORRE TODOS VERTICES ADJACENTES
				int weight = peso(G->aG,u->name,u->adj[i]->name,G->aG.size()); // CALCULO DO PESO DA ARESTA (u,w) w==adj[i]
				if (u->distance + weight < u->adj[i]->distance){
					u->adj[i]->estado = ROTULADO;
					u->adj[i]->distance = u->distance+weight;
					u->adj[i]->pred = u;
					
				if (u->adj[i]->enfileirado==false){
						Q.push(u->adj[i]);
						u->adj[i]->enfileirado=true;
					}
					
				}
			}
					
			Q.pop(); //REMOVE O PRIMEIRO DA FILA
			u->enfileirado=false;
			u->estado=EXAMINADO;
		
			cout<<"Distancia Raiz ate "<<u->name<<" = "<<u->distance<<endl;
			
	}

}

int main(){
 ifstream input("grafo_input.txt");//lendo a matriz de incidência

//================================//Criando o grafo
	  Graph *G = (Graph*)new Graph; //Alocando espaço para o grafo
	  int n,m;
	  input >> n; //número de vértices
	  input >> m; //número de arestas
	  make_graph(input,G,n,m); //função que cria o grafo a partir da matriz de incidência
 //================================
  bellman(G,G->VG[0]); //Bellman recebe o grafo criado, o conjunto de arestas
  //		
 return 0;
}

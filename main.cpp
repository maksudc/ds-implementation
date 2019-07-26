#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


typedef char VAL_TYPE;

class Edge;
class Node;

class Edge{
public:

    Node *in;
    Node *out;
    int weight;

    Edge(Node *_in, Node *_out, int _weight){

        in = _in;
        out = _out;
        weight = _weight;
    }

    Node *getNeighbor(Node *node){

        if(in == node){
            return out;
        }
        return in;
    }
};

class Node{

public:

    VAL_TYPE value;
    vector<Edge *> edges;

    Node(int _value){
        value = _value;
    }
};

struct EdgeSortComparator{
public:
    bool operator()(Edge *edge1, Edge *edge2){
        return edge1->weight < edge2->weight;
    }
};

class Graph{

private:

    Node *Union_Find(map<Node *, Node *> &parentMap, Node *node);
    Node *Union_Join(map<Node *, Node *> &parentMap, Node *node1, Node *node2);

    void printKruskal(vector<Edge *> minimumSpanningTree);
public:

    vector<Node *> V;
    vector<Edge *> E;

    Graph(){

    }

    void addNode(Node *node){
        V.push_back(node);
    }

    void addEdge(Node *node1, Node *node2, int weight){

        Edge *edge = new Edge(node1, node2, weight);
        E.push_back(edge);

        node1->edges.push_back(edge);
        node2->edges.push_back(edge);
    }

    int kruskal();
    int prim();

    ~Graph(){

        for(unsigned int I=0; I < V.size(); I++){
            delete V[I];
        }
        V.clear();

        for(unsigned int J=0; J < E.size(); J++){
            delete E[J];
        }
        E.clear();
    }
};

Node *Graph::Union_Find(map<Node *, Node *> &parentMap, Node *node){
    //@TODO: Union-Find node

    Node *current = node;
    while(parentMap[current] != current){
        current = parentMap[current];
    }

    return current;
}

Node *Graph::Union_Join(map<Node *, Node *> &parentMap, Node *node1, Node *node2){
    //@TODO: Union-Join node

    Node *parent1 = this->Union_Find(parentMap, node1);
    Node *parent2 = this->Union_Find(parentMap, node2);

    parentMap[parent2] = parent1;
}


void Graph::printKruskal(vector<Edge *> minimumSpanningTree){

    cout << "kruskal edges: " << endl;
    for(unsigned int I=0; I < minimumSpanningTree.size(); I++){
        cout << minimumSpanningTree[I]->in->value << "-" << minimumSpanningTree[I]->out->value << " weight: " << minimumSpanningTree[I]->weight << endl;
    }
}


int Graph::kruskal(){

    vector<Edge *> minimumSpanningTree;

    sort(this->E.begin(), this->E.end(), EdgeSortComparator());

    map<Node *, Node *> parentMap;
    for(unsigned int I=0; I < this->V.size(); I++){
        parentMap[V[I]] = V[I];
    }

    for(unsigned int I=0; I < this->E.size(); I++){
        Edge *edge = E[I];

        Node *in = edge->in;
        Node *out = edge->out;

        if(Union_Find(parentMap, in) != Union_Find(parentMap, out)){
            minimumSpanningTree.push_back(edge);
            this->Union_Join(parentMap, in, out);
        }
    }

    printKruskal(minimumSpanningTree);

    int totalWeight = 0;
    for(unsigned int I=0; I < minimumSpanningTree.size(); I++){
        totalWeight += minimumSpanningTree[I]->weight;
    }

    return totalWeight;
}

int Graph::prim(){
    
    for(unsigned int I=0; I < this->V.size(); I++){
        V[I]->distance = numeric_limits<int>::max();
    }
    
    Node *source = this->V[0];
    source->distance = 0;
    
    map<Node *, bool> insideQMap;
    priority_queue<Node *, vector<Node *>, NodeComparator> Q;
    
    for(unsigned int I=0; I < this->V.size(); I++){
        Q.push(V[I]);
        insideQMap[V[I]] = true;
    }
    
    int remainingNodeCount = V.size();
    int totalWeight = 0;
    
    while(!Q.empty() && remainingNodeCount > 0){
        
        Node *current = Q.top();
        Q.pop();
        remainingNodeCount--;
        
        totalWeight += current->distance;
    }
    
    while(!Q.empty()){
        Q.pop();
    }
    
    return totalWeight;
}

int main()
{

    Graph *graph = new Graph();

    const int NODE_COUNT = 9;

    map<VAL_TYPE, Node *> nodeMap;

    for(VAL_TYPE c='a'; c <= 'i'; c++){

        Node *node = new Node(c);
        nodeMap[c] = node;

        graph->V.push_back(node);
    }

    const VAL_TYPE ins[] = {'a', 'a', 'b', 'b', 'c', 'c', 'c', 'd', 'd', 'e', 'f', 'g', 'g', 'h'};
    vector<VAL_TYPE> inNodeVals(ins, ins + sizeof(ins)/sizeof(ins[0]));

    const VAL_TYPE outs[] = {'b', 'h', 'c', 'h', 'f', 'd', 'i', 'f', 'e', 'f', 'g', 'i', 'h', 'i'};
    vector<VAL_TYPE> outNodeVals(outs, outs + sizeof(outs)/sizeof(outs[0]));

    const int weightsArr[] = {
        4,8,8,11,4,7,2,14,9,10,2,6,1,7
    };
    vector<int> weights(weightsArr, weightsArr + sizeof(weightsArr)/sizeof(weightsArr[0]));

    int edgeCount = weights.size();

    for(unsigned int I=0; I < edgeCount; I++){
        graph->addEdge(nodeMap[ins[I]], nodeMap[outs[I]], weights[I]);
    }

    cout << "kruskal: " << graph->kruskal() << endl;
    cout << "prim: " << graph->prim() << endl;

    delete graph;

    return 0;
}

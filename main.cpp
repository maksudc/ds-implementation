#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <stack>
#include <queue>

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
    int distance;

    Node(int _value){
        value = _value;
        distance = numeric_limits<int>::max();
    }
};

struct EdgeSortComparator{
public:
    bool operator()(Edge *edge1, Edge *edge2){
        return edge1->weight < edge2->weight;
    }
};

struct NodeComparator{
public:
    bool operator()(Node *node1, Node *node2){
        return node1->distance > node2->distance;
    }
};

class Graph{

private:

    Node *Union_Find(map<Node *, Node *> &parentMap, Node *node);
    Node *Union_Join(map<Node *, Node *> &parentMap, Node *node1, Node *node2);

    void printMST(vector<Edge *> minimumSpanningTree);
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


void Graph::printMST(vector<Edge *> minimumSpanningTree){

    cout << "edges: " << endl;
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

    printMST(minimumSpanningTree);

    int totalWeight = 0;
    for(unsigned int I=0; I < minimumSpanningTree.size(); I++){
        totalWeight += minimumSpanningTree[I]->weight;
    }

    return totalWeight;
}

int Graph::prim(){

    map<Node *, Edge *> lightEdgeMap;
    map<Node *, bool> insideQMap;
    priority_queue<Node *, vector<Node *>, NodeComparator> Q;

    Node *source = this->V[0];
    source->distance = 0;

    for(unsigned int I=0; I < this->V.size(); I++){

        Q.push(V[I]);
        insideQMap[V[I]] = true;
        lightEdgeMap.insert(pair<Node * , Edge *>(V[I], NULL));
    }

    int remainingNodeCount = V.size();
    int totalWeight = 0;

    vector<Edge *> minimumSpanningTree;

    cout << "starting Q processing: " << endl;

    while(!Q.empty() && remainingNodeCount > 0){

        Node *current = Q.top();
        Q.pop();

        if(!insideQMap[current]){
            continue;
        }

        cout << "current: "<< current->value << " distance: " << current->distance << endl;

        if(current != source){
            minimumSpanningTree.push_back(lightEdgeMap[current]);
            totalWeight += lightEdgeMap[current]->weight;
        }

        insideQMap[current] = false;
        remainingNodeCount--;

        for(unsigned int I=0; I < current->edges.size(); I++){

            Edge *edge = current->edges[I];
            Node *other = edge->getNeighbor(current);

            if(insideQMap[other]){

                if(other->distance > edge->weight){
                    other->distance = edge->weight;
                    lightEdgeMap[other] = edge;
                }

                cout << "other: " << other->value << " distance: " << other->distance << endl;

                Q.push(other);
            }
        }
    }

    while(!Q.empty()){
        Q.pop();
    }

    printMST(minimumSpanningTree);

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

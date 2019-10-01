#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <stack>
#include <queue>
#include <cmath>

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

class Heap{

    vector<Node *> arr;
    map<Node *, int> positionMap;

public:

    Heap(vector<Node *> _arr){
        arr = _arr;

        for(int I=1; I <= arr.size(); I++){
            Node *node = arr[indexOf(I)];
            positionMap[node] = I;
        }

        build_heap();
    }

    void build_heap(){

        int N = size();
        for(int I=(int)floor(N/2); I > 0; I--){
            min_heapify(I);
        }
    }

    Node *getMin(){
        if(!empty()){
            return arr[indexOf(1)];
        }
        return NULL;
    }

    Node *extractMin(){

        Node *minNode = getMin();

        cout << "min: " << minNode->value << " key: " << minNode->distance << endl;

        Node *lastNode = getAt(size());

        cout << "swap: " << lastNode->value << " key: " << lastNode->distance << endl;

        arr[indexOf(size())] = minNode;
        arr[indexOf(1)] = lastNode;
        arr.pop_back();

        positionMap[lastNode] = 1;

        positionMap.erase(positionMap.find(minNode));

        min_heapify(1);

        return minNode;
    }

    void decreaseKey(Node *node, int key){

        if(positionMap.find(node) != positionMap.end()){
            if(key < node->distance){
                node->distance = key;
            }

            int position = positionMap[node];
            Node *current = node;

            Node *parent = getParent(position);
            position = positionMap[parent];

            while(parent != NULL){

                min_heapify(position);

                parent = getParent(position);
                if(parent != NULL){
                    position = positionMap[parent];
                }
            }
        }
    }

    void min_heapify(int position){

        Node *left = getLeft(position);
        Node *right = getRight(position);

        Node *current = getAt(position);

        if(current == NULL){
            return;
        }

        Node *minNode = current;
        int minPosition = position;

        cout << "current: " << current->value << ":" << current->distance << endl;
        cout << "left: " << left->value << ":" << left->distance << endl;
        cout << "right: " << right->value << ":" << right->distance << endl;

        if(left != NULL){
            if(current->distance > left->distance){
                minNode = left;
                minPosition = 2 * position;
            }
        }

        if(right != NULL){
            if(minNode->distance > right->distance){
                minNode = right;
                minPosition = 2 * position + 1;
            }
        }

        if(minPosition != position){

            Node *temp = getAt(minPosition);

            arr[indexOf(minPosition)] = current;
            arr[indexOf(position)] = minNode;

            positionMap[current] = minPosition;
            positionMap[minNode] = position;

            min_heapify(minPosition);
        }

        cout << "Heapify compplete " << endl;
    }

    void push(Node *node){

        arr.push_back(node);
        positionMap[node] = size();

        int temp = node->distance;
        node->distance = numeric_limits<int>::max();

        decreaseKey(node, temp);
    }

    Node *getAt(int position){
        if(indexOf(position) < size()){
            return arr[indexOf(position)];
        }
        return NULL;
    }

    int getPosition(Node *node){

        if(positionMap.find(node) != positionMap.end()){
            return positionMap[node];
        }

        return NULL;
    }

    Node *getParent(int position){
        int parentPosition = (int)floor(position/2);
        if(indexOf(parentPosition) >= 0 && indexOf(parentPosition) < size()){
            return arr[indexOf(parentPosition)];
        }
        return NULL;
    }

    int indexOf(int position){
        return position-1;
    }

    Node *getLeft(int position){
        if( indexOf(2 * position) < size()){
            return arr[indexOf(2 * position)];
        }
        return NULL;
    }

    Node *getRight(int position){
        if( indexOf(2 * position + 1) < size()){
            return arr[indexOf(2 * position + 1)];
        }
        return NULL;
    }

    int size(){
        return arr.size();
    }

    bool empty(){
        return size() == 0;
    }
};

int Graph::prim(){

    map<Node *, Edge *> lightEdgeMap;
    map<Node *, bool> insideQMap;
    //priority_queue<Node *, vector<Node *>, NodeComparator> Q;

    Node *source = this->V[0];
    source->distance = 0;

    for(unsigned int I=0; I < this->V.size(); I++){

        //Q.push(V[I]);
        insideQMap[V[I]] = true;
        lightEdgeMap.insert(pair<Node * , Edge *>(V[I], NULL));
    }

    Heap heap(this->V);

    int remainingNodeCount = V.size();
    int totalWeight = 0;

    vector<Edge *> minimumSpanningTree;

    cout << "starting Q processing: " << endl;

    while(!heap.empty()){ //&& remainingNodeCount > 0){

        Node *current = heap.extractMin();//Q.top();
        //Q.pop();

        //if(!insideQMap[current]){
        //    continue;
        //}

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

                    heap.decreaseKey(other, edge->weight);
                    //other->distance = edge->weight;
                    lightEdgeMap[other] = edge;
                }

                cout << "other: " << other->value << " distance: " << other->distance << endl;

                //Q.push(other);
            }
        }
    }

    //while(!Q.empty()){
    //    Q.pop();
    //}

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

#ifndef _DEFINE_ROUTING_
#define _DEFINE_ROUTING_

#include <string>
#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>
#include <map>
#include <iomanip>
#include <assert.h>     /* assert */
#include "myMinHeap.h"

using namespace std;
//typedef pair<int, int> Pos;
typedef pair<int,int> Pos;
typedef pair<Pos,Pos> NetPos;


class TaskNode
{
#define NAME_LEN  5
#define LOAD_RN   10000

public:
  TaskNode(){}
  TaskNode(Pos p, double l){
    pos = p;
    _distance = l; 
  }
   //TaskNode(const string& n, size_t l) : _name(n), _load(l) {}
  ~TaskNode() {}

  void operator -= (double l) { _distance -= l; }
  bool operator == (const TaskNode& n) const {
    if(n.getPos() == pos)  
      return true; 
    else 
      return false;
  }
  bool operator < (const TaskNode& n) const { 
    return _distance < n._distance; 
  }
  size_t operator () () const;
  friend ostream& operator << (ostream& os, const TaskNode& n);

  void setDistance(double d){ _distance = d; }
  const Pos & getPos() const { return pos; }
  double getDistance() const { return _distance; }

  friend ostream& operator << (ostream& os, const TaskNode& n);

private:
  //pair<int,int> Pos;
  Pos pos;
  double   _distance;
   //string   _name;
  
};

class NetNode
{
#define NAME_LEN  5
#define LOAD_RN   10000

public:
  NetNode(){}
  NetNode(NetPos p, int id){
    netId = id;
    pos = p;
    _load = netDistance();
  }
   //TaskNode(const string& n, size_t l) : _name(n), _load(l) {}
  ~NetNode() {}
  double netDistance(){
    Pos s = pos.first;
    Pos e = pos.second;
    double x = abs( s.first - e.first );
    double y = abs( s.second - e.second );
    if(x == 0){
      return y;
    }
    else if(y==0){
      return x;
    }
    else{ 
      if(x>=y){
        return 2*y + x;
      }
      else{
        return 2*x + y;
      }
    }
  }
  void operator -= (double l) { _load -= l; }
  bool operator == (const NetNode& n) const { 
    if(n.getNetPos() == pos)  
      return true; 
    else 
      return false; 
  }
  bool operator < (const NetNode& n) const { 
    return _load < n._load; 
  }
  size_t operator () () const{ return 0; }
  //friend ostream& operator << (ostream& os, const TaskNode& n);

  const NetPos & getNetPos() const { return pos; }
  int getNetId() const { return netId; }
  double getLoad() const { return _load; }

  //friend ostream& operator << (ostream& os, const TaskNode& n);

private:
  //pair<int,int> Pos;
  NetPos pos;
  double _load;
  int netId;
   //string   _name;
};

class overflowNode
{
#define NAME_LEN  5
#define LOAD_RN   10000

public:
  overflowNode(){}
  overflowNode(int id, int l){
    netId = id;
    _load = l;
  }
   //TaskNode(const string& n, size_t l) : _name(n), _load(l) {}
  ~overflowNode() {}
  void operator -= (double l) { _load -= l; }
  bool operator == (const overflowNode& n) const { 
    if(n.getNetId() == netId)  
      return true; 
    else 
      return false; 
  }
  //*** make it a max heap
  bool operator < (const overflowNode& n) const { 
    return _load > n.getLoad(); 
  }
  size_t operator () () const{ return 0; }
  //friend ostream& operator << (ostream& os, const TaskNode& n);

  //const NetPos & getNetPos() const { return pos; }
  int getNetId() const { return netId; }
  double getLoad() const { return _load; }

  //friend ostream& operator << (ostream& os, const TaskNode& n);

private:
  double _distance;
  double _load;
  int netId;
};


class Routing
{
  public:
    enum traverse{
      empty,
      inside,
      end
    };
    enum direction{
      up,
      right,
      down,
      left
    };


    friend class TaskNode;
    Routing(void){}

    // set the four integer's value
    void readIn(int h, int v, int c, int n );

    //read in the database, all the 2 pin net
    void readNets ( pair<int, int> & s, pair<int, int> & e);

    void globalRouting(int mode);

    void setNetOrder();

    //SingleSourceShortestPath
    void SSSP(int s);
    void relaxation(TaskNode& );
    bool nodeRelaxation(TaskNode& _t, Pos temp, int i);
    bool checkValidPos(Pos& t) const;
    void traversePath(int netId, Pos p);

    //Initialize single sourse,
    void initializeTables(); 
    void updateEdgeDemandTable(Pos s, Pos e);
    void reduceEdgeDemandTable(Pos s, Pos e);

    void updateEdgeWeightTable();
    void updateTraverseTable();
    void initializePiTable();
    //============rip up and rerout============
    void ripUpRerout();

    void ripUp(int netId);

    void setRipUpEdgeDemandTable();

    void updateOverflowTable();

    void getBadNets(int percent=10);

    //===========print funcitons===============
    void printNets();
    void printPath(int netId);
    void printTables(){
      //printTable<double>(edgeDemand,0);
      //printTable<double>(edgeWeight,1);
      //printTable<double>(vertexDistance,2);
      printEdgeTable();
      printDemandTable();
      printPiTable();
      cout << "Total overflow = " << totalOverflow() 
           << ", total wirelength = " << totalWirelength() << endl;
    }
    void printOverflowAndWirelength(){
      cout << "Total overflow = " << totalOverflow() 
          << ", total wirelength = " << totalWirelength() << endl; 
    }

    template<class T> 
    void printTable (T** table, int mode=0);
    template<class T>
    void printEdgeAndDemandTable(int mode);
    void printEdgeTable();
    void printDemandTable();
    void printPiTable();
    void printTraverseTable();
    void printHeap();

    void outputRouting(ostream& outfile) const;
    void outputOverflow(ostream& outfile) const;

    //=============print end====================

    int totalOverflow();

    int totalWirelength();

    //from s to e -> set the value of position e to "s"
    void updatePiTable(Pos s, Pos e){
      piTable[e.first][e.second] = s;
    }

    void setTraverseTable(Pos p, int i=2){
      traverseTable[p.first][p.second] = i;
    }


    //from left to right
    double getHEdgeWeight(Pos p) const{
      return HedgeWeight[p.first][p.second];
      //return 0;
    }
    //from down to up 
    double getVEdgeWeight(Pos p) const{
      return VedgeWeight[p.first][p.second];
      //return 0;
    }

    double edgeWeightFunction(int demand){
        //return pow(2,demand/capacity)-1;
      return pow(base,demand/capacity);
    }
    //==============edge demand=============
    double getHEdgeDemand(Pos p) const{
      return HedgeDemand[p.first][p.second];
      //return 0;
    }
    //from down to up 
    double getVEdgeDemand(Pos p) const{
      return VedgeDemand[p.first][p.second];
      //return 0;
    }
    void setHEdgeDemand(Pos p, int w) {
      HedgeDemand[p.first][p.second] = w;
    }
    //from down to up 
    void setVEdgeDemand(Pos p, int w) {
      VedgeDemand[p.first][p.second] = w;
    }
    int getEdgeDemand(Pos s, Pos e);
    // return # of tiles in horizontal direction
    int gNumHTiles(){ return numHTilesm; }

    // return # of tiles in vertical direction
    int gNumVTiles(){ return numVTilesm; }

    // return the capacity of each tile edge
    int gCapacity(){ return capacity; }

    // return # of nets
    int gNumNets(){ return numNets; }

    // return the starting x and y position for the net
    pair<int,int> gNetStart( int id )const { return netsPos[ id ].first; }

    // return the ending x and y position for the net
    pair<int,int> gNetEnd( int id )const { return netsPos[ id ].second; }

    pair<int,int> gPiTable(Pos p)const{ return piTable[p.first][p.second]; }


  private:
    //# of tiles in horizontal direction
    //# of tiles in vertical direction
    //the capacity of each tile edge
    //return # of nets
    int numHTilesm, numVTilesm, capacity, numNets;
    int _totalOverflow;
    vector<NetPos> netsPos;
    int* netOrder;
    int base;
    //should I calculate edge weight every time? Yes!
    //double** edgeWeight;
    //double** edgeDemand;
    //horizontal edges
    double** HedgeWeight;
    //vertical edges
    double** VedgeWeight;
    //Demand
    int** HedgeDemand;
    int** VedgeDemand;

    vector<Pos>* netsPath;
    //do we need this??
    double** vertexDistance;
    //======= rip up======
    double** overflowTable;
    double** ripUpEdgeDemand;
    std::map <int, vector<Pos> > ripNetsPath;
    vector <int> ripNets;
    MinHeap<overflowNode> _overflowHeap;
    //====end rup up======
    
    pair<int, int>** piTable;
    int** traverseTable;
    MinHeap<TaskNode>* _taskHeap;
    MinHeap<NetNode> _netHeap;
    //traverse trav;
};


#endif

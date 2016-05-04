#include "routing.h"

  void Routing::readIn(int h, int v, int c, int n ){
    numHTilesm = h;
    numVTilesm = v;
    capacity = c;
		numNets = n;
		netsPos.reserve(n);
		netOrder = new int [numNets];
		netsPath = new vector<Pos>[numNets];
    base = 2;
	}
  void Routing::readNets (pair<int, int> & s, pair<int, int> &e)
  {
		NetPos temp(s,e);
		netsPos.push_back(temp);
  }
  //update
  //========================================================
  //================ edge demand table =====================
  //========================================================
  int Routing::getEdgeDemand(Pos s, Pos e){
    if(s.first==e.first){
      //vertical edge
      if(s.second>e.second){
        return VedgeDemand[s.first][s.second];
      }
      else{
        return VedgeDemand[e.first][e.second]; 
      }
    }
    else if(s.second==e.second){
      //horizontal edge
      if(s.first>e.first){
        return HedgeDemand[s.first][s.second];
      }
      else{
        return HedgeDemand[e.first][e.second]; 
      }
    }
    else{
      cout << "something wrong!";
      return 0;
    }
  }

  void Routing::updateEdgeDemandTable(Pos s, Pos e){
    if(s.first==e.first){
      //vertical edge
      if(s.second>e.second){
        ++VedgeDemand[s.first][s.second];
      }
      else{
        ++VedgeDemand[e.first][e.second]; 
      }
    }
    else if(s.second==e.second){
      //horizontal edge
      if(s.first>e.first){
        ++HedgeDemand[s.first][s.second];
      }
      else{
        ++HedgeDemand[e.first][e.second]; 
      }
    }
    else{
      cout << "something wrong!";
    }
  }
  void Routing::reduceEdgeDemandTable(Pos s, Pos e){
    if(s.first==e.first){
      //vertical edge
      if(s.second>e.second){
        --VedgeDemand[s.first][s.second];
      }
      else{
        --VedgeDemand[e.first][e.second]; 
      }
    }
    else if(s.second==e.second){
      //horizontal edge
      if(s.first>e.first){
        --HedgeDemand[s.first][s.second];
      }
      else{
        --HedgeDemand[e.first][e.second]; 
      }
    }
    else{
      cout << "something wrong!";
    }
  }
  //========================================================
  //================ End edge demand table =================
  //========================================================

  void Routing::updateEdgeWeightTable(){
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        HedgeWeight[i][j] = edgeWeightFunction(HedgeDemand[i][j]);
      }
    }
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        VedgeWeight[i][j] = edgeWeightFunction(VedgeDemand[i][j]);
      }
    }

  }
  void Routing::updateTraverseTable(){
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        traverseTable[i][j] = 0;
      }
    } 
  }
  //end update

  //initializeTables
  void Routing::initializePiTable(){
    Pos Nil(-1,-1);
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        piTable[i][j]= Nil;
      }
    }
  }

  void Routing::initializeTables(){
    piTable = new Pos* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      piTable[i] = new Pos [numVTilesm];
    }
    //edge demand
    HedgeDemand = new int* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      HedgeDemand[i] = new int [numVTilesm];
      for(int j=0; j < numVTilesm; ++j)
        HedgeDemand[i][j] = 0;
    }
    VedgeDemand = new int* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      VedgeDemand[i] = new int [numVTilesm];
      for(int j=0; j < numVTilesm; ++j)
        VedgeDemand[i][j] = 0;
    }
    //end
    //edge weight
    HedgeWeight = new double* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      HedgeWeight[i] = new double [numVTilesm];
    }
    VedgeWeight = new double* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      VedgeWeight[i] = new double [numVTilesm];
    }
    //end
    vertexDistance = new double* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      vertexDistance[i] = new double [numVTilesm];
    }
    traverseTable = new int* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      traverseTable[i] = new int [numVTilesm];
    }
    //rip up
    overflowTable = new double* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      overflowTable[i] = new double [numVTilesm];
    }
    ripUpEdgeDemand = new double* [numHTilesm];
    for(int i=0; i < numHTilesm; i++){
      ripUpEdgeDemand[i] = new double [numVTilesm];
    }

  }
  //end Initialization
  //========================================================
  //==================global routing========================
  //========================================================
  void Routing::globalRouting(int mode){
  	switch(mode){
  		case 0:
  			_taskHeap = new MinHeap<TaskNode>();
  			//setNetOrder();
        initializeTables();
        #ifdef r_debug
        printTables();
        #endif
  			for(int idNet = 0; idNet < gNumNets(); ++idNet){
  				SSSP(idNet);

          traversePath(idNet, gNetEnd(idNet) );
        #ifdef r_debug
        cout << "--> Table After SSSP!" << endl;
        printTables();
        #endif
  			}

  		break;
      
      case 1:
        _taskHeap = new MinHeap<TaskNode>();
        setNetOrder();
        initializeTables();
        int idNet;
        for(int i = 0; i < gNumNets(); ++i){
          idNet = netOrder[i];
          SSSP(idNet);
      #ifdef t_debug
       // cout << "NetId = "  << idNet << endl;
       // printTraverseTable();
       // printPiTable();
      #endif
          if( netsPath[idNet].size()!=0 ){
            vector<Pos> empty;
            netsPath[idNet] = empty;
          }
          traversePath(idNet, gNetEnd(idNet) );
          //printTables();
        }
        totalOverflow();
        ripUpRerout();
      break;


  		default:
  		break;
  	}
  }
  void Routing::setNetOrder(){
    for(int i=0; i<numNets; ++i){
      NetNode node( netsPos[i], i);
      _netHeap.push_back(node);
    }
    _netHeap.build_up_heap();
    for(int i=0; i<numNets; ++i){
      netOrder[i] = _netHeap.min().getNetId();
      //cout<< "test " << netOrder[i] << endl;
      _netHeap.delMin();
    }
    #ifdef s_debug
    cout << "Net Order -> " << endl;
    for(int i=0, j=0 ; i<numNets; ++i){
      if(j == 5 )
        cout << endl;
      cout << (int)netOrder[i] << " ";
      ++j;
    }
    cout << endl;
    #endif

  }

  //SingleSourceShortestPath
  void Routing::SSSP(int s){
  	initializePiTable();
    updateEdgeWeightTable();
    updateTraverseTable();
    //cout << "Clean tables!" << endl;
    //printTraverseTable();
  	TaskNode t( gNetStart(s), 0 );
    assert(_taskHeap->size()==0);
  	_taskHeap->insert(t);
    //setTraverseTable( t.getPos(), end );
  	//std::queue<Pos> q;
  	//q.push_back(q);
    #ifdef t_debug
    int c = 0;
    int dummy=0;
    #endif
  	while(_taskHeap->size()!=0){
  		TaskNode t = _taskHeap->min();
      #ifdef t_debug
        if(s == 47){
          if(dummy<=0)
            cin >> dummy;
          --dummy;
          ++c;
          cout << "NetId = "  << s << ", count = " << c << endl;
          Pos pos;
          cout << "min: (" << t << " = " << t.getDistance() << endl;
          pos = gNetStart(s);
          cout << "(" << pos.first << "," << pos.second << ")";
          pos = gNetEnd(s);
          cout << " (" << pos.first << "," << pos.second << ")" << endl;
          printTraverseTable();
          printPiTable();
          printHeap();
        }
      #endif
      _taskHeap->delMin();
      setTraverseTable( t.getPos(), end );
  		//netsPath[s].push_back( t.getPos() );
  		relaxation(t);
  	}
    //printTables();
  }
  //========================================================
  //================== Relaxation ==========================
  //========================================================
  void Routing::relaxation(TaskNode& _t){
    Pos temp;
    Pos t = _t.getPos();
    #ifdef r_debug
    cout << "-> " << t.first << " " << t.second << endl;
    #endif
    //double d = _t.getDistance();
    //up
    temp = t;
    ++temp.second;
    if( checkValidPos(temp) ){
      nodeRelaxation(_t, temp, up);
    }
    //down
    temp = t;
    --temp.second;
    if( checkValidPos(temp) ){
      nodeRelaxation(_t, temp, down);
    }
    //left
    temp = t;
    --temp.first;
    if( checkValidPos(temp) ){
      nodeRelaxation(_t, temp, left);
    }
    //right
    temp = t;
    ++temp.first;
    if( checkValidPos(temp) ){
      nodeRelaxation(_t, temp, right);
    }
  }

  bool Routing::nodeRelaxation(TaskNode& _t, Pos temp, int i){
    Pos t = _t.getPos();
    double d = _t.getDistance();
    TaskNode node;
    if(i==up){
      node = TaskNode( temp, d+getVEdgeWeight(temp) );
    }
    else if(i==down){
      node = TaskNode( temp, d+getVEdgeWeight(t) );
    }
    else if(i==left){
      node = TaskNode( temp, d+getHEdgeWeight(t) );
    }
    else if(i==right){
      node = TaskNode( temp, d+getHEdgeWeight(temp) );
    }

    //TaskNode node( temp, d+getEdgeWeight(temp) );
    #ifdef r_debug
    cout << "Relaxation " << temp.first << " " << temp.second << endl;
    #endif
    //case1:==0, not traversed
    //case2:==1, in heap table
    //case3:==2, already traverse, not in heaptable
    if( traverseTable[temp.first][temp.second]==empty ){
        //TaskNode node( temp, d+getEdgeWeight(temp) );
        #ifdef r_debug
        cout << "Insert " << temp.first << " " << temp.second << endl;
        #endif
        _taskHeap->insert(node);
        updatePiTable(t,temp);
        #ifdef r_debug
        cout << "Path " << t.first << " " << t.second;
        cout << " -> " << temp.first << " " << temp.second << endl;
        #endif
        setTraverseTable( temp , inside );
    }
    else if( traverseTable[temp.first][temp.second]==inside )
    {
      //TaskNode node()
      if( _taskHeap->update(node) ){
        #ifdef t_debug
        cout << "node = " << node << endl;
        cout << "Update -> " << temp.first << " " << temp.second 
             << endl;
        #endif
        updatePiTable(t,temp);
      }
    }
    else{

    }
    return true;
  }
  //========================================================
  //================== Relaxation end ======================
  //========================================================

  bool Routing::checkValidPos(Pos& t)const{
    if(t.first < numHTilesm && t.first >=0 ){
      if(t.second < numVTilesm && t.second >=0 ){
        return true;
      }
    }
    return false;
  }
  void Routing::traversePath(int netId, Pos p){
    netsPath[netId].push_back(p);
    Pos s = gPiTable(p);
    Pos startp(-1,-1);
    if( s != startp ){
      traversePath( netId, s );
      updateEdgeDemandTable(s, p);
    }
    #ifdef u_debug
    if( gNetStart(netId)!= netsPath[netId].back() ){
      cout << "NetId = "  << netId << endl;
      printTraverseTable();
      printPiTable();
    }
    #endif
  }

  //========================================================
  //================global routing End =====================
  //========================================================

//SSSP  End
//========================================================
//=====================rip up and rerout===================
//========================================================
  //step1: calculate overflow of each net ( by heap )
  //step2: rip up 10% and update the edgedemand table
  //step3: run sssp and store the result in a map
  //step4: compare the result and the current result

  void Routing::ripUpRerout(){
    //setRipUpEdgeDemandTable();
    //updateOverflowTable();
    int max = 20;
    int times = 10;
    int nowOverflow;
    base = 5;
    //int Afteroverflow;
    int bestTime = 0;
    int bestOverflow = _totalOverflow;
    int percent;
    //cout <<  ;
    for(int i=0; i<max; ++i){
      nowOverflow = _totalOverflow;
      if(i!=0){
        for(int i=0; i<4; ++i)
          cout << char(8);
      }
      //cout << "times = " << setw(4) << i << ", totalOverflow = " << nowOverflow << endl;

      //cout << ", Overflow = " << nowOverflow << endl;
      if( nowOverflow == 0 ){
        break;
      }
      if(i<=5){
        percent = 10;
      }
      else if(i <= 10)
        percent = 20;
      else if(i < 15)
        percent = 20+i;
      else 
        percent = 20+i;
      getBadNets(percent);
      //printDemandTable();
      for(int j=0, _size = ripNets.size(); j < _size; ++j){
        ripUp( ripNets[j] );
      //  printDemandTable();
      //  int stop;
      //  cin >> stop;
      }

      for(int j=0, _size = ripNets.size(); j < _size; ++j){
        int idNet = ripNets[j];
        SSSP( ripNets[j] );
        if( netsPath[idNet].size()!=0 ){
          vector<Pos> empty;
          netsPath[idNet] = empty;
        }
        traversePath(idNet, gNetEnd(idNet) );
        //printTables();
      }

      totalOverflow();
      if( bestOverflow > _totalOverflow ){
        bestOverflow = _totalOverflow;
        bestTime = i;
      }
      if( _totalOverflow <= bestOverflow && i > times ){
        break;
      }
    }
   // cout << "Best overflow = " << bestOverflow 
   //     << ", times = " << times << endl;

  }

  void Routing::ripUp(int netId ){
    for(int j=0, _length = netsPath[ netId ].size()-1; j < _length; ++j){
      reduceEdgeDemandTable( netsPath[ netId ][ j ], netsPath[ netId ][ j+1 ] );
    //  cout <<  netId << ":" << netsPath[ netId ][ j ].first << "," << netsPath[ netId ][ j ].second << ")(";
      //cout << "edge demand = " << getEdgeDemand( netsPath[ netId ][ j ], netsPath[ netId ][ j+1 ] )
      //    << endl;
    }
   // cout << endl;
  }

  void Routing::setRipUpEdgeDemandTable(){
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        //ripUpEdgeDemand[i][j] = edgedemand[i][j];  
      }
    }
  }

  void Routing::getBadNets(int percent){
    NetPos _net;
    Pos s , e;
    int numRip = numNets/percent;
    int _demand=0;
    int flow=0;
    //int i=0;
    //int d=0;
    TaskNode node;
    Pos temp, t;
    if( _overflowHeap.empty()!=0 ){
      MinHeap<overflowNode> empty;
      _overflowHeap = empty ;
    }

    for(int id=0; id<numNets; ++id){
      int _size = netsPath[id].size()-1;
      //cout << "size = " << _size << endl;
      for(int j=0; j < _size; ++j){
        s = netsPath[id][j];
        e = netsPath[id][j+1];
        //cout << "(" << s.first << "," << s.second << ") " 
        //      << "(" << e.first << "," << e.second << ")" << endl;
        if(s.first==e.first){   //vertical movement
          if(s.second < e.second ){  //up
            _demand = getVEdgeDemand(e);
          } 
          else{  //down
            _demand = getVEdgeDemand(s);
          }
          if( _demand > capacity ){
            flow = flow + 1;
          }
        }
        else{   //horizontal movement
          assert( s.second == e.second );
          if( s.first < e.first ){  //right
            _demand = getHEdgeDemand(e);
          }
          else{    //left
            _demand = getHEdgeDemand(s);
          }
          if( _demand > capacity ){
            flow = flow + 1;
          }
        }
      }
      overflowNode node(id, flow);
      _overflowHeap.insert(node);
      #ifdef v_debug
      //cout << "Id = " << id << ", overflow = " << flow << endl;
      //cout << "Max = " << 
      #endif
      flow = 0;
    }
    vector <int> empty;
    ripNets.swap( empty );
    for(int i=0; i < numRip; ++i){
      if( _overflowHeap.min().getLoad()==0 )
        break;
      ripNets.push_back( _overflowHeap.min().getNetId() );
      #ifdef heap_debug
      overflowNode node = _overflowHeap.min();
      cout << "Max[" << node.getNetId() << "] = " << node.getLoad() << endl;
      #endif
      _overflowHeap.delMin();
    }
  }

  void Routing::updateOverflowTable(){
    int over = 0;
    int flow = 0;
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        if(HedgeDemand[i][j] > capacity){
          flow = HedgeDemand[i][j] - capacity;
          ripUpEdgeDemand[i][j] = flow;
          over = over + flow;
        }
        else{
          ripUpEdgeDemand[i][j] = 0;
        }
      }
    }
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        if(VedgeDemand[i][j] > capacity){
          flow = HedgeDemand[i][j] - capacity;
          ripUpEdgeDemand[i][j] = flow;
          over = over + flow;
        }
        else{
          ripUpEdgeDemand[i][j] = 0;
        }
      }
    }
    _totalOverflow = over;
  }



//==========================================================

//get wirelength and overflow

  int Routing::totalWirelength(){
    int l=0;
    for(int i=0; i < numNets; ++i){
      l = l + netsPath[i].size()-1;
    }
    return l;
  }

  int Routing::totalOverflow(){
    int over = 0;
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        if(HedgeDemand[i][j] > capacity){
          over = over + (HedgeDemand[i][j] - capacity);
        }
      }
    }
    for(int i=0; i < numHTilesm; ++i){
      for(int j=0; j < numVTilesm; ++j){
        if(VedgeDemand[i][j] > capacity){
          over = over + (VedgeDemand[i][j] - capacity);
        }
      }
    }
    _totalOverflow = over;
    return over;
  }

//end get wirelength and overflow

//overloading <<

  ostream& operator << (ostream& os, const TaskNode& n)
  {
     return os << "(" << n.getPos().first << ", " << n.getPos().second 
               << ", " << n._distance <<  ")";
  }

//end overloading

//========================================================
//==================== Print =============================
//========================================================

	void Routing::printNets(){
    cout << "=====Nets=====" << endl;
		for (int idNet = 0; idNet < gNumNets(); ++idNet){
      pair<int, int> posS = gNetStart( idNet );
      pair<int, int> posE = gNetEnd( idNet );
      cout << idNet << " " << posS.first << " " << posS.second << " "
           << posE.first << " " << posE.second << endl;
    }
    cout << "==============" << endl;
	}
  template<class T> 
  void Routing::printTable (T** table, int mode){
    if(mode==0)
      cout<<"=========Edge Demand Table=======" << endl;
    else if(mode==1)
      cout<<"=========Edge Weight Table=======" << endl;
    else if(mode==2)
      cout<<"=====Vertex Distance Table=======" << endl;
    for(int i=numVTilesm-1; i >= 0; --i){
      cout << "[" << i << "]  ";
      for(int j=0; j < numHTilesm; ++j){
        cout << std::setw(5) << std::left << table[j][i];
      }
      cout << endl;
    }
    cout << "    ";
    for(int i=0; i < numHTilesm; ++i){
      cout << "[" << i << "]  ";
    }
    cout << endl;
    cout << "=================================" << endl;
  }
  template<class T>
  void Routing::printEdgeAndDemandTable(int mode){
    if(mode==0){
      cout<<"=========Edge Weight Table=======" << endl;
    }
    else if(mode==1)
      cout<<"=========Edge Demand Table=======" << endl; 
  }
  void Routing::printEdgeTable(){
    cout<<"=========Edge Weight Table=======" << endl;
    for(int i=numVTilesm-1; i >= 0; --i){
      cout << "[" << i << "]  ";
      for(int j=0; j < numHTilesm; ++j){
        cout << " . " << std::setw(3) << std::left << HedgeWeight[j][i];
      }
      cout << endl;
      cout << "   " ;
      for(int j=0; j < numHTilesm; ++j){
        cout << "   " << std::setw(3) << std::left << VedgeWeight[j][i];
      }
      cout << endl;
    }
    cout << "    ";
    for(int i=0; i < numHTilesm; ++i){
      cout << "[" << i << "]   ";
    }
    cout << endl;
    cout << "=================================" << endl;
  }
  void Routing::printDemandTable(){
    cout<<"=========Edge Demand Table=======" << endl;
    for(int i=numVTilesm-1; i >= 0; --i){
      cout << "[" << i << "]  ";
      for(int j=0; j < numHTilesm; ++j){
        cout << std::setw(3) << std::left << HedgeDemand[j][i] << " . " ;
      }
      cout << endl;
      cout << "     " ;
      for(int j=0; j < numHTilesm; ++j){
        cout << "   " << std::setw(3) << std::left << VedgeDemand[j][i];
      }
      cout << endl;
    }
    cout << "       ";
    for(int i=0; i < numHTilesm; ++i){
      cout << "[" << i << "]   ";
    }
    cout << endl;
    cout << "=================================" << endl;    
  }


  void Routing::printPiTable(){
    cout<<"=============Pi Table============" << endl;
    for(int i=numVTilesm-1; i >= 0; --i){
      cout << "[" << setw(2) <<  i << "]  ";
      for(int j=0; j < numHTilesm; ++j){
        cout << "(" << setw(2) << piTable[j][i].first << ","
             << setw(2) << piTable[j][i].second << ") ";
      }
      cout << endl;
    }
    cout << "        ";
    for(int i=0; i < numHTilesm; ++i){
      cout << "[" << i << "]    ";
    }
    cout << endl;
    cout << "=================================" << endl;      
  }

  void Routing::printTraverseTable(){
    cout<<"==========Traverse Table============" << endl;
    for(int i=numVTilesm-1; i >= 0; --i){
      cout << "[" << setw(2) <<  i << "]  ";
      for(int j=0; j < numHTilesm; ++j){
        cout << "( " << traverseTable[j][i] << " ) ";
      }
      cout << endl;
    }
    cout << "      ";
    for(int i=0; i < numHTilesm; ++i){
      cout << "[" << i << "]  ";
    }
    cout << endl;
    cout << "=================================" << endl;   
  }

  void Routing::printHeap(){
    cout << "heap table -> " << endl;
    for(int i=0, _size = _taskHeap->size(); i < _size; ++i){
      cout << (*_taskHeap)[i];
    }
    cout << endl;
  }

  //========================================================
  //==================== Print End =========================
  //========================================================

//print end

//output file
  void Routing::outputRouting(ostream& outfile) const{
    //netId # of route
    //route list
    for(int i=0; i < numNets; ++i){
      int _size = netsPath[i].size();
      outfile << i << " " << _size-1 << endl;
      for(int j=_size-1; j > 0; --j){
        outfile << netsPath[i][j].first << " " << netsPath[i][j].second << " ";
        outfile << netsPath[i][j-1].first << " " << netsPath[i][j-1].second << " " << endl; 
      }
    }
  }
  void Routing::outputOverflow(ostream& outfile) const{
    Pos s, e;
    int flow=0;
    int _demand;
    for(int id=0; id<numNets; ++id){
      int _size = netsPath[id].size()-1;
      //cout << "size = " << _size << endl;
      for(int j=0; j < _size; ++j){
        s = netsPath[id][j];
        e = netsPath[id][j+1];
        //cout << "size= " << _size << endl;
        //cout << "(" << s.first << "," << s.second << ") " 
        //      << "(" << e.first << "," << e.second << ")" << endl;
        if(s.first==e.first){   //vertical movement
          if(s.second < e.second ){  //up
            _demand = getVEdgeDemand(e);
          } 
          else{  //down
            _demand = getVEdgeDemand(s);
          }
          if( _demand > capacity ){
            flow = flow + 1;
          }
        }
        else{   //horizontal movement
          assert( s.second == e.second );
          if( s.first < e.first ){  //right
            _demand = getHEdgeDemand(e);
          }
          else{    //left
            _demand = getHEdgeDemand(s);
          }
          if( _demand > capacity ){
            flow = flow + 1;
          }
        }
      }
      //overflowNode node(id, flow);
      //_overflowHeap.insert(node);
      #ifdef v_debug
      //cout << "Id = " << id << ", overflow = " << flow << endl;
      //cout << "Max = " << 
      #endif
      outfile << "[" << id << "]" << flow << endl;
      flow = 0;
    }
  }

//end output

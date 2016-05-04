/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
  MinHeap( size_t s = 0) { 
    if (s != 0) _data.reserve(s);
  }
  ~MinHeap() {}

  void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
  const Data& min() const {
    if(!empty()){
      return _data[0]; 
    }
    Data* d = new Data;
    //Data d();
    return *d;
  }
   void insert(const Data& d) { 
      int i=_data.size();
      _data.push_back(d);
      while(_data[i]<_data[(i-1)/2]){
        //TaskNode _t = _data[i];
        //_data[i] = _data[(i-1)/2];
        //_data[(i-1)/2] = d;
        swap( (i-1)/2, i );
        i = (i-1)/2;
      }
   }

  void push_back(const Data& d){
    _data.push_back(d);
  }

  void build_up_heap(){
    int _size = _data.size();
    for( int i=floor( _size/2 ) ; i>=0 ; i-- )
    {
        min_heapify( i );
    }
  }

  void delMin() {
    if(!empty()){
      swap(0,_data.size()-1);
      _data.pop_back();
      min_heapify(0);
    }
  }
  void delData(size_t i) { 
    if(i<_data.size()){
      swap(i,_data.size()-1);
      //cout<< "r= " << _data.back() << endl;
      _data.pop_back();
      min_heapify(i);
    }
  }


  //TODO:customize
  bool update(Data& d){
    bool heapify = false;
    int i;
    for( i=0; i<_data.size(); i++){
      if(_data[i]==d){
        if(d<_data[i]){
          _data[i].setDistance( d.getDistance() );
          heapify = true;
        }
      }
    }
    if(heapify){
      up_heapify(i);
      return true;
    }else
      return false;
  }
  Data search(Data& d){
    for(int i=0; i<_data.size(); i++){
      if(_data[i]==d)
        return _data[i];
    }
  }

  Data increase_min(size_t l){
    _data[0]+=l;
    Data temp = _data[0];
    //cout << "Task assignment succeeds..." << endl;
    //cout << "Updating min: " << _data[0] <<endl;
    min_heapify(0);
    return temp;
   // return true;
  }

  //myDef
  bool empty() const { return _data.size()==0; }
  

  
  void up_heapify(int i){
    //int heap_size = _data.size();
    int parent;
    //int sibling;
    /*
    if(i%2)
      sibling = i+1;
    else
      sibling = i-1;
    */
    parent = (i-1)/2;
    int min = parent;
    //cout<<"A["<<i<<"]= "<< A[i]<<"  l="<<l<<"  r="<<r<<endl;
    if( parent >= 0 )
    {
      if( _data[i]<_data[parent] )
        min = i;
    }
    /*
    if( sibling<heap_size && sibling>=0 )
    {
      if ( _data[sibling]<_data[min] )
        min = sibling;
    }
    */
    if( min != parent )
    {
      swap(i, min);
      up_heapify(parent);
    }
  }

  void min_heapify(  int i )
  {
    int heap_size = _data.size();
    int l;
    int r;
    int min=i;
    l=2*i+1;
    r=2*i+2;
    //cout<<"A["<<i<<"]= "<< A[i]<<"  l="<<l<<"  r="<<r<<endl;
    if( l<heap_size )
    {
      if( _data[l]<_data[i] )
        min = l;
    }
    if( r<heap_size )
    {
      if ( _data[r]<_data[min] )
        min = r;
    }
    if( min != i )
    {
      swap(i, min);
      min_heapify(min);
    }

}

void printHeap(){
  std::cout << "All heap: " << std::endl;
  for (size_t i = 0, n = _data.size(); i < n; ++i)
     std::cout << _data[i] << std::endl;
}


private:
  void swap(int i, int j){
    Data _t = _data[i];
    _data[i] = _data[j];
    _data[j] = _t;
    //std::cout << _data[i] << " " << _data[j] << std::endl;
    //printHeap();
  }
   // DO NOT add or change data members
   std::vector<Data>   _data;
   //double** edgeDemand;
};

#endif // MY_MIN_HEAP_H

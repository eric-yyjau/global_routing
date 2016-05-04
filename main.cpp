/**************************************************************************
 * File       [ main.cpp ]
 * Author     [ wlkb83 ]
 * Synopsis   [ demonstration for the usage of parser.h ]
 * Usage      [ ./parser [inputfileName] ]
 * Date       [ 2014/12/28 created ]
**************************************************************************/

#include "parser.h"
#include "routing.h"

int main(int argc, char **argv)
{
    if( argc < 2 ){ cout << "Usage: ./parser [input_file_name]" << endl; return 1; }
    AlgTimer t;
    AlgParser parser;

    // read the file in the first argument
    if( ! parser.read( argv[1] ) ) { return 1; }

    // show the information of the input
/*    
    cout << "grid " << parser.gNumHTiles() << " " << parser.gNumVTiles() << endl;
    cout << "capacity " << parser.gCapacity() << endl;
    cout << "num net " << parser.gNumNets() << endl;
	cout << "----------------------" << endl;
	cout << "-- Complete parsing --" << endl;
	cout << "----------------------" << endl;
*/

    //start to do !
  Routing router;
  router.readIn( parser.gNumHTiles(), parser.gNumVTiles(), \
    parser.gCapacity(), parser.gNumNets() );
  for (int idNet = 0; idNet < parser.gNumNets(); ++idNet){
    pair<int, int> posS = parser.gNetStart( idNet );
    pair<int, int> posE = parser.gNetEnd( idNet );
    router.readNets(posS, posE);
  }
  //router.printNets();

  router.globalRouting(1);

  if(argc == 3){
    ofstream outfile(argv[2], ios::out);
    router.outputRouting(outfile);
  }
  else{
    router.outputRouting(cout);
  }
  //router.outputOverflow(cout);
  router.printOverflowAndWirelength();


  cout << "The execution spends " << t.End() << " seconds" << endl;

  return 0;


}

void intput(AlgParser& parser ){
      cout << "grid " << parser.gNumHTiles() << " " << parser.gNumVTiles() << endl;
      cout << "capacity " << parser.gCapacity() << endl;
      cout << "num net " << parser.gNumNets() << endl;
      for (int idNet = 0; idNet < parser.gNumNets(); ++idNet){
          pair<int, int> posS = parser.gNetStart( idNet );
          pair<int, int> posE = parser.gNetEnd( idNet );
          cout << idNet << " " << posS.first << " " << posS.second << " "
                               << posE.first << " " << posE.second << endl;
      }
      
    cout << "----------------------" << endl;
    cout << "-- Complete parsing --" << endl;
    cout << "----------------------" << endl;
}


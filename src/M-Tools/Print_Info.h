#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Print_Inform{

      public:
           void print_Errors(int sam);
           void print_Warrings(int sam);
           void print_Happy(int sam);

};

void Print_Inform::print_Errors(int sam){

     if (sam==1) cout << "Errors :: the number of coordination not equal to the number of parameter"<<endl;
     if (sam==2) cout << "Error: Cannot open file : ";
     if (sam==3) cout << " Error : Arguments must start with -\n"<<endl;
     if (sam==4) cout << "Error: Different size of two vectors, check your input files! "<<endl;
     if (sam==5) cout << "ERROR: (Read_DCD) DCD file not open";
     if (sam==6) cout <<"Read_DCD> End of DCD file reached." << endl;
     if (sam==7) cout <<"ERROR: Improper format for DCD file" << endl;
     if (sam==8) cout <<"ERROR: Early end to DCD file" << endl;
     }

void Print_Inform::print_Warrings(int sam){


     }

void Print_Inform::print_Happy(int sam){
     if (sam==1) cout << ".. When the Spring Festival, everyone was eating the dumplings. " <<endl;
     if (sam==2) cout << ".. Wow, It's a Christmas present! "<<endl;
     if (sam==3) cout << ".. Standing on the shoulder of giants. "<<endl;
     if (sam==4) cout << ".. Come on! .>V5<. Success is waving to you! "<<endl;
     if (sam==5) cout << ".. Do you know Chinese Kung Fu, Avatar? Maybe you could show me your tail, gu..gu. "<<endl;
     if (sam==6) cout << ".. If you meet Optimus Prime, tell him, a boy is waiting for him in China."<<endl;
     if (sam==7) cout << ".. All things are difficult before they are easy, we learn so little and forget so much."<<endl;
     }

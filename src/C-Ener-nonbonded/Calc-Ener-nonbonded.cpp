/*
 *
 *   Fitting-Coarse-Grained-Parameters version-1.0.1
 *    
 *      This sub-code main function is to calculate the nonbonded energy that used to fit the nonbonded parameters.
 *      Two out files, energy_vdw.dat and energy_ele.dat
 *
 *                                     Created by Allen Wang on 2015/12
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <math.h>
#include <omp.h>
#include "../M-Tools/version.h"
#include "../M-Tools/Print_Info.h"
#include "Energy-nonbond.h"

using namespace std;

string coorFile, parmFile;
int Tn=0, inter_num=0, ITy=1;
int biframe, enframe, totalAnumber;
float refeDIE=1.0;
vector<float> atomChg, part_atomChg;
vector<string> atomType, atomSeg, atomEps, part_atomSeg, part_atomEps;

void PrintHelp(){
     cout << "Auto-Fitting-CG-Parameters version " << Version <<endl;
     cout << "Usage : afcgp.nonbonded-part [Options] Value" << endl;
     cout << "\t-x (Input)  *XYZ, selected trajectory files [Required]"<<endl;
     cout << "\t-p (Input)  *NPM, nonbonded parameter file [Required]"<<endl;
     cout << "\t-t (Input)  Number of threads used by the parallel, default is auto"<<endl;
     cout << "\t-b (Input)  first frame number" <<endl;
     cout << "\t-e (Input)  last  frame number" <<endl;
     return ;
}

void param(int argc, char *argv[]){
     Print_Inform Errors_Temp;
     int i = 1;
     if (argc == 1) PrintHelp();
     while (i<argc){
           if (argv[i][0] != '-'){
              cout << "Argument ::" << i;
              Errors_Temp.print_Errors(3);
              return ;
              }
           switch (argv[i][1]){
                  case 'x': coorFile=argv[i+1];break;
                  case 'p': parmFile=argv[i+1];break;
                  case 't': Tn=atoi(argv[i+1]);break;
                  case 'b': biframe=atoi(argv[i+1]);break;
                  case 'e': enframe=atoi(argv[i+1]);break;
                  case 'i': ITy=atoi(argv[i+1]);break;
                  case 'd': refeDIE=atof(argv[i+1]);break;
                  }
                  i+=2;
             }
}

void Load_nonBparm(const char * infilename){
     Print_Inform Errors_Temp;
     ifstream infile(infilename, ifstream::in);
     if (!infile){
        Errors_Temp.print_Errors(2);
        cout << infilename << endl;
        return ;
     }

     map<string, string> searchSeg, searchEps, searchChg, searchBuf;

     string buffer;
     while (getline(infile, buffer)){
           
           stringstream strin(buffer);
           vector<string> temp_buffer;
           string one_buffer;
           
           while (strin >> one_buffer)
                 temp_buffer.push_back(one_buffer);
           
           searchSeg[temp_buffer[1]] = temp_buffer[2];
           searchEps[temp_buffer[1]] = temp_buffer[3];
           searchChg[temp_buffer[1]] = temp_buffer[4];
           searchBuf[temp_buffer[1]] = temp_buffer[5];
     }
    
     for (int i=0; i < totalAnumber; i++){
         atomSeg.push_back(searchSeg[atomType[i]]);
         atomEps.push_back(searchEps[atomType[i]]);
         atomChg.push_back(atof(searchChg[atomType[i]].c_str()));
         if ( atof(searchBuf[atomType[i]].c_str()) != 0.0 ) inter_num++;
         } 
  
     for (int i=0; i<inter_num; i++){
         part_atomSeg.push_back(searchSeg[atomType[i]]);
         part_atomEps.push_back(searchEps[atomType[i]]);
         part_atomChg.push_back(atof(searchChg[atomType[i]].c_str()));
         }
          
}


int main(int argc, char *argv[]){

    param(argc, argv);

    Energy_Nonbond interandextra, onlyinter;

    Load_nonBparm(parmFile.c_str());

    ofstream outfile_vdw;
    ofstream outfile_ele;
    
    outfile_vdw.open("energyVdw.DAT");
    outfile_ele.open("energyEle.DAT");

    outfile_vdw << inter_num <<endl;
    outfile_ele << inter_num <<endl;

    Print_Inform Errors_Temp;
    ifstream infile(coorFile.c_str(), ifstream::in);
    if (!infile){
        Errors_Temp.print_Errors(2);
        cout << coorFile.c_str() << endl;
        return 0;
     }

     vector<float> coor_x, coor_y, coor_z;
     int current_number =0;
     int frame_pumb = 0;     

     string buffer;
     while (getline(infile, buffer)){
           
           current_number++;
           stringstream strin(buffer);
           vector<string> temp_buffer;
           string one_buffer;
           
           if (current_number > biframe*totalAnumber && current_number <= enframe*totalAnumber){
              frame_pumb++;

              while (strin >> one_buffer)
                   temp_buffer.push_back(one_buffer);
              
              coor_x.push_back(atof(temp_buffer[1].c_str()));
              coor_y.push_back(atof(temp_buffer[2].c_str()));
              coor_z.push_back(atof(temp_buffer[3].c_str()));

              if (frame_pumb = totalAnumber){

                 frame_pumb = 0;
                 vector<float> coor_total, coor_inter;
                 int start_num = coor_x.size() - totalAnumber;

                 for( int i=start_num; i<coor_x.size(); i++ ){
                    coor_total.push_back(coor_x[i]);
                    coor_total.push_back(coor_y[i]);
                    coor_total.push_back(coor_z[i]);
                    if (i-start_num < inter_num){
                       coor_inter.push_back(coor_x[i]);
                       coor_inter.push_back(coor_y[i]);
                       coor_inter.push_back(coor_z[i]);
                       }// if -> i-start_num
                    } // for -> start_num
                  
                 interandextra.Load_Parameters(coor_total, atomChg, atomSeg, atomEps);
                 onlyinter.Load_Parameters(coor_inter, part_atomChg, part_atomSeg, part_atomEps);

                 for (int i=0; i<inter_num; i++){
                     outfile_vdw << i <<" "<< interandextra.Calc_vdw(i, ITy, Tn) <<" "<< onlyinter.Calc_vdw(i, ITy, Tn)<<endl;
                     outfile_ele << i <<" "<< interandextra.Calc_ele(i, refeDIE, Tn)<<" "<< onlyinter.Calc_ele(i, refeDIE, Tn)<<endl;
                     }

                 } //frame_pumb
              
            } // if -> current_number

     } // while -> getline
}

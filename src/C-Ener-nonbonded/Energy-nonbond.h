/*
 *
 *   Fitting-Coarse-Grained-Parameters version-1.0.1
 *                 Created by Allen Wang on 2015/12
 *
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <set>
#include <math.h>
#include <omp.h>
#include "../M-Tools/version.h"

using namespace std;


#define const_ele 138.935485


class Energy_Nonbond{

      public:
           void  Load_Parameters(vector<float> coor, vector<float> chgM, vector<string> paramS, vector<string> paramE);
           float Calc_vdw(int sam, int itype, int coren);
           float Calc_ele(int sam, float die, int coren);

      private:
           vector <float> coorx, coory, coorz, chg;
           vector<string> paraS, paraE; 
           int atomNumber;
       
           float Calc_lj1(int sam, int san, int itype);
           float Calc_e1(int sam, int san, float die);

}; // end of class

float Calc_distance(float x1, float y1, float z1, float x2, float y2, float z2){
      
      float dx = x1 - x2;
      float dy = y1 - y2;
      float dz = z1 - z2;

      float distance = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2));

      return distance;
      
      }

void Energy_Nonbond::Load_Parameters(vector<float> coor, vector<float> chgM, vector<string> paramS, vector<string> paramE){

/* only one frame coordination is permitted */

     Print_Inform echoinform;

     int atomNumber = coor.size()/3;

     if (atomNumber != paramS.size() || atomNumber != paramE.size()){
        echoinform.print_Errors(1);
        return ;
        }     
        coorx.clear();
        coory.clear();
        coorz.clear();
        paraS.clear();
        paraE.clear();
        chg.clear();

     for (int i=0; i<atomNumber; i++){
         int serx=3*i + 1;
         int sery=3*i + 2;
         int serz=3*i + 3;
         coorx.push_back(coor[serx]);
         coory.push_back(coor[sery]);
         coorz.push_back(coor[serz]);
         paraS.push_back(paramS[i]);
         paraE.push_back(paramE[i]);
         chg.push_back(chgM[i]);
         }
     }

float Energy_Nonbond::Calc_vdw(int sam, int itype, int coren){

/*
    itype --> different Lennard-Jones interaction, examples '12-6'

    coren --> number of threads used by the parallel
*/
      float tenergy;

      omp_set_num_threads(coren);

#pragma omp parallel for schedule(dynamic, 1)
      for (int i=0; i<atomNumber; i++){

          if (sam != i) tenergy += Calc_lj1(sam, i, itype);
          }       
      
      return tenergy;

}


float Energy_Nonbond::Calc_ele(int sam, float die, int coren){

/*
    die   --> dielectric const

    coren --> number of threads used by the parallel
*/
      float tenergy;

      omp_set_num_threads(coren);

#pragma omp parallel for schedule(dynamic, 1)
      for (int i=0; i<atomNumber; i++){

          if (sam != i) tenergy += Calc_e1(sam, i, die);

          }

      return tenergy;

}



float Energy_Nonbond::Calc_lj1(int sam, int san, int itype){

/* 
 
   Energy = 4 * Ep * ( (sigma/r)**12 - (sigma/r)**6 )  --> itype = 1
   Energy = Ep * ( (sigma/r)**12 - 2*(sigma/r)**6 )    --> itype = 2

*/

      float distance = Calc_distance(coorx[sam], coory[sam], coorz[sam], coorx[san], coory[san], coorz[san]);

      float sigma = (atof(paraS[sam].c_str()) + atof(paraS[san].c_str()))/2;
      float epsilon = sqrt(atof(paraE[sam].c_str())*atof(paraE[san].c_str()));

      float ratio1 = sigma/distance;
      
      if (itype == 1) return 4*epsilon*(pow(ratio1,12) - pow(ratio1,6));
      if (itype == 2) return  epsilon * (pow(ratio1,12) - 2 * pow(ratio1,6));
      
      }


float Energy_Nonbond::Calc_e1(int sam, int san, float die){

/*   Energy = const_ele*chg1*chg2/die/distance  */

      float distance = Calc_distance(coorx[sam], coory[sam], coorz[sam], coorx[san], coory[san], coorz[san]);
      
      return const_ele*chg[sam]*chg[san]/(die*distance);

      }



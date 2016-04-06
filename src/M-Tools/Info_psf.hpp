
/*
  Name: Info_psfFile
  Copyright: 
  Author: Allen Wang
  Date: 25/03/16 09:15
  Description: Read psf format file used by NAMD, then get informations from PSF file
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

using namespace std;

class Info_psfFile{
      
      public:
             void Readpsf(const char * psffilename);
             vector <float> get_Chg(int serial_mol); // '-1' for all
             vector <float> get_Mass(int serial_mol);
             vector <int> get_Bond(int serial_mol);
             vector <int> get_Angle(int serial_mol);
             vector <int> get_Torsion(int serial_mol);
             
      private:
              vector< vector <string> > Prop_Atom;
              vector< vector <string> > Prop_Bond;
              vector< vector <string> > Prop_Angle;
              vector< vector <string> > Prop_Torsion;
              vector<int> For_Bond, For_Angle, For_Torsion;
              
      }; // end of this class

int SelectKeyWord(string instr){
    stringstream strin(instr);
    string fake_word;
    while(strin >> fake_word){
                if (fake_word=="!NATOM") return 1;
                if (fake_word=="!NBOND") return 2;
                if (fake_word=="!NTHETA") return 3;
                if (fake_word=="!NPHI") return 4;
                if (fake_word=="!NIMPHI") return 5;
                }
    }


vector <float> Info_psfFile::get_Chg(int serial_mol){
       vector<float> Atom_Charge;
       
       for(int i=0; i<Prop_Atom.size();i++){
                  if (serial_mol == -1){
                                 Atom_Charge.push_back(atof(Prop_Atom[i][7].c_str()));
                                 }
                  if (serial_mol != -1){
                                 if(atoi(Prop_Atom[i][3].c_str())==serial_mol){
                                                                               Atom_Charge.push_back(atof(Prop_Atom[i][7].c_str()));
                                                                               }
                                 }
                  }
       return Atom_Charge;                      
       }

vector <float> Info_psfFile::get_Mass(int serial_mol){
       vector<float> Atom_Mass;
       
       for(int i=0; i<Prop_Atom.size();i++){
                  if(serial_mol == -1){
                                Atom_Mass.push_back(atof(Prop_Atom[i][8].c_str()));
                                }
                  if(serial_mol != -1){
                                if(atoi(Prop_Atom[i][3].c_str())==serial_mol){
                                                                              Atom_Mass.push_back(atof(Prop_Atom[i][8].c_str()));
                                                                              }
                                }
                  }
       return Atom_Mass;
       }

vector <int> Info_psfFile::get_Bond(int serial_mol){
       vector <int> Bond_conect;
       
       for(int i=0; i<Prop_Bond.size(); i++){
               for(int j=0; j<For_Bond[i]; j++){
                       int Ser_Con=atoi(Prop_Bond[i][j].c_str());
                       if(serial_mol == -1){
                                     Bond_conect.push_back(Ser_Con);
                                     }
                       if(serial_mol != -1){
                                     if(atoi(Prop_Atom[Ser_Con][3].c_str())==serial_mol){
                                                                                         Bond_conect.push_back(Ser_Con);
                                                                                         }
                                     }
                       }
               }
       return Bond_conect;
       
       }

vector <int> Info_psfFile::get_Angle(int serial_mol){
       vector <int> Angle_conect;
       
       for(int i=0; i<Prop_Angle.size(); i++){
               for(int j=0; j<For_Angle[i]; j++){
                       int Ser_Con=atoi(Prop_Angle[i][j].c_str());
                       if(serial_mol == -1){
                                     Angle_conect.push_back(Ser_Con);
                                     }
                       if(serial_mol != -1){
                                     if(atoi(Prop_Atom[Ser_Con][3].c_str())==serial_mol){
                                                                                         Angle_conect.push_back(Ser_Con);
                                                                                         }
                                     }
                       }
               }
       return Angle_conect;
       }

vector <int> Info_psfFile::get_Torsion(int serial_mol){
       vector <int> Torsion_conect;
       
       for(int i=0; i<Prop_Torsion.size(); i++){
               for(int j=0; j<For_Torsion[i]; j++){
                       int Ser_Con=atoi(Prop_Torsion[i][j].c_str());
                       if(serial_mol == -1){
                                     Torsion_conect.push_back(Ser_Con);
                                     }
                       if(serial_mol != -1){
                                     if(atoi(Prop_Atom[Ser_Con][3].c_str())==serial_mol){
                                                                                         Torsion_conect.push_back(Ser_Con);
                                                                                         }
                                     }
                       }
               }
       return Torsion_conect;
       }

void Info_psfFile::Readpsf(const char * psffilename){
     
     int Line_Info_Atom, Line_Info_Bond, Line_Info_Angle;
     int Line_Info_Torsion, Line_Info_End;
     
     ifstream infile(psffilename, ifstream::in);
     if (!infile){
                  cout << "Error: Cannot open file : " << psffilename << endl;
                  return ;
                  }
     string buffer;
     int ReadType=0;
     int Serial_Line = 0;
     while(getline(infile, buffer)){
                           stringstream strin(buffer);
                           string information;
                           Serial_Line++;
                           while(strin >> information){
                                       ReadType=SelectKeyWord(information);
                                       if (ReadType==1)  Line_Info_Atom    = Serial_Line;
                                       if (ReadType==2)  Line_Info_Bond    = Serial_Line;
                                       if (ReadType==3)  Line_Info_Angle   = Serial_Line;
                                       if (ReadType==4)  Line_Info_Torsion = Serial_Line;
                                       if (ReadType==5)  Line_Info_End     = Serial_Line;
                                       }
                           string Info_Line;
                           vector<string> Info_Atom, Info_Bond, Info_Angle, Info_Torsion;
                           
                           if (Serial_Line>Line_Info_Atom && Serial_Line<Line_Info_Bond){
                                                          while(strin >> Info_Line){
                                                                      Info_Atom.push_back(Info_Line);
                                                                      }
                                                          }
                           if (Serial_Line>Line_Info_Bond && Serial_Line<Line_Info_Angle){
                                                          while(strin >> Info_Line){
                                                                      Info_Bond.push_back(Info_Line);
                                                                      }
                                                          }
                           if (Serial_Line>Line_Info_Angle && Serial_Line<Line_Info_Torsion){
                                                           while(strin >> Info_Line){
                                                                       Info_Angle.push_back(Info_Line);
                                                                       }
                                                           }
                           if (Serial_Line>Line_Info_Torsion && Serial_Line<Line_Info_End){
                                                             while(strin >> Info_Line){
                                                                         Info_Torsion.push_back(Info_Line);
                                                                      }
                                                             }
                           Prop_Atom.push_back(Info_Atom);
                           Prop_Bond.push_back(Info_Bond);
                           Prop_Angle.push_back(Info_Angle);
                           Prop_Torsion.push_back(Info_Torsion);
                           For_Bond.push_back(Info_Bond.size());
                           For_Angle.push_back(Info_Angle.size());
                           For_Torsion.push_back(Info_Torsion.size());
                           
                           }
                           
     }
     

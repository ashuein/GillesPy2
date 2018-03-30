#include <string>
#include <vector>
#include <iostream>
#include "model.h"
#include "ssa.h"
using namespace Gillespy;


//Default constants
const uint number_trajectories = 7;
const uint number_timesteps = 101;
const double end_time = 100;
const int random_seed = 9001;
const double volume = 1.0;


//Species Init
std :: string s_names[] = {"A","B","C","D"};
uint populations[] = {301, 120, 0, 0}; 
//Reactions Init
std :: string r_names[] = {"r1","r2","r3"};


//User defined constants
const double rate1 = 0.0017;
const double rate2 = 0.5;
const double rate3 = 0.1;

class PropensityFunction : public IPropensityFunction{
public:
  double evaluate(uint reaction_number, uint* state){
    switch(reaction_number){
    case 0:
      return rate1 * state[0] * state[1] / volume;
    case 1:
      return rate2 * state[2];
    case 2:
      return rate3 * state[2];
    default: //Error
      return -1;
    }
  }
};

int main(){
  std :: vector<std :: string> species_names(s_names, s_names + sizeof(s_names)/sizeof(std :: string));
  std :: vector<uint> species_populations(populations, populations + sizeof(populations)/sizeof(populations[0]));
  std :: vector<std :: string> reaction_names(r_names, r_names + sizeof(r_names)/sizeof(std :: string));
  
  Model model(species_names, species_populations, reaction_names);

  //Begin reaction species changes
  model.reactions[0].species_change[0] = -1;
  model.reactions[0].species_change[1] = -1;
  model.reactions[0].species_change[2] = 1;

  model.reactions[1].species_change[0] = 1;
  model.reactions[1].species_change[1] = 1;
  model.reactions[1].species_change[2] = -1;
  
  model.reactions[2].species_change[1] = 1;
  model.reactions[2].species_change[2] = -1;
  model.reactions[2].species_change[3] = 1;

  //End reaction species changes
  model.update_affected_reactions();
  
  IPropensityFunction *propFun = new PropensityFunction();
  Simulation simulation(&model, number_trajectories, number_timesteps, end_time, propFun, random_seed);
  ssa_direct(&simulation);
  std :: cout << simulation << std :: endl;
  delete propFun;
  return 0;
}
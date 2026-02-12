# Thorium-ADS-Optimization
This repository contains a radiation simulation project based on Geant4 version 11.03 [MT]. 
The primary focus is calculating and comparing the Absorbed Dose ($D = \frac{E}{m}$)  in a scoring volume using different particles, energies, and interaction mechanisms.


## Prerequisites & Setup
Environment
The simulation is developed on a WSL-based Ubuntu environment. To run the simulation, the Geant4 environment must be sourced:

```
source /home/umut/geant4-v11.3.0-install/bin/geant4.sh
``` 

Ensure the following datasets are manually installed in the share/Geant4/data directory to avoid runtime exceptions:

    G4PARTICLEXS 4.0 (Cross-sections) 
    G4EMLOW 8.5 (Low-energy EM interactions)

## Execution
To run a simulation using a macro file and log the output:
``` 
./exampleB1 <macro_file>.mac > <output_log>.txt
```

## Experiment Logs & Results
Three main experiments have been conducted using the FTFP_BERT physics engine with 10,000 events each. The scoring volume has a mass of 8300.87 kg. 

### Key Observations
Particle Comparison: At 1 GeV, Gamma rays resulted in a ~2.16x higher dose than Protons, likely due to secondary electromagnetic showers.


Energy Efficiency: Dropping Gamma energy from 1 GeV to 10 MeV showed a non-linear dose reduction, indicating higher interaction efficiency at lower therapeutic energy ranges

## Project Structure
src/ & include/: Source code for detector construction and primary generators.

*.mac: Macro files for different experimental configurations.

experiment_reports.txt: Detailed scientific logs of each run.

*.log / *.txt: Raw output data from Geant4 worker threads.

## Future Work
B1-004: Material substitution study replacing Water with Lead (G4_Pb) to evaluate shielding effectiveness.



```
 Author: Umut
 Date: February 2026
```
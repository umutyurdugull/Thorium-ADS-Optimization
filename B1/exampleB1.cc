#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

int main(int argc,char** argv)
{
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4Random::setTheEngine(new CLHEP::RanecuEngine);

#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
  auto runManager = new G4RunManager;
#endif

  runManager->SetUserInitialization(new B1::DetectorConstruction());

  auto physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new B1::ActionInitialization());

  auto visManager = new G4VisExecutive;
  visManager->Initialize();

  auto UImanager = G4UImanager::GetUIpointer();

  if ( ! ui ) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}

/* NOTES:
1. G4MTRunManager is used to enable multi-threading support for parallel event processing.
2. The number of threads is automatically set to the number of available CPU cores.
3. FTFP_BERT is the physics list responsible for hadronic and electromagnetic interactions.
4. ActionInitialization registers user actions like RunAction and EventAction for each thread.
*/
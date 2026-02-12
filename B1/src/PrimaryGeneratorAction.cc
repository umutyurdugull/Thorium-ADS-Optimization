#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  
  fParticleGun->SetParticleDefinition(particle);
  
  
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm, 2.0*cm, -15.0*cm));

  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); 

 
  fParticleGun->SetParticleEnergy(1. * GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // RASTGELELEŞTİRME KODLARINI SİLDİK
  // Böylece her atış tam merkezden Toryum hedefine gidecek.
  fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
#include "DetectorConstruction.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"

namespace B1
{

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  G4double env_sizeXY = 200 * cm, env_sizeZ = 300 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  G4bool checkOverlaps = true;

  G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
  auto logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);

  auto solidEnv = new G4Box("Envelope", 0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);
  auto logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  G4double density;
  G4int ncomponents, natoms;

  G4Isotope* iso_Th232 = new G4Isotope("Thorium232", 90, 232, 232.04*g/mole);
  G4Element* el_Th = new G4Element("Thorium", "Th", ncomponents=1);
  el_Th->AddIsotope(iso_Th232, 100.*perCent);

  G4Material* Th_Mat = new G4Material("ThoriumMetal", density= 11.7*g/cm3, ncomponents=1);
  Th_Mat->AddElement(el_Th, natoms=1);
  G4Material* shape1_mat = Th_Mat;

  G4Material* refl_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");

  G4ThreeVector pos1 = G4ThreeVector(0, 0, -60 * cm);
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 80 * cm);

  G4double refl_sizeXY = 110 * cm;
  G4double refl_sizeZ  = 110 * cm;
  auto solidRefl = new G4Box("Reflector", 0.5 * refl_sizeXY, 0.5 * refl_sizeXY, 0.5 * refl_sizeZ);
  auto logicRefl = new G4LogicalVolume(solidRefl, refl_mat, "Reflector");
  new G4PVPlacement(nullptr, pos1, logicRefl, "Reflector", logicEnv, false, 0, checkOverlaps);

  G4double shape1_rmina = 0. * cm, shape1_rmaxa = 45. * cm;
  G4double shape1_rminb = 0. * cm, shape1_rmaxb = 50. * cm;
  G4double shape1_hz = 50. * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                 shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1, shape1_mat, "Shape1");
  new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicShape1, "Shape1", logicRefl, false, 0, checkOverlaps);

  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
  G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
  G4double shape2_dz = 6 * cm;
  auto solidShape2 = new G4Trd("Shape2", 0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb, 0.5 * shape2_dz);
  auto logicShape2 = new G4LogicalVolume(solidShape2, shape2_mat, "Shape2");
  new G4PVPlacement(nullptr, pos2, logicShape2, "Shape2", logicEnv, false, 0, checkOverlaps);

  fScoringVolume = logicShape1;

  return physWorld;
}

}

/*

Hazırlayan : Umut YURDUGÜL

Bu kodda "Büyük Blok" konfigürasyonuna ek olarak bir "Nötron Reflektörü" (Yansıtıcı) deneyi kurgulanmıştır. 
1. Materyal: Nötronları yansıtma kapasitesi yüksek olan Grafit (G4_GRAPHITE) sisteme dahil edilmiştir.
2. Hiyerarşi (Daughter-Mother): Toryum bloğu (Shape1) artık doğrudan 'logicEnv' içine değil, 'logicRefl' (Yansıtıcı) içine yerleştirilmiştir. 
   Bu sayede Toryum'dan kaçmaya çalışan nötronlar önce Grafit tabakasına çarpar.
3. Pozisyonlama: Reflektör 'pos1' (-60 cm) konumuna yerleştirilmiş, Toryum bloğu ise reflektörün tam merkezine (0,0,0) konumlandırılmıştır.
4. Beklenen Sonuç: Bu yapı ile kaçak nötronların sisteme geri dönmesi sağlanarak, önceki deneyde elde edilen Th-233 breeding (yakıt üretim) oranının 
   ve nötron ekonomisinin (Notron_Verimi_Proton_Basina) artırılması hedeflenmektedir.
*/
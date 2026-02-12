import re
import csv
import os

def log_parse_et(dosya_yolu):
    sonuclar = {
        "Toplam_Enerji_Birikimi_GeV": 0.0,
        "Emilen_Doz_nanoGy": 0.0,
        "Uretilen_Net_Notron_Sayisi": 0,
        "Toryum_Breeding_Sayisi_Th233": 0,
        "Fisyon_Izleri_Sr_Sb_Ce_Ge": 0,
        "Kacak_Notron_Sayisi": 0,
        "Ortalama_Notron_Enerjisi_MeV": 0.0,
        "Notron_Verimi_Proton_Basina": 0.0
    }

    if not os.path.exists(dosya_yolu):
        print("Log dosyasi bulunamadi")
        return None

    notron_enerjileri = []
    with open(dosya_yolu, 'r') as f:
        lines = f.readlines()
        content = "".join(lines)

        edep_matches = re.findall(r"cumulated edep per run in scoring volume = ([\d\.]+) (GeV|MeV)", content)
        if edep_matches:
            val, birim = edep_matches[-1]
            sonuclar["Toplam_Enerji_Birikimi_GeV"] = float(val) if birim == "GeV" else float(val) / 1000.0

        dose_matches = re.findall(r"Absorbed dose per run in scoring volume = edep/mass = ([\d\.]+) (nanoGy|picoGy)", content)
        if dose_matches:
            val, birim = dose_matches[-1]
            sonuclar["Emilen_Doz_nanoGy"] = float(val) if birim == "nanoGy" else float(val) / 1000.0

        for i in range(len(lines)):
            if "Particle = neutron" in lines[i]:
                sonuclar["Uretilen_Net_Notron_Sayisi"] += 1
                
                search_idx = i + 1
                is_kacak = False
                energy_found = False
                
                while search_idx < len(lines) and "*****************" not in lines[search_idx]:
                    current_line = lines[search_idx]
                    
                    if "initStep" in current_line and not energy_found:
                        e_match = re.search(r"(\d+\.?\d*)\s*(MeV|keV|eV)", current_line)
                        if e_match:
                            e_val = float(e_match.group(1))
                            unit = e_match.group(2)
                            if unit == "keV": e_val /= 1000
                            elif unit == "eV": e_val /= 1000000
                            notron_enerjileri.append(e_val)
                            energy_found = True
                    
                    if "OutOfWorld" in current_line:
                        is_kacak = True
                    
                    search_idx += 1
                
                if is_kacak:
                    sonuclar["Kacak_Notron_Sayisi"] += 1

        sonuclar["Toryum_Breeding_Sayisi_Th233"] = content.count("Particle = Th233")
        
        fisyon_list = ["Sr97", "Sb129", "Ce145", "Ge79"]
        for el in fisyon_list:
            sonuclar["Fisyon_Izleri_Sr_Sb_Ce_Ge"] += content.count(f"Particle = {el}")

    if notron_enerjileri:
        sonuclar["Ortalama_Notron_Enerjisi_MeV"] = sum(notron_enerjileri) / len(notron_enerjileri)
    
    sonuclar["Notron_Verimi_Proton_Basina"] = sonuclar["Uretilen_Net_Notron_Sayisi"] / 100.0

    return sonuclar

log_dosyasi = "sim_output.txt"
veri = log_parse_et(log_dosyasi)

if veri:
    with open('Thorium_ADS_Results.csv', 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=veri.keys())
        writer.writeheader()
        writer.writerow(veri)
    
    print("Veriler CSV Dosyasina Aktarildi")
    for k, v in veri.items():
        print(f"{k}: {v}")
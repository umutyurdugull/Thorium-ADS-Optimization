import re
import csv
import os

def analiz_et(log_file, etiket):
    if not os.path.exists(log_file):
        return None

    data = {
        "Deney": etiket,
        "Edep_Total_GeV": 0.0,
        "Net_Notron_Sayisi": 0,
        "Uranyum_Oncusu_Th233": 0,
        "Fisyon_Olaylari": 0,
        "Kacak_Notron_Sayisi": 0,
        "Ortalama_Notron_Enerjisi_MeV": 0.0,
        "Notron_Verimi_Proton_Basina": 0.0
    }

    notron_enerjileri = []
    
    with open(log_file, 'r') as f:
        lines = f.readlines()
        content = "".join(lines)

        edep = re.findall(r"cumulated edep per run in scoring volume = ([\d\.]+) (GeV|MeV)", content)
        if edep:
            val, unit = edep[-1]
            data["Edep_Total_GeV"] = float(val) if unit == "GeV" else float(val)/1000

        dose = re.findall(r"Absorbed dose per run in scoring volume = edep/mass = ([\d\.]+) (nanoGy|picoGy)", content)

        for i, line in enumerate(lines):
            if "Particle = neutron" in line:
                data["Net_Notron_Sayisi"] += 1
                search_idx = i + 1
                energy_found = False
                while search_idx < len(lines) and "*****************" not in lines[search_idx]:
                    if "initStep" in lines[search_idx] and not energy_found:
                        e_match = re.search(r"(\d+\.?\d*)\s*(MeV|keV|eV)", lines[search_idx])
                        if e_match:
                            e_val = float(e_match.group(1))
                            unit = e_match.group(2)
                            if unit == "keV": e_val /= 1000
                            elif unit == "eV": e_val /= 1000000
                            notron_enerjileri.append(e_val)
                            energy_found = True
                    if "OutOfWorld" in lines[search_idx]:
                        data["Kacak_Notron_Sayisi"] += 1
                    search_idx += 1

        data["Uranyum_Oncusu_Th233"] = content.count("Particle = Th233")
        fisyon_elemanlar = ["Sr97", "Sb129", "Ce145", "Ge79"]
        for el in fisyon_elemanlar:
            data["Fisyon_Olaylari"] += content.count(f"Particle = {el}")

    if notron_enerjileri:
        data["Ortalama_Notron_Enerjisi_MeV"] = sum(notron_enerjileri) / len(notron_enerjileri)
    
    data["Notron_Verimi_Proton_Basina"] = data["Net_Notron_Sayisi"] / 100.0

    return data


dosyalar = [
    ("small_output.txt", "Kucuk_Blok_10cm"),
    ("large_output.txt", "Buyuk_Blok_100cm"),
    ("large_reflector_output.txt", "Buyuk_Blok_Reflektorlu")
]

sonuclar = []
for dosya, etiket in dosyalar:
    res = analiz_et(dosya, etiket)
    if res:
        sonuclar.append(res)

if sonuclar:
    with open('Toryum_ADS_Karsilastirma.csv', 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=sonuclar[0].keys())
        writer.writeheader()
        writer.writerows(sonuclar)
    
    print("Tum veriler 'Toryum_ADS_Karsilastirma.csv' dosyasina aktarildi.")
    for s in sonuclar:
        print(f"\n--- {s['Deney']} ---")
        print(f"Edep: {s['Edep_Total_GeV']} GeV | Notron Verimi: {s['Notron_Verimi_Proton_Basina']}")
        print(f"Breeding (Th233): {s['Uranyum_Oncusu_Th233']} | Kacalar: {s['Kacak_Notron_Sayisi']}")
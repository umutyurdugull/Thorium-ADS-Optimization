import matplotlib.pyplot as plt

# Verilerimiz
deneyler = ['Kucuk (10cm)', 'Buyuk (100cm)', 'Buyuk + Reflektor']
breeding = [80, 387, 430]
verim = [7.02, 12.41, 12.25]

fig, ax1 = plt.subplots(figsize=(10, 6))

# Breeding Bar Grafigi
color = 'tab:blue'
ax1.set_xlabel('Deney Senaryosu')
ax1.set_ylabel('Th-233 Breeding Sayisi', color=color)
ax1.bar(deneyler, breeding, color=color, alpha=0.6, label='Breeding (Th-233)')
ax1.tick_params(axis='y', labelcolor=color)

# Notron Verimi Line Grafigi
ax2 = ax1.twinx()
color = 'tab:red'
ax2.set_ylabel('Proton Basina Notron Verimi', color=color)
ax2.plot(deneyler, verim, color=color, marker='o', linewidth=2, label='Notron Verimi')
ax2.tick_params(axis='y', labelcolor=color)

plt.title('Toryum ADS Sisteminde Geometrik Optimizasyon Sonuclari')
fig.tight_layout()
plt.savefig('Toryum_Analiz_Grafigi.png')
print("Grafik 'Toryum_Analiz_Grafigi.png' olarak kaydedildi.")
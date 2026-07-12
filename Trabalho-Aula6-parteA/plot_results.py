import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results_summary.csv")

fig, axes = plt.subplots(1, 2, figsize=(12,5))

ax = axes[0]
ax.bar(df["size"].astype(str), df["mean_score"], yerr=df["std_score"],
       capsize=6, color="#4C72B0", edgecolor="black")
ax.set_xlabel("Tamanho da sala (h=w)")
ax.set_ylabel("Utilidade média (score final)")
ax.set_title("Utilidade média ± desvio padrão\n(10 configurações por tamanho)")
ax.axhline(0, color="black", linewidth=0.8)

ax2 = axes[1]
ax2.bar(df["size"].astype(str), df["mean_moves"], yerr=df["std_moves"],
        capsize=6, color="#DD8452", edgecolor="black")
ax2.set_xlabel("Tamanho da sala (h=w)")
ax2.set_ylabel("Número médio de movimentos")
ax2.set_title("Movimentos até resolver\n(sucesso em 100% dos casos)")

plt.tight_layout()
plt.savefig("resultados.png", dpi=150)
print("saved resultados.png")

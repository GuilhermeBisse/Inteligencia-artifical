# Trabalho desenvolvido pelos alunos Felipe Matsuo, Guilherme Bisse, Gustavo Abelio e Pedro Ito

# Esse script python é para análise dos resultados obtidos no arquivo "resultados_puzzle.csv" e geração de gráficos.
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

PASTA_GRAFICOS = "graficos-analise"

if not os.path.exists(PASTA_GRAFICOS):
    os.makedirs(PASTA_GRAFICOS)

df = pd.read_csv("resultados_puzzle.csv", sep=";")
df["Passos"] = pd.to_numeric(df["Passos"], errors="coerce")

df_sucesso = df.dropna(subset=["Passos"])
df_falha = df[df["Passos"].isna()]

print("Total:", len(df))
print("Sucessos:", len(df_sucesso))
print("Falhas:", len(df_falha))

df_sucesso["Algoritmo_fmt"] = df_sucesso["Algoritmo"]

df_sucesso.loc[
    (df_sucesso["Algoritmo"] == "A*") & (df_sucesso["Heuristica"] == "Manhattan"),
    "Algoritmo_fmt"
] = "A* hm"

df_sucesso.loc[
    (df_sucesso["Algoritmo"] == "A*") & (df_sucesso["Heuristica"] == "ForaLugar"),
    "Algoritmo_fmt"
] = "A* hfora"

df_sucesso.loc[
    (df_sucesso["Algoritmo"] == "IDA*") & (df_sucesso["Heuristica"] == "Manhattan"),
    "Algoritmo_fmt"
] = "IDA* hm"

df_sucesso.loc[
    (df_sucesso["Algoritmo"] == "IDA*") & (df_sucesso["Heuristica"] == "ForaLugar"),
    "Algoritmo_fmt"
] = "IDA* hfora"

media = df_sucesso.groupby("Algoritmo_fmt")[["Tempo(s)", "Passos"]].mean()
desvio = df_sucesso.groupby("Algoritmo_fmt")[["Tempo(s)", "Passos"]].std()

print("\nMÉDIAS\n", media)
print("\nDESVIO PADRÃO\n", desvio)

plt.figure()
for alg in df_sucesso["Algoritmo_fmt"].unique():
    sub = df_sucesso[df_sucesso["Algoritmo_fmt"] == alg]
    plt.scatter(sub["Passos"], sub["Tempo(s)"], label=alg)

plt.yscale("log")
plt.xlabel("Número de Passos")
plt.ylabel("Tempo (s)")
plt.legend()
plt.tight_layout()
plt.savefig(f"{PASTA_GRAFICOS}/tempo_vs_passos.png")

plt.figure()
media["Tempo(s)"].plot(kind="bar")
plt.title("Tempo Médio por Algoritmo")
plt.ylabel("Tempo (s)")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig(f"{PASTA_GRAFICOS}/tempo_medio.png")

plt.figure()
media["Passos"].plot(kind="bar")
plt.title("Passos Médios")
plt.ylabel("Passos")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig(f"{PASTA_GRAFICOS}/passos_medios.png")

plt.figure()
df_sucesso.boxplot(column="Tempo(s)", by="Algoritmo_fmt")
plt.yscale("log")
plt.title("Distribuição do Tempo (log)")
plt.suptitle("")
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig(f"{PASTA_GRAFICOS}/boxplot_tempo.png")

falhas = df.groupby("Algoritmo")["Passos"].apply(lambda x: x.isna().sum())
print("\nFALHAS\n", falhas)

mais_rapido = media["Tempo(s)"].idxmin()
melhor_caminho = media["Passos"].idxmin()
mais_falhas = falhas.idxmax()

print("\nCONCLUSÕES")
print("Mais rápido:", mais_rapido)
print("Melhor caminho:", melhor_caminho)
print("Mais falhas:", mais_falhas)

with open(f"{PASTA_GRAFICOS}/relatorio.txt", "w") as f:
    f.write("MÉDIAS\n")
    f.write(media.to_string())
    f.write("\n\nDESVIO PADRÃO\n")
    f.write(desvio.to_string())
    f.write("\n\nFALHAS\n")
    f.write(falhas.to_string())
    f.write("\n\nCONCLUSÕES\n")
    f.write(f"Mais rápido: {mais_rapido}\n")
    f.write(f"Melhor caminho: {melhor_caminho}\n")
    f.write(f"Mais falhas: {mais_falhas}\n")
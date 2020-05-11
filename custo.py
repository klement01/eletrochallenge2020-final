def p_parque(h):
	if 7 <= h < 22:
		return 3800
	return 3325

def demanda(h):
	if (6 <= h < 14) or (18 <= h):
		return 8510
	return 4620

preco = 0.503

custo_diario = 0
for h in range(24):
	custo_diario += demanda(h) - p_parque(h)
custo_diario *= preco

print("Condições ideais (operação contínua):")
print(f"Custo diário: {custo_diario:.3f}")
print(f"Custo mensal: {custo_diario*30:.3f}")

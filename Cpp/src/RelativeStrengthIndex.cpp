/*
 * RelativeStrengthIndex.cpp
 *
 *  Created on: 23 de jan de 2017
 *      Author: leo
 */

#include "RelativeStrengthIndex.h"

using namespace std;

vector<Indice*> RelativeStrengthIndex::instancia_combinacoes(Security_data sd) {
	vector<Indice*> indices;

	for (size_t w = (size_t) RSI_TAMANHO_INICIAL; w <= (size_t) RSI_TAMANHO_MAXIMO; w += (size_t) RSI_TAMANHO_INCREMENTO) {
		for (int lu = (int) RSI_LIMIAR_SUPERIOR_INICIAL; lu <= (int) RSI_LIMIAR_SUPERIOR_MAXIMO; lu += (int) RSI_LIMIAR_SUPERIOR_INCREMENTO) {
			for (int li = (int) RSI_LIMIAR_INFERIOR_INICIAL; li <= (int) RSI_LIMIAR_INFERIOR_MAXIMO; li += (int) RSI_LIMIAR_INFERIOR_INCREMENTO) {
				indices.push_back(new RelativeStrengthIndex(sd, w, lu, li));
			}
		}
	}

	return indices;
}

short *RelativeStrengthIndex::pontos_de_decisao() {
	setPeriodoAtual(1);

	double* ups = new double[w];
	double* downs = new double[w];

	for (size_t i = 0; i < w; ++i) {
		auto up_and_down = calcula_up_and_down();

		ups[i] = get < 0 > (up_and_down);
		downs[i] = get < 1 > (up_and_down);

		incrementa_periodo();
	}

	double averageCloseUP = Utils::calcula_media(ups, 0, w);
	double averageCloseDOWN = Utils::calcula_media(downs, 0, w);

	delete[] ups;
	delete[] downs;

	rsi_array[periodo_anterior] = rsi(averageCloseUP, averageCloseDOWN);

	while (periodo_atual < security_data.tamanho_close_prices_vet) {

		auto up_and_down = calcula_up_and_down();

		averageCloseUP = ((averageCloseUP * (w - 1)) + get < 0 > (up_and_down)) / w;
		averageCloseDOWN = ((averageCloseDOWN * (w - 1)) + get < 1 > (up_and_down)) / w;

		rsi_array[periodo_atual] = rsi(averageCloseUP, averageCloseDOWN);

		pontos_decisao[periodo_atual] = this->decisao(periodo_anterior);

		incrementa_periodo();
	}

//	Utils::printVet(rsi_array, 0, security_data.tamanho_close_prices_vet + 1);

//	Utils::printVet(pontos_decisao, 0, security_data.tamanho_close_prices_vet + 1);

	return pontos_decisao;
}

double RelativeStrengthIndex::rsi(double averageCloseUP, double averageCloseDOWN) {
	double rs = averageCloseUP / averageCloseDOWN;
	return (100 - (100 / (1 + rs)));
}

short RelativeStrengthIndex::decisao() {
	//TODO:
	return decisao(periodo_anterior);
}

short RelativeStrengthIndex::decisao(size_t dia_de_referencia) {
	if (rsi_array[dia_de_referencia] > limiar_superior)
		return VENDE;
	else if (rsi_array[dia_de_referencia] < limiar_inferior)
		return COMPRA;
	else
		return NADA;
}

int RelativeStrengthIndex::periodoInicial() {
	return this->w + 1;
}

RelativeStrengthIndex::RelativeStrengthIndex(const Security_data sd, int w, int limiar_superior, int limiar_inferior)
		: w(w), limiar_superior(limiar_superior), limiar_inferior(limiar_inferior) {
	security_data = sd;

	pontos_decisao = new short[security_data.tamanho_close_prices_vet + 1];
	rsi_array = new double[security_data.tamanho_close_prices_vet + 1];

	Utils::set_value(UNSETED, rsi_array, 0, w);
}

RelativeStrengthIndex::~RelativeStrengthIndex() {
	delete[] pontos_decisao;
	delete[] rsi_array;
}

void RelativeStrengthIndex::imprime_resultado() {
	printf("%25s - Janela: %3d - Lim. Sup: %d - Lim. Inf: %d | ", RELATIVESTRENGTHINDEX_STR, (int) w, limiar_superior, limiar_inferior);
}


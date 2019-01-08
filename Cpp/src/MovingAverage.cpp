/*
 * MovingAverage.cpp
 *
 *  Created on: 16 de jan de 2017
 *      Author: leo
 */

#include "MovingAverage.h"

vector<Indice*> MovingAverage::instancia_combinacoes(Security_data sd) {
	vector<Indice*> indices;

	for (size_t w = (size_t) MA_TAMANHO_INICIAL; w <= (size_t) MA_TAMANHO_MAXIMO; w += (size_t) MA_TAMANHO_INCREMENTO) {
		indices.push_back(new MovingAverage(sd, w));
	}

	return indices;
}

short* MovingAverage::pontos_de_decisao() {
	setPeriodoAtual(w);

	while (periodo_atual < (security_data.tamanho_close_prices_vet + 1)) {

		pontos_decisao[periodo_atual] = decisao();

		incrementa_periodo();
	}

	return pontos_decisao;
}

short MovingAverage::decisao() {
	media_movel[periodo_anterior] = Utils::calcula_media(security_data.close_prices_vet, periodo_atual - w, periodo_atual);

	return decisao(periodo_anterior);
}

short MovingAverage::decisao(size_t dia_de_referencia) {
	if (security_data.close_prices_vet[dia_de_referencia] > media_movel[dia_de_referencia])
		return VENDE;
	else if (security_data.close_prices_vet[dia_de_referencia] < media_movel[dia_de_referencia])
		return COMPRA;
	else
		return NADA;
}

int MovingAverage::periodoInicial() {
	return this->w;
}

void MovingAverage::imprime_resultado() {
	printf("%25s - Janela: %3d | ", MOVINGAVERAGE_STR, (int) w);
}

MovingAverage::MovingAverage(const Security_data sd, int w)
		: w(w) {
	security_data = sd;

	pontos_decisao = new short[security_data.tamanho_close_prices_vet + 1];
	media_movel = new double[security_data.tamanho_close_prices_vet + 1];
}

MovingAverage::~MovingAverage() {
	delete[] pontos_decisao;
	delete[] media_movel;
}


/*
 * BollingerBands.cpp
 *
 *  Created on: 27 Oct 2016
 *      Author: leo
 */

#include "ThreadHandle.h"
#include "Utils.h"
#include "BollingerBands.h"

#include "Domain.h"
#include "TraderIndiceSimulator.h"

using namespace std;

vector<Indice*> BollingerBands::instancia_combinacoes(Security_data sd) {
	vector<Indice*> indices;

	for (size_t w = (size_t) BB_TAMANHO_INICIAL; w <= (size_t) BB_TAMANHO_MAXIMO; w += (size_t) BB_TAMANHO_INCREMENTO) {
		for (float d = (float) BB_DESVIOS_INICIAL; d <= (float) BB_DESVIOS_MAXIMO; d += (float) BB_DESVIOS_INCREMENTO) {
			indices.push_back(new BollingerBands(sd, w, d));
		}
	}

	return indices;
}

short* BollingerBands::pontos_de_decisao() {
	setPeriodoAtual(w);

	while (periodo_atual < (security_data.tamanho_close_prices_vet + 1)) {

		pontos_decisao[periodo_atual] = decisao();

		incrementa_periodo();
	}

	return pontos_decisao;
}

short BollingerBands::decisao() {
	media_movel[periodo_anterior] = Utils::calcula_media(security_data.close_prices_vet, periodo_atual - w, periodo_atual);

	double desvio = Utils::calcula_desvio(security_data.close_prices_vet, periodo_atual - w, periodo_atual, media_movel[periodo_anterior]);

	upper_band[periodo_anterior] = media_movel[periodo_anterior] + (d) * desvio;
	lower_band[periodo_anterior] = media_movel[periodo_anterior] - (d) * desvio;

	return decisao(periodo_anterior);
}

short BollingerBands::decisao(size_t dia_de_referencia) {
	if (security_data.close_prices_vet[dia_de_referencia] >= upper_band[dia_de_referencia])
		return VENDE;
	else if (security_data.close_prices_vet[dia_de_referencia] <= lower_band[dia_de_referencia])
		return COMPRA;
	else
		return NADA;
}

int BollingerBands::periodoInicial() {
	return this->w;
}

void BollingerBands::imprime_resultado() {
	printf("%25s - Janela: %3d - Desvios: %4.1f | ", BOLLINGERBANDS_STR, (int) w, d);
//	cout << setw(30) << left << title << "  " << setw(30) << left << author << "  " << '$' << setw(6) << right << price << "  " << qty << endl;

}

BollingerBands::BollingerBands(const Security_data sd, int w, float d)
		: w(w), d(d) {
	security_data = sd;

	pontos_decisao = new short[security_data.tamanho_close_prices_vet + 1];
	media_movel = new double[security_data.tamanho_close_prices_vet + 1];
	upper_band = new double[security_data.tamanho_close_prices_vet + 1];
	lower_band = new double[security_data.tamanho_close_prices_vet + 1];
}

BollingerBands::~BollingerBands() {
	delete[] pontos_decisao;
	delete[] media_movel;
	delete[] upper_band;
	delete[] lower_band;
}

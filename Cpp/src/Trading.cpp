/*
 * Trading.cpp
 *
 *  Created on: 2 de jan de 2017
 *      Author: leo
 */

#include "Trading.h"

using namespace std;

void Trading::run_indice_simulations() {
	for (Security_data sd : security_datas) {
		cout << "----------------------------------------------------------------------------------------------------------------------------------\n";

		TraderIndiceSimulator* traderIndiceSimulator;

		vector<pair<string, short>> decisoes;

		if (indices_to_use.count(BOLLINGERBANDS_STR)) {
			traderIndiceSimulator = new TraderIndiceSimulator(sd, BollingerBands::instancia_combinacoes(sd));
			traderIndiceSimulator->run_simulation();

			traderIndiceSimulator->imprime_resultado();

			if (traderIndiceSimulator->getRunnerSelecionado() != nullptr) {
				decisoes.push_back(make_pair(BOLLINGERBANDS_STR, traderIndiceSimulator->getRunnerSelecionado()->indice->decisao()));
			}

			delete traderIndiceSimulator;
		}

		if (indices_to_use.count(MOVINGAVERAGE_STR)) {
			traderIndiceSimulator = new TraderIndiceSimulator(sd, MovingAverage::instancia_combinacoes(sd));
			traderIndiceSimulator->run_simulation();

			traderIndiceSimulator->imprime_resultado();

			if (traderIndiceSimulator->getRunnerSelecionado() != nullptr) {
				decisoes.push_back(make_pair(MOVINGAVERAGE_STR, traderIndiceSimulator->getRunnerSelecionado()->indice->decisao()));
			}

			delete traderIndiceSimulator;
		}

		if (indices_to_use.count(RELATIVESTRENGTHINDEX_STR)) {
			traderIndiceSimulator = new TraderIndiceSimulator(sd, RelativeStrengthIndex::instancia_combinacoes(sd));
			traderIndiceSimulator->run_simulation();

			traderIndiceSimulator->imprime_resultado();

			if (traderIndiceSimulator->getRunnerSelecionado() != nullptr) {
				decisoes.push_back(make_pair(RELATIVESTRENGTHINDEX_STR, traderIndiceSimulator->getRunnerSelecionado()->indice->decisao()));
			}

			delete traderIndiceSimulator;
		}

		decisoes_dos_indices.emplace(sd.ticker, decisoes);

		Utils::printVet(decisoes.data(), 0, decisoes.size(), [](pair<string, short> e) {return "Decisao: " + to_string(get<1>(e)) + " " + get<0>(e);});

		cout << "----------------------------------------------------------------------------------------------------------------------------------\n";
	}
}

void Trading::set_and_run(Security_data sd, vector<Indice*> combinacoes_de_indice) {
}

Trading::Trading(vector<Security_data> sds)
		: security_datas(sds) {
}

Trading::~Trading() {
}

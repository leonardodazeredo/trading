/*
 * Trader.cpp
 *
 *  Created on: 23 Oct 2016
 *      Author: leo
 */
#include "TraderIndiceSimulator.h"

#include "Indice.h"
#include "BollingerBands.h"
#include "ThreadHandle.h"

void TraderIndiceSimulator::run_simulation(bool parallel) {
	if (general_verbose_mode) {
		cout << "\n Numero de combinacoes do indice: " << simulation_runners.size() << endl;
	}

	if (parallel) {
		ThreadHandle::create_threads_and_join_all(simulation_runners.data(), simulation_runners.size(), IndiceSimulationRunner::indiceSimulationRunner_threader);
	}
	else {
		for (IndiceSimulationRunner* indiceSimulationRunner : simulation_runners) {
			indiceSimulationRunner->run();
		}
	}
}

void TraderIndiceSimulator::run_simulation() {
	run_simulation(indice_simulation_parallel_mode);
}

void TraderIndiceSimulator::imprime_resultado() {
	const IndiceSimulationRunner* indiceSimulationRunnerSelecionado = getRunnerSelecionado();

	if (general_debug_mode) {
		for (long i = ((debug_mode_result_list_size <= simulation_runners.size() ? debug_mode_result_list_size : simulation_runners.size()) - 1); i >= 0; i--) {

			printf("Ativo: %6s | ", simulation_runners[i]->security_data.ticker.c_str());
			printf("Acum.: %8.2f %% | ", AS_RELATIVE_ASPERCENTAGE(simulation_runners[i]->resultados.retorno_acumulado));
			printf("%10.2f(r) | ", simulation_runners[i]->getCapitalReal(simulation_runners[i]->security_data.tamanho_close_prices_vet - 1));
			printf("%8d(a) | ", (int) simulation_runners[i]->simulation_data.num_acoes_periodo_array[simulation_runners[i]->security_data.tamanho_close_prices_vet - 1]);
			printf("Volat.: %8.2f %% | ", simulation_runners[i]->resultados.volatilidade);
			simulation_runners[i]->indice->imprime_resultado();
			printf("\n");

		}

		return;
	}

	if (indiceSimulationRunnerSelecionado != nullptr) {
		printf("Ativo: %6s | ", indiceSimulationRunnerSelecionado->security_data.ticker.c_str());
		printf("Acum.: %8.2f %% | ", AS_RELATIVE_ASPERCENTAGE(indiceSimulationRunnerSelecionado->resultados.retorno_acumulado));
		printf("Volat.: %8.2f %% | ", indiceSimulationRunnerSelecionado->resultados.volatilidade);
		indiceSimulationRunnerSelecionado->indice->imprime_resultado();
		printf("\n");
	}
}

TraderIndiceSimulator::TraderIndiceSimulator(Security_data sd, vector<Indice*> combinacoes_de_indice)
		: security_data(sd) {
	for (Indice* indice : combinacoes_de_indice) {
		simulation_runners.push_back(new IndiceSimulationRunner(this, security_data, indice));
	}
}

TraderIndiceSimulator::~TraderIndiceSimulator() {
	for (IndiceSimulationRunner* indiceSimulationRunner : simulation_runners) {
		delete indiceSimulationRunner;
	}
}

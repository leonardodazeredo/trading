/*
 * IndiceSimulationRunner.cpp
 *
 *  Created on: 10 de jan de 2017
 *      Author: leo
 */

#include "IndiceSimulationRunner.h"

//std::ostream& operator<<(std::ostream& os, const IndiceSimulationRunner& obj){
//    return os << "teste";
//}

void IndiceSimulationRunner::run() {
	resultados.reset();

	size_t periodo_corrente = indice->periodoInicial();

	Utils::ones(simulation_data.retorno_periodo_array, 0, periodo_corrente + 1);

	simulation_data.capital_periodo_array[periodo_corrente] = simulation_data.capital_periodo_array[periodo_corrente - 1] = capital_ini;
	simulation_data.num_acoes_periodo_array[periodo_corrente] = simulation_data.num_acoes_periodo_array[periodo_corrente - 1] = num_acoes_ini;

	auto* pontos_decisao = indice->pontos_de_decisao();

	while (periodo_corrente < (security_data.tamanho_close_prices_vet)) {
		simulation_data.capital_periodo_array[periodo_corrente] = simulation_data.capital_periodo_array[periodo_corrente - 1];
		simulation_data.num_acoes_periodo_array[periodo_corrente] = simulation_data.num_acoes_periodo_array[periodo_corrente - 1];

		int num_acoes_compra = (int) (simulation_data.capital_periodo_array[periodo_corrente] / security_data.close_prices_vet[periodo_corrente]);

		if (pontos_decisao[periodo_corrente] == VENDE && simulation_data.num_acoes_periodo_array[periodo_corrente] > 0) {
			simulation_data.capital_periodo_array[periodo_corrente] += (security_data.close_prices_vet[periodo_corrente] * simulation_data.num_acoes_periodo_array[periodo_corrente]);
			simulation_data.num_acoes_periodo_array[periodo_corrente] = 0;
		}
		else if (pontos_decisao[periodo_corrente] == COMPRA && num_acoes_compra > 0) {
			simulation_data.num_acoes_periodo_array[periodo_corrente] += num_acoes_compra;
			simulation_data.capital_periodo_array[periodo_corrente] -= (security_data.close_prices_vet[periodo_corrente] * num_acoes_compra);
		}

		simulation_data.retorno_periodo_array[periodo_corrente] = getCapitalReal(periodo_corrente) / getCapitalReal(periodo_corrente - 1);

		periodo_corrente++;
	}

	calcula_resultados();
}

IndiceSimulationRunner::IndiceSimulationRunner(TraderIndiceSimulator* traderIndiceSimulator, Security_data sd, Indice* indice)
		: traderIndiceSimulator(traderIndiceSimulator), security_data(sd), indice(indice) {
//IndiceSimulationRunner::IndiceSimulationRunner(Security_data sd, Indice* indice)
//		: security_data(sd), indice(indice) {

	simulation_data.capital_periodo_array = new double[sd.tamanho_close_prices_vet + 1];
	simulation_data.retorno_periodo_array = new double[sd.tamanho_close_prices_vet + 1];
	simulation_data.num_acoes_periodo_array = new size_t[sd.tamanho_close_prices_vet + 1];
}

IndiceSimulationRunner::~IndiceSimulationRunner() {
	delete[] simulation_data.capital_periodo_array;
	delete[] simulation_data.retorno_periodo_array;
	delete[] simulation_data.num_acoes_periodo_array;

	delete indice;
}

/**
 * para uso com o phtread
 */
void* IndiceSimulationRunner::indiceSimulationRunner_threader(void *threadarg) {
	Thread_data* td = (Thread_data*) threadarg;

	size_t tam_thread_data = td->tam_thread_data;
	IndiceSimulationRunner** runners = (IndiceSimulationRunner**) (td->dataArray);

	if (general_verbose_mode) {
		char buffer[50];
		sprintf(buffer, "- thread %d: Iniciando IndiceSimulationRunner com %lu combinacoes.\n", td->id, tam_thread_data);
		cout << buffer;
	}

	for (size_t i = 0; i < tam_thread_data; i++) {
		(*(runners + i))->run();
	}

	if (general_verbose_mode) {
		char buffer[50];
		sprintf(buffer, "- thread %d: Finalizando IndiceSimulationRunner com %lu combinacoes.\n", td->id, tam_thread_data);
		cout << buffer;
	}

	pthread_exit(nullptr);
}

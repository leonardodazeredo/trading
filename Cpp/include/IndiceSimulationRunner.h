/*
 * IndiceSimulationRunner.h
 *
 *  Created on: 10 de jan de 2017
 *      Author: leo
 */

#ifndef INDICESIMULATIONRUNNER_H_
#define INDICESIMULATIONRUNNER_H_

#include "Domain.h"
#include "Utils.h"
#include "Indice.h"

class TraderIndiceSimulator;

class IndiceSimulationRunner {
	public:
		TraderIndiceSimulator* traderIndiceSimulator;

		Security_data security_data;
		Simulation_data simulation_data;
		Resultados_struct resultados;


		Indice* indice = nullptr;

//		IndiceSimulationRunner(Security_data sd, Indice* indice);
		IndiceSimulationRunner(TraderIndiceSimulator* traderIndiceSimulator,Security_data sd, Indice* indice);
		virtual ~IndiceSimulationRunner();

		void run();

		static void* indiceSimulationRunner_threader(void *threadarg);

		static int compara_retorno_acumulado(const void* t1, const void* t2);

		inline void calcula_resultados() {
			resultados.volatilidade = getVolatilidade();
			resultados.retorno_acumulado = getRetornoAcumulado();
		}

		inline double getCapitalReal(int periodo) {
			return (simulation_data.capital_periodo_array[periodo] + simulation_data.num_acoes_periodo_array[periodo] * security_data.close_prices_vet[periodo]);
		}

		inline double getRetornoAcumulado() {
			if (resultados.retorno_acumulado == UNSETED) {
				double val = Utils::produtorio(simulation_data.retorno_periodo_array, 0, security_data.tamanho_close_prices_vet);
				resultados.retorno_acumulado = isnan(val) ? -1 : val;
			}

			return resultados.retorno_acumulado;
		}

		inline double getVolatilidade() {
			if (resultados.volatilidade == UNSETED) {
				resultados.volatilidade = Utils::volatilidade_anualizada(simulation_data.retorno_periodo_array, 0, security_data.tamanho_close_prices_vet);
			}

			return resultados.volatilidade;
		}
};

#endif /* INDICESIMULATIONRUNNER_H_ */

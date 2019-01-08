/*
 * Trader.h
 *
 *  Created on: 23 Oct 2016
 *      Author: leo
 */

#ifndef TRADERINDICESIMULATOR_H_
#define TRADERINDICESIMULATOR_H_

#include "Domain.h"
#include "Indice.h"
#include "IndiceSimulationRunner.h"
#include "Utils.h"

using namespace std;

class TraderIndiceSimulator {
	public:
		Security_data security_data;

		vector<IndiceSimulationRunner*> simulation_runners;
		IndiceSimulationRunner* runner_selecionado = nullptr;

		double volatilidade_minima = 0.0;

		TraderIndiceSimulator(Security_data sd, vector<Indice*> combinacoes_de_indice);
		virtual ~TraderIndiceSimulator();

		void run_simulation(bool parallel);
		void run_simulation();

		void imprime_resultado();

		static bool compara_retorno_acumulado(const IndiceSimulationRunner* a, const IndiceSimulationRunner* b) {
			return (a->resultados.retorno_acumulado > b->resultados.retorno_acumulado);
		}

		static bool compara_volatilidade(const IndiceSimulationRunner* a, const IndiceSimulationRunner* b) {
			return (a->resultados.volatilidade < b->resultados.volatilidade);
		}

		static bool compara_retorno_acumulado_e_volatilidade(const IndiceSimulationRunner* a, const IndiceSimulationRunner* b) {
			if (a->resultados.retorno_acumulado != b->resultados.retorno_acumulado)
				return (a->resultados.retorno_acumulado > b->resultados.retorno_acumulado);

			return (a->resultados.volatilidade < b->resultados.volatilidade);
		}

		static bool compara_volatilidade_dinamicamente(const IndiceSimulationRunner* a, const IndiceSimulationRunner* b) {
			//TODO:
			return false;
		}

		void runSelection() {
//			Utils::printVet(simulation_runners.data(), 0, simulation_runners.size(),[](IndiceSimulationRunner* i) {return i->resultados.retorno_acumulado;});

//TODO: ponto de potencial melhora de performace efetuando a operação de filter localmente.
			vector<IndiceSimulationRunner*> runners = Utils::filter(simulation_runners, [](IndiceSimulationRunner* i) {return (i->resultados.retorno_acumulado > 1.0);});

			if (!runners.empty()) {
				IndiceSimulationRunner* runner_volatilidade_minima = *min_element(runners.begin(), runners.end(), compara_volatilidade);
				volatilidade_minima = runner_volatilidade_minima->resultados.volatilidade;

				runners = Utils::filter(runners, [](IndiceSimulationRunner* i) {return i->resultados.volatilidade <= (i->traderIndiceSimulator->volatilidade_minima + volatilidade_aceitavel);});

				if (indice_simulation_metodo_selecao == CAPITAL_ACUMULADO) {
					std::sort(runners.begin(), runners.end(), compara_retorno_acumulado);
				}
				else if (indice_simulation_metodo_selecao == VOLATILIDADE_ANUALIZADA) {
					std::sort(runners.begin(), runners.end(), compara_volatilidade);
				}
				else if (indice_simulation_metodo_selecao == CAPITAL_ACUMULADO_E_VOLATILIDADE_ANUALIZADA) {
					std::sort(runners.begin(), runners.end(), compara_retorno_acumulado_e_volatilidade);
				}
				else if (indice_simulation_metodo_selecao == CAPITAL_VOLATILIDADE_DINAMICAMENTE) {
					std::sort(runners.begin(), runners.end(), compara_volatilidade_dinamicamente);
				}

				runner_selecionado = runners[0];
			}

			if (general_debug_mode)
				simulation_runners = runners;
		}

		const IndiceSimulationRunner* getRunnerSelecionado() {
			if (runner_selecionado == nullptr) {
				this->runSelection();
			}

			return runner_selecionado;
		}
};

#endif /* TRADERINDICESIMULATOR_H_ */

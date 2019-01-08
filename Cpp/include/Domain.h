/*
 * Constants.h
 *
 *  Created on: 28 de nov de 2016
 *      Author: leo
 */

#ifndef DOMAIN_H_
#define DOMAIN_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <string>
#include <tuple>
#include <utility>
#include <unordered_map>

#include <iomanip>
#include <libconfig.h++>

#include "global.h"

#define CONFIG_FILE "trademe.conf"

#define UNSETED -100

#define COMPRA 1
#define NADA 0
#define VENDE -1

#define AS_PERCENTAGE(v) ((v) * 100)
#define AS_RELATIVE_ASPERCENTAGE(v) ((v - 1) * 100)

#define CAPITAL_ACUMULADO "capital_acumulado"
#define VOLATILIDADE_ANUALIZADA "volatilidade_anualizada"
#define CAPITAL_ACUMULADO_E_VOLATILIDADE_ANUALIZADA "capital_acumulado_e_volatilidade_anualizada"
#define CAPITAL_VOLATILIDADE_DINAMICAMENTE "compara_volatilidade_dinamicamente"

using namespace std;

struct Resultados_struct {
		double retorno_acumulado = UNSETED;
		double volatilidade = UNSETED;
		double sharpe = UNSETED;

		void reset() {
			retorno_acumulado = UNSETED;
			volatilidade = UNSETED;
			sharpe = UNSETED;
		}
};

struct Simulation_data {
		size_t* num_acoes_periodo_array = nullptr;
		double* capital_periodo_array = nullptr;
		double* retorno_periodo_array = nullptr;
};

struct Security_data {
		float* close_prices_vet = nullptr;
		size_t tamanho_close_prices_vet = 0;
		string ticker;
};

struct Thread_data {
		int id;
		void* source = nullptr;
		size_t tam_thread_data = 0;
		void* dataArray = nullptr;
};

class Domain {
	public:
		Domain() = delete;
		Domain(const Domain&) = delete;
		Domain(Domain&&) = delete;
		~Domain();

		static void load_domain_configuration();
};

#endif /* DOMAIN_H_ */

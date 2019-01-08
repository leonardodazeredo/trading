/*
 * Trading.h
 *
 *  Created on: 2 de jan de 2017
 *      Author: leo
 */

#ifndef TRADING_H_
#define TRADING_H_

#include "BollingerBands.h"
#include "MovingAverage.h"
#include "RelativeStrengthIndex.h"

#include "Domain.h"
#include "TraderIndiceSimulator.h"
#include "SecurityDataHandler.h"

using namespace std;

class Trading {
	public:
		vector<Security_data> security_datas;

		unordered_map<string, vector<pair<string, short>>> decisoes_dos_indices;

		Trading(vector<Security_data> sds);
		virtual ~Trading();

		void run_indice_simulations();

		void set_and_run(Security_data sd, vector<Indice*> combinacoes_de_indice);
	};

#endif /* TRADING_H_ */

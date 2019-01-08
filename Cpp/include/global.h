/*
 * global.h
 *
 *  Created on: 23 de jan de 2017
 *      Author: leo
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include <set>

using namespace std;

class global {
	public:
		global() = delete;
		~global();
};

extern set<string> indices_to_use;

extern bool general_debug_mode;
extern size_t debug_mode_result_list_size;
extern bool general_verbose_mode;

extern float capital_ini;
extern size_t num_acoes_ini;

extern size_t num_of_cores;

extern string dados_path;
extern string tickers[10];
extern size_t num_ativos;

extern bool indice_simulation_parallel_mode;

extern bool security_handling_parallel_mode;

extern string indice_simulation_metodo_selecao;

extern float volatilidade_aceitavel;

extern size_t periodos_de_simulacao;


#endif /* GLOBAL_H_ */

/*
 * global.cpp
 *
 *  Created on: 23 de jan de 2017
 *      Author: leo
 */

#include "global.h"

global::~global() {
}

set<string> indices_to_use;

bool general_debug_mode;
size_t debug_mode_result_list_size;
bool general_verbose_mode;

float capital_ini;
size_t num_acoes_ini;

size_t num_of_cores;

string dados_path;
string tickers[10];
size_t num_ativos;

bool indice_simulation_parallel_mode;

bool security_handling_parallel_mode;

string indice_simulation_metodo_selecao;

float volatilidade_aceitavel = 100;

size_t periodos_de_simulacao;

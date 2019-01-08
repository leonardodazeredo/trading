/*
 * SecurityDataHandler.cpp
 *
 *  Created on: 16 de jan de 2017
 *      Author: leo
 */

#include "SecurityDataHandler.h"

SecurityDataHandler::SecurityDataHandler() {
}

SecurityDataHandler::~SecurityDataHandler() {
	for (Security_data sd : security_datas) {
		delete[] sd.close_prices_vet;
	}
}

vector<Security_data> SecurityDataHandler::loadData() {
	FILE * pFile;

	float* close_prices_vet;
	int close_prices_vet_tamanho;

	for (size_t t = 0; t < num_ativos; t++) {
		char buffer[100];
		sprintf(buffer, "%s/tabela_%s_bollinger.bin", dados_path.c_str(), tickers[t].c_str());

		pFile = fopen(buffer, "rb");

		if (pFile == nullptr) {
			char buffer[100];
			sprintf(buffer, "\nFalha ao abrir o arquivo de dados para o ticker %s.\n Saida de erro: ", tickers[t].c_str());
			cout << buffer << stderr;
			exit(EXIT_FAILURE);
		}
		else {
			fread(&close_prices_vet_tamanho, sizeof(int), 1, pFile);
			close_prices_vet = new float[close_prices_vet_tamanho];
			fread(close_prices_vet, sizeof(float), close_prices_vet_tamanho, pFile);
			fclose(pFile);

			Security_data security_data;

			security_data.close_prices_vet = close_prices_vet;
			security_data.tamanho_close_prices_vet = close_prices_vet_tamanho;
			security_data.ticker = tickers[t];

			security_datas.push_back(security_data);
		}
	}

	return security_datas;
}

/*
 * Constants.cpp
 *
 *  Created on: 28 de nov de 2016
 *      Author: leo
 */

#include "Domain.h"

Domain::~Domain() {
}
using namespace libconfig;

void Domain::load_domain_configuration() {
	Config cfg;

	// Read the file. If there is an error, report it and exit.
	try {
		cfg.readFile(CONFIG_FILE);
	}
	catch (const FileIOException &fioex) {
		std::cerr << "Erro de I/O ao ler arquivo de confguracao." << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const ParseException &pex) {
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
		exit(EXIT_FAILURE);
	}

	const Setting& root = cfg.getRoot();

	try {
//		----------------------------------------------------------------------------------------------------------
		const Setting &general = root["general"];

		general_debug_mode = general.lookup("debug-mode");

		debug_mode_result_list_size = (unsigned int) general.lookup("debug-mode-result-list-size");

		general_verbose_mode = general.lookup("verbose-mode");

		capital_ini = (float) general.lookup("starting-capital");

		num_acoes_ini = (unsigned int) general.lookup("starting-action-number");
//		----------------------------------------------------------------------------------------------------------
		const Setting &system = root["system"];

		num_of_cores = (unsigned int) system.lookup("core-number");
//		----------------------------------------------------------------------------------------------------------
		const Setting &security = root["security"];

		const Setting &tickers_array = security.lookup("tickers");
		for (int i = 0; i < tickers_array.getLength(); i++) {
			tickers[i] = (const char *) tickers_array[i];
		}

		string auxSrt1 = security.lookup("data-directory");
		dados_path = auxSrt1;

		num_ativos = (unsigned int) security.lookup("number");
//		----------------------------------------------------------------------------------------------------------
		const Setting &indice_simulation = root["indice-simulation"];

		indice_simulation_parallel_mode = indice_simulation.lookup("parallel-mode");

		const Setting &indices_array = indice_simulation.lookup("indices-to-use");
		for (int i = 0; i < indices_array.getLength(); i++) {
			indices_to_use.insert((const char *) indices_array[i]);
		}

		cout << "Simulando com os indices: ";
		for (string s : indices_to_use) {
			cout << s << " ";
		}
		cout << endl;

		string auxSrt2 = indice_simulation.lookup("metodo-selecao");
		indice_simulation_metodo_selecao = auxSrt2;

		cout << "Metodo de selecao na simulação: " << indice_simulation_metodo_selecao << endl;

		volatilidade_aceitavel = indice_simulation.lookup("volatilidade-aceitavel");

		cout << "Volatilidade Aceitavel: min + " << volatilidade_aceitavel << "%" << endl;

		periodos_de_simulacao = (int) indice_simulation.lookup("periodos-de-simulacao");

		cout << "Simulando indices com ultimos " << periodos_de_simulacao << " periodos." << endl;
//		----------------------------------------------------------------------------------------------------------
		const Setting &security_handling = root["security-handling"];

		security_handling_parallel_mode = security_handling.lookup("parallel-mode");
//		----------------------------------------------------------------------------------------------------------
	}
	catch (const SettingNotFoundException &nfex) {
		std::cerr << "Faltando dados de configuração." << std::endl;
		exit(EXIT_FAILURE);
	}

//	// Output a list of all books in the inventory.
//	try {
//		const Setting &books = root["general"];
//		int count = books.getLength();
//
//		cout << setw(30) << left << "TITLE" << "  " << setw(30) << left << "AUTHOR" << "   " << setw(6) << left << "PRICE" << "  " << "QTY" << endl;
//
//		for (int i = 0; i < count; ++i) {
//			const Setting &book = books[i];
//
//			// Only output the record if all of the expected fields are present.
//			string title, author;
//			double price;
//			int qty;
//
//			if (!(book.lookupValue("title", title) && book.lookupValue("author", author) && book.lookupValue("price", price) && book.lookupValue("qty", qty)))
//				continue;
//
//			cout << setw(30) << left << title << "  " << setw(30) << left << author << "  " << '$' << setw(6) << right << price << "  " << qty << endl;
//		}
//		cout << endl;
//	}
//	catch (const SettingNotFoundException &nfex) {
//		// Ignore.
//	}
}

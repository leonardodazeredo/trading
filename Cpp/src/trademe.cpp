//============================================================================
// Name        : Trading.cpp
// Author      : Leonardo D
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "Utils.h"
#include "Domain.h"
#include "SecurityDataHandler.h"
#include "Trading.h"

using namespace std;

int main(int argc, char *argv[]) {
	Domain::load_domain_configuration();

	SecurityDataHandler sdh;
	vector<Security_data> security_datas = sdh.loadData();

	Trading tranding(security_datas);
	tranding.run_indice_simulations();

	return EXIT_SUCCESS;
}

/*
 * SecurityDataHandler.h
 *
 *  Created on: 16 de jan de 2017
 *      Author: leo
 */

#ifndef SECURITYDATAHANDLER_H_
#define SECURITYDATAHANDLER_H_

#include "Domain.h"

class SecurityDataHandler {
	public:
		vector<Security_data> security_datas;

		SecurityDataHandler();
		~SecurityDataHandler();

		vector<Security_data> loadData();
};

#endif /* SECURITYDATAHANDLER_H_ */

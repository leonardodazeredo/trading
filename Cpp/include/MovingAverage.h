/*
 * MovingAverage.h
 *
 *  Created on: 16 de jan de 2017
 *      Author: leo
 */

#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

#define MOVINGAVERAGE_STR "MovingAverage"

#define MA_TAMANHO_MAXIMO 90
#define MA_TAMANHO_INCREMENTO 1
#define MA_TAMANHO_INICIAL 2

#define MA_APROX_COMBINACOES ((MA_TAMANHO_MAXIMO - MA_TAMANHO_INICIAL) / MA_TAMANHO_INCREMENTO)

#include "Domain.h"
#include "Indice.h"
#include "Utils.h"

class MovingAverage: public Indice {
	public:
		size_t w = UNSETED;

		double* media_movel = nullptr;

		MovingAverage(const Security_data sd, int w);
		virtual ~MovingAverage();

		short decisao() override;
		short decisao(size_t dia_de_referencia) override;
		short* pontos_de_decisao() override;
		int periodoInicial() override;
		static vector<Indice*> instancia_combinacoes(Security_data sd);

		void imprime_resultado() override;

		int getW() const {
			return w;
		}

		void setW(int w) {
			this->w = w;
		}
};

#endif /* MOVINGAVERAGE_H_ */

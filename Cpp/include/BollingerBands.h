/*
 * BollingerBands.h
 *
 *  Created on: 27 Oct 2016
 *      Author: leo
 */

#ifndef BOLLINGERBANDS_H_
#define BOLLINGERBANDS_H_

#define BOLLINGERBANDS_STR "BollingerBands"

#define BB_TAMANHO_MAXIMO 60
#define BB_DESVIOS_MAXIMO 4.0f

#define BB_TAMANHO_INCREMENTO 1
#define BB_DESVIOS_INCREMENTO 0.1f

#define BB_TAMANHO_INICIAL 2
#define BB_DESVIOS_INICIAL 0.1f

#define BB_APROX_COMBINACOES (((BB_TAMANHO_MAXIMO - BB_TAMANHO_INICIAL) / BB_TAMANHO_INCREMENTO) * ((BB_DESVIOS_MAXIMO - BB_DESVIOS_INICIAL) / BB_DESVIOS_INCREMENTO))

#include "Indice.h"

class BollingerBands: public Indice {
	public:
		size_t w = UNSETED;
		float d = UNSETED;

		double* media_movel = nullptr;
		double* upper_band = nullptr;
		double* lower_band = nullptr;

		BollingerBands(const Security_data sd, int w, float d);

		virtual ~BollingerBands();

		short decisao() override;
		short decisao(size_t dia_de_referencia) override;
		short* pontos_de_decisao() override;
		int periodoInicial() override;
		static vector<Indice*> instancia_combinacoes(Security_data sd);

		void imprime_resultado() override;

		float getD() const {
			return d;
		}

		void setD(float d) {
			this->d = d;
		}

		int getW() const {
			return w;
		}

		void setW(int w) {
			this->w = w;
		}
};

#endif /* BOLLINGERBANDS_H_ */

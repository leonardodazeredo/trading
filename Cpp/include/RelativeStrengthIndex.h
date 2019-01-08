/*
 * RelativeStrengthIndex.h
 *
 *  Created on: 23 de jan de 2017
 *      Author: leo
 */

#ifndef RELATIVESTRENGTHINDEX_H_
#define RELATIVESTRENGTHINDEX_H_

#include "Domain.h"
#include "Indice.h"
#include "Utils.h"

#define RELATIVESTRENGTHINDEX_STR "RelativeStrengthIndex"

#define RSI_TAMANHO_MAXIMO 90
#define RSI_TAMANHO_INCREMENTO 1
#define RSI_TAMANHO_INICIAL 2

#define RSI_LIMIAR_SUPERIOR_MAXIMO 90
#define RSI_LIMIAR_SUPERIOR_INCREMENTO 5
#define RSI_LIMIAR_SUPERIOR_INICIAL 60

#define RSI_LIMIAR_INFERIOR_MAXIMO 50
#define RSI_LIMIAR_INFERIOR_INCREMENTO 5
#define RSI_LIMIAR_INFERIOR_INICIAL 10

#define RSI_APROX_COMBINACOES \
(((RSI_TAMANHO_MAXIMO - RSI_TAMANHO_INICIAL) / RSI_TAMANHO_INCREMENTO) * \
((RSI_LIMIAR_SUPERIOR_MAXIMO - RSI_LIMIAR_SUPERIOR_INICIAL) / RSI_LIMIAR_SUPERIOR_INCREMENTO) * \
((RSI_LIMIAR_INFERIOR_MAXIMO - RSI_LIMIAR_INFERIOR_INICIAL) / RSI_LIMIAR_INFERIOR_INCREMENTO))

using namespace std;

class RelativeStrengthIndex: public Indice {
	public:
		size_t w = UNSETED;

		double* rsi_array = nullptr;

		int limiar_superior;

		int limiar_inferior;

		RelativeStrengthIndex(const Security_data sd, int w, int limiar_superior, int limiar_inferior);
		virtual ~RelativeStrengthIndex();

		short decisao() override;
		short decisao(size_t dia_de_referencia) override;
		short* pontos_de_decisao() override;
		int periodoInicial() override;
		static vector<Indice*> instancia_combinacoes(Security_data sd);

		void imprime_resultado() override;

		double rsi(double averageCloseUP, double averageCloseDOWN);

		tuple<double, double> calcula_up_and_down() {
			double up;
			double down;

			double difference = security_data.close_prices_vet[periodo_atual] - security_data.close_prices_vet[periodo_anterior];

			if (difference > 0) {
				up = difference;
				down = 0;
			}
			else if (difference < 0) {
				up = 0;
				down = difference * (-1);
			}
			else {
				up = down = 0;
			}

			return make_tuple(up, down);
		}

		int getW() const {
			return w;
		}

		void setW(int w) {
			this->w = w;
		}
};

#endif /* RELATIVESTRENGTHINDEX_H_ */

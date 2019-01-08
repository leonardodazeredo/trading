/*
 * Indice.h
 *
 *  Created on: 3 de dez de 2016
 *      Author: leo
 */

#ifndef INDICE_H_
#define INDICE_H_

#include "Domain.h"

class Indice {
	public:
		Security_data security_data;
		short* pontos_decisao = nullptr;
		int dia_de_referencia = UNSETED;

		size_t periodo_atual = 1;
		size_t periodo_anterior = 0;

		virtual ~Indice();

		const static int combinacoes;

		virtual short decisao() = 0;
		virtual short decisao(size_t dia_de_referencia) = 0;
		virtual short* pontos_de_decisao() = 0;
		virtual int periodoInicial() = 0;

		virtual void imprime_resultado() = 0;

		void incrementa_periodo() {
			periodo_atual++;
			periodo_anterior = periodo_atual - 1;
		}

		void setPeriodoAtual(size_t periodoAtual = 1) {
			periodo_atual = periodoAtual;
			periodo_anterior = periodo_atual - 1;
		}

		int getDiaDeReferencia() const {
			return dia_de_referencia;
		}

		void setDiaDeReferencia(int diaDeReferencia) {
			dia_de_referencia = diaDeReferencia;
		}

		short* getPontosDecisao() const {
			return pontos_decisao;
		}

		void setPontosDecisao(short* pontosDecisao) {
			pontos_decisao = pontosDecisao;
		}
};

#endif /* INDICE_H_ */

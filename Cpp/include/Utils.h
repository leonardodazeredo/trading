/*
 * Utils.h
 *
 *  Created on: 5 de dez de 2016
 *      Author: leo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Domain.h"

class Utils {
	public:
		Utils() = delete;
		Utils(const Utils&) = delete;
		Utils(Utils&&) = delete;

		template<class T1, class T2, class T3>
		static void printVet(const T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3, class A>
		static void printVet(const T1* vet, T2 ini, T3 fim, A access);

		template<class T1, class T2, class T3, class T4>
		static double calcula_desvio(const T1* vet, T2 ini, T3 fim, T4 media);

		template<class T1, class T2, class T3>
		static double calcula_media(const T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3, class T4>
		static double calcula_desvio(double (*f)(T1), const T1* vet, T2 ini, T3 fim, T4 media);

		template<class T1, class T2, class T3>
		static double calcula_media(double (*f)(T1), const T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3>
		static double stale_min_max(T1 num, T2 min, T3 max);

		template<class T1>
		static double stale(T1 num);

		template<class T1, class T2, class T3>
		static double produtorio(const T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3>
		static double volatilidade_anualizada(const T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3>
		static void ones(T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3>
		static void zeros(T1* vet, T2 ini, T3 fim);

		template<class T1, class T2, class T3, class T4>
		static void set_value(T1 value, T2* vet, T3 ini, T4 fim);

		template<class T1, class T2>
		static double media_harmonica(T1 num1, T2 num2);

		template<class T1, class T2>
		static void sort_object_array(T1* object_array, T2 number_of_objects, int (*compar)(const void*, const void*));

		template<typename C, typename P>
		static C filter(C const & container, P pred);
};

template<typename C, typename P>
inline C Utils::filter(const C& container, P pred) {
	C filtered(container.size());

	auto it = copy_if(container.begin(), container.end(), filtered.begin(), pred);
	filtered.resize(std::distance(filtered.begin(), it));

	return filtered;
}

template<class T1, class T2, class T3, class A>
inline void Utils::printVet(const T1* vet, T2 ini, T3 fim, A access) {
	for (size_t i = ini; i < (size_t) fim; i++) {
		cout << i << " - " << access(vet[i]) << endl;
	}
}

template<class T1, class T2, class T3>
inline void Utils::printVet(const T1* vet, T2 ini, T3 fim) {
	for (size_t i = ini; i < (size_t) fim; i++) {
		cout << i << " - " << vet[i] << endl;
	}
}

template<class T1, class T2, class T3, class T4>
inline double Utils::calcula_desvio(const T1* vet, T2 ini, T3 fim, T4 media) {
	double sum = 0.0;
	for (size_t i = ini; i < (size_t) fim; i++) {
		sum += pow((vet[i] - media), 2.0);
	}

	sum = sum / (fim - ini);
	sum = sqrt(sum);

	return sum;
}

template<class T1, class T2, class T3>
inline double Utils::calcula_media(const T1* vet, T2 ini, T3 fim) {
	double sum = 0.0;
	for (size_t i = ini; i < (size_t) fim; i++) {
		sum += vet[i];
	}

	return sum / (fim - ini);
}

template<class T1, class T2, class T3, class T4>
inline double Utils::calcula_desvio(double (*f)(T1), const T1* vet, T2 ini, T3 fim, T4 media) {
	double sum = 0.0;
	for (size_t i = ini; i < (size_t) fim; i++) {
		sum += pow(((*f)(vet[i]) - media), 2.0);
	}

	sum = sum / (fim - ini);
	sum = sqrt(sum);

	return sum;
}

template<class T1, class T2, class T3>
inline double Utils::calcula_media(double (*f)(T1), const T1* vet, T2 ini, T3 fim) {
	double sum = 0.0;
	for (size_t i = ini; i < (size_t) fim; i++) {
		sum += (*f)(vet[i]);
	}

	return sum / (fim - ini);
}

template<class T1, class T2, class T3>
inline double Utils::stale_min_max(T1 num, T2 min, T3 max) {
	return 0;
}

template<class T1>
inline double Utils::stale(T1 num) {
	return 0;
}

template<class T1, class T2, class T3>
inline double Utils::produtorio(const T1* vet, T2 ini, T3 fim) {
	double prod = 1.0;
	for (size_t i = ini; i < (size_t) fim; i++) {
		prod *= vet[i];
	}
	return prod;
}

template<class T1, class T2, class T3>
inline double Utils::volatilidade_anualizada(const T1* vet, T2 ini, T3 fim) {
	double media = Utils::calcula_media(log10, vet, ini, fim);
	double vol = Utils::calcula_desvio(log10, vet, ini, fim, media) * sqrt(252);

	return isnan(vol) ? -1 : AS_PERCENTAGE(vol);
}

template<class T1, class T2, class T3>
inline void Utils::ones(T1* vet, T2 ini, T3 fim) {
	for (size_t i = ini; i < (size_t) fim; i++) {
		vet[i] = (T1) 1;
	}
}

template<class T1, class T2, class T3>
inline void Utils::zeros(T1* vet, T2 ini, T3 fim) {
	for (size_t i = ini; i < (size_t) fim; i++) {
		vet[i] = (T1) 0;
	}
}

template<class T1, class T2>
inline double Utils::media_harmonica(T1 num1, T2 num2) {
	return 2 * ((num1 * num2) / (num1 + num2));
}

template<class T1, class T2>
inline void Utils::sort_object_array(T1* object_array, T2 number_of_objects, int (*compar)(const void*, const void*)) {
	qsort((void*) object_array, number_of_objects, sizeof(T1), compar);
}

template<class T1, class T2, class T3, class T4>
inline void Utils::set_value(T1 value, T2* vet, T3 ini, T4 fim) {
	for (size_t i = ini; i < (size_t) fim; i++) {
		vet[i] = (T2) value;
	}
}

//extern clock_t clock_t_inicio = 0, clock_t_fim = 0;
//void init_clock() {
//	srand(time(nullptr));
//	clock_t_inicio = clock();
//}
//
//void end_clock() {
//	clock_t_fim = clock();
//}
//
//void print_clock_result() {
//	printf("Tempo: %f segundos\n", ((float) clock_t_fim - (float) clock_t_inicio) / (CLOCKS_PER_SEC));
//}

#endif /* UTILS_H_ */

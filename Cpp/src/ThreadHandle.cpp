/*
 * ThreadHandle.cpp
 *
 *  Created on: 28 de nov de 2016
 *      Author: leo
 */
#include <pthread.h>

#include "Utils.h"
#include "ThreadHandle.h"
#include "TraderIndiceSimulator.h"

using namespace std;

void ThreadHandle::join_all_threads(pthread_t* threads, size_t number_of_threads) {
	void** status = nullptr;

	for (size_t i = 0; i < (size_t) number_of_threads; i++) {
		int tc = 0;
		tc = pthread_join(threads[i], status);
		if (tc) {
			cout << "Erro ao fazer join das threads. Erro " << tc << endl;
			exit(-1);
		}
	}
}

void ThreadHandle::join_all_threads(pthread_t* threads) {
	void** status = nullptr;

	for (size_t i = 0; i < (size_t) num_of_cores; i++) {
		int tc = 0;
		tc = pthread_join(threads[i], status);
		if (tc) {
			cout << "Erro ao fazer join das threads. Erro " << tc << endl;
			exit(-1);
		}
	}

	delete[] threads;
}

ThreadHandle::~ThreadHandle() {
}

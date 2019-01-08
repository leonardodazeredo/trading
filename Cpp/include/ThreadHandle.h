/*
 * ThreadHandle.h
 *
 *  Created on: 28 de nov de 2016
 *      Author: leo
 */

#include "Domain.h"
#include "Utils.h"

#ifndef THREADHANDLE_H_
#define THREADHANDLE_H_

class ThreadHandle {
	public:
		ThreadHandle() = delete;
		ThreadHandle(const ThreadHandle&) = delete;
		ThreadHandle(ThreadHandle&&) = delete;
		virtual ~ThreadHandle();

		template<class T1, class T2>
		static pthread_t* create_threads(void* (*start_routine)(void*), T1* thread_data_array, T2 number_of_threads);

		template<class T1>
		static pthread_t* create_threads(void* (*start_routine)(void*), T1* thread_data_array);

		template<class T1, class T2>
		static void create_threads_and_join_all(T1* data, T2 data_size, void* (*start_routine)(void*));

		template<class T1, class T2>
		static Thread_data* thread_data_array(T1* data, T2 data_size);

		template<class T1, class T2>
		static void populate_thread_data_array(Thread_data* thread_data_array, T1* data, T2 data_size);

		static void join_all_threads(pthread_t* threads, size_t number_of_threads);
		static void join_all_threads(pthread_t* threads);
};

template<class T1, class T2>
inline pthread_t* ThreadHandle::create_threads(void* (*start_routine)(void*), T1* thread_data_array, T2 number_of_threads) {
	pthread_t* threads = new pthread_t[(size_t) number_of_threads];

	for (size_t i = 0; i < (size_t) number_of_threads; i++) {
		int tc = 0;
		tc = pthread_create(&threads[i], nullptr, start_routine, (void*) &thread_data_array[i]);
		if (tc) {
			cout << "Erro ao criar threads. " << i << ". Erro: " << tc << endl;
			exit(-1);
		}
	}

	return threads;
}

template<class T1>
inline pthread_t* ThreadHandle::create_threads(void* (*start_routine)(void*), T1* thread_data_array) {
	pthread_t* threads = new pthread_t[(size_t) num_of_cores];

	for (size_t i = 0; i < (size_t) num_of_cores; i++) {
		int tc = 0;
		tc = pthread_create(&threads[i], nullptr, start_routine, (void*) &thread_data_array[i]);
		if (tc) {
			cout << "Erro ao criar threads. " << i << ". Erro: " << tc << endl;
			exit(-1);
		}
	}

	return threads;
}

template<class T1, class T2>
inline void ThreadHandle::populate_thread_data_array(Thread_data* thread_data_array, T1* data, T2 data_size) {
	size_t tam_thread_data = size_t((size_t) data_size / (size_t) num_of_cores);
	size_t i;
	for (i = 0; i < num_of_cores - 1; i++) {
		thread_data_array[i].id = i;
		thread_data_array[i].tam_thread_data = tam_thread_data;
		thread_data_array[i].dataArray = data + (i * tam_thread_data);
	}

	thread_data_array[num_of_cores - 1].id = i;
	thread_data_array[num_of_cores - 1].tam_thread_data = data_size - ((num_of_cores - 1) * tam_thread_data);
	thread_data_array[num_of_cores - 1].dataArray = data + ((num_of_cores - 1) * tam_thread_data);
}

template<class T1, class T2>
inline Thread_data* ThreadHandle::thread_data_array(T1* data, T2 data_size) {
	Thread_data* thread_data_array = new Thread_data[(size_t) num_of_cores];
	ThreadHandle::populate_thread_data_array(thread_data_array, data, data_size);

	return thread_data_array;
}

template<class T1, class T2>
inline void ThreadHandle::create_threads_and_join_all(T1* data, T2 data_size, void* (*start_routine)(void*)) {
	Thread_data* thread_data_array = ThreadHandle::thread_data_array(data, data_size);

	pthread_t* threads = ThreadHandle::create_threads(start_routine, thread_data_array);

	ThreadHandle::join_all_threads(threads);

	delete[] thread_data_array;
}

#endif /* THREADHANDLE_H_ */

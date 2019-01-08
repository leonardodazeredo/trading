#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <pthread.h>

#define DADOS_PATH "dadosHistoricos/tabela_"

#define NUM_RESULTADO 1
// #define NUM_RESULTADO number_of_threads

#define NUM_ATIVOS 10

#define TAMANHO_PADRAO 10
#define DESVIOS_PADRAO 0.5

#define TAMANHO_MAXIMO 90
#define DESVIOS_MAXIMO 4

#define TAMANHO_INCREMENTO 1
#define DESVIOS_INCREMENTO 0.1

#define TAMANHO_INICIAL 2
#define DESVIOS_INICIAL 0.0

#define CAPITAL_INICIAL 100000.0

#define ACOES_INICIAL 0

using namespace std;

string tickers[] = { "YHOO", "SPY", "AAPL", "MSFT", "XOM", "JNJ", "AMZN", "INTC", "GE", "T" };
// string tickers[] = { "YHOO", "SPY", "AAPL", "MSFT", "XOM","JNJ", "AMZN", "FB", "GE", "T" };
// string tickers[] = { "AAPL", "XOM", "AMZN"};
// string tickers[] = { "AAPL"};
// string tickers[] = { "YHOO"};
// string tickers[] = { "FB"};
// string tickers[] = { "SPY"};

struct thread_data {
        int thread_id;

        float* historico;
        int tamanho;

        int tamanho_janela;
        float num_desvios;

        double capital = CAPITAL_INICIAL;

        int num_acoes = ACOES_INICIAL;

        int* pontos_decisao;

        double* capital_dia_array;
        double* retorno_dia_array;
};

typedef struct thread_data thread_data;

void create_threads(pthread_t* threads, void* (*start_routine)(void*), thread_data* thread_data_array, int number_of_threads) {
    for (int i = 0; i < (int) number_of_threads; i++) {
        int tc = 0;
        tc = pthread_create(&threads[i], NULL, start_routine, (void*) &thread_data_array[i]);
        if (tc) {
            cout << "Unable to create thread " << i << ". Erro: " << tc << endl;
            exit(-1);
        }
    }
}

void join_all_threads(pthread_t* threads, void** status, int number_of_threads) {
    for (int i = 0; i < (int) number_of_threads; i++) {
        int tc = 0;
        tc = pthread_join(threads[i], status);
        if (tc) {
            cout << "Error:unable to join. Erro " << tc << endl;
            exit(-1);
        }
    }
}

void printVet(double* vet, int ini, int fim) {
    for (int i = ini; i < fim; i++) {
        printf("%d - %f\n",i, vet[i]);
    }
}

double calcula_media(float* historico, int ini, int fim) {
    double sum = 0.0;
    for (int i = ini; i < fim; i++) {
        sum += historico[i];
    }

    return sum / (fim - ini);
}

double calcula_desvio(float* historico, int ini, int fim, double media) {
    double sum = 0.0;
    for (int i = ini; i < fim; i++) {
        sum += pow((historico[i] - media), 2.0);
    }

    sum = sum / (fim - ini);
    sum = sqrt(sum);

    return sum;
}

int decisao_bollinger_bands(float* historico, double* upper_band, double* lower_band, int dia_de_referencia) {
    int decisao = 0;

    if (historico[dia_de_referencia] >= upper_band[dia_de_referencia]) {
        decisao = -1; // cout << "<<VENDE\n";
    }
    else
        if (historico[dia_de_referencia] <= lower_band[dia_de_referencia]) {
            decisao = 1; // cout << "<<COMPRA\n";
        }

    return decisao;
}

void bollinger_bands(float* historico, int tamanho, int w, float d, int* pontos_decisao) {
    int dia_corrente = w;
    int dia_anterior = w - 1;

    double* media_movel;
    double* upper_band;
    double* lower_band;
    double desvio;

    media_movel = new double[tamanho + 1];
    upper_band = new double[tamanho + 1];
    lower_band = new double[tamanho + 1];

    // std::cout << "\n" << pontos_decisao << std::endl;

    while (dia_corrente < (tamanho + 1)) {
        media_movel[dia_anterior] = calcula_media(historico, dia_corrente - w, dia_corrente);

        desvio = calcula_desvio(historico, dia_corrente - w, dia_corrente, media_movel[dia_anterior]);

        upper_band[dia_anterior] = media_movel[dia_anterior] + (d) * desvio;
        lower_band[dia_anterior] = media_movel[dia_anterior] - (d) * desvio;

        pontos_decisao[dia_corrente] = decisao_bollinger_bands(historico, upper_band, lower_band, dia_anterior);

        dia_corrente++;
        dia_anterior++;
    }

    delete[] media_movel;
    delete[] upper_band;
    delete[] lower_band;
}

void printVet(int* vet, int ini, int fim) {
	for (int i = ini; i < fim; i++) {
		printf("%d - %d\n",i, vet[i]);
	}
}

void* trader_threader(void *threadarg) {
    thread_data* td = (thread_data*) threadarg;

    int dia_corrente = td->tamanho_janela;
    int dia_anterior = td->tamanho_janela - 1;

    td->capital_dia_array = new double[td->tamanho];
    td->retorno_dia_array = new double[td->tamanho];

    td->capital_dia_array[dia_corrente] = td->capital;
    td->capital_dia_array[dia_anterior] = td->capital;

    td->pontos_decisao = new int[td->tamanho + 1];

    bollinger_bands(td->historico, td->tamanho, td->tamanho_janela, td->num_desvios, td->pontos_decisao);

    // cout << "iniciado print\n";
    //
    // printVet(td->pontos_decisao, dia_corrente, td->tamanho);
    //
    // cout << "fim do print\n";

    // pthread_exit(NULL);

    while (dia_corrente < (td->tamanho + 1)) {
        td->capital_dia_array[dia_corrente] = td->capital_dia_array[dia_anterior];

        int num_acoes_compra = (int) (td->capital_dia_array[dia_corrente] / td->historico[dia_corrente]);

        // if (dia_corrente == 10){
        //   printf("%d - %d - capital_dia_array[dia_corrente] = %f - historico[dia_corrente] = %f", dia_corrente, num_acoes_compra, td->capital_dia_array[dia_corrente], td->historico[dia_corrente]);
		    // }

        if (td->pontos_decisao[dia_corrente] == -1 && td->num_acoes > 0) {
            (td->capital_dia_array)[dia_corrente] += (td->historico[dia_corrente] * td->num_acoes); // cout << ">>VENDE\n";
            td->num_acoes = 0;
        }
        else if (td->pontos_decisao[dia_corrente] == 1 && num_acoes_compra > 0) {
            td->num_acoes += num_acoes_compra;
            td->capital_dia_array[dia_corrente] -= (td->historico[dia_corrente] * num_acoes_compra); // cout << "<<COMPRA\n";
        }

        td->retorno_dia_array[dia_corrente] = td->capital_dia_array[dia_corrente] / td->capital_dia_array[dia_anterior];

        dia_corrente++;
        dia_anterior++;
    }

    td->capital = td->capital_dia_array[dia_anterior];

    // cout << "iniciado print\n";
    //
    // printVet(td->capital_dia_array,  td->tamanho_janela, td->tamanho);
    //
    // cout << "fim do print\n";

    // printVet(td->capital_dia_array, 0, 2602);
    //
    // printf("%d\n", dia_corrente);

    delete[] td->pontos_decisao;

    delete[] td->capital_dia_array;
    delete[] td->retorno_dia_array;

    pthread_exit(NULL);
}

double retorno_acumulado(double* retorno_dia_array, int dia_inicio, int dia_fim) {
    double retorno_acumulado = 1;

    for (int i = dia_inicio; i < dia_fim; i++) {
        retorno_acumulado *= retorno_dia_array[i];
    }

    return retorno_acumulado;
}

int compara_threads_por_capital(const void* t1, const void* t2) {
    if (((thread_data*) t1)->capital < ((thread_data*) t2)->capital)
        return -1;
    if (((thread_data*) t1)->capital == ((thread_data*) t2)->capital)
        return 0;
    if (((thread_data*) t1)->capital > ((thread_data*) t2)->capital)
        return 1;
}

void popula_thread_data_bollinger_bands(thread_data* thread_data_array, int number_of_threads, float* close_prices_vet, int tamanho) {
    int i;

    for (i = 0; i < (int) number_of_threads; i++) {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].historico = close_prices_vet;
        thread_data_array[i].tamanho = tamanho;
    }

    i = 0;
    for (int w = TAMANHO_INICIAL; w <= (int) TAMANHO_MAXIMO; w +=
    TAMANHO_INCREMENTO)
    {
        for (float d = DESVIOS_INICIAL; d <= (int) DESVIOS_MAXIMO; d +=
        DESVIOS_INCREMENTO)
        {
            thread_data_array[i].tamanho_janela = w;
            thread_data_array[i].num_desvios = d;

            i++;
        }
    }

    // thread_data_array[0].thread_id = 0;
    // thread_data_array[0].historico = close_prices_vet;
    // thread_data_array[0].tamanho = tamanho;
    // thread_data_array[0].tamanho_janela = 3;
    // thread_data_array[0].num_desvios = 1.4;
}

void imprime_resultado_bollinger_bands(thread_data* thread_data_array, int number_of_threads, string ticker) {
    // cout << "\n\nInprimindo resultado:\n";

    for(int i = number_of_threads - NUM_RESULTADO; i < (int)number_of_threads; i++ )
    {
        printf("%f", thread_data_array[i].capital);
        cout << ", ";
        cout << thread_data_array[i].tamanho_janela;
        cout << ", ";
        cout << thread_data_array[i].num_desvios;

        cout << " " << ticker;
        cout << endl;
    }

    // cout << "\nUltimo:\n";
    // printf("%f", thread_data_array[number_of_threads - 1].capital);
    // cout << ", ";
    // cout << thread_data_array[number_of_threads - 1].tamanho_janela;
    // cout << ", ";
    // cout << thread_data_array[number_of_threads - 1].num_desvios;
    // cout << " " << ticker;

    cout << endl;
}

//-----------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    clock_t inicio, fim;
    srand(time(NULL));

    void* status = NULL;
    int number_of_threads = 1;

    FILE * pFile;
    int tamanho, i;
    float* close_prices_vet;

    if (argc != 1) {
        return EXIT_FAILURE;
    }
    else {
        inicio = clock();
        for (size_t t = 0; t < NUM_ATIVOS; t++) {
            string str = DADOS_PATH + tickers[t] + "_bollinger.bin";
            const char * filePath = str.c_str();

            pFile = fopen(filePath, "rb");

            if (pFile == NULL) {
                cout << "Could not open file.\n" << "File error", stderr;
                return EXIT_FAILURE;
            }
            else {
                fread(&tamanho, sizeof(int), 1, pFile);
                close_prices_vet = new float[tamanho];
                fread(close_prices_vet, sizeof(float), tamanho, pFile);
                fclose(pFile);

                number_of_threads = ((TAMANHO_MAXIMO - TAMANHO_INICIAL) / TAMANHO_INCREMENTO + 1) * ((DESVIOS_MAXIMO - DESVIOS_INICIAL) / DESVIOS_INCREMENTO + 1);

                // number_of_threads = 1;

                thread_data* thread_data_array = new thread_data[number_of_threads];
                popula_thread_data_bollinger_bands(thread_data_array, number_of_threads, close_prices_vet, tamanho);

                pthread_t* threads = new pthread_t[number_of_threads];



                create_threads(threads, trader_threader, thread_data_array, number_of_threads);

                join_all_threads(threads, &status, number_of_threads);

                qsort((void*) thread_data_array, number_of_threads, sizeof(thread_data), compara_threads_por_capital);

                imprime_resultado_bollinger_bands(thread_data_array, number_of_threads, tickers[t]);



                // cout << "\nThreads geradas: " << number_of_threads;
                // cout << endl;
                // cout << endl;


                delete[] close_prices_vet;
                delete[] threads;
                delete[] thread_data_array;
            }
        }

          fim = clock();

        cout << "Tempo: " << ((float) fim - (float) inicio) / (CLOCKS_PER_SEC) << " segundos";
        cout << endl;
        cout << endl;

        pthread_exit(NULL);
    }
}

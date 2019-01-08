//nvcc vetor_runtime.cu -arch=sm_20  -o vetor
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cuda_runtime.h>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <ctime>

#define DADOS_PATH "../DadosHistoricos/tabela_YHOO_bollinger.bin"

#define TAMANHO_PADRAO 10
#define DESVIOS_PADRAO 0.5

#define TAMANHO_MAXIMO 90
#define DESVIOS_MAXIMO 4

#define TAMANHO_INCREMENTO 1
#define DESVIOS_INCREMENTO 0.1

#define TAMANHO_INICIAL 2
#define DESVIOS_INICIAL 0.0

#define CAPITAL_INICIAL 0.0

#define CHECK_ERROR(call) do {                                                    \
   if( cudaSuccess != call) {                                                             \
      std::cerr << std::endl << "CUDA ERRO: " <<                             \
         cudaGetErrorString(call) <<  " in file: " << __FILE__                \
         << " in line: " << __LINE__ << std::endl;                               \
         exit(0);                                                                                 \
   } } while (0)


using namespace std;

__device__
float calcula_media(float* historico, int ini, int fim)
{
  int i;
  float sum = 0.0;
  for(i=ini; i<fim; i++)
  {
    sum+= historico[i];
  }

  return sum/(fim-ini);
}

__device__
float calcula_desvio(float* historico, int ini, int fim, float media)
{
  int i;
  float sum = 0.0;
  for(i=ini; i<fim; i++)
  {
    // sum+= pow((historico[i]-media),2.0);
     sum+= ((historico[i]-media)*(historico[i]-media));
  }

  sum = sum/(fim-ini);
  sum = sqrt(sum);

  return sum;
}

__global__  void 
trader_thread(float* historico, int* tamanhos_janelas, float* num_desvios, float* capital)
{
   int i = blockDim.x * blockIdx.x + threadIdx.x; //???????????????????

   int tamanho = 0;

   int dia_corrente = tamanhos_janelas[i];
   int dia_anterior = tamanhos_janelas[i] - 1;
   float  media_movel;
   float desvio;

   while(dia_corrente<(tamanho + 1))
   {
      media_movel = calcula_media(historico, dia_corrente - tamanhos_janelas[i], dia_corrente);

      desvio = calcula_desvio(historico, dia_corrente - tamanhos_janelas[i], dia_corrente, media_movel);

      if(historico[dia_anterior] >= media_movel + (num_desvios[i])*desvio)
      {
         (capital[i])+= (historico[dia_corrente]);
      }
      else if(historico[dia_anterior] <= media_movel - (num_desvios[i])*desvio)
      {
         (capital[i])-= (historico[dia_corrente]);
      }

      dia_corrente++;
      dia_anterior++;
  }
}

int main (int argc, char **argv)
{
   FILE * pFile;

   int h_tamanho;

   int* h_tamanhos_janelas = NULL;
   int* d_tamanhos_janelas = NULL;

   float* h_close_prices_vet = NULL;
   float* h_num_desvios = NULL;
   float* h_capital = NULL;
   float* d_close_prices_vet = NULL;
   float* d_num_desvios = NULL;
   float* d_capital = NULL;

   int number_of_threads;

   if (argc != 1)
   {
      return EXIT_FAILURE;
   }
   else
   {
      pFile = fopen(DADOS_PATH , "rb" );

      if (pFile==NULL)
      {
         cout<<"Could not open file.\n" << "File error",stderr;
         return EXIT_FAILURE;
      }
      else
      {
         fread(&h_tamanho,sizeof(int),1,pFile);
         h_close_prices_vet = (float*) malloc(sizeof(float)*h_tamanho);

         fread(h_close_prices_vet,sizeof(float), h_tamanho,pFile);
         fclose (pFile);

         number_of_threads = ((TAMANHO_MAXIMO - TAMANHO_INICIAL)/TAMANHO_INCREMENTO + 1) * ((DESVIOS_MAXIMO - DESVIOS_INICIAL)/DESVIOS_INCREMENTO + 1);

         h_tamanhos_janelas = (int*) malloc(sizeof(int)*number_of_threads);
         h_num_desvios = (float*) malloc(sizeof(float)*number_of_threads);
         h_capital = (float*) malloc(sizeof(float)*number_of_threads);

         int i = 0;
         for (int w = TAMANHO_INICIAL; w <= (int) TAMANHO_MAXIMO ; w+=TAMANHO_INCREMENTO)
         {
           for (float d = DESVIOS_INICIAL; d <= (int) DESVIOS_MAXIMO; d+=DESVIOS_INCREMENTO)
           {
             h_tamanhos_janelas[i] = w;
             h_num_desvios[i] = d;
             h_capital[i] = CAPITAL_INICIAL;

             i++;
           }
         }

         float elapsedTimeGPU = 0.0f, elapsedTimeMEM = 0.0f;
         
         cudaEvent_t e_Start,e_Stop;

         size_t free = 0, total = 0;
         cout << endl << "CUDA runtime versao: " << CUDART_VERSION << endl;
         
         //Reset no device
         CHECK_ERROR(cudaDeviceReset());

         //Verificando espaço livre em memória
         CHECK_ERROR(cudaMemGetInfo(&free, &total));
         cout << "Memoria livre: " << (free / 1024 / 1024)   << " MB\n";
         cout << "Memoria total: " << (total / 1024 / 1024)  << " MB\n";
         
         //Criando eventos
         CHECK_ERROR(cudaEventCreate(&e_Start));
         CHECK_ERROR(cudaEventCreate(&e_Stop));
         
         //Aloca memória GPU
         CHECK_ERROR(cudaMalloc((void**) &d_close_prices_vet, h_tamanho * sizeof(float)));
         CHECK_ERROR(cudaMalloc((void**) &d_tamanhos_janelas, number_of_threads * sizeof(int)));
         CHECK_ERROR(cudaMalloc((void**) &d_num_desvios,      number_of_threads * sizeof(float)));
         CHECK_ERROR(cudaMalloc((void**) &d_capital,          number_of_threads * sizeof(float)));
         
         //Copiando CPU --> GPU

         CHECK_ERROR(cudaEventRecord(e_Start, cudaEventDefault));
         
         CHECK_ERROR(cudaMemcpy(d_close_prices_vet, h_close_prices_vet,          h_tamanho * sizeof(float),  cudaMemcpyHostToDevice)); 
         CHECK_ERROR(cudaMemcpy(d_tamanhos_janelas, h_tamanhos_janelas,  number_of_threads * sizeof(float),  cudaMemcpyHostToDevice)); 
         CHECK_ERROR(cudaMemcpy(d_num_desvios,      h_num_desvios,       number_of_threads * sizeof(float),  cudaMemcpyHostToDevice)); 
         CHECK_ERROR(cudaMemcpy(d_capital,          h_capital,           number_of_threads * sizeof(float),  cudaMemcpyHostToDevice)); 
         
         CHECK_ERROR(cudaEventRecord(e_Stop, cudaEventDefault));
         CHECK_ERROR(cudaEventSynchronize(e_Stop));
         CHECK_ERROR(cudaEventElapsedTime(&elapsedTimeMEM, e_Start, e_Stop));
         CHECK_ERROR(cudaEventRecord(e_Start, cudaEventDefault));
         
         int numBlocks = 2;                                       //????????????????????????
         int threadsPerBlock = number_of_threads / numBlocks;     //????????????????????????
         
         
         trader_thread<<<numBlocks, threadsPerBlock>>> (d_close_prices_vet, d_tamanhos_janelas, d_num_desvios,d_capital);
         
         CHECK_ERROR(cudaDeviceSynchronize());
         
         CHECK_ERROR(cudaEventRecord(e_Stop, cudaEventDefault));
         CHECK_ERROR(cudaEventSynchronize(e_Stop));
         CHECK_ERROR(cudaEventElapsedTime(&elapsedTimeGPU, e_Start, e_Stop));

         
         //Copiando GPU --> CPU
         float elapsedTime = 0.0f;
         CHECK_ERROR(cudaEventRecord(e_Start, cudaEventDefault));
         
         CHECK_ERROR(cudaMemcpy(h_capital, d_capital, number_of_threads * sizeof(float),  cudaMemcpyDeviceToHost));
         
         CHECK_ERROR(cudaEventRecord(e_Stop, cudaEventDefault));
         CHECK_ERROR(cudaEventSynchronize(e_Stop));
         CHECK_ERROR(cudaEventElapsedTime(&elapsedTime, e_Start, e_Stop));
         elapsedTimeMEM += elapsedTime;
        
         cout << endl << "Tempo gasto [MEM]: " << elapsedTimeMEM << " (ms)" << endl;
         cout << endl << "Tempo gasto [GPU]: " << elapsedTimeGPU << " (ms)" << endl;
         
         cout <<  endl << "Resultado: "<<  endl;


         CHECK_ERROR(cudaFree(d_close_prices_vet));
         CHECK_ERROR(cudaFree(d_tamanhos_janelas));
         CHECK_ERROR(cudaFree(d_num_desvios));
         CHECK_ERROR(cudaFree(d_capital));
         
         delete [] h_close_prices_vet;
         delete [] h_tamanhos_janelas;
         delete [] h_num_desvios;
         delete [] h_capital;

         // free(h_close_prices_vet);
         // free(h_tamanhos_janelas);
         // free(h_num_desvios);
         // free(h_capital);
         
         cout << "FIM" << endl;
         
         return EXIT_SUCCESS;
      }
   }
}

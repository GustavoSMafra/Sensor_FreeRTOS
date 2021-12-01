#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include <sys/time.h>

//  Estados possíveis do sensor
#define ESTAVEL 1
#define INSTAVEL 0

// Sensores utilizados no sistema
int sensor_duto_gas = ESTAVEL;
int sensor_duto_oleo = ESTAVEL;
int sensor_poco = ESTAVEL;


// Sensor no duto de gás
void thread_duto_gas(void *pvParameter){
    int aleatoridade = 0;
    int tempo = 0;

    struct timeval tv_now;

    while(1){
        gettimeofday(&tv_now, NULL);
        uint64_t time_start = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        aleatoridade = (rand()%100); // Situação da leitura
        if(aleatoridade == 1){ // Instabilidade
            sensor_duto_gas = INSTAVEL;
            tempo = (rand()%100)+50;
            vTaskDelay(tempo / portTICK_RATE_MS); // Tempo do reparo do problema
            gettimeofday(&tv_now, NULL);
            uint64_t time_end = ((int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec) - time_start;
            printf("\n\nSensor do duto de gas instável");
            printf("\nAplicação de contra medida");
            printf("\nContra medida concluida (Gás) em %lld ms",  time_end/1000); // Conclusão + Tempo gasto
        }else{
            sensor_duto_gas = ESTAVEL;
        }
        vTaskDelay(21 / portTICK_RATE_MS); 
        // Delay para um sensor localizado a 400 metros da base, sendo que demoram 5ms pra propagar por 100 metros
    }

}

// Sensor no duto de oleo
void thread_duto_oleo(void *pvParameter){
    int aleatoridade = 0;
    int tempo = 0;
    
    struct timeval tv_now;

    while(1){
        gettimeofday(&tv_now, NULL);
        uint64_t time_start = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        aleatoridade = (rand()%100); // Situação da leitura
        if(aleatoridade == 1){ // Instabilidade
            sensor_duto_oleo = INSTAVEL;
            tempo = (rand()%100)+50;
            vTaskDelay(tempo / portTICK_RATE_MS);  // Tempo do reparo do problema
            gettimeofday(&tv_now, NULL);
            uint64_t time_end = ((int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec) - time_start;
            printf("\n\nSensor do duto de oleo instável");
            printf("\nAplicação de contra medida");
            printf("\nContra medida concluida (Óleo) em %lld ms",  time_end/1000); // Conclusão + Tempo gasto
        }else{
            sensor_duto_oleo = ESTAVEL;
        }
        vTaskDelay(21 / portTICK_RATE_MS); 
        // Delay para um sensor localizado a 400 metros da base, sendo que demoram 5ms pra propagar por 100 metros
    }

}

// Sensor no poço
void thread_poco(void *pvParameter){
    int aleatoridade = 0;
    int tempo = 0;
    
    struct timeval tv_now;
    
    while(1){
        gettimeofday(&tv_now, NULL);
        uint64_t time_start = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
        aleatoridade = (rand()%100); // Leitura do sensor
        if(aleatoridade == 1){ // Instabildiade
            sensor_poco = INSTAVEL;
            tempo = (rand()%100)+100;
            vTaskDelay(tempo / portTICK_RATE_MS); // Tempo gasto para o reinicamento do sistema
            gettimeofday(&tv_now, NULL); 
            uint64_t time_end = ((int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec) - time_start;
            printf("\n\nSensor do poço instável");
            printf("\nAplicação de contra medida de desligamento");
            printf("\nContra medida concluida (Poço) em %lld ms",  time_end/1000); // Conclusão + Tempo gasto
        }else{
            sensor_poco = ESTAVEL;
        }
        vTaskDelay(11 / portTICK_RATE_MS); // Tempo para releitura do sensor do poço, fixada em 10 ms (Tempo não descrito no trabalho)
    }
}

void app_main(){

    nvs_flash_init();
    printf("Start ->");
    // Criação das tarefas para cada sensor
    xTaskCreatePinnedToCore(&thread_duto_gas, "Sensor_gas", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&thread_duto_oleo, "Sensor_oleo", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(&thread_poco, "Sensor_poco", 2048, NULL, 3, NULL, 1);

}
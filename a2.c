#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define M 37
#define N 5
#define P 6

typedef struct {
    int id;
    sem_t *startSem;
    sem_t *stopSem;
} thread_struct;

void *thread_function_1(void *param){
    thread_struct *s = (thread_struct*)param;

    if (s->id == 2)
    {
        sem_wait(s->startSem);
    }

    if (s->id == 3)
    {
        sem_t *Sem = sem_open("sem1", O_CREAT, 0644, 0);
        sem_wait(Sem);
    }

    info(BEGIN, 3, s->id);

    if (s->id == 1)
    {
        sem_post(s->startSem);
    }

    if (s->id == 1)
    {
        sem_wait(s->stopSem);
    }

    info(END, 3, s->id);

    if (s->id == 3)
    {
        sem_t *Sem = sem_open("sem2", O_CREAT, 0644, 0);
        sem_post(Sem);
    }

    if (s->id == 2)
    {
        sem_post(s->stopSem);
    }

    return NULL;
}

void *thread_function_2(void *param){
    thread_struct *s = (thread_struct*)param;

    sem_wait(s->startSem);

    info(BEGIN, 5, s->id);

    //if(s->id < 12 && s->id > 6)
      // sem_wait(s->startSem);


    info(END, 5, s->id);
    sem_post(s->startSem);

    return NULL;
}
void *thread_function_3(void *param){
    thread_struct *s = (thread_struct*)param;

    if (s->id == 1)
    {
        sem_t *Sem = sem_open("sem2", O_CREAT, 0644, 0);
        sem_wait(Sem);
    }

    info(BEGIN, 7, s->id);


    info(END, 7, s->id);

    if (s->id == 5)
    {
        sem_t *Sem = sem_open("sem1", O_CREAT, 0644, 0);
        sem_post(Sem);
    }

    return NULL;
}

int main(){
    init();

    info(BEGIN, 1, 0);
        pid_t pid2 = fork();
        if(pid2 == 0)
        {
            info(BEGIN, 2, 0);

                pid_t pid4 = fork();
                if(pid4 == 0)
                {
                    info(BEGIN, 4, 0);

                    pid_t pid5 = fork();
                    if(pid5 == 0)
                    {
                        info(BEGIN, 5, 0);

                        sem_t startSem;
                        sem_init(&startSem, 0, 6);
                        int i;
                        thread_struct *params = (thread_struct*)malloc(sizeof(thread_struct)*M);
                        pthread_t *process5_threads = (pthread_t*)malloc(sizeof(pthread_t)*M);

                        for(i = 0; i < M; i++){
                            params[i].id = i+1;
                            params[i].startSem = &startSem;
                            pthread_create(&process5_threads[i], NULL, thread_function_2, &params[i]);
                        }
                        for(i = 0; i < M; i++){
                            pthread_join(process5_threads[i], NULL);
                        }

                        info(END, 5, 0);
                        exit(5);
                    }
                    pid_t pid8 = fork();
                    if(pid8 == 0)
                    {
                        info(BEGIN, 8, 0);
                        info(END, 8, 0);
                        exit(8);
                    }

                    waitpid(pid5, NULL, 0);
                    waitpid(pid8, NULL, 0);
                    info(END, 4, 0);
                    exit(4);
                }
            waitpid(pid4, NULL, 0);

            info(END, 2, 0);
            exit(2);
        }
        pid_t pid3 = fork();
        if(pid3 == 0)
        {
            info(BEGIN, 3, 0);


            sem_t startSem;
            sem_t stopSem;
            sem_init(&startSem, 0, 0);
            sem_init(&stopSem, 0, 0);
            int i;
            thread_struct *params = (thread_struct*)malloc(sizeof(thread_struct)*N);
            pthread_t *process3_threads = (pthread_t*)malloc(sizeof(pthread_t)*N);

            for(i = 0; i < N; i++){
                params[i].id = i+1;
                params[i].startSem = NULL;
                params[i].stopSem = NULL;
                if (params[i].id == 1 || params[i].id == 2)
                {
                    params[i].startSem = &startSem;
                    params[i].stopSem = &stopSem;
                }

                pthread_create(&process3_threads[i], NULL, thread_function_1, &params[i]);
            }
            for(i = 0; i < N; i++){
                pthread_join(process3_threads[i], NULL);
            }

            pid_t pid6 = fork();
            if(pid6 == 0)
            {
                info(BEGIN, 6, 0);
                info(END, 6, 0);
                exit(6);
            }

            waitpid(pid6, NULL, 0);
            info(END, 3, 0);
            exit(3);
        }
        pid_t pid7 = fork();
        if(pid7 == 0)
        {
            info(BEGIN, 7, 0);


            sem_t startSem;
            sem_init(&startSem, 0, 0);
            int i;
            thread_struct *params = (thread_struct*)malloc(sizeof(thread_struct)*P);
            pthread_t *process7_threads = (pthread_t*)malloc(sizeof(pthread_t)*P);

            for(i = 0; i < P; i++){
                params[i].id = i+1;
                params[i].startSem = NULL;

                pthread_create(&process7_threads[i], NULL, thread_function_3, &params[i]);
            }
            for(i = 0; i < P; i++){
                pthread_join(process7_threads[i], NULL);
            }


            info(END, 7, 0);
            exit(7);
        }

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    waitpid(pid7, NULL, 0);
    info(END, 1, 0);


    return 0;
}

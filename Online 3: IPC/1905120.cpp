
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>


using namespace std;


int n, plus, minus, plusB, minusB;

sem_t lckm;
sem_t lckp;


void * thread_minus(void * arg)
{   
    while(true) {
        sem_wait(&lckm);

        if(plusB > n) 
            pthread_exit(NULL);

        printf("-");

        minus++;

        sem_post(&lckm);

        if(minus == minusB) {
            sem_wait(&lckm);
            minus = 0;
            minusB--;
            sem_post(&lckp);
        }

    }
}

void * thread_plus(void * arg)
{	

    while(true) {

        sem_wait(&lckp);

        printf("+");
        plus++;
        sem_post(&lckp);

        if(plus == plusB) {
            sem_wait(&lckp);
            plus = 0;
            plusB++;
            printf("\n");

            sem_post(&lckm);

            if(plusB > n) 
                pthread_exit(NULL);

        }
    }
 
}


int main() {

    scanf("%d", &n);

    plus = 0;
    minus = 0;
    minusB = 2*n - 1;
    plusB = 1;

    sem_init(&lckm, 0, 1);
    sem_init(&lckp, 0, 0);


    pthread_t thread1;
	pthread_t thread2;

    pthread_create(&thread1,NULL,thread_minus,NULL);
	pthread_create(&thread2,NULL,thread_plus, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);


    sem_destroy(&lckm);
    sem_destroy(&lckp);
    return 0;
}
    
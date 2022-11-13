#include <pthread.h>
#include <math.h>
#include <semaphore.h>
//counting variables
int burger_req
int Buffer_milk[9] = {0,};
int milk_req;
int milk_index;
int milk_read;

//Cheese variables
int Buffer_Cheese[4] = {0,};
int cheese_req;
int cheese_index;
int cheese_next;


//counting semaphors used.
sem_t milk_e;
sem_t milk_mutex;
sem_t milk_full;
sem_t cheese_e;
sem_t cheese_mutex;
sem_t cheese_full;

int main()
{
    printf("How many burgers do you want?");
    scanf("%d", &burger_req);
    milk_req = burger_req * 6;
    cheese_req = burger_req * 2;
    sem_init(&milk_e, 0,9);
    sem_init(&milk_full, 0,0);
    sem_init(&milk_mutex, 0,1);
    sem_init(&cheese_mutex, 0, 1);
    sem_init(&cheese_full, 0,0);
    sem_init((&cheese_e, 0, 4));
    pthread_t t1, t2, t3, t4, t5, t6;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&t1, &attr, milk_prod, (void*)1);
    pthread_create(&t1, &attr, milk_prod, (void*)2);
    pthread_create(&t1, &attr, milk_prod, (void*)3);

    pthread_create(&t2, &attr, cheese_prod, (void*)4);
    pthread_create(&t2, &attr, cheese_prod, (void*)5);
    
    pthread_create(&t3, &attr, burger_prod, (void*)6);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}

//milk producer
//while there is still milk needed, it will continue creating milk 
void *milk_prod(void *param)
{
    int id = (int)param;
    while(milk_req>0)
    {
        sem_wait(&milk_e);
        sem_wait(&milk_mutex);
        Buffer_milk[milk_index] = id;
        milk_index = milk_index+1;
        if(milk_index == 9)
            milk_index = 0;
        milk_req = milk_req - 1;
        sem_post(&milk_mutex);
        sem_post(&milk_full);
    }
    
}
//Cheese Producer
// takes three milk for one cheese.
//waits until theres milk available using the milk semaphor.
void *cheese_prod(void *param)
{
    int id = (int)param;
    while(cheese_req > 0)
    {
        sem_wait(&cheese_e);
        sem_wait(&cheese_mutex);
        for(int i = 3; i > 0; i--)
        {
            sem_wait(&milk_full);
            Buffer_Cheese[cheese_index] = Buffer_Cheese[cheese_index] + Buffer_milk[milk_read] * pow(10,i);
            sem_post(&milk_e);
            milk_read = milk_read+1;
            if(milk_read ==9)
                milk_read = 0;
        }
        Buffer_Cheese[cheese_index] = Buffer_Cheese[cheese_index] + id;
        cheese_index = cheese_index + 1;
        cheese_req = cheese_req - 1;
        if(cheese_index == 4)
            cheese_index = 0;
        sem_post(&cheese_mutex);
        sem_post(&cheese_full);
    }
}


//takes two cheese to make one burger.
//continues to make burgers while needed.
//waits for the semaphor for the cheese buffer for when cheese is available.
void *burger_prod(void* param)
{
    int id = 0;
    while(burger_req > 0)
    {
        for(int i = 0; i < 2; i--)
        {
            sem_wait(&cheese_full);
            if(i == 1)
            {

            }
            id = Buffer_Cheese[cheese_next];
            sem_post(&cheese_e);
            cheese_next = cheese_next+1;
            if(cheese_index == 4)
                cheese_index=0;
        printf("Cheeseburger %d id: %d\n",i+1, id);
        id = 0;
        burger_req= burger_req-1;
        }
    }
}
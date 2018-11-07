#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h> 
#include<semaphore.h>

int num_students,turn;
sem_t assistent_sem,student_sem,turn_sem;

int random_number(int min,int max){
  return ((rand()%(max-min+1))+min);
}

void* assistent(void *params){
  sem_wait(&assistent_sem);
  printf("the assistent is sleeping\n");
  while(num_students){
    sem_wait(&assistent_sem);
    printf("the assistent is awake\n");
    printf("the assistent is helping a student%d\n",turn);
    int helping_time = random_number(1,3);
    sleep(helping_time);
    sem_post(&turn_sem);    
    printf("the assistent is sleeping\n");
    sem_post(&student_sem);
  }
  pthread_exit(0);
}

void* student(void *params){
  int *id = (int*) params;
  printf("the student%d was created\n",*id);
  printf("the student%d is waiting\n",*id);
  sem_wait(&student_sem);
  printf("the student%d is getting help\n",*id);
  sem_wait(&turn_sem);
  turn = *id;
  sem_post(&assistent_sem);
  printf("the student%d was helped\n",*id);
  num_students--;
  pthread_exit(0);
}

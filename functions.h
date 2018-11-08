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

void program(int id){
  printf("the student(%d) started programing\n",id);
  int programing_time = random_number(1,3);
  sleep(programing_time);
  printf("the student(%d) stoped programing\n",id);
}

void wait_for_student(){
  sem_wait(&student_sem);
}

void wait_for_assistent(int id){
  printf("the student(%d) is waiting\n",id);
  sem_wait(&assistent_sem);
}

void get_help(int id){
  turn = id;
  printf("the student(%d) is getting help\n",turn);
  sem_post(&student_sem);
}

void help_student(){
  printf("the assistent is helping a student(%d)\n",turn);
  int helping_time = random_number(1,3);
  sleep(helping_time);
  printf("the student(%d) was helped\n",turn);
}

void relise_student(){
  printf("the assistent is sleeping\n");
  sem_post(&turn_sem);
  sem_post(&assistent_sem);

}

void* assistent(void *params){
  sem_wait(&student_sem);
  sem_wait(&turn_sem);
  printf("the assistent is sleeping\n");
  int n = num_students *3;
  while(n){
    wait_for_student();
    printf("the assistent is awake\n");
    help_student();
    n--;
    relise_student();
  }
  pthread_exit(0);
}

void* student(void *params){
  int *id = (int*) params;
  printf("the student(%d) was created\n",*id);
  int times_helped;
  for(times_helped =0; times_helped < 3; times_helped++){
    program(*id);
    wait_for_assistent(*id);
    get_help(*id);
    sem_wait(&turn_sem);
  }

  pthread_exit(0);
}

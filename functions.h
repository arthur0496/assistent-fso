#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h> 
#include<semaphore.h>

int num_students,turn,*queue,queue_max_size,queue_size = 0,assistent_state;
sem_t assistent_sem,student_sem,turn_sem[40];
pthread_mutex_t queue_mutex,assistent_state_mutex;

int random_number(int min,int max){
  return ((rand()%(max-min+1))+min);
}

void add_to_queue(int id){
  printf("the student(%d) is sitting in chair number %d\n",id,queue_size);
  queue[queue_size++] = id;

}

void remove_from_queue(){
  turn = queue[0];
  printf("the student(%d) is getting help\n",turn);
  int i;
  for(i = 0; i < queue_size; i++){
    queue[i] = queue[i+1];
    printf("the student(%d) is sitting in chair number %d\n",turn,i);
  }
  queue_size--;

}

void take_a_nap(){
  pthread_mutex_lock(&assistent_state_mutex); 
  assistent_state = 1;
  printf("the assistent is sleeping\n");
  pthread_mutex_unlock(&assistent_state_mutex); 
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

int get_help(int id){
  pthread_mutex_lock(&queue_mutex);
  pthread_mutex_lock(&assistent_state_mutex); 
  if(assistent_state){
    turn = id;
    printf("the student(%d) is getting help\n",turn);
    sem_post(&student_sem);
  }
  else if(queue_size < queue_max_size){
    add_to_queue(id);
  }
  else{
    pthread_mutex_unlock(&assistent_state_mutex); 
    pthread_mutex_unlock(&queue_mutex);
    return 0;
  }
  pthread_mutex_unlock(&assistent_state_mutex); 
  pthread_mutex_unlock(&queue_mutex);
  return 1;
}

void help_student(){
  printf("the assistent is helping a student(%d)\n",turn);
  int helping_time = random_number(1,3);
  sleep(helping_time);
  printf("the student(%d) was helped\n",turn);
}

void relise_student(){
  sem_post(&turn_sem[turn-1]);
  pthread_mutex_lock(&queue_mutex);
  if(!queue_size){  
    take_a_nap();
    sem_post(&assistent_sem);
  }
  pthread_mutex_unlock(&queue_mutex);

}

void* assistent(void *params){
  sem_wait(&student_sem);
  take_a_nap();
  int n = num_students *3;
  while(n){
    if(assistent_state){
      wait_for_student();
      assistent_state = 0;
      printf("the assistent is awake\n");
    }
    else{
      pthread_mutex_lock(&queue_mutex);
      remove_from_queue();
      pthread_mutex_unlock(&queue_mutex);
    }
    help_student();
    relise_student();
    n--;
  }
  pthread_exit(0);
}

void* student(void *params){
  int *id = (int*) params;
  int i = *id;
  sem_wait(&turn_sem[i-1]);
  printf("the student(%d) was created\n",*id);
  int times_helped;
  for(times_helped =0; times_helped < 3; times_helped++){
    program(*id);
    if(get_help(*id)){
      sem_wait(&turn_sem[i-1]);
    }
    else{
      times_helped--;
    }
  }
  printf("the student %d ended his program\n",i); 
  pthread_exit(0);
}

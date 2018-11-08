#include<stdio.h>
#include"functions.h"

int main (){
  srand(time(NULL));  
  num_students = random_number(3,40);
  int num = num_students;

  queue_max_size = num/2;
  queue = (int*)malloc(sizeof(int)*queue_max_size);

  pthread_mutex_init(&queue_mutex,NULL);
  pthread_mutex_init(&assistent_state_mutex,NULL);

  int count;
  for(count = 0; count < num; count++){
    sem_init(&turn_sem[count],0,1);
  }

  sem_init(&assistent_sem,0,1);
  sem_init(&student_sem,0,1);

  pthread_t assistent_id;
  pthread_attr_t assistent_attr;
  pthread_attr_init(&assistent_attr);
  pthread_create(&assistent_id,&assistent_attr,assistent,NULL);
 
  int id[num];
  pthread_t student_id[num];
  pthread_attr_t student_attr[num];
  int i;
  for(i = 0; i < num; i++){
    id[i]=i+1;
    pthread_attr_init(&student_attr[i]);
    pthread_create(&student_id[i],&student_attr[i],student, &id[i]);
  }

  for(i = 0; i < num; i++){
    pthread_join(student_id[i],NULL);
  }
  pthread_cancel(assistent_id); 
  pthread_join(assistent_id,NULL);
  printf("\n");
  
/*  sem_destroy(&assistent_sem);
  sem_destroy(&student_sem);
  for(i = 0; i < num; i++){
    sem_destroy(&turn_sem[i]);
  }
  pthread_mutex_destroy(&queue_mutex);
  free(queue);
  free(turn_sem);
*/
  return 0;
}

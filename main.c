#include<stdio.h>
#include"functions.h"

int main (){
  srand(time(NULL));  
  //num_students = random_number(3,40);
  num_students = 3;

  int num = num_students;

  printf("%d\n",num);
  
  sem_init(&assistent_sem,0,1);
  sem_init(&student_sem,0,1);
  sem_init(&turn_sem,0,1);

  pthread_t assistent_id;
  pthread_attr_t assistent_attr;
  pthread_attr_init(&assistent_attr);
  pthread_create(&assistent_id,&assistent_attr,assistent,NULL);
 
  int id[9] = {1,2,3,4,5,6,7,8,9};
  pthread_t student_id[num];
  pthread_attr_t student_attr[num];
  int i;
  for(i = 0; i < num; i++){
    pthread_attr_init(&student_attr[i]);
    pthread_create(&student_id[i],&student_attr[i],student, &id[i]);
  }

  for(i = 0; i < num; i++){
    pthread_join(student_id[i],NULL);
  }
  pthread_join(assistent_id,NULL);
  printf("\n");

  return 0;
}

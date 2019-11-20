#include "driver.h"
unsigned int inqueue =0 ;
unsigned int runtask =0 ;
int count1 =0 ;
struct task_list list ; 
int start_timer(){
    timer1_isr_init();
  timer1_attachInterrupt(timer_ISR);
   timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
   timer1_write(500000);
   interrupts();
}

uint32_t add_task_timer(int Delay, int  period, void (*pTask)()){
  if (list.num==0) {
    list.tSCH[0].pTask = pTask;
    list.tSCH[0].Delay =Delay;
    list.tSCH[0].Period= period;
    list.tSCH[0].id = 0;
    list.num++;
    return 0;
  } 
  if (list.num == MAXTASK) return -1 ;
  int i =0 ;
  while (i<list.num && Delay > list.tSCH[i].Delay ){
  i++;
 // Serial.print("delay : ");
  //Serial.println(Delay);
 // Serial.print("index : ");
//  Serial.println(i);
}
    
    if (i == list.num ) {
      list.tSCH[i].pTask = pTask;
      list.tSCH[i].Delay = Delay;
      list.tSCH[i].Period = period ;
      list.tSCH[i].id = i ;
    } else {
      memmove(&list.tSCH[i+1],&list.tSCH[i],sizeof(list.tSCH[0])*(list.num-i));
      list.tSCH[i].pTask = pTask;
      list.tSCH[i].Delay = Delay;
      list.tSCH[i].Period = period ;
      list.tSCH[i].id = i ;
    }
    list.num++;
    return i ;
  
}
int remove_timer(uint32_t id ){
  int i =0 ;
  while (list.tSCH[i].id != id && i < list.num  ) i ++ ;
    if (i == list.num ) return -2 ;
    if (i== list.num -1) {
      list.tSCH[i].pTask=0x0000;
      list.tSCH[i].Period=0;
      list.tSCH[i].Delay =0;
      list.tSCH[i].id =-1 ;
    } else {
      memmove(&list.tSCH[i] ,&list.tSCH[i+1],(list.num -i -1)*sizeof(task_t)  );
    }
    list.num -- ;
  
}

int timer_dispatch (){
  if ( runtask) {
    int i=0;
  //   Serial.println(list.num);
    while (i<list.num  )
    {
     // Serial.print("inqueue : ");
      //Serial.println(i);
      if (list.tSCH[i].Delay*10 ==inqueue){
        count1 = i ;
        
    //   Serial.println(list.tSCH[i].Delay);
        (*(list.tSCH[i].pTask))() ;
        if (list.tSCH[i].Period ==0) remove_timer(list.tSCH[i].id);
         else list.tSCH[i].Delay = list.tSCH[i].Period;
      } else {
        
        list.tSCH[i].Delay -= inqueue/10;        
      }
      i++;
    }
    i = count1 ;
    //Serial.println(count1);
    while (i>=0 )
    {
      //Serial.println(i);
      task_t temp = list.tSCH[i];
      int j =i+ 1 ; 
      while (j<list.num &&list.tSCH[i].Delay>list.tSCH[j].Delay) j++;
       memmove(&list.tSCH[i],&list.tSCH[i+1],(j-i -1)*sizeof(task_t));
       list.tSCH[j-1] = temp ;
      i--;
    }
      inqueue=0;
        runtask =0;
        count1=0;
        return 1; 
  }
return 0 ;
}

void timer_ISR() {
   inqueue ++;
   if (list.tSCH[0].Delay*10 == inqueue){
     runtask = 1 ; 
     
    // Serial.print("delay : ");
     //Serial.println(list.tSCH[0].Delay);
  
   }
}
int stop_timer (){
  Serial.println("stopping timer ");
  timer1_disable();
  noInterrupts();
}

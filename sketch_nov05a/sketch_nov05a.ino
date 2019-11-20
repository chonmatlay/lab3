#include "driver.h"
gen_func(1);
gen_func(2);
gen_func(3);
gen_func(4);
gen_func(5);
gen_func(6);
gen_func(7);
gen_func(8);
gen_func(9);
gen_func(10);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(300);
 Serial.println();
  add_task_timer(5,7 ,func(1));
  add_task_timer(9,4,func(2));
   start_timer() ;
}
  int i =2 ;
void loop() {
  // put your main code here, to run repeatedly:
int ret =timer_dispatch();
if (ret){
if ( i==3){ 
 Serial.println("add another task");
 
  add_task_timer(4,5,func(3));
  add_task_timer(6,0,func(4));
  i++; 
 } else  i++;
if (i ==100) stop_timer();
}
}

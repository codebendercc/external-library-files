///////////////////////////////////////////////////////////////////////////
//
//  ########   #  ####   ######    ######  ######    #####    #####  ######
//  #   #   #  #  #   #  #     #  #        #     #  #     #  #       #
//  #   #   #  #  #   #  #     #   #####   ######   #######  #       ######
//  #   #   #  #  #   #  #     #        #  #        #     #  #       #
//  #   #   #  #  #   #  ######   ######   #        #     #   #####  ######
//  R=========E=========S=========E=========A=========R=========C=========H
//
//                        Computing the World Within
//
//  MindSpace Research, Est. September 24, 1999
//
///////////////////////////////////////////////////////////////////////////

#include <osw_min.h>

osw_task Hz5, sec1, sec5;

void* go5Hz(void* _pData)
{
  Serial.print('.');
  return OSW_NULL;
}

void* go1sec(void* _pData)
{
  Serial.print('s');
  return OSW_NULL;
}

void* go5sec(void* _pData)
{
  Serial.println('S');
  return OSW_NULL;
}


void setup() {                
  Serial.begin(9600); 
  Hz5.taskCreate("5Hz", go5Hz, OSW_NULL, 200); 
  sec1.taskCreate("1sec", go1sec, OSW_NULL, 1000); 
  sec5.taskCreate("5sec", go5sec, OSW_NULL, 5000); 
}

void loop() {
  osw_tasks_go();
}

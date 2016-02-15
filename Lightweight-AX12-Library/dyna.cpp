//Super Lightweight Dynamixel AX-12 Library
//by Andrew Bischoff

#include "dyna.h"



void setRegister(unsigned char ID, unsigned char ins, unsigned char data)
{
    unsigned char Checksum = (~(ID + 4 + 3 + ins + data))&0xFF;
    Serial3.write(0xff);                
    Serial3.write(0xff);
    Serial3.write(ID);
    Serial3.write(4);//length
    Serial3.write(0x03);//write
    Serial3.write(ins);
    Serial3.write(data);
    Serial3.write(Checksum);
    delay(4);        
}

void factoryReset(unsigned char id)
{
    Serial3.write(0xff);                
    Serial3.write(0xff);
    Serial3.write(id);
    Serial3.write(2);//len
    Serial3.write(0x06);//reset instruction
    Serial3.write(~(id + 2+ 0x06));//checksum
    delay(4);        
}

void setRegister2(unsigned char ID, unsigned char ins, int data)
{
    unsigned char dataH = data >> 8;  
    unsigned char dataL = data;
    unsigned char Checksum = (~(ID + 5 + 3 + ins + dataH + dataL))&0xFF;
    Serial3.write(0xff);                
    Serial3.write(0xff);
    Serial3.write(ID);
    Serial3.write(5);//length
    Serial3.write(0x03);//write
    Serial3.write(ins);
    Serial3.write(dataL);
    Serial3.write(dataH);
    Serial3.write(Checksum);
    delay(4);              
}

unsigned int readRegister2(unsigned char ID, unsigned char ins){	
   while(Serial3.available()>0){
      Serial3.read();
   }
    boolean valid = false;
    unsigned char Checksum = (~(ID + 4  + 0x02 + ins + 2));
    Serial3.write(0xff);
    Serial3.write(0xff);
    Serial3.write(ID);
    Serial3.write(4);//length
    Serial3.write(0x02);//read
    Serial3.write(ins);
    Serial3.write(2);
    Serial3.write(Checksum);
    
    for(int i=0;i<8;i++)Serial3.read();
    delay(5);
    
    
    byte buf[8];
    while((Serial3.available() > 0)){
      if(Serial3.read()==255 && Serial3.peek()){
        //possible packet found
        buf[0]=0xff;//ff
        buf[1]=Serial3.read();//ff
        buf[2]=Serial3.read();//id
        buf[3]=Serial3.read();//len
        buf[4]=Serial3.read();//err
        buf[5]=Serial3.read();//par
        buf[6]=Serial3.read();//par
        buf[7]=Serial3.read();//check
        unsigned char checksum = ~(buf[2] + buf[3] + buf[4] + buf[5] + buf[6]);
        if(checksum==buf[7]){
          valid=true;
        }
        else{
          valid=false;
        }
      }
      else{
        Serial3.read();
      }      
    } 
	
	unsigned int num = buf[6];
	num = num<<8;
	num+=buf[5];
    
    if(valid)
      return num;
    else
      return -1;
}

int readRegister(unsigned char ID, unsigned char ins){	
   while(Serial3.available()>0){
      Serial3.read();
   }
    boolean valid = false;
    unsigned char Checksum = (~(ID + 4  + 0x02 + ins + 1));
    Serial3.write(0xff);
    Serial3.write(0xff);
    Serial3.write(ID);
    Serial3.write(4);//length
    Serial3.write(0x02);//read
    Serial3.write(ins);
    Serial3.write(1);
    Serial3.write(Checksum);
    
    for(int i=0;i<7;i++)Serial3.read();
    delay(10);
    
    
    byte buf[7];
    while((Serial3.available() > 0)){
      if(Serial3.read()==255 && Serial3.peek()){
        //possible packet found
        buf[0]=0xff;//ff
        buf[1]=Serial3.read();//ff
        buf[2]=Serial3.read();//id
        buf[3]=Serial3.read();//len
        buf[4]=Serial3.read();//err
        buf[5]=Serial3.read();//par
        buf[6]=Serial3.read();//check
        unsigned char checksum = ~(buf[2] + buf[3] + buf[4] + buf[5]);
        if(checksum==buf[6]){
          valid=true;
        }
        else{
          valid=false;
        }
      }
      else{
        Serial3.read();
      }      
    } 
    
    if(valid)
      return (buf[5]);
    else
      return -1;
}





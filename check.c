#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
char ps2(void);
#define RLEDs ((volatile long *) 0xFF200000)
int main(){
    char text[] = "Once upon a time a little girl tried to make a living by selling matches in the street." ;
    int index = 0;
    while(1){
        char target = tolower(text[index]);
        char input = ps2();
        if (target == input){
            index = index +1;
        }
		*RLEDs = index;
    }
}

char ps2(void){
    unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
    volatile int* ps2_ptr = (int*)0xff200100;
    int data = *(ps2_ptr);
    bool valid = data & 0x8000;
    int make_code[26] = {0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A };
    if(valid){
        byte1 = byte2;
		byte2 = byte3;
		byte3 = data & 0xFF;
    }
    char output = 0;
    for(int i = 0; i <26; i++){
        if(byte3 == make_code[i]){
            output = 97 + i;
            return output;
        }
    }
    if (byte3 == 0x29){
        output = 32;
    }
    else if (byte3 == 0x41){
        output = ',';
    }
    else if (byte3 == 0x49){
        output = '.';
    }
    return output;
}
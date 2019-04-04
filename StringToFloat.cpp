#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <float.h>

enum State{START, PLUS, MINUS, GOT_DECIMAL, WHOLE, DECIMAL, NOT_NUMBER, EXP_SIGN, GOT_EXP, EXP_NUMBER, EXP_CALCULATION, DONE};

float stringToFloat(const char input[]){
	float num = 0;
	int index = 0;
	int sign = 1;
	int expSign = 1;
	int power = -1;
	float exponent = 0;

	State s = START;
	
	char digit = input[index];
	
	while (s != DONE){
		
		switch(s){
			case START:
				if (digit == '+'){
					s = PLUS;
				}
				else if (digit == '-'){
					s = MINUS;
				}
				else if (digit == '.'){
					s = GOT_DECIMAL;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = WHOLE;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case PLUS:
				index++;
				digit = input[index];
				if (digit == '.'){
					s = GOT_DECIMAL;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = WHOLE;
				}
				else{
					s = NOT_NUMBER;
				}
				break;	
			
			case MINUS:
				index++;
				digit = input[index];
				sign = -1;
				if (digit == '.'){
					s = GOT_DECIMAL;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = WHOLE;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case GOT_DECIMAL:
				index++;
				digit = input[index];
				if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = DECIMAL;
				}
				else if (digit == 'e' || digit == 'E'){
					s = GOT_EXP;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case WHOLE:
				if (num > FLT_MAX/10 - (int)(digit - '0')){
					s = DONE;
					num = INFINITY;
					digit = '\0';
				}
				else if (num < -FLT_MAX/10 + (int)(digit - '0')){
					s = DONE;
					num = -INFINITY;
					digit = '\0';
				}
				else{
					num *= 10;
					num += (digit - '0') * sign;
					index++;
					digit = input[index];
				}
				
				if (digit == '\0'){
					s = DONE;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = WHOLE;
				}
				else if (digit == 'e' || digit == 'E'){
					s = GOT_EXP;
				}
				else if (digit == '.'){
					s = GOT_DECIMAL;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case DECIMAL:
				num += sign * (int)(digit-'0') * pow(10, power);
				
				index++;
				digit = input[index];
				power--;
				
				if (digit == '\0'){
					s = DONE;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = DECIMAL;
				}
				else if (digit == 'e' || digit == 'E'){
					s = GOT_EXP;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case GOT_EXP:
				index++;
				digit = input[index];
				
				if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = EXP_NUMBER;
				}
				else if (digit == '-' || digit == '+'){
					s = EXP_SIGN;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
				
			case EXP_SIGN:
				if (digit == '-'){
					expSign = -1;
				}
				index++;
				digit = input[index];
				
				if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = EXP_NUMBER;
				}
				else{
					s = NOT_NUMBER;
				}
				break;	
			
			case EXP_NUMBER:
				exponent *= 10;
				exponent += (int)(digit-'0') * sign;
				
				index++;
				digit = input[index];
				
				if (digit == '\0'){
					s = EXP_CALCULATION;
				}
				else if ((int)(digit - '0') >= 0 && (int)(digit - '0') <= 9){
					s = EXP_NUMBER;
				}
				else{
					s = NOT_NUMBER;
				}
				break;
			
			case EXP_CALCULATION:
				if (num > FLT_MAX/(pow (10, exponent))){
					num = INFINITY;
				}
				else if (num < -FLT_MAX/(pow(10, exponent))){
					num = -INFINITY;
				}
				else{
					num *= pow(10, exponent);
				}
				s = DONE;
				break;
				
			case NOT_NUMBER:
				num = NAN;
				s = DONE;
				break;
			
			case DONE:
				break;
			default:
				break;
		}
	}
	
	return num;
}
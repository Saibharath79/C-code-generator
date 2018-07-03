#include<stdio.h>
#include<malloc.h>
int main(){
int A,AX,BX,CX,DX,EX,FX,GX,HX;
const int B=1;
const int S=1;
A=0;
scanf("%d",&AX);
DX=B;
BX=B;
CX=B;
X:
if(BX<=AX){
CX=CX*BX;
BX=DX+BX;
goto X;
}else{
printf("%d\n",CX);
}return 0;
}
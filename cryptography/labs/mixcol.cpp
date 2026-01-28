#include <bits/stdc++.h>
void print(int arr[4][4]){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            std::cout << std::hex << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
}

//reduction of a0 + a1x + a2x2 + a3x3
int x_mul(int val){
    int temp = val << 1;
    if(val & 0x80)  
        temp ^= 0x1B ;
    return temp & 0xFF; 
}

int x_plus_mul(int val){
    return x_mul(val)^val;
}
void mixcol(int arr[4][4],int col){
    int temp[4];
        for(int j = 0 ; j < 4 ; j++){
            temp[j] = x_mul(arr[j][col])^x_plus_mul(arr[(j+1)%4][col])^arr[(j+2)%4][col]^arr[(j+3)%4][col];
        }
        for (int j = 0; j < 4; j++) {
        arr[j][col] = temp[j];
    }
}
int main(){
    int arr[4][4] = {{0xAB,0xD4,0x11,0x98},{0x9F,0x5E,0x6B,0xC2},{0x3D,0x72,0xE1,0x4A},{0xF8,0x23,0x7C,0x15}};
    printf("Matrix before Mix Column:\n");
    print(arr);
    printf("Matrix after Mix Column:\n");
    for(int i = 0 ; i < 4 ; i++){
        mixcol(arr,i);
    }
    print(arr);
}
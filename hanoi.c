#include <stdio.h>
#include <math.h>
#define PLATE_MAX 50

struct pole{
    int arr[PLATE_MAX];
    int count;
} pole[3];

void printTower(){
    int i, j;
    for (i=0; i<3; i++)
    {
        printf("%d: ", i);
        if (pole[i].arr[0] == 0) printf("x");
        else {
            for (j=0; pole[i].arr[j]!=0; j++){
               printf("%d ", pole[i].arr[j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int move(int get1, int get2){
    if (get1<0 || get1>2 || get2<0 || get2>2 || get1 == get2){
        printf("must be between 0 and 2, also must be different number. \n");
        return 0;
    }
    if (pole[get1].arr[0] == 0){
        printf("not enough plates to move. \n");
        return 0;
    }
    int lastget1, lastget2;
    for (lastget1 = 0; pole[get1].arr[lastget1+1]!=0; lastget1++){}
    for (lastget2 = 0; pole[get2].arr[lastget2]!=0; lastget2++){}
    if (pole[get2].arr[0] != 0) {
        if (pole[get1].arr[lastget1] > pole[get2].arr[lastget2-1]) {
            printf("Wrong access. \n");
            return 0;
        }
    }
    pole[get2].arr[lastget2] = pole[get1].arr[lastget1];
    pole[get1].arr[lastget1] = 0;
    
    return 1; //정상적으로 실행되면 1 반환.
}

int checkFinish(int n){
    if (pole[n].arr[pole[n].count] != 0){
        return 1;
    }
    return 0;
}

int advsolve(int n, int from, int to) {
    int via = 3 ^ from ^ to;
    if (n==1){
        move(from, to);
        printf("Moving plate %d from pole %d to pole %d\n", 1, from, to);
        printTower();
        return 0;
    } else {
        advsolve(n-1, from, via);
        move(from, to);
        printf("Moving plate %d from pole %d to pole %d\n", n, from, to);
        printTower();
        advsolve(n-1, via, to);
        return 0;
    }
}

int main(){
    int num, i, objectpole, get1, get2, howmanymove = 0;
    char atosol;
    printf("How many plates? (value must be between 1 and 50): "); 
    scanf("%d", &num);
    while (num < 1 || num > 50){
        printf("Must be bigger than 0 and smaller than 51\nPlease type again :");
        scanf("%d", &num);
    }
    printf("Which pole do you want to move? (value must be betweeen 1 and 2): "); 
    scanf("%d", &objectpole);
    while (objectpole < 1 || objectpole > 2){
        printf("Must be bigger than 0 and smaller than 51\nPlease type again :");
        scanf("%d", &objectpole);
    }
    printf("Autosolve? (Y/N): "); //자동풀이
    scanf(" %c", &atosol);
    while (atosol!='y'&&atosol!='n'&&atosol!='Y'&&atosol!='N'){
        printf("\nAutosolve? (must be Y or N): ");
        scanf(" %c", &atosol);
    }
    printf("\n");
    for (i=0; i<3; i++){ //초기화, 쓰래깃값 방지
        pole[i].count = num-1;
        for (int n=0; n<num; n++) {
            pole[i].arr[n]=0;
        }
    }
    for (i=0; i<num; i++){ //첫기둥 초기화
      pole[0].arr[i] = num-i;
    }
    printTower();
    if(atosol=='y'||atosol=='Y'){
        advsolve(num, 0, objectpole);
        printf("Number of moves: %g\n", round(pow(2, num)-1));
    } else if (atosol=='n'||atosol=='N'){
        printf("Input should be two different digits between 0 and 2 with a space between two digits. \nFor example: 0 1 \n");
        while(1) {
            printf("Move: ");
            scanf("%d %d", &get1, &get2);
            if (move(get1, get2)) howmanymove++; //1 반환받으면 howmanymove++로 움직임 횟수 추가, 0이면 return 0; 으로 프로그렘 종료.
            else return 0;
            printTower();
            if (checkFinish(objectpole)){
                printf("Number of moves: %d\n", howmanymove);
                printf("Finish\n");
                break;
            }
        }
    }
    return 0;
}

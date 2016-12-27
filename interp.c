/* this file is used for tutorial to build the compiler step by step*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int token;            /* current token*/
char *src, *old_src;  /* pointer to source code string;*/
int poolsize;         /* default size of text/data/stack*/
int line;             /* line number*/

int *text ,
    *old_text , 
    *stack;
char *data;

int *pcd_cnt , /*procedure count*/
    *stk_ptr , /*current stack pointer*/
    *bss_ptr , /*point to some loc in stack*/
    reg , /*AX? general register , store result after any command*/
    cycle;

/*instructions of CPU. They are based on x86 , but more simple*/
/*instructions with arg will put first*/
enum { LEA , IMM , JMP , CALL , JZ  , JNZ , ENT , ADJ , LEV , LI , LC , SI , SC , PUSH ,
       OR , XOR , AND , EQ , NE , LT , GT  , LE  , GE  , SHL , SHR , ADD , SUB , MUL , DIV , MOD ,
       OPEN , READ , CLOS ,PRTF , MALC , MSET , MCMP , EXIT }; 

void next(){
    token = *src++;
    return;
}

void expression(int level){
    /* do nothing*/
}

void program(){
    next(); /* get next token*/
    while (token > 0){
        printf("token is: %c\n", token);
        next();
    }
}


int eval(){
    int op , *tmp;
    while(1){
        switch(op){
            case IMM:/*load Immediate(IMM) data to reg*/
                reg = *pcd_cnt++;
                break;
            case LC:/*load char(LC) to reg from adr*/
                reg = *(char *)reg;
                break;
            case LI:/*load int(LI) to reg from adr*/
                reg = *(int *)reg;
                break;
            case SC:/*save char(SC) to adr from reg , adr on stk*/
                reg = *(char *)*stk_ptr++ = reg;
                break;
            case SI:/*save int(SI) to adr from reg , adr on stk*/
                *(int *)*stk_ptr++ = reg;
                //(get a adr from stk , and save val into adr's loc
                break;
            case PUSH:/*push the val of reg onto stk*/
                *--stk_ptr = reg;//(push a adr into stk)
                break;
            case JMP:
                pcd_cnt = (int *)*pcd_cnt;
                break;
            case JZ:
                pcd_cnt = reg ? pcd_cnt + 1 : (int *)*pcd_cnt;
                break;
            case JNZ:
                pcd_cnt = reg ? (int *)*pcd_cnt : pcd_cnt + 1;
                break;
            case CALL:
                *--stk_ptr = (int)(pcd_cnt + 1); pcd_cnt = (int *)*pcd_cnt;
                break;
            case ENT:
                *--stk_ptr = (int)bss_ptr; 
                bss_ptr = stk_ptr; 
                stk_ptr = stk_ptr - *pcd_cnt++;
                break;
            case ADJ:



        }
    }

    return 0;
}

int main(int argc, char **argv)
{

    int i, fd;

    argc--;
    argv++;

    poolsize = 256 * 1024; /* arbitrary size*/
    line = 1;

    if ((fd = open(*argv, 0)) < 0){
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(poolsize))){
        printf("could not malloc(%d) for source area\n", poolsize);
        return -1;
    }
    /* read the source file*/
    if ((i = read(fd, src, poolsize-1)) <= 0){
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0; /* add EOF character*/
    close(fd);

    /*malloc memory for virtual machine*/
    if(!(text = old_text = malloc(poolsize))){
        printf("couldn't malloc %d for text area\n" , poolsize);
        return -1;
    }
    if(!(data = malloc(poolsize))){
        printf("couldn't malloc %d for data area\n" , poolsize);
        return -1;
    }
    if(!(stack = malloc(poolsize))){
        printf("couldn't malloc %d for stack area\n" , poolsize);
        return -1;
    }
    memset(text , 0 , poolsize);
    memset(data , 0 , poolsize);
    memset(stack , 0 , poolsize);

    bss_ptr = stk_ptr = (int *)((int)stack + poolsize);
    reg = 0;

    program();
    return eval();
}

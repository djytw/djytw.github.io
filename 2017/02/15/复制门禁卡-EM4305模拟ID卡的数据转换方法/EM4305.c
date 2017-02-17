#include <stdio.h>
#include <string.h>
#define write(x) if(pos>=32){ \
					page6<<=1; \
					page6+=(x); \
				}else{ \
					page5<<=1; \
					page5+=(x); \
					pos++; \
				}
void calc(int id){
	printf("CARD: %010d %03d,%05d\nCARD: 0x%08X\n",id,(id>>16)&0xff,id&0xffff,id);
	int i,j,t;
	int ecc_r[4],ecc;	//ecc是行校验码，ecc_r[i]是第i列的列校验码
	int pos=19;			//有19位数据固定在开头
	unsigned int page5=0x7fc00, page6=0;
	memset(ecc_r,0,sizeof(ecc_r));
	//第一步：写入ID号以及行校验
	for(i=7;i>=0;i--){ 
		ecc=0;
		for(j=3;j>=0;j--){ 				//32位2进制数，从高到低写入page中
			t=(id>>i*4+j)&1;			//t是当前位
			ecc+=t;						//ecc是行校验码
			ecc_r[3-j]+=t;
			write(t);
		}
		write(ecc&1);					//每4个循环写入一次校验码
	}
	//第二步：写入列校验和结束符
	for(i=0;i<4;i++){					
		write(ecc_r[i]&1);
	}
	write(0);
	//第三步：翻转
	int tr[]={0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 12, 7, 15};
			//16进制数翻转结果数组 
	t=page5;page5=0;
	for(i=3;i>=0;i--){
		page5<<=4;
		page5+=tr[(t>>i*8)&0xf];
		page5<<=4;
		page5+=tr[(t>>i*8+4)&0xf];
	}
	t=page6;page6=0;
	for(i=3;i>=0;i--){
		page6<<=4;
		page6+=tr[(t>>i*8)&0xf];
		page6<<=4;
		page6+=tr[(t>>i*8+4)&0xf];
	}
	printf("Page4: %08X\nPage5: %08X\nPage6: %08X\n",0x5f800100,page5,page6);
}
int main(int argc, char** argv){
	if(argc<2){
		printf("Usage: EM4305 <id number>\n");
	}else{
		int id;
		sscanf(argv[1],"%d",&id);
		calc(id);
	}
}
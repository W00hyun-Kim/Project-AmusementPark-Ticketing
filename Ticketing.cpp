#include <stdio.h>
#include <time.h>
#include <string.h>    // strcmp �Լ��� ����� ��� ����
#include <stdlib.h>

int ticketType,ticketDayType,preferenceType,amount,price,resultPrice,isExit;
int totalPrice = 0;
int position = 0;
int filePosition = 0;
int reportPosition = 0;
int orderList[100][6] = {0};

//���� �̿�� ���� 
const int COMP_BABY_PRICE = 15000,
  		  COMP_ADULT_DAY_PRICE = 62000, COMP_ADULT_NIGHT_PRICE = 50000, 
		  COMP_TEEN_DAY_PRICE = 54000, COMP_TEEN_NIGHT_PRICE = 43000,
		  COMP_KID_DAY_PRICE = 47000, COMP_KID_NIGHT_PRICE = 36000,
		  COMP_OLD_DAY_PRICE = 47000, COMP_OLD_NIGHT_PRICE = 36000;

//��ũ �̿�� ����
const int PARK_BABY_PRICE = 15000,
  		  PARK_ADULT_DAY_PRICE = 59000, PARK_ADULT_NIGHT_PRICE = 47000, 
		  PARK_TEEN_DAY_PRICE = 52000, PARK_TEEN_NIGHT_PRICE = 41000,
		  PARK_KID_DAY_PRICE = 46000, PARK_KID_NIGHT_PRICE = 35000,
		  PARK_OLD_DAY_PRICE = 46000, PARK_OLD_NIGHT_PRICE = 35000;  
	
//ID ��ȣ �м�
char num[13];

const int OLD_GENERATION = 1900, NEW_GENERATION = 2000,
		  MALE_OLD = 1, FEMALE_OLD = 2, MALE_NEW = 3, FEMALE_NEW = 4;
			  
//���̿� ���� ���� 
const int MAX_BABY = 2, MIN_KIDS = 3, MIN_TEEN =13, MIN_ADULT =19,
		  MAX_KIDS = 12, MAX_TEEN = 18, MAX_ADULT = 64, MIN_OLD = 65;
	
//������
const float DISABLE_DISCOUNT_RATE = 0.5, SOLDIER_DISCOUNT_RATE = 0.51,
			MULTICHILD_DISCOUNT_RATE = 0.7, MERIT_DISCOUNT_RATE = 0.5,
			PREGNANT_DISCOUNT_RATE = 0.5;
	
//�ִ� �ֹ���
const int MAX_COUNT = 10, MIN_COUNT =1;
	

int ageCal(char idNum[13]) {
	int yearIndex, year, month, day, man;
	//���� �ð� �ҷ����� �Լ� 
	struct tm*t;
	time_t base = time(NULL);	
	t = localtime(&base);	
	
	//�ֹε�Ϲ�ȣ �� ���ڸ�(����) 
	yearIndex = (idNum[0]-48)*10 + (idNum[1]-48);
	if(yearIndex <=22) {
		year = 2000 + yearIndex;
	} else {
		year = 1900 + yearIndex ; 
	}		
	
	//���� ���� 
	int age = (t->tm_year+1900)-year + 1;
	
	//�ֹι�ȣ ���� ���ϱ�
	month = (idNum[2]-48)*10 + (idNum[3]-48);
	day = (idNum[4]-48)*10 + (idNum[5]-48);
	
	//������ �������� -1, ���� ���������� -2 	
	//���� ������ ��(������ ������) 
	if(month>t->tm_mon+1) {
		 man = age -2;
	
	//���� ���� �� �� �� �� 
	} else if(month==t->tm_mon+1) {
		if(day> t->tm_mday) {
			man = age -2;
		} else {
			man = age -1;
		}
	} else {
		man = age-1;
	}
	return man;
}

int calAgeGroup(int age) {

	if(age<MAX_BABY) {
		return 1;
	} else if(age>=MIN_KIDS && age<=MAX_KIDS) {
		return 2;
	} else if(age>=MIN_TEEN && age<=MAX_TEEN) {
		return 3;
	} else if(age>=MIN_OLD) {
		return 4;
	} else if(age>=MIN_ADULT && age<=MAX_ADULT){
		return 5;
	}
}

int calPriceProcess(int ticketType, int ticketDayType, int agegroup) {
	int price;
	
	if(ticketType==1) {
		if(ticketDayType==1) {
			switch(agegroup) {
				case 1 : {	//baby
					price = COMP_BABY_PRICE;
					break;
				}
				case 2: {	//kids 
					price = COMP_KID_DAY_PRICE;
					break;
				}
				case 3 : {	//teen
					price = COMP_TEEN_DAY_PRICE;
					break;
				}
				case 4 : {	//old
					price = COMP_OLD_DAY_PRICE;
					break;
				}
				case 5: {	//adult
					price = COMP_ADULT_DAY_PRICE;				
					break;
				}					
			}
		} else if(ticketDayType==2) {	//night�϶� 
			switch(agegroup) {
				case 1 : {	//baby
					price = COMP_BABY_PRICE;
					break;
				}
				case 2: {	//kids 
					price = COMP_KID_NIGHT_PRICE;
					break;
				}
				case 3 : {	//teen
					price = COMP_TEEN_NIGHT_PRICE;
					break;
				}
				case 4 : {	//old
					price = COMP_OLD_NIGHT_PRICE;
					break;
				}
				case 5: {	//adult
					price = COMP_ADULT_NIGHT_PRICE;				
					break;
				}					
			}
		}
	} else if(ticketType==2) {			//park���϶�  
		if(ticketDayType==1) {			//day �� �� 
			switch(agegroup) {
				case 1 : {	//baby
					price = COMP_BABY_PRICE;
					break;
				}
				case 2: {	//kids 
					price = PARK_KID_DAY_PRICE;
					break;
				}
				case 3 : {	//teen
					price = PARK_TEEN_DAY_PRICE;
					break;
				}
				case 4 : {	//old
					price = PARK_OLD_DAY_PRICE;
					break;
				}
				case 5: {	//adult
					price = PARK_ADULT_DAY_PRICE;				
					break;
				}					
			}
		} else if(ticketDayType==2) {	//night�϶� 
			switch(agegroup) {
				case 1 : {	//baby
					price = COMP_BABY_PRICE;
					break;
				}
				case 2: {	//kids 
					price = PARK_KID_NIGHT_PRICE;
					break;
				}
				case 3 : {	//teen
					price = PARK_TEEN_NIGHT_PRICE;
					break;
				}
				case 4 : {	//old
					price = PARK_OLD_NIGHT_PRICE;
					break;
				}
				case 5: {	//adult
					price = PARK_ADULT_NIGHT_PRICE;				
					break;
				}					
			}
		}
	}
	return price;
}

int calDiscount(int price, int preferenceType) {
	if(preferenceType==1) {	//���� 
		price;
	} else if(preferenceType==2) {	//����� 
		price = price*DISABLE_DISCOUNT_RATE;
	} else if(preferenceType==3) {	//���������� 
		price =  price*MERIT_DISCOUNT_RATE;
	} else if(preferenceType==4) {	//���ڳ�
		price =  price*MULTICHILD_DISCOUNT_RATE;						
	} else if(preferenceType==5) {	//�ӻ�� 
		price = price*PREGNANT_DISCOUNT_RATE;
	} else if(preferenceType==6) {	//�ް��庴 
		price = price*SOLDIER_DISCOUNT_RATE; 
	}
						
	return price;
}


char *preferenceTypeConverter(int preferenceType) {
	
	char* prefer[6] = {"������� ����","����� �������","���������� �������","�ް��庴 �������","�ӻ�� �������","���ڳ� �������"};	
	return prefer[preferenceType-1];
	
}

char *agegroupConverter(int agegroup) {
	char* age[5] = {"����","���","û�ҳ�","����","����"};	
	return age[agegroup-1];
}


int main() {
	int orderCount = 0;
	int newOrderCount = 1;
	
	while(newOrderCount==1) {
		int orderCount = 0;
		do {	
			
				while(true) {
			
					printf("�̿�� Ÿ���� �����ϼ���.\n");
					printf("1. �����̿��\n");
					printf("2. ��ũ�̿��\n>> ");
					scanf("%d",&ticketType);
					printf("\n");
					
					printf("������ �����ϼ���.\n");
					printf("1. �ְ���(1Day)\n");
					printf("2. �߰���(After4)\n>> "); 
					scanf("%d",&ticketDayType);
					printf("\n");
						
					printf("�ֹι�ȣ�� �Է��ϼ���.\n>> ");
					scanf("%s", num);	
					int age = ageCal(num);
					int agegroup = calAgeGroup(age);	
					printf("\n");
					
					printf("��� �ֹ��Ͻðڽ��ϱ�?(�ִ� 10��)\n>> ");
					scanf("%d",&amount);
					printf("\n");
					
					if(ticketType==1) {
						printf("�������� �����ϼ���.\n");
						printf("1. ����(���� ���� �ڵ� ó��))\n");
						printf("2. �����\n"); 
						printf("3. ����������\n"); 
						printf("4. �ް��庴\n"); 
						printf("5. �ӻ��\n"); 
						printf("6. �ٵ����ູī�� ������\n"); 
						scanf("%d",&preferenceType);
						printf("\n");
		
					} else if(ticketType==2) {
						printf("�������� �����ϼ���.\n");
						printf("1. ����(���� ���� �ڵ� ó��))\n");
						printf("2. �����\n"); 
						printf("3. ����������\n"); 
						printf("4. �ް��庴\n"); 
						scanf("%d",&preferenceType);
						printf("\n");				
					}
								
									
					price = calPriceProcess(ticketType, ticketDayType, agegroup);
					price = calDiscount(price, preferenceType);	
					resultPrice = price*amount;	//���� 
								
					orderList[orderCount][0] = ticketType;
					orderList[orderCount][1] = ticketDayType;
					orderList[orderCount][2] = agegroup;
					orderList[orderCount][3] = amount;
					orderList[orderCount][4] = resultPrice;
					orderList[orderCount][5] = preferenceType;
					orderCount++;
														
					totalPrice = totalPrice + resultPrice;
					
					printf("������ %d�� �Դϴ�.\n",resultPrice);
					printf("�����մϴ�.\n\n");
									
				
					printf("��� �߱� �Ͻðڽ��ϱ�?\n");
					printf("1. Ƽ�Ϲ߱�\n");
					printf("2. ����\n>> ");
					scanf("%d", &isExit);	
					printf("\n");
						
					if(isExit==2) {
						printf("�߱��� �����մϴ�. �����մϴ�.\n\n");	
						break;
					}
				}
			} while(isExit == 1) ;
			
			printf("--------------------**************************-----------------------\n");	
			printf("--------------------***L O T T E  W O R L D***-----------------------\n");
			printf("--------------------**************************-----------------------\n");
			printf("  �� �� ��    �� ��     �� ��    �� ��    �� ��     �� �� �� ��   \n");
			printf("---------------------------------------------------------------------\n");
			
//			printf("%d,%d,%d,%d,%d,%d\n",orderList[0][0],orderList[0][1],orderList[0][2],orderList[0][3],orderList[0][4],orderList[0][5]);
//			printf("%d,%d,%d,%d,%d,%d\n",orderList[1][0],orderList[1][1],orderList[1][2],orderList[1][3],orderList[1][4],orderList[1][5]);
			
			for(int index=0; index < orderCount; index ++) {	
				printf(" %5s ",(orderList[index][0]==1)?"�����̿��":"��ũ�̿��");					
				printf(" %5s ",(orderList[index][1]==1)?"�ְ���":"�߰���");					
				printf(" %7s ",agegroupConverter(orderList[index][2]));		
				printf("%6d�� ", orderList[index][3]);		
				printf("%8d�� ",orderList[index][4]);		
				printf(" %10s\n",preferenceTypeConverter(orderList[index][5]));
			}
			
			printf("\n");
			printf("����� �Ѿ��� %d���Դϴ�. \n", totalPrice);	
			totalPrice = 0;	
			printf("-----------------------------------------------------------------\n");
			printf("��� ����(1: ���ο� �ֹ�, 2: ���α׷� ����)\n>> ");
			scanf("%d",&newOrderCount);
			printf("\n");
			if(newOrderCount==2) {
				break;
			}
	}
	
				
}

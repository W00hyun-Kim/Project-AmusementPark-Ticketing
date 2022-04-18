#include <stdio.h>
#include <time.h>
#include <string.h>    // strcmp �Լ��� ����� ��� ����
#include <stdlib.h>

int ticketType,ticketDayType,preferenceType,amount,price,resultPrice,isExit;
int totalPrice = 0;
int position = 0;
int orderList[100][6] = {0};
int orderCount = 0;
int newOrderCount = 1;

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
		year = NEW_GENERATION + yearIndex;
	} else {
		year = OLD_GENERATION + yearIndex ; 
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
	
	//group1=baby, 2=kids, 3=teen, 4=old, 5=adult
	int price;
	int compDayPrice[5] = {15000,47000,54000,47000,62000}; 
	int compNightPrice[5] = {15000,36000,43000,36000,50000};
	int parkDayPrice[5]={15000,46000,52000,46000,59000};
	int parkNightPrice[5]={15000,35000,41000,35000,47000};

	if(ticketType==1) {
		switch(ticketDayType) {
			case 1 : {
				price = compDayPrice[agegroup-1];
				return price; 
				break;
			}
			case 2 : {
				price = compNightPrice[agegroup-1];
				return price;
				break;
			}
		}
	}	
	if(ticketType==2) {
		switch(ticketDayType) {
			case 1 : {
				price = parkDayPrice[agegroup-1];
				return price; 
				break;
			}
			case 2 : {
				price = compNightPrice[agegroup-1];
				return price;
				break;
			}
		}
	}
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

void selectTicket() {
	do {
			printf("�̿�� Ÿ���� �����ϼ���.\n");
			printf("1. �����̿��\n");
			printf("2. ��ũ�̿��\n>> ");
			scanf("%d",&ticketType);
			printf("\n");
			
			if (ticketType>2) {
				printf("���⿡�� �������ּ���.\n\n");			
			} 
	} while(ticketType>2);
}

void selectDayNightTicket() {
	do {
			printf("������ �����ϼ���.\n");
			printf("1. �ְ���(1Day)\n");
			printf("2. �߰���(After4)\n>> "); 
			scanf("%d",&ticketDayType);
			printf("\n");
			
			if (ticketDayType>2) {
				printf("���⿡�� �������ּ���.\n\n");			
			} 
	} while(ticketDayType>2);
}	

void inputID() {
	do {
			printf("�ֹι�ȣ�� �Է��ϼ���.\n>> ");
			scanf("%s", num);
			
			if (strlen(num)!=13) {
				printf("��Ŀ� ���� �Է����ּ���.(13�ڸ�)\n\n");			
			} 
	} while(strlen(num)!=13);	
}

void ticketAmount() {
	do {
			printf("Ƽ���� �� �� �����Ͻðڽ��ϱ�?(�ִ� 10��)\n>> ");
			scanf("%d",&amount);
			printf("\n");
			
			if (amount>10) {
				printf("�ִ� 10����� ������ �� �ֽ��ϴ�.\n\n");			
			} 
	} while(amount>10);
}

void selectPreference_COMP() {
	do {
			printf("�������� �����ϼ���.\n");
			printf("1. ����(���� ���� �ڵ� ó��))\n");
			printf("2. �����\n"); 
			printf("3. ����������\n"); 
			printf("4. �ް��庴\n"); 
			printf("5. �ӻ��\n"); 
			printf("6. �ٵ����ູī�� ������\n"); 
			scanf("%d",&preferenceType);
			printf("\n");
			
			if (preferenceType>6) {
				printf("���⿡�� �������ּ���.\n\n");			
			} 
	} while(preferenceType>6);	
}

void selectPreference_PARK() {
	do {
			printf("�������� �����ϼ���.\n");
			printf("1. ����(���� ���� �ڵ� ó��))\n");
			printf("2. �����\n"); 
			printf("3. ����������\n"); 
			printf("4. �ް��庴\n"); 
			scanf("%d",&preferenceType);
			printf("\n");
			
			if (preferenceType>4) {
				printf("���⿡�� �������ּ���.\n\n");			
			} 
	} while(preferenceType>4);	
}

void saveArray(int agegroup) {
	 
	orderList[orderCount][0] = ticketType;
	orderList[orderCount][1] = ticketDayType;
	orderList[orderCount][2] = agegroup;
	orderList[orderCount][3] = amount;
	orderList[orderCount][4] = resultPrice;
	orderList[orderCount][5] = preferenceType;
	orderCount++;
											
	printf("������ %d�� �Դϴ�.\n",resultPrice);
	printf("�����մϴ�.\n\n");
					

	printf("��� �߱� �Ͻðڽ��ϱ�?\n");
	printf("1. Ƽ�Ϲ߱�\n");
	printf("2. ����\n>> ");
	scanf("%d", &isExit);	
	printf("\n");
			
}

void printBill() {
	printf("--------------------**************************-----------------------\n");	
	printf("--------------------***L O T T E  W O R L D***-----------------------\n");
	printf("--------------------**************************-----------------------\n");
	printf("  �� �� ��    �� ��     �� ��    �� ��    �� ��     �� �� �� ��   \n");
	printf("---------------------------------------------------------------------\n");
	
	for(int index=0; index < orderCount; index ++) {	
		printf(" %5s ",(orderList[index][0]==1)?"�����̿��":"��ũ�̿��");					
		printf(" %5s ",(orderList[index][1]==1)?"�ְ���":"�߰���");					
		printf(" %7s ",agegroupConverter(orderList[index][2]));		
		printf("%6d�� ", orderList[index][3]);		
		printf("%8d�� ",orderList[index][4]);		
		printf(" %10s\n",preferenceTypeConverter(orderList[index][5]));
	}
	
	printf("\n");
	printf("����� �Ѿ��� %d���Դϴ�. \n\n", totalPrice);	
	totalPrice = 0;	
	printf("-----------------------------------------------------------------\n");
	printf("��� ����(1: ���ο� �ֹ�, 2: ���α׷� ����)\n>> ");
	scanf("%d",&newOrderCount);
	printf("\n");
}

void oneRoundBill() {
	
	while(newOrderCount==1) {
		printf("===================== WELCOME TO LOTTE WORLD =====================\n\n");
		orderCount = 0;
		do {				
				while(true) {
					selectTicket();						//�̿�� ���� �����ϱ�			
					selectDayNightTicket();				//�ְ�,�߰��� �����ϱ� 
						
					inputID(); 							//�ֹε�Ϲ�ȣ(13�ڸ�)�Է¹ޱ� 
					int age = ageCal(num);				//�ֹι�ȣ�� ���� �����ϱ� 
					int agegroup = calAgeGroup(age);	//���̱׷�(����,û�ҳ�....)�����ϱ�	
					printf("\n");
					
					ticketAmount();						//Ƽ���� �ż� 
					
					if(ticketType==1) {
						selectPreference_COMP();		//���� ���ΰ����� ������� 
					} else if(ticketType==2) {
						selectPreference_PARK();		//��ũ�� ���� �� ���� ������ ������� 
					}
																	
					price = calPriceProcess(ticketType, ticketDayType, agegroup);
					price = calDiscount(price, preferenceType);	
					resultPrice = price*amount;	//���� 
					totalPrice = totalPrice + resultPrice;
	  
					saveArray(agegroup);				
											
					if(isExit==2) {
					printf("�߱��� �����մϴ�. �����մϴ�.\n\n");	
					break;
					}								
				}
			} while(isExit == 1) ;
			
			printBill();
			if(newOrderCount==2) {
				break;
			}
	}
	
}


int main() {
	
	oneRoundBill();
			
}

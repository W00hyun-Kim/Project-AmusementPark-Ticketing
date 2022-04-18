#include <stdio.h>
#include <time.h>
#include <string.h>    // strcmp 함수가 선언된 헤더 파일
#include <stdlib.h>

int ticketType,ticketDayType,preferenceType,amount,price,resultPrice,isExit;
int totalPrice = 0;
int position = 0;
int orderList[100][6] = {0};
int orderCount = 0;
int newOrderCount = 1;

//종합 이용권 가격 
const int COMP_BABY_PRICE = 15000,
  		  COMP_ADULT_DAY_PRICE = 62000, COMP_ADULT_NIGHT_PRICE = 50000, 
		  COMP_TEEN_DAY_PRICE = 54000, COMP_TEEN_NIGHT_PRICE = 43000,
		  COMP_KID_DAY_PRICE = 47000, COMP_KID_NIGHT_PRICE = 36000,
		  COMP_OLD_DAY_PRICE = 47000, COMP_OLD_NIGHT_PRICE = 36000;

//파크 이용권 가격
const int PARK_BABY_PRICE = 15000,
  		  PARK_ADULT_DAY_PRICE = 59000, PARK_ADULT_NIGHT_PRICE = 47000, 
		  PARK_TEEN_DAY_PRICE = 52000, PARK_TEEN_NIGHT_PRICE = 41000,
		  PARK_KID_DAY_PRICE = 46000, PARK_KID_NIGHT_PRICE = 35000,
		  PARK_OLD_DAY_PRICE = 46000, PARK_OLD_NIGHT_PRICE = 35000;  
	
//ID 번호 분석
char num[13];

const int OLD_GENERATION = 1900, NEW_GENERATION = 2000,
			  
//나이에 따른 범위 
const int MAX_BABY = 2, MIN_KIDS = 3, MIN_TEEN =13, MIN_ADULT =19,
		  MAX_KIDS = 12, MAX_TEEN = 18, MAX_ADULT = 64, MIN_OLD = 65;
	
//할인율
const float DISABLE_DISCOUNT_RATE = 0.5, SOLDIER_DISCOUNT_RATE = 0.51,
			MULTICHILD_DISCOUNT_RATE = 0.7, MERIT_DISCOUNT_RATE = 0.5,
			PREGNANT_DISCOUNT_RATE = 0.5;
	
//최대 주문량
const int MAX_COUNT = 10, MIN_COUNT =1;
	
int ageCal(char idNum[13]) {	
	int yearIndex, year, month, day, man;
	//오늘 시간 불러오는 함수 
	struct tm*t;
	time_t base = time(NULL);	
	t = localtime(&base);	
	
	//주민등록번호 앞 두자리(연도) 
	yearIndex = (idNum[0]-48)*10 + (idNum[1]-48);
	if(yearIndex <=22) {
		year = NEW_GENERATION + yearIndex;
	} else {
		year = OLD_GENERATION + yearIndex ; 
	}		
	
	//기준 나이 
	int age = (t->tm_year+1900)-year + 1;
	
	//주민번호 월일 구하기
	month = (idNum[2]-48)*10 + (idNum[3]-48);
	day = (idNum[4]-48)*10 + (idNum[5]-48);
	
	//생일이 지났으면 -1, 생일 안지났으면 -2 	
	//월이 지났을 때(생일이 지났음) 
	if(month>t->tm_mon+1) {
		 man = age -2;
	
	//월이 같을 때 일 수 비교 
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
	if(preferenceType==1) {	//없음 
		price;
	} else if(preferenceType==2) {	//장애인 
		price = price*DISABLE_DISCOUNT_RATE;
	} else if(preferenceType==3) {	//국가유공자 
		price =  price*MERIT_DISCOUNT_RATE;
	} else if(preferenceType==4) {	//다자녀
		price =  price*MULTICHILD_DISCOUNT_RATE;						
	} else if(preferenceType==5) {	//임산부 
		price = price*PREGNANT_DISCOUNT_RATE;
	} else if(preferenceType==6) {	//휴가장병 
		price = price*SOLDIER_DISCOUNT_RATE; 
	}
						
	return price;
}

char *preferenceTypeConverter(int preferenceType) {
	
	char* prefer[6] = {"우대적용 없음","장애인 우대적용","국가유공자 우대적용","휴가장병 우대적용","임산부 우대적용","다자녀 우대적용"};	
	return prefer[preferenceType-1];
	
}

char *agegroupConverter(int agegroup) {
	char* age[5] = {"유아","어린이","청소년","노인","성인"};	
	return age[agegroup-1];
}

void selectTicket() {
	do {
			printf("이용권 타입을 선택하세요.\n");
			printf("1. 종합이용권\n");
			printf("2. 파크이용권\n>> ");
			scanf("%d",&ticketType);
			printf("\n");
			
			if (ticketType>2) {
				printf("보기에서 선택해주세요.\n\n");			
			} 
	} while(ticketType>2);
}

void selectDayNightTicket() {
	do {
			printf("권종을 선택하세요.\n");
			printf("1. 주간권(1Day)\n");
			printf("2. 야간권(After4)\n>> "); 
			scanf("%d",&ticketDayType);
			printf("\n");
			
			if (ticketDayType>2) {
				printf("보기에서 선택해주세요.\n\n");			
			} 
	} while(ticketDayType>2);
}	

void inputID() {
	do {
			printf("주민번호를 입력하세요.\n>> ");
			scanf("%s", num);
			
			if (strlen(num)!=13) {
				printf("양식에 맞춰 입력해주세요.(13자리)\n\n");			
			} 
	} while(strlen(num)!=13);	
}

void ticketAmount() {
	do {
			printf("티켓을 몇 장 구매하시겠습니까?(최대 10개)\n>> ");
			scanf("%d",&amount);
			printf("\n");
			
			if (amount>10) {
				printf("최대 10장까지 구매할 수 있습니다.\n\n");			
			} 
	} while(amount>10);
}

void selectPreference_COMP() {
	do {
			printf("우대사항을 선택하세요.\n");
			printf("1. 없음(나이 우대는 자동 처리))\n");
			printf("2. 장애인\n"); 
			printf("3. 국가유공자\n"); 
			printf("4. 휴가장병\n"); 
			printf("5. 임산부\n"); 
			printf("6. 다둥이행복카드 소지자\n"); 
			scanf("%d",&preferenceType);
			printf("\n");
			
			if (preferenceType>6) {
				printf("보기에서 선택해주세요.\n\n");			
			} 
	} while(preferenceType>6);	
}

void selectPreference_PARK() {
	do {
			printf("우대사항을 선택하세요.\n");
			printf("1. 없음(나이 우대는 자동 처리))\n");
			printf("2. 장애인\n"); 
			printf("3. 국가유공자\n"); 
			printf("4. 휴가장병\n"); 
			scanf("%d",&preferenceType);
			printf("\n");
			
			if (preferenceType>4) {
				printf("보기에서 선택해주세요.\n\n");			
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
											
	printf("가격은 %d원 입니다.\n",resultPrice);
	printf("감사합니다.\n\n");
					

	printf("계속 발권 하시겠습니까?\n");
	printf("1. 티켓발권\n");
	printf("2. 종료\n>> ");
	scanf("%d", &isExit);	
	printf("\n");
			
}

void printBill() {
	printf("--------------------**************************-----------------------\n");	
	printf("--------------------***L O T T E  W O R L D***-----------------------\n");
	printf("--------------------**************************-----------------------\n");
	printf("  이 용 권    권 종     신 분    개 수    가 격     할 인 적 용   \n");
	printf("---------------------------------------------------------------------\n");
	
	for(int index=0; index < orderCount; index ++) {	
		printf(" %5s ",(orderList[index][0]==1)?"종합이용권":"파크이용권");					
		printf(" %5s ",(orderList[index][1]==1)?"주간권":"야간권");					
		printf(" %7s ",agegroupConverter(orderList[index][2]));		
		printf("%6d개 ", orderList[index][3]);		
		printf("%8d원 ",orderList[index][4]);		
		printf(" %10s\n",preferenceTypeConverter(orderList[index][5]));
	}
	
	printf("\n");
	printf("입장료 총액은 %d원입니다. \n\n", totalPrice);	
	totalPrice = 0;	
	printf("-----------------------------------------------------------------\n");
	printf("계속 진행(1: 새로운 주문, 2: 프로그램 종료)\n>> ");
	scanf("%d",&newOrderCount);
	printf("\n");
}

void oneRoundBill() {
	
	while(newOrderCount==1) {
		printf("===================== WELCOME TO LOTTE WORLD =====================\n\n");
		orderCount = 0;
		do {				
				while(true) {
					selectTicket();						//이용권 종류 선택하기			
					selectDayNightTicket();				//주간,야간권 선택하기 
						
					inputID(); 							//주민등록번호(13자리)입력받기 
					int age = ageCal(num);				//주민번호로 나이 추출하기 
					int agegroup = calAgeGroup(age);	//나이그룹(노인,청소년....)추출하기	
					printf("\n");
					
					ticketAmount();						//티켓의 매수 
					
					if(ticketType==1) {
						selectPreference_COMP();		//종합 할인가능한 우대조건 
					} else if(ticketType==2) {
						selectPreference_PARK();		//파크권 구매 시 할인 가능한 우대조건 
					}
																	
					price = calPriceProcess(ticketType, ticketDayType, agegroup);
					price = calDiscount(price, preferenceType);	
					resultPrice = price*amount;	//최종 
					totalPrice = totalPrice + resultPrice;
	  
					saveArray(agegroup);				
											
					if(isExit==2) {
					printf("발권을 종료합니다. 감사합니다.\n\n");	
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

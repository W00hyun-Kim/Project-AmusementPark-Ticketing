#include <stdio.h>
#include <time.h>
#include <string.h>    // strcmp 함수가 선언된 헤더 파일
#include <stdlib.h>

int ticketType,ticketDayType,preferenceType,amount,price,resultPrice,isExit;

//종합 이용권 가격 
const int COMP_BABY_PRICE = 0,
  		  COMP_ADULT_DAY_PRICE = 62000, COMP_ADULT_NIGHT_PRICE = 50000, 
		  COMP_TEEN_DAY_PRICE = 54000, COMP_TEEN_NIGHT_PRICE = 43000,
		  COMP_KID_DAY_PRICE = 47000, COMP_KID_NIGHT_PRICE = 36000,
		  COMP_OLD_DAY_PRICE = 47000, COMP_OLD_NIGHT_PRICE = 36000;

//파크 이용권 가격
const int PARK_BABY_PRICE = 0,
  		  PARK_ADULT_DAY_PRICE = 59000, PARK_ADULT_NIGHT_PRICE = 47000, 
		  PARK_TEEN_DAY_PRICE = 52000, PARK_TEEN_NIGHT_PRICE = 41000,
		  PARK_KID_DAY_PRICE = 46000, PARK_KID_NIGHT_PRICE = 35000,
		  PARK_OLD_DAY_PRICE = 46000, PARK_OLD_NIGHT_PRICE = 35000;  
	
//ID 번호 분석
char num[13];

const int OLD_GENERATION = 1900, NEW_GENERATION = 2000,
		  MALE_OLD = 1, FEMALE_OLD = 2, MALE_NEW = 3, FEMALE_NEW = 4;
			  
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
		year = 2000 + yearIndex;
	} else {
		year = 1900 + yearIndex ; 
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
	int price;
	
	if(ticketType==1) {
		if(ticketDayType==1) {
			switch(agegroup) {
				case 1 : {	//baby
					price = 0;
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
		} else if(ticketDayType==2) {	//night일때 
			switch(agegroup) {
				case 1 : {	//baby
					price = 0;
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
	} else if(ticketType==2) {			//park권일때  
		if(ticketDayType==1) {			//day 일 때 
			switch(agegroup) {
				case 1 : {	//baby
					price = 0;
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
		} else if(ticketDayType==2) {	//night일때 
			switch(agegroup) {
				case 1 : {	//baby
					price = 0;
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



int main() {
	do {			
		while(true) {
			if(isExit==2) {
				printf("발권을 종료합니다. 감사합니다.");
				break;
			}
			printf("이용권 타입을 선택하세요.\n");
			printf("1. 종합이용권\n");
			printf("2. 파크이용권\n");
			scanf("%d",&ticketType);
			
			printf("권종을 선택하세요.\n");
			printf("1. 주간권\n");
			printf("2. 야간권\n"); 
			scanf("%d",&ticketDayType);
				
			printf("주민번호를 입력하세요.\n");
			scanf("%s", num);	
			int age = ageCal(num);
			int agegroup = calAgeGroup(age);	
			
			printf("몇개를 주문하시겠습니까?(최대 10개)\n");
			scanf("%d",&amount);
			
						
			printf("우대사항을 선택하세요.\n");
			printf("1. 없음(나이 우대는 자동 처리))\n");
			printf("2. 장애인\n"); 
			printf("3. 국가유공자\n"); 
			printf("4. 다자녀\n"); 
			printf("5. 임산부\n"); 
			printf("6. 휴가장병\n"); 
			scanf("%d",&preferenceType);
				
			price = calPriceProcess(ticketType, ticketDayType, agegroup);
			price = calDiscount(price, preferenceType);	
			resultPrice = price*amount;	//최종 
						
			printf("가격은 %d원 입니다.\n",resultPrice);
			printf("감사합니다.\n\n");
			
			
//			 = {(ticketType==1)?"종합이용권":"파크이용권",(ticketDayType==1)?"주간권":"야간권", amount,
//									agegroupConverter(agegroup),resultPrice,preferenceTypeConverter(preferenceType)};
									
			//{(ticketType==1)?"종합이용권":"파크이용권"};					
			
			
			printf("계속 발권 하시겠습니까?\n");
			printf("1. 티켓발권\n");
			printf("2. 종료\n");
			scanf("%d", &isExit);		
		}
	} while(isExit == 1) ;
	
	printf("-----------------LOTTE WORLD---------------------");
	
	
		
	
}

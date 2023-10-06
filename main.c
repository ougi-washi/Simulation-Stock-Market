#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Market{
	int stocks;
	float stock_price;
};

struct Trader{
	float money;
	int stocks;
	float greed;
	int intrest;
	int money_spent;
};

void buy(struct Market *m,struct Trader *t,int stock);
void sell(struct Market *m,struct Trader *t,int stock);
void intrest_reg(struct Trader *t,struct Market *m);

int main(void){
	// preferably number of traders should be power of 10
	int number_of_traders = 1000;
	int money_given = 10000;
	
	struct Trader traders [number_of_traders];
	
	for (int i=0;i<number_of_traders;i++){
		traders[i].money = money_given;
		traders[i].greed = ((((i)-((i)%(number_of_traders/10)))/(number_of_traders/10))+1);
		traders[i].intrest = 0;
		traders[i].stocks= 0.0;
		traders[i].money_spent= 0;
	}
	struct Market market;
	market.stocks = 2000000;
	market.stock_price = 10.0;
	
	int transactions = 0;
	
	FILE *ptr = fopen("StockPrices.txt","w");
	
	int money_invested;
	
	while ((market.stock_price>0.0001)&&(transactions<40)){
		
		//Stocks bought shouldn't exceed stocks available
		do {
			//money invested is random taking greed as reference 
			//0 < greed < 10
			//0 < (greed / 10) < 1
			money_invested = rand()%(int)(traders[transactions%number_of_traders].money*(traders[transactions%number_of_traders].greed/10));
		}while (((int)floor(money_invested/(market.stock_price))>market.stocks));
		
		int stocks_bought = (int)floor(money_invested/(market.stock_price));
		float stored_value = market.stock_price;
		char str[10];
		sprintf(str, "%f", market.stock_price);
		fprintf(ptr,str);
		fprintf(ptr,"\n");
		
		if (market.stocks>0){
			buy(&market,&traders[transactions%number_of_traders],stocks_bought);
		}
		
		for(int i =0; i <number_of_traders; i++){
			intrest_reg(&traders[i],&market);
		}
		
		for(int i =0; i <number_of_traders; i++){
			if (((traders[i].intrest>=traders[i].greed)&&(traders[i].intrest!=0))||(traders[i].intrest<=-traders[i].greed)&&(traders[i].intrest!=0)){
				sell(&market,&traders[i],traders[i].stocks);
				
			}
			
		}
		
		transactions +=1;
		
	}
	
	fclose(ptr);
	
	system("manim -pql visual.py StockPricesCurve");
	
}


void buy(struct Market *m,struct Trader *t,int stock){
	m->stocks -= stock;
	t->stocks += stock;
	t->money -= stock * m->stock_price;
	for (int i =0; i<stock;i++){
		m->stock_price = m->stock_price*(1.005);
	}
	t->intrest = 0;
	t->money_spent = stock* m->stock_price;
}

void sell(struct Market *m,struct Trader *t,int stock){
	m->stocks += stock;
	t->stocks -= 0;
	t->money += stock * m->stock_price;
	for (int i =0; i<stock;i++){
		m->stock_price = m->stock_price*(0.995);
	}
	t->intrest = 0;
}

void intrest_reg(struct Trader *t,struct Market *m){
	t->intrest = (((t->stocks)*(m->stock_price)-(t->money_spent))/(t->money_spent));
}
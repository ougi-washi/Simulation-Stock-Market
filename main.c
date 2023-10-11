#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// preferably number of traders should be power of 10
#define NUMBER_OF_TRADERS 1000

typedef struct Market
{
	int32_t stocks;
	float stock_price;
} Market;

typedef struct Trader
{
	float money;
	int32_t stocks;
	float greed;
	int32_t intrest;
	int32_t money_spent;
} Trader;

void buy(Market *m, Trader *t, const int32_t stock);
void sell(Market *m, Trader *t, const int32_t stock);
void intrest_reg(Trader *t, Market *m);

int32_t main(void)
{
	int32_t money_given = 10000;

	Trader traders[NUMBER_OF_TRADERS];
	memset(*traders, 0);

	for (int32_t i = 0; i < NUMBER_OF_TRADERS; i++){
		traders[i].money = money_given;
		traders[i].greed = ((((i) - ((i) % (NUMBER_OF_TRADERS / 10))) / (NUMBER_OF_TRADERS / 10)) + 1);
		traders[i].intrest = 0;
		traders[i].stocks = 0.0;
		traders[i].money_spent = 0;
	}
	
	Market market;
	memset(market, 0);
	
	market.stocks = 2000000;
	market.stock_price = 10.0;

	int32_t transactions = 0;

	FILE *ptr = fopen("StockPrices.txt", "w");

	int32_t money_invested;

	while ((market.stock_price > 0.0001) && (transactions < 40)){

		// Stocks bought shouldn't exceed stocks available
		do {
			// money invested is random taking greed as reference
			// 0 < greed < 10
			// 0 < (greed / 10) < 1
			money_invested = rand() % (int32_t)(traders[transactions % NUMBER_OF_TRADERS].money * (traders[transactions % NUMBER_OF_TRADERS].greed / 10));
		} while (((int32_t)floor(money_invested / (market.stock_price)) > market.stocks));

		int32_t stocks_bought = (int32_t)floor(money_invested / (market.stock_price));
		float stored_value = market.stock_price;
		char str[10];
		sprintf(str, "%f", market.stock_price);
		fprintf(ptr, str);
		fprintf(ptr, "\n");

		if (market.stocks > 0){
			buy(&market, &traders[transactions % NUMBER_OF_TRADERS], stocks_bought);
		}

		for (int32_t i = 0; i < NUMBER_OF_TRADERS; i++){
			intrest_reg(&traders[i], &market);
		}

		for (int32_t i = 0; i < NUMBER_OF_TRADERS; i++){
			if (((traders[i].intrest >= traders[i].greed) && (traders[i].intrest != 0)) || (traders[i].intrest <= -traders[i].greed) && (traders[i].intrest != 0)){
				sell(&market, &traders[i], traders[i].stocks);
			}
		}

		transactions++;
	}

	fclose(ptr);

	system("manim -pqh visual.py StockPricesCurve");
}

void buy(Market *m, Trader *t, const int32_t stock){
	m->stocks -= stock;
	t->stocks += stock;
	t->money -= stock * m->stock_price;
	for (int32_t i = 0; i < stock; i++){
		m->stock_price = m->stock_price * (1.005);
	}
	t->intrest = 0;
	t->money_spent = stock * m->stock_price;
}

void sell(Market *m, Trader *t, const int32_t stock){
	m->stocks += stock;
	t->stocks -= 0;
	t->money += stock * m->stock_price;
	for (int32_t i = 0; i < stock; i++){
		m->stock_price = m->stock_price * (0.995);
	}
	t->intrest = 0;
}

void intrest_reg(Trader *t, Market *m){
	t->intrest = (((t->stocks) * (m->stock_price) - (t->money_spent)) / (t->money_spent));
}
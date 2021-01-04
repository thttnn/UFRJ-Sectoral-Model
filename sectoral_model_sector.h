/*****EXOGENOUS VARIABLES*****/

EQUATION("Sector_Effective_Orders")
/*
Sector Variable.
Semi-Endogenous Real Demand
Exogenous fixed rate of growth + quality growth + random shock
*/
	v[0]=VL("Sector_Effective_Orders",1);																//effective orders in the last period
	v[1]=V("real_demand_growth");																		//exogenous rate of growth									
	v[2]=V("demand_shock_standard_deviation");															//sd of demand shocks, set to zero if no demand shocks
	v[3]=V("demand_shock_average");                     												//average demand shock, baseline=0;
	v[4]=norm(v[3], v[2]);																				//demand shock at each time step
	v[5]=V("elasticity_quality");
	v[6]=VL("Sector_Avg_Quality", 1);																	//average quality in the last period
	v[7]=VL("Sector_Avg_Quality", 2);																	//average quality in the last period in t-2
	if(v[7]!=0)
		v[8]=(v[6]-v[7])/v[7];
	else
		v[8]=1;
	v[9]=v[0]*(1+v[1]+v[5]*v[8]+v[4]);
RESULT(v[9])


EQUATION("Price_Capital_Goods")
/*
Price of Capital Goods
One of the first variables to be computated.
It is adjusted annually in four possible ways:
1-By sector's price growth
2-By an exogenous growth rate
3-By a combination of both.
4-Not adjusted, fixed.
User Instructions: 
To use only productivity growth, set "capital_price_growth" paramater to zero and "capital_price_inflation_adjustment" above zero (as desired).
To use only exogenous growth, set "capital_price_growth" paramater above zero (as desired) and "capital_price_inflation_adjustment" to zero.
To use a combination of both, set both parameters as desired. 
To use it fixed, set both parameters to zero.
*/
	v[0]=VL("Price_Capital_Goods",1);                                                          	 	//price of capital goods in the last period
	v[1]=V("annual_period"); 																		//annual period parameter (baseline=4)
	v[2]= fmod((double) t,v[1]);                                                         			//divide the time period by the annual period parameter
	if(v[2]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust capital price)
		{
		v[3]=V("capital_price_growth");																//exogenous growth of capital price
		v[4]=V("capital_price_inflation_adjustment");												//how capital price is adjusted based on sector inflation
		v[6]=VL("Sector_Avg_Price", 1);                                								//sector average price in the last period
		v[7]=VL("Sector_Avg_Price", (v[1]+1));                        								//sector average price five periods before
		if(v[7]!=0)
			v[8]=(v[6]-v[7])/v[7];                                                      			//annual growth of price index (annual inflation)
		else
			v[8]=0;
		v[5]=v[0]*(1+v[3]+v[8]*v[4]);                                              					//current price will be the last period's multiplied by a rate of growth which is a combination of endogenous and exogenous growth rates
		}
	else                                                                             	 			//if the rest of the division is not zero, do not adjust wages
		v[5]=v[0];                                                                       			//current capital price will be the last period's
RESULT(v[5])


EQUATION("Price_Inputs")
/*
Price of Inputs
One of the first variables to be computated.
It is adjusted annually in four possible ways:
1-By sector' price growth
2-By an exogenous growth rate
3-By a combination of both above.
4-Not adjusted, fixed. 
User Instructions: 
To use only productivity growth, set "input_price_growth" paramater to zero and "input_price_inflation_adjustment" above zero (as desired).
To use only exogenous growth, set "input_price_growth" paramater above zero (as desired) and "input_price_inflation_adjustment" to zero.
To use a combination of both, set both parameters as desired. 
To use it fixed, set both parameters to zero.
*/
	v[0]=VL("Price_Inputs",1);                                                          	 		//input price in the last period
	v[1]=V("annual_period");																		//annual period parameter (baseline=1)
	v[2]= fmod((double) t,v[1]);                                                         			//divide the time period by the annual period parameter
	if(v[2]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust wages)
		{
		v[3]=V("input_price_growth");																//exogenous growth of input price
		v[4]=V("input_price_inflation_adjustment");													//how input price is adjusted based on sector inflation
		v[6]=VL("Sector_Avg_Price", 1);                                								//sector average price in the last period
		v[7]=VL("Sector_Avg_Price", (v[1]+1));                        								//sector average price five periods before
		if(v[7]!=0)
			v[8]=(v[6]-v[7])/v[7];                                                      			//annual growth of price index (annual inflation)
		else
			v[8]=0;
		v[5]=v[0]*(1+v[3]+v[8]*v[4]);                                              					//current price will be the last period's multiplied by a rate of growth which is a combination of endogenous and exogenous growth rates
		}
	else                                                                             	 			//if the rest of the division is not zero, do not adjust wages
		v[5]=v[0];                                                                       			//current input price will be the last period's
RESULT(v[5])

/*****SECTOR AVERAGES AND MAX*****/

EQUATION("Sector_Avg_Competitiveness")
/*
Average competitiveness, weighted by firm's market share
*/
	v[0]=0;                                         	//initializes the CYCLE
	CYCLE(cur, "FIRMS")                             	//CYCLE trought all firms in the sector
	{
		v[1]=VS(cur, "Firm_Competitiveness");           //firm's competitiveness
		v[2]=VLS(cur, "Firm_Market_Share", 1);          //firm's market share in the last period
		v[0]=v[0]+v[1]*v[2];                          	//sector average competitiveness will be a average of firms competitiveness weighted by their respective market shares
	}
RESULT(v[0])

EQUATION("Sector_Avg_Price")
RESULT(WHTAVE("Firm_Price", "Firm_Market_Share"))

EQUATION("Sector_Avg_Wage")
RESULT(WHTAVE("Firm_Wage", "Firm_Market_Share"))

EQUATION("Sector_Avg_Markup")
RESULT(WHTAVE("Firm_Effective_Markup", "Firm_Market_Share"))

EQUATION("Sector_Avg_Debt_Rate")
RESULT(WHTAVE("Firm_Debt_Rate", "Firm_Market_Share"))

EQUATION("Sector_Avg_Desired_Debt_Rate")
RESULT(WHTAVE("Firm_Desired_Debt_Rate", "Firm_Market_Share"))

EQUATION("Sector_Avg_Quality")
RESULT(WHTAVE("Firm_Quality", "Firm_Market_Share"))

EQUATION("Sector_Avg_Productivity")
RESULT(WHTAVE("Firm_Avg_Productivity", "Firm_Market_Share"))

EQUATION("Sector_Max_Productivity")          
RESULT(MAX("Firm_Frontier_Productivity"))

EQUATION("Sector_Max_Quality")
RESULT(MAX("Firm_Quality"))

EQUATION("Sector_Sales")                                                            
RESULT(SUM("Firm_Sales"))

/*****SECTOR AGGREGATES*****/

EQUATION("Sector_Inventories")                               
RESULT(SUM("Firm_Stock_Inventories"))

EQUATION("Sector_Effective_Production")                     
RESULT(SUM("Firm_Effective_Production"))

EQUATION("Sector_Productive_Capacity")                    
RESULT(SUM("Firm_Productive_Capacity"))

EQUATION("Sector_Sum_Market_Share")
RESULT(SUM("Firm_Market_Share"))

EQUATION("Sector_Stock_Loans")
RESULT(SUM("Firm_Stock_Loans"))

EQUATION("Sector_Stock_Deposits")
RESULT(SUM("Firm_Stock_Deposits"))

EQUATION("Sector_Indirect_Tax")
RESULT(SUM("Firm_Indirect_Tax"))

EQUATION("Sector_Profits")
RESULT(SUM("Firm_Net_Profits"))


EQUATION("Sector_Wages")
/*
Sum of wages of firms in the sector
*/
	v[1]=0;
	CYCLE(cur1, "FIRMS")                               				//CYCLE trought all firms of the sector
		{
			v[2]=VS(cur1, "Firm_Wage");                             //firm's wage
			v[3]=VS(cur1, "Firm_Effective_Production");             //firm's effective production
			v[4]=VS(cur1, "Firm_Avg_Productivity");            		//firm's productivity 
			v[5]=VS(cur1, "Firm_RND_Expenses");                     //firm's rnd expeses, returned as salary to researchers
			if (v[4]!=0)
				v[1]=v[1]+v[3]*(v[2]/v[4])+v[5];                   	//sums up all firms' wage, determined by a unitary wage (sectorial wage divided by firm's productivity) multiplied by firm's effective production plus RND expenses
			else
				v[1]=v[1]+v[5];
		}
RESULT(v[1])


EQUATION("Sector_Income")
/*
Sum of wages and profits
*/
	v[0]=V("Sector_Wages");
	v[1]=V("Sector_Profits");
	v[2]=v[0]+v[1];
RESULT(v[2])


/*****SECTOR ANALYSIS VARIABLES*****/


EQUATION("Sector_Profit_Rate")
/*
Sector Variable for Analysis
*/
	v[0]=0;                           		//initializes the CYCLE    
	CYCLE(cur, "FIRMS")               		//CYCLE trought all firms of the sector
	{
		v[1]=VS(cur, "Firm_Net_Profits");  	//firm's profits
		v[2]=VS(cur, "Firm_Capital");		//firm's capital
		if(v[2]!=0)
			v[0]=v[0]+(v[1]/v[2]);          //sums up firm's profit rate
		else
			v[0]=v[0];
	}
	v[3]=COUNT("FIRMS");
	if (v[3]!=1)
		v[4]=v[0]/v[3];
	else	
		v[4]=0;
RESULT(v[4])


EQUATION("Sector_Normalized_HHI")
/*
Sector Variable for Analysis
*/
	v[0]=0;                           		//initializes the CYCLE    
	CYCLE(cur, "FIRMS")               		//CYCLE trought all firms of the sector
	{
		v[1]=VS(cur, "Firm_Market_Share");  //firm's market share
		v[0]=v[0]+v[1]*v[1];            	//sums up firm's market share squared
	}
	v[2]=COUNT("FIRMS");
	if (v[2]!=1)
		v[3]=(v[0]-(1/v[2]))/(1- (1/v[2]));
	else	
		v[3]=1;
RESULT(v[3])


EQUATION("Sector_Turbulence")
/*
Sector Variable for Analysis
*/
	v[0]=0;                                           //initializes the CYCLE 
	CYCLE(cur, "FIRMS")                               //CYCLE trough all firms 
	{
	v[1]=VS(cur,"firm_date_birth");                   //firm's date of birth
 	if(v[1]==(double)t)                               //if the time period is the same of the firm's date of birth
 		v[4]=0;                                       //use zero
 	else                                              //if the time period is no the same of the firm's date of birth
 		{
 		v[2]=VS(cur,"Firm_Effective_Market_Share");   //firm's effective market share in current period
 		v[3]=VLS(cur,"Firm_Effective_Market_Share",1);//firm's effective market share in the last period
 		v[4]=abs(v[2]-v[3]);                          //returns the absolute value of the difference
 		}
 	v[0]=v[0]+v[4];                                   //sums up all absolute values for all firms
	}
RESULT(v[0])


EQUATION("Sector_Demand_Met")
/*
Percentage of demand fulfilled by each sector
*/
	v[0]=V("Sector_Effective_Orders");                     	//total effective orders of the sector
	v[1]=V("Sector_Sales");                                	//total sales of the sector
  	if(v[0]>0)                                           	//if effective orders is positive
    	v[2]=v[1]/v[0];                                    	//the percentage of the demand met by the sector will be the total sales over effective orders
  	else                                                 	//if effective orders is not positive
    	v[2]=1;                                            	//the percentage of the demand met by the sector is 100%
RESULT(v[2])


EQUATION("Sector_Capacity_Utilization")
/*
Sector Capacity Utilization
*/
	v[0]=VL("Sector_Productive_Capacity",1);    
	v[1]=V("Sector_Effective_Production"); 
	if(v[0]!=0)
		v[2]=v[1]/v[0];
	else
		v[2]=0;                    
RESULT(v[2])


EQUATION("Sector_Income_Growth");
/*
Annual growth of generated nominal income by the sector
*/
if(t>50)
	{
	v[0]=V("annual_period");
	v[1]=0;
	for (i=0; i<=(v[0]-1); i++)
		{
		v[2]=VL("Sector_Income", i);
		v[1]=v[1]+v[2];
		}
	v[3]=0;
	for (i=v[0]; i<=(2*v[0]-1); i++)
		{
		v[4]=VL("Sector_Income", i);
		v[3]=v[3]+v[4];
		}
	if (v[3]!=0)
		v[5]=(v[1]-v[3])/v[3];
	else
		v[5]=1;	
	}
else
	v[5]=0;
USE_SAVED
RESULT(v[5])


EQUATION("Sector_Inflation")
/*
Annual growth of the sector average price
*/
if(t>50)
	{
	v[0]=V("annual_period");
	v[1]=VL("Sector_Avg_Price",1);
	v[2]=VL("Sector_Avg_Price",(v[0]+1));
	v[3]=(v[1]/v[2])-1;
	}
else
	v[3]=0;
RESULT(v[3])


EQUATION("Sector_Employment")
/*
Sum up firm's employment, given by firm's effective production over firm's avg productivity
*/
	v[0]=0;                                        		//initializes the CYCLE
	CYCLE(cur, "FIRMS")                            		//CYCLE trought the firms
	{
		v[1]=VS(cur, "Firm_Effective_Production");      //firm's effective production
		v[2]=VS(cur, "Firm_Avg_Productivity");   		//firm's productivity in the last period
		if(v[2]!=0)
			v[0]=v[0]+v[1];                       		//sums up the ratio between effective production and productivity
		else
			v[0]=v[0];
	}
RESULT(v[0])




//****************************************FIRM PRICE AND COSTS*************************************//

EQUATION("Price_Capital_Goods")
/*
Price of Capital Goods
One of the first variables to be computated.
It is adjusted annually in four possible ways:
1-By productivity growth
2-By an exogenous groth rate
3-By a combination of both.
4-Not adjusted, fixed.
User Instructions: 
To use only productivity growth, set "capital_price_growth" paramater to zero and "capital_price_passthrough_productivity" above zero (as desired).
To use only exogenous growth, set "capital_price_growth" paramater above zero (as desired) and "capital_price_passthrough_productivity" to zero.
To use a combination of both, set both parameters as desired. 
To use it fixed, set both parameters to zero.
*/
	v[0]=VL("Price_Capital_Goods",1);                                                          	 	//price of capital goods in the last period
	v[1]=V("annual_period"); 																		//annual period parameter (baseline=4)
	v[2]= fmod((double) t,v[1]);                                                         			//divide the time period by the annual period parameter
	if(v[2]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust capital price)
		{
		v[3]=VL("Sector_Avg_Productivity", 1);                                           	 		//sector average productivity in the last period
		v[4]=VL("Sector_Avg_Productivity", (v[1]+1));                                      			//sector average productivity five periods before
		if(v[4]!=0)
			v[5]=(v[3]-v[4])/v[4];                                                       			//annual growth of sector average productivity
		else
			v[5]=1;
		v[6]=V("capital_price_passthrough_productivity");                                       	//passthrough of productivity parameter
		v[7]=V("capital_price_growth");																//exogenous growth of capital price
		v[8]=v[0]*(1+v[5]*v[6]+v[7]);                                              					//current price will be the last period's multiplied by a rate of growth which is a combination of endogenous and exogenous growth rates
		}
	else                                                                             	 			//if the rest of the division is not zero, do not adjust wages
		v[8]=v[0];                                                                       			//current capital price will be the last period's
RESULT(v[8])


EQUATION("Price_Inputs")
/*
Price of Inputs
One of the first variables to be computated.
It is adjusted annually in four possible ways:
1-By productivity growth
2-By an exogenous groth rate
3-By a combination of both above.
4-Not adjusted, fixed. 
User Instructions: 
To use only productivity growth, set "input_price_growth" paramater to zero and "inputs_price_passthrough_productivity" above zero (as desired).
To use only exogenous growth, set "input_price_growth" paramater above zero (as desired) and "inputs_price_passthrough_productivity" to zero.
To use a combination of both, set both parameters as desired. 
To use it fixed, set both parameters to zero.
*/
	v[0]=VL("Price_Inputs",1);                                                          	 		//input price in the last period
	v[1]=V("annual_period");																		//annual period parameter (baseline=1)
	v[2]= fmod((double) t,v[1]);                                                         			//divide the time period by the annual period parameter
	if(v[2]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust wages)
		{
		v[3]=VL("Sector_Avg_Productivity", 1);                                           	 		//sector average productivity in the last period
		v[4]=VL("Sector_Avg_Productivity", (v[1]+1));                                      			//sector average productivity five periods before
		if(v[4]!=0)
			v[5]=(v[3]-v[4])/v[4];                                                       			//annual growth of sector average productivity
		else
			v[5]=1;
		v[6]=V("inputs_price_passthrough_productivity");                                       		//passthrough of productivity parameter
		v[7]=V("input_price_growth");																//exogenous growth of input price
		v[8]=v[0]*(1+v[5]*v[6]+v[7]);                                              					//current price will be the last period's multiplied by a rate of growth which is a combination of endogenous and exogenous growth rates
		}
	else                                                                             	 			//if the rest of the division is not zero, do not adjust wages
		v[8]=v[0];                                                                       			//current wages will be the last period's
RESULT(v[8])


EQUATION("Firm_Wage")
/*
Nominal Wage of the firm. It increases year by year depending on firm's avg productivity.
To let the nominal wage fixed and equal to all firms, set "wage_passthrough_productivity" as zero.
*/
	v[0]=VL("Firm_Wage",1);                                                          	 			//firm wage in the last period
	v[1]=V("annual_period");
	v[2]= fmod((double) t,v[1]);                                                         			//divide the time period by the annual period parameter
	if(v[2]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust wages)
		{
		v[3]=VL("Firm_Avg_Productivity", 1);                                           	 			//firm average productivity in the last period
		v[4]=VL("Firm_Avg_Productivity", (v[1]+1));                                     			//firm average productivity five periods before
		if(v[4]!=0)
			v[5]=(v[3]-v[4])/v[4];                                                      			 //annual growth of sector average productivity
		else
			v[5]=0;
		v[6]=VL("Sector_Avg_Price", 1);                                								//sector average price in the last period
		v[7]=VL("Sector_Avg_Price", (v[1]+1));                        								//sector average price five periods before
			v[8]=(v[6]-v[7])/v[7];                                                      			//annual growth of price index (annual inflation)
		v[9]=V("wage_passthrough_productivity");                                       				//pass through of productivity to wages
		v[10]=V("wage_passthrough_inflation");														//pass through of inflation to wages
		v[11]=v[0]*(1+v[5]*v[9]+v[8]*v[10]);                                            			//current wage 
		}
	else                                                                             	 			//if the rest of the division is not zero, do not adjust wages
		v[11]=v[0];                                                                       			//current wages will be the last period's
RESULT(v[11])


EQUATION("Firm_Variable_Cost")
/*
Variable unit cost is the wage cost (nominal wages over productivity) plus intermediate costs
*/
	v[1]=V("Firm_Wage");
	v[2]=VL("Firm_Avg_Productivity",1); 
	v[3]=V("input_tech_coefficient");                               			  
	v[4]=V("Price_Inputs");           
	if(v[2]!=0)
		v[5]=(v[1]/v[2])+(v[3]*v[4]);
	else
		v[5]=0;
RESULT(v[5])


EQUATION("Firm_Desired_Markup")
/*
Firm Variable
*/
	v[11]=V("markup_period");
	v[12]=V("id_firm_number");
	v[0]=VL("Firm_Desired_Markup",1);                                             					//strategic markup in the last period
	v[1]= fmod((double)t, v[11]);                                         						//divides the time period by 8

	if(v[1]==0)                                                              						//if the rest of the above division is zero, adjust strategic markup
		{
		v[2]=VL("Firm_Avg_Potential_Markup",1);                                     				//average potential markup of the firm in the last period        
		v[3]=V("market_share_expansion");                                           				//market-share expantion parameter
		v[4]=V("markup_apropriation");                                              				//profits apropriation parameter   
		v[5]=V("Firm_Desired_Market_Share");                                        				//desired market-share 
		v[6]=VL("Firm_Avg_Market_Share",1);                                    						//firm average market share in the last period   
		v[7]=VL("Firm_Competitiveness",1);                                          				//firm's competitiveness in the last period
		v[8]=VL("Sector_Avg_Competitiveness",1);                               						//sector's average competitiveness in the last period
		v[9]=v[3]*((v[7]-v[8])/v[8]);                                          						//how much to adjust based on the percentual diference between firm's competitiveness and sector's average competitiveness and the expantion parameter
		if(v[2]*(1+v[9])>v[0]&&v[6]>v[5])                                      						//if the already adjusted average potential markup is higher than desired strategic markup of the last period and firm's average market share is higher than desired market share
			v[10]=v[0]+v[4]*(v[2]*(1+v[9])-v[0]);                              						//the firm adjusts the strategic markup. 
		else                                                                   						//if the adjusted average potential markup is not higher than desired nor the firm's average market-share is not higher than desired
			v[10]=v[0];                                                        						//strategic markup will be the last period's                                            
		}
	else                                                                     						//if the rest of the above division is not zero, do not adjust strategic markup
		v[10]=v[0];                                                            						//strategic markup will be the last period's	
		
		
		
		if(v[1]==0)                                                                      	 			//if the rest of the above division is zero (beggining of the year, adjust wages)
		{
		v[23]=VL("Firm_Avg_Productivity", 1);                                           	 			//firm average productivity in the last period
		v[24]=VL("Firm_Avg_Productivity", (v[11]+1));                                     				//firm average productivity five periods before
		if(v[24]!=0)
			v[25]=(v[23]-v[24])/v[24];                                                      			//productivity growth
		else
			v[25]=0;
			
		v[26]=VL("Firm_Quality", 1);                                           	 						//firm quality in the last period
		v[27]=VL("Firm_Quality", (v[11]+1));                                     						//firm quality five periods before
		if(v[27]!=0)
			v[28]=(v[26]-v[27])/v[27];                                                      			//quality growth
		else
			v[28]=0;
		
		v[29]=V("wage_passthrough_productivity"); 
		v[30]=V("markup_passthrough_quality");
		v[31]=v[0]*(1+(1-v[29])*v[25]+v[30]*v[28]);														//firm increase markup if wages do not increase as productivity and if it had quality increase
		}
		else
		v[31]=v[0];
			
RESULT(v[31]) 


EQUATION("Firm_Price")
/*
Firm's effective price is a average between the desired price and the sector average price
The strategic_price_weight parameter determines how firms value their desired price. If set to 1, effective price will be desired price. 
*/
	v[0]=VL("Firm_Price",1);                                                   						//firm's price in the last period
	v[1]=V("Firm_Variable_Cost");                          											//firm's variable cost 
	v[2]=V("Firm_Desired_Markup");                         											//firm's desired markup
	v[3]=V("strategic_price_weight");                                          						//strategic weight parameter
	v[4]=VL("Sector_Avg_Price", 1);                                            						//sector average price in the last period
	v[5]=v[3]*(v[1]*v[2])+(1-v[3])*(v[4]);                                          				//firm's price is a average between the desired price and the sector average price
	if((v[1]*v[2])>0)                                                                 				//if desired price is positive
		v[6]=max(0.01,v[5]);                                                   						//firm's price can never be zero or lower
	else                                                                       						//if desired price is not positive
		v[6]=v[0];                                                             						//firm's price will be the last period's
	v[7]=V("indirect_tax_rate");
	v[8]=v[6]/(1-v[7]);
RESULT(v[8])


EQUATION("Firm_Effective_Markup")
/*
Effective Markup is the Effective Price over the Variable Cost
*/
	v[0]=V("Firm_Price");
	v[1]=V("Firm_Variable_Cost");
	if(v[1]!=0)
		v[2]=v[0]/v[1];
	else
		v[2]=0;
RESULT(v[2])


EQUATION("Firm_Desired_Market_Share")
/*
Desired Market Share is a simple average between last period's desired market share and firm's average market share
*/
	v[0]=V("markup_period");
	v[1]=VL("Firm_Desired_Market_Share", 1);                       									//desired market share in the last period
	v[2]=VL("Firm_Avg_Market_Share", 1);                        									//firm's average market share (desired)
	v[3]= fmod((double) t-1, v[0]);                               									//devides the last period by eight
	if(v[3]==0)                                                   									//if the rest of the above division is zero, adjust desired market share
		v[4]=(v[1]+v[2])/2;                                         								//current desired market share is a simple average between last period's desired market share and firm's average market share
	else                                                          									//if the rest of the above division is not zero, do not adjust desired market share
 		v[4]=v[1];                                                 									//firm's desired market share will be equal to the last period's
RESULT(v[4])


EQUATION("Firm_Avg_Market_Share")
/*
Average Market Share between the market share of the firm in the last markup period
*/
	v[0]=V("markup_period");
	v[3]=0;										   													//initializes the sum
	for (v[1]=0; v[1]<=(v[0]-1); v[1]=v[1]+1)														//from 0 to markup period-1 lags
		{
		v[2]=VL("Firm_Market_Share", v[1]);															//computates firm's market share of the current lag
		v[3]=v[3]+v[2];																				//sum up firm's lagged market share
		}
	v[4]=v[3]/v[0];																					//average firm's market share of the last investment period
RESULT(v[4])


EQUATION("Firm_Potential_Markup")
/*
Potential markup is the sector average price over firm's variable costs
*/
	v[0]=V("Sector_Avg_Price");                                       								//sector average price
	v[1]=V("Firm_Variable_Cost");                                     								//firm's variable cost
	if(v[1]!=0)                                                       								//if firm's variable cost is not zero
		v[2]=v[0]/v[1];                                               								//potential markup is the sector average price over firm's variable costs
	else                                                             								//if firm's variable cost is zero
		v[2]=0;                                                       								//potential markup is zero
RESULT(v[2])


EQUATION("Firm_Avg_Potential_Markup")
/*
Average Potential Markup between the potential markup of the firm in the last 8 periods
*/
	v[0]=V("markup_period");
	v[3]=0;																							//initializes the sum
	for (v[1]=0; v[1]<=(v[0]-1); v[1]=v[1]+1)														//from 0 to markup period-1 lags
		{
		v[2]=VL("Firm_Potential_Markup", v[1]);														//computates firm's potential markup of the current lag
		v[3]=v[3]+v[2];																				//sum up firm's lagged potential markup
		}
	v[4]=v[3]/v[0];																					//average firm's market share of the last potential markup
RESULT(v[4])


EQUATION("Firm_Market_Share")
/*
Firm Variable
*/
	v[0]=VL("Firm_Market_Share", 1);                												//firm's market share in the last period
	v[1]=V("Sector_Avg_Competitiveness");          													//sector average competitiveness
	v[3]=V("Firm_Competitiveness");                 												//firm's competitiveness
	if(v[1]!=0)                                     												//if the sector average competitiveness is not zero
		v[4]=v[0]+v[0]*((v[3]/v[1])-1);             												//firm's market share will be the last period's inscresed by the adjustment paramter times the ratio between firm's competitiveness and sector average competitiveness
	else                                            												//if the sector average competitiveness is zero
		v[4]=0;                                     												//firm's market share will be zero (testar, remover)

RESULT(v[4])


EQUATION("Firm_Competitiveness")
/*
Competitiveness depends on the quality of the product, the price and the delivery delay of the firm
*/
	v[0]=VL("Firm_Price",1);                                           								//firm's price in the last period
	v[1]=V("elasticity_price");                                        								//price elasticity
	v[2]=VL("Firm_Quality",1);                                         								//product quaility
	v[3]=V("elasticity_quality");                                      								//quality elasticity
	v[4]=VL("Firm_Competitiveness",1);                                 								//firm's competitiveness in the last period
	v[5]=VL("Firm_Delivery_Delay",1);                                  								//firm's delivery delay in the last period
	v[6]=V("elascitity_delay");                                       								//delay elasticity	
   	if(v[0]!=0&&v[2]!=0&&v[5]!=0)                                               					//if the price was not zero neither the quality
     	v[7]=(pow(v[2],v[3]))*(1/pow(v[0],v[1]))*(1/pow(v[5],v[6]));   								//firm's competitiveness will be given by the quality powered by its elasticity over the price, powered by its elasticicty, and the delivery delay, powered by its elasticicty
   	else                                                               								//if either the price or the quality was zero 
     	v[7]=v[4];                                                     								//firm's competitiveness will be the competitiveness in the last period
RESULT(v[7])

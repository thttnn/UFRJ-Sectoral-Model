//****************************************FIRM PRICE AND COSTS*************************************//

EQUATION("Firm_Desired_Markup")
/*
Firm Variable
*/
	v[0]=V("markup_period");								//sector markup period parameter
	v[1]=fmod((t+v[0]),v[0]);								//devides the current time step by the markup period and takes the rest
	v[2]=V("id_firm_number");								//firm number
	v[3]=fmod((v[2]+v[0]),v[0]);							//divides the firm number by the investment period and takes the rest
	v[4]=VL("Firm_Desired_Markup",1);                       //strategic markup in the last period
	v[5]=V("Firm_Desired_Market_Share");                    //desired market-share 
	v[6]=VL("Firm_Avg_Market_Share",1);                     //firm average market share in the last period   
	if (v[3]==v[1] && v[5]<v[6])							//if the firm number rest matches the time step rest, its markup adjustment period for that firm
		{
		v[7]=VL("Firm_Quality", 1);                        	//firm quality in the last period
		v[8]=VL("Sector_Avg_Quality", 1);                  	//sector average quality
		if(v[8]!=0)
			v[9]=(v[7]-v[8])/v[8];                      	//quality difference
		else
			v[9]=0;
		
		v[10]=V("markup_passthrough_quality");
		v[11]=max(v[4],(v[4]*(1+v[10]*v[9])));				//firm increase markup if wages do not increase as productivity and if it had quality increase
		}												
	else													//if the rests do not match, do not ajust markup
		v[11]=v[4];												
RESULT(v[11])


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
			v[5]=(v[3]-v[4])/v[4];                                                      			//annual growth of sector average productivity
		else
			v[5]=0;
		
		v[6]=VL("Sector_Avg_Price", 1);                                								//sector average price in the last period
		v[7]=VL("Sector_Avg_Price", (v[1]+1));                        								//sector average price five periods before
		if(v[7]!=0)
			v[8]=(v[6]-v[7])/v[7];                                                      			//annual growth of price index (annual inflation)
		else
			v[8]=0;
		
		v[12]=VL("Sector_Employment", 1);                                							//sector employment in the last period
		v[13]=VL("Sector_Employment", (v[1]+1));                        							//sector employment five periods before
		if(v[13]!=0)
			v[14]=(v[12]-v[13])/v[13];                                                      		//annual growth of sector employment
		else
			v[14]=0;
		
		v[9]=V("wage_passthrough_productivity");                                       				//pass through of productivity to wages
		v[10]=V("wage_passthrough_inflation");														//pass through of inflation to wages
		v[15]=V("wage_passthrough_employment");														//pass through of employment to wages
		v[11]=v[0]*(1+v[5]*v[9]+v[8]*v[10]+v[15]*v[14]);                                            //current wage 
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
	v[14]=V("markup_passthrough_tax");
	v[8]=v[6]/(1-(v[14]*v[7]));
	
	v[9]=V("markup_period");								//sector markup period parameter
	v[10]=fmod((t+v[9]),v[9]);								//devides the current time step by the markup period and takes the rest
	v[11]=V("id_firm_number");								//firm number
	v[12]=fmod((v[11]+v[9]),v[9]);							//divides the firm number by the investment period and takes the rest
	if (v[10]==v[12])
		v[13]=v[8];
	else
		v[13]=v[0];
RESULT(v[13])


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
	v[0]=V("markup_period");								//sector markup period parameter
	v[1]=fmod((t+v[0]),v[0]);								//devides the current time step by the markup period and takes the rest
	v[2]=V("id_firm_number");								//firm number
	v[3]=fmod((v[2]+v[0]),v[0]);							//divides the firm number by the investment period and takes the rest
	v[4]=VL("Firm_Desired_Market_Share", 1);                       									//desired market share in the last period
	v[5]=VL("Firm_Avg_Market_Share", 1);                        									//firm's average market share (desired)
	if(v[3]==v[1])                                                   								//if the rest of the above division is zero, adjust desired market share
		v[6]=(v[4]+v[5])/2;                                         								//current desired market share is a simple average between last period's desired market share and firm's average market share
	else                                                          									//if the rest of the above division is not zero, do not adjust desired market share
 		v[6]=v[4];                                                 									//firm's desired market share will be equal to the last period's
RESULT(v[6])


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


EQUATION("Firm_Market_Share")
/*
Firm Variable
*/
	v[0]=VL("Firm_Market_Share", 1);                												//firm's market share in the last period
	v[1]=V("Sector_Avg_Competitiveness");          													//sector average competitiveness
	v[2]=V("sector_competitiveness_adjustment");										
	v[3]=V("Firm_Competitiveness");                 												//firm's competitiveness
	if(v[1]!=0)                                     												//if the sector average competitiveness is not zero
		v[4]=v[0]+v[2]*v[0]*((v[3]/v[1])-1);             												//firm's market share will be the last period's inscresed by the adjustment paramter times the ratio between firm's competitiveness and sector average competitiveness
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

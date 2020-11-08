//*****COSTS*****//

EQUATION("UDC")
/*
Unit Direct Cost (UDC) is the wage cost (nominal wages over productivity) plus intermediate costs
*/
	v[0]=V("Intermediate_Goods_Costs");
	v[1]=V("Wage");
	v[2]=V("Firm_Avg_Productivity");
	v[3]=(v[1]/v[2])+v[0];
RESULT(v[3])


EQUATION("Direct_Labour_Cost")
/*

*/
	v[0]=V("Wage");
	v[1]=V("Firm_Avg_Productivity");
	v[2]=v[0]/v[1];
RESULT(v[2])



EQUATION("Intermediate_Goods_Costs")
/*
Intermediate goods costs sums up the input costs of diferent sectors
*/
	v[0]=SUM("Input_Cost");
RESULT(v[0])


EQUATION("UIC") // Introduzido por João 30/05/2019
/*
Unit Indirect Cost (UIC)
*/
	v[0]=V("Productive_Capacity");                   	          	  //firm's productive capacity
	v[1]=V("productive_capacity_cost");                             //firm's capacity cost
	v[2]=VL("Consumer_Price_Index",1);
	v[3]=V("Effective_Production"); 																//Firm's effective production
	v[4]=V("Depreciation_Expenses");
	v[5]=V("imitation_revenue_proportion");           	            //share of revenue destinated to imitation 
	v[6]=V("innovation_revenue_proportion");            						//share of revenue destinanted to innovation
	v[7]=V("id_innovative_firm");                                   //identifies if the firm is innovative or not
	v[8]=V("quality_innovation_revenue_proportion");             		//share of revenue destinated to product innovation
	v[9]=V("quality_imitation_revenue_proportion");
	v[10]=V("Revenue");
	v[11]=VL("UIC",1);
	v[12]=v[0]*v[1]*v[2]/v[3];																			//Overhead Unit Costs
	v[13]=v[4]/v[3];																								//Depreciation Unit Cost
	v[14]=v[10]*(v[5]+v[6]*v[7]+v[8]+v[9])/v[3];										//R&D Unit Cost
	v[15]=v[12]+v[13]+v[14];
RESULT(v[15])


EQUATION("UC") // Introduzido por João 30/05/2019
/*
Unit Cost (UC)
*/
	v[0]=V("UDC");																									//Unit Direct Cost
	v[1]=V("UIC");																									//Unit Indirect Cost
	v[2]=v[0]+v[1];
RESULT(v[2])


EQUATION("NUC") // Introduzido por João 30/05/2019
/*
Normal Unit Cost (NUC)
*/
	v[1]=V("productive_capacity_cost");                             //firm's capacity cost
	v[2]=V("desired_degree_capacity_utilization");	 								//Desired Degree of Capacity Utilization
	v[3]=VL("Consumer_Price_Index",1);
	v[4]=(v[1]*v[3]/v[2]);																					//***Normal Unit Overhead Costs
	v[5]=V("UDC");																									//***Unit Direct Cost
	v[6]=V("depreciation_rate");                                    //depreciation rate of capital goods
	v[7]=VL("Price_Capital_Goods",1);                               //price of capital goods
	cur = SEARCH_CND("id_input_sector", 2);                         //search the inputs for the capital good sector one
	v[8]=VS(cur,"capital_output_ratio");                            //capital outpur ratio            
	v[9]=(v[6]/6)*v[8]*v[7]/v[2];                                   //***Normal Unit Depreciation Cost
	v[10]=V("imitation_revenue_proportion");           	            //share of revenue destinated to imitation 
	v[11]=V("innovation_revenue_proportion");            						//share of revenue destinanted to innovation
	v[12]=V("id_innovative_firm");                                  //identifies if the firm is innovative or not
	v[13]=V("quality_innovation_revenue_proportion");             	//share of revenue destinated to product innovation
	v[14]=V("quality_imitation_revenue_proportion");								//share of revenue destinated to product imitation
	v[15]=VL("Price",1);																						//Price is the unit revenue. The last price is used as a proxy to normal price.
	v[16]=v[15]*(v[10]+v[11]*v[12]+v[13]+v[14]);										//***Normal Unit R&D Cost	
	v[17]=v[4]+v[5]+v[9]+v[16];																			//***NUC
RESULT(v[17])
	


//*****PRICE*****//


EQUATION("Price")
/*
Firm's effective price is a average between the desired price and the sector average price with an downward rigidity
*/
	v[0]=V("M");                                                      
	v[1]=V("NUC");                                                  
	v[2]=V("theta");                                               //weight parameter
	v[3]=VL("Avg_Price", 1); 					                           //sector average price (including the foreign market) in the last period
	v[4]=v[0]*v[1]*v[2]+(1-v[2])*v[3];                                			         //firm's price is a average between the desired price and the sector average price
	v[5]=max(v[4],v[1]);
RESULT(v[5])

EQUATION("Open_Economy_Avg_Price")
/*
Firm Variable
*/
	v[0]=V("Avg_Price");
	v[1]=V("Foreign_Price");
	v[2]=V("Exchange_Rate");
	v[3]=v[1]*v[2];
	v[4]=V("Effective_Orders_Sector");
	v[5]=V("Exports");
	v[6]=v[5]/v[4];
	v[7]=(1-v[6])*v[0]+v[6]*v[3];
RESULT(v[7])



EQUATION("Exports_Weight")
	v[4]=V("Effective_Orders_Sector");
	v[5]=V("Exports");
	v[6]=v[5]/v[4];
RESULT(v[6])


//*****MARK-UP*****//

EQUATION("M")
/*
Firm Variable
*/
	v[0]=VL("M",1);
	v[1]=V("markup_freq");
	v[2]= fmod((double) t,v[1]);                                                          
	if(v[2]==0)
		{   
		v[3]=VL("Market_Share",1);
		v[4]=VL("Target_Market_Share",1);
		v[5]=V("adjust_m");
		v[6]=V("Firm_Avg_Sales");
		v[7]=VL("Firm_Avg_Sales",v[1]);
		if((v[3]>v[4] && v[6]>v[7]) || (v[3]<v[4] && v[6]<v[7]))
		v[8]=v[0]+v[0]*v[5]*((v[3]-v[4])/v[4]);	 
		else
		v[8]=v[0];
		} 		
	else
		v[8]=v[0];
		v[9]=min(v[8],10);
RESULT(v[9])


EQUATION("Orders_Growth")
/*
Comment
*/

	v[0]=V("markup_freq");
	v[1]=V("Effective_Orders"); 
	v[2]=VL("Effective_Orders",v[0]);
	v[3]=(v[1]-v[2])/v[2];
		
RESULT(v[3])


EQUATION("Target_Growth")
/*
Comment
*/                                       
	v[0]=VL("Target_Growth", 1);                           
	v[1]=V("markup_freq");
	v[2]= fmod((double) t,v[1]);                                   
	if(v[2]==0)                                                   
		{ 
		v[3]=VL("Orders_Growth", 1);                        
  	v[4]=(v[0]+v[3])/2;                                         
  	}
	else                                                          
 		v[4]=v[0];
  v[5]=max(v[4],0);                             
RESULT(v[5])


EQUATION("Target_Market_Share")
/*
Desired Market Share is a simple average between last period's desired market share and firm's average market share
*/
	v[0]=VL("Target_Market_Share", 1);                           
	v[1]=V("markup_freq");
	v[2]= fmod((double) t,v[1]);                                   
	if(v[2]==0)                                                   
		{ 
		v[3]=VL("Firm_Avg_Market_Share", 1);                        
  	v[4]=(v[0]+v[3])/2;                                         
  	}
	else                                                          
 		v[4]=v[0];                                                  
RESULT(v[4])


EQUATION("Firm_Avg_Market_Share")
/*
Average Market Share between the market share of the firm in the last markup period
*/
	v[0]=V("markup_freq");
	v[3]=0;										   						//initializes the sum
	for (v[1]=1; v[1]<=(v[0]); v[1]=v[1]+1)							//from 0 to markup period-1 lags
		{
		v[2]=VL("Market_Share", v[1]);									//computates firm's market share of the current lag
		v[3]=v[3]+v[2];													//sum up firm's lagged market share
		}
	v[4]=v[3]/v[0];														//average firm's market share of the last investment period
RESULT(v[4])


EQUATION("Target_Sales")
/*
Comment
*/                                       
	v[0]=VL("Target_Sales", 1);                           
	v[1]=V("markup_freq");
	v[2]= fmod((double) t,v[1]);                                   
	if(v[2]==0)                                                   
		{ 
		v[3]=V("Firm_Avg_Sales");                        
  	v[4]=VL("Firm_Avg_Sales", v[1]);                                         
  	v[5]=v[3]*(1+((v[3]-v[4])/v[4]));
  	}
	else                                                          
 		v[5]=v[0];                           
RESULT(v[5])


EQUATION("Firm_Avg_Sales")
/*

*/
	v[0]=V("markup_freq");
	v[3]=0;										   						
	for (v[1]=1; v[1]<=(v[0]); v[1]=v[1]+1)							
		{
		v[2]=VL("Sales", v[1]);									
		v[3]=v[3]+v[2];													
		}
	v[4]=v[3]/v[0];														
RESULT(v[4])


EQUATION("Markup")
/*
Effective Markup is the Effective Price over the UDC
*/
	v[0]=V("Price");
	v[1]=V("NUC");
	v[2]=v[0]/v[1];
RESULT(v[2])



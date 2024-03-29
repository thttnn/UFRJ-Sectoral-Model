//#define EIGENLIB			// uncomment to use Eigen linear algebra library
//#define NO_POINTER_INIT	// uncomment to disable pointer checking

#include "fun_head_fast.h"

// do not add Equations in this area

MODELBEGIN

EQUATION("Initialization")
/*
This variable initializes the model.
*/

CYCLE(cur, "SECTORS")
{	
	v[0]=VS(cur,"investment_period");               				
	v[1]=VS(cur,"markup_period");								
	v[2]=VS(cur,"annual_period");
	v[3]=VS(cur,"depreciation_period");
	v[4]=VS(cur,"number_object_firms");
	v[5]=VS(cur,"initial_productivity");
	v[6]=VS(cur,"initial_markup");
	v[7]=VS(cur,"initial_wage");
	v[8]=VS(cur,"initial_quality");
	v[9]=VS(cur,"input_tech_coefficient");
	v[10]=VS(cur,"capital_output_ratio");
	v[11]=VS(cur,"desired_inventories_proportion");	
	v[12]=VS(cur,"desired_degree_capacity_utilization");
	v[13]=VS(cur,"initial_demand_proportion");
	v[14]=VS(cur,"initial_input_price");
	v[15]=VS(cur,"initial_capital_price_scale");
	v[16]=VS(cur,"initial_process_product_share");
	v[17]=VS(cur,"initial_innovation_imitation_share");
	v[18]=VS(cur,"initial_desired_debt_rate");
	v[19]=VS(cur,"initial_liquidity_preference");

	v[20]=v[6]*((v[7]/v[5])+(v[14]*v[9]));														//sector initial price, given initial markup, wage and input cost
	v[21]=v[13]/v[4]; 																			//initial production of each firm
	v[22]=((v[21]*(1+v[11]))/v[12])*v[10];														//number of capital goods of each firm
	for(i=1; i<v[2]; i++)
		WRITELLS(cur, "Sector_Effective_Orders", v[13]*v[4], 0, i);               				//Effective_Orders_Sectors equals demand_initial		
		WRITELLS(cur, "Sector_Avg_Competitiveness", 1, 0, 1);                     				//if all firms are the same, equals 1 by definition
		WRITELLS(cur, "Sector_Productive_Capacity", ((v[13]*(1+v[11]))/v[12]), 0, 1);           //if all firms are the same, equals 1 by definition
	for(i=1 ; i<=v[2]+1 ; i++)
		{
		WRITELLS(cur, "Sector_Avg_Productivity", v[5], 0,  i);                      			//If all capital goods have the same productivity, Max_Productivity equals productivity_initial 
		WRITELLS(cur, "Sector_Avg_Price", v[20], 0, i);                                   		//Avg_Price equals avg_price initial
		}
		WRITELLS(cur, "Sector_Max_Productivity", v[5], 0,  1);                      			//If all capital goods have the same productivity, Max_Productivity equals productivity_initial 
		WRITELLS(cur, "Sector_Avg_Quality", v[8], 0,  1);
		WRITELLS(cur, "Sector_Avg_Quality", v[8], 0,  2);
		WRITELLS(cur, "Sector_Max_Quality", v[8], 0,  1);
		WRITELLS(cur, "Price_Capital_Goods", v[15]*v[20], 0,  1);
		WRITELLS(cur, "Price_Inputs", v[14], 0,  1);
			
	cur1=SEARCHS(cur, "FIRMS");																	//search the first and only instance of firms below each sector
		//Begin Writting Firm Variables
		WRITES(cur1, "firm_date_birth", 0);                                   					//zero by definition
	for (i=1 ; i<=v[0] ; i++)                                									//for (investment_period) lags
	  	{
	  	WRITELLS(cur1, "Firm_Demand_Productive_Capacity_Replacement", 0, 0, i);					//no replacement initially
	  	WRITELLS(cur1, "Firm_Debt_Rate", 0, 0, i);												//no debt initially
	  	WRITELLS(cur1, "Firm_Demand_Capital_Goods", 0, 0, i);
	  	WRITELLS(cur1, "Firm_Frontier_Productivity", v[5], 0, i);                 				//frontier productivity will be the initial frontier productivity
		}
	for(i=1; i<=(v[0]+1); i++)										 							//for (investment period+1) lags (7)
		WRITELLS(cur1, "Firm_Demand_Productive_Capacity_Expansion", 0, 0, i);     				//write 0 
	for (i=1 ; i<=(2*v[0]) ; i++)																//for (2*investment period+1) lags
	  	WRITELLS(cur1, "Firm_Effective_Orders", v[21], 0, i);                     				//firm's effective orders will be sector's effective orders (given by demand_initial) divided by the number of firms
	for (i=1 ; i<=(v[1]-1) ; i++)																//for (markup_period-1) lags
		WRITELLS(cur1, "Firm_Market_Share", (1/v[4]), 0, i);             						//firm's market share will be the inverse of the number of firms in the sector (initial market share)

		WRITELLS(cur1, "Firm_Price", v[20], 0, 1);                  							//firm's price
	  	WRITELLS(cur1, "Firm_Desired_Market_Share", (1/v[4]), 0, 1);                  			//firm's desired market share will be twice the initial market share  
	  	WRITELLS(cur1, "Firm_Avg_Market_Share", (1/v[4]), 0, 1);                     			//firm's avg market share will be the initial market share
	  	WRITELLS(cur1, "Firm_Desired_Markup", v[6], 0, 1); 										//desired markup will be the initial markup
	  	WRITELLS(cur1, "Firm_Stock_Inventories", (v[21]*v[11]), 0, 1);                        	//firm's inventories will be the sales times the desired inventories proportion (parameter)
	  	WRITELLS(cur1, "Firm_Stock_Inputs", (v[9]*v[21]), 0, 1);                      			//firm's stock of imputs will be the sales times the input tech relationship
	  	WRITELLS(cur1, "Firm_Productive_Capacity", ((v[21]*(1+v[11]))/v[12]), 0, 1);			//firm productive capacity will be the sales divided by the desired degree of capacity utlization (parameter)
	  	WRITELLS(cur1, "Firm_Capital", (v[22]*v[15]), 0, 1);									//firm nominal capital equals number of capital if capital goods price equals 1
	  	WRITELLS(cur1, "Firm_Wage", v[7], 0, 1); 												//firm's nominal wage equals sector nominal wage initial
		WRITELLS(cur1, "Firm_Quality", v[8], 0, 1); 	
		WRITELLS(cur1, "Firm_Competitiveness", 1, 0, 1);                           				//if all firms are the same
	  	WRITELLS(cur1, "Firm_Delivery_Delay", 1, 0, 1);                           		  		//it is assumed that all demand is met initially, so equals 1 by definition
		WRITELLS(cur1, "Firm_Stock_Deposits", 0, 0, 1);											//no financial assets initially
	  	WRITELLS(cur1, "Firm_Stock_Loans", 0, 0, 1);                                    		//no debt initially
	  	WRITELLS(cur1, "Firm_Avg_Debt_Rate", 0, 0, 1);                       					//no debt initially
	  	WRITELLS(cur1, "Firm_Process_RND_Share", v[16], 0, 1);                       					
	  	WRITELLS(cur1, "Firm_Innovation_RND_Share", v[17], 0, 1); 
	  	WRITELLS(cur1, "Firm_Desired_Debt_Rate", v[18], 0, 1); 
	  	WRITELLS(cur1, "Firm_Liquidity_Preference", v[19], 0, 1);                       					
	  	
	 	//Begin Creating Firms and writting some parameters
	 	for(i=1; i<=(v[4]-1); i++)																//for the number of firms of each sector (defined by the parameter)
	 	{
		cur4=ADDOBJ_EXLS(cur,"FIRMS", cur1, 0);                             					//create new firm using the first and only firm as example
			if(V("switch_randomize_initialization")==1)
			{
			WRITELLS(cur4, "Firm_Process_RND_Share", uniform(0.1,0.9), 0, 1);
			WRITELLS(cur4, "Firm_Innovation_RND_Share", uniform(0.1,0.9), 0, 1);
			}
		}		
				
	 	CYCLES(cur, cur1, "FIRMS")                                                 				//CYCLE trough all firms
			{
			v[70]=SEARCH_INSTS(cur, cur1);														//search current firm position in the total economy
			WRITES(cur1, "id_firm_number", v[70]);                         						//write the firm number as the current position (only initial difference between firms)
			
			//Begin creating capital goods and writting "capital_good_date_birth"		
			for(i=1; i<=v[22]-1; i++)                        									//for the number of capital goods of each firm
				{
				cur2=ADDOBJS(cur1,"CAPITALS");			                       					//create new capital goods using the first one as example
				WRITES(cur2, "id_capital_good_number", i);										//write the capital good number
				}
				CYCLES(cur1, cur3, "CAPITALS")
					{
					WRITELLS(cur3, "Capital_Good_Acumulated_Production", 0, 0, 1);      			//zero by definition
					WRITES(cur3, "capital_good_productive_capacity", (1/v[10]));     				//inverse of capital output ratio  
					WRITES(cur3, "capital_good_productivity_initial", v[5]);       		  			//defined in the control parameters
					WRITES(cur3, "capital_good_to_replace", 0);                          			
					WRITES(cur3, "capital_good_date_birth", 0);									//write the capital good date of birth
					WRITES(cur3, "capital_good_depreciation_period", 1000);
					}
			
			
			}
					
}			
PARAMETER
RESULT(0)


EQUATION("Time_Step")
/*
Root Variable
Determines the order in which the variables will be calculated in each time step. It is the logical sequence of the model in each time step.
*/

/*****PRICE, COSTS, COMPETITIVENESS AND MARKET-SHARE*****/   
V("Price_Capital_Goods");								//autonomous
V("Price_Inputs");										//autonomous                        
V("Firm_Wage");											//autonomous                                  	   	                                    		                                       
V("Firm_Variable_Cost");                                    
V("Firm_Desired_Market_Share");                             
V("Firm_Desired_Markup");                                                                      
V("Firm_Price");                                            
V("Sector_Avg_Price"); 

V("Firm_Competitiveness");								//autonomous
V("Sector_Avg_Competitiveness");                               
V("Firm_Market_Share"); 
V("Firm_Avg_Market_Share");
                                                                      		            		
/*****EFFECTIVE PRODUCTION AND INPUT DEMAND*****/
V("Firm_Expected_Sales");								//autonomous                                 
V("Firm_Planned_Production");                        	
V("Capital_Good_Productivity");                        		                                                                                                          		
V("Firm_Effective_Production");                             
V("Firm_Capacity_Utilization");	
V("Firm_Max_Productivity");
V("Firm_Avg_Productivity");	
V("Firm_Input_Demand_Next_Period");
V("Firm_Stock_Inputs");   						

/*****EFFECTIVE ORDERS, SALES*****/
V("Sector_Effective_Orders");							//autonomous
V("Firm_Effective_Orders");                                 
V("Firm_Sales");                                           
V("Firm_Delivery_Delay");                                  
V("Sector_Sales");                                     		
V("Firm_Stock_Inventories");                               
V("Firm_Effective_Market_Share"); 

/*****REVENUE AND PROFITS*****/                          
V("Firm_Revenue");                                          
V("Firm_Indirect_Tax");                                     
V("Firm_Net_Revenue");									   	
V("Firm_RND_Expenses");									   	                            		           		
V("Firm_Interest_Payment");
V("Firm_Debt_Payment");
V("Firm_Financial_Obligations");
V("Firm_Deposits_Return");
V("Firm_Net_Profits");                                      
V("Firm_Retained_Profits");                                 
V("Firm_Distributed_Profits");                             
                      	
/*****R&D*****/
V("Firm_Process_RND_Share");
V("Firm_Innovation_RND_Share");
V("Firm_Innovation_Productivity");                          
V("Firm_Imitation_Productivity"); 
V("Firm_Innovation_Quality");                          
V("Firm_Imitation_Quality");                           
V("Firm_Frontier_Productivity");                            
V("Firm_Quality");										   	

/*****CAPITAL AND INVESTMENT DECISIONS*****/
V("Firm_Investment_Period");								//autonomous					   		
V("Firm_Productive_Capacity_Depreciation");					//autonomous                 
V("Firm_Productive_Capacity");   
V("Firm_Capital");                          
 				
V("Firm_Expected_Sales_Long_Term");                         
V("Firm_Desired_Expansion_Investment_Expenses");            
V("Firm_Desired_Replacement_Investment_Expenses");			
V("Firm_Desired_Investment_Expenses");

/*****FINANCIAL CONSTRAINTS*****/

V("Firm_Avg_Debt_Rate");//autonomous
V("Firm_Interest_Rate_Short_Term");	
V("Firm_Interest_Rate_Long_Term");
V("Firm_Desired_Debt_Rate");	
V("Firm_Liquidity_Preference");								
V("Firm_Retained_Deposits");								
V("Firm_Internal_Funds");																
V("Firm_Available_Loans");                                  
V("Firm_Demand_Loans");
V("Firm_Effective_Loans");
V("Firm_Total_Funds");                                       

/*****EFFECTIVE INVESTMENT*****/
V("Firm_Effective_Expansion_Investment_Expenses");          
V("Firm_Demand_Productive_Capacity_Expansion");              
V("Firm_Available_Funds_Replacement");                       
V("Firm_Demand_Productive_Capacity_Replacement");           
V("Firm_Effective_Replacement_Investment_Expenses");
V("Firm_Demand_Capital_Goods");							   	
V("Firm_Effective_Investment_Expenses");
V("Firm_Available_Funds_After_Replacement");				

/*****STOCKS*****/
V("Firm_Stock_Loans_Short_Term");
V("Firm_Stock_Loans_Long_Term");
V("Firm_Stock_Loans");                                       
V("Firm_Stock_Deposits");									                                          
V("Firm_Debt_Rate");                                       

/*****SECTOR RESULTS*****/
V("Sector_Profit_Rate");                             	
V("Sector_Normalized_HHI");                            	
V("Sector_Turbulence");               	
V("Sector_Demand_Met");                                    		               
V("Sector_Inventories");                                     		
V("Sector_Effective_Production");											
V("Sector_Productive_Capacity");									
V("Sector_Sum_Market_Share");                                  
V("Sector_Indirect_Tax"); 
V("Sector_Stock_Loans");
V("Sector_Stock_Deposits");                                     	
V("Sector_Avg_Wage"); 
V("Sector_Max_Productivity");                                       	
V("Sector_Avg_Markup");                                            		
V("Sector_Avg_Productivity");                                   	    	
V("Sector_Avg_Debt_Rate");                                     	
V("Sector_Avg_Desired_Debt_Rate");                                       	
V("Sector_Max_Quality");                              	
V("Sector_Avg_Quality");                                          	
                                  
RESULT(t)

/**************************VARIABLES******************************/

#include "sectoral_model_price.h"			// Firm's Price Variables	
#include "sectoral_model_production.h"		// Firm's Production Variables
#include "sectoral_model_sales.h"			// Firm's Sales Variables
#include "sectoral_model_rnd.h"				// Firm's R&D Variables
#include "sectoral_model_profits.h"			// Firm's Profit Variables
#include "sectoral_model_investment.h"		// Firm's Investment Variables
#include "sectoral_model_finance.h"			// Firm's Finance Variables
#include "sectoral_model_entry_exit.h"  	// Entry and Exit Variables
#include "sectoral_model_sector.h" 			// Sectoral Variables     
#include "sectoral_model_capital_goods.h"  	// Capital Goods Object Variables

MODELEND

// do not add Equations in this area

void close_sim( void )
{
	// close simulation special commands go here
}

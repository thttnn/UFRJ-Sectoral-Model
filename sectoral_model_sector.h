//****************************************SECTOR AGREGATES*************************************//


EQUATION("Sector_Profit_Rate")
/*
Sector Variable for Analysis
*/
	v[0]=0;                           		//initializes the CYCLE    
	CYCLE(cur, "FIRMS")               		//CYCLE trought all firms of the sector
	{
		v[1]=VS(cur, "Firm_Net_Profits");  		//firm's profits
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



/*****SECTOR AGGREGATES*****/


EQUATION("Sector_Sales")                          
/*
Sum up the sales of all firms in the sector
*/
	v[0]=SUM("Firm_Sales");                                     
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


EQUATION("Sector_Inventories")
/*
Sum of the Inventories 
*/
	v[0]=SUM("Firm_Stock_Inventories");                                
RESULT(v[0])


EQUATION("Sector_Effective_Production")
/*
Sum of the Effective Production 
*/
	v[0]=SUM("Firm_Effective_Production");                        
RESULT(v[0])


EQUATION("Sector_Productive_Capacity")
/*
Sum of the Productive Capacity 
*/
	v[0]=SUM("Firm_Productive_Capacity");                     
RESULT(v[0])


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


EQUATION("Sector_Sum_Market_Share")
/*
Sum of the Market shares: this variable works to verify the Fischer Equation
*/
	v[0]=SUM("Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Indirect_Tax")
/*
Sum of the indirect taxes paid by firms in the sector
*/
	v[0]=SUM("Firm_Indirect_Tax");
RESULT(v[0])


EQUATION("Sector_Profits")
/*
Sum of the net profits of firms in the sector
*/
	v[0]=SUM("Firm_Net_Profits");
RESULT(v[0])


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


EQUATION("Sector_Stock_Loans")
/*
Sum of the stock of laons of each firm in the sector
*/
	v[0]=SUM("Firm_Stock_Loans");
RESULT(v[0])


EQUATION("Sector_Stock_Deposits")
/*
Sum of the stock of deposits of each firm in the sector
*/
	v[0]=SUM("Firm_Stock_Deposits");
RESULT(v[0])


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


/*****SECTOR AVERAGES, SD AND MAX*****/


EQUATION("Sector_Avg_Price")
/*
Average weighted by firm's market share
*/
	v[0]=WHTAVE("Firm_Price", "Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Avg_Wage")
/*
Sector average nominal wage, weighted by firm's market share
*/
	v[0]=WHTAVE("Firm_Wage", "Firm_Market_Share");
RESULT(v[0])


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


EQUATION("Sector_Max_Productivity")
/*
Maximum productivity of the sector will be the higher between firm's frontier productivities
*/
	v[0]=MAX("Firm_Frontier_Productivity");          
RESULT(v[0])


EQUATION("Sector_Avg_Markup")
/*
Average markup of the sector, weighted by firm's effective market share
*/
	v[0]=WHTAVE("Firm_Effective_Markup", "Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Avg_Productivity")
/*
Sector average productivity will be the average of firms productivity weighted by their market shares
*/
	v[0]=WHTAVE("Firm_Avg_Productivity", "Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Avg_Debt_Rate")
/*
Sector average debt rate will be the average of firms productivity weighted by their market shares
*/
	v[0]=WHTAVE("Firm_Debt_Rate", "Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Avg_Desired_Debt_Rate")
/*
Sector average debt rate will be the average of firms productivity weighted by their market shares
*/
	v[0]=WHTAVE("Firm_Desired_Debt_Rate", "Firm_Market_Share");
RESULT(v[0])


EQUATION("Sector_Max_Quality")
/*
Maximum firm quality of the sector.
*/
	v[0]=MAX("Firm_Quality");
RESULT(v[0])


EQUATION("Sector_Avg_Quality")
	v[0]=WHTAVE("Firm_Quality", "Firm_Market_Share");
RESULT(v[0])

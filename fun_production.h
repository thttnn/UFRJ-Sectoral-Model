//****************************************FIRM PRODUCTION and INPUTS*************************************//


EQUATION("Firm_Expected_Sales")
/*
Firm's expected sales are calculated from an average of effective sales from the two previous periods, applying a expected growth rate. This expected growth rate is obtained from comparing the average of the two previous periods with the average of the two before that, adjusted by an expectation parameter.
*/
	v[1]=VL("Firm_Effective_Orders", 1);                    										//firm's effective orders lagged 1
	v[2]=VL("Firm_Effective_Orders", 2);                    										//firm's effective orders lagged 2
	v[3]=V("expectations");                            												//firm expectations
	if(v[2]!=0)                                           											//if firm's effective orders lagged 2 is not zero
		{
		v[4]=v[1]*(1+v[3]*((v[1]-v[2])/v[2]));              										//expected sales will be the effective orders in the last period multiplied by the growth rate between the two periods adjusted by the expectations parameter
		v[5]=max(0,v[4]);                                  											//expected sales can never be negative
		}
	else                                                  											//if firm's effective orders lagged 2 is zero 
		v[5]=v[1];                                         											//expected sales will be equal to effective orders of the last period
RESULT(v[5])


EQUATION("Firm_Planned_Production")
/*
It's calculated from expected sales for the period, considering the proportion of sales that is desired to be kept as inventories and discounting the acumulated stock of inventories in the last period. 
*/
	v[1]=V("Firm_Expected_Sales");                          										//firm's expected sales
	v[2]=V("desired_inventories_proportion");             											//firm's desired inventories ratio as a proportion of sales
	v[3]=VL("Firm_Stock_Inventories",1);                    										//firm's stock of inventories in the last period
	v[4]=v[1]*(1+v[2])-v[3];                            											//planned production will be expected sales plus the desired proportion of investories minus the existing stock of inventories
RESULT(v[4])


EQUATION("Firm_Effective_Production")
/*
This variable calculates the effective production of each firm, based on already constrained planned production. Here, it alocates production in capital goods vintages, in order of productivity.
*/
	v[0]=V("Firm_Planned_Production");                                                              //firm's planned production		
	v[1]=VL("Firm_Productive_Capacity", 1);                 										//calls the firm's productive capacity of the last period
	v[2]=V("input_tech_coefficient"); 																//techincal input coefficient
	v[3]=VL("Firm_Stock_Inputs",1);																	//current stock of inputs
	if(v[2]!=0)
		v[4]=max(0, min((min(v[1],v[0])),(v[3]/v[2])));          									//planned production can never be more then the maximum productive capacity, never more than input availability, and can never be negative
	else
		v[4]=max(0, (min(v[1],v[0])));
	SORT("CAPITALS", "Capital_Good_Productivity", "DOWN");                                        	//rule for the use of capital goods, sorts firm's capital goods by productivity in a decreasing order
	v[5]=0;                                                                                      	//initializes the CYCLE
	CYCLE(cur, "CAPITALS")                                                                        	//CYCLE trought the capital goods of the firm
	{
		v[6]=VS(cur, "capital_good_productive_capacity");                                          	//capital productivity capacity
		v[7]=max(0,(min(v[6], v[4])));                                                             	//maximum capacity of each capital goods, constrained by effective planned production, and it can not be negative
		WRITES(cur, "Capital_Good_Production", v[7]);                                              	//the capacity of each capital goods is in fact its production
		v[4]=v[4]-v[7];                                                                            	//it subracts the production of the first capital good from the effective planned production before going to the next capital good
		v[5]=v[5]+v[7];                                                                            	//sums up the production of each capital good to determine firm's effective production
	}
RESULT(v[5])


EQUATION("Firm_Input_Demand_Next_Period")
/*
The demand for inputs of each sector of intermediate goods, which should be produced during current period and to be used in the next period, is calculated based on the necessity of programed production, in which a expected growth rate is applied and the remaing stock is discounted. 
The expected growth rate is defined by the average of effective orders of the last two periods, extrapolated by a expectation parameter.
The intermediate goods sectors demand exactly the amount they will need to produce in the next period, without discouting the initial stock, which is now know, since the intermediate goods production in the current period will be defined by the demand for inputs of all sectors and not the programed production of the intermediate sector.
*/

	v[0]=V("Firm_Planned_Production");                     											//firm's planned production for the current period
	v[1]=V("input_tech_coefficient");           													//input technical coefficient
	v[2]=V("Firm_Effective_Production");															//firm's effective production
	
	v[3]=VL("Firm_Effective_Orders", 1);                   											//firm's effective orders lagged 1 
	v[4]=VL("Firm_Effective_Orders", 2);                   											//firm's effective orders lagged 1
	v[5]=V("expectations");                            												//firm's expectation parameter
	if(v[4]!=0)                                       												//if effective orders is not 0
		v[6]=1+(v[5]*((v[3]-v[4])/v[4]));               											//gives an expected growth rate, based on the effective orders
	else                                              												//if effective orders is 0
		v[6]=1;                                         											//the expected growth rate is 1
		
	v[7]=VL("Firm_Stock_Inputs",1);                   												//gives the stock of imputs remaining from the last period
	
	v[8]=(v[0]*v[1])*v[6]-(v[7]-(v[2]*v[1]));              											//gives the amount of imputs necessary for the next period, multiplying the current planned production by the exptected growth rate and by the imput techinical relanshionship and subtracting the current stock of inputs minus the amount used in effective produtcion
	v[9]=max(v[8],0);                               												//the demand of imputs for the next period can never be negative
RESULT(v[9])


EQUATION("Firm_Stock_Inputs")
/*
The stock of inputs of each firm at the end of the period is calculated by summing the stock of inputs in the last period with the amount demanded for the production in the next period minus the amount used in the production of the current period
*/
	v[0]=V("Firm_Input_Demand_Next_Period");              											//amount of inputs demanded in the current period to be used in the production of the next period                          
	v[1]=V("input_tech_coefficient");                     											//input technical coefficient               			 
	v[2]=V("Firm_Effective_Production");                  											//firm's effective production   
	v[3]=VL("Firm_Stock_Inputs", 1);	                 											//firm's stock of inputs in the last period
	v[4]=v[3]+v[0]-(v[1]*v[2]);                      	  											//the current stock of inputs is the stock in the last period plus the amount of the demand for the next period that was effetivly met, minus the amount used in effective production                 
	v[5]=max(v[4],0);                                     											//the current stock can never be negative
RESULT(v[5])


EQUATION("Firm_Avg_Productivity")
/*
Firm's productivity will be an average of each capital good productivity weighted by their repective production	
*/
	v[0]=V("Firm_Effective_Production");                                							//firm's effective production
	v[1]=VL("Firm_Avg_Productivity", 1);                           									//firm's average productivity in the last period
	v[2]=0;                                                        									//initializes the CYCLE
	v[3]=0;                                                        									//initializes the CYCLE
	CYCLE(cur, "CAPITALS")                                          								//CYCLE trought firm's capital goods
	{
		v[4]=VS(cur, "Capital_Good_Productivity");                   								//capital good productivity
		v[5]=VS(cur, "Capital_Good_Production");                    								//capital good production
		v[2]=v[2]+v[4]*v[5];                                        								//sums up the product of each capital good productivity and production
		v[3]=v[3]+v[5];                                             								//sums up the production of each capital good
	}
	if (v[3]!=0)                                                   									//if the sum of the production is not zero
		v[6]=v[2]/v[3];                                             								//firm's average productivity will be the average of each capital good productivity weighted by its respective production
	else                                                           									//if the sum of the production is zero
		v[6]=v[1];                                                   								//firm's average production will be the last period's 
RESULT(v[6])


EQUATION("Firm_Max_Productivity")
/*
Maximum productivity among firm's capital goods
*/
	v[0]=COUNT("CAPITALS");
	if(v[0]!= 0)
		v[1]=MAX("Capital_Good_Productivity");
	else
		v[1]=0;
RESULT(v[1])


EQUATION("Firm_Capacity_Utilization")
/*
Firm effective production over firm productive capacity
*/
	v[0]=V("Firm_Effective_Production");
	v[1]=VL("Firm_Productive_Capacity",1);
	if(v[1]!=0)
		v[2]=v[0]/v[1];
	else
		v[2]=0;
RESULT(v[2])

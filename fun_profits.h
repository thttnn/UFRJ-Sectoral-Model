//****************************************FIRM PROFITS*************************************//


EQUATION("Firm_Revenue")
/*
Revenue depends on sales and price
*/
	v[0]=V("Firm_Sales");                                             											//firm's sales 
	v[1]=V("Firm_Price");                                             											//firm's price
	v[2]=v[0]*v[1];
RESULT(v[2])


EQUATION("Firm_Indirect_Tax")
/*
Indirect Tax of the firm is the revenue multiplied by the tax parameter
*/
	v[0]=V("Firm_Revenue");
	v[1]=V("indirect_tax_rate");
	v[2]=v[0]*v[1];
RESULT(v[2])


EQUATION("Firm_Net_Revenue")
/*
Firm's net revenue, discounting taxes and R&D expenses.
*/
	v[0]=V("Firm_Revenue");
	v[1]=V("indirect_tax_rate");
	v[2]=V("rnd_revenue_proportion");
	v[4]=v[0]*(1-v[1])*(1-v[2]);
RESULT(v[4])


EQUATION("Firm_RND_Expenses")
/*
Firm's R&D expenses, subtracted from the revenue after taxes.
It will be distributed to income class as wages.
*/
	v[0]=V("Firm_Revenue");
	v[1]=V("indirect_tax_rate");
	v[2]=V("rnd_revenue_proportion");
	v[4]=v[0]*(1-v[1])*(v[2]);
RESULT(v[4])


EQUATION("Firm_Interest_Payment")
/* 
Sum up total interest payment on all firm's loans. Interest rates are fixed for each loan. 
switch_interest_payment
0-->pre-fixed interest
1-->pos-fixed interest
*/
	v[0]=V("switch_interest_payment");
	v[1]=V("Firm_Interest_Rate_Long_Term");
	v[2]=V("Firm_Interest_Rate_Short_Term");
	v[3]=0;												//initializes the CYCLE
	CYCLE(cur, "LOANS")									//CYCLE trough all firm's loans
	{
		v[4]=VS(cur, "firm_loan_total_amount");			//debt current amount 
		v[5]=VS(cur, "firm_loan_interest_rate");		//debt interest rate
		v[6]=VS(cur, "id_firm_loan_short_term");
		v[7]=VS(cur, "id_firm_loan_long_term");
		if(v[0]==0)
			v[8]=v[4]*v[5];
		if(v[0]==1)
		{
			if(v[6]==1)
				v[8]=v[4]*v[2];
			else 
				v[8]=v[4]*v[1];
		}
		v[3]=v[3]+v[8];
	}                                      			               
RESULT(v[3])								


EQUATION("Firm_Debt_Payment")
/* 
Sum up total debt payment on all firm's loans. Amortizations are fixed for each loan. 
This variable also adjusts the total amount of each loan and delete loan objects if all debt is paid.
*/
	v[0]=SUM("firm_loan_fixed_amortization");		 			//sum up all amortizations for current period
		CYCLE_SAFE(cur, "LOANS")							//CYCLE trough all firm's loans
		{
		v[4]=VS(cur, "firm_loan_total_amount");					//debt current amount 
		v[5]=VS(cur, "firm_loan_fixed_amortization");			//debt fixed amortization
		v[6]=v[4]-v[5]*(1+v[1]);								//new total amount
		v[7]=VS(cur, "firm_loan_fixed_object");					//identifies if it is fixed object, necessary for model structure
		if (v[7]!=1)
			{	
			if (v[6]>0)											//if there is still amount to be amortized
				WRITES(cur, "firm_loan_total_amount", v[6]);	//write the new amount
			else												//if all amount was already amortized
				DELETE(cur);									//delete current loan
			}
		}
RESULT(v[0])


EQUATION("Firm_Financial_Obligations")
/*
Firm Financial Obligations in the current period is the sum of interest payment and debt payment
*/
	v[1]=V("Firm_Interest_Payment");
	v[2]=V("Firm_Debt_Payment");
	v[3]=v[1]+v[2]; 
RESULT(v[3])


EQUATION("Firm_Deposits_Return")
/*
Firm interest receivment on deposits
*/
v[0]=VL("Firm_Stock_Deposits",1);
v[1]=V("interest_rate_deposits");
v[2]=v[0]*v[1];
RESULT(v[2])


EQUATION("Firm_Net_Profits")
/*
Firm profit, including
*/
	v[0]=V("Firm_Net_Revenue");                                       											//firm's net revenue
	v[1]=V("Firm_Effective_Production");                              											//firm's effective production
	v[2]=V("Firm_Variable_Cost");                                     											//firm's variable cost	
	v[3]=V("Firm_Financial_Obligations");							  											//firm's financial obligations
	v[4]=V("Firm_Deposits_Return");
	v[5]=v[0]-(v[2]*v[1])-v[3]+v[4];										  									//firm's operational profits
RESULT(v[5])


EQUATION("Firm_Retained_Profits")
/*
Profit retained by the sector after being distributed to class and paid interest on the debt and separate the expense for depreciation.
*/
	v[0]=V("Firm_Net_Profits");                                        											//firm's profits            
	v[1]=V("profits_distribution_rate");                               											//firm's profit distribution parameter                            
	if(v[0]>0)                                                         											//if net profits is positive
		v[2]=(1-v[1])*v[0];                                           											//retained profits is positive, to be added to stock of deposits
	else                                                               											//if net profits is zero or negative                                                                     
		v[2]=v[0];                                                     											//retained profits equals net profits, to be discounted from stock of deposits                                                         
RESULT(v[2])


EQUATION("Firm_Distributed_Profits")
/*
Amount of profits distributed to the income classes
*/
	v[0]=V("Firm_Net_Profits");                                        											//firm's profits            
	v[1]=V("profits_distribution_rate");                               											//firm's profit distribution parameter  
	if(v[0]>0)                                                         											//if net profits is positive
		v[2]=v[1]*v[0];                                                											//distributed profits
	else                                                               											//if net profits is zero or negative                                                                     
		v[2]=0;																											
RESULT(v[2])


EQUATION("Firm_Profit_Rate")
/*
Net profits over total capital (last period)
Analysis Variable
*/
	v[0]=V("Firm_Net_Profits");
	v[1]=VL("Firm_Capital",1);
	if(v[1]!=0)
		v[2]=v[0]/v[1];
	else
		v[2]=0;
RESULT(v[2])
//****************************************FIRM DEMAND, SALES AND INVENTORIES*************************************//


EQUATION("Firm_Effective_Orders")
/*
Firm effective production is calculated in the sectoral demand distribution.
This variables takes saves the value and clears the temporary parameter.
*/
	v[1]=V("Firm_Market_Share");
	v[2]=V("Sector_Effective_Orders");
	v[3]=v[1]*v[2];
RESULT(v[3])


EQUATION("Firm_Sales")
/*
The observed sales are defined by the minimum between the effective orders and the effective production plus inventories of each sector.In the case of intermediate sectors, the sale of inputs from their inventories to meet the extraordinary demand at the beginning of the period should be added to the total sold.In the case of the agricultural sector, the observed sales are equal to the current production.
*/
	v[0]=VL("Firm_Stock_Inventories",1);                   												//stock of inventories of the firm
	v[1]=V("Firm_Effective_Production");                   												//firm's effective production in that period
	v[2]=V("Firm_Effective_Orders");                       												//firm's effective orders
	v[3]=v[0]+v[1];                                        												//total amount of available products
	v[4]=max(0, min(v[3],v[2]));                           												//firm's sales will be the effective orders, constrained by the total amount of available products, and can never be negative
RESULT(v[4])


EQUATION("Firm_Stock_Inventories")
/*
The finished product stock at the end of the period will be calculated by adding the remaining stock of products at the beginning of the period to which it was produced and discounting the quantity sold. In the case of the agricultural sector, there is no stock of finished products at the end of the period.
*/
	v[0]=V("Firm_Effective_Production");                    											//firm's effective production                 
	v[1]=V("Firm_Sales");                                   											//firm's sales
	v[2]=VL("Firm_Stock_Inventories", 1);                  												//stock of inventories in the last period
	v[3]=v[0]-v[1]+v[2];                                    											//the stock of inventories in the end of the period will be the difference between effective production and sales, added to the current stock of inventories
RESULT(v[3])


EQUATION("Firm_Delivery_Delay")
/*
Firm Variable
*/
	v[0]=V("Sector_Effective_Orders");              													//effective orders of the sector
	v[1]=V("Firm_Market_Share");                    													//firm's market share
	v[2]=v[0]*v[1];                                 													//firm's effective orders
	v[3]=V("Firm_Sales");                           													//firm's sales
	if (v[3]!=0)                                    													//if firm's sales is not zero
		v[4]=v[2]/v[3];                             													//delivery delay will be determined by the ratio between effective orders and sales
	else                                            													//if firm's sales is zero
		v[4]=1;                                     													//delivery delay will be one
RESULT(v[4])


EQUATION("Firm_Effective_Market_Share")
/*
Effective market share is given by firm's sales over total sales of the sector
*/
	v[0]=V("Firm_Sales");
	v[1]=V("Sector_Sales");
	if (v[1]!=0)
		v[2]=v[0]/v[1];
	else
		v[2]=0;
RESULT(v[2])

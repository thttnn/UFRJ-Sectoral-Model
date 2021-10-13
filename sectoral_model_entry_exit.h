/*****ENTRY AND EXIT VARIABLES*****/

EQUATION("Exit")
/*
This variable counts productive capacity that exited the sector in each time step. Firm objects are deleted inside that variable due to two possibilities: small market share or high debt rate. In the second case, if entry conitions are met, the firm can be bought by a new one.
*/

v[0]=V("survival_period");
v[9]=V("switch_exit");

	i = COUNT( "FIRMS" );										// count the existing number of firms 
	v[11]=v[12]=v[14]=v[24]=0;									// initialize the cycles
	CYCLE_SAFE( cur, "FIRMS" )									// use a robust cycle to delete objects
	{
     v[1]=VS(cur, "Firm_Market_Share");							//firm's curent market share
     v[2]=VS(cur, "Firm_Productive_Capacity");					//firm's current productive capacity																				
     v[3]=VS(cur, "firm_date_birth");							//firm's date birth
	 v[4]=VS(cur, "Firm_Avg_Debt_Rate");						//firm's avg debt rate
     v[5]=VLS(cur, "Firm_Avg_Debt_Rate", 1);					//firm's avg debt rate in the last period
     v[6]=VS(cur, "Firm_Stock_Loans");
     v[7]=VS(cur, "Firm_Stock_Deposits");
     v[8]=VS(cur, "Firm_Capital");
    
     if ( v[9]==1 && v[1] <= 0.001 && i > 1 && t>(v[0]+v[3]))				//if firm's market share is near zero
     {
      	if (v[7]>=v[6])											//firm pays current debt with current deposits 
      		{
      		v[11]=v[11]+(v[7]-v[6]);							//deposits distributed to income classes (no need to substract from bank's stock)
      		v[14]=v[14];										//defaulted loans
      		}
      	else													//if firm can not pay current debt with current deposits
      		{
       		v[11]=v[11];										//deposits distributed to income classes (no need to substract from bank's stock)
      		v[14]=v[14]+(v[6]-v[7]);							//defaulted loans
      		}
      DELETE(cur); 												//delete current firm
      v[24]=v[24]+1;											//count number of exited firms
	  --i;														//subtract 1 from the number of firms counter
     }
     else														//if firm's current market share is not near zero																																						//if firm's avg debt rate is not higher than 1 (do not delete current firm						
     {
		if( v[9]==1 && v[4]>1 && i>1 && t>(v[0]+v[3]))
		{
      	if (v[7]>=v[6])											//firm pays current debt with current deposits 
      		{
      		v[11]=v[11]+(v[7]-v[6]);							//deposits distributed to income classes (no need to substract from bank's stock)
      		v[14]=v[14];										//defaulted loans
      		}
      	else													//if firm can not pay current debt with current deposits
      		{
       		v[11]=v[11];										//deposits distributed to income classes (no need to substract from bank's stock)
      		v[14]=v[14]+(v[6]-v[7]);							//defaulted loans
      		}
		DELETE(cur); 											//delete current firm
		v[24]=v[24]+1;											//count number of exited firms
		--i;					
		}
		else
		{
        v[11]=v[11];											//deposits distributed to income classes (no need to substract from bank's stock)
      	v[14]=v[14];					  						//defaulted loans
      	v[24]=v[24];
      	}
	 }
	WRITE("Exit_Deposits_Distributed", v[11]);
	WRITE("Exit_Defaulted_Loans", v[14]);
	}			

	v[60]=SUM("Firm_Market_Share");												//sum of firms market share 
	CYCLE(cur, "FIRMS")															//cycle trough firms to normalize market shares
	{
	v[61]=VS(cur,"Firm_Market_Share");											//current firm's market share
	WRITES(cur, "Firm_Market_Share", (v[61]/v[60]));							//divide firm's market share by the sum of market shares
	}
	
RESULT(v[24])

EQUATION_DUMMY("Exit_Deposits_Distributed", "Exit")
EQUATION_DUMMY("Exit_Defaulted_Loans", "Exit")



EQUATION("Sector_Entry_Condition")
/*
Can only be 0 or 1, if all enter conditions are met.
*/
	v[0]=V("switch_entry");
	v[1]=V("annual_period");
	v[2]=VL("Sector_Effective_Orders",1);
	v[3]=VL("Sector_Effective_Orders",2);
	v[4]=VL("Sector_Effective_Orders",3);
	v[5]=VL("Sector_Effective_Orders",v[1]);
	v[6]=v[2]-v[5];
	v[7]=(v[2]-v[3])/v[3];
	v[8]=(v[3]-v[4])/v[4];
  
    if(v[0]!=0 && v[6]>0 && v[7]>0 && v[8]>0)
     v[9]=1;
    else
     v[9]=0;
 
	if(v[9]==1)
	{
		cur1 = RNDDRAW( "FIRMS" , "Firm_Market_Share");
		cur2 = ADDOBJ_EX( "FIRMS", cur1);
		
		/*
		WRITES (cur2, "Firm_Market_Share", 0);
		WRITES (cur2, "firm_date_birth", t);
		WRITES (cur2, "Firm_Stock_Loans",0);							//firm's stock of debt is the price of capital goods bought
        WRITES (cur2, "Firm_Stock_Deposits",0);
		
		v[50]=COUNTS(cur2,"CAPITALS");
			  CYCLE_SAFES(cur2, cur3, "CAPITALS")								//CYCLE trough firm's capitals
				{
					if(v[50]>1)
						{
						DELETE(cur3);												//delete the current capital
						v[50]=v[50]-1;
						}
					else	
						v[50]=v[50];
				}
			CYCLES(cur2, cur3, "CAPITALS")
			{
			WRITES(cur3, "capital_good_productive_capacity",0);
			WRITES(cur3, "capital_good_date_birth",t);					//write the new date of birth
			WRITES(cur3, "capital_good_depreciation_period",(t+v[38])); //write the new date of birth
			WRITES(cur3, "capital_good_to_replace",0);					//write current capital goods as not to replace
			WRITES(cur3, "capital_good_productivity_initial",0);	//write current capital initial productivity
			WRITELS(cur3, "Capital_Good_Acumulated_Production",0,t);	//write current capital acumulated production as zero
			}
			
			CYCLE_SAFES(cur, cur1, "FIRM_LOANS")
			{
				v[51]=VS(cur1, "firm_loan_fixed_object");
				if (v[51]!=1)
				DELETE(cur1);
			}
			*/
		
	}          
RESULT(v[9])



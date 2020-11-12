//****************************************FIRM RND*************************************//


EQUATION("Firm_Frontier_Productivity")
/*
The new productivity is the maximum among the previous one and the ones possibly obtained imitation and innovation.  
*/
	v[0]=VL("Firm_Frontier_Productivity", 1);       //frontier productivity of the firm in the last period
	v[1]=V("Firm_Imitation_Productivity");          //productivity achievied with imitation in that period
	v[2]=V("Firm_Innovation_Productivity");         //productivity achievied with innovation in that period
	v[3]=max(v[0],max(v[1],v[2]));             		//current frontier productivity is the maximum between the three
RESULT(v[3])


EQUATION("Firm_Quality")
/*
The new quality is the maximum among the previous one and the ones possibly obtained imitation and innovation. 
*/
	v[0]=VL("Firm_Quality", 1);       				//frontier quality of the firm in the last period
	v[1]=V("Firm_Imitation_Quality");          		//quality achievied with imitation in that period
	v[2]=V("Firm_Innovation_Quality");         		//quality achievied with innovation in that period
	v[3]=max(v[0],max(v[1],v[2]));        			//current frontier quality is the maximum between the three
RESULT(v[3])


EQUATION("Firm_Imitation_Productivity")
/*
Process Imitation. The sucess depends on the amount of recources alocated to imitation. Firms search for best productivity of the sector, trying to copy if succeded.
*/
	v[0]=V("Firm_RND_Expenses");                //firm's rnd expenses
	v[1]=V("Firm_Process_RND_Share");     		//firm's share of rnd expenses destinated to process rnd
	v[2]=V("Firm_Innovation_RND_Share");		//firm's share of rnd expenses destinated to innovation
	v[3]=v[0]*v[1]*(1-v[2]);                    //amount of recources for process imitation
	v[4]=1-exp(-v[3]);                   		//probability of success of the imitation depends on amount of recources available
	
	if(RND<v[4])                              	//draws a random number. if it is lower then imitation probability
     	v[5]=VL("Sector_Max_Productivity", 1);  //imitation has succeded and the firm can copy the maximum probability of the sector in the last period
  	else                                      	//if the random number is not lower than imitation probability
     	v[5]=0;                                 //imitation failed and return a productivity zero
RESULT(v[5])


EQUATION("Firm_Innovation_Productivity")
/*
Process Innovation. The sucess depends on the amount ou recources alocated to innovation. Firms search for new productivity  and the result depends on a random distribution with exonegous parameters.
*/
	v[0]=V("Firm_RND_Expenses");                //firm's rnd expenses
	v[1]=V("Firm_Process_RND_Share");     		//firm's share of rnd expenses destinated to process rnd
	v[2]=V("Firm_Innovation_RND_Share");		//firm's share of rnd expenses destinated to innovation
	v[3]=v[0]*v[1]*v[2];                    	//amount of recources for process innovation
	v[4]=1-exp(-v[3]);                   		//probability of success of the innovation depends on amount of recources available
	
	if(RND<v[4])                                //draws a random nuumber. if it is lower then innovation probability 
		{
		v[5]=V("std_dev_innovation");           //innovation standard deviation
		v[6]=V("initial_productivity");			//initial frontier productivity
		v[7]=V("tech_opportunity_productivity");//technological opportunity parameter
		v[10]=log(v[6])+(double)t*(v[7]);   	//the average of the innovation distribution will be the initial frontier productivity plus the opportunity parameter of the science park times the time period
		v[11]=exp(norm(v[10],v[5]));            //the innovation productivity will be a draw from a normal distribution with average depending of the tech regime and std. dev fixed
		}
	else                                        //if the random number is not lower then  the innovation probability
		v[11]=0;                                //innovation failed and the productivity is zero
RESULT(v[11])


EQUATION("Firm_Imitation_Quality")
/*
Product Imitation. The sucess depends on the amount of recources alocated to imitation. Firms search for best best quality of the sector, trying to copy if succeded.
*/
	v[0]=V("Firm_RND_Expenses");                //firm's rnd expenses
	v[1]=V("Firm_Process_RND_Share");     		//firm's share of rnd expenses destinated to process rnd
	v[2]=V("Firm_Innovation_RND_Share");		//firm's share of rnd expenses destinated to innovation
	v[3]=v[0]*(1-v[1])*(1-v[2]);                //amount of recources for product imitation
	v[4]=1-exp(-v[3]);                   		//probability of success of the imitation depends on amount of recources available
	
	if(RND<v[4])                              	//draws a random number. if it is lower then imitation probability
     	v[5]=VL("Sector_Max_Quality", 1);  		//imitation has succeded and the firm can copy the maximum quality of the sector in the last period
  	else                                      	//if the random number is not lower than imitation probability
     	v[5]=0;                                 //imitation failed and return a quality zero
RESULT(v[5])


EQUATION("Firm_Innovation_Quality")
/*
Product Innovation. The sucess depends on the amount ou recources alocated to product innovation. Firms search for new quality and the result depends on a random distribution with exonegous parameters.
*/
	v[0]=V("Firm_RND_Expenses");                //firm's rnd expenses
	v[1]=V("Firm_Process_RND_Share");     		//firm's share of rnd expenses destinated to process rnd
	v[2]=V("Firm_Innovation_RND_Share");		//firm's share of rnd expenses destinated to innovation
	v[3]=v[0]*(1-v[1])*v[2];                   	//amount of recources for process innovation
	v[4]=1-exp(-v[3]);                   		//probability of success of the innovation depends on amount of recources available
	
	if(RND<v[4])                                //draws a random nuumber. if it is lower then innovation probability 
		{
		v[5]=V("std_dev_innovation");           //innovation standard deviation
		v[6]=V("initial_quality");				//initial frontier productivity
		v[7]=V("tech_opportunity_quality");     //technological opportunity parameter
		v[10]=log(v[6])+(double)t*(v[7]);   	//the average of the innovation distribution will be the initial quality plus the opportunity parameter of the science park times the time period
		v[11]=exp(norm(v[10],v[5]));            //the innovation productivity will be a draw from a normal distribution with average depending of the tech regime and std. dev fixed
		}
	else                                        //if the random number is not lower then  the innovation probability
		v[11]=0;                                //innovation failed and quality is zero
RESULT(v[11])


EQUATION("Firm_Process_RND_Share")
/*
This variable defines the allocation of expenses between process and product rnd.
If the firm is gaining market share, it decreases product rnd and increases process rnd. The opposite is valid.
*/

	v[1]=V("Firm_Desired_Market_Share");                //desired market-share 
	v[2]=VL("Firm_Avg_Market_Share",1);                 //firm average market share in the last period   
	v[3]=VL("Firm_Process_RND_Share",1);				//last period share (initial alocation is 0.5
	v[4]=V("process_product_adjustment");				//adjustment parameter							

	if (v[1]>v[2])										//if current avg market share is lower than desired												
		v[5]=v[3]-v[4];									//reduces process rnd allocation
	if (v[1]<v[2])										//if current avg market share is higher than desired
		v[5]=v[3]+v[4];									//increases process rnd allocation
	else
		v[5]=v[3];	
RESULT(v[5])


EQUATION("Firm_Innovation_RND_Share")
/*
This variable defines the allocation of expenses between innovation and imitation.
If the firm is gaining market share, it decreases imitation and increases innovation. The opposite is valid.
*/

	v[1]=V("Firm_Desired_Market_Share");                //desired market-share 
	v[2]=VL("Firm_Avg_Market_Share",1);                 //firm average market share in the last period   
	v[3]=VL("Firm_Innovation_RND_Share",1);				//last period share (initial alocation is 0.5
	v[4]=V("innovation_imitation_adjustment");			//adjustment parameter							

	if (v[1]>v[2])										//if current avg market share is lower than desired												
		v[5]=v[3]-v[4];									//reduces process rnd allocation
	if (v[1]<v[2])										//if current avg market share is higher than desired
		v[5]=v[3]+v[4];									//increases process rnd allocation
	else
		v[5]=v[3];	
RESULT(v[5])

 /*
 * Copyright (c) May 2019, Paul van Haastrecht
*/

#include "sqr.h"
#include "sqr.cpp"
#include "iostream"
#include "sps30.h"      //sensirion's
#include "sps30lib.h"   //paulvha's

typedef struct sps_par
{
    /* option SPS30 parameters */
    uint32_t interval;          // set Auto clean interval
    bool    fanclean;          // perform fan clean now
    bool    dev_info_only;     // only display device info.
    
    /* option program variables */
    uint16_t loop_count;        // number of measurement
    uint16_t loop_delay;        // loop delay in between measurements
    bool timestamp;             // add timestamp in output
    int verbose;                // verbose level
    bool   mass;                // display mass
    bool   num;                 // display numbers
    bool   partsize;            // display partsize
    bool   relation;            // include correlation calc (SDS or Dylos)
    bool   DevStatus;            // display device status 
    bool   OptMode ;            //  perform sleep /wake up during wait-time

    /* to store the SPS30 values */
    struct sps_values v;
        
} sps_par;

/* global constructor */ 
SPS30 MySensor;

/*********************************************************************
*  @brief close hardware and program correctly
**********************************************************************/
void closeout()
{
   /* reset pins in Raspberry Pi */
   MySensor.close();

   exit(EXIT_SUCCESS);
}


/*********************************************************************
* @brief catch signals to close out correctly 
* @param  sig_num : signal that was raised
* 
**********************************************************************/
void signal_handler(int sig_num)
{
    switch(sig_num)
    {
        case SIGINT:
        case SIGKILL:
        case SIGABRT:
        case SIGTERM:
        default:

            closeout();
            break;
    }
}

/*****************************************
 * @brief setup signals 
 *****************************************/
void set_signals()
{
    struct sigaction act;
    
    memset(&act, 0x0,sizeof(act));
    act.sa_handler = &signal_handler;
    sigemptyset(&act.sa_mask);
    
    sigaction(SIGTERM,&act, NULL);
    sigaction(SIGINT,&act, NULL);
    sigaction(SIGABRT,&act, NULL);
    sigaction(SIGSEGV,&act, NULL);
    sigaction(SIGKILL,&act, NULL);
}

/*********************************************
 * @brief generate timestamp
 * 
 * @param buf : returned the timestamp
 *********************************************/  
void get_time_stamp(char * buf)
{
    time_t ltime;
    struct tm *tm ;
    
    ltime = time(NULL);
    tm = localtime(&ltime);
    
    static const char wday_name[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    
    static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    sprintf(buf, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
    wday_name[tm->tm_wday],  mon_name[tm->tm_mon],
    tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
    1900 + tm->tm_year);
}

/************************************************
 * @brief  initialise the variables 
 * @param sps : pointer to SPS30 parameters
 ************************************************/
void init_variables(struct sps_par *sps)
{
    if (MySensor.begin() != ERR_OK) {
        //p_printf(RED,(char *)"Error during setting I2C\n");
        exit(EXIT_FAILURE);
    }
    
    /* option SPS30 parameters */
    sps->interval = 604800;         // default value for autoclean
    sps->fanclean = false;
    sps->dev_info_only = false;
    
    /* option program variables */
    sps->loop_count = 10;           // number of measurement
    sps->loop_delay = 5;            // loop delay in between measurements
    sps->timestamp = false;         // NOT include timestamp in output
    sps->verbose = 0;               // No verbose level
    sps->mass = true;               // display mass by default
    sps->num = true;                // display num by default
    sps->partsize = false;          // display partsize by default
    sps->relation = false;          // display correlation Dylos/SDS
    sps->DevStatus = false;         // display device status 
    sps->OptMode = false;           //  perform sleep /wake up during wait-time

}

/**********************************************************
 * @brief initialise the Raspberry PI and SPS30 / Dylos hardware 
 * @param sps : pointer to SPS30 parameters
 *********************************************************/
void init_hw(struct sps_par *sps)
{
    uint32_t val;
    
    /* progress & debug messages tell driver */
    MySensor.EnableDebugging(sps->verbose);
  
    /* check for auto clean interval update */
    if (MySensor.GetAutoCleanInt(&val) != ERR_OK) {
        //p_printf(RED,(char *)"Could not obtain the Auto Clean interval\n");
        closeout();
    }
    
    if (val != sps->interval) {
        if (MySensor.SetAutoCleanInt(sps->interval) != ERR_OK) {
            //p_printf(RED,(char *)"Could not set the Auto Clean interval\n");
            closeout();
        }
        else {
            //p_printf(GREEN,(char *)"Auto Clean interval has been changed from %d to %d seconds\n",
                val, sps->interval);
        }
    }  


}

//MySensor.GetValues(&sps->v)



void main_loop(struct sps_par *sps)
{
    int     loop_set, reset_retry = RESET_RETRY;
    bool    first=true;

    
    /* if only device info was requested */
    if (sps->dev_info_only) return;

    /* instruct to start reading */
    if (MySensor.start() == false) {
        //p_printf(RED,(char *)  "Can not start measurement:\n");
        return;
    }
    
    //p_printf(GREEN,(char *)  "Starting SPS30 measurement:\n");

    /* check for manual fan clean (can only be done after start) */
    if(sps->fanclean) {
        
        if (MySensor.clean()) 
            //p_printf(BLUE,(char *)"A manual fan clean instruction has been sent\n");
        else
            //p_printf(RED,(char *)"Could not force a manual fan clean\n");
    }
                    
    /*  check for endless loop */
    if (sps->loop_count > 0 ) loop_set = sps->loop_count;
    else loop_set = 1;
    
    /* loop requested */
    while (loop_set > 0)  {
        
        if(MySensor.Check_data_ready()) {
            reset_retry = RESET_RETRY;
            //do_output(sps);
            //MySensor.GetValues(&sps->v);
        }
        else  {
            if (reset_retry-- == 0) {
                
                //p_printf (RED, (char *) "Retry count exceeded. perform softreset\n");
                MySensor.reset();
                reset_retry = RESET_RETRY;
                first = true;
            }
            else  {
                /* Prevent message when previous mode of the SPS30 was 
                 * STOP continuous measurement. It needs 4 seconds 
                 * at least for the first results in that case */
                 
                if (first)  first = false;
                else printf("no data available\n");
            }
        }
        
        // if sleep was requisted during wait
        if (sps->OptMode) MySensor.sleep();
        
        /* delay for seconds */
        sleep(sps->loop_delay);

        // if sleep was requisted during wait
        if (sps->OptMode) {
            MySensor.wakeup();   
        
            // give time to measure new results
            delay(4000);     
        }
        
        /* check for endless loop */
        if (sps->loop_count > 0) loop_set--;
    }
    
    printf("Reached the loopcount of %d.\nclosing down\n", sps->loop_count);
}       



int main()
{
//std::cout << "Hello World!" << sqr(2) << std::endl;

 int opt;
    struct sps_par sps; // parameters
    
    /* set signals */
    set_signals(); 
 
    /* set the initial values */
    init_variables(&sps);

    /* initialise hardware */
    init_hw(&sps);

    /* main loop to read SPS30 results */
    main_loop(&sps);

    closeout();



return 0;
}
    

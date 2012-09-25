#include <termios.h> 
#include "serial.h"
#include <fcntl.h>
#include <errno.h>

state curr_state={
	115200,
	'8',
	'n',
	'1',
	'n',
	'n',
};




void usage(char *str) {
}
/*
void usage(char *str) {
    fprintf(stderr,"%s\n",str);    
    fprintf(stderr, "Usage: nanocom device [options]\n\n"
            "\tdevice\t\tSpecifies which serial port device to use, on most systems this will be /dev/ttyS0 or /dev/ttyS1\n\n" 
            "\t[options] All options are required\n\n"
            "\t-b Bit rate, the bit rate to use, valid options are 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200.\n"
            "\t-p Parity setting, n for none, e for even or o for odd. Must be the same as the remote system.\n"
            "\t-s Stop bits, the number of stop bits (1 or 2)\n"
            "\t-d Data bits, the nuimber of data bits (7 or 8)\n"
            "\t-f Flow control setting, h for hardware (rts/cts), s for software (Xon/xoff) or n for none.\n"
            "\t-e Echo setting\n"
            "\t\tl Local echo (echos everything you type, use if you don't see what you type)\n"
            "\t\tr Remote echo (use if remote system doesn't see what is typed OR enable local echo on remote system)\n"
            "\t\tn No echoing.\n\n");         
  exit(1);
}
*/


void init_stdin(struct termios *sts) {
   /* again, some arbitrary things */
   sts->c_iflag &= ~BRKINT;
   sts->c_iflag |= IGNBRK;
   sts->c_lflag &= ~ISIG;
   sts->c_cc[VMIN] = 1;
   sts->c_cc[VTIME] = 0;
   sts->c_lflag &= ~ICANON;
   
   
   /*enable local echo:*/

   if (curr_state.echo_type=='l')
   {
       sts->c_lflag |= (ECHO | ECHOCTL | ECHONL);
   }
   else
   {
       sts->c_lflag &= ~(ECHO | ECHOCTL | ECHONL);
   }
}









/* setup the terminal according to settings in curr_state*/
void init_comm(int pf) {
    struct termios pts;
    tcgetattr(pf, &pts);
    /*make sure pts is all blank and no residual values set
    safer than modifying current settings*/
    pts.c_lflag=0; /*implies non-canoical mode*/
    pts.c_iflag=0;
    pts.c_oflag=0;
    pts.c_cflag=0;
  
    switch (curr_state.baud_rate)
    {
        case 300:
             pts.c_cflag |= B300;
            break;
        case 1200:
            pts.c_cflag |= B1200;
            break;
        case 2400:
            pts.c_cflag |= B2400;
            break;
        case 4800:
            pts.c_cflag |= B4800;
            break;
        case 9600:
            pts.c_cflag |= B9600;
            break;
        case 19200:
            pts.c_cflag |= B19200;
            break;
        case 38400:
            pts.c_cflag |= B38400;
            break;
        case 57600:
            pts.c_cflag |= B57600;
            break;
        case 115200:
            pts.c_cflag |= B115200;
            break;
        default:
            usage("ERROR: Baud rate must be 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600 or 115200");
            break;
    }           
   
    switch (curr_state.data_bits)
    {
        case '7':
            pts.c_cflag |= CS7;        
            break;
        case '8':
            pts.c_cflag |= CS8;  
            break;
       default:
           usage("ERROR: Only 7 or 8 databits are allowed");
           break;
    }
    
    switch (curr_state.stop_bits) {
    
        case '1':
           /*1 stop bit, default setting no action required*/
           break;
           
        case '2':
           /*2 stop bits*/
           pts.c_cflag |= CSTOPB;            
           break;
           
        default:
           usage("ERROR: Stop bit must contain a value of 1 or 2");
           break;
    }
    
    switch(curr_state.flow_control)
    {
        /*hardware flow control*/
        case 'h':
            pts.c_cflag |= CRTSCTS;      
            break;
    
        /*software flow control*/
        case 's':
            pts.c_iflag |= IXON | IXOFF | IXANY;               
            break;
        
        /*no flow control*/
        case 'n':        
            break;
            
        default:
            usage("ERROR: Flow control must be either h for hardware, s for software or n for none");
            break;
    }    
    
    /*ignore modem lines like hangup*/
    pts.c_cflag |= CLOCAL;
    /*let us read from this device!*/
    pts.c_cflag |= CREAD;
    
    switch (curr_state.parity)  {
        case 'n':
            /*turn off parity*/ 
            break;
            
        case 'e':
            /*turn on parity checking on input*/
            pts.c_iflag |= INPCK;
 
            /*set parity on the cflag*/
            pts.c_cflag |= PARENB;
            
            /*even parity default, no  need to set it*/       
            break;
            
        case 'o':
            /*turn on parity checking on input*/
            pts.c_iflag |= INPCK;
            
            /*set parity on the cflag*/
            pts.c_cflag |= PARENB;
            
            /*set parity to odd*/
            pts.c_cflag |= PARODD;
            break;
            
        default:
            usage("Error: Parity must be set to n (none), e (even) or o (odd)");
            break;
    }
    
    switch(curr_state.echo_type)
    {
        case 'r':
            pts.c_lflag |= (ECHO | ECHOCTL | ECHONL);  
            break;
        case 'n':
            /*handled in init_stdin*/
            break;
        case 'l':
            break;
        default:
            usage("Error: Echo type must be l (local), r (remote) or n (none)");
            break;
    }      
    

    
    
    /*perform newline mapping, useful when dealing with dos/windows systems*/
    pts.c_oflag = OPOST;
  
   /*pay attention to hangup line*/
   pts.c_cflag |= HUPCL;
   
   /*don't wait between receiving characters*/
   pts.c_cc[VMIN] = 1;
   pts.c_cc[VTIME] = 0;
   

    /*translate incoming CR to LF, helps with windows/dos clients*/
    pts.c_iflag |= ICRNL;
    /*translate outgoing CR to CRLF*/
   pts.c_oflag |= ONLCR;

   /*set new attributes*/
   tcsetattr(pf, TCSANOW, &pts);  
}








void serial_init(char* dev)
{

	int pf;

	/* open the device */
	pf = open(dev, O_RDWR);
	if (pf < 0) {
	        usage("Cannot open device");
	}
    	/*setup serial port with new settings*/
	tcflush(pf,TCIOFLUSH);
   	 init_comm(pf);
	tcflush(pf,TCIOFLUSH);
//    	display_state();
		
	close(pf);
	
}

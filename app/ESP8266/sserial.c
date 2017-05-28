#define CR     0x0D  
    
    
    
  /* 
   * Superclass to initialize the UART. 
   */  
  extern void $Super$$__rt_entry(void);  
    
  void $Sub$$__rt_entry(void)    
  {  
    $Super$$__rt_entry();  
  }  
    
  /* 
   * Implementation of putchar (also used by printf function to output data) 
   */  
  int sendchar (int ch)    
  {                 /* Write character to Serial Port    */  
   return 1;  
  }  
    
    
  int getkey (void)    
  {                     /* Read character from Serial Port   */  
   return 1;  
  }  
#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
using namespace std;

class Option
{
   protected:
      double strike_price;
      bool   is_america_style;
      
      double up_out_barrier;
      double down_out_barrier;
      
      double up_in_barrier;
      double down_in_barrier;
       
   public:
  
      Option(double K, bool is_america_style = false, 
      			double up_out =0, double down_out=0): 
      					strike_price(K), is_america_style(is_america_style) ,
      					up_out_barrier (up_out), down_out_barrier(down_out) {}

      virtual double price(double current_stock_price) const
      {
         throw "Invalid call to Option::price";
      }
      
      virtual double price(double current_stock_price, double expectation) const
      {
    	if ( up_out_barrier &&  current_stock_price >= up_out_barrier)	
      	 		return 0;
      	 
      	 if (down_out_barrier && current_stock_price <= down_out_barrier)
      	 		return 0;
      	 
         if (is_america_style) 
         	return price(current_stock_price) > expectation ?
         				price(current_stock_price) : expectation;
         else
         	 return expectation;
      } 

      bool isAmericaStyle() const
      {
         return is_america_style;
      }
};


class BinoModel
{
  private:
      double vol;
      double init_price;
      int step;
      double interest_rate;
      double time;

      double up;
      double down;      
      double sqrt_dt;
      double dt;
      
      double  p_up;
      double  p_down;
      
      void generate_leaf( vector<double>& leaf)
      {
         leaf.clear();
         for (int d = step ; d >=0 ; d--)
              leaf.push_back( pow( down, d) 
                              * pow (up , step-d)
                              * init_price
                             );
      }



  public:
      BinoModel( int s0,
                 double t = 1,
                 double v = 0, 
                 double r = 0, 
                 double n = 100
                 ):
                vol(v),
                init_price(s0),
                step(n),
                interest_rate(r),
                time(t)
      {
         dt= t/n;
         sqrt_dt= sqrt(dt);
         
         up = exp (sqrt_dt*vol);
         down = exp (-sqrt_dt*vol);

         p_up= (exp ( r*t/n ) -down ) / ( up -down);
         p_down = 1- p_up;

      }

      double val(const Option * const option)
      {
         vector<double> leaf;
         generate_leaf(leaf);

         int n = step ;
         
         for (int i=0; i<=n ; i++)
            leaf[i] = option->price(leaf[i]);
          
          
         while (n--)
           for (int u = 0; u <= n  ; u++)
           { 
               double expectation = exp( -1 * dt * interest_rate) * 
                               ( p_up * leaf[u+1] + p_down * leaf[u]);
               double st = pow( down, n-u) * pow (up , u) * init_price;
                              
 			   leaf[u]= option->price(st, expectation);
           }
         return leaf[0];
      }
      
      double val_mc(const Option * const option)
      {	
      	bool active = false;
      	double sum =0;
      	double st= init_price;
		
		srand(5);
      	for (int i=0; i<1000000; i++ )
      	{
      		active = false;
      		st= init_price;
      		for (int j=0; j<step;j++)
      		{
      			double dice = rand() / double(RAND_MAX);
      			
      			double p = dice < p_up ? up : down;
      			st *= p;
      		
      			if (st >= 105){
      			
      				active = true;
      				}
      		}
      		sum += active ? option->price(st):0;
      	}
      	return (sum/1000000)*exp(-time* interest_rate);
      }
    
};


class CallOption : public Option
{
   public:
      
      CallOption(double K, bool is_america_style = false, 
      			double up_out =0, double down_out=0): Option(K,is_america_style,up_out,down_out) {}
      
      double price(double current_stock_price) const
      {
        double val = current_stock_price - strike_price;
        return val < 0 ? 0 : val;    
      }
      
};


class PutOption : public Option
{
   public:
   
    PutOption(double K, bool is_america_style = false, 
      			double up_out =0, double down_out=0): Option(K,is_america_style,up_out,down_out){}
   
     double price(double current_stock_price) const 
     { 
       double val = strike_price - current_stock_price;
       return val < 0 ? 0 : val;
     }
};

class Stock : public Option
{
   public:
     Stock(): Option(0,false) {}
     double price(double current_stock_price) const 
     { 
         return current_stock_price;
     }
};



int main()
{

		BinoModel  bmodel(100, 0.5, 0.15, 0.04, 30) ;
        
        /*Option * e_call_prt = new CallOption(100,false);
        cout<<"Euro call :"<< bmodel.val(e_call_prt)  << endl ;
        
        Option * e_put_prt = new PutOption(100,false);
        cout<<"Euro put :"<< bmodel.val(e_put_prt)  << endl ;
        
        Option * a_call_prt = new CallOption(100,true);
        cout<<"America call :"<< bmodel.val(a_call_prt)  << endl ;
        
        Option * a_put_prt = new PutOption(100,true);
        cout<<"America put :"<< bmodel.val(a_put_prt)  << endl ;
        */
        
        Option * e_call_prt = new CallOption(100,false);
    	Option * e_put_prt = new PutOption(100,false);
    	Option * a_call_prt = new CallOption(100,true);
        Option * a_put_prt = new PutOption(100,true);
        
        
        cout<< "Change So"<<endl;
        for (int i=95;i<=105;i++)
        {
        	BinoModel  model(i, 0.5, 0.15, 0.04, 30) ;

        	cout<<i<<"\t"
        	<<model.val(e_call_prt)<< "\t"
        	<<model.val(e_put_prt)<< "\t"
        	<<model.val(a_call_prt)<< "\t"
        	<<model.val(a_put_prt)<< endl;
        }
       
         
                
        cout<< "Change N"<<endl;
        for (int i=50;i<=1000;i+=50)
        {
        	BinoModel  model(100, 0.5, 0.15, 0.04, i) ;

        	cout<<i<<"\t"
        	<<model.val(e_call_prt)<< "\t"
        	<<model.val(e_put_prt)<< "\t"
        	<<model.val(a_call_prt)<< "\t"
        	<<model.val(a_put_prt)<< endl;
        }
       
        cout<<"Barrier put K =100 and  up out = 105"<<endl;
        Option * e_barrier_put = new PutOption(100, false, 105);
        BinoModel  model(100, 0.5, 0.15, 0.04, 1000) ;
        cout<< model.val(e_barrier_put) <<endl;
        
        
        cout<<"Barrier put K =100 and  up out = 104 America Put"<<endl;
        Option * a_barrier_put = new PutOption(100, true, 104);
        cout<< model.val(a_barrier_put) <<endl;
        
        
        
        cout<<" Barrier put K=100 and up in = 105 E "<<endl;
         cout<< model.val_mc(e_barrier_put) <<endl;
        
	cout<<"end"<<endl;		
}



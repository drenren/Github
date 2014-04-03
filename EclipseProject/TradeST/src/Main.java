import java.sql.SQLException;
import java.util.Calendar;

import market.Quote;
import market.StockDBFeed;
import market.StockDataFeed;
import market.StockTradingDay;


public class Main {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		StockDataFeed feed= new StockDBFeed(	
								"ec2-54-227-237-63.compute-1.amazonaws.com",
								"rdong",
								"atom2415",
								"mysql"
							);
		
		Calendar date= Calendar.getInstance();
		date.set(2013, 4, 1);
		
		StockTradingDay td20130501=new StockTradingDay(date);
		
		feed.subscribe("AA", date);
		Quote quote;
		while ((quote=feed.get_next_quote())!= null )
		{
			td20130501.set_new_quote(quote);

		}
		
		Calendar time= Calendar.getInstance();
		time.set(Calendar.HOUR_OF_DAY, 10);
		time.set(Calendar.MINUTE, 0);
		
		System.out.print(td20130501.compute_EMA(15, time));
		
		
	}

}



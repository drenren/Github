package market;

import java.util.Calendar;


public class StockDataFeed {
	public Quote get_next_quote() throws Exception
	{
		throw new Exception("StockDateFeed::get_next_quote invalid call to the base method");
	}
	
	public void subscribe(String _ticker, Calendar _date) throws Exception
	{
		throw new Exception("StockDateFeed::subscrible invalid call to the base method");
	}
	
}
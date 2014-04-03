package market;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.text.SimpleDateFormat;


public class StockTradingDay {
	
	static final private int TradeMinutes = (int) (6.5 * 60);
	static final private int StartMinute = 9*60+30;

	private Quote quotes[] = new Quote[TradeMinutes] ;
	
	final public Calendar date;
	
	public StockTradingDay(Calendar _date)
	{
		this.date=_date;
	}
	
	private int get_trading_minute(Calendar _time)
	{
		
		return _time.get(Calendar.MINUTE) + _time.get(Calendar.HOUR_OF_DAY)*60 - StartMinute;
		
	}
	
	public void set_new_quote(Quote _quote) throws Exception
	{
		
		int idx = get_trading_minute(_quote.date);
		quotes[idx] = _quote;	
		
	}
	
	
	public double compute_EMA(int _period, Calendar _time) throws Exception
	{
		double sum=0;
		double pow_one_minus_a = 1;
		
		double a = (double)2/(_period +1);
		
		int idx = get_trading_minute(_time);
		for (int i=0; i<_period; i++)
		{
			if ( quotes[idx-i] == null)
				throw new Exception("StockTradingDay::Comput_EMA : quote "+idx+" doesn't exists !");
			sum+= pow_one_minus_a * quotes[idx-i].close;
			pow_one_minus_a*=(1-a);
		}
		return sum*a/0.8647;
	}

}